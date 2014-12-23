#include "Polish.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <random>

#include "CentroDados.h"

using std::stringstream;


double MIN_OPT_VALUE = 0.0;


Polish::Polish( OPT_GUROBI * &lp, VariableMIPUnicoHash const & hashVars, string originalLogFile, int phase )
	: lp_(lp), vHashTatico_(hashVars), maxTime_(0), maxTempoSemMelhora_(9999999999), objAtual_(999999999999.9),
	melhorou_(true), melhora_(0), runtime_(0), nrIterSemMelhora_(0), maxIterSemMelhora_(4),
	fixarVarsTatProf_(false),
	nrPrePasses_(-1), heurFreq_(0.8), module_(Polish::TATICO), fixType_(2), status_(OPTSTAT_MIPOPTIMAL),
	phase_(phase), percUnidLivres_(70), useFreeBlockPerCluster_(true), clusterIdxFreeUnid_(-1),
	tryBranch_(false), okIter_(true),
	k_(3)
{
	originalLogFileName_ = originalLogFile;
	hasOrigFile_ = (strcmp(originalLogFile.c_str(), "") == 0) ? false: true;
}

Polish::Polish( OPT_GUROBI * &lp, VariableOpHash const & hashVars, string originalLogFile, int phase )
	: lp_(lp), vHashOp_(hashVars), maxTime_(0), maxTempoSemMelhora_(9999999999), objAtual_(999999999999.9),
	melhorou_(true), melhora_(0), runtime_(0), nrIterSemMelhora_(0), maxIterSemMelhora_(4),
	fixarVarsTatProf_(false),
	nrPrePasses_(-1), heurFreq_(0.8), module_(Polish::OPERACIONAL), fixType_(2), status_(OPTSTAT_MIPOPTIMAL),
	phase_(phase), percUnidLivres_(70), useFreeBlockPerCluster_(true), clusterIdxFreeUnid_(-1),
	tryBranch_(false), okIter_(true),
	k_(3)
{
	originalLogFileName_ = originalLogFile;
	hasOrigFile_ = (strcmp(originalLogFile.c_str(), "") == 0) ? false: true;
}

Polish::~Polish()
{
	if (idxSol_)
		delete [] idxSol_;
	if (ubVars_)
		delete [] ubVars_;
	if (lbVars_)   
		delete [] lbVars_;
	if (idxs_)
		delete [] idxs_;
	if (vals_)
		delete [] vals_;
	if (bds_)
		delete [] bds_;
}

void Polish::init()
{
	if (!lp_) return;
	
    srand(time(NULL));
         
    initLogFile();

	lp_->setCallbackFunc( NULL, NULL );

	// Timers
    tempoPol_.reset();
    tempoPol_.start();
    tempoSemMelhora_.reset();
    tempoSemMelhora_.start();
	
	// Fix type
	if (module_==Polish::TATICO)
		fixType_ = 1;	
	if (module_==Polish::OPERACIONAL)
		fixType_ = 2;

	// Constants
	tempoIni_ = 70;

	// Init values
	timeIter_ = tempoIni_;
	perc_ = 0;
    objAtual_ = 999999999999.9;

	idxs_ = new int[lp_->getNumCols()*2];
	vals_ = new double[lp_->getNumCols()*2];
	bds_ = new BOUNDTYPE[lp_->getNumCols()*2];
		
    // ORIGINAL VARIABLES BOUNDS
    ubVars_ = new double[ lp_->getNumCols() ];
    lbVars_ = new double[ lp_->getNumCols() ];
    lp_->getUB(0,lp_->getNumCols()-1,ubVars_);
    lp_->getLB(0,lp_->getNumCols()-1,lbVars_);
	
	// VARIABLES INDICES
    idxSol_ = new int[ lp_->getNumCols() ];
    for ( int i = 0; i < lp_->getNumCols(); i++ )
    {
      idxSol_[ i ] = i;
    }

	mapVariables();
	loadUnidades();
	setAllFreeUnidade();
	clusterUnidadesByProfs();
}

void Polish::mapVariables()
{
	if (module_==Polish::TATICO)
		mapVariablesTat();
}

void Polish::mapVariablesTat()
{
	if (module_!=Polish::TATICO)
		return;

    for ( auto vit = vHashTatico_.begin(); vit != vHashTatico_.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
        {
			vHashTatX_[vit->first] = vit->second;
		}
        if ( vit->first.getType() == VariableMIPUnico::V_PROF_AULA )
        {
			vHashTatK_[vit->first] = vit->second;
		}
        if ( vit->first.getType() == VariableMIPUnico::V_PROF_TURMA )
        {
			vHashTatY_[vit->first] = vit->second;
		}
        if ( vit->first.getType() == VariableMIPUnico::V_ABERTURA )
        {
			vHashTatZ_[vit->first] = vit->second;
		}
	}
}

void Polish::loadUnidades()
{
    for ( auto vit = vHashTatX_.begin(); vit != vHashTatX_.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
        {
			if(vit->first.getUnidade())
				unidades_.insert(vit->first.getUnidade());
		}
	}
}

// -------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Cluster unidades

void Polish::clusterUnidadesByProfs()
{		
	// -------------------
	map<Professor*, set<Unidade*>> profUnidcluster;
	map<Unidade*, set<Professor*>> unidProfs;
	mapProfUnidFromVariables(profUnidcluster,unidProfs);
	
	// -------------------
	// Calcula o nr de professores em comum para cada par de unidades
	map<Unidade*, map<Unidade*, int>> parUnidNrProfComum;
	calculaNrProfComumParUnid(profUnidcluster, parUnidNrProfComum);
	
	// -------------------
	// Cluster unidades que compartilham um nr m�nimo de professores com alguma outra
	set<Unidade*> unidsAddedToSomeCluster;
	calculaClustersProfsComuns(unidsAddedToSomeCluster, unidProfs, parUnidNrProfComum);
	
	// -------------------
	// Garante que todas as unidades est�o em algum cluster.
	// Insere unidades que isoladas (n�o compartilham professores com nenhuma outra).
	includeSingleClusters(unidsAddedToSomeCluster);
}

void Polish::mapProfUnidFromVariables(map<Professor*, set<Unidade*>> &profUnidcluster, 
									map<Unidade*, set<Professor*>> &unidProfs)
{
    for ( auto vit = vHashTatK_.begin(); vit != vHashTatK_.end(); vit++ )
    {
		if (vit->first.getProfessor()->eVirtual()) continue;

		profUnidcluster[vit->first.getProfessor()].insert(vit->first.getUnidade());
		unidProfs[vit->first.getUnidade()].insert(vit->first.getProfessor());
	}
}

void Polish::calculaNrProfComumParUnid(map<Professor*, set<Unidade*>> const &profUnidcluster, 
									map<Unidade*, map<Unidade*, int>> &parUnidNrProfComum)
{
	// -------------------------------------------
	// Calcula o nr de professores em comum para cada par de unidades
	for ( auto pit = profUnidcluster.cbegin(); pit != profUnidcluster.cend(); pit++ )
    {
		if (pit->first->eVirtual()) continue;
		
		for ( auto uit1 = pit->second.cbegin(); uit1 != pit->second.cend(); uit1++ )
		{
			auto finder1 = parUnidNrProfComum.find(*uit1);
			if ( finder1 == parUnidNrProfComum.end())
			{
				map<Unidade*, int> empty;
				finder1 = parUnidNrProfComum.insert( std::pair<Unidade*, map<Unidade*, int>>(*uit1, empty) ).first;
			}

			for ( auto uit2 = pit->second.cbegin(); uit2 != pit->second.cend(); uit2++ )
			{				
				if (uit1==uit2) continue;

				auto finder2 = finder1->second.find(*uit2);
				if ( finder2 == finder1->second.end())
				{
					finder2 = finder1->second.insert( std::pair<Unidade*,int>(*uit2, 0) ).first;
				}
				
				finder2->second++;
			}
		}
	}
}

void Polish::calculaClustersProfsComuns(set<Unidade*> &unidsAddedToSomeCluster, 
									map<Unidade*, set<Professor*>> const &unidProfs,
									map<Unidade*, map<Unidade*, int>> const &parUnidNrProfComum)
{
	for ( auto uit1 = parUnidNrProfComum.cbegin(); uit1 != parUnidNrProfComum.cend(); uit1++ )
    {
		Unidade* const u1 = uit1->first;

		int nrProfsU1 = 0;
		auto unidProfFinder = unidProfs.find(u1);
		if (unidProfFinder!=unidProfs.end()) 
			nrProfsU1 = unidProfFinder->second.size();

		set<Unidade*> cluster;
		cluster.insert(u1);
		unidsAddedToSomeCluster.insert(u1);

		for ( auto uit2 = uit1->second.cbegin(); uit2 != uit1->second.cend(); uit2++ )
		{
			Unidade* const u2 = uit2->first;
			int nrProfsComuns = uit2->second;

			if (4*nrProfsComuns > nrProfsU1)
			{
				cluster.insert(u2);
				unidsAddedToSomeCluster.insert(u2);
			}
		}

		addCluster(cluster);
	}
}

void Polish::includeSingleClusters(set<Unidade*> const &unidsAddedToSomeCluster)
{
	for ( auto uit1 = unidades_.cbegin(); uit1 != unidades_.cend(); uit1++ )
    {
		if (unidsAddedToSomeCluster.find(*uit1) == unidsAddedToSomeCluster.cend())
		{
			set<Unidade*> cluster;
			cluster.insert(*uit1);
			addCluster(cluster);
		}
	}

	stringstream str;
	str << "\n\nClusters de unidades: " << unidClustersByProfs_.size();
	printLog(str.str());	
	for ( auto itcluster = unidClustersByProfs_.cbegin(); itcluster != unidClustersByProfs_.cend(); itcluster++ )
    {
		stringstream ss;
		ss << "\nCluster " << itcluster->first << ": ";
		for ( auto uit = itcluster->second.cbegin(); uit != itcluster->second.cend(); uit++ )
			ss << (*uit)->getId() << " ";
		printLog(ss.str());
	}
}

void Polish::addCluster(set<Unidade*> const & cluster)
{
	int idx = unidClustersByProfs_.size();
	unidClustersByProfs_.insert(pair<int,set<Unidade*>> (idx,cluster));
}

// -------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Main polish

bool Polish::polish(double* xS, double maxTime, int percIni, double maxTempoSemMelhora)
{
	bool polishing=true;
    
	init();	      

	xSol_ = xS;

    perc_ = percIni;
	maxTime_ = maxTime;
	maxTempoSemMelhora_ = maxTempoSemMelhora;
	
	if( !needsPolish() )
	{
		okIter_ = false;
		polishing = false;
	}

	std::cout << "\nIniciando iteracoes de polish...";
	
	setParams(timeIter_);

    while (okIter_)
    {     
		fixVars();

		logIter( perc_, timeIter_ );

		if(tryBranch_)
			mainLocalBranching();

		optimize();

		double objN, gap;
		getSolution(objN, gap);
	  
		setMelhora(objN);

		updatePercAndTimeIter(objN, gap);

		checkTimeWithoutImprov(objN);
	  
		updateObj(objN);
	  
		checkTimeLimit();
	  
		unfixBounds();
    }		
	
	//if (phase_ == Polish::PH_MIN_PV)
	//	mainLocalBranching();
	
	closeLogFile();
	restoreOriginalLogFile();
	
    return polishing;
}

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Fix variables

void Polish::fixVars()
{
	if (module_ == Polish::TATICO)
	    fixVarsTatico();
	if (module_ == Polish::OPERACIONAL)
	    fixVarsOp();
}

void Polish::fixVarsTatico()
{
	fixUnidsTatico();

	if (fixType_==1)
	{
		decideVarsToFixType1();
		fixVarsType1();
		fixVarsProfType1();
	}
	else
	{
		fixVarsType2Tatico();
	}
}

void Polish::fixVarsOp()
{
	fixVarsType2Op();
}

void Polish::decideVarsToFixType1()
{
	if ( fixType_ != 1 ) return;

	paraFixarUm_.clear();
	paraFixarZero_.clear();

	if (perc_<= 0) return;

    // Seleciona turmas e disciplinas para fixar    
    int nBds = 0;
	auto vit = vHashTatZ_.begin();
    while ( vit != vHashTatZ_.end() )
    {
		if ( vit->first.getType() == VariableMIPUnico::V_ABERTURA )
		{
			if ( rand() % 100 >= perc_  )
			{
				vit++;
				continue;
			}

			if (xSol_[vit->second] > 0.1 )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = (int)(xSol_[vit->second]+0.5);
				bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
				std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
				paraFixarUm_.insert(auxPair);
			}
			else
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = 0.0;
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
				std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
				paraFixarZero_.insert(auxPair);
			}
		}

		vit++;
    }
    lp_->chgBds(nBds,idxs_,bds_,vals_);
    lp_->updateLP();
}

void Polish::fixVarsProfType1()
{
	if (fixType_ != 1) return;
	if (!fixarVarsTatProf_) return;

    int nBds = 0;
	auto vit = vHashTatY_.begin();
    while ( vit != vHashTatY_.end() )
    {
		if ( vit->first.getType() == VariableMIPUnico::V_PROF_TURMA )
		{
			if ( rand() % 100 >= perc_ )
			{
				vit++;
				continue;
			}

			// Fixa a n�o-utiliza��o de professor virtual
			if (xSol_[vit->second] < 0.1 && vit->first.getProfessor()->eVirtual())
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = 0.0;
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
			// Fixa a utiliza��o de professor real
			//else if (xSol_[vit->second] > 0.1 && !vit->first.getProfessor()->eVirtual())
			//{
			//	idxs_[nBds] = vit->second;
			//	vals_[nBds] = 1.0;
			//	bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
			//	nBds++;
			//}
		}

		vit++;
    }
    lp_->chgBds(nBds,idxs_,bds_,vals_);
    lp_->updateLP();
}

void Polish::fixVarsType1()
{
	if ( fixType_ != 1 ) return;

	if (paraFixarUm_.size() == 0 && paraFixarZero_.size() == 0)
		return;

    int nBds = 0;
    for ( auto vit = vHashTatX_.begin(); vit != vHashTatX_.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
        {
			std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
			if ( paraFixarUm_.find(auxPair) != paraFixarUm_.end() )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = (int)(xSol_[vit->second]+0.5);
				bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
			}
			else if ( paraFixarZero_.find(auxPair) != paraFixarZero_.end() )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = 0.0;
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
        }
    }

    lp_->chgBds(nBds,idxs_,bds_,vals_);
    lp_->updateLP();	
}

void Polish::fixVarsType2Tatico()
{
	if ( fixType_ != 2 ) return;

    int nBds = 0;
    for ( auto vit = vHashTatico_.begin(); vit != vHashTatico_.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS || 
			 vit->first.getType() == VariableMIPUnico::V_PROF_AULA )
        {
			if ( rand() % 100 >= perc_  )
			{
				continue;
			}

			if ( xSol_[vit->second] > 0.1 )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = (int)(xSol_[vit->second]+0.5);
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
				idxs_[nBds] = vit->second;
				vals_[nBds] = (int)(xSol_[vit->second]+0.5);
				bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
			}
			else
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = 0.0;
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
        }
    }

    lp_->chgBds(nBds,idxs_,bds_,vals_);
    lp_->updateLP();
}

void Polish::fixVarsType2Op()
{
    int nBds = 0;
    for ( auto vit = vHashOp_.begin(); vit != vHashOp_.end(); vit++ )
    {
        if ( vit->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
        {
			if ( rand() % 100 >= perc_  )
			{
				continue;
			}

			if ( xSol_[vit->second] > 0.1 )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = (int)(xSol_[vit->second]+0.5);
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
				idxs_[nBds] = vit->second;
				vals_[nBds] = (int)(xSol_[vit->second]+0.5);
				bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
			}
			else
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = 0.0;
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
        }
    }

    lp_->chgBds(nBds,idxs_,bds_,vals_);
    lp_->updateLP();
}

void Polish::fixUnidsTatico()
{
	if (unidades_.size() > 1)
	{
		if(!tryBranch_)
			fixVarsDifUnidade();
	}
}

void Polish::fixVarsDifUnidade()
{
	if (perc_<= 0) return;

	if (allUnidadesAreFree()) return;

    // Seleciona turmas e disciplinas para fixar    
    int nBds = 0;	
    for (auto vit = vHashTatX_.begin(); vit != vHashTatX_.end(); vit++)
    {
		if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
		{
			if ( isFree(vit->first.getUnidade()) )
				continue;

			if (xSol_[vit->second] > 0.1 )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = (int)(xSol_[vit->second]+0.5);
				bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
			}
			else
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = 0.0;
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
		}		
    }
    lp_->chgBds(nBds,idxs_,bds_,vals_);
    lp_->updateLP();
}

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Decide free blocks

void Polish::chooseClusterFreeUnidade()
{
	if(clusterIdxToBeChosen_.size()==0)
	{
		for(auto itIdx=unidClustersByProfs_.cbegin(); itIdx!=unidClustersByProfs_.cend(); itIdx++)
			clusterIdxToBeChosen_.insert(itIdx->first);
	}
	
	int at = rand() % clusterIdxToBeChosen_.size();
	auto iter = std::next(clusterIdxToBeChosen_.begin(),at);
	clusterIdxFreeUnid_ = (*iter);
	clusterIdxToBeChosen_.erase(iter);
}

void Polish::chooseClusterAndSetFreeUnidade()
{
	chooseClusterFreeUnidade();

	auto itClusterAt = unidClustersByProfs_.find(clusterIdxFreeUnid_);
	if (itClusterAt == unidClustersByProfs_.end())
		return;

	for (auto itUnid=itClusterAt->second.cbegin(); itUnid!=itClusterAt->second.cend(); itUnid++)
	{
		addFreeUnid(*itUnid);
	}
}

// ---------------------------------------

int Polish::getNrFreeUnidade()
{
	if (percUnidLivres_>100) percUnidLivres_=100;
	int nr = ((double) percUnidLivres_ /100) * unidades_.size();
	return nr;
}

void Polish::chooseRandUnidade()
{
	int n = rand() % unidades_.size();
	Unidade *unid = *std::next(unidades_.begin(), n);
	
	addFreeUnid(unid);
}

void Polish::chooseRandAndSetFreeUnidade()
{
	while (unidadeslivres_.size() != getNrFreeUnidade())
	{
		chooseRandUnidade();		
	}
}

// ---------------------------------------

void Polish::chooseAndSetFreeUnidades()
{
	clearFreeUnidade();
	decideTypeOfUnidToFree();

	if (useFreeBlockPerCluster_)
		chooseClusterAndSetFreeUnidade();
	else
		chooseRandAndSetFreeUnidade();
}

void Polish::decideTypeOfUnidToFree()
{
	useFreeBlockPerCluster_=true;
	if (rand()%10 >= 7)
		useFreeBlockPerCluster_=false;
}

void Polish::setNextRandFreeUnidade(int adjustPercUnid)
{
	static int counter=0;
	counter++;

	adjustPercFreeUnid(adjustPercUnid);

	if (counter>=4)
	{
		counter=0;
		setAllFreeUnidade();
		return;
	}

	chooseAndSetFreeUnidades();
}

Unidade* Polish::getUnidadeAt(int at)
{
	if (at < 0 || at > unidades_.size()-1)
	{
		stringstream ss;
		ss <<"Position " << at << " for getting unidade invalid. Minimum=0 and Maximum="
			<< unidades_.size()-1 << std::endl;
		printLog(ss.str());
		CentroDados::printError("bool Polish::getUnidadeAt()",ss.str());
		return nullptr;
	}
	return *std::next(unidades_.begin(), at);
}

void Polish::adjustPercFreeUnid(int adjustment)
{
	percUnidLivres_ += adjustment;
	
	if (percUnidLivres_>70) percUnidLivres_=70;
	if (percUnidLivres_<20) percUnidLivres_=20;
}

void Polish::addFreeUnid(Unidade* unid)
{
	unidadeslivres_.insert(unid);

	if (allUnidadesAreFree()) tryBranch_=false;
	//else tryBranch_=true;
}

void Polish::clearFreeUnidade()
{
	unidadeslivres_.clear();
	tryBranch_=false;
}

void Polish::setAllFreeUnidade()
{
	for (auto it=unidades_.cbegin(); it!=unidades_.cend(); it++)
		addFreeUnid(*it);
}

bool Polish::isFree(Unidade* const unid)
{
	return (unidadeslivres_.find(unid) != unidadeslivres_.end());
}

bool Polish::allUnidadesAreFree()
{
	bool free = (unidadeslivres_.size() == unidades_.size());

	return free;
}

bool Polish::thereIsFreeUnid()
{
	return (unidadeslivres_.size() != 0);
}

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

void Polish::optimize()
{
    lp_->setTimeLimit( timeIter_ );
    lp_->copyMIPStartSol( lp_->getNumCols(), idxSol_, xSol_ );
    lp_->updateLP();
    status_ = lp_->optimize( METHOD_MIP );
	
	runtime_ = (lp_->getRunTime());
	timeLeft_ = fabs(timeIter_-runtime_);

	checkFeasibility();
}

void Polish::getSolution(double &objN, double &gap)
{
    objN = objAtual_;
	gap = 100;
	if (optimized())
	{
		lp_->getX( xSol_ );
		objN = lp_->getObjVal();
		gap = lp_->getMIPGap() * 100;
	}
}

void Polish::setMelhora( double objN )
{
	// Indicador de melhora_ da solu��o.
	// 0 indica nenhuma melhora_
	// 1 indica 100% de melhora_
	melhorou_ = (fabs(objN-objAtual_) > 0.001);
	melhora_=0.0;
	if (melhorou_)
	{
		melhora_ = fabs(objN-objAtual_);
		if(objAtual_!=0) melhora_ /= objAtual_;
	}
}

void Polish::updatePercAndTimeIter(double objN, double gap)
{			  	  	  
	  if (perc_ <= 0)
	  {
		  okIter_ = false;
		  return;
	  }

	  if (optimal() || gap <= 1.0)		// TINY GAP
	  {
		  updatePercAndTimeIterSmallGap( objN );
	  }
	  else									// BIG GAP
	  {
		  updatePercAndTimeIterBigGap( objN );
	  }
}

void Polish::updatePercAndTimeIterSmallGap( double objN )
{
	resetIterSemMelhora();

	adjustOkIter(objN);

	adjustPercOrUnid();

	adjustTime();
}

void Polish::updatePercAndTimeIterBigGap( double objN )
{
	if (!melhorou_ && perc_>0)
	{
		checkIterSemMelhora();

		chgParams(); 			  
				
		adjustTime();
		
		setNextRandFreeUnidade(-10);

		checkEndDueToIterSemMelhora();
	}
}

void Polish::adjustPercOrUnid()
{
	if (perc_ <= 10)
	{
		if(perc_ > 0 && !melhorou_) decreasePercOrFreeUnid(perc_);	// free the problem
	}
	else
	{		
		int percToSubtract = 0;
		if(optimal() && timeLeft_>0.7*timeIter_)
			percToSubtract = 10;			// decrease the fixed portion if it was easy (fast) to solve
		else if(!melhorou_)
			percToSubtract = 10;			// decrease the fixed portion if no improvement was made
		
		decreasePercOrFreeUnid(percToSubtract);
	}
}

void Polish::decreasePercOrFreeUnid(int percToSubtract)
{
	if(percToSubtract<=0)
		return;

	if (!allUnidadesAreFree())
	{
		setNextRandFreeUnidade(10);
		return;
	}
	
	perc_ -= percToSubtract;
	if (perc_<0) perc_ = 0;
}

void Polish::adjustTime()
{
	if (!melhorou_ && timeLimitReached())
		increaseTime();			// time limit reached => increases time limit
	
	if (melhorou_)
		decreaseTime();			// Adjust time limit in case it is too high
}

void Polish::increaseTime()
{
	int incremTime = 20;			// increases the time limit by a fixed amount
	incremTime += (100-perc_)*0.2;	// increases the time limit the more perc_ is close to 0.
	timeIter_ += incremTime;

	// ToDo: substituir essa fixa��o de tempo m�ximo por um controle de acordo com o
	// percentual de tempo gasto no branch and bound.

	if (perc_ >= 10 && perc_ < 30){
		if(timeIter_ > 350) timeIter_ = 350;
	}
	else if (perc_ < 40){
		if(timeIter_ > 250) timeIter_ = 250;
	}
	else if (perc_ >= 40){
		if(timeIter_ > 180) timeIter_ = 180;
	}
}

void Polish::decreaseTime()
{
	if (perc_==0)
		timeIter_ = getLeftTime();		// next iteration will be the last one
	else
	{
		// Adjust time limit in case it is too high
		double minExcess = max( 0.5*timeIter_, 50 );
		if ( timeLeft_ > minExcess )
			timeIter_ = runtime_+minExcess;
	}
}

void Polish::adjustOkIter(double objN)
{
	// Obj acchieved a known minimal possible value
	if (fabs(objN-MIN_OPT_VALUE) < 10e-6)
		okIter_ = false;		

	// optimal!
	if (perc_ <= 0 && allUnidadesAreFree())
		okIter_ = false;
}

void Polish::resetIterSemMelhora()
{
	nrIterSemMelhora_=0;
}

void Polish::checkIterSemMelhora()
{
	if (!optimal() && !melhorou_)
	{
		nrIterSemMelhora_++;
	}
}

void Polish::checkEndDueToIterSemMelhora()
{
	if (nrIterSemMelhora_ >	maxIterSemMelhora_)
	{
		if (perc_ <= 10)
		{
			heurFreq_ = 0.8;
			perc_ = 0;
			timeIter_ = getLeftTime();

			stringstream ss;
			ss << "\nSetting perc = 0 due to no change of best solution for more than " << maxIterSemMelhora_ 
				<<  " iterations. Lp will be 100% free at next iteration and time will be the total left time.";
			printLog(ss.str());
		}
	}
}

void Polish::chgFixType()
{
	if (module_==Polish::TATICO)
		fixType_ = (fixType_ == 1 ? 2:1);	// alternate fixType
	
	if (module_==Polish::OPERACIONAL)
		fixType_ = 2;
}

void Polish::checkTimeWithoutImprov(double objN)
{
	if ( fabs(objN-objAtual_) < 0.0001 )
	{
		/* no improvement */
		tempoSemMelhora_.stop();
		double tempoAtual = tempoSemMelhora_.getCronoTotalSecs();
		tempoSemMelhora_.start();
		if ( tempoAtual >= maxTempoSemMelhora_ )
		{
			/* if there is too much time without any improvement, then quit */
			okIter_ = false;
			tempoSemMelhora_.stop();
			tempoPol_.stop();
			cout << "Abort by timeWithoutChange. Limit of time without improvement" << tempoAtual << ", BestObj " << objN;
			if (polishFile_)
			{
				stringstream ss;
				ss << "Abort by timeWithoutChange. Limit of time without improvement" 
					<< tempoAtual << ", BestObj " << objN;
				printLog(ss.str());
			}
		}
	}
	else
	{
		tempoSemMelhora_.stop();
		tempoSemMelhora_.reset();
		tempoSemMelhora_.start();		
	}
}

void Polish::updateObj(double objN)
{
	objAtual_ = objN;
}

void Polish::checkTimeLimit()
{
	if ( okIter_ )
    {
		double tempoAtual = getTotalElapsedTime();
		if ( tempoAtual >= maxTime_ )
		{
			okIter_ = false;
			tempoPol_.stop();
			tempoSemMelhora_.stop();
			cout << "\nTempo maximo atingido: " << tempoAtual << "s, maximo:" << maxTime_ << endl;
			if(polishFile_)
			{
				stringstream ss;
				ss << "\nTempo maximo atingido: " << tempoAtual << "s, maximo:" << maxTime_;
				printLog(ss.str());
			}
		}
	}
}

void Polish::unfixBounds()
{
	if (module_ == Polish::TATICO)
		unfixBoundsTatico();
	if (module_ == Polish::OPERACIONAL)
		unfixBoundsOp();
}

void Polish::unfixBoundsTatHash(VariableMIPUnicoHash const & hashVar)
{
      // Volta bounds
      int nBds = 0;
      for ( auto vit = hashVar.begin(); vit != hashVar.end(); vit++ )
      {
		  idxs_[nBds] = vit->second;
		  vals_[nBds] = ubVars_[vit->second];
		  bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
		  nBds++;
		  idxs_[nBds] = vit->second;
		  vals_[nBds] = lbVars_[vit->second];
		  bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
		  nBds++;
      }	  
      lp_->chgBds(nBds,idxs_,bds_,vals_);
      lp_->updateLP();
}

void Polish::unfixBoundsTatico()
{
	// Volta bounds
	unfixBoundsTatHash(vHashTatX_);	  
	unfixBoundsTatHash(vHashTatZ_);	
	if (fixarVarsTatProf_)
	{
		//unfixBoundsTatHash(vHashTatK_);
		unfixBoundsTatHash(vHashTatY_);
	}
}

void Polish::unfixBoundsOp()
{
      // Volta bounds
      int nBds = 0;
      for ( auto vit = vHashOp_.begin(); vit != vHashOp_.end(); vit++ )
      {
		 if ( vit->first.getType() == VariableOp::V_X_PROF_AULA_HOR || 
			  vit->first.getType() == VariableOp::V_FOLGA_DEMANDA )
         {
            idxs_[nBds] = vit->second;
            vals_[nBds] = ubVars_[vit->second];
            bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
            nBds++;
            idxs_[nBds] = vit->second;
            vals_[nBds] = lbVars_[vit->second];
            bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
            nBds++;
         }         
      }	  
      lp_->chgBds(nBds,idxs_,bds_,vals_);
      lp_->updateLP();
}

void Polish::logIter(double perc_, double timeIter_)
{
	cout <<"POLISH COM PERC = " << perc_ << ", TEMPOITER = " << timeIter_ << endl; fflush(0);
	if(polishFile_)
	{
		stringstream ss;
		ss <<"---------------------------------------------------------------------------\n\n";
		ss <<"POLISH COM PERC = " << perc_ << ", TEMPOITER = " << timeIter_;
		ss << "\nheurFreq_ = " << heurFreq_;
		ss << "\nfixType_ = " << fixType_;
		ss << "\ntempo ja corrido = " << getTotalElapsedTime() << "\ttempo max = " << maxTime_;
				
		ss << "\nuseFreeBlockPerCluster_ = " << useFreeBlockPerCluster_;
		ss << "\ntotal de unidades = " << unidades_.size();
		ss << "\nunidades livres (" << unidadeslivres_.size() << "): ";
		for (auto it=unidadeslivres_.begin(); it!=unidadeslivres_.end(); it++)
			ss << " " << (*it)->getId();
		
		ss << endl;
		printLog(ss.str());
	}
}

void Polish::setLpPrePasses()
{
	if (nrPrePasses_ == -1)
		nrPrePasses_ = 5;
	else if (nrPrePasses_ > 2)
		nrPrePasses_--;
	else return;

	bool success = lp_->setPrePasses(nrPrePasses_);

	if (!success){
		stringstream ss;
		ss << "void Polish::setLpPrePasses(): fail!";
		printLog(ss.str());
	}
}

void Polish::chgLpRootRelax()
{
	//lp_->getItCnt();
	//lp_->setSimplexLimitIteration();

	bool success = lp_->setMIPStartAlg(METHOD_BARRIERANDCROSSOVER);
	
	stringstream ss;
	if (success)
		ss << "Polish::chgLpRootRelax(): METHOD_BARRIERANDCROSSOVER";
	else
		ss << "void Polish::chgLpRootRelax(): fail!";
	printLog(ss.str());	
}

void Polish::setParams(double timeIter_)
{
	lp_->setPrePasses(nrPrePasses_);
  	lp_->setNumIntSols(10000000);
    lp_->setTimeLimit( timeIter_ );
    lp_->setMIPRelTol( 0.1 );
    lp_->setMIPEmphasis( 1 );					// 1 = find better solutions
    lp_->setHeurFrequency( heurFreq_ );
	lp_->setCuts(2);
	lp_->setCliqueCuts(2);
	lp_->setMIRCuts(2);
	lp_->setMIPStartAlg(METHOD_BARRIERANDCROSSOVER);
}

void Polish::chgParams()
{
//	if (perc_ <= 30)
//		lp_->setMIPEmphasis(2);					// 2 = prove optimality
	
	//setNewHeurFreq();
	if (nrIterSemMelhora_ >= 2)
		heurFreq_ = 1.0;

    lp_->setHeurFrequency( heurFreq_ );
	lp_->setCuts(2);
}

void Polish::setNewHeurFreq()
{
	std::default_random_engine generator( time(NULL) );

	std::normal_distribution<double> distribution(0.5,0.25);
	
	heurFreq_ = distribution(generator);
	if (heurFreq_ < 0 || heurFreq_ > 1)
	{
		stringstream msg;
		msg << "heurFreq_ = " << heurFreq_ << " out of bounds!";
		CentroDados::printError("double Polish::setNewHeurFreq()", msg.str());
		heurFreq_ = 0.5;
	}
}

bool Polish::optimized()
{
	if (status_ == OPTSTAT_MIPOPTIMAL || status_ == OPTSTAT_LPOPTIMAL || status_ == OPTSTAT_FEASIBLE)
		return true;
	return false;
}

bool Polish::infeasible()
{
	if (status_ == OPTSTAT_INFEASIBLE)
		return true;
	return false;
}

bool Polish::optimal()
{
	if (status_ == OPTSTAT_MIPOPTIMAL)
		return true;
	return false;
}

bool Polish::unoptimized()
{
	if (status_ == OPTSTAT_UNOPTIMIZED)
		return true;
	return false;
}

bool Polish::timeLimitReached()
{	
	if (lp_->getGurobiStat() == GRB_TIME_LIMIT)
		return true;
	return false;
}

void Polish::checkFeasibility()
{
	if (infeasible())
	{
		stringstream ss;
		ss <<"Error! Model is infeasible. Aborting..." << std::endl;
		printLog(ss.str());
		CentroDados::printError("bool Polish::checkFeasibility()",ss.str());
		lp_->writeProbLP("infeasibleModelPolish");
	//	throw ss.str();
	}
}

double Polish::getTotalElapsedTime()
{
	tempoPol_.stop();
	double tempoAtual = tempoPol_.getCronoTotalSecs();
	tempoPol_.start();
	return tempoAtual;
}

double Polish::getLeftTime()
{
	return maxTime_ - getTotalElapsedTime();
}

bool Polish::needsPolish()
{
	// ------------------------------------------------------------
	// Procura rapidamente a solu��o exata, caso j� se esteja perto do �timo
	if(polishFile_)
	{
		stringstream ss;
		ss << "Verificando necessidade de polishing...";
		printLog(ss.str());
	}

	lp_->setNumIntSols(10000000);
	
	optimize();
	
	double objN, gap;
	getSolution(objN, gap);

	if (optimized())
	{
		if ( gap < 2.0 )
		{
			cout << "\n\nPolish desnecessario, gap =" << gap << std::endl;
			if(polishFile_)
			{
				stringstream ss;
				ss <<"Polish desnecessario, gap = " << gap << std::endl;
				printLog(ss.str());
			}
			return false;
		}
	}

	cout << "\n\nPolishing..." << std::endl << std::endl;
	if(polishFile_)
	{
		stringstream ss;
		ss <<"\nPolishing...\n\n";
		ss <<"-----------------------------------------------------------\n";
		printLog(ss.str());
	}

	return true;
}

void Polish::guaranteeSol()
{
	// -------------------------------------------------------------
    // Garante que n�o dar� erro se houver um getX depois desse polish,
    // j� que o lp sobre altera��o nos bounds no final.
    if(polishFile_)
    {
		stringstream ss;
		ss << "---------------------------------------------------------------------------"
			<< "\n---------------------------------------------------------------------------"
			<< "\nGarantindo presenca da solucao atraves de getX...";
		printLog(ss.str());
    }

    lp_->copyMIPStartSol( lp_->getNumCols(), idxSol_, xSol_ );
	lp_->setTimeLimit( 50 );
	status_ = lp_->optimize( METHOD_MIP );
	lp_->getX(xSol_);
}

void Polish::initLogFile()
{
   // set log file
   static int id=0;
   id++;
   stringstream ss;
   ss << originalLogFileName_ << "Polish" << id;
   
   setOptLogFile(polishFile_,ss.str());
}

void Polish::printLog( string msg )
{
	if(polishFile_)
	{
		polishFile_.flush();
		polishFile_.seekp(0,ios::end);
		polishFile_ << msg << endl;
		polishFile_.flush();
	}
	else std::cout << "\nWarning: Polish log file has not been set.";
}

void Polish::setOptLogFile(std::ofstream &logMip, string name, bool clear)
{
   stringstream ss;
   ss << name << ".log";
   
   if (!clear)
	   logMip.open(ss.str(),ios::app);
   else
	   logMip.open(ss.str(),ios::out);

   if(lp_)
	   lp_->setLogFile((char*)ss.str().c_str());
}

void Polish::closeLogFile()
{
	if (polishFile_.is_open())
		polishFile_.close();
}

void Polish::restoreOriginalLogFile()
{
	if (hasOrigFile_)
	{
		std::ofstream out;
		setOptLogFile(out,originalLogFileName_,false);
	}
}



// -------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------
// Local Branching

void Polish::mainLocalBranching()
{	
	k_ = 3;
	timeIterLB_ = 400;
	bestObjLB_ = objAtual_;
	lbRowNr_ = lp_->getNumRows();

	int at=0;
	int maxIter=1;

	setParams(timeIterLB_);

	bool end = false;
	while(!end)
	{
		string str = "\n******************************************************";
		printLog(str);

		bool improved = localBranching();

		if (optimal())
			k_++;
		if (!optimal() && !improved)
			k_--;
		if (!optimal() && improved)
			timeIterLB_ += 50;

		if (fabs(bestObjLB_-MIN_OPT_VALUE) < 10e-6)
			 end=true;

		at++;
		if (at > maxIter) end=true;
	}
	
	string str = "\n******************************************************";
	printLog(str);

	objAtual_ = bestObjLB_;
}

bool Polish::localBranching()
{
	bool oneimprove = false;

//	if (perc_ >= 40) return oneimprove;
	
	string str = "\n==========================";
	printLog(str);
		
	// First local branch
	firstLBConstr();
	
	doLocalBranch();
	
	bool improved = true;
	if (!melhorouLB_)
		improved = false;
	else
		oneimprove = true;

	// Iterates while there is improvement
	while (improved)
	{	
		string str = "\n--------------------------";
		printLog(str);

		updateLBConstr();
		
		doLocalBranch();

		if (!melhorouLB_)
			improved = false;
		else
			oneimprove = true;
	}
	
	stringstream ss2;
	ss2 << "\nNo improvement was made. The End!\n";
	printLog(ss2.str());

	removeLBConstr();

	return oneimprove;
}


void Polish::doLocalBranch()
{
	stringstream ss;
	ss << "\n\nLocalBranching for k = " << k_ << "...\n";
	printLog(ss.str());

	optimizeLB();
		
	double objN, gap;
	getSolution(objN, gap);

	melhorouLB_ = fabs(objN - bestObjLB_) > 0.0001;

	if (melhorouLB_) bestObjLB_ = objN;
}

void Polish::firstLBConstr()
{			
	int numIdx0 = 0;
	int numIdx1 = 0;
	int* idxsVars0 = new int[lp_->getNumCols()];
	int* idxsVars1= new int[lp_->getNumCols()];
	double* coefs0 = new double[lp_->getNumCols()];
	double* coefs1 = new double[lp_->getNumCols()];

	getLocalBranchVariables(idxsVars0, idxsVars1, coefs0, coefs1, numIdx0, numIdx1);
			
	// --------------------------------------------------

	//k = (int) ((numIdx0+numIdx1) * 0.05) + 1;
	
	// --------------------------------------------------
	int rhs = k_ - numIdx1;
	OPT_ROW row (OPT_ROW::ROWSENSE::LESS, rhs, "LocalBranch");
	row.insert(numIdx0,idxsVars0,coefs0);
	row.insert(numIdx1,idxsVars1,coefs1);
	lp_->addRow(row);
    lp_->updateLP();


	static bool wroteLp=false;
	if (!wroteLp)
	{
		wroteLp=true;
		stringstream sstr;
		sstr << "LocalBranch-k" << k_;
		lp_->writeProbLP( sstr.str().c_str());
	}

	delete [] idxsVars0;
	delete [] idxsVars1;
	delete [] coefs0;
	delete [] coefs1;
}

void Polish::updateLBConstr()
{			
	int numIdx0 = 0;
	int numIdx1 = 0;
	int* idxsVars0= new int[lp_->getNumCols()];
	int* idxsVars1= new int[lp_->getNumCols()];
	double* coefs0 = new double[lp_->getNumCols()];
	double* coefs1 = new double[lp_->getNumCols()];

	getLocalBranchVariables(idxsVars0, idxsVars1, coefs0, coefs1, numIdx0, numIdx1);

    for (int at = 0; at != numIdx0; at++)
    {
		lp_->chgCoef(lbRowNr_, idxsVars0[at], coefs0[at]);		
    }

    for (int at = 0; at != numIdx1; at++)
    {
		lp_->chgCoef(lbRowNr_, idxsVars1[at], coefs1[at]);		
    }

	// --------------------------------------------------
	int rhs = k_ - numIdx1;
	
	int* rowNr = new int[1];
	rowNr[0] = lbRowNr_;

	lp_->chgRHS(lbRowNr_,rhs);
	//lp_->chgCoefList(numIdx0,rowNr,idxsVars0,coefs0);
	//lp_->chgCoefList(numIdx1,rowNr,idxsVars1,coefs1);	
	lp_->updateLP();


	delete [] idxsVars0;
	delete [] idxsVars1;
	delete [] coefs0;
	delete [] coefs1;
}

void Polish::getLocalBranchVariables(int* idxsVars0, int* idxsVars1, double* coefs0, double* coefs1, int &numIdx0, int &numIdx1)
{			
	numIdx0 = 0;
	numIdx1 = 0;

    for (auto vit = vHashTatico_.begin(); vit != vHashTatico_.end(); vit++)
    {
		if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
		{
			if ( isFree(vit->first.getUnidade()) )
				continue;

			if (xSol_[vit->second] > 0.1)
			{
				idxsVars1[numIdx1] = (vit->second);
				coefs1[numIdx1] = -1.0;
				numIdx1++;
			}
			else
			{
				idxsVars0[numIdx0] = (vit->second);
				coefs0[numIdx0] = 1.0;
				numIdx0++;
			}
		}
		
    }
}

void Polish::optimizeLB()
{
    lp_->setTimeLimit( timeIterLB_ );
    lp_->copyMIPStartSol( lp_->getNumCols(), idxSol_, xSol_ );
    lp_->updateLP();
    status_ = lp_->optimize( METHOD_MIP );
	
	runtimeLB_ = (lp_->getRunTime());

	checkFeasibility();
}

void Polish::removeLBConstr()
{
	int* rowNr = new int[1];
	rowNr[0] = lbRowNr_;

	lp_->delSetRows(1,rowNr);
	lp_->updateLP();

	delete [] rowNr;
}