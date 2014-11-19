#include "Polish.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <random>

#include "CentroDados.h"

using std::stringstream;


double MIN_OPT_VALUE = 0.0;


Polish::Polish( OPT_GUROBI * &lp, VariableMIPUnicoHash const & hashVars, string originalLogFile )
	: lp_(lp), vHashTatico_(hashVars), maxTime_(0), maxTempoSemMelhora_(9999999999), objAtual_(999999999999.9),
	melhorou_(false), melhora_(0), runtime_(0),
	nrPrePasses_(-1), heurFreq_(0.7), module_(Polish::TATICO)
{
	originalLogFileName_ = originalLogFile;
	hasOrigFile_ = (strcmp(originalLogFile.c_str(), "") == 0) ? false: true;
}

Polish::Polish( OPT_GUROBI * &lp, VariableOpHash const & hashVars, string originalLogFile )
	: lp_(lp), vHashOp_(hashVars), maxTime_(0), maxTempoSemMelhora_(9999999999), objAtual_(999999999999.9),
	melhorou_(false), melhora_(0), runtime_(0),
	nrPrePasses_(-1), heurFreq_(0.7), module_(Polish::OPERACIONAL)
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
	tempoIni_ = 50;

	// Init values
	tempoIter_ = tempoIni_;
	perc_ = 0;
    status_ = 0;
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
}

bool Polish::polish(double* xS, double maxTime, int percIni, double maxTempoSemMelhora)
{
	bool polishing=true;
    
	init();	      

	xSol_ = xS;

    srand(123);
         
    perc_ = percIni;
	maxTime_ = maxTime;
	maxTempoSemMelhora_ = maxTempoSemMelhora;

    bool okIter = true;

    initLogFile();

	if( !needsPolish() )
	{
		okIter = false;
		polishing = false;
	}

	setParams(tempoIter_);

    while (okIter)
    {     
		if (fixType_==1)
		{
			// Seleciona turmas e disciplinas para fixar
			std::set<std::pair<int,Disciplina*> > paraFixarUm;
			std::set<std::pair<int,Disciplina*> > paraFixarZero;

			decideVarsToFix( paraFixarUm, paraFixarZero );
			fixVarsType1( paraFixarUm, paraFixarZero );
		}
		else
		{
			fixVarsType2();
		}

		logIter( perc_, tempoIter_ );

		optimize();

		double objN, gap;
		getSolution(objN, gap);
	  
		setMelhora(objN);

		updatePercAndTimeIter(okIter, objN, gap);

		checkTimeWithoutImprov(okIter, objN);
	  
		updateObj(objN);
	  
		checkTimeLimit(okIter);
	  
		unfixBounds();
    }
         
	// Agora xSol_ � atributo do MIPUnico, ent�o o carregamento da solucao n�o faz getX mais,
	// sendo dispensavel essa garantia de solu��o aqui.
	// guaranteeSol();

	closeLogFile();
	restoreOriginalLogFile();
	
    return polishing;
}


void Polish::decideVarsToFix( std::set<std::pair<int,Disciplina*> > &paraFixarUm, 
							  std::set<std::pair<int,Disciplina*> > &paraFixarZero )
{
    // Seleciona turmas e disciplinas para fixar
    if ( fixType_ == 1 )
    {
        int nBds = 0;
		auto vit = vHashTatico_.begin();
        while ( vit != vHashTatico_.end() )
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
					paraFixarUm.insert(auxPair);
				}
				else
				{
					idxs_[nBds] = vit->second;
					vals_[nBds] = 0.0;
					bds_[nBds] = BOUNDTYPE::BOUND_UPPER;
					nBds++;
					std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
					paraFixarZero.insert(auxPair);
				}
			}

			vit++;
        }
        lp_->chgBds(nBds,idxs_,bds_,vals_);
        lp_->updateLP();
    }
}


void Polish::fixVarsType1( std::set<std::pair<int,Disciplina*> > const &paraFixarUm, 
					  std::set<std::pair<int,Disciplina*> > const &paraFixarZero )
{
    int nBds = 0;
    for ( auto vit = vHashTatico_.begin(); vit != vHashTatico_.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
        {
			std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
			if ( paraFixarUm.find(auxPair) != paraFixarUm.end() )
			{
				idxs_[nBds] = vit->second;
				vals_[nBds] = (int)(xSol_[vit->second]+0.5);
				bds_[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
			}
			else if ( paraFixarZero.find(auxPair) != paraFixarZero.end() )
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

void Polish::fixVarsType2()
{
	if (module_ == Polish::TATICO)
	    fixVarsType2Tatico();
	if (module_ == Polish::OPERACIONAL)
	    fixVarsType2Op();
}

void Polish::fixVarsType2Tatico()
{
    int nBds = 0;
    for ( auto vit = vHashTatico_.begin(); vit != vHashTatico_.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
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

void Polish::optimize()
{
    lp_->setTimeLimit( tempoIter_ );
    lp_->copyMIPStartSol( lp_->getNumCols(), idxSol_, xSol_ );
    lp_->updateLP();
    status_ = lp_->optimize( METHOD_MIP );
	
	runtime_ = (lp_->getRunTime()/CLOCKS_PER_SEC);
	  
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

void Polish::updatePercAndTimeIter( bool &okIter, double objN, double gap )
{			  	  	  
	  if (perc_ <= 0)
	  {
		  okIter = false;
		  return;
	  }

	  if (unoptimized())					// time limit
	  {
		  tempoIter_ += 50;
		  setLpPrePasses();
	  }
	  else if (optimal() || gap <= 1.0)		// TINY GAP
	  {
		  updatePercAndTimeIterSmallGap( okIter, objN );
	  }
	  else									// BIG GAP
	  {
		  updatePercAndTimeIterBigGap( objN );
	  }
}

void Polish::updatePercAndTimeIterSmallGap( bool &okIter, double objN )
{
	if (melhorou_)
	{
		// Adjust time limit in case it is too high
		double minExcess = max( 0.5*tempoIter_, 50 );
		if ( fabs(tempoIter_-runtime_) > minExcess )
			tempoIter_ = runtime_+minExcess;
	}

	if (fabs(objN-MIN_OPT_VALUE) < 10e-6)	// Obj acchieved a known minimal possible value
	{
		okIter = false;						// optimal!
	}

	if (perc_ <= 10)
	{
		if (perc_ <= 0) okIter = false;		// optimal!
		else if(!melhorou_) perc_ = 0;		// free the problem
	}
	else
	{		
		if(!melhorou_) perc_ -= 10;			// decrease the fixed portion
		if (perc_<0) perc_ = 0;

		if (perc_==0) tempoIter_ *= 2;		// next iteration will be the last one
	}
}

void Polish::updatePercAndTimeIterBigGap( double objN )
{
	if (!melhorou_ && perc_>0)
	{
		chgParams(); 			  

		int incremTime = 20;					// increases the time limit by a fixed amount
		incremTime += (100-perc_)*0.2;			// increases the time limit the more perc_ is close to 0.
		if (!optimal()) tempoIter_ += incremTime;
	}
}

void Polish::checkTimeWithoutImprov( bool &okIter, double objN )
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
			okIter = false;
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

void Polish::checkTimeLimit( bool &okIter )
{
	if ( okIter )
    {
		tempoPol_.stop();
		double tempoAtual = tempoPol_.getCronoTotalSecs();
		tempoPol_.start();
		if ( tempoAtual >= maxTime_ )
		{
			okIter = false;
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

void Polish::unfixBoundsTatico()
{
      // Volta bounds
      int nBds = 0;
      for ( auto vit = vHashTatico_.begin(); vit != vHashTatico_.end(); vit++ )
      {
         if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS || 
			  vit->first.getType() == VariableMIPUnico::V_ABERTURA )
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

void Polish::logIter(double perc_, double tempoIter_)
{
	cout <<"POLISH COM PERC = " << perc_ << ", TEMPOITER = " << tempoIter_ << endl; fflush(0);
	if(polishFile_)
	{
		stringstream ss;
		ss <<"---------------------------------------------------------------------------\n\n";
		ss <<"POLISH COM PERC = " << perc_ << ", TEMPOITER = " << tempoIter_;
		ss << "\nheurFreq_ = " << heurFreq_;
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

void Polish::setParams(double tempoIter_)
{
	lp_->setPrePasses(nrPrePasses_);
  	lp_->setNumIntSols(10000000);
    lp_->setTimeLimit( tempoIter_ );
    lp_->setMIPRelTol( 0.1 );
    lp_->setMIPEmphasis( 1 );
    lp_->setHeurFrequency( heurFreq_ );
	lp_->setCuts(1);
}

void Polish::chgParams()
{
	setNewHeurFreq();

    lp_->setHeurFrequency( heurFreq_ );
	lp_->setCuts(2);
}

void Polish::setNewHeurFreq()
{
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.5,0.25);
	
	heurFreq_ = distribution(generator);
	if (heurFreq_ < 0 || heurFreq_ > 1)
	{
		stringstream msg;
		msg << "heurFreq_ = " << heurFreq_ << " out of bounds!";
		CentroDados::printError("double Polish::getNewHeurFreq()", msg.str());
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

void Polish::checkFeasibility()
{
	if (infeasible())
	{
		stringstream ss;
		ss <<"Error! Model is infeasible. Aborting..." << std::endl;
		printLog(ss.str());
		CentroDados::printError("bool Polish::needsPolish()",ss.str());
		throw ss.str();
	}
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

	if (optimized())
	{
	    lp_->getX( xSol_ );	  
		if ( lp_->getMIPGap() * 100 < 2.0 )
		{
			cout << "\n\nPolish desnecessario, gap =" << lp_->getMIPGap() * 100 << std::endl;
			if(polishFile_)
			{
				stringstream ss;
				ss <<"Polish desnecessario, gap = " << lp_->getMIPGap() * 100 << std::endl;
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
	polishFile_.flush();
	polishFile_.seekp(0,ios::end);
	polishFile_ << msg << endl;
	polishFile_.flush();
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