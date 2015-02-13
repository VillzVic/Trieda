#include "Polish.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <random>

#include "CentroDados.h"
#include "MIPUnicoParametros.h"
#include "GoalStatus.h"
#include "Utilidade.h"

using std::stringstream;

bool SO_USAR_WORST_CLUSTER=true;


Polish::Polish( OPT_GUROBI * &lp, VariableMIPUnicoHash const & hashVars, string originalLogFile, int phase, double maxFOAddValue )
	: lp_(lp), vHashTatico_(hashVars), minOptValue_(maxFOAddValue), maxTime_(0),
	maxTempoSemMelhora_(9999999999), objAtual_(999999999999.9), gapAtual_(999999999999.9),
	melhorou_(true), melhora_(0), runtime_(0), nrIterSemMelhoraConsec_(0), nrIterSemMelhora_(0), maxIterSemMelhora_(8),
	fixarVarsTatProf_(true), perc_(0), tempoIni_(80),
	nrPrePasses_(-1), heurFreq_(0.8), module_(Polish::TATICO), fixType_(2), status_(OPTSTAT_MIPOPTIMAL),
	phase_(phase), percUnidFixed_(30), useFreeBlockPerCluster_(true), clusterIdxFreeUnid_(-1), idFreeUnid_(-1),
	tryBranch_(false), okIter_(true), fixTypeAnt_(-1), useFixationByUnid_(true),
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
	{
		fixType_ = 1;
		//fixType_ = 4;
		//useFixationByUnid_=false;
	}
		
	// Init values
	timeIter_ = tempoIni_;
    
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
	
	getAllProfessors();

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
		if ( vit->first.getType() == VariableMIPUnico::V_ALUNO_CREDITOS )
        {
			vHashTatV_[vit->first] = vit->second;
		}
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
		if ( vit->first.getType() == VariableMIPUnico::V_FOLGA_GAP_PROF )
        {
			vHashTatFolgaProfGap_[vit->first] = vit->second;
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

int Polish::getSolValue(int col)
{
	return (int) (xSol_[col] + 0.5);
}

// -------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Cluster professors

void Polish::getAllProfessors()
{
	if (fixType_ != 4) return;

    for ( auto vit = vHashTatY_.begin(); vit != vHashTatY_.end(); vit++)
    {
		if ( vit->first.getType() == VariableMIPUnico::V_PROF_TURMA )
		{			
			if (!vit->first.getProfessor()->eVirtual())
			{
				professors_.insert(vit->first.getProfessor());
			}
		}		
    }
}


// -------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Cluster unidades

void Polish::clusterUnidadesByProfs()
{		
	// -------------------
	mapProfUnidFromVariables();
	
	// -------------------
	// Calcula o nr de professores em comum para cada par de unidades
	map<Unidade*, map<Unidade*, int>> parUnidNrProfComum;
	calculaNrProfComumParUnid(parUnidNrProfComum);
	
	// -------------------
	// Cluster unidades que compartilham um nr mínimo de professores com alguma outra
	set<Unidade*> unidsAddedToSomeCluster;
	calculaClustersProfsComuns(unidsAddedToSomeCluster, parUnidNrProfComum);
	
	// -------------------
	// Garante que todas as unidades estão em algum cluster.
	// Insere unidades que isoladas (não compartilham professores com nenhuma outra).
	includeSingleClusters(unidsAddedToSomeCluster);
}

void Polish::mapProfUnidFromVariables()
{
    for ( auto vit = vHashTatK_.begin(); vit != vHashTatK_.end(); vit++ )
    {
		if (vit->first.getProfessor()->eVirtual()) continue;

		profUnidcluster_[vit->first.getProfessor()].insert(vit->first.getUnidade());
		unidProfs_[vit->first.getUnidade()].insert(vit->first.getProfessor());
	}
}

void Polish::calculaNrProfComumParUnid(map<Unidade*, map<Unidade*, int>> &parUnidNrProfComum)
{
	// -------------------------------------------
	// Calcula o nr de professores em comum para cada par de unidades
	for ( auto pit = profUnidcluster_.cbegin(); pit != profUnidcluster_.cend(); pit++ )
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
									map<Unidade*, map<Unidade*, int>> const &parUnidNrProfComum)
{
	for ( auto uit1 = parUnidNrProfComum.cbegin(); uit1 != parUnidNrProfComum.cend(); uit1++ )
    {
		Unidade* const u1 = uit1->first;

		int nrProfsU1 = 0;
		auto unidProfFinder = unidProfs_.find(u1);
		if (unidProfFinder!=unidProfs_.end()) 
			nrProfsU1 = unidProfFinder->second.size();

		set<Unidade*> clusterUnids;
		clusterUnids.insert(u1);
		unidsAddedToSomeCluster.insert(u1);

		for ( auto uit2 = uit1->second.cbegin(); uit2 != uit1->second.cend(); uit2++ )
		{
			Unidade* const u2 = uit2->first;
			int nrProfsComuns = uit2->second;

			if (4*nrProfsComuns > nrProfsU1)
			{
				clusterUnids.insert(u2);
				unidsAddedToSomeCluster.insert(u2);
			}
		}

		addCluster(clusterUnids);
	}

	removeDuplicatedClusters();
}

void Polish::removeDuplicatedClusters()
{
	for (auto itC1 = unidClustersByProfs_.begin(); 
		itC1 != unidClustersByProfs_.end(); itC1++)
	{
		set<int> toRemove;
		for (auto itC2 = std::next(itC1); 
			itC2 != unidClustersByProfs_.cend(); itC2++)
		{
			if (equals(itC1->second, itC2->second))
				toRemove.insert(itC2->first);
		}

		// Remove
		for (auto itRem = toRemove.cbegin(); 
			itRem != toRemove.cend(); itRem++)
		{
			unidClustersByProfs_.erase(*itRem);
		}
	}
}

bool Polish::equals(set<Unidade*> const &c1, set<Unidade*> const &c2)
{
	if (c1.size() != c2.size()) 
		return false;
	for (auto itUnid1 = c1.cbegin(); itUnid1 != c1.cend(); itUnid1++)
	{
		if(c2.find(*itUnid1) == c2.end())
			return false;
	}
	return true;
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

void Polish::getProfsAssocCluster(set<Unidade*> const &cluster, set<Professor*> &clusterProfs)
{
	for (auto itU = cluster.cbegin(); itU != cluster.cend(); itU++)
	{		
		auto unidProfFinder=unidProfs_.find(*itU);
		if (unidProfFinder!=unidProfs_.end()) 
		{				
			clusterProfs.insert(unidProfFinder->second.cbegin(), unidProfFinder->second.cend());
		}
	}
}

void Polish::addCluster(set<Unidade*> const & cluster)
{
	int idx = unidClustersByProfs_.size();
	unidClustersByProfs_.insert(pair<int,set<Unidade*>> (idx,cluster));

	set<Professor*> clusterProfs;
	getProfsAssocCluster(cluster,clusterProfs);
	clusterIdxProfs_.insert(pair<int,set<Professor*>> (idx,clusterProfs));
}

// -------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Main polish

bool Polish::polish(double* xS, double maxTime, int percIni, double maxTempoSemMelhora, GoalStatus* const goal)
{
	bool polishing=true;
    
	init();	      
	
	xSol_ = xS;

    perc_ = percIni;
	maxTime_ = maxTime;
	maxTempoSemMelhora_ = maxTempoSemMelhora;

	const double alpha = 2;
	timeIter_ += (90 - perc_) * alpha;

	if( !needsPolish() )
	{
		okIter_ = false;
		polishing = false;
	}

	std::cout << "\nIniciando iteracoes de polish...";
	
	setParams(timeIter_);

	int i=0;
    while (okIter_)
    {     
		logIter( perc_, timeIter_ );

		fixVars();
		
		if(tryBranch_)
			mainLocalBranching();

		optimize();

		double objN;
		getSolution(objN);
	  
		setMelhora(objN);

		updatePercAndTimeIter(objN);

		checkTimeWithoutImprov(objN);
	  
		updateObj(objN);
	  
		checkTimeLimit();
	  
		unfixBounds();
		
		i++;
    }

	updateGoal(goal);
	
	stringstream ss;
	ss << "\n\nNumber of performed iterations: " << i;
	printLog(ss.str());
		
	lp_->setCallbackFunc( NULL, NULL );

	closeLogFile();
	restoreOriginalLogFile();
	
    return polishing;
}

void Polish::updateGoal(GoalStatus* const goal)
{
	goal->setValue(objAtual_);
	goal->setGap(gapAtual_);

	double tempoAtual = getTotalElapsedTime();
	goal->setRunTime(tempoAtual);
	
	bool opt = (allFree() && optimal());
	goal->setIsOpt(opt);
}

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Fix variables

void Polish::fixVars()
{
	if (module_ == Polish::TATICO)
	    fixVarsTatico();
}

void Polish::fixVarsTatico()
{	
	if (fixType_==1)
	{
		fixUnidsTatico();
		fixVarsType1Tatico();
	}
	else if (fixType_==2)
	{
		fixUnidsTatico();
		fixVarsType2Tatico();
	}
	else if (fixType_==3)
	{
		fixVarsType3Tatico();
	}
	else if (fixType_==4)
	{
		unordered_set<Professor*> fixedProfs;
		decideVarsToFixVarsType4(fixedProfs);
		fixVarsType4Prof(fixedProfs);
	}
}

void Polish::clearVarsToFixType1()
{
	paraFixarUm_.clear();
	paraFixarZero_.clear();
}

bool Polish::ehMarreta(VariableMIPUnico v)
{
	if ( v.getType() == VariableMIPUnico::V_ALUNO_CREDITOS )
	{
		if (Utilidade::stringContem(v.getDisciplina()->getCodigo(), "EDF"))
			return true;
		if (v.getAluno()->ehFormando() || v.getAluno()->possuiEquivForcada())
			return true;
	}
	return false;
}

void Polish::decideVarsToFixMarreta()
{
	if (phase_ != MIPUnicoParametros::MIP_MARRETA) return;
	if (perc_ <= 0) return;

    // Seleciona turmas e disciplinas para fixar    
    int nBds = 0;	
    for (auto vit = vHashTatV_.begin(); vit != vHashTatV_.end(); vit++)
    {
		if ( vit->first.getType() == VariableMIPUnico::V_ALUNO_CREDITOS )
		{			
			if (ehMarreta(vit->first))
			if (rand() % 100 >= perc_)
				continue;

			double value = (int)(xSol_[vit->second]+0.5);

			if (!vit->first.getAluno()->possuiEquivForcada() && value > 0.1)
				continue;

			if (value > 0.1) bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
			else bds_[nBds] = BOUNDTYPE::BOUND_UPPER;

			idxs_[nBds] = vit->second;
			vals_[nBds] = value;
			nBds++;
		}
    }
	if (nBds)
	{
	    lp_->chgBds(nBds,idxs_,bds_,vals_);
		lp_->updateLP();
	}
}

void Polish::decideVarsToFixOther()
{
	if (phase_ == MIPUnicoParametros::MIP_MARRETA) return;
	if (perc_ <= 0) return;

    // Seleciona turmas e disciplinas para fixar    
    int nBds = 0;	
    for (auto vit = vHashTatZ_.begin(); vit != vHashTatZ_.end(); vit++)
    {
		if ( vit->first.getType() == VariableMIPUnico::V_ABERTURA )
		{
			if ( rand() % 100 >= perc_  )
				continue;
			
			int value = (int)(xSol_[vit->second]+0.5);

			if (value == 1 )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = value;
				bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
				std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
				paraFixarUm_.insert(auxPair);
			}
			else if (value == 0 )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = value;
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
				std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
				paraFixarZero_.insert(auxPair);
			}
		}
    }
	if (nBds)
	{
    //lp_->chgBds(nBds,idxs_,bds_,vals_);
    //lp_->updateLP();
	}
}

void Polish::decideVarsToFixByPhase()
{
	if (phase_ == MIPUnicoParametros::MIP_MARRETA)
		decideVarsToFixMarreta();
	else
		decideVarsToFixOther();
}

void Polish::fixVarsType1Tatico()
{
	clearVarsToFixType1();
	decideVarsToFixByPhase();
	fixVarsType1();
	fixVarsProfType1();	
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

			// Fixa a não-utilização de professor virtual
			if (xSol_[vit->second] < 0.1 && vit->first.getProfessor()->eVirtual())
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = 0.0;
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
			// Fixa a utilização de professor real
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
	if (nBds)
	{
		lp_->chgBds(nBds,idxs_,bds_,vals_);
		lp_->updateLP();
	}
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
	if (nBds)
	{
	    lp_->chgBds(nBds,idxs_,bds_,vals_);
		lp_->updateLP();
	}
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
	if (nBds)
	{
	    lp_->chgBds(nBds,idxs_,bds_,vals_);
		lp_->updateLP();
	}
}

void Polish::fixVarsType3()
{
	if ( fixType_ != 3 ) return;
	
    int nBds = 0;
    for ( auto vit = vHashTatX_.begin(); vit != vHashTatX_.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
        {
			if (vit->first.getUnidade()->getId() == idFreeUnid_) continue;

			int value = (int)(xSol_[vit->second]+0.5);

			if (value > 0)
				bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
			else
				bds_[nBds] = BOUNDTYPE::BOUND_UPPER;

			idxs_[nBds] = vit->second;
			vals_[nBds] = value;
			nBds++;
        }
    }
	if (nBds)
	{
	    lp_->chgBds(nBds,idxs_,bds_,vals_);
		lp_->updateLP();
	}
}

void Polish::fixVarsType3Tatico()
{
	if ( fixType_ != 3 ) return;

	 int idx = rand() % unidades_.size();
	 auto uit = unidades_.begin();
	 std::advance(uit, idx);
	 idFreeUnid_ = (*uit)->getId();

	 fixVarsType3();
}

void Polish::decideVarsToFixVarsType4(unordered_set<Professor*> &fixedProfs)
{
	if (fixType_ != 4) return;

    for ( auto pit = professors_.begin(); pit != professors_.end(); pit++)
    {		
		if ( rand() % 100 >= perc_  )
			continue;

		fixedProfs.insert(*pit);
    }
}

void Polish::fixVarsType4Prof(unordered_set<Professor*> const &fixedProfs)
{
	if (fixType_ != 4) return;
	
    int nBds = 0;
    for (auto vit = vHashTatY_.begin(); vit != vHashTatY_.end(); vit++)
    {
		if ( vit->first.getType() != VariableMIPUnico::V_PROF_TURMA ) 
			continue;
		if ( fixedProfs.find(vit->first.getProfessor()) == fixedProfs.end() )
			continue;

		int value = getSolValue(vit->second);
		BOUNDTYPE btype = (value==0? BOUNDTYPE::BOUND_UPPER : BOUNDTYPE::BOUND_LOWER);

		idxs_[nBds] = vit->second;
		vals_[nBds] = value;
		bds_[nBds] = btype;
		nBds++;
    }
	if (nBds)
	{
	    lp_->chgBds(nBds,idxs_,bds_,vals_);
		lp_->updateLP();
	}
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

			if (xSol_[vit->second] > 0.1)
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
	if (nBds)
	{
	    lp_->chgBds(nBds,idxs_,bds_,vals_);
		lp_->updateLP();
	}
}

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Decide free blocks

int Polish::getFOValueProfGap(Professor* p)
{
	int value=0;
    for (auto vit = vHashTatFolgaProfGap_.begin(); vit != vHashTatFolgaProfGap_.end(); vit++)
    {
		if (vit->first.getType() != VariableMIPUnico::V_FOLGA_GAP_PROF)
			continue;	// unexpected!
		if (vit->first.getProfessor() != p)
			continue;
		value += xSol_[vit->second];
	}
	return value;
}

int Polish::getFOValueClusterUnidade_MinPV(int idxCluster)
{
	auto finderIdx = unidClustersByProfs_.find(idxCluster);
	set<Unidade*>* ptrUnids=nullptr;
	if (finderIdx != unidClustersByProfs_.end())
		ptrUnids = & finderIdx->second;
	if (!ptrUnids) return 0;

	int value=0;
	for (auto vit = vHashTatK_.begin(); vit != vHashTatK_.end(); vit++)
    {
		if ( vit->first.getType() != VariableMIPUnico::V_PROF_AULA )
			continue;	// unexpected!
		if ( !vit->first.getProfessor()->eVirtual() )
			continue;

		// se pertence ao cluster, incrementa o nro de creditos virtuais
		if (ptrUnids->find(vit->first.getUnidade()) != ptrUnids->cend())
		if (xSol_[vit->second] > 0.1)
			value += 1; // cada variavel k corresponde a 1 crédito
	}	
	return value;
}

int Polish::getFOValueClusterUnidade_MinGapProf(int idxCluster)
{
	int value=0;
	auto finderIdx = clusterIdxProfs_.find(idxCluster);
	if (finderIdx != clusterIdxProfs_.end())
	{
		for (auto pit=finderIdx->second.cbegin(); pit!=finderIdx->second.cend(); pit++ )
		{
			Professor* p = *pit;
			if (!p->eVirtual()) value += getFOValueProfGap(p);
		}	
	}	
	return value;
}

int Polish::getFOValueClusterUnidade(int idxCluster)
{
	if (phase_== MIPUnicoParametros::MIP_MIN_VIRT)
		return getFOValueClusterUnidade_MinPV(idxCluster);
	if (phase_== MIPUnicoParametros::MIP_MIN_GAP_PROF)
		return getFOValueClusterUnidade_MinGapProf(idxCluster);
	return 0;
}

void Polish::fillClusterIdxToBeChosen()
{		
	// Reinicia o ciclo
	for(auto itIdx=unidClustersByProfs_.cbegin(); itIdx!=unidClustersByProfs_.cend(); itIdx++)
	{
		int valueFO = getFOValueClusterUnidade(itIdx->first);
		clusterIdxToBeChosen_[valueFO].insert(itIdx->first);
	}
}

void Polish::reorderClusterIdxToBeChosen()
{
	if(clusterIdxToBeChosen_.size()==0 || SO_USAR_WORST_CLUSTER)
	{
		fillClusterIdxToBeChosen();
		return;
	}

	std::set<int> idxs;
	for(auto itIdx=clusterIdxToBeChosen_.cbegin(); itIdx!=clusterIdxToBeChosen_.cend(); itIdx++)
		idxs.insert(itIdx->second.cbegin(), itIdx->second.cend());

	clusterIdxToBeChosen_.clear();
	for (auto itIdx=idxs.cbegin(); itIdx!=idxs.cend(); itIdx++)
	{
		int valueFO = getFOValueClusterUnidade(*itIdx);
		clusterIdxToBeChosen_[valueFO].insert(*itIdx);
	}
}

void Polish::chooseRandClusterFreeUnidade()
{
	if (clusterIdxToBeChosen_.size()==0) return;

	// Escolhe aleatoriamente um cluster pra ser livre na lista de opções
	int atValue = rand() % clusterIdxToBeChosen_.size();
	auto iterValue = std::next(clusterIdxToBeChosen_.begin(),atValue);

	int atIdx = rand() % iterValue->second.size();
	auto iterIdx = std::next(iterValue->second.begin(),atIdx);
	clusterIdxFreeUnid_ = (*iterIdx);

	iterValue->second.erase(iterIdx);
	if (iterValue->second.size()==0)
		clusterIdxToBeChosen_.erase(iterValue);
}

void Polish::chooseWorstClusterFreeUnidade()
{
	if (clusterIdxToBeChosen_.size()==0) return;
	
	// Escolhe o cluster com pior FO associada pra ser livre na lista de opções
	auto iterValue = std::prev(clusterIdxToBeChosen_.end());

	int atIdx = rand() % iterValue->second.size();
	auto iterIdx = std::next(iterValue->second.begin(),atIdx);
	clusterIdxFreeUnid_ = (*iterIdx);

	iterValue->second.erase(iterIdx);
	if (iterValue->second.size()==0)
		clusterIdxToBeChosen_.erase(iterValue);
}

void Polish::chooseClusterFreeUnidade()
{
	// Atualizar ordenação de clusterIdxToBeChosen_
	reorderClusterIdxToBeChosen();
	
	if (phase_== MIPUnicoParametros::MIP_MIN_GAP_PROF || SO_USAR_WORST_CLUSTER)
		chooseWorstClusterFreeUnidade();
	else
		chooseRandClusterFreeUnidade();
}

void Polish::chooseClusterAndSetFreeUnidade()
{
	chooseClusterFreeUnidade();

	auto itClusterAt = unidClustersByProfs_.find(clusterIdxFreeUnid_);
	if (itClusterAt == unidClustersByProfs_.end())
		return;	// error!
	for (auto itUnid=itClusterAt->second.cbegin(); itUnid!=itClusterAt->second.cend(); itUnid++)
	{
		addFreeUnid(*itUnid);
	}
}

// ---------------------------------------

int Polish::getPercUnidCurrentFixed()
{
	double n = unidadeslivres_.size();
	double t = unidades_.size();
	double p = 100 * (n/t);
	return (int) p;
}

int Polish::getNrFreeUnidade()
{
	return unidadeslivres_.size();
}

int Polish::getNrUnidToBeFree()
{
	if (percUnidFixed_>100) percUnidFixed_=100;
	if (percUnidFixed_<0) percUnidFixed_=0;
	int nr = ((double) (100-percUnidFixed_) /100) * unidades_.size();
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
	while (unidadeslivres_.size() < getNrUnidToBeFree())
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

	chooseRandAndSetFreeUnidade();
}

void Polish::decideTypeOfUnidToFree()
{
	useFreeBlockPerCluster_=true;
	if (rand()%10 >= 5)
		useFreeBlockPerCluster_=false;
}

void Polish::setNextRandFreeUnidade(int adjustPercUnid)
{
	if (!useFixationByUnid_) return;

	static int counter=0;
	counter++;

	adjustPercFreeUnid(adjustPercUnid);

	if (counter >= 4 || perc_ <= 0)
	{
		counter=0;
		setAllFreeUnidade();
		return;
	}

	if (fixTypeAnt_!=3)
	{
		int chgType = rand() % 10;
		if (chgType>=7) fixType_ = 3;
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
	percUnidFixed_ += adjustment;
	
	if (percUnidFixed_>90) percUnidFixed_=90;
	if (percUnidFixed_<10) percUnidFixed_=10;
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

void Polish::getSolution(double &objN)
{
    objN = objAtual_;
	gapAtual_ = 100;
	if (optimized())
	{
		lp_->getX( xSol_ );
		objN = lp_->getObjVal();
		gapAtual_ = lp_->getMIPGap() * 100;
	}
}

void Polish::setMelhora( double objN )
{
	// Indicador de melhora_ da solução.
	// 0 indica nenhuma melhora_
	// 1 indica 100% de melhora_
	melhorou_ = (fabs(objN-objAtual_) > 0.001);
	melhora_=0.0;
	if (melhorou_)
	{
		melhora_ = fabs(objN-objAtual_);
		if(objAtual_!=0) melhora_ /= objAtual_;
		resetIterSemMelhoraConsec();
	}
}

void Polish::updatePercAndTimeIter(double objN)
{			  	  	  
	  if (allFree())
	  {
		  printLog("All free: The End!");
		  okIter_ = false;
		  return;
	  }
	  
	  int tempFixType = fixTypeAnt_;
	  fixTypeAnt_ = fixType_;
	  if (fixTypeAnt_==3) fixType_ = tempFixType;

	  updateIterSemMelhora();
	  
	  if (optimal() || gapAtual_ <= 1.0)		// TINY GAP
	  {
		  updatePercAndTimeIterSmallGap( objN );
	  }
	  else								// BIG GAP
	  {
		  updatePercAndTimeIterBigGap();
	  }
	  
	  if (allFree())
	  {
		  setAllFreeConfig();
	  }
}

void Polish::updatePercAndTimeIterSmallGap( double objN )
{	
	adjustOkIter(objN);

	adjustPercOrUnid();

	adjustTime();
}

void Polish::updatePercAndTimeIterBigGap()
{
	if (!melhorou_)
	{
		chgParams(); 			  
				
		int acresm = (allUnidadesAreFree() ? 0: 10);
		setNextRandFreeUnidade(acresm);

		checkDecreaseDueToIterSemMelhora();

		adjustTime();		
	}
}

void Polish::adjustPercOrUnid()
{
	if (perc_ <= 10)
	{
		if(perc_ > 0 && !melhorou_) 
			decreasePercOrFreeUnid(perc_);	// free the problem
	}
	else
	{		
		if(optimal() && timeLeft_>0.7*timeIter_)
			decreasePercOrFreeUnid(5);			// decrease the fixed portion if it was easy (fast) to solve
		else if(!melhorou_)
		{
			if (nrIterSemMelhora_ > 1 || perc_ <= 35)
				decreasePercOrFreeUnid(10);			// decrease the fixed portion if no improvement was made		
		}
	}
}

void Polish::decreasePercOrFreeUnid(int percToSubtract)
{
	if(percToSubtract<=0)
		return;

	if(checkDecreaseDueToIterSemMelhora())
		return;

	if (!allUnidadesAreFree())
	{
		setNextRandFreeUnidade(-10);
		return;
	}
	
	decreasePerc(percToSubtract);
}

void Polish::decreasePerc(int percToSubtract)
{
	perc_ -= percToSubtract;
	if (perc_<0) perc_ = 0;
	resetIterSemMelhoraConsec();
	resetIterSemMelhora();
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
	int incremTime = 10;								// increases the time limit by a fixed amount
	incremTime += (100-perc_)*0.2;						// increases the time limit the more perc_ is close to 0.
	incremTime += (100-getPercUnidCurrentFixed())*0.2;	// increases the time limit the more blocks are free.
	timeIter_ += incremTime;

	// ToDo: substituir essa fixação de tempo máximo por um controle de acordo com o
	// percentual de tempo gasto no branch and bound.

	if (allFree())
		timeIter_ = getLeftTime();		// next iteration will be the last one
	else if (perc_ > 0 && perc_ < 30){
		if(timeIter_ > 350) timeIter_ = 350 + (100-getPercUnidCurrentFixed())*0.5;
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
	if (allFree())
		timeIter_ = getLeftTime();		// next iteration will be the last one
	else
	{
		// Adjust time limit in case it is too high
		double minExcess = max( 0.5*timeIter_, 50 );
		if (timeLeft_ > minExcess)
			timeIter_ = runtime_+minExcess;
	}
}

void Polish::adjustOkIter(double objN)
{
	// Obj acchieved a known minimal possible value
	if (globalOptimal(objN))
		okIter_ = false;		

	// final!
	if (allFree())
		okIter_ = false;
}

bool Polish::allFree()
{
	return (perc_ <= 0 && allUnidadesAreFree());
}

void Polish::setAllFreeConfig()
{
	timeIter_ = getLeftTime();
	lp_->setMIPEmphasis(2);

	#if defined SOLVER_GUROBI && defined USAR_CALLBACK
	cbData_.timeLimit = getLastTimeWithoutImprov();
	cbData_.gapMax = 10;
	lp_->setCallbackFunc( &MIPcallback, &cbData_ );
	#endif
}

bool Polish::globalOptimal(double objN)
{
	// Obj acchieved a known minimal possible value
	if (lp_->getObjSen() == OPTSENSE_MINIMIZE)
	if (objN <= minOptValue_ + 10e-8)
	{
		printLog("\nGlobal optimal!!!");
		return true;
	}

	if (lp_->getObjSen() == OPTSENSE_MAXIMIZE)
	if (objN >= minOptValue_ - 10e-8)
	{
		printLog("\nGlobal optimal!!!");
		return true;
	}
	
	return false;
}

void Polish::resetIterSemMelhoraConsec()
{
	nrIterSemMelhoraConsec_=0;
}

void Polish::resetIterSemMelhora()
{
	nrIterSemMelhora_=0;
}

void Polish::updateIterSemMelhora()
{
	if (!melhorou_)
	{
		nrIterSemMelhoraConsec_++;
		nrIterSemMelhora_++;
	}
}

bool Polish::checkDecreaseDueToIterSemMelhora()
{
	if (nrIterSemMelhoraConsec_ >	maxIterSemMelhora_)
	{
		decreasePerc(5);
		stringstream ss;
		ss << "\nDecreasing 5\% in fixed solution due to no change of best solution for more than "
			<< maxIterSemMelhora_ <<  " consecutive iterations.";
		printLog(ss.str());
		return true;
	}
	return false;
}

void Polish::checkEndDueToIterSemMelhora()
{
	if (nrIterSemMelhoraConsec_ >	maxIterSemMelhora_)
	{
		if (perc_ <= 10)
		{
			heurFreq_ = 0.8;			
			timeIter_ = getLeftTime();
			decreasePerc(perc_);

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
}

double Polish::getTimeWithoutImprov()
{
	tempoSemMelhora_.stop();
	double tempoAtual = tempoSemMelhora_.getCronoTotalSecs();
	tempoSemMelhora_.start();

	return tempoAtual;
}

void Polish::resetTimeWithoutImprov()
{
	tempoSemMelhora_.stop();
	tempoSemMelhora_.reset();
	tempoSemMelhora_.start();
}

double Polish::getLastTimeWithoutImprov()
{
	return maxTempoSemMelhora_ - getTimeWithoutImprov();
}

void Polish::checkTimeWithoutImprov(double objN)
{
	if ( fabs(objN-objAtual_) < 0.0001 )
	{
		/* no improvement */

		if (getLastTimeWithoutImprov() <= 0)
		{
			/* if there is too much time without any improvement, then quit */
			okIter_ = false;
			tempoSemMelhora_.stop();
			tempoPol_.stop();
			cout << "Abort by timeWithoutChange. Limit of time without improvement" 
				<< getTimeWithoutImprov() << ", BestObj " << objN;
			if (polishFile_)
			{
				stringstream ss;
				ss << "Abort by timeWithoutChange. Limit of time without improvement" 
					<< getTimeWithoutImprov() << ", BestObj " << objN;
				printLog(ss.str());
			}
		}
	}
	else resetTimeWithoutImprov();
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
	if ( phase_ == MIPUnicoParametros::MIP_MARRETA )
		unfixBoundsTatHash(vHashTatV_);	
	
	unfixBoundsTatHash(vHashTatX_);	  
	unfixBoundsTatHash(vHashTatZ_);	
	if (fixarVarsTatProf_)
	{
		//unfixBoundsTatHash(vHashTatK_);
		unfixBoundsTatHash(vHashTatY_);
	}
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
		ss << "\tpercUnidFixed_ = " << percUnidFixed_;
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

	//bool success = lp_->setPrePasses(nrPrePasses_);

	//if (!success){
	//	stringstream ss;
	//	ss << "void Polish::setLpPrePasses(): fail!";
	//	printLog(ss.str());
	//}
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
	lp_->setIntEps(1e-7);
	//lp_->setRhsEps(1e-4);
  	lp_->setNumIntSols(10000000);
    lp_->setTimeLimit( timeIter_ );
    lp_->setMIPRelTol(1e-4);
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
	if (nrIterSemMelhoraConsec_ >= 2)
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
		ss <<"Error! Model is infeasible!" << std::endl;
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
	// Procura rapidamente a solução exata, caso já se esteja perto do ótimo
	if(polishFile_)
	{
		stringstream ss;
		ss << "Verificando necessidade de polishing...";
		printLog(ss.str());
	}

	lp_->setNumIntSols(10000000);
	
	optimize();
	
	double objN;
	getSolution(objN);

	if (optimized())
	{
		updateObj(objN);
		if ( gapAtual_ < 1.0 )
		{
			cout << "\n\nPolish desnecessario, gap =" << gapAtual_ << std::endl;
			if(polishFile_)
			{
				stringstream ss;
				ss <<"Polish desnecessario, gap = " << gapAtual_ << std::endl;
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
    // Garante que não dará erro se houver um getX depois desse polish,
    // já que o lp sobre alteração nos bounds no final.
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
   
   std::string phaseName = MIPUnicoParametros::getGoalToString(phase_);
   ss << phaseName;

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

		if (globalOptimal(bestObjLB_))
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
		
	double objN;
	getSolution(objN);

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
