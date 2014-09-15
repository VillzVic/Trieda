#include <math.h>
#include "CPUTimerWin.h"

#include "EstimaTurmas.h"
#include "CPUTimerWin.h"


using namespace std;

EstimaTurmas::EstimaTurmas( ProblemData * &aProblemData, bool *endCARREGA_SOLUCAO ) : Solver( aProblemData )
{
	
   CARREGA_SOLUCAO = endCARREGA_SOLUCAO;

   try
   {
#ifdef SOLVER_CPLEX
	   lp = new OPT_CPLEX; 
#endif
#ifdef SOLVER_GUROBI
	   lp = new OPT_GUROBI; 
#endif
   }
   catch(...)
   {
   }

}

EstimaTurmas::~EstimaTurmas()
{
   if ( lp != NULL )
   {
      delete lp;
   }   
}

void EstimaTurmas::getSolution( ProblemSolution * problem_solution ){}

int EstimaTurmas::solve(){return 1;}

void EstimaTurmas::solveEstimaTurmas( int campusId, int prioridade )
{	
	std::cout<<"\nSolving...\n"; fflush(NULL);
	solveModeloEstimaTurmas( campusId, prioridade );

	std::cout<<"\nCarregando solucao estims turmas...\n"; fflush(NULL);
	carregaVariaveis( campusId, prioridade );
	
	std::cout<<"\mCalculando resultados...\n"; fflush(NULL);
	calculaResultados( campusId, prioridade );

	cout<<"\nSetando numero de turmas...\n" <<endl;
	setNumTurmas( campusId, prioridade );

	return; 
}

std::string EstimaTurmas::getEstimaTurmaLpFileName( int campusId, int prioridade, int fase )
{
   std::string solName;
 
   solName += "EstimaTurma";
   solName += problemData->inputIdToString();

   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }

   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }

   if ( fase != 0 )
   {
		stringstream ss;
		ss << fase;
		solName += "_Fase"; 
		solName += ss.str();   		
   }
   
   return solName;
}

std::string EstimaTurmas::getSolBinFileName( int campusId, int prioridade, int fase )
{
	std::string solName;
	solName += "EstimaTurmaBin";
	solName += problemData->inputIdToString();
	 
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }

   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   if ( fase != 0 )
   {
		stringstream ss;
		ss << fase;
		solName += "_Fase"; 
		solName += ss.str();   		
   }

   solName += ".bin";
      
   return solName;
}
std::string EstimaTurmas::getNumTurmasFileName( int campusId, int prioridade, int fase )
{
	std::string solName;
	solName += "EstimaNroTurma";
	solName += problemData->inputIdToString();
	  
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }

   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   if ( fase != 0 )
   {
		stringstream ss;
		ss << fase;
		solName += "_Fase"; 
		solName += ss.str();   		
   }

   solName += ".txt";
      
   return solName;
}

std::string EstimaTurmas::getSolucaoEstimaTurmaFileName( int campusId, int prioridade, int fase )
{
	std::string solName;
	solName += "solucaoEstimaTurma";
	solName += problemData->inputIdToString();

   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   if ( fase != 0 )
   {
		stringstream ss;
		ss << fase;
		solName += "_Fase"; 
		solName += ss.str();   		
   }

   solName += ".txt";
      
   return solName;
}


void EstimaTurmas::writeSolBin( int campusId, int prioridade, int fase, int type, double *xSol )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (ESTIMA_TURMA_BIN):
			strcpy( solName, getSolBinFileName( campusId, prioridade, fase ).c_str() );
			break;
		case (ESTIMA_TURMA_BIN1):
			strcpy( solName, "1" );
			strcpy( solName, getSolBinFileName( campusId, prioridade, fase ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	FILE * fout = fopen( solName, "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em EstimaTurmas::writeSolBin( int campusId, int prioridade, int fase, int type, double *xSol ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{
		int nCols = lp->getNumCols();

		fwrite( &nCols, sizeof( int ), 1, fout );
		for ( int i = 0; i < lp->getNumCols(); i++ )
		{
			double value = (int)( xSol[ i ] + 0.5 );
			fwrite( &( value ), sizeof( double ), 1, fout );
		}

		fclose( fout );
	}
}

int EstimaTurmas::readSolBin( int campusId, int prioridade, int fase, int type, double *xSol )
{
	char solName[1024]="\0";

	switch (type)
	{
		case (ESTIMA_TURMA_BIN):
			strcpy( solName, getSolBinFileName( campusId, prioridade, fase ).c_str() );
			break;
		case (ESTIMA_TURMA_BIN1):
			strcpy( solName, "1" );
			strcpy( solName, getSolBinFileName( campusId, prioridade, fase ).c_str() );
			break;
	}

	// READS THE SOLUTION
		
	cout<<"====================> carregando solucao " <<solName <<endl; fflush(NULL);
	FILE* fin = fopen( solName,"rb");

	if ( fin == NULL )
	{
		std::cout << "<============ Arquivo " << solName << " nao encontrado. Fim do carregamento de solucao.\n\n"; fflush(NULL);
		return (0);
	}

	int nCols = 0;
    int nroColsLP = lp->getNumCols();

	fread(&nCols,sizeof(int),1,fin);
   
	if ( nCols == nroColsLP )
	{
		for (int i =0; i < nCols; i++)
		{
			double auxDbl;
			fread(&auxDbl,sizeof(double),1,fin);
			(xSol)[i] = auxDbl;
		}
	}
	else
	{
		std::cout << "\nErro em readSolBin(int campusAtualId, int prioridade, int r): "
					<< " \nNumero diferente de variaveis: " << nCols << " != " << nroColsLP; fflush(NULL);
		return (0);
	}
	fclose(fin);
	
	return (1);
}

int EstimaTurmas::readSolTxt( int campusId, int prioridade, int fase, int type, double *xSol )
{
   char solName[1024]="\0";

	switch (type)
	{
		case (ESTIMA_TURMA_BIN):
			strcpy( solName, getSolucaoEstimaTurmaFileName( campusId, prioridade, fase ).c_str() );
			break;
		case (ESTIMA_TURMA_BIN1):
			strcpy( solName, "1" );
			strcpy( solName, getSolucaoEstimaTurmaFileName( campusId, prioridade, fase ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	ifstream fin( solName, ios_base::in );
	if ( fin == NULL )
	{
		std::cout << "<============ Arquivo " << solName << " nao encontrado. Fim do carregamento de solucao.\n\n"; fflush(NULL);
		return 0;
	}
	else
	{
      
      for (int i=0; i < lp->getNumCols(); i++)
      {
         xSol[i] = 0.0;
      }

      bool finishVars = false;
      std::map<std::string,double> varVals;

      while ( !finishVars )
      {
         double val = 0.0;
         char buf[2048];
         std::string varName = "";
         fin >> buf;
         varName = buf;
         fin >> buf;

         if ( strcmp(buf,"=") != 0 )
         {
            finishVars = true;
            break;
         }

         fin >> val;

         varVals[varName] = val;
      }

      VariableEstimaTurmasHash::iterator vit = vHashTatico.begin();
      while ( vit != vHashTatico.end() )
      {
         VariableEstimaTurmas v = vit->first;
         int col = vit->second;
         double value = (int)( xSol[ col ] + 0.5 );
         std::string varName = v.toString();

         if ( varVals.find(varName) != varVals.end() )
         {
            xSol[col] = varVals[varName];
         }

         vit++;
      }
      varVals.clear();
      fin.close();
   }

   return 1;
}

int EstimaTurmas::writeGapTxt( int campusId, int prioridade, int fase, int type, double gap )
{
	#ifndef PRINT_LOGS
		return 1;
	#endif

	std::string step;
	switch (type)
	{
		case (ESTIMA_TURMA_BIN):
			step = "Final";		
			break;
		case (ESTIMA_TURMA_BIN1):
			step = "Zera Sabado";
			break;
	}

 	// Imprime Gap
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";

	outGaps.open(gapFilename, ofstream::app);
	if ( !outGaps )
	{
		std::cerr<<"\nErro: Abertura do arquivo " << gapFilename << " falhou em EstimaTurmas::writeGapTxt() em " << step << endl;
		return 0;
	}
	else
	{
		outGaps << "Estima Turmas (" << step << ") - campus "<< campusId << ", prioridade " << prioridade << ", fase "<< fase;
		outGaps << "\nGap = " << gap << "%";
		
		if ( type == ESTIMA_TURMA_BIN )
			outGaps << "\n\n------------------------------------------------------------------------------------------------";
		outGaps << "\n\n";
		
		outGaps.close();
		return 1;
	} 
}

void EstimaTurmas::writeSolTxt( int campusId, int prioridade, int fase, int type, double *xSol )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (ESTIMA_TURMA_BIN):
			strcpy( solName, getSolucaoEstimaTurmaFileName( campusId, prioridade, fase ).c_str() );			
			break;
		case (ESTIMA_TURMA_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolucaoEstimaTurmaFileName( campusId, prioridade, fase ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	ofstream fout( solName, ios_base::out );
	if ( fout == NULL )
	{
		std::cout << "\nErro em EstimaTurmas::writeSolTxt( int campusId, int prioridade, int fase, int type ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{		
		
		VariableEstimaTurmasHash::iterator vit = vHashTatico.begin();
		while ( vit != vHashTatico.end() )
		{
				VariableEstimaTurmas v = vit->first;
				int col = vit->second;
				double value = (int)( xSol[ col ] + 0.5 );
		  
				fout << v.toString() << " = " << value << endl;
				  
				vit++;
		}

		fout.close();
	}
}


void EstimaTurmas::setNumTurmas( int campusAtualId, int prioridade )
{
   // ---------------------------------------------------------------------------------------------------------
	std::map< Disciplina*, std::map< ConjuntoSala*, int, LessPtr<ConjuntoSala> >, LessPtr<Disciplina> > mapDiscSalaNumTurmas;

   ITERA_GGROUP_LESSPTR ( it, solVarsEstimaTurmas, VariableEstimaTurmas )
   {
	   VariableEstimaTurmas v = **it;
	   if ( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
	   {		   
		   // numero de turmas da disciplina por cjtSala
		   std::map< ConjuntoSala*, int, LessPtr<ConjuntoSala> > *mapSalas = &mapDiscSalaNumTurmas[ v.getDisciplina() ];
		   if ( mapSalas->find( v.getSubCjtSala() ) != mapSalas->end() )
		   {
			   mapDiscSalaNumTurmas[ v.getDisciplina() ][ v.getSubCjtSala() ] += (int) (v.getValue() + 0.5);
		   }
		   else
		   {
			   mapDiscSalaNumTurmas[ v.getDisciplina() ][ v.getSubCjtSala() ] = (int) (v.getValue() + 0.5);
		   }
	   }
   }

   // ---------------------------------------------------------------------------------------------------------
   std::map< Disciplina*, double, LessPtr<Disciplina> > mapDiscTCapMediaPorTurma;
   std::map< Disciplina*, double, LessPtr<Disciplina> > mapDiscPCapMediaPorTurma;

   ITERA_GGROUP_LESSPTR ( itDisc, problemData->disciplinas, Disciplina )
   {
	   std::map< Disciplina*, std::map< ConjuntoSala*, int, LessPtr<ConjuntoSala> >, LessPtr<Disciplina> >::iterator 
		   itMapDiscSalas = mapDiscSalaNumTurmas.find( *itDisc );
	   if ( itMapDiscSalas != mapDiscSalaNumTurmas.end() )
	   {
		   Disciplina *disc = itMapDiscSalas->first;
		   int nTurmas = 0;
		   double capMediaAlunos = 0;
		   
		   int demandaTotal = problemData->retornaDemandasDiscNoCampus( disc->getId(), campusAtualId, prioridade ).size();

		   std::map< ConjuntoSala*, int, LessPtr<ConjuntoSala> > mapSalas = itMapDiscSalas->second;

		   std::map< ConjuntoSala*, int, LessPtr<ConjuntoSala> >::iterator itMapCjtSala = mapSalas.begin();
		   for ( ; itMapCjtSala != mapSalas.end(); itMapCjtSala++ )
		   {
			   ConjuntoSala* cjtSala = itMapCjtSala->first;
			   int cjtSalaId = cjtSala->getId();
			   int nTurmasNaSala = itMapCjtSala->second;
		   
			   capMediaAlunos += nTurmasNaSala * cjtSala->getCapacidadeRepr();
			   nTurmas += nTurmasNaSala;
			   disc->setSubCjtSalaNumTurmas( cjtSalaId, nTurmasNaSala );
		   }

		   if ( nTurmas > 0 )
			   capMediaAlunos = capMediaAlunos / nTurmas;

		   if ( disc->getId() < 0 )
		   {
			   mapDiscPCapMediaPorTurma[disc] = capMediaAlunos;
		   }
		   else
		   {
			   mapDiscTCapMediaPorTurma[disc] = capMediaAlunos;
		   }

	//	   if ( demandaTotal > capMediaAlunos/3 ||
	//		    (problemData->parametros->min_alunos_abertura_turmas && 
	//			 demandaTotal > problemData->parametros->min_alunos_abertura_turmas_value) )
	//	   {
			   nTurmas++;
			   nTurmas += (int) (0.2 * nTurmas); // aumenta o numero estimado de turmas
	//	   }

		   disc->setNumTurmas( nTurmas );
	   }
	   else itDisc->setNumTurmas(0);
   }
   
   // ---------------------------------------------------------------------------------------------------------
   if ( problemData->existeSolTaticoInicial() )
   {
		ITERA_GGROUP_LESSPTR ( itDisc, problemData->disciplinas, Disciplina )
		{
			// Se houver solução inicial, garante o mínimo de turmas presente na solução inicial
			int nTurmasDiscSolInic = problemData->getSolTaticoInicial()->getSolNumTurmas(campusAtualId, *itDisc);
			if ( (*itDisc)->getNumTurmas() < nTurmasDiscSolInic )
				(*itDisc)->setNumTurmas( nTurmasDiscSolInic );
		}
   }

   // ---------------------------------------------------------------------------------------------------------
   if ( problemData->parametros->discPratTeor1xN )				// Relação 1xN entre turmas práticas e teóricas
   {
	   std::map< Disciplina*, double, LessPtr<Disciplina> >::iterator 
		   itDiscP = mapDiscPCapMediaPorTurma.begin();
	   for ( ; itDiscP != mapDiscPCapMediaPorTurma.end(); itDiscP++ )
	   {
		   Disciplina *discP = itDiscP->first;
		   Disciplina *discT = problemData->retornaDisciplina( - discP->getId() );
		   
		   double capMediaPorTurmaPrat = itDiscP->second;		
		   double capMediaPorTurmaTeor = mapDiscTCapMediaPorTurma[discT];
		   int demandaTotal = problemData->retornaDemandasDiscNoCampus( discT->getId(), campusAtualId, prioridade ).size();
		   
		   std::cout << "\nDisciplina " << discP->getId();
		   std::cout << "\n\tcapMediaPorTurmaPrat = " << capMediaPorTurmaPrat;
		   std::cout << "\n\tcapMediaPorTurmaTeor = " << capMediaPorTurmaTeor;
		   std::cout << "\n\tdemanda maxima = " << demandaTotal;

		   int nroTurmasPratPorTeor = 1;
		   if ( demandaTotal > capMediaPorTurmaPrat )
		   {
			   if ( capMediaPorTurmaPrat!=0 )
				   nroTurmasPratPorTeor = ceil( capMediaPorTurmaTeor/capMediaPorTurmaPrat );
			   if ( nroTurmasPratPorTeor == 0 )
				   nroTurmasPratPorTeor = 1;
		   }

		   discT->setTurmasPratPorTeor( nroTurmasPratPorTeor );
		   
		   std::cout << "\n\tnroTurmasPratPorTeor = " << nroTurmasPratPorTeor;

		   for ( int turmaT=1; turmaT <= discT->getNumTurmas(); turmaT++ )
		   {
			   GGroup<int> turmasP = discT->getTurmasAssociadas(turmaT);
			   ITERA_GGROUP_N_PT( itTurmaP, turmasP, int )
			   {
				   discP->setTurmaTeorAssociada( *itTurmaP, turmaT );
			   }
		   }

		   int nroTotalPrat = nroTurmasPratPorTeor * discT->getNumTurmas();
		  // if ( nroTotalPrat > discP->getNumTurmas() )
			   discP->setNumTurmas( nroTotalPrat );
	   }
   }
   else if ( problemData->parametros->discPratTeor1x1 )			// Relação 1x1 entre turmas práticas e teóricas
   {
	   // Práticas
	   ITERA_GGROUP_LESSPTR ( itDiscPrat, problemData->novasDisciplinas, Disciplina )
	   {
		   Disciplina *discTeorica = problemData->refDisciplinas[ - itDiscPrat->getId() ];
		   if ( itDiscPrat->getNumTurmas() > discTeorica->getNumTurmas() )
			   discTeorica->setNumTurmas( itDiscPrat->getNumTurmas() );
		   else 
				(*itDiscPrat)->setNumTurmas( discTeorica->getNumTurmas() );

		   discTeorica->setTurmasPratPorTeor( 1 );
	   }
   }
   // ---------------------------------------------------------------------------------------------------------
   
   int nTurmasTotal = 0;
   ITERA_GGROUP_LESSPTR ( itDisc, problemData->disciplinas, Disciplina )
   {
	   nTurmasTotal += itDisc->getNumTurmas();
   }
   problemData->setNumTurmasTotalEstimados( campusAtualId, nTurmasTotal );

   imprimeNumTurmas( campusAtualId, prioridade, 0 );
   imprimeTurmasPT_1xN( campusAtualId, prioridade, 0 );
}

void EstimaTurmas::imprimeNumTurmas( int campusAtualId, int prioridade, int fase )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	std::string nTurmasFileName( getNumTurmasFileName( campusAtualId, prioridade, fase ) );	
	ofstream fout;
	fout.open( nTurmasFileName, ios::out );      
	if ( fout == NULL )
	{
		std::cout << "\nErro em setNumTurmas(int campusAtualId, int prioridade, int fase): arquivo "
					<< nTurmasFileName << " nao encontrado.\n";
		return;
	}
    ITERA_GGROUP_LESSPTR ( itDisc, problemData->disciplinas, Disciplina )
    {
	   fout << "\nDisciplina " << itDisc->getId() << ": " << itDisc->getNumTurmas();
    }
    fout.close();
}

void EstimaTurmas::imprimeTurmasPT_1xN( int campusAtualId, int prioridade, int fase )
{
	if ( ! problemData->parametros->discPratTeor1xN )
	{	
		return;
	}

	#ifndef PRINT_LOGS
		return;
	#endif

	std::string nTurmasFileName( getNumTurmasFileName( campusAtualId, prioridade, fase ) );	
	ofstream fout;
	fout.open( nTurmasFileName, ios::app );      
	if ( fout == NULL )
	{
		std::cout << "\nErro em imprimeTurmasPT_1xN(int campusAtualId, int prioridade, int fase): arquivo "
					<< nTurmasFileName << " nao pode ser aberto.\n";
		return;
	}
    ITERA_GGROUP_LESSPTR ( itDisc, problemData->disciplinas, Disciplina )
    {
		fout << "\nDisciplina " << itDisc->getId();

		if ( problemData->getDisciplinaTeorPrat( *itDisc ) == NULL )
			continue;

		for ( int turma = 1; turma <= itDisc->getNumTurmas(); turma++ )
		{
			fout << "\n\tTurma " << turma << ":  ";
			GGroup<int> turmasAssociadas = itDisc->getTurmasAssociadas(turma);
			ITERA_GGROUP_N_PT( itTurmaAssoc, turmasAssociadas, int )
			{
				fout << *itTurmaAssoc << "; ";
			}
		}
    }
    fout.close();
}

void EstimaTurmas::calculaResultados( int campusId, int prioridade )
{
	int totalTurmas = 0;
	int nroAtendimentoAlunoDemanda = 0;
	int nroAtendAlDemSemPT = 0;
	int nroNaoAtendAlDemSemPT = 0;
	int nroNaoAtendAlDem = 0;
	int totalCargaHoraria = 0;
	double creditosPagos = 0.0;
	double receitaTotal = 0.0;
	double custoDocente = 0.0;

	ITERA_GGROUP_LESSPTR( itSol, this->solVarsEstimaTurmas, VariableEstimaTurmas )
	{		 
		 VariableEstimaTurmas *v = *itSol;

		 Disciplina *vDisc;
		 ConjuntoSala *vSala;
		 int vCampusId;
		 int nCreds;		 
		 int vTurno;
		 Calendario *vCalendario;
		 double receitaAluno;
		 double valorCredPago;
		 
         switch( v->getType() )
         {
			 case VariableEstimaTurmas::V_EST_NUM_ATEND:	// a_{d,s,g}				 
					 vDisc = v->getDisciplina();
					 vTurno = v->getTurno();

					 nroAtendimentoAlunoDemanda += v->getValue();
					 if ( vDisc->getId() > 0 )
						nroAtendAlDemSemPT += v->getValue();

					 totalCargaHoraria += v->getValue() * vDisc->getTotalCreditos();
					 break;
			 case VariableEstimaTurmas::V_EST_NUM_TURMAS:	// x_{d,s,g}
					 vDisc = v->getDisciplina();
					 vSala = v->getSubCjtSala();
					 vTurno = v->getTurno();
					 vCampusId = v->getSubCjtSala()->getCampusId();

					 creditosPagos += v->getDisciplina()->getTotalCreditos() * v->getValue();

					 valorCredPago = problemData->refCampus[vCampusId]->getCusto();
					 custoDocente += valorCredPago * v->getDisciplina()->getTotalCreditos() * v->getValue();
					 totalTurmas += v->getValue();
					 break;
			 case::VariableEstimaTurmas::V_EST_SLACK_DEMANDA_ALUNO:
					vDisc = v->getDisciplina();
					nroNaoAtendAlDem++;
					if ( vDisc->getId() > 0 )
						nroNaoAtendAlDemSemPT++;
			 case::VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC:				 
					receitaAluno = v->getAluno()->getReceita( v->getDisciplina() );
					receitaTotal += v->getDisciplina()->getTotalCreditos() * receitaAluno;
         }

	}

	int runtime = this->getRunTime();

	problemData->imprimeAlocacoesGerais( campusId, prioridade, 0, toString(EST), nroAtendAlDemSemPT, 0,
					totalCargaHoraria, creditosPagos, totalTurmas, custoDocente, receitaTotal, runtime );

}

void EstimaTurmas::carregaVariaveis( int campusAtualId, int prioridade )
{
	std::cout<<"\nLoading solution..."; fflush(NULL);
	   
   lp->updateLP();

   std::cout<<"\nGetting number of cols..."; fflush(NULL);
   int nroColsLP = lp->getNumCols();

   std::cout<<" =  " << nroColsLP; fflush(NULL);
   
   double * xSol = NULL;
   xSol = new double[ nroColsLP ];
	
	#pragma region Carrega solucao  
   std::cout << "\n\nCarregando solucao do estima-turmas...\n"; fflush(NULL);
   if ( (*this->CARREGA_SOLUCAO) )
   {
	   int status = readSolTxt( campusAtualId, prioridade, 0, EstimaTurmas::OutPutFileType::ESTIMA_TURMA_BIN, xSol );
	   if ( !status )
	   {
		   std::cout << "\nErro em EstimaTurmas::carregaVariaveis( int campusId, int prioridade ): arquivo nao encontrado.\n";
		   exit(0);
	   }
   }
   else
   {
	   lp->getX( xSol ); fflush( NULL );
   }
	#pragma endregion

   std::map< std::pair<Disciplina*, Oferta*>, int > mapSlackDemanda;
   std::map< std::pair<Disciplina*, Oferta*>, int >::iterator itMapSlackDemanda;
   
   // -----------------------------------------------------
   // Deleta todas as variaveis referenciadas
   std::cout<<"\nDeleting old solution..."; fflush(NULL);
   ITERA_GGROUP_LESSPTR ( it, solVarsEstimaTurmas, VariableEstimaTurmas )
   {
		delete *it;    
   }
   solVarsEstimaTurmas.clear();
   // -----------------------------------------------------

   char solFilename[1024];
   strcpy( solFilename, getSolucaoEstimaTurmaFileName( campusAtualId, prioridade, 0 ).c_str() );

   std::ofstream fout ( solFilename, std::ios::out );
   if ( fout == NULL )
   {
	   std::cout << "\nErro em EstimaTurmas::carregaVariaveis( int campusAtualId, int prioridade )"
				 << "\nArquivo nao pode ser aberto\n";
	   fout.open( "solSubstituto.txt", std::ios::out );
	   if ( fout == NULL )
	   {
			std::cout <<"\nErro de novo. Finalizando execucao...\n";
			exit(0);
	   }
   }
      
   std::cout<<"\nReading new solution..."; fflush(NULL);

   VariableEstimaTurmasHash::iterator vit = vHashTatico.begin();
   while ( vit != vHashTatico.end() )
   {
	  VariableEstimaTurmas* v = new VariableEstimaTurmas( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         char auxName[100];
         lp->getColName( auxName, col, 100 );
         fout << auxName << " = " << v->getValue() << "\n";

		 solVarsEstimaTurmas.add( v );
      }
	  else
		  delete v;
	 
      vit++;
   }
   
	//fout << "\nAlunosDemanda atendidos = " << nroAtendimentoAlunoDemanda
	//	<< "\nAlunosDemanda nao-atendidos = " << nroNaoAtendAlDem
	//	<< "\nAlunosDemanda atendidos sem divisao PT = " << nroAtendAlDemSemPT
	//	<< "\nAlunosDemanda nao-atendidos sem divisao PT = " << nroNaoAtendAlDemSemPT
	//	<< "\nCreditos pagos = " << creditosPagos
	//	<< "\nCusto Docente / Receita = " << 100 * custoDocente / receitaTotal;
	
	std::cout << std::endl;
	std::cout << std::endl;

    if ( fout )
    {
		fout.close();
    }	
    
    if ( xSol )
    {
       delete [] xSol;
    }
   
    return;
}

int EstimaTurmas::solveModeloEstimaTurmas( int campusId, int prioridade )
{	

	std::cout<<"\n------------------------------EstimaTurmas -----------------------------\n"; fflush(NULL);
		
	int status = 0;
		
	bool CARREGA_SOL_PARCIAL = * this->CARREGA_SOLUCAO;

   if ( (*this->CARREGA_SOLUCAO) )
   {
	   char solName[1024];
	   strcpy( solName, getSolucaoEstimaTurmaFileName( campusId, prioridade, 0 ).c_str() );
	   FILE* fin = fopen( solName,"rb");
	   if ( fin == NULL )
	   {
		   std::cout << "\nA partir de " << solName << " , nao foram lidas mais solucoes.\n"; fflush(NULL);
		   *CARREGA_SOLUCAO = false;
	   }
	   else
	   {
		   std::cout << "\nArquivo-solucao encontrado.\n"; fflush(NULL);
		  fclose(fin);
	   }
   }

   int varNum = 0;
   int constNum = 0;
   
   if ( lp != NULL )
   {
      lp->freeProb();
      delete lp;
#ifdef SOLVER_CPLEX
	   lp = new OPT_CPLEX; 
#endif
#ifdef SOLVER_GUROBI
	   lp = new OPT_GUROBI; 
#endif
   }

   if ( vHashTatico.size() > 0 )
   {
		vHashTatico.clear();
   }
   if ( cHashTatico.size() > 0 )
   {
	   cHashTatico.clear();
   }

   char lpName[1024], id[100];
   strcpy( lpName, getEstimaTurmaLpFileName( campusId, prioridade, 0 ).c_str() );

	if ( problemData->parametros->funcao_objetivo == 0
		|| problemData->parametros->funcao_objetivo == 2 )
	{
		lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
	}
	else if( problemData->parametros->funcao_objetivo == 1 )
	{
		lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
	}

   std::cout<<"\nCreating LP...\n"; fflush(NULL);
    
   if ( problemData->parametros->otimizarPor == "ALUNO" )
   {
	    // Variable creation
	    varNum = criaVariaveisEstimaTurmas( campusId, prioridade );
	    lp->updateLP();
	    printf( "Total of Variables: %i\n\n", varNum ); fflush(NULL);
		
		if ( ! (*this->CARREGA_SOLUCAO) )
		{
		   // Constraint creation
		   constNum = criaRestricoesEstimaTurmas( campusId, prioridade );
		   lp->updateLP();
		   printf( "Total of Constraints: %i\n\n", constNum ); fflush(NULL);
		
		   	#ifdef PRINT_LOGS
				lp->writeProbLP( lpName );
			#endif
		   
		}
   }
   else
   {
		std::cerr<<"\nErro: Parametro otimizarPor deve ser ALUNO!\n"; fflush(NULL);
		exit(0);
   }  

   if ( ! (*this->CARREGA_SOLUCAO) )
   {   
	   double * xSol = NULL;
	   xSol = new double[ lp->getNumCols() ];

	   //atendMaximoSemSabado( campusId, prioridade, fase, CARREGA_SOL_PARCIAL, xSol );

	   solvePorFasesDoDia( campusId, prioridade, CARREGA_SOL_PARCIAL, xSol );
	   
#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		lp->setTimeLimit( OptimizTimeLimit[Solver::EST] );
		//lp->setMIPRelTol( 0.01 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		//lp->setPolishAfterTime(7200);
		//lp->setPolishAfterIntSol(1);
		lp->setMIPEmphasis(1);
		lp->setPolishAfterTime(5400);
		lp->setSymetry(0);
		//lp->setCuts(1);
		lp->updateLP();
#endif
#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		lp->setTimeLimit( 3600 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime( 1800 );
		lp->setMIPEmphasis(1);
		lp->setSymetry(0);
		lp->setCuts(1);
		
		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::EST);		
		cb_data.gapMax = 50;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif

		lp->updateLP();
#endif

		// GENERATES SOLUTION
		
		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readSolBin( campusId, prioridade, 0, OutPutFileType::ESTIMA_TURMA_BIN, xSol );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else writeSolTxt( campusId, prioridade, 0, OutPutFileType::ESTIMA_TURMA_BIN, xSol );
		}
		if ( !CARREGA_SOL_PARCIAL )
		{		
			// GENERATES SOLUTION
			std::cout<<"\n\nOptimizing...\n\n"; fflush(NULL);
			status = lp->optimize( METHOD_MIP );
			std::cout<<"\n\nOtimizado! Status = "<<status<<"\n\n"; fflush(NULL);
			lp->getX(xSol);

			std::cout<<"\nWriting bin...\n"; fflush(NULL);
			writeSolBin( campusId, prioridade, 0, OutPutFileType::ESTIMA_TURMA_BIN, xSol );
			writeSolTxt( campusId, prioridade, 0, OutPutFileType::ESTIMA_TURMA_BIN, xSol );
		}	
				
	    // Fixa o atendimento feito e imprime o lp fixado
		#ifdef PRINT_LOGS		
		//fixaAtendParcial( campusId, prioridade, xSol );		
		//lp->setNumIntSols(1);
  //      status = lp->optimize(METHOD_MIP);
		//lp->getX(xSol);
		#endif

	    // Imprime Gap
		writeGapTxt( campusId, prioridade, 0, OutPutFileType::ESTIMA_TURMA_BIN, lp->getMIPGap() * 100 );
		
		delete [] xSol;   
	}

	std::cout<<"\n------------------------------Fim  -----------------------------\n"; fflush(NULL);

   return status;
}

int EstimaTurmas::solvePorFasesDoDia( int campusId, int prioridade, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n==========================>>>>";
	std::cout << "\nAtendimento geral por fases do dia...\n"; fflush(NULL);
		
	int status = 0;
	
	// -------------------------------------------------------------------
	    
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";	
		
	
	// =====================================================================================
	// CALCULA ATENDIMENTO POR PARTES DO DIA

	int *idxUB = new int[lp->getNumCols()*2];
	double *valUB = new double[lp->getNumCols()*2];
	double *valOrigUB_0 = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB = new BOUNDTYPE[lp->getNumCols()*2];

	int *idxLB_X = new int[lp->getNumCols()*2];
	double *valLB_X = new double[lp->getNumCols()*2];
	double *valOrigLB_X = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsLB_X = new BOUNDTYPE[lp->getNumCols()*2];
		
	int *idxN = new int[lp->getNumCols()];
	for ( VariableEstimaTurmasHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		idxN[vit->second] = vit->second;
	}

	int nChgLB_X = 0;
	for( auto itFase = problemData->fasesDosTurnos.begin(); itFase != problemData->fasesDosTurnos.end(); itFase++ )
	{		 
		int fase = itFase->first;

		std::cout << "\n======>> Fase " << fase << endl;

		// Fixa não-atendimentos de fases à frente.
		int nChgUB = 0;
		for ( VariableEstimaTurmasHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
		{
			VariableEstimaTurmas v = vit->first;
									
			if ( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
			if ( v.getTurno() > fase )
			{
				double ub = (int)( lp->getUB( vit->second ) + 0.5 );
				double lb = (int)( lp->getLB( vit->second ) + 0.5 );

				if ( lb != ub )
				{
					valOrigUB_0[nChgUB] = lp->getUB( vit->second );
					idxUB[nChgUB] = vit->second;
					valUB[nChgUB] = 0.0;
					btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;
					nChgUB++;
				}
			}
		}
		lp->chgBds( nChgUB, idxUB, btsUB, valUB );

		stringstream ss;
		ss << fase;
		std::string sf = ss.str();

		char lpName[1024];
		strcpy( lpName, getEstimaTurmaLpFileName( campusId, prioridade, fase ).c_str() );
	
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName );
		#endif
				  				            
		lp->updateLP();
		
		if ( CARREGA_SOL_PARCIAL )
		{
				// procura e carrega solucao parcial
			int statusReadBin = readSolTxt( campusId, prioridade, fase, Solver::EST, xS );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else{
				writeSolTxt( campusId, prioridade, fase, Solver::EST, xS );
				lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );					
			}								
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
				#ifdef SOLVER_CPLEX
					lp->setNumIntSols(0);

					double runtime = 3600;					
					if (fase==1) runtime = this->getTimeLimit(Solver::EST_M);
					if (fase==2) runtime = this->getTimeLimit(Solver::EST_T);
					if (fase==3) runtime = this->getTimeLimit(Solver::EST_N);
					lp->setTimeLimit( runtime );

					lp->setPreSolve(OPT_TRUE);
					lp->setHeurFrequency(1.0);
					lp->setMIPEmphasis(0);
					lp->setSymetry(0);
					lp->setCuts(2);
					lp->setPolishAfterNode(30);
					lp->setNodeLimit(1000000);
					lp->setVarSel(0);
					lp->setHeur(10);
					lp->setRins(10);
					lp->setMIPRelTol( 0.0 );
					lp->setMIPScreenLog( 4 );
					lp->setPreSolve(OPT_TRUE);
				#elif defined SOLVER_GUROBI
					lp->setNumIntSols(0);

					double runtime = 3600;					
					if (fase==1) runtime = this->getTimeLimit(Solver::EST_M);
					if (fase==2) runtime = this->getTimeLimit(Solver::EST_T);
					if (fase==3) runtime = this->getTimeLimit(Solver::EST_N);
					lp->setTimeLimit( runtime );

					lp->setPreSolveIntensity( OPT_LEVEL1 );
					lp->setHeurFrequency(1.0);
					lp->setMIPEmphasis(0);
					lp->setSymetry(0);
					lp->setCuts(0);
					lp->setPolishAfterTime( runtime / 2 );
					lp->setNodeLimit(10000000);
					lp->setMIPScreenLog( 4 );
					lp->setMIPRelTol( 0.0 );

					#if defined SOLVER_GUROBI && defined USAR_CALLBACK
					
					double maxruntime = 1800;					
					if (fase==1) maxruntime = this->getMaxTimeNoImprov(Solver::EST_M);
					if (fase==2) maxruntime = this->getMaxTimeNoImprov(Solver::EST_T);
					if (fase==3) maxruntime = this->getMaxTimeNoImprov(Solver::EST_N);
					lp->setTimeLimit( runtime );

					cb_data.timeLimit = maxruntime;
					cb_data.gapMax = 10;
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
					#endif
				#endif

				lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );
						
				// GENERATES SOLUTION
				std::cout << "\n\nOptimize...\n\n";
				status = lp->optimize( METHOD_MIP );
				lp->getX(xS);
							
		
				writeSolBin( campusId, prioridade, fase, Solver::EST, xS );
				writeSolTxt( campusId, prioridade, fase, Solver::EST, xS );
		}		
		
		// Volta bounds originais de variaveis x_{d,s,g} que foram fixadas como NÃO atendidas.
		lp->chgBds( nChgUB, idxUB, btsUB, valOrigUB_0 );


		// Fixar o atendimento parcial ( fd_{a,d} = 0 ;  x_{d,s,g} = value )
		int nAtends = 0;
		nChgUB = 0;
		for ( VariableEstimaTurmasHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
		{
			VariableEstimaTurmas v = vit->first;
									
			if ( v.getType() == VariableEstimaTurmas::V_EST_SLACK_DEMANDA_ALUNO )
			{
				double value = (int)( xS[ vit->second ] + 0.5 );
				if ( value == 0.0 )
				{
					idxUB[nChgUB] = vit->second;
					valUB[nChgUB] = 0.0;
					btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;
					nChgUB++;
					nAtends++;
				}
			}
			else if ( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
			if ( v.getTurno() <= fase )
			{
				double value = (int)( xS[ vit->second ] + 0.5 );				
				double ub = (int)( lp->getUB( vit->second ) + 0.5 );
				double lb = (int)( lp->getLB( vit->second ) + 0.5 );

				if ( value > 0 )
				if ( lb != ub )
				{
					valOrigLB_X[nChgLB_X] = lp->getLB( vit->second );
					idxLB_X[nChgLB_X] = vit->second;
					valLB_X[nChgLB_X] = value;
					btsLB_X[nChgLB_X] = BOUNDTYPE::BOUND_LOWER;
					nChgLB_X++;
				}
			}
		}
		lp->chgBds( nChgUB, idxUB, btsUB, valUB );			// Fixa fd = 0
		lp->chgBds( nChgLB_X, idxLB_X, btsLB_X, valLB_X );	// Fixa x = 1

		std::cout << "\n---> Total de alunos-demanda atendidos ao fim da fase " << fase << ": " << nAtends << endl;


		#pragma region Imprime Gap
	 	// Imprime Gap
		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em EstimaTurmas::solvePorFasesDoDia().\n";
		}
		else
		{
			outGaps << "EstimaTurmas - fase do dia " << fase << " - campus "<< campusId 
				<< " - prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
		} 
		#pragma endregion
		
	}

	// Volta lower bounds originais de variaveis x_{d,s,g}
	lp->chgBds( nChgLB_X, idxLB_X, btsLB_X, valOrigLB_X );

				  				          	
	std::cout << "\n================================================================================";
	
	delete [] idxLB_X;
	delete [] valLB_X;
	delete [] btsLB_X;
	delete [] valOrigLB_X;

	delete [] idxUB;
	delete [] valUB;
	delete [] btsUB;
	delete [] valOrigUB_0;
	
	delete [] idxN;

	return status;
}


int EstimaTurmas::atendMaximoSemSabado( int campusId, int prioridade, bool &CARREGA_SOL_PARCIAL, double *xSol )
{
	std::cout << "\n===========>>";
	std::cout << "\nAtendimento fixo em 0 caso a fase do dia seja unica para o sabado...\n"; fflush(NULL);

	int status = 0;
	int *idxUB = new int[lp->getNumCols()];
	double *valOrigUB = new double[lp->getNumCols()];
	double *valUB = new double[lp->getNumCols()];
	BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()];

	int nChgUB = 0;	

	std::map< VariableEstimaTurmas, int >::iterator vit;
	for ( vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableEstimaTurmas v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS ) //&& xS[vit->second] < 0.1 )
		{
			GGroup<int> diasNoTurno = v.getDisciplina()->diasQuePossuiTurno( v.getTurno() );

			// Se o unico dia no turno for o sabado, impede provisoriamente o atendimento dessa disciplina nesse turno
			// a fim de limitar e facilitar o problema em primeiro plano
			if ( diasNoTurno.size() == 1 )
			if ( diasNoTurno.find( 7 ) != diasNoTurno.end() )
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 0.0;
				valOrigUB[nChgUB] = lp->getUB( vit->second );
				bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
				nChgUB++;
			}
		}
	}

	lp->chgBds(nChgUB,idxUB,bts,valUB);
	lp->updateLP();

	// -------------------------------------------------------------------
	
    char lpName[1024];
    strcpy( lpName, getEstimaTurmaLpFileName( campusId, prioridade, 0 ).c_str() );

	std::string lpName2;
	lpName2 += "fixaZeroSab_";
	lpName2 += string(lpName);

	#ifdef PRINT_LOGS
		lp->writeProbLP( lpName2.c_str() );
	#endif
				  				            
	#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::EST_FIXA_SAB_ZERO) );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPEmphasis(1);
		lp->setSymetry(0);
		lp->setCuts(2);
		lp->setPolishAfterNode(30);
		lp->setNodeLimit(1000000);
		lp->setVarSel(0);
		lp->setHeur(10);
		lp->setRins(10);
		lp->setMIPRelTol( 0.0 );
		lp->setMIPScreenLog( 4 );
		lp->setPreSolve(OPT_TRUE);
	#endif

	#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::EST_FIXA_SAB_ZERO) );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPEmphasis(1);
		lp->setSymetry(0);
		lp->setCuts(0);
		lp->setPolishAfterTime(3600);
		lp->setNodeLimit(10000000);
		lp->setMIPScreenLog( 4 );
		lp->setMIPRelTol( 0.0 );
	#endif
			
	lp->updateLP();
		
	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolBin( campusId, prioridade, 0, EstimaTurmas::ESTIMA_TURMA_BIN1, xSol );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, 0, EstimaTurmas::ESTIMA_TURMA_BIN1, xSol );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		// GENERATES SOLUTION
		status = lp->optimize( METHOD_MIP );
		lp->getX(xSol);						
		std::cout << "\nStatus ESTIMA_TURMA_BIN1 =  " << status << endl;

		writeSolBin( campusId, prioridade, 0, EstimaTurmas::ESTIMA_TURMA_BIN1, xSol );
		writeSolTxt( campusId, prioridade, 0, EstimaTurmas::ESTIMA_TURMA_BIN1, xSol );		
	}		

	#ifdef PRINT_LOGS
	#pragma region Imprime Gap	
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";	
	outGaps.open(gapFilename, ofstream::app);
	if ( !outGaps )
	{
		std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em EstimaTurmas::().\n";
	}
	else
	{
		outGaps << "Estima Turmas - Zero em Sabado de Turno Unico - campus "<< campusId << ", prioridade " << prioridade;
		outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
		outGaps << "\n\n";
		outGaps.close();
	} 
	#pragma endregion
	#endif
	
	
	// -------------------------------------------------------------------
	// Volta com os bounds originais
	
	lp->chgBds(nChgUB,idxUB,bts,valOrigUB);	
    lp->updateLP();
	
	// -------------------------------------------------------------------


	std::cout << "\n===========>>";
	std::cout << "\nFixando atendimento obtido...\n"; fflush(NULL);

	int nroAlunosDemandaAtendidos=0;
	nChgUB = 0;	
	for ( vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableEstimaTurmas v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariableEstimaTurmas::V_EST_SLACK_DEMANDA_ALUNO && xSol[vit->second] < 0.1 )
		{
			idxUB[nChgUB] = vit->second;
			valUB[nChgUB] = 0.0;
			bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
			nChgUB++;

			nroAlunosDemandaAtendidos++;				
		}
	}

	lp->chgBds(nChgUB,idxUB,bts,valUB);
	lp->updateLP();
	  
	std::cout << "\n\nNumero de atendidos: "<< nroAlunosDemandaAtendidos <<endl <<endl;
	std::cout << "\n================================================================================";

	delete [] idxUB;
	delete [] valUB;
	delete [] valOrigUB;
	delete [] bts;

	return status;
}

int EstimaTurmas::fixaAtendParcial( int campusId, int P, double *xS )
{
	// ATENÇÃO: Cuidado para NUNCA usar esse método antes de um getX sem optimize.

	std::cout << "\n===========>>";
	std::cout << "\nFixando atendimento parcial...\n"; fflush(NULL);

	int *idxUB = new int[lp->getNumCols()];
	double *valUB = new double[lp->getNumCols()];
	BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()];

	int nroAlunosDemandaAtendidos=0;
	int nChgUB = 0;	

	std::map< VariableEstimaTurmas, int >::iterator vit;
	for ( vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableEstimaTurmas v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariableEstimaTurmas::V_EST_SLACK_DEMANDA_ALUNO && xS[vit->second] < 0.1 )
		{
			idxUB[nChgUB] = vit->second;
			valUB[nChgUB] = 0.0;
			bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
			nChgUB++;

			nroAlunosDemandaAtendidos++;		
		}
	}

	lp->chgBds(nChgUB,idxUB,bts,valUB);
	lp->updateLP();

	std::string lpName( getEstimaTurmaLpFileName( campusId, P, 0 ) );
	lpName.append( "-fixado" );
	lp->writeProbLP( lpName.c_str() );

	delete [] idxUB;
	delete [] valUB;
	delete [] bts;

	return nroAlunosDemandaAtendidos;
}


void EstimaTurmas::chgCoeffList(
   std::vector< std::pair< int, int > > cL,
   std::vector< double > cLV )
{
   lp->updateLP();

   int * rList = new int[ cL.size() ];
   int * cList = new int[ cL.size() ];
   double * vList = new double[ cLV.size() ];

   for ( int i = 0; i < (int) cL.size(); i++ )
   {
      rList[ i ] = cL[ i ].first;
      cList[ i ] = cL[ i ].second;
      vList[ i ] = cLV[ i ];
   }

   lp->chgCoefList( (int) cL.size(), rList, cList, vList );
   lp->updateLP();

   delete [] rList;
   delete [] cList;
   delete [] vList;
}


   /********************************************************************
   **             CRIAÇÃO DE VARIAVEIS DO EstimaTurmas                **
   *********************************************************************/

   
int EstimaTurmas::criaVariaveisEstimaTurmas( int campusId, int P )
{
	int numVars=0;

	int numVarsAnterior = 0;
	CPUTimer timer;
	double dif = 0.0;
	
	timer.start();
	numVars += this->criaVariavelNumTurmas( campusId, P ); // x_{d,u,s,g}
	timer.stop();
	dif = timer.getCronoCurrSecs();	
	std::cout << "numVars \"x\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = numVars;


	timer.start();
	numVars += this->criaVariavelLimiteSupCredsNasSalas( campusId, P ); // H
	timer.stop();
	dif = timer.getCronoCurrSecs();	
	std::cout << "numVars \"H\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = numVars;
	

	timer.start();
	numVars += this->criaVariavelFolgaTurmasMesmaDiscNaSala( campusId, P ); // fs_{d,u,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();	
	std::cout << "numVars \"fs\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = numVars;

	timer.start();
	numVars += this->criaVariavelNumAtendPorTurno( campusId, P ); // a_{d,s,g}
	timer.stop();
	dif = timer.getCronoCurrSecs();	
	std::cout << "numVars \"a\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = numVars;

	#ifdef EQUIVALENCIA_DESDE_O_INICIO
		timer.start();
		numVars += this->criaVariavelFolgaDemandaDiscAluno( campusId, P ); // fd_{d,a}
		timer.stop();
		dif = timer.getCronoCurrSecs();	
		std::cout << "numVars \"fd\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
		numVarsAnterior = numVars;

		timer.start();
		numVars += this->criaVariavelAlocaAlunoTurmaDiscEquiv( campusId, P ); // s_{d,a,cp}
		timer.stop();
		dif = timer.getCronoCurrSecs();	
		std::cout << "numVars \"s\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
		numVarsAnterior = numVars;
		
		/*
		timer.start();
		numVars += this->criaVariavelSalaAluno( campusId ); // as_{a,s}
		timer.stop();
		dif = timer.getCronoCurrSecs();	
		std::cout << "numVars \"as\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
		numVarsAnterior = numVars;
		*/

	#else	
		timer.start();
		numVars += this->criaVariavelFolgaDemanda( campusId, P ); // fd_{d,sl}
		timer.stop();
		dif = timer.getCronoCurrSecs();	
		std::cout << "numVars \"fd\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
		numVarsAnterior = numVars;
	#endif

	timer.start();
	numVars += this->criaVariavelSomaCredSalaPorTurno( campusId ); // xcs1_{s,g}, xcs2_{s,g}, xcs3_{s,g}	
	timer.stop();
	dif = timer.getCronoCurrSecs();	
	std::cout << "numVars \"xcs1{s,g}, xcs2_{s,g}, xcs3_{s,g}\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = numVars;
		
	
	timer.start();
	numVars += this->criaVariavelNumDisciplinas( campusId, P ); // k_{d}	
	timer.stop();
	dif = timer.getCronoCurrSecs();	
	std::cout << "numVars \"k_{d}\": " << (numVars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = numVars;
		
	

	return numVars;
}

// x_{d,s,g}
int EstimaTurmas::criaVariavelNumTurmas( int campusId, int P )
{
	int numVars=0;

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<TurnoIES*, 
				GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> > 
		*mapDisc = & problemData->mapDisciplina_TurnoIES_AlunosDemanda[campusId];

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<TurnoIES*, 
			GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> >::iterator
			itMapDisc = (*mapDisc).begin();
	for ( ; itMapDisc != (*mapDisc).end(); itMapDisc++ )
	{
		Disciplina *disciplina = itMapDisc->first;

		GGroup<int> turnos;

		map<TurnoIES*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > 
			*mapTurnoIES = & itMapDisc->second[P]; // P1

		map<TurnoIES*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >::iterator
			itMapTurnoIES = (*mapTurnoIES).begin();
		for ( ; itMapTurnoIES != (*mapTurnoIES).end(); itMapTurnoIES++ )
		{
			TurnoIES *turnoIES = itMapTurnoIES->first;

			int demanda = itMapTurnoIES->second.size();

			GGroup< Horario*, LessPtr<Horario> > 
				horarios = disciplina->getHorariosOuCorrespondentes(turnoIES);
			ITERA_GGROUP_LESSPTR( itH, horarios, Horario )
			{
				DateTime dt = itH->horario_aula->getInicio();
				int turno = problemData->getFaseDoDia( dt );
				turnos.add( turno );
				if ( turnos.size() == problemData->getNroTotalDeFasesDoDia() ) break;
			}

			if (horarios.size() == 0 && demanda)
			{
				std::cout << "\nErro. Existe possivel demanda para disciplina " << disciplina->getId()
				<< " no turno " << turnoIES->getId() << ", mas nao existe horarios da disciplina no turno.";
			}

			if ( turnos.size() == problemData->getNroTotalDeFasesDoDia() ) break;
		}
		
		ITERA_GGROUP_N_PT( itTurno, turnos, int )
		{
			int turno = *itTurno;

			std::map< int, ConjuntoSala* >::iterator itMapCjtSala = disciplina->cjtSalasAssociados.begin();
			for( ; itMapCjtSala != disciplina->cjtSalasAssociados.end(); itMapCjtSala++ )
			{
				ConjuntoSala *cjtSala = itMapCjtSala->second;

				if ( cjtSala->getCampusId() != campusId ) continue;

				VariableEstimaTurmas v;
				v.reset();
				v.setType( VariableEstimaTurmas::V_EST_NUM_TURMAS );

				v.setDisciplina( disciplina );	 // d
				v.setSubCjtSala( cjtSala );		// tps  
				v.setTurno( turno );			// g

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					vHashTatico[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = 20.0; // TO DO
		
					OPT_COL col( OPT_COL::VAR_INTEGRAL, 10.0, lowerBound, upperBound,
						( char * )v.toString().c_str());

					lp->newCol( col );
					numVars++;
				}
			}
		}
	}

	return numVars;
}

// Hs_{cp}
int EstimaTurmas::criaVariavelLimiteSupCredsNasSalas( int campusId, int P )
{
	int num_vars = 0;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus *cp = *itCampus;

	    if ( cp->getId() != campusId )
	    {
		   continue;
	    }

       VariableEstimaTurmas v;
		v.reset();
      v.setType( VariableEstimaTurmas::V_EST_LIM_SUP_CREDS_SALA);
		v.setCampus( cp );

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();

			double upperbound = 10000.0;

			/*ITERA_GGROUP_N_PT( itDisc, problemData->cp_discs[ cp->getId() ], int )
			{
				Disciplina* disciplina = problemData->refDisciplinas[ *itDisc ];

				int nCreds = disciplina->getCredTeoricos() + disciplina->getCredPraticos();

				upperbound += nCreds * disciplina->getNumTurmas() * disciplina->getTempoCredSemanaLetiva();				
			}*/

			double coef = 1.0;
			
			double lowerBound = 0.0;		

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperbound, ( char * )v.toString().c_str() );

			lp->newCol( col );
			num_vars++;
		}
	}

	return num_vars;
}

// fs_{d,s,g}
int EstimaTurmas::criaVariavelFolgaTurmasMesmaDiscNaSala( int campusId, int P )
{
	int num_vars = 0;	    

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<TurnoIES*, 
				GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> > 
		mapDisc = problemData->mapDisciplina_TurnoIES_AlunosDemanda[campusId];

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<TurnoIES*, 
			GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> >::iterator
			itMapDisc = mapDisc.begin();
	for ( ; itMapDisc != mapDisc.end(); itMapDisc++ )
	{
		Disciplina *disciplina = itMapDisc->first;

		std::map< int, ConjuntoSala* >::iterator itMapCjtSala = disciplina->cjtSalasAssociados.begin();
		for( ; itMapCjtSala != disciplina->cjtSalasAssociados.end(); itMapCjtSala++ )
		{		
			Unidade *unidade = problemData->refUnidade[ itMapCjtSala->second->salas.begin()->second->getIdUnidade() ];			
			if ( unidade->getIdCampus() != campusId )
			{
				continue;
			}
					
			std::map< int, std::map<int, GGroup<Horario*, LessPtr<Horario>>> >::iterator 
				itTurno = disciplina->turnos.begin();
			for ( ; itTurno != disciplina->turnos.end(); itTurno++ )
			{
				int turno = (*itTurno).first;

				VariableEstimaTurmas v;
				v.reset();
				v.setType( VariableEstimaTurmas::V_EST_SLACK_DISC_SALA);
				v.setDisciplina( disciplina );
				v.setUnidade( unidade );
				v.setSubCjtSala( itMapCjtSala->second );
				v.setTurno( turno );

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					vHashTatico[v] = lp->getNumCols();

					double coef = 1.0;

					double upperBound = 100.0;						 
					double lowerBound = 0.0;

					OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				}
			}
		}
	}

	return num_vars;
}

// fd_{d,sl}
int EstimaTurmas::criaVariavelFolgaDemanda( int campusId, int P )
{
	int numVars=0;

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<Calendario*, 
				GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<Calendario>> >, LessPtr<Disciplina> > 
		mapDisc = problemData->mapDisciplina_Calendarios_AlunosDemanda[campusId];

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<Calendario*, 
			GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<Calendario>> >, LessPtr<Disciplina> >::iterator
			itMapDisc = mapDisc.begin();
	for ( ; itMapDisc != mapDisc.end(); itMapDisc++ )
	{
		Disciplina *disciplina = itMapDisc->first;
		
		map<Calendario*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<Calendario> > 
			mapCalend = itMapDisc->second[P]; // P1

		map<Calendario*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<Calendario> >::iterator
			itMapCalend = mapCalend.begin();
		for ( ; itMapCalend != mapCalend.end(); itMapCalend++ )
		{			
			Calendario *calendario = itMapCalend->first;
			
			double nAlunosDemanda = itMapCalend->second.size();

			VariableEstimaTurmas v;
			v.reset();
			v.setType( VariableEstimaTurmas::V_EST_SLACK_DEMANDA );
			v.setDisciplina( disciplina );			// d
			v.setSemanaLetiva( calendario );		// sl

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = nAlunosDemanda;
		
				OPT_COL col( OPT_COL::VAR_INTEGRAL, 20.0, lowerBound, upperBound,
					( char * )v.toString().c_str());

				lp->newCol( col );
				numVars++;
			}
		}
	}


	return numVars;
}

// a_{d,s,g}
int EstimaTurmas::criaVariavelNumAtendPorTurno( int campusId, int P )
{
	
	int numVars=0;

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<TurnoIES*, 
				GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> > 
		*mapDisc = & problemData->mapDisciplina_TurnoIES_AlunosDemanda[campusId];

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<TurnoIES*, 
			GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> >::iterator
			itMapDisc = (*mapDisc).begin();
	for ( ; itMapDisc != (*mapDisc).end(); itMapDisc++ )
	{
		Disciplina *disciplina = itMapDisc->first;
		
		map<int /*Turno*/, int /*NroAlunoDemanda*/ > mapTurnoQtdAlDem;

		map<TurnoIES*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > 
			*mapTurnoIES = & itMapDisc->second[P]; // P1

		map<TurnoIES*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >::iterator
			itMapTurnoIES = (*mapTurnoIES).begin();
		for ( ; itMapTurnoIES != (*mapTurnoIES).end(); itMapTurnoIES++ )
		{
			TurnoIES *turnoIES = itMapTurnoIES->first;
			int qtd = itMapTurnoIES->second.size();

			GGroup<int> turnos;
			GGroup< Horario*, LessPtr<Horario> > 
				horarios = disciplina->getHorariosOuCorrespondentes(turnoIES);
			ITERA_GGROUP_LESSPTR( itH, horarios, Horario )
			{
				DateTime dt = itH->horario_aula->getInicio();
				int turno = problemData->getFaseDoDia( dt );
				turnos.add( turno );
				if ( mapTurnoQtdAlDem.find(turno) == mapTurnoQtdAlDem.end() )
					mapTurnoQtdAlDem[turno] = 0;
			}
			ITERA_GGROUP_N_PT( itTurno, turnos, int )
			{
				mapTurnoQtdAlDem[*itTurno] += qtd;
			}
		}
		
		map<int /*Turno*/, int /*NroAlunoDemanda*/ >::iterator
			itMapTurno = mapTurnoQtdAlDem.begin();
		for ( ; itMapTurno != mapTurnoQtdAlDem.end(); itMapTurno++ )
		{
			int turno = itMapTurno->first;
			double nAlunosDemanda = itMapTurno->second;
							
			std::map< int, ConjuntoSala* >::iterator itMapCjtSala = disciplina->cjtSalasAssociados.begin();
			for( ; itMapCjtSala != disciplina->cjtSalasAssociados.end(); itMapCjtSala++ )
			{
				ConjuntoSala *cjtSala = itMapCjtSala->second;
					
				if ( cjtSala->getCampusId() != campusId ) continue;

				VariableEstimaTurmas v;
				v.reset();
				v.setType( VariableEstimaTurmas::V_EST_NUM_ATEND );

				v.setDisciplina( disciplina );	  // d
				v.setTurno( turno );			  // g
				v.setSubCjtSala( cjtSala );		// s

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					vHashTatico[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = OPT_INF;
		
					OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, lowerBound, upperBound,
						( char * )v.toString().c_str());

					lp->newCol( col );
					numVars++;
				}
			}
		}
	}

	return numVars;
}
   
// fd_{d,a}
int EstimaTurmas::criaVariavelFolgaDemandaDiscAluno( int campusId, int P )
{
   int numVars = 0;
   
   Campus *cp = problemData->refCampus[campusId];

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	    Aluno *aluno = *itAluno;

		if ( !aluno->hasCampusId( campusId ) )
	    {
		    continue;
	    }

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
			{
				// Se for pratica, não cria no modelo com equivalencia. Olha-se só a folga da teórica,
				// e a variavel de alocação s_{a,dt} = s_{a,dp}
				if ( disciplina->getId() < 0 )
					continue;
			}
			#endif

			VariableEstimaTurmas v;
			v.reset();
			v.setType( VariableEstimaTurmas::V_EST_SLACK_DEMANDA_ALUNO );

			v.setAlunoDemanda( *itAlDemanda );
			v.setDisciplina( disciplina );  // d
			v.setAluno( aluno );			// a

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{				
				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;

				double coef = 0.0;
				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = - 50 * disciplina->getTotalCreditos() * aluno->getOferta( itAlDemanda->demanda )->getReceita();
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 10 * disciplina->getTotalCreditos() * cp->getCusto();
				}

#ifdef UNIT
            coef = 1000.0;
#endif
								
				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
				( char * )v.toString().c_str() );

				lp->newCol( col );
				numVars++;
			}
        }
   }

   return numVars;
}

// s_{d,a,g,cp}
int EstimaTurmas::criaVariavelAlocaAlunoTurmaDiscEquiv( int campusId, int P )
{
	int numVars = 0;
	
	Campus *cp = problemData->refCampus[campusId];
	
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			if ( itAlDemanda->getCampus()->getId() != campusId )
				continue;

			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;
			
			std::map<Disciplina*, GGroup<int>> mapDiscTurnos;
			mapDiscTurnos[disciplina] = problemData->getTurnosComunsViaveis( disciplina, (*itAlDemanda)->demanda->getTurnoIES() );
			
			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );			

			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );
		
			if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
			{
				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{					
					GGroup<int> turnosViaveis = problemData->alocacaoEquivViavelTurnosIES( (*itAlDemanda)->demanda, *itDisc );
					if ( turnosViaveis.size() )
					{
						mapDiscTurnos[*itDisc] = turnosViaveis;
						disciplinasPorAlDem.add( *itDisc );
					}
				}
			}

			if ( P > 1 )
			{
				int chRequerida = aluno->getCargaHorariaOrigRequeridaP1();
				int chAtendida = problemData->cargaHorariaJaAtendida( aluno );
				if ( chRequerida - chAtendida <= 0 )
						continue;
			}

			ITERA_GGROUP_LESSPTR( itDiscEq, disciplinasPorAlDem, Disciplina )
			{
				Disciplina *disciplinaEquiv = (*itDiscEq);
													
				GGroup<int> turnos = mapDiscTurnos[disciplinaEquiv];
				ITERA_GGROUP_N_PT( itTurno, turnos, int )
				{
					VariableEstimaTurmas v;
					v.reset();
					v.setType( VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC );
					v.setAlunoDemanda( *itAlDemanda );
					v.setAluno( aluno );
					v.setDisciplina( disciplinaEquiv );
					v.setTurno( *itTurno );
					v.setCampus( cp );

					if ( vHashTatico.find( v ) == vHashTatico.end() )
					{
						vHashTatico[ v ] = lp->getNumCols();
									
						double lowerBound = 0.0;
						double upperBound = 1.0;
						double coef = 0.0;		
					
						OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

						lp->newCol( col );
						numVars++;
					}
				}
			}
		}
	}

	return numVars;
}

// xcs1_{s}, xcs2_{s}, xcs3_{s}, xcs4_{s}
int EstimaTurmas::criaVariavelSomaCredSala( int campusId )
{
	int numVars = 0;
   double upperBound1 = 0;
   double upperBound2 = 0;
   double upperBound3 = 0;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
		{
			continue;
		}

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            VariableEstimaTurmas v;
            v.reset();
            v.setType( VariableEstimaTurmas::V_EST_CRED_SALA_F1 );
            v.setCampus( *itCampus );          // c
            v.setUnidade( *itUnidade );		   // u
            v.setSubCjtSala( *itCjtSala );	   // s

    //        double htps = ( itCjtSala->minLimiteTempoPermitidoNaSemana() + 
				//itCjtSala->maxTempoPermitidoNaSemana(problemData->mapDiscSubstituidaPor) )/2 ;

			double minTime = v.getSubCjtSala()->minLimiteTempoPermitidoNaSemana();
			double maxTime = v.getSubCjtSala()->maxTempoPermitidoNaSemana(problemData->mapDiscSubstituidaPor);
			double htps = 0;

			if ( maxTime >= 2*minTime ) // Se há diferença significativa entre tamanho das semanas letivas
				htps = ( maxTime + minTime ) / 3;
			else
				htps = ( maxTime + minTime ) / 2;


            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
               vHashTatico[v] = lp->getNumCols();

               double coef = 0.0;

               double lowerBound = 0.0;
  
               upperBound1 = (int)(htps * 0.5 + 0.5); // 50% do tempo
			   
               OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound1,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );

               numVars++;
            }

            v.reset();
            v.setType( VariableEstimaTurmas::V_EST_CRED_SALA_F2 );
            v.setCampus( *itCampus );          // c
            v.setUnidade( *itUnidade );		   // u
            v.setSubCjtSala( *itCjtSala );	   // s

            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
               vHashTatico[v] = lp->getNumCols();

               double coef = 1.0;

               double lowerBound = 0.0;
			                  
               upperBound2 = (int)(htps * 0.3 + 0.5);	// 30% do tempo
               
               OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound2,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );

               numVars ++;
            }

            v.reset();
            v.setType( VariableEstimaTurmas::V_EST_CRED_SALA_F3 );
            v.setCampus( *itCampus );          // c
            v.setUnidade( *itUnidade );		   // u
            v.setSubCjtSala( *itCjtSala );	   // s

            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
               vHashTatico[v] = lp->getNumCols();

               double coef = 4.0;

               double lowerBound = 0.0;

               upperBound3 = (int)(htps * 0.1 + 0.5);	// 10% do tempo
               
               OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound3,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );

               numVars ++;
            }

            v.reset();
            v.setType( VariableEstimaTurmas::V_EST_CRED_SALA_F4 );
            v.setCampus( *itCampus );          // c
            v.setUnidade( *itUnidade );		   // u
            v.setSubCjtSala( *itCjtSala );	   // s

            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
               vHashTatico[v] = lp->getNumCols();

               double coef = 10.0;

               double lowerBound = 0.0;
               double upperBound = 1.0;

               upperBound = htps - upperBound1 - upperBound2 - upperBound3;	// 10% do tempo

               OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );

               numVars ++;
            }
         }
      }
   }

	return numVars;
}
   
// xcs1_{s,g}, xcs2_{s,g}, xcs3_{s,g}, xcs4_{s,g}
int EstimaTurmas::criaVariavelSomaCredSalaPorTurno( int campusId )
{
	int numVars = 0;
   double upperBound1 = 0;
   double upperBound2 = 0;
   double upperBound3 = 0;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
		{
			continue;
		}

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			for ( int turno = 1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
			{
				double htps = itCjtSala->maxLimiteTempoPermitidoNaSemana( turno );
				
				VariableEstimaTurmas v;
				v.reset();
				v.setType( VariableEstimaTurmas::V_EST_CRED_SALA_F1 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s
				v.setTurno( turno );			   // g

				if ( vHashTatico.find(v) == vHashTatico.end() )
				{
				   vHashTatico[v] = lp->getNumCols();

				   double coef = 0.0;

				   double lowerBound = 0.0;
  
				   upperBound1 = (int)(htps * 0.5 + 0.5); // 50% do tempo
			   
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound1,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   numVars++;
				}

				v.reset();
				v.setType( VariableEstimaTurmas::V_EST_CRED_SALA_F2 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s
				v.setTurno( turno );			   // g

				if ( vHashTatico.find(v) == vHashTatico.end() )
				{
				   vHashTatico[v] = lp->getNumCols();

				   double coef = 0.1;

				   double lowerBound = 0.0;
			                  
				   upperBound2 = (int)(htps * 0.3 + 0.5);	// 30% do tempo
               
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound2,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   numVars ++;
				}

				v.reset();
				v.setType( VariableEstimaTurmas::V_EST_CRED_SALA_F3 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s
				v.setTurno( turno );			   // g

				if ( vHashTatico.find(v) == vHashTatico.end() )
				{
				   vHashTatico[v] = lp->getNumCols();

				   double coef = 0.4;

				   double lowerBound = 0.0;

				   upperBound3 = (int)(htps * 0.1 + 0.5);	// 10% do tempo
               
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound3,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   numVars ++;
				}

				v.reset();
				v.setType( VariableEstimaTurmas::V_EST_CRED_SALA_F4 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s
				v.setTurno( turno );			   // g

				if ( vHashTatico.find(v) == vHashTatico.end() )
				{
				   vHashTatico[v] = lp->getNumCols();

				   double coef = 1.0;

				   double lowerBound = 0.0;
				   double upperBound = 1.0;

				   upperBound = htps - upperBound1 - upperBound2 - upperBound3;	// 10% do tempo

				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   numVars ++;
				}
			}
         }
      }
   }

	return numVars;
}
   
// as_{a,s}
int EstimaTurmas::criaVariavelSalaAluno( int campusId )
{
   int numVars = 0;
   
   Campus *cp = problemData->refCampus[campusId];

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	    Aluno *aluno = *itAluno;
		
		// Agrupa as salas nas quais um aluno pode vir a ter aula
		GGroup<ConjuntoSala*, LessPtr<ConjuntoSala>> cjtSalas;	
		ITERA_GGROUP_LESSPTR( itAlDem, aluno->demandas, AlunoDemanda )
		{	
			if ( itAlDem->getCampus()->getId() != campusId )
			{
				continue;
			}

			Disciplina *disciplina = (*itAlDem)->demanda->disciplina;

			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );
		
			#ifdef EQUIVALENCIA_DESDE_O_INICIO	
			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );		
			if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) 
			{	// aluno não alocado
				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{
					if ( problemData->alocacaoEquivViavel( itAlDem->demanda, *itDisc ) )
					{
						disciplinasPorAlDem.add( *itDisc );
					}
				}
			}
			#endif			

			ITERA_GGROUP_LESSPTR( itDisc, disciplinasPorAlDem, Disciplina )
			{
				std::map< int, ConjuntoSala* >::iterator itMapSala = itDisc->cjtSalasAssociados.begin();
				for( ; itMapSala != itDisc->cjtSalasAssociados.end(); itMapSala++ )
				{
					if ( itMapSala->second->getCampusId() == campusId )
						cjtSalas.add( itMapSala->second );
				}
			}
		}

		// Cria uma variavel para cada par (aluno,cjtSala)
		ITERA_GGROUP_LESSPTR( itCjtSala, cjtSalas, ConjuntoSala )
		{			
			if ( itCjtSala->getCampusId() != campusId ) continue;

			VariableEstimaTurmas v;
			v.reset();
			v.setType( VariableEstimaTurmas::V_EST_ALUNO_SALA );

			v.setSubCjtSala( *itCjtSala );  // s
			v.setAluno( aluno );			// a

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{				
				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;
				double coef = 1.0;
								
				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
				( char * )v.toString().c_str() );

				lp->newCol( col );
				numVars++;
			}
		}
   }

   return numVars;
}

// k_{d}
int EstimaTurmas::criaVariavelNumDisciplinas( int campusId, int P )
{
	int numVars=0;

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<TurnoIES*, 
				GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> > 
		*mapDisc = & problemData->mapDisciplina_TurnoIES_AlunosDemanda[campusId];

	std::map< Disciplina*, 
		std::map< int /*P*/, 
			map<TurnoIES*, 
			GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> >::iterator
			itMapDisc = (*mapDisc).begin();
	for ( ; itMapDisc != (*mapDisc).end(); itMapDisc++ )
	{
		Disciplina *disciplina = itMapDisc->first;
				
		VariableEstimaTurmas v;
		v.reset();
		v.setType( VariableEstimaTurmas::V_EST_USA_DISCIPLINA );
		v.setDisciplina( disciplina );	 // d
				
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[ v ] = lp->getNumCols();
									
			double lowerBound = 0.0;
			double upperBound = 1.0;
		
			OPT_COL col( OPT_COL::VAR_BINARY, 1.0, lowerBound, upperBound,
				( char * )v.toString().c_str());

			lp->newCol( col );
			numVars++;
		}			
	}

	return numVars;
}



   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO EstimaTurmas              **
   *********************************************************************/

int EstimaTurmas::criaRestricoesEstimaTurmas( int campusId, int prioridade )
{
	int restricoes = 0;
	CPUTimer timer;
	double dif = 0.0;

	int numRestAnterior = 0;


   timer.start();
   restricoes += criaRestricaoCapacidadeSala(campusId);					
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numRest \"Capacidade Sala\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;

   timer.start();
   restricoes += criaRestricaoTempoMaxSala(campusId);					
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numRest \"Tempo Max Sala\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;

#ifdef UNIT
   timer.start();
   restricoes += criaRestricaoTempoMaxSalaSabado(campusId);					
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numRest \"Tempo Max Sala aos Sabados Para Disciplinas de Turno Simples\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;

   timer.start();
   restricoes += criaRestricaoTempoMaxAlunoSabado(campusId);					
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numRest \"Tempo Max Aluno aos Sabados Para Disciplinas de Turno Simples\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;	
#endif


   timer.start();
   restricoes += criaRestricaoDistribuiEntreSalas(campusId);					
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numRest \"Distribui entre Salas\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;


    timer.start();
    restricoes += criaRestricaoTurmasDifMesmaDiscSalaDif(campusId);					
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numRest \"Turmas Dif Salas Dif\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	

    timer.start();
    restricoes += criaRestricaoSomaCredSalaPorTurno(campusId);	
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numRest \"Distribui creditos nas salas\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;	
	
	if ( problemData->parametros->discPratTeor1x1 )
	{
		timer.start();
		restricoes += criaRestricaoNroTurmasPratTeorIgual(campusId);	
		timer.stop();
		dif = timer.getCronoCurrSecs();
		std::cout << "numRest \"Total de turmas praticas e teoricas iguais\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;	
	}


	#ifdef EQUIVALENCIA_DESDE_O_INICIO
		timer.start();
	   restricoes += criaRestricaoAtendeDemandasComAluno(campusId);					
		timer.stop();
		dif = timer.getCronoCurrSecs();
		std::cout << "numRest \"Atende Demandas\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;

		timer.start();
	   restricoes += criaRestricaoAtendeAlunoDemanda(campusId, prioridade);					
		timer.stop();
		dif = timer.getCronoCurrSecs();
		std::cout << "numRest \"Atende AlunoDemanda\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;

	   timer.start();
	   restricoes += criaRestricaoDiscPraticaTeoricaPorAluno(campusId);					
		timer.stop();
		dif = timer.getCronoCurrSecs();
		std::cout << "numRest \"Atend Disc Pratica Teorica\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		
		/*
	   timer.start();
	   restricoes += criaRestricaoNroSalasPorAluno(campusId);					
		timer.stop();
		dif = timer.getCronoCurrSecs();
		std::cout << "numRest \"Nro de salas por aluno\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		*/

	#else
		timer.start();
	   restricoes += criaRestricaoAtendeDemandas(campusId);					
		timer.stop();
		dif = timer.getCronoCurrSecs();
		std::cout << "numRest \"Atende Demandas\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;

		timer.start();
	   restricoes += criaRestricaoDiscPraticaTeorica(campusId);					
		timer.stop();
		dif = timer.getCronoCurrSecs();
		std::cout << "numRest \"Atende Teorica-Pratica\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
	#endif

    timer.start();
    restricoes += criaRestricaoUsaDisciplina(campusId);	
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numRest \"Usa Disciplina\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;	
				

	return restricoes;
}

int EstimaTurmas::criaRestricaoTempoMaxSala( int campusId )
{
   int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
      VariableEstimaTurmas v = vit->first;

      double coef = 0.0;
      if( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
		{
         c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_TEMPO_MAX_SALA );
         c.setSubCjtSala( v.getSubCjtSala() );
         c.setTurno(v.getTurno());

         cit = cHashTatico.find(c);

         double coef = 1.0;

         Disciplina* disciplina = v.getDisciplina();
		 Sala * sala = v.getSubCjtSala()->salas.begin()->second;

         coef = disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos();

         if ( cit == cHashTatico.end() )
         {
//            double htps;
//#ifdef UNIT
//			std::cout<<"\nATENCAO: TEMPO FIXO EM ESTIMA TURMAS!!\n";
//			if ( v.getTurno() == 1 || v.getTurno() == 2 )
//				htps = sala->diasLetivos.size() * 300;
//			else // noite
//				htps = sala->diasLetivos.size() * 200;
//#else
//			htps = v.getSubCjtSala()->minLimiteTempoPermitidoNaSemana( v.getTurno() ); // tempo maximo da sala na semana no turno
//#endif			
			double maxTime = v.getSubCjtSala()->maxLimiteTempoPermitidoNaSemana( v.getTurno() );
			double rhs = maxTime;

			//double minTime = v.getSubCjtSala()->minLimiteTempoPermitidoNaSemana();
			//double maxTime = v.getSubCjtSala()->maxTempoPermitidoNaSemana(problemData->mapDiscSubstituidaPor);
			//double htps = 0;

			//if ( maxTime >= 2*minTime ) // Se há diferença significativa entre tamanho das semanas letivas
			//	htps = ( maxTime + minTime ) / 3;
			//else
			//	htps = ( maxTime + minTime ) / 2;

            nnz = v.getSubCjtSala()->disciplinas_associadas.size() * 5; // estimando em media 5 turmas por disciplina

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
		}
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

// Restrição para ser usada especificamente no caso da UNIT, que tem disponibilidade
// limitada nos sábados para algumas disciplinas
int EstimaTurmas::criaRestricaoTempoMaxSalaSabado( int campusId )
{
   int numRest=0;

#ifndef UNIT
	return numRest;
#endif

   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
   {		
      VariableEstimaTurmas v = vit->first;

      double coef = 0.0;
      if( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
	  {		  
         Disciplina* disciplina = v.getDisciplina();
		 int turno = v.getTurno();

		 bool soPossuiSabado=true;
       GGroup< Calendario*, LessPtr<Calendario> > calends = disciplina->getCalendarios();
		 ITERA_GGROUP_LESSPTR( itCalend, calends, Calendario )
		 {
			 // Verifica se o unico dia no turno é sábado
			GGroup< HorarioDia*, LessPtr<HorarioDia> > horsDia = problemData->getHorariosDiaPorTurno( disciplina, *itCalend, turno );
			ITERA_GGROUP_LESSPTR( itHorarioDia, horsDia, HorarioDia )
			{
				if ( itHorarioDia->getDia() != 7 ) // dia 7 = sábado
				{
					soPossuiSabado = false; break;
				}
			}
			if ( !soPossuiSabado ) break;
		 }

		 if ( !soPossuiSabado ) continue;

         c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_TEMPO_MAX_SALA_SAB );
         c.setSubCjtSala( v.getSubCjtSala() );
         c.setTurno( turno );
		 
         double coef = disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos();
		 
         cit = cHashTatico.find(c);
         if ( cit == cHashTatico.end() )
         {
            double htps;
			/*
			std::cout<<"\nATENCAO: TEMPO MAX NO SAB FIXO EM ESTIMA TURMAS!!\n";

#ifdef KROTON
			if ( v.getTurno() == 1 || v.getTurno() == 2 ) // 1. Manha ou 2. Tarde
				htps = 225;
			else // noite
				htps = 200;
#elif defined UNIT
			if ( v.getTurno() == 1 || v.getTurno() == 2 ) // 1. Manha ou 2. Tarde
				htps = 300;
			else // noite
				htps = 200;
#endif
			*/

			htps = v.getSubCjtSala()->minLimiteTempoPermitidoNaSemana( v.getTurno() ); // tempo maximo da sala na semana no turno

            nnz = v.getSubCjtSala()->disciplinas_associadas.size() * 5 + 10; // estimando em media 5 turmas por disciplina

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::LESS, htps, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
	  }
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}


// Restrição para ser usada especificamente no caso da UNIT, que tem disponibilidade
// limitada nos sábados para algumas disciplinas
int EstimaTurmas::criaRestricaoTempoMaxAlunoSabado( int campusId )
{
   int numRest=0;

#ifndef UNIT
	return numRest;
#endif

   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
   {		
      VariableEstimaTurmas v = vit->first;

      double coef = 0.0;
      if( v.getType() == VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC )
	  {		  
         Disciplina* disciplina = v.getDisciplina();
		 int turno = v.getTurno();
		 Aluno *aluno = v.getAluno();
		 AlunoDemanda *alunoDem = v.getAlunoDemanda(); 

		 bool turnoSoNoSabado=true;

		 // Verifica se o unico dia no turno é sábado
		 GGroup< HorarioDia*, LessPtr<HorarioDia> > horsDia = 
			 problemData->getHorariosDiaPorTurno( disciplina, alunoDem->demanda->getCalendario(), turno );
		 ITERA_GGROUP_LESSPTR( itHorarioDia, horsDia, HorarioDia )
		 {
			if ( itHorarioDia->getDia() != 7 ) // dia 7 = sábado
			{
				turnoSoNoSabado = false; break;
			}
		 }
		 
		 if ( !turnoSoNoSabado ) continue;

         c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_TEMPO_MAX_ALUNO_SAB );
         c.setTurno( turno );
		 c.setAluno( aluno );
		 
         double coef = disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos();
		 
         cit = cHashTatico.find(c);
         if ( cit == cHashTatico.end() )
         {
			double rhs = 0;
			/*
			std::cout<<"\nATENCAO: TEMPO MAX NO SAB PARA ALUNO FIXO EM ESTIMA TURMAS!!\n";

#ifdef KROTON
			if ( v.getTurno() == 1 || v.getTurno() == 2 ) // 1. Manha ou 2. Tarde
				rhs = 225;
			else // noite
				rhs = 200;
#elif defined UNIT
			if ( v.getTurno() == 1 || v.getTurno() == 2 ) // 1. Manha ou 2. Tarde
				rhs = 300;
			else // noite
				rhs = 200;
#endif
				*/

			// Calcula htps
			GGroup<HorarioAula*, LessPtr<HorarioAula>> horsSab = alunoDem->demanda->getCalendario()->retornaHorariosDisponiveisNoDia(7);
			ITERA_GGROUP_LESSPTR( itHorAula, horsSab, HorarioAula )
			{
				if ( problemData->getFaseDoDia( itHorAula->getInicio() ) == turno )
					rhs++;
			}
			rhs *= alunoDem->demanda->getCalendario()->getTempoAula();


			nnz = aluno->demandas.size(); // estimativa

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
	  }
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}


int EstimaTurmas::criaRestricaoAtendeDemandas( int campusId )
{
	int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
   {		
      VariableEstimaTurmas v = vit->first;

      if( v.getType() == VariableEstimaTurmas::V_EST_NUM_ATEND )
	  {
			 c.reset();
			 c.setType( ConstraintEstimaTurmas::C_EST_ATEND_DEMANDA );
			 c.setDisciplina( v.getDisciplina() );
			 c.setSemanaLetiva(v.getSemanaLetiva());

			 cit = cHashTatico.find(c);

			 double coef = 1.0;

			 if ( cit == cHashTatico.end() )
			 {
				nnz = 50;
				double rhs = problemData->getNumDemandaPorDiscECalendario( campusId, v.getDisciplina(), v.getSemanaLetiva() );

				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::EQUAL, rhs, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
	  }
      else if( v.getType() == VariableEstimaTurmas::V_EST_SLACK_DEMANDA )
	  {

			c.reset();
			c.setType( ConstraintEstimaTurmas::C_EST_ATEND_DEMANDA );
			c.setDisciplina( v.getDisciplina() );
			c.setSemanaLetiva(v.getSemanaLetiva());

			cit = cHashTatico.find(c);

			double coef = 1.0;

			if ( cit == cHashTatico.end() )
			{
				nnz = 50;
				double rhs = problemData->getNumDemandaPorDiscECalendario( campusId, v.getDisciplina(), v.getSemanaLetiva() );

				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::EQUAL, rhs, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			}
			else
			{
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			}
		}
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

int EstimaTurmas::criaRestricaoCapacidadeSala( int campusId )
{
	int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
      VariableEstimaTurmas v = vit->first;

      if( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
		{
         c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_CAPACIDADE_SALAS );
         c.setDisciplina( v.getDisciplina() );
         c.setTurno(v.getTurno());
		 c.setSubCjtSala( v.getSubCjtSala() );

         cit = cHashTatico.find(c);

         double coef = 1.0;

         coef = v.getSubCjtSala()->salas.begin()->second->getCapacidade();

         if ( cit == cHashTatico.end() )
         {
			 nnz = 2;

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
		}
      else if( v.getType() == VariableEstimaTurmas::V_EST_NUM_ATEND )
		{
         c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_CAPACIDADE_SALAS );
         c.setDisciplina( v.getDisciplina() );
         c.setTurno(v.getTurno());
		 c.setSubCjtSala( v.getSubCjtSala() );

         cit = cHashTatico.find(c);

         double coef = -1.0; 
		 
		 // Multiplica por um fator de correção, para que tenhamos
		 // mais turmas sendo criadas do que o que seria estritamente calculado pelo modelo
		 
#ifdef UNIRITTER
		 double *factors = new double[4];
		 if ( v.getSubCjtSala()->ehLab() ) // é laboratorio
		 {
			 factors[0] = 1.2;
			 factors[1] = 1.35;
			 factors[2] = 1.45;
			 factors[3] = 1.5;
		 }
		 else
		 {
			 factors[0] = 1.15;
			 factors[1] = 1.3;
			 factors[2] = 1.4;
			 factors[3] = 1.45;
		 }

		 double varUpperBound = v.getSubCjtSala()->salas.begin()->second->getCapacidade();
		 if ( varUpperBound <= 30 )
			 coef *= factors[0];
		 else if ( varUpperBound > 30 && varUpperBound <= 70 )
			 coef *= factors[1];
		 else if ( varUpperBound > 70 && varUpperBound <= 100 )
			 coef *= factors[2];
		 else if ( varUpperBound > 100 )
			 coef *= factors[3];
#elif defined KROTON
		 double *factors = new double[4];
		 if ( v.getSubCjtSala()->ehLab() ) // é laboratorio
		 {
			 factors[0] = 1.1;
			 factors[1] = 1.2;
			 factors[2] = 1.3;
			 factors[3] = 1.4;
		 }
		 else
		 {
			 factors[0] = 1.0;
			 factors[1] = 1.1;
			 factors[2] = 1.2;
			 factors[3] = 1.3;
		 }

		 double varUpperBound = v.getSubCjtSala()->salas.begin()->second->getCapacidade();
		 if ( varUpperBound <= 30 )
			 coef *= factors[0];
		 else if ( varUpperBound > 30 && varUpperBound <= 70 )
			 coef *= factors[1];
		 else if ( varUpperBound > 70 && varUpperBound <= 100 )
			 coef *= factors[2];
		 else if ( varUpperBound > 100 )
			 coef *= factors[3];
#elif defined UNIT
		 double *factors = new double[4];
		 if ( v.getSubCjtSala()->ehLab() ) // é laboratorio
		 {
			 factors[0] = 1.1;
			 factors[1] = 1.2;
			 factors[2] = 1.3;
			 factors[3] = 1.4;
		 }
		 else
		 {
			 factors[0] = 1.0;
			 factors[1] = 1.1;
			 factors[2] = 1.2;
			 factors[3] = 1.3;
		 }

		 double varUpperBound = v.getSubCjtSala()->salas.begin()->second->getCapacidade();
		 if ( varUpperBound <= 30 )
			 coef *= factors[0];
		 else if ( varUpperBound > 30 && varUpperBound <= 70 )
			 coef *= factors[1];
		 else if ( varUpperBound > 70 && varUpperBound <= 100 )
			 coef *= factors[2];
		 else if ( varUpperBound > 100 )
			 coef *= factors[3];
#endif

         if ( cit == cHashTatico.end() )
         {
			 nnz = 2;

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
		}
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

int EstimaTurmas::criaRestricaoDistribuiEntreSalas( int campusId )
{
	int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   int varHIdx = -1;

   // Encontra variavel H
   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{
      VariableEstimaTurmas v = vit->first;

      if ( v.getType() == VariableEstimaTurmas::V_EST_LIM_SUP_CREDS_SALA && v.getCampus()->getId() == campusId )
      {
         varHIdx = vit->second;
      }
   }

   if (  varHIdx < 0 )
      return numRest;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
      VariableEstimaTurmas v = vit->first;

      double coef = 0.0;
      if( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
		{
         c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_DISTRIBUI_ENTRE_SALAS );
         c.setSubCjtSala( v.getSubCjtSala() );
         c.setCampus(v.getCampus());

         cit = cHashTatico.find(c);

         double coef = 1.0;

         Disciplina* disciplina = v.getDisciplina();

         coef = -disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos();

         if ( cit == cHashTatico.end() )
         {
            nnz = 100;

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

            row.insert( vit->second, coef );
            row.insert( varHIdx,1.0);

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
		}
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

int EstimaTurmas::criaRestricaoTurmasDifMesmaDiscSalaDif( int campusId )
{
	int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
      VariableEstimaTurmas v = vit->first;

      if( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
		{
         c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_TURMA_DIF_MESMA_DISC_SALA_DIF );
         c.setDisciplina( v.getDisciplina() );
         c.setSubCjtSala(v.getSubCjtSala());
		 c.setTurno( v.getTurno() );

         cit = cHashTatico.find(c);

         double coef = 1.0;

         if ( cit == cHashTatico.end() )
         {
            nnz = 2;

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
		}
      else if( v.getType() == VariableEstimaTurmas::V_EST_SLACK_DISC_SALA )
		{
         c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_TURMA_DIF_MESMA_DISC_SALA_DIF );
         c.setDisciplina( v.getDisciplina() );
         c.setSubCjtSala(v.getSubCjtSala());
		 c.setTurno( v.getTurno() );

         cit = cHashTatico.find(c);

         double coef = -1.0;

         if ( cit == cHashTatico.end() )
         {
            nnz = 2;

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
		}
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

int EstimaTurmas::criaRestricaoDiscPraticaTeorica( int campusId )
{
	int numRest=0;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;

   vit = vHashTatico.begin();

	for ( ; vit != vHashTatico.end(); vit++ )
	{
      v = vit->first;
	
      if( v.getType() == VariableEstimaTurmas::V_EST_SLACK_DEMANDA )
		{			
			
			Disciplina *disciplinaPratica = NULL;

			// Só para disciplinas praticas/teoricas
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{			
				std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
				if ( itMapDisc == problemData->refDisciplinas.end() )
					continue;
				else
					disciplinaPratica = itMapDisc->second;
			}
			else
			{
				disciplinaPratica = v.getDisciplina();
			}
		
			c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_DISC_PRATICA_TEORICA );
			c.setDisciplina( disciplinaPratica );
         c.setSemanaLetiva(v.getSemanaLetiva());

			double coef = 0.0;
			if ( v.getDisciplina()->getId() < 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz = 2;

				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				numRest++;
			}
		}
	}

	return numRest;
}


int EstimaTurmas::criaRestricaoDiscPraticaTeoricaPorAluno( int campusId )
{
	int numRest=0;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;

   vit = vHashTatico.begin();

	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;
	
		if( v.getType() == VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC )
		{			
			
			Disciplina *disciplinaPratica = NULL;

			// Só para disciplinas praticas/teoricas
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{			
				std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
				if ( itMapDisc == problemData->refDisciplinas.end() )
					continue;
				else
					disciplinaPratica = itMapDisc->second;
			}
			else
			{
				disciplinaPratica = v.getDisciplina();
			}
		
			c.reset();
			c.setType( ConstraintEstimaTurmas::C_EST_DISC_PRATICA_TEORICA );
			c.setDisciplina( disciplinaPratica );
			c.setAluno( v.getAluno() );

			double coef = 0.0;
			if ( v.getDisciplina()->getId() < 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz = 2*3; // 2 vars * nroTurnos

				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				numRest++;
			}
		}
	}

	return numRest;
}

/*
	Para cada aluno a, disciplina d, campus cp:

	sum[g] sum[deq] s_{a,deq,g,cp} + fd_{d,a} = 1

	aonde deq é uma disciplina equivalente a d, ou a propria d. Caso deq tenha creditos praticos e teoricos, 
	só entram na restricao os teoricos.
	E g é o turno que pode ser considerado para alocação de a em deq
*/
int EstimaTurmas::criaRestricaoAtendeAlunoDemanda( int campusId, int prioridade )
{
   int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
   {		
      VariableEstimaTurmas v = vit->first;

      if ( v.getType() == VariableEstimaTurmas::V_EST_SLACK_DEMANDA_ALUNO ||
		   v.getType() == VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC )
	  {         		  
		 Disciplina *disciplina;

		 double coef = 0.0;
		 if( v.getType() == VariableEstimaTurmas::V_EST_SLACK_DEMANDA_ALUNO ) // fd_{a,d}
		 {
			 coef = 1.0;
			 disciplina = v.getDisciplina();
			 // Pula disciplina pratica
			 if ( disciplina->getId() < 0 )
				 continue;
		 }
		 else if ( v.getType() == VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC ) // s_{a,d,g}
		 {
			 coef = 1.0;
			 disciplina = v.getAlunoDemanda()->demanda->disciplina;

			// Pula disciplina original pratica
			if ( disciplina->getId() < 0 )
				continue;
			
			// Pula disciplina pratica equivalente
			 if (v.getDisciplina()->getId() < 0 )
				continue;
		 }

		 c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_DEMANDA_EQUIV_ALUNO );
         c.setDisciplina( disciplina );
         c.setAluno( v.getAluno() );
		 
         cit = cHashTatico.find(c);		 
         if ( cit == cHashTatico.end() )
         {
            nnz = 10;
            double rhs = 1.0;

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::EQUAL, rhs, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
	  }
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}


int EstimaTurmas::criaRestricaoAtendeDemandasComAluno( int campusId )
{
   int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
   {		
      VariableEstimaTurmas v = vit->first;

      if ( v.getType() == VariableEstimaTurmas::V_EST_NUM_ATEND ||
		   v.getType() == VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC )
	  {         
		 double coef = 0.0;
		 if( v.getType() == VariableEstimaTurmas::V_EST_NUM_ATEND )
		 {
			 coef = 1.0;
		 }
		 else if ( v.getType() == VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC )
		 {
			 coef = -1.0;
		 }

		 c.reset();
         c.setType( ConstraintEstimaTurmas::C_EST_ATEND_DEMANDA );
         c.setDisciplina( v.getDisciplina() );
         c.setTurno( v.getTurno() );
		 
         cit = cHashTatico.find(c);		 
         if ( cit == cHashTatico.end() )
         {
            nnz = 200;
            double rhs = 0.0;

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::EQUAL, rhs, name );

            row.insert( vit->second, coef );

            cHashTatico[ c ] = lp->getNumRows();
            lp->addRow( row );
            numRest++;
         }
         else
         {
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(coef);
         }
	  }
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

// Restrição Distribui Creditos Entre as Salas
/*
	Para cada sala s e turno g:

	sum[d] tempo{d} * C{d} * x{d,s,g} = xcs1{s,g} + xcs2{s,g} + xcs3{s,g} + xcs4{s,g}
*/
int EstimaTurmas::criaRestricaoSomaCredSalaPorTurno( int campusId )
{
   int numRest = 0;
   char name[ 100 ];
   int nnz;

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   
   Disciplina * disciplina = NULL;

    vit = vHashTatico.begin();
 	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;

		double coef = 0.0;		
		Campus *cp = NULL;
		Unidade *u = NULL;

		if( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
		{			
			coef = v.getDisciplina()->getTempoCredSemanaLetiva() * v.getDisciplina()->getTotalCreditos();				
			int unidId = v.getSubCjtSala()->salas.begin()->second->getIdUnidade();
			u = problemData->refUnidade[unidId];
			cp = problemData->refCampus[u->getIdCampus()];
		}
		else
		{
			if( v.getType() == VariableEstimaTurmas::V_EST_CRED_SALA_F1 )
			{			
				coef = -1.0;
			}
			else if( v.getType() == VariableEstimaTurmas::V_EST_CRED_SALA_F2 )
			{			
				coef = -1.0;
			}
			else if( v.getType() == VariableEstimaTurmas::V_EST_CRED_SALA_F3 )
			{			
				coef = -1.0;
			}
			else if( v.getType() == VariableEstimaTurmas::V_EST_CRED_SALA_F4 )
			{			
				coef = -1.0;
			}
			else
			{
				continue;
			}
			cp = v.getCampus();
			u = v.getUnidade();
		}

		ConjuntoSala *cjtSala = v.getSubCjtSala();
		int turno = v.getTurno();

		// --------------------------------------------------------
		c.reset();
		c.setType( ConstraintEstimaTurmas::C_EST_SOMA_CRED_SALA );
		c.setCampus( cp );
		c.setUnidade( u );
		c.setSubCjtSala( cjtSala );
		c.setTurno( turno );

		cit = cHashTatico.find(c);
		if(cit != cHashTatico.end())
		{
			lp->chgCoef(cit->second, vit->second, coef);
		}
		else
		{
			int nnz = 200;

			sprintf( name, "%s", c.toString().c_str() ); 
			OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

			row.insert(vit->second, coef);

			cHashTatico[ c ] = lp->getNumRows();

			lp->addRow( row );
			numRest++;
		}

	}

	return numRest;
}


int EstimaTurmas::criaRestricaoNroSalasPorAluno( int campusId )
{
   int numRest = 0;
   char name[ 100 ];
   int nnz;

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   
   Disciplina * disciplina = NULL;

    vit = vHashTatico.begin();
 	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;

		double coef = 0.0;

		if( v.getType() == VariableEstimaTurmas::V_EST_ALUNO_SALA ) // as
		{			
			coef = v.getAluno()->demandas.size() * 2; // bigM
		}
		else if( v.getType() == VariableEstimaTurmas::V_EST_ALOCA_ALUNO_DISC ) // s
		{					
			coef = -1.0;			
		}
		else
		{
			continue;
		}

		ConjuntoSala *cjtSala = v.getSubCjtSala();

		// --------------------------------------------------------
		c.reset();
		c.setType( ConstraintEstimaTurmas::C_EST_ALUNO_SALA );		
		c.setAluno( v.getAluno() );
		c.setSubCjtSala( v.getSubCjtSala() );
		
		cit = cHashTatico.find(c);
		if(cit != cHashTatico.end())
		{
			lp->chgCoef(cit->second, vit->second, coef);
		}
		else
		{
			int nnz = 10;

			sprintf( name, "%s", c.toString().c_str() ); 
			OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

			row.insert(vit->second, coef);

			cHashTatico[ c ] = lp->getNumRows();

			lp->addRow( row );
			numRest++;
		}

	}

	return numRest;
}

int EstimaTurmas::criaRestricaoNroTurmasPratTeorIgual( int campusId )
{
	int numRest=0;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;

   vit = vHashTatico.begin();

	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;
	
		if( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
		{			
			
			Disciplina *disciplinaPratica = NULL;

			// Só para disciplinas praticas/teoricas
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{			
				std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
				if ( itMapDisc == problemData->refDisciplinas.end() )
					continue;
				else
					disciplinaPratica = itMapDisc->second;
			}
			else
			{
				disciplinaPratica = v.getDisciplina();
			}
		
			c.reset();
			c.setType( ConstraintEstimaTurmas::C_EST_NRO_TURMAS_DISC_PRAT_TEOR_IGUAL );
			c.setDisciplina( disciplinaPratica );
			
			double coef = 0.0;
			if ( v.getDisciplina()->getId() < 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz = 200;

				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				numRest++;
			}
		}
	}

	return numRest;
}

int EstimaTurmas::criaRestricaoUsaDisciplina( int campusId )
{
   int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariableEstimaTurmas v;
   ConstraintEstimaTurmas c;
   VariableEstimaTurmasHash::iterator vit;
   ConstraintEstimaTurmasHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableEstimaTurmas v = vit->first;

		if( v.getType() == VariableEstimaTurmas::V_EST_NUM_TURMAS )
		{
			 c.reset();
			 c.setType( ConstraintEstimaTurmas::C_EST_USA_DISC );
			 c.setDisciplina(v.getDisciplina());
			 			 
			 double coef = -1.0;

			 cit = cHashTatico.find(c);
			 if ( cit == cHashTatico.end() )
			 {
				nnz = v.getDisciplina()->cjtSalasAssociados.size() * 3 + 1;

				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
		}
		else if( v.getType() == VariableEstimaTurmas::V_EST_USA_DISCIPLINA )
		{
			 c.reset();
			 c.setType( ConstraintEstimaTurmas::C_EST_USA_DISC );
			 c.setDisciplina(v.getDisciplina());
			 			 
			 double coef = 100.0;	// big M

			 cit = cHashTatico.find(c);
			 if ( cit == cHashTatico.end() )
			 {			
				nnz = v.getDisciplina()->cjtSalasAssociados.size() * 3 + 1;

				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
		}
   }

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}
