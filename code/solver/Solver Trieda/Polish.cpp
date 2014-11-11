#include "Polish.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#include <sstream>

using std::stringstream;


double MIN_OPT_VALUE = 0.0;


Polish::Polish( OPT_GUROBI * &lp, VariableMIPUnicoHash const & hashVars, string originalLogFile )
	: lp_(lp), vHashTatico(hashVars), maxTime_(0), maxTempoSemMelhora_(9999999999), objAtual(999999999999.9),
	nrPrePasses_(-1)
{
	originalLogFileName = originalLogFile;
	hasOrigFile = (strcmp(originalLogFile.c_str(), "") == 0) ? false: true;
}

Polish::~Polish()
{
	if (idxSol)
		delete [] idxSol;
	if (ubVars)
		delete [] ubVars;
	if (lbVars)   
		delete [] lbVars;
	if (idxs)
		delete [] idxs;
	if (vals)
		delete [] vals;
	if (bds)
		delete [] bds;
}

void Polish::init()
{
	if (!lp_) return;
	
	// Timers
    tempoPol.reset();
    tempoPol.start();
    tempoSemMelhora.reset();
    tempoSemMelhora.start();
	
	// Constants
	tempoIni = 50;
	fixType = 2;
	
	// Init values
	tempoIter = tempoIni;
	perc = 0;
    status = 0;
    objAtual = 999999999999.9;

	idxs = new int[lp_->getNumCols()*2];
	vals = new double[lp_->getNumCols()*2];
	bds = new BOUNDTYPE[lp_->getNumCols()*2];
		
    // ORIGINAL VARIABLES BOUNDS
    ubVars = new double[ lp_->getNumCols() ];
    lbVars = new double[ lp_->getNumCols() ];
    lp_->getUB(0,lp_->getNumCols()-1,ubVars);
    lp_->getLB(0,lp_->getNumCols()-1,lbVars);
	
	// VARIABLES INDICES
    idxSol = new int[ lp_->getNumCols() ];
    for ( int i = 0; i < lp_->getNumCols(); i++ )
    {
      idxSol[ i ] = i;
    }
}

bool Polish::polish(double* xS, double maxTime, int percIni, double maxTempoSemMelhora)
{
	bool polishing=true;
    
	init();	      

	xSol = xS;

    srand(123);
         
    perc = percIni;
	maxTime_ = maxTime;
	maxTempoSemMelhora_ = maxTempoSemMelhora;

    bool okIter = true;

    initLogFile();

	if( !needsPolish() )
	{
		okIter = false;
		polishing = false;
	}

	setParams(tempoIter);

    while (okIter)
    {     
		if (fixType==1)
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

		logIter( perc, tempoIter );

		optimize();

		double objN, gap;
		getSolution(objN, gap);
	  
		updatePercAndTimeIter( okIter, objN, gap );

		checkTimeWithoutImprov(okIter, objN);
	  
		updateObj(objN);
	  
		checkTimeLimit(okIter);
	  
		unfixBounds();
    }
         
	// Agora xSol é atributo do MIPUnico, então o carregamento da solucao não faz getX mais,
	// sendo dispensavel essa garantia de solução aqui.
	// guaranteeSol();
	
	restoreOriginalLogFile();
	
    return polishing;
}


void Polish::decideVarsToFix( std::set<std::pair<int,Disciplina*> > &paraFixarUm, 
							  std::set<std::pair<int,Disciplina*> > &paraFixarZero )
{
    // Seleciona turmas e disciplinas para fixar
    if ( fixType == 1 )
    {
        int nBds = 0;
		auto vit = vHashTatico.begin();
        while ( vit != vHashTatico.end() )
        {
			if ( vit->first.getType() == VariableMIPUnico::V_ABERTURA )
			{
				if ( rand() % 100 >= perc  )
				{
					vit++;
					continue;
				}

				if (xSol[vit->second] > 0.1 )
				{
					idxs[nBds] = vit->second;
					vals[nBds] = (int)(xSol[vit->second]+0.5);
					bds[nBds] = BOUNDTYPE::BOUND_LOWER;
					nBds++;
					std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
					paraFixarUm.insert(auxPair);
				}
				else
				{
					idxs[nBds] = vit->second;
					vals[nBds] = 0.0;
					bds[nBds] = BOUNDTYPE::BOUND_UPPER;
					nBds++;
					std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
					paraFixarZero.insert(auxPair);
				}
			}

			vit++;
        }
        lp_->chgBds(nBds,idxs,bds,vals);
        lp_->updateLP();
    }
}


void Polish::fixVarsType1( std::set<std::pair<int,Disciplina*> > const &paraFixarUm, 
					  std::set<std::pair<int,Disciplina*> > const &paraFixarZero )
{
    int nBds = 0;
    for ( auto vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
        {
			std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
			if ( paraFixarUm.find(auxPair) != paraFixarUm.end() )
			{
				if ( xSol[vit->second] > 0.1 )
				{
					idxs[nBds] = vit->second;
					vals[nBds] = (int)(xSol[vit->second]+0.5);
					bds[nBds] = BOUNDTYPE::BOUND_UPPER;
					nBds++;
					idxs[nBds] = vit->second;
					vals[nBds] = (int)(xSol[vit->second]+0.5);
					bds[nBds] = BOUNDTYPE::BOUND_LOWER;
					nBds++;
				}
				else
				{
					idxs[nBds] = vit->second;
					vals[nBds] = 0.0;
					bds[nBds] = BOUNDTYPE::BOUND_UPPER;
					nBds++;
				}
			}
			else if ( paraFixarZero.find(auxPair) != paraFixarZero.end() )
			{
				idxs[nBds] = vit->second;
				vals[nBds] = 0.0;
				bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
        }
    }

    lp_->chgBds(nBds,idxs,bds,vals);
    lp_->updateLP();	
}

void Polish::fixVarsType2()
{
    int nBds = 0;
    for ( auto vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
    {
        if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
        {
			if ( rand() % 100 >= perc  )
			{
				continue;
			}

			if ( xSol[vit->second] > 0.1 )
			{
				idxs[nBds] = vit->second;
				vals[nBds] = (int)(xSol[vit->second]+0.5);
				bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
				idxs[nBds] = vit->second;
				vals[nBds] = (int)(xSol[vit->second]+0.5);
				bds[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
			}
			else
			{
				idxs[nBds] = vit->second;
				vals[nBds] = 0.0;
				bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
        }
    }

    lp_->chgBds(nBds,idxs,bds,vals);
    lp_->updateLP();
}

void Polish::optimize()
{
    lp_->setTimeLimit( tempoIter );
    lp_->copyMIPStartSol( lp_->getNumCols(), idxSol, xSol );
    lp_->updateLP();
    status = lp_->optimize( METHOD_MIP );
}

void Polish::getSolution(double &objN, double &gap)
{
    objN = objAtual;
	gap = 100;
	if (optimized())
	{
		lp_->getX( xSol );
		objN = lp_->getObjVal();
		gap = lp_->getMIPGap() * 100;
	}
}

void Polish::updatePercAndTimeIter( bool &okIter, double objN, double gap )
{
	  bool optimal = (status==OPTSTAT_MIPOPTIMAL);

	  // Indicador de melhora da solução.
	  // 0 indica nenhuma melhora
	  // 1 indica 100% de melhora
	  bool melhorou = (fabs(objN-objAtual) > 0.001);
	  double melhora=0.0;
	  if (melhorou)
	  {
		  melhora = fabs(objN-objAtual);
		  if(objAtual!=0) melhora /= objAtual;
	  }
	  
	#pragma region SET NEW VALUES FOR PERC AND TEMPOITER
	  if (status == OPTSTAT_UNOPTIMIZED)	// time limit
	  {
		  tempoIter += 50;
		  setLpPrePasses();
	  }
	  else if ( optimal || gap <= 1.0 )		// TINY GAP
	  {
		  if (melhorou)
		  {
			  // Adjust time limit in case it is too high
			  double minExcess = max( 0.5*tempoIter, 50 );
			  double runtime = (lp_->getRunTime()/CLOCKS_PER_SEC);
			  if ( fabs(tempoIter-runtime) > minExcess )
				  tempoIter = runtime+minExcess;
		  }

		  if ( fabs(objN-MIN_OPT_VALUE) < 10e-6)// Obj acchieved a known minimal possible value
		  {
			  okIter = false;
		  }

		  if ( perc<=10 )
		  {
			  if (perc <= 0) okIter = false;	// ótimo!
			  else if(!melhorou) perc = 0;		// problema livre
		  }
		  else
		  {
			  // diminui a parcela fixa
			  //if(melhora>0.7) perc -= 5;
			  //if(melhora<0.01) perc -= 10;
			  if(!melhorou) perc -= 10;
		  }
	  }
	  else									// BIG GAP
	  {
		  if (!melhorou)
		  {
			  int increm = 5;
			  if (perc+increm < 100)
				  perc += increm;

			  chgParams(); 			  

			  int incremTime = 20;					// increases the time limit by a fixed amount
			  incremTime += (100-perc)*0.2;			// increases the time limit the more perc is close to 0.
			  if (!optimal) tempoIter += incremTime;
		  }
	  }
	#pragma endregion
}

void Polish::checkTimeWithoutImprov( bool &okIter, double objN )
{
	if ( fabs(objN-objAtual) < 0.0001 )
	{
		/* no improvement */
		tempoSemMelhora.stop();
		double tempoAtual = tempoSemMelhora.getCronoTotalSecs();
		tempoSemMelhora.start();
		if ( tempoAtual >= maxTempoSemMelhora_ )
		{
			/* if there is too much time without any improvement, then quit */
			okIter = false;
			tempoSemMelhora.stop();
			tempoPol.stop();
			cout << "Abort by timeWithoutChange. Limit of time without improvement" << tempoAtual << ", BestObj " << objN;
			if (polishFile)
			{
				polishFile.flush();
				polishFile.seekp(0, ios::end);
				polishFile << "Abort by timeWithoutChange. Limit of time without improvement" 
				<< tempoAtual << ", BestObj " << objN;
				polishFile.flush();
			}
		}
	}
	else
	{
		tempoSemMelhora.stop();
		tempoSemMelhora.reset();
		tempoSemMelhora.start();		
	}
}

void Polish::updateObj(double objN)
{
	objAtual = objN;
}

void Polish::checkTimeLimit( bool &okIter )
{
	if ( okIter )
    {
		tempoPol.stop();
		double tempoAtual = tempoPol.getCronoTotalSecs();
		tempoPol.start();
		if ( tempoAtual >= maxTime_ )
		{
			okIter = false;
			tempoPol.stop();
			tempoSemMelhora.stop();
			cout << "\nTempo maximo atingido: " << tempoAtual << "s, maximo:" << maxTime_ << endl;
			if(polishFile)
			{
				polishFile.flush();
				polishFile.seekp(0, ios::end);
				polishFile << "\nTempo maximo atingido: " << tempoAtual << "s, maximo:" << maxTime_ << endl;
				polishFile.flush();
			}
		}
	}
}

void Polish::unfixBounds()
{
      // Volta bounds
      int nBds = 0;
      for ( auto vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
      {
         if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS || 
			  vit->first.getType() == VariableMIPUnico::V_ABERTURA )
         {
            idxs[nBds] = vit->second;
            vals[nBds] = ubVars[vit->second];
            bds[nBds] = BOUNDTYPE::BOUND_UPPER;
            nBds++;
            idxs[nBds] = vit->second;
            vals[nBds] = lbVars[vit->second];
            bds[nBds] = BOUNDTYPE::BOUND_LOWER;
            nBds++;
         }         
      }	  
      lp_->chgBds(nBds,idxs,bds,vals);
      lp_->updateLP();
}

void Polish::logIter(double perc, double tempoIter)
{
	cout <<"POLISH COM PERC = " << perc << ", TEMPOITER = " << tempoIter << endl; fflush(0);
	if(polishFile)
	{
		polishFile.flush();
		polishFile.seekp(0, ios::end);
		polishFile <<"---------------------------------------------------------------------------"
				<< std::endl << std::endl;
		polishFile <<"POLISH COM PERC = " << perc << ", TEMPOITER = " << tempoIter << endl;
		polishFile.flush();
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

	if (!success) polishFile << "void Polish::setLpPrePasses(): fail!";
}

void Polish::setParams(double tempoIter)
{
	lp_->setPrePasses(nrPrePasses_);
  	lp_->setNumIntSols(10000000);
    lp_->setTimeLimit( tempoIter );
    lp_->setMIPRelTol( 0.1 );
    lp_->setMIPEmphasis( 1 );
    lp_->setHeurFrequency( 0.7 );
	lp_->setCuts(1);
}

void Polish::chgParams()
{
    lp_->setMIPRelTol( 0.1 );
    lp_->setMIPEmphasis( 2 );
    lp_->setHeurFrequency( 0.5 );
	lp_->setCuts(2);
}

bool Polish::optimized()
{
	if (status == OPTSTAT_MIPOPTIMAL || status == OPTSTAT_LPOPTIMAL || status == OPTSTAT_FEASIBLE)
		return true;
	return false;
}

bool Polish::needsPolish()
{
	// ------------------------------------------------------------
	// Procura rapidamente a solução exata, caso já se esteja perto do ótimo
	if(polishFile)
	{
		polishFile.flush();
		polishFile.seekp(0, ios::end);
		polishFile <<"Verificando necessidade de polishing..." << std::endl;
		polishFile.flush();
	}

	lp_->setNumIntSols(10000000);
	lp_->setTimeLimit( 50 );
    lp_->copyMIPStartSol( lp_->getNumCols(), idxSol, xSol );
    lp_->updateLP();
	  
    status = lp_->optimize( METHOD_MIP );	  
	if (optimized())
	{
	    lp_->getX( xSol );	  
		if ( lp_->getMIPGap() * 100 < 2.0 )
		{
			cout << "\n\nPolish desnecessario, gap =" << lp_->getMIPGap() * 100 << std::endl;
			if(polishFile)
			{
				polishFile.flush();
				polishFile.seekp(0, ios::end);
				polishFile <<"Polish desnecessario, gap = " << lp_->getMIPGap() * 100 << std::endl;
				polishFile.flush();
			}
			return false;
		}
	}

	cout << "\n\nPolishing..." << std::endl << std::endl;
	if(polishFile)
	{
		polishFile.flush();
		polishFile.seekp(0, ios::end);
		polishFile <<"\nPolishing..." << std::endl << std::endl;
		polishFile <<"-----------------------------------------------------------"
			<< std::endl << std::endl;
		polishFile.flush();
	}

	return true;
}

void Polish::guaranteeSol()
{
	// -------------------------------------------------------------
    // Garante que não dará erro se houver um getX depois desse polish,
    // já que o lp sobre alteração nos bounds no final.
    if(polishFile)
    {
		polishFile.flush();
		polishFile.seekp(0, ios::end);
		polishFile << "---------------------------------------------------------------------------"
				<< "\n---------------------------------------------------------------------------"
				<< "\nGarantindo presenca da solucao atraves de getX..." << std::endl;
		polishFile.flush();
    }
    lp_->copyMIPStartSol( lp_->getNumCols(), idxSol, xSol );
	lp_->setTimeLimit( 50 );
	status = lp_->optimize( METHOD_MIP );
	lp_->getX(xSol);
}

void Polish::initLogFile()
{
   // set log file
   static int id=0;
   id++;
   stringstream ss;
   ss << originalLogFileName << "Polish" << id;
   
   setOptLogFile(polishFile,ss.str());
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

void Polish::restoreOriginalLogFile()
{
	if (hasOrigFile)
	{
		std::ofstream out;
		setOptLogFile(out,originalLogFileName,false);
	}
}