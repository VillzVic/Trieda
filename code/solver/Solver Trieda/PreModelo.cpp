#include "PreModelo.h"

using namespace std;


bool FIXA_SALAS_ESTIMATURMAS = false;
bool PREMODELO_POR_TURNO = true;
bool PREMODELO_SEM_VAR_N = true;


PreModelo::PreModelo( ProblemData * aProblemData, 
				GGroup< VariablePre *, LessPtr<VariablePre> > *aSolVarsXPre,
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *avars_xh,
				bool *endCARREGA_SOLUCAO )
				: Solver( aProblemData )
{
   pSolVarsXPre = aSolVarsXPre;
   
   vars_xh = *avars_xh;

   CARREGA_SOLUCAO = endCARREGA_SOLUCAO;

   USAR_EQUIVALENCIA = problemData->parametros->considerar_equivalencia_por_aluno;
   NAO_CRIAR_RESTRICOES_CJT_ANTERIORES = true;
   FIXAR_P1 = true;
   
   if ( FIXA_SALAS_ESTIMATURMAS )
	   std::cout << "\nFIXA_SALAS_ESTIMATURMAS";  
   if ( PREMODELO_POR_TURNO )
	   std::cout << "\nPREMODELO_POR_TURNO";

   TEMPO_PRETATICO = 3600;
   PERMITIR_INSERCAO_ALUNODEMANDAP2_EM_TURMAP1 = false;

#ifdef USA_ESTIMA_TURMAS
   MODELO_ESTIMA_TURMAS = true;
#else
   MODELO_ESTIMA_TURMAS = false;
#endif

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

PreModelo::~PreModelo()
{
   if ( lp != NULL )
   {
      delete lp;
   }   
}


void PreModelo::getSolution( ProblemSolution * problem_solution ){}

int PreModelo::solve(){return 1;}


void PreModelo::chgCoeffList(
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

int PreModelo::retornaTempoDeExecucaoPreModelo( int campusId, int cjtAlunosId, int prioridade )
{
	if ( campusId == 19 ) // campus 2, mais dificil
	{
		return TEMPO_PRETATICO*3;
	}
	else if ( campusId == 20 ) // campus 28, mais facil
	{
		return TEMPO_PRETATICO*3;	
	}
	else // geral
	{
		return TEMPO_PRETATICO*3;	
	}
}


std::string PreModelo::getEquivPreFileName( int campusId, int prioridade, int r )
{
	std::string solName;
	solName += "EquivPreModelo";
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

   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }
     
   solName += ".txt";
	 
   return solName;
}


std::string PreModelo::getPreLpFileName( int campusId, int prioridade, int cjtAlunosId, int r )
{
	std::string solName;
	solName += "SolverTriedaPreTatico";
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

   if ( cjtAlunosId != 0 )
   {
	    stringstream ss;
		ss << cjtAlunosId;		
		solName += "_Cjt"; 
		solName += ss.str();   		
   }

   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }
         
   return solName;
}

std::string PreModelo::getSolPreBinFileName( int campusId, int prioridade, int cjtAlunosId, int r, int particao )
{
	std::string solName;
	solName += "solPreBin";
	solName +=problemData->inputIdToString();

   if ( particao != 0 )
   {
		stringstream ss;
		ss << particao;
		solName += "_Part"; 
		solName += ss.str();   		
   }

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

   if ( cjtAlunosId != 0 )
   {
	    stringstream ss;
		ss << cjtAlunosId;		
		solName += "_Cjt"; 
		solName += ss.str();   		
   }

   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }

   solName += ".bin";
      
   return solName;
}

std::string PreModelo::getSolucaoPreTaticoFileName( int campusId, int prioridade, int cjtAlunosId, int r, int particao )
{
	std::string solName;
	solName += "solucaoPreTatico";
	solName +=problemData->inputIdToString();
	
   if ( particao != 0 )
   {
		stringstream ss;
		ss << particao;
		solName += "_Part"; 
		solName += ss.str();   		
   }

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

   if ( cjtAlunosId != 0 )
   {
	    stringstream ss;
		ss << cjtAlunosId;		
		solName += "_Cjt"; 
		solName += ss.str();   		
   }

   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }

   solName += ".txt";
      
   return solName;
}

std::string PreModelo::getSolVarsPreFileName( int campusId, int prioridade, int cjtAlunosId, int particao )
{
	std::string solName;
	solName += "solVarsPre";
	solName +=problemData->inputIdToString();

   if ( particao != 0 )
   {
		stringstream ss;
		ss << particao;
		solName += "_Part"; 
		solName += ss.str();   		
   }

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

   if ( cjtAlunosId != 0 )
   {
	    stringstream ss;
		ss << cjtAlunosId;		
		solName += "_Cjt"; 
		solName += ss.str();   		
   }

   solName += ".txt";
      
   return solName;
}

int PreModelo::readSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type, double *xSol )
{
	char solName[1024]="\0";

	switch (type)
	{
		case (PRE_TAT):
			strcpy( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT_SAB_ZERO):
			strcpy( solName, "sabZero" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
		case (PRE_TAT1):
			strcpy( solName, "1" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT2):
			strcpy( solName, "2" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT3):
			strcpy( solName, "3" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT4):
			strcpy( solName, "4" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT_INIT):
			strcpy( solName, "i" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
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
		std::cout << "\nErro em readSolBin(int campusAtualId, int prioridade, int cjtAlunos, int r): "
					<< " \nNumero diferente de variaveis: " << nCols << " != " << nroColsLP; fflush(NULL);
		return (0);
	}
	fclose(fin);
	
	return (1);
}

void PreModelo::writeSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type, double *xSol )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (PRE_TAT):
			strcpy( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT_SAB_ZERO):
			strcpy( solName, "sabZero" );
			strcpy( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT1):
			strcpy( solName, "1" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT2):
			strcpy( solName, "2" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT3):
			strcpy( solName, "3" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT4):
			strcpy( solName, "4" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT_INIT):
			strcpy( solName, "i" );
			strcat( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	FILE * fout = fopen( solName, "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em PreModelo::writeSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type ):"
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

void PreModelo::writeSolTxt( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type, double *xSol )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (PRE_TAT):
			strcpy( solName, getSolucaoPreTaticoFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT_SAB_ZERO):
			strcpy( solName, "sabZero" );
			strcat( solName, getSolucaoPreTaticoFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT1):
			strcpy( solName, "1" );
			strcat( solName, getSolucaoPreTaticoFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT2):
			strcpy( solName, "2" );
			strcat( solName, getSolucaoPreTaticoFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT3):
			strcpy( solName, "3" );
			strcat( solName, getSolucaoPreTaticoFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT4):
			strcpy( solName, "4" );
			strcat( solName, getSolucaoPreTaticoFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
		case (PRE_TAT_INIT):
			strcpy( solName, "i" );
			strcat( solName, getSolucaoPreTaticoFileName( campusId, prioridade, cjtAlunosId, r, particao ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	ofstream fout( solName, ios_base::out );
	if ( fout == NULL )
	{
		std::cout << "\nErro em PreModelo::writeSolTxt( int campusId, int prioridade, int cjtAlunosId, int r, int tatico,int particao, int type ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{		
		int nAtend = 0, nAtendSemPT = 0;

		VariablePreHash::iterator vit = vHashPre.begin();
		while ( vit != vHashPre.end() )
		{
			VariablePre v = vit->first;
			int col = vit->second;
			double value = (int)( xSol[ col ] + 0.5 );
		 
			fout << v.toString() << " = " << value << endl;
				  
			if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
			{
				nAtend += value;
				if ( v.getDisciplina()->getId() > 0 )
				nAtendSemPT += value;
			}

			vit++;
		}
		fout << "\n\nTotal de AlunoDemanda atendidos = " << nAtend << endl;
		fout << "\n\nTotal de AlunoDemanda atendidos sem divisao PT = " << nAtendSemPT << endl;

		fout.close();
	}
}

void PreModelo::writeSolTxt( string solName, double *xSol, bool printZeros )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	// WRITES SOLUTION
		
	ofstream fout( solName.c_str(), ios_base::out );
	if ( fout == NULL )
	{
		std::cout << "\nErro em PreModelo::writeSolTxt( char* solName, double *xSol ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{		
		int nAtend = 0, nAtendSemPT = 0;
		int nTurmas = 0, nTurmasSemPT = 0;

		VariablePreHash::iterator vit = vHashPre.begin();
		while ( vit != vHashPre.end() )
		{
			VariablePre v = vit->first;
			int col = vit->second;
			double value = (int)( xSol[ col ] + 0.5 );
		  
			if ( value != 0 || printZeros )
				fout << v.toString() << " = " << value << endl;
			
			if ( value > 0 )
			{
				if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
				{
					nAtend++;
					if ( v.getDisciplina()->getId() > 0 )
						nAtendSemPT++;
				}
				if ( vit->first.getType() == VariablePre::V_PRE_ABERTURA )
				{
					VariablePre v = vit->first;
					int value = (int)(xSol[vit->second]+0.5);
					
					nTurmas++;
					if ( v.getDisciplina()->getId() > 0 )
						nTurmasSemPT++;
				}
			}

			vit++;
		}
		fout << "\n\nNro turmas abertas sem P = " << nTurmasSemPT << endl;
		fout << "Total de AlunoDemanda atendidos sem P = " << nAtendSemPT << endl;
		fout << "\nNro turmas abertas com P = " << nTurmas << endl;
		fout << "Total de AlunoDemanda atendidos com P= " << nAtend;

		fout.close();
	}
}

void PreModelo::writeSolBin( string solName, double *xSol )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	// WRITES SOLUTION
		
	FILE * fout = fopen( solName.c_str(), "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em PreModelo::writeSolBin( string solName, double *xSol ):"
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


void PreModelo::sincronizaSolucao()
{
   // --------------------------------------------------------------------
   // insere as variaveis x na lista referenciada por pSolVarsXPre que contém a solução até o momento.
	
	ITERA_GGROUP_LESSPTR ( itVar, this->solVarsXPre, VariablePre )
	{
		VariablePre *v = *itVar;

		if ( (*pSolVarsXPre).find( v ) == (*pSolVarsXPre).end() )
			pSolVarsXPre->add( v );
		else
			delete v;
	}   
   

   // ----------------------------------------------------
   // delete this->solVarsPreS

	ITERA_GGROUP_LESSPTR ( itVar, this->solVarsPreS, VariablePre )
	{
		delete *itVar;
	}
	this->solVarsPreS.clear();

   // ----------------------------------------------------

   return;
}

void PreModelo::testeCarregaPreSol( int campusId, int prioridade, int cjtAlunosId, int r )
{
	double * xSol = NULL;

	lp->updateLP();
	int nroColsLP = lp->getNumCols();
	xSol = new double[ nroColsLP ];
   
	char solName[1024];

	strcpy( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, 0 ).c_str() );
	cout<<"====================> TESTE => carregando pre " <<solName <<endl;
	FILE* fin = fopen( solName,"rb");
	if ( fin == NULL )
	{
		std::cout << "\nErro em PreModelo::testeCarregaPreSol(int campusId, int prioridade, int cjtAlunosId, int r): arquivo "
					<< solName << " nao encontrado.\n"; fflush(0);
		return;
	}

	int nCols = 0;
	fread(&nCols,sizeof(int),1,fin);

	if ( nCols == nroColsLP )
	{
		for (int i =0; i < nCols; i++)
		{
			double auxDbl;
			fread(&auxDbl,sizeof(double),1,fin);
			xSol[i] = auxDbl;
		}
	}
	else
	{
		std::cout << "\nErro em testeCarregaPreSol(int campusAtualId, int prioridade, int cjtAlunos, int r): "
					<< " \nNumero diferente de variaveis: " << nCols << " != " << nroColsLP; fflush(0);
		return;
	}
	fclose(fin);
	
	int nChg = 0;
	int *idx = new int[lp->getNumCols()*2];
	double *valOrigUB = new double[lp->getNumCols()*2];
	double *valOrigLB = new double[lp->getNumCols()*2];
	double *valUB = new double[lp->getNumCols()*2];
	double *valLB = new double[lp->getNumCols()*2];
	BOUNDTYPE *bts1 = new BOUNDTYPE[lp->getNumCols()*2];
	BOUNDTYPE *bts2 = new BOUNDTYPE[lp->getNumCols()*2];
	
	std::cout << "\nCalculating change of bounds..."; fflush(0);

    VariablePreHash::iterator vit;
	for ( vit = vHashPre.begin(); vit != vHashPre.end() ; vit++ )
	{
		int col = vit->second;
		double ub = xSol[ col ];
		double lb = xSol[ col ];
		
		if ( (int)( ub + 0.5 ) == 1 && 
			(vit->first.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ||
			 vit->first.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO) )
		{
			idx[nChg] = col;
			valOrigUB[nChg] = lp->getUB(col);
			valOrigLB[nChg] = lp->getLB(col);
			valUB[nChg] = ub + 1e-5;
			valLB[nChg] = lb - 1e-5;
			bts1[nChg] = BOUNDTYPE::BOUND_UPPER;
			bts2[nChg] = BOUNDTYPE::BOUND_LOWER;

			nChg++;
		}
	}
	 
	std::cout << "\nChanging bounds..."; fflush(0);

	lp->chgBds(nChg,idx,bts1,valUB);
	lp->chgBds(nChg,idx,bts2,valLB);
	lp->updateLP();

	std::cout << " bounds changed!\n"; fflush(0);

	lp->setTimeLimit( 1e10 );
	lp->setMIPRelTol( 0.01 );
	lp->setPreSolve(OPT_TRUE);
	lp->setHeurFrequency(1.0);
	lp->setMIPEmphasis(0);
	lp->setSymetry(0);
	lp->setMIPScreenLog( 4 );
	lp->setNumIntSols(1);

	lp->updateLP();
	
	char lpName[1024];
    strcpy( lpName, getPreLpFileName( campusId, prioridade, cjtAlunosId, r ).c_str() );
	strcat( lpName, "_teste" );


	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName );
	#endif

	std::cout << "\nOptimizing..."; fflush(0);

	int status = lp->optimize( METHOD_MIP );	

	std::cout << " finish!"; fflush(0);

	lp->chgBds(nChg,idx,bts1,valOrigUB);
	lp->chgBds(nChg,idx,bts2,valOrigLB);
	lp->updateLP();

}


/*
	Retorna o valor minimo a ser assumido pela variavel em sua criação.
	Função usada nas rodadas de prioridade maior que 1, a fim de considerar
	a otimização do tatico COM horários nas iterações anteriores.
*/
int PreModelo::fixaLimitesVariavelPre( VariablePre *v )
{	   
	switch( v->getType() )
	{
		
		 case VariablePre::V_ERROR:
		 {
			 return true;
		 }
		 case VariablePre::V_PRE_CREDITOS:  //  x_{i,d,u,s} 
		 {
			 int nCreditos = 0;

			 // x_{i,d,u,s,hi,hf,t}
			 ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getTurma() == v->getTurma() &&
					  vSol->getDisciplina() == v->getDisciplina() &&
					  vSol->getUnidade() == v->getUnidade() &&
					  vSol->getSubCjtSala() == v->getSubCjtSala() )
				 {
					 HorarioAula *hi = vSol->getHorarioAulaInicial();
					 HorarioAula *hf = vSol->getHorarioAulaFinal();
					 Calendario *sl = hi->getCalendario();
					 int nCreds = sl->retornaNroCreditosEntreHorarios( hi, hf );
					 nCreditos += nCreds;
				 }				
			 }
			 return nCreditos;
		 }
		 case VariablePre::V_PRE_OFERECIMENTO:  //  o_{i,d,u,s}
		 {
			 // x_{i,d,u,s,hi,hf,t}
			 ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getTurma() == v->getTurma() &&
					  vSol->getDisciplina() == v->getDisciplina() &&
					  vSol->getUnidade() == v->getUnidade() &&
					  vSol->getSubCjtSala() == v->getSubCjtSala() )
					  return 1;
			 }

			 return 0;
		 }
		case VariablePre::V_PRE_ABERTURA: // z_{i,d,cp}
		 {
			 // x_{i,d,u,s,t}
			 ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getTurma() == v->getTurma() &&
					  vSol->getDisciplina() == v->getDisciplina() &&
					  problemData->retornaCampus( vSol->getUnidade()->getId() ) == v->getCampus() )
					  return 1;
			 }

			 return 0;
		 }
		 case VariablePre::V_PRE_ALOC_ALUNO:  //  b_{i,d,c}
		 {
			 Curso *curso = v->getCurso();

			 ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
			 {
				 if ( itOft->curso == curso )
				 {
					 int ofertaId = itOft->getId();
					 int discId = v->getDisciplina()->getId();
					 int turma = v->getTurma();

					 if ( problemData->atendeTurmaDiscOferta( turma, discId, ofertaId ) )
					 {
						return 1;
					 } 					
				 }
			 }
			 return 0;
		 }
		 case VariablePre::V_PRE_SLACK_DEMANDA: // fd_{d,a}
		 {
			 return 0;
		 } 		 
		 case VariablePre::V_PRE_SLACK_COMPARTILHAMENTO:  // fc_{i,d,c1,c2}
		 {
			 Curso* curso1 = v->getParCursos().first;
			 Curso* curso2 = v->getParCursos().second;
			 int discId = v->getDisciplina()->getId();
			 int turma = v->getTurma();

			 ITERA_GGROUP_LESSPTR( itOft1, problemData->ofertas, Oferta )
			 {
				 if ( itOft1->curso == curso1 )
				 {
					int ofertaId = itOft1->getId();

					if ( problemData->atendeTurmaDiscOferta( turma, discId, ofertaId ) )
					{
						 ITERA_GGROUP_INIC_LESSPTR( itOft2, itOft1, problemData->ofertas, Oferta )
						 {
							 if ( itOft2->curso == curso2 )
							 {
								 int ofertaId = itOft2->getId();

								 if ( problemData->atendeTurmaDiscOferta( turma, discId, ofertaId ) )
								 {
									return 1;
								 } 
							 }
						 }
					} 
				 }
			 }

			 return 0;
		 }		 
		 case VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC:  // s_{a,i,d,cp}
		 {
			 Aluno* aluno = v->getAluno();
			 int campusId = v->getCampus()->getId();
			 Disciplina *disciplina = v->getDisciplina();
			 int turma = v->getTurma();

		 	 std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator 
				 itMap = problemData->mapAluno_CampusTurmaDisc.find( aluno );
			 
			 if ( itMap == problemData->mapAluno_CampusTurmaDisc.end() )
				 return 0;

			 Trio< int, int, Disciplina* > trio;
			 trio.set( campusId, turma, disciplina );

			 GGroup< Trio< int , int, Disciplina* > >::iterator itGGroup =
				 itMap->second.find( trio );
			 
			 if ( itGGroup != itMap->second.end() )
			 {
				  return 1;
			 }

			 return 0;
		 }
		 case VariablePre::V_PRE_FORMANDOS_NA_TURMA: // f_{i,d,cp}
		 {
			 int campusId = v->getCampus()->getId();
			 Disciplina *disciplina = v->getDisciplina();
			 int turma = v->getTurma();

			 Trio< int, int, Disciplina* > trio;
			 trio.set( campusId, turma, disciplina );

			 std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, 
				 GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
				 itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
			 		 	 
			 if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
				 return 0;
			 
			 ITERA_GGROUP_LESSPTR( itAlDem, itMap->second, AlunoDemanda )
			 {
				 Aluno *aluno = problemData->retornaAluno( (*itAlDem)->getAlunoId() );
				 if ( aluno->ehFormando() )
				 {
					return 1;
				 }
			 }

			 return 0;		 
		 }		 

		 default:
		 {
			return 0;
			break;
		 }
	}

}

void PreModelo::carregaVariaveisSolucaoPreTatico_CjtAlunos( int campusId, int prioridade, int cjtAlunos, int r )
{
	//testeCarregaPreSol( campusId, prioridade, cjtAlunos, r );

   double * xSol = NULL;
   VariablePreHash::iterator vit;
   
   lp->updateLP();
   int nroColsLP = lp->getNumCols();
   xSol = new double[ nroColsLP ];
   
   
   std::cout << "\n\nCarregando solucao do pre-modelo...\n";

   if ( *(this->CARREGA_SOLUCAO) )
   {
	   int status = readSolBin( campusId, prioridade, cjtAlunos, r, 0, 0, OptimizStep::PRE_TAT, xSol );
	   if ( !status )
	   {
		   std::cout << "\nErro em PreModelo::carregaVariaveisSolucaoPreTatico_CjtAlunos( int campusId, int prioridade, int cjtAlunos, int r ): arquivo nao encontrado.\n";
		   exit(0);
	   }
   }
   else
   {
	   lp->getX( xSol );
   }

   
   char solFilename[1024];
   strcpy( solFilename, getSolucaoPreTaticoFileName( campusId, prioridade, cjtAlunos, r, 0 ).c_str() );

   FILE * fout = fopen( solFilename, "wt" );

   if ( fout == NULL )
   {
	   std::cout << "\nErro em PreModelo::carregaVariaveisSolucaoPreTatico_CjtAlunos( int campusAtualId, int prioridade, int cjtAlunos, int r )"
				 << "\nArquivo " << solFilename << " nao pode ser aberto\n";
	   fout = fopen( "solPreSubstituto.txt", "wt" );
	   if ( fout == NULL )
	   {
			std::cout <<"\nErro de novo. Finalizando execucao...\n";
			exit(0);
	   }
   }

   problemData->mapCreditosSalas.clear();
   int nroNaoAtendimentoAlunoDemanda = 0;
   int nroAtendimentoAlunoDemanda = 0;

   vit = vHashPre.begin();
   while ( vit != vHashPre.end() )
   {
	  double value = xSol[ vit->second ];

      if ( value > 0.00001 )
      {
		  VariablePre * v = new VariablePre( vit->first );
		  int col = vit->second;
		  v->setValue( xSol[ col ] );
		           
		 char auxName[100];
         lp->getColName( auxName, col, 100 );
         fprintf( fout, "%s = %f\n", auxName, v->getValue() );

         switch( v->getType() )
         {
			 case VariablePre::V_ERROR:
				std::cout << "Variável inválida " << std::endl;
				break;
			 case VariablePre::V_PRE_CREDITOS:
				//std::cout << "Oferta de " << v->getValue()
				//		  << " creditos da disciplina " << v->getDisciplina()->getCodigo()
				//		  << " para a turma " << v->getTurma()
				//		  << " para alguma de sala do conjunto de salas " << v->getSubCjtSala()->getId()
				//		  << std::endl << std::endl;
				problemData->mapCreditosSalas[v->getSubCjtSala()] += ((int)(xSol[ col ] + 0.5)) * ((int)(v->getDisciplina()->getTempoCredSemanaLetiva()+0.5));
				solVarsXPre.add( v );
				break;			
			 case VariablePre::V_PRE_OFERECIMENTO:
				#ifdef SALA_UNICA_POR_TURMA
				  //std::cout << "Oferta da disciplina " << v->getDisciplina()->getCodigo()
						//	<< " para a turma " << v->getTurma()
						//	<< " para alguma de sala do conjunto de salas " << v->getSubCjtSala()->getId()
						//	<< std::endl << std::endl;
				 
				 if ( problemData->mapCreditosSalas.find(v->getSubCjtSala()) == problemData->mapCreditosSalas.end() )
					  problemData->mapCreditosSalas[v->getSubCjtSala()] = v->getDisciplina()->getTotalTempo();
				 else problemData->mapCreditosSalas[v->getSubCjtSala()] += v->getDisciplina()->getTotalTempo();				  
				  
				  solVarsXPre.add( v );
				#endif
				break;
			 case VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC: 
				 this->preencheMapAtendimentoAluno( v );
				 nroAtendimentoAlunoDemanda++;
				 solVarsPreS.add( v );
				 break;
			 case VariablePre::V_PRE_SLACK_DEMANDA:				 				 
				 // ------------ Preenche listSlackDemandaAluno ----------------
				 if ( problemData->parametros->otimizarPor == "ALUNO" )
				 {
					 Disciplina *d = v->getDisciplina();
					 Aluno *aluno = v->getAluno();

					 AlunoDemanda *ad = problemData->procuraAlunoDemanda( d->getId(), aluno->getAlunoId(), prioridade );

					 problemData->listSlackDemandaAluno.add( ad );		 
					 
					 nroNaoAtendimentoAlunoDemanda++;		 					 
				 }
				 // ------------------------------------------------------------
				 break; 
         }

		 if ( v->getType() != VariablePre::V_PRE_CREDITOS && 
			  v->getType() != VariablePre::V_PRE_OFERECIMENTO && 
			  v->getType() != VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		 {
			 delete v;
		 }
		 else if ( v->getType() == VariablePre::V_PRE_OFERECIMENTO )
		 {
			#ifndef SALA_UNICA_POR_TURMA
			   delete v;
			#endif			 
		 }
	 
      }

      vit++;
   }
   
   fprintf( fout, "\nAlunosDemanda nao atendidos = %d\n", nroNaoAtendimentoAlunoDemanda );
   fprintf( fout, "AlunosDemanda atendidos = %d", nroAtendimentoAlunoDemanda );

   if ( fout )
   {
      fclose( fout );
   }
      
   if ( vHashPre.size() > 0 )
   {
	   vHashPre.clear();
   }
   if ( cHashPre.size() > 0 )
   {
	   cHashPre.clear();
   }

	std::cout<<"\nImprime utilizacao das salas...\n"; fflush(NULL);
	problemData->imprimeUtilizacaoSala( campusId, prioridade, cjtAlunos, false, r, toString(PRE_TAT) );

   if ( xSol )
   {
      delete [] xSol;
   }
      
   bool violou = problemData->violaMinTurma( campusId );
   
   std::cout << "\nSolucao do pre-modelo carregada com sucesso!\n";
   std::cout << "\n-----------------------------------------------------------------\n";
   std::cout << "-----------------------------------------------------------------\n\n";

   return;
}

void PreModelo::solvePreTaticoMain( int campusId, int prioridade, int cjtAlunosId, int r )
{
	std::cout<<"\n\n---------------------Rodada "<< r <<" -----------------------\n";
	std::cout<<"\n------- Campus "<< campusId << " , Conjunto-Aluno "<< cjtAlunosId << ", Prior " << prioridade << "----------\n";
	std::cout<<"\n------------------------------Pre-modelo------------------------------\n"; fflush(NULL);
				
	CPUTimer timer;
	timer.start();

	int statusPre = solvePreTaticoCjtAlunos( campusId, prioridade, cjtAlunosId, r );    

	//int statusPre = solvePreTaticoCjtAlunos_testeFases( campusId, prioridade, cjtAlunosId, r );    

	carregaVariaveisSolucaoPreTatico_CjtAlunos( campusId, prioridade, cjtAlunosId, r );

	timer.stop();
	double runtime = timer.getCronoCurrSecs();
												
	problemData->preencheMapsTurmasTurnosIES();
						
	problemData->imprimeAlocacaoAlunos( campusId, prioridade, cjtAlunosId, false, r, toString(PRE_TAT), runtime );

	#ifdef EQUIVALENCIA_DESDE_O_INICIO
		this->atualizarDemandasEquiv( campusId, prioridade, r );
	#endif

	sincronizaSolucao();

	problemData->imprimeDiscNTurmas( campusId, prioridade, cjtAlunosId, false, r, 0 );

}


int PreModelo::solvePreTaticoCjtAlunos( int campusId, int prioridade, int cjtAlunosId, int r )
{
	int status = 0;
	
	bool CARREGA_SOL_PARCIAL = *( this->CARREGA_SOLUCAO );

	bool CARREGOU_HEURISTICA = false;

   if ( * this->CARREGA_SOLUCAO )
   {
#ifdef HEURISTICA_P2
		if ( leSolucaoHeuritica( campusId, prioridade, cjtAlunosId ) )
		{
			CARREGOU_HEURISTICA = true;		
			problemData->imprimeAlocacaoAlunos( campusId, prioridade, cjtAlunosId, true, r, toString(PRE_TAT), 0.0 );
			problemData->imprimeDiscNTurmas( campusId, prioridade, cjtAlunosId, true, r, 0 );
		}
#endif
	   char solName[1024];
	   strcpy( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, 0 ).c_str() );
	   FILE* fin = fopen( solName,"rb");
	   if ( fin == NULL )
	   {
		   std::cout << "\nA partir de " << solName << " , nao foram lidas mais solucoes.\n"; fflush(NULL);
		   *(this->CARREGA_SOLUCAO) = false;
	   }
	   else
	   {
		   fclose(fin);
	   }
   }

#ifdef HEURISTICA_P2
   // --------------------------------------------------------
   // HEURISTICA

   // Aloca alunos de prioridade 2 em turmas já existentes
   if ( ! (*this->CARREGA_SOLUCAO) && !CARREGOU_HEURISTICA && prioridade > 1 )
   {
   	CPUTimer timer;
	timer.start();

#ifndef TATICO_COM_HORARIOS
	   this->heuristicaAlocaAlunos( campusId, prioridade, cjtAlunosId );
#else
	   this->heuristica2AlocaAlunos( campusId, prioridade, cjtAlunosId );
#endif

	timer.stop();
	double runtime = timer.getCronoCurrSecs();

	   problemData->imprimeAlocacaoAlunos( campusId, prioridade, cjtAlunosId, true, r, toString(PRE_TAT), runtime );
	   problemData->imprimeDiscNTurmas( campusId, prioridade, cjtAlunosId, true, r, 0 );
	   this->imprimeAlocacaoFinalHeuristica( campusId, prioridade, cjtAlunosId );

	   escreveSolucaoBinHeuristica( campusId, prioridade, cjtAlunosId );	   
   
	   CARREGA_SOL_PARCIAL=false;
   }
   // --------------------------------------------------------
#endif
      
   if ( prioridade>1 )
   {
	   problemData->removeDemandasEmExcesso( campusId, prioridade, cjtAlunosId );
	   problemData->reduzNroTurmasPorDisciplina( campusId, prioridade, prioridade*r );
   }

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
   
   if ( vHashPre.size() > 0 )
   {
	   vHashPre.clear();
   }
   if ( cHashPre.size() > 0 )
   {
	   cHashPre.clear();
   }

   int varNum = 0;
   int constNum = 0;

   char lpName[1024], id[100];
   strcpy( lpName, getPreLpFileName( campusId, prioridade, cjtAlunosId, r ).c_str() );

   if ( problemData->parametros->funcao_objetivo == 0
      || problemData->parametros->funcao_objetivo == 2 )
   {
      lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
   }
   else if( problemData->parametros->funcao_objetivo == 1 )
   {
      lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
   }

   printf( "Creating LP...\n" );

   // Variable creation
   varNum = cria_preVariaveis( campusId, prioridade, cjtAlunosId, r );

   lp->updateLP();
      
   printf( "Total of Variables: %i\n\n", varNum );
      
	// -----------------------------------------------------
	// Deleta as variaveis em solVarsXPre 
	ITERA_GGROUP_LESSPTR ( it, solVarsXPre, VariablePre )
	{
		delete *it;    
	}
	solVarsXPre.clear();
    // --------------------------------------------

   if ( ! (*this->CARREGA_SOLUCAO) )
   {
	    ofstream outGaps;
		std::string gapFilename( "gap_input" );
		gapFilename += problemData->getInputFileName();
		gapFilename += ".txt";	

	    // Constraint creation
	    constNum = cria_preRestricoes( campusId, prioridade, cjtAlunosId, r );

	    lp->updateLP();

	    printf( "Total of Constraints: %i\n\n", constNum );

		std::string lpName1;
		lpName1 += "1";
		lpName1 += string(lpName);

		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName1.c_str() );
		#endif	

		// Save the original objetive function
		double *objOrig = new double[lp->getNumCols()];
		lp->getObj(0,lp->getNumCols()-1,objOrig);
				
		// Index of all variables
		int *idxN = new int[lp->getNumCols()];
		for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
		{
			idxN[vit->second] = vit->second;
		}


		bool statusSolInicial = false;
				
		if ( problemData->existeSolTaticoInicial() )
		{
			std::cout << "\n=========================================";
			std::cout << "\nUsando solucao inicial...\n"; fflush(NULL);
		
			int nChg = 0;
			int *idx = new int[lp->getNumCols()*2];
			double *values = new double[lp->getNumCols()*2];

			VariablePreHash::iterator vit;
			vit = vHashPre.begin();
			for ( ; vit != vHashPre.end(); vit++ )
			{
				VariablePre v = vit->first;
				if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
   				{
					int lb = (int)(lp->getLB(vit->second)+0.5);
					int ub = (int)(lp->getUB(vit->second)+0.5);
				
					if ( lb != ub && problemData->getSolTaticoInicial()->isInSol(v) )
					{
						idx[nChg] = vit->second;
						values[nChg] = 1.0;
						nChg++;
					}
				}
				else if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
   				{
					int lb = (int)(lp->getLB(vit->second)+0.5);
					int ub = (int)(lp->getUB(vit->second)+0.5);
				
					if ( lb != ub && problemData->getSolTaticoInicial()->isInSol(v) )
					{
						idx[nChg] = vit->second;
						values[nChg] = 1.0;
						nChg++;
					}
				}
			}

			lp->updateLP();	   
			lp->setAdvance(2);
            statusSolInicial = lp->copyMIPStartSol( nChg, idx, values );
            lp->updateLP();

			if ( !statusSolInicial )
				std::cout<<"\n\ncpySolInicial Status = error" << endl;
			else
				std::cout<<"\n\ncpySolInicial Status = successful" << endl;
			fflush(NULL);
			
			delete [] values;
			delete [] idx;
			// -------------------------------------------------------------------
		}
		
		double *xS = new double[lp->getNumCols()];
		VariablePreHash::iterator vit;

		if ( !statusSolInicial )
		{
			int *idxUB = new int[lp->getNumCols()*2];
			double *valUB = new double[lp->getNumCols()*2];
			BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()*2];
			int nChgUB = 0;

			// -------------------------------------------------------------------
			// Fixa o ub = 0 para todas as variaveis que possuem lb zero e são livres

			std::cout << "\n===================>>>>";
			std::cout << "\nGarantindo solucao...\n"; fflush(NULL);
		   
			std::map< int, double > vHashLivresOriginais;
			nChgUB = 0;

			vit = vHashPre.begin();
			for ( ; vit != vHashPre.end(); vit++ )
			{
				VariablePre v = vit->first;

				if ( v.getType() == VariablePre::V_PRE_ABERTURA )
   				{
					int lb = (int)(lp->getLB(vit->second)+0.5);
					int ub = (int)(lp->getUB(vit->second)+0.5);
				
					if ( lb == 0 && ub != 0 )
					{
						vHashLivresOriginais[ vit->second ] = lp->getUB(vit->second);

						idxUB[nChgUB] = vit->second;
						valUB[nChgUB] = 0.0;
						bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;
						nChgUB++;
					}
				}
			}

			lp->chgBds(nChgUB,idxUB,bts,valUB);
			// -------------------------------------------------------------------
			
			lp->updateLP();
	
#ifdef SOLVER_CPLEX
			lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT1) );
			lp->setMIPRelTol( 0.01 );
			lp->setPreSolve(OPT_TRUE);
			lp->setHeurFrequency(1.0);
			lp->setMIPEmphasis(0);
			lp->setSymetry(0);
			lp->setMIPScreenLog( 4 );
			lp->setNumIntSols(1);
			lp->updateLP();
#elif SOLVER_GUROBI
			lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT1) );
			lp->setMIPRelTol( 0.01 );
			lp->setPreSolveIntensity( OPT_LEVEL1 );
			lp->setHeurFrequency(1.0);
			lp->setMIPEmphasis(0);
			lp->setSymetry(0);
			lp->setMIPScreenLog( 4 );
			lp->setNumIntSols(1);
			
			#if defined SOLVER_GUROBI && defined USAR_CALLBACK
			cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::PRE_TAT1);
			cb_data.gapMax = 80;
			lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
			#endif

			lp->updateLP();
#endif
					

			// GENERATES SOLUTION 		 
			this->nroPreSolAvaliadas = 0;
			std::cout << "\n\nOptimize...\n\n";
			status = lp->optimize( METHOD_MIP );
			lp->getX(xS);
		
			writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT1, xS );
			writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT1, xS );


		#pragma region Imprime Gap
	 		// Imprime Gap
			outGaps.open(gapFilename, ofstream::app);
			if ( !outGaps )
			{
				std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::solvePreTaticoCjtAlunos().\n";
			}
			else
			{
				outGaps << "Pre-modelo (Garante Solucao) - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
				outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
				outGaps << "\n\n";
				outGaps.close();
			} 
		#pragma endregion
	    
			// -------------------------------------------------------------------
			// Volta as variaveis de abertura de turma z_{i,d,cp} que estavam livres
			nChgUB = 0;
         
			for ( std::map< int, double >::iterator it = vHashLivresOriginais.begin();
				  it != vHashLivresOriginais.end(); it++)
			{
				int col = (*it).first;
				double ub = (*it).second;
				idxUB[nChgUB] = col;
				valUB[nChgUB] = ub;
				bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;
				nChgUB++;
			}
	 
			lp->chgBds(nChgUB,idxUB,bts,valUB);
			lp->updateLP();

			// -------------------------------------------------------------------
				
			delete[] idxUB;
			delete[] valUB;
			delete[] bts;
		}

		
		// =====================================================================================
		
		//atendMaximoSemSabado( campusId, prioridade, cjtAlunosId, r, CARREGA_SOL_PARCIAL, xS );
		
		// =====================================================================================

		// Já incluído na maximização de atendimento por fases
		//if ( problemData->parametros->priorizarCalouros || problemData->parametros->priorizarFormandos )
		//{
		//	this->solveMaxAtendCalourosFormandos(campusId, prioridade, cjtAlunosId, r, CARREGA_SOL_PARCIAL, xS);
		//}

		
		// =====================================================================================

//		status = this->solveMaxAtend( campusId, prioridade, cjtAlunosId, r, CARREGA_SOL_PARCIAL, xS );
		status = this->solveMaxAtendPorFasesDoDia( campusId, prioridade, cjtAlunosId, r, CARREGA_SOL_PARCIAL, xS );
						
		// =====================================================================================

		// Segunda etapa: acrescenta váriaveis s_{i,d,a} e restrições relacionadas para associação de alunos

#ifdef PREMODELO_AGRUPA_ALUNO_POR_CURSO
		int oldNumVars = lp->getNumCols();
		
		// Add vars
		varNum += this->add_preVariaveis( campusId, prioridade, cjtAlunosId, r );
	    lp->updateLP();

		#ifdef PRINT_cria_variaveis
			cout << "\nNumber of new variables: " << varNum - oldNumVars;
			cout << "\nNew Total of Variables: "<< varNum;
		#endif

		int oldNumConst = lp->getNumRows();
		
	    // Constraint creation
	    constNum += this->add_preRestricoes( campusId, prioridade, cjtAlunosId, r );
	    lp->updateLP();

		#ifdef PRINT_cria_restricoes
			cout << "\nNumber of new constraints: " << constNum - oldNumConst;
			cout << "\nNew Total of Constraints: "<< constNum;
		#endif   
	
		// Salva a função objetivo original
      double *copy_objN = new double[ lp->getNumCols() ];
		for ( int i=0; i<oldNumVars; i++ ) copy_objN[i] = objN[i];
		lp->getObj( oldNumVars, lp->getNumCols()-1, copy_objN );

		// Deleta vetores com tamanho antigo
		delete[] xS;
		delete[] objN;
		delete[] idxN;
		delete[] idxUB;
		delete[] valUB;
		delete[] valFO;
		delete[] idxFO;
		delete[] bts;

		// Realoca vetores
		xS = new double[lp->getNumCols()];
		
		objN = new double[lp->getNumCols()];
		for ( int i=0; i<lp->getNumCols(); i++ ) objN[i] = copy_objN[i];
		
		delete[] copy_objN;

		idxN = new int[lp->getNumCols()];
		idxUB = new int[lp->getNumCols()*2];
		valUB = new double[lp->getNumCols()*2];
		valFO = new double[lp->getNumCols()*2];
		idxFO = new int[lp->getNumCols()*2];
		bts = new BOUNDTYPE[lp->getNumCols()*2];		

		vit = vHashPre.begin();
		for ( ; vit != vHashPre.end(); vit++ )
		{
			idxN[vit->second] = vit->second;
		}
#endif


		// =====================================================================================

	//	this->solveMinTurmasConflitos(campusId, prioridade, cjtAlunosId, r, CARREGA_SOL_PARCIAL, xS);
		
		// =====================================================================================

	  	  
	  std::cout << "\n=======================================>>>>";
	  std::cout<<"\n\nFO original!\n\n"; fflush(NULL);
	  
      // -------------------------------------------------------------------
	  // Volta com a FO original
	  lp->chgObj(lp->getNumCols(),idxN,objOrig);
      lp->updateLP();  

	  
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName );
		#endif

	  if ( CARREGA_SOL_PARCIAL )
	  {
			// procura e carrega solucao parcial
			int statusReadBin = readSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT, xS );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
	  }
	  if ( !CARREGA_SOL_PARCIAL )
	  {
		#ifdef SOLVER_CPLEX
		  lp->setNumIntSols(0);
		  lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT) );
		  lp->setPreSolve(OPT_TRUE);
		  lp->setHeurFrequency(1.0);
		  lp->setMIPEmphasis(0);
		  lp->setSymetry(0);
		  lp->setPolishAfterNode(10000000000);
		  lp->setPolishAfterTime(1600);
		  lp->setNodeLimit(10000000);
		  lp->setCuts(0);
		  lp->setMIPScreenLog( 4 );
		#endif
		#ifdef SOLVER_GUROBI
		  lp->setNumIntSols(0);
		  lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT) );
		  lp->setPreSolveIntensity( OPT_LEVEL1 );
		  lp->setHeurFrequency(1.0);
		  lp->setMIPEmphasis(0);
		  lp->setSymetry(0);
		  lp->setPolishAfterTime( this->getTimeLimit(Solver::PRE_TAT)*(0.66) );
		  lp->setNodeLimit(10000000);
		  lp->setCuts(0);
		  lp->setMIPScreenLog( 4 );
		#endif

			// GENERATES SOLUTION 		 
			this->nroPreSolAvaliadas = 0;
			std::cout << "\n\nOptimize...\n\n";
			
			lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );

			status = lp->optimize( METHOD_MIP );		
						
			lp->getX(xS);

			std::cout<<"\n\nOtimizado! Status = " << status << endl; fflush(NULL);
	  
			writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT, xS );
	  }      

	  
	  std::cout << "\n================================================================================";
	  std::cout<<"\n\nFim da otimizacao!" << endl << endl; fflush(NULL);
      
	  delete[] xS;
	  delete[] objOrig;
	  delete[] idxN;

	  // Imprime Gap
		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::solvePreTaticoCjtAlunos().\n";
		}
		else
		{
			outGaps << "Pre-modelo final - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
		}		
   }
   else
   {
	   int nAddedVars = this->add_preVariaveis( campusId, prioridade, cjtAlunosId, r);
	   varNum += nAddedVars;
   
	   lp->updateLP();
      
	  #ifdef PRINT_cria_variaveis
	    cout<< "\nNumber of Pre-Variables added: " << nAddedVars << "\n";
	    cout<< "Total of Pre-Variables: " << varNum << "\n\n";
	  #endif
   }

   return status;
}


int PreModelo::solvePreTaticoCjtAlunos_testeFases( int campusId, int prioridade, int cjtAlunosId, int r )
{
	int status = 0;
	
	bool CARREGA_SOL_PARCIAL = *( this->CARREGA_SOLUCAO );

	bool CARREGOU_HEURISTICA = false;

   if ( * this->CARREGA_SOLUCAO )
   {
#ifdef HEURISTICA_P2
		if ( leSolucaoHeuritica( campusId, prioridade, cjtAlunosId ) )
		{
			CARREGOU_HEURISTICA = true;		
			problemData->imprimeAlocacaoAlunos( campusId, prioridade, cjtAlunosId, true, r, toString(PRE_TAT), 0.0 );
			problemData->imprimeDiscNTurmas( campusId, prioridade, cjtAlunosId, true, r, 0 );
		}
#endif
	   char solName[1024];
	   strcpy( solName, getSolPreBinFileName( campusId, prioridade, cjtAlunosId, r, 0 ).c_str() );
	   FILE* fin = fopen( solName,"rb");
	   if ( fin == NULL )
	   {
		   std::cout << "\nA partir de " << solName << " , nao foram lidas mais solucoes.\n"; fflush(NULL);
		   *(this->CARREGA_SOLUCAO) = false;
	   }
	   else
	   {
		   fclose(fin);
	   }
   }

#ifdef HEURISTICA_P2
   // --------------------------------------------------------
   // HEURISTICA

   // Aloca alunos de prioridade 2 em turmas já existentes
   if ( ! (*this->CARREGA_SOLUCAO) && !CARREGOU_HEURISTICA && prioridade > 1 )
   {
   	CPUTimer timer;
	timer.start();

#ifndef TATICO_COM_HORARIOS
	   this->heuristicaAlocaAlunos( campusId, prioridade, cjtAlunosId );
#else
	   this->heuristica2AlocaAlunos( campusId, prioridade, cjtAlunosId );
#endif

	timer.stop();
	double runtime = timer.getCronoCurrSecs();

	   problemData->imprimeAlocacaoAlunos( campusId, prioridade, cjtAlunosId, true, r, toString(PRE_TAT), runtime );
	   problemData->imprimeDiscNTurmas( campusId, prioridade, cjtAlunosId, true, r, 0 );
	   this->imprimeAlocacaoFinalHeuristica( campusId, prioridade, cjtAlunosId );

	   escreveSolucaoBinHeuristica( campusId, prioridade, cjtAlunosId );	   
   
	   CARREGA_SOL_PARCIAL=false;
   }
   // --------------------------------------------------------
#endif
      
   if ( prioridade>1 )
   {
	   problemData->removeDemandasEmExcesso( campusId, prioridade, cjtAlunosId );
	   problemData->reduzNroTurmasPorDisciplina( campusId, prioridade, prioridade*r );
   }

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
   
   if ( vHashPre.size() > 0 )
   {
	   vHashPre.clear();
   }
   if ( cHashPre.size() > 0 )
   {
	   cHashPre.clear();
   }

   int varNum = 0;
   int constNum = 0;

   char lpName[1024], id[100];
   strcpy( lpName, getPreLpFileName( campusId, prioridade, cjtAlunosId, r ).c_str() );

   if ( problemData->parametros->funcao_objetivo == 0
      || problemData->parametros->funcao_objetivo == 2 )
   {
      lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
   }
   else if( problemData->parametros->funcao_objetivo == 1 )
   {
      lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
   }

   printf( "Creating LP...\n" );

   // Variable creation
   varNum = cria_preVariaveis( campusId, prioridade, cjtAlunosId, r );

   lp->updateLP();
      
   printf( "Total of Variables: %i\n\n", varNum );
      
	// -----------------------------------------------------
	// Deleta as variaveis em solVarsXPre 
	ITERA_GGROUP_LESSPTR ( it, solVarsXPre, VariablePre )
	{
		delete *it;    
	}
	solVarsXPre.clear();
    // --------------------------------------------

   if ( ! (*this->CARREGA_SOLUCAO) )
   {
	    ofstream outGaps;
		std::string gapFilename( "gap_input" );
		gapFilename += problemData->getInputFileName();
		gapFilename += ".txt";	

	    // Constraint creation
	    constNum = cria_preRestricoes( campusId, prioridade, cjtAlunosId, r );

	    lp->updateLP();

	    printf( "Total of Constraints: %i\n\n", constNum );

		std::string lpName1;
		lpName1 += "1";
		lpName1 += string(lpName);
						
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName1.c_str() );		
		#endif

		bool statusSolInicial = false;
				
		if ( problemData->existeSolTaticoInicial() )
		{
			std::cout << "\n=========================================";
			std::cout << "\nUsando solucao inicial...\n"; fflush(NULL);
		
			int nChg = 0;
			int *idx = new int[lp->getNumCols()*2];
			double *values = new double[lp->getNumCols()*2];

			VariablePreHash::iterator vit;
			vit = vHashPre.begin();
			for ( ; vit != vHashPre.end(); vit++ )
			{
				VariablePre v = vit->first;
				if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
   				{
					int lb = (int)(lp->getLB(vit->second)+0.5);
					int ub = (int)(lp->getUB(vit->second)+0.5);
				
					if ( lb != ub && problemData->getSolTaticoInicial()->isInSol(v) )
					{
						idx[nChg] = vit->second;
						values[nChg] = 1.0;
						nChg++;
					}
				}
				else if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
   				{
					int lb = (int)(lp->getLB(vit->second)+0.5);
					int ub = (int)(lp->getUB(vit->second)+0.5);
				
					if ( lb != ub && problemData->getSolTaticoInicial()->isInSol(v) )
					{
						idx[nChg] = vit->second;
						values[nChg] = 1.0;
						nChg++;
					}
				}
			}

			lp->updateLP();	   
			lp->setAdvance(2);
            statusSolInicial = lp->copyMIPStartSol( nChg, idx, values );
            lp->updateLP();

			if ( !statusSolInicial )
				std::cout<<"\n\ncpySolInicial Status = error" << endl;
			else
				std::cout<<"\n\ncpySolInicial Status = successful" << endl;
			fflush(NULL);
			
			delete [] values;
			delete [] idx;
			// -------------------------------------------------------------------
		}
		
		int *idxUB = new int[lp->getNumCols()*2];
		double *valUB = new double[lp->getNumCols()*2];
		BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()*2];
		double *xS = new double[lp->getNumCols()];

		int nChgUB = 0;
		VariablePreHash::iterator vit;

		if ( !statusSolInicial )
		{
			// -------------------------------------------------------------------
			// Fixa o ub = 0 para todas as variaveis que possuem lb zero e são livres

			std::cout << "\n===================>>>>";
			std::cout << "\nGarantindo solucao...\n"; fflush(NULL);
		   
			std::map< int, double > vHashLivresOriginais;
			nChgUB = 0;

			vit = vHashPre.begin();
			for ( ; vit != vHashPre.end(); vit++ )
			{
				VariablePre v = vit->first;

				if ( v.getType() == VariablePre::V_PRE_ABERTURA )
   				{
					int lb = (int)(lp->getLB(vit->second)+0.5);
					int ub = (int)(lp->getUB(vit->second)+0.5);
				
					if ( lb == 0 && ub != 0 )
					{
						vHashLivresOriginais[ vit->second ] = lp->getUB(vit->second);

						idxUB[nChgUB] = vit->second;
						valUB[nChgUB] = 0.0;
						bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;
						nChgUB++;
					}
				}
			}

			lp->chgBds(nChgUB,idxUB,bts,valUB);
			// -------------------------------------------------------------------
			
			lp->updateLP();
	
#ifdef SOLVER_CPLEX
			lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT1) );
			lp->setMIPRelTol( 0.01 );
			lp->setPreSolve(OPT_TRUE);
			lp->setHeurFrequency(1.0);
			lp->setMIPEmphasis(0);
			lp->setSymetry(0);
			lp->setMIPScreenLog( 4 );
			lp->setNumIntSols(1);
			lp->updateLP();
#elif SOLVER_GUROBI
			lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT1) );
			lp->setMIPRelTol( 0.01 );
			lp->setPreSolveIntensity( OPT_LEVEL1 );
			lp->setHeurFrequency(1.0);
			lp->setMIPEmphasis(0);
			lp->setSymetry(0);
			lp->setMIPScreenLog( 4 );
			lp->setNumIntSols(1);
			
			#if defined SOLVER_GUROBI && defined USAR_CALLBACK
			cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::PRE_TAT1);
			cb_data.gapMax = 80;
			lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
			#endif

			lp->updateLP();
#endif

			std::string lpName1;
			lpName1 += "1";
			lpName1 += string(lpName);

			#ifdef PRINT_LOGS
			lp->writeProbLP( lpName1.c_str() );		
			#endif

			
			// GENERATES SOLUTION 		 
			this->nroPreSolAvaliadas = 0;
			std::cout << "\n\nOptimize...\n\n";
			status = lp->optimize( METHOD_MIP );
			lp->getX(xS);
		
			writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT1, xS );
			writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT1, xS );
							

		#pragma region Imprime Gap
	 		// Imprime Gap
			outGaps.open(gapFilename, ofstream::app);
			if ( !outGaps )
			{
				std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::solvePreTaticoCjtAlunos().\n";
			}
			else
			{
				outGaps << "Pre-modelo (Garante Solucao) - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
				outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
				outGaps << "\n\n";
				outGaps.close();
			} 
		#pragma endregion
	    
			// -------------------------------------------------------------------
			// Volta as variaveis de abertura de turma z_{i,d,cp} que estavam livres
			nChgUB = 0;
         
			for ( std::map< int, double >::iterator it = vHashLivresOriginais.begin();
				  it != vHashLivresOriginais.end(); it++)
			{
				int col = (*it).first;
				double ub = (*it).second;
				idxUB[nChgUB] = col;
				valUB[nChgUB] = ub;
				bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;
				nChgUB++;
			}
	 
			lp->chgBds(nChgUB,idxUB,bts,valUB);
			lp->updateLP();

			// -------------------------------------------------------------------
				
		}
		
		
		if ( problemData->parametros->priorizarCalouros || problemData->parametros->priorizarFormandos )
		{
//			solveMaxAtendCalourosFormandos(campusId, prioridade, cjtAlunosId, r, CARREGA_SOL_PARCIAL, xS);
		}

		
      // -------------------------------------------------------------------
		
	    std::cout << "\n=======================================>>>>";

		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		lp->setCallbackFunc( NULL, NULL );
		#endif

		this->solveAbreTurmasPorFaixasPreTatico( xS, 3600 * 10, 60 );
			
		writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT4, xS );
		writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT4, xS );
		
      // -------------------------------------------------------------------
	  	  
	  std::cout << "\n=======================================>>>>";
	  std::cout<<"\n\nOriginal...\n\n"; fflush(NULL);

      // -------------------------------------------------------------------
	  
	  int *idxN = new int[ lp->getNumCols() ];	  
	  for ( VariablePreHash::iterator v_it = vHashPre.begin(); v_it != vHashPre.end(); v_it++ )
	  {
		  idxN[vit->second] = v_it->second;
	  }

      lp->updateLP();
   	  #ifdef PRINT_LOGS
		lp->writeProbLP( lpName );		
	  #endif
	  	  		
	  if ( CARREGA_SOL_PARCIAL )
	  {
			// procura e carrega solucao parcial
			int statusReadBin = readSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT, xS );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
	  }
	  if ( !CARREGA_SOL_PARCIAL )
	  {
		#ifdef SOLVER_CPLEX
		  lp->setNumIntSols(0);
		  lp->setTimeLimit( 3600 );
		  lp->setPreSolve(OPT_TRUE);
		  lp->setHeurFrequency(1.0);
		  lp->setMIPEmphasis(0);
		  lp->setSymetry(0);
		  lp->setPolishAfterNode(10000000000);
		  lp->setPolishAfterTime(1600);
		  lp->setNodeLimit(10000000);
		  lp->setCuts(0);
		  lp->setMIPScreenLog( 4 );
		#endif
		#ifdef SOLVER_GUROBI
		  lp->setNumIntSols(0);
		  lp->setTimeLimit( 3600 );
		  lp->setPreSolveIntensity( OPT_LEVEL1 );
		  lp->setHeurFrequency(1.0);
		  lp->setMIPEmphasis(0);
		  lp->setSymetry(0);
		  lp->setPolishAfterNode(10000000000);      
		  lp->setNodeLimit(10000000);
		  lp->setCuts(0);
		  lp->setMIPScreenLog( 4 );
		#endif

			// GENERATES SOLUTION 		 
			this->nroPreSolAvaliadas = 0;
			std::cout << "\n\nOptimize...\n\n";
			
			lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );

			status = lp->optimize( METHOD_MIP );
			lp->getX(xS);

			std::cout<<"\n\nOtimizado! Status = " << status << endl; fflush(NULL);
	  
			writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT, xS );
	  }      

	  
	  std::cout << "\n================================================================================";
	  std::cout<<"\n\nFim da otimizacao!" << endl << endl; fflush(NULL);
      
	  delete[] xS;
	  delete[] idxN;
      delete[] idxUB;
      delete[] bts;
      delete[] valUB;

	  // Imprime Gap
		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::solvePreTaticoCjtAlunos().\n";
		}
		else
		{
			outGaps << "Pre-modelo final - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
		}		
   }
   else
   {
	   int nAddedVars = this->add_preVariaveis( campusId, prioridade, cjtAlunosId, r);
	   varNum += nAddedVars;
   
	   lp->updateLP();
      
	  #ifdef PRINT_cria_variaveis
	    cout<< "\nNumber of Pre-Variables added: " << nAddedVars << "\n";
	    cout<< "Total of Pre-Variables: " << varNum << "\n\n";
	  #endif
   }

   return status;
}


int PreModelo::atendMaximoSemSabado( int campusId, int prioridade, int cjtAlunos, int r, bool &CARREGA_SOL_PARCIAL, double *xSol )
{
	std::cout << "\n===========>>";
	std::cout << "\nAtendimento fixo em 0 caso a fase do dia seja unica para o sabado...\n"; fflush(NULL);

	int status = 0;
	int *idxUB = new int[lp->getNumCols()];
	double *valOrigUB = new double[lp->getNumCols()];
	double *valUB = new double[lp->getNumCols()];
	BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()];

	int nChgUB = 0;	

	std::map< VariablePre, int >::iterator vit;
	for ( vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO && xSol[vit->second] < 0.1 )
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
	strcpy( lpName, this->getPreLpFileName( campusId, prioridade, cjtAlunos, r ).c_str() );

	std::string lpName2;
	lpName2 += "fixaZeroSab_";
	lpName2 += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName2.c_str() );
	#endif
				  				            
	#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT_SAB_ZERO) );
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
	#endif

	#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT_SAB_ZERO) );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPEmphasis(0);
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
		int statusReadBin = readSolBin( campusId, prioridade, cjtAlunos, r, 0, 0, Solver::PRE_TAT_SAB_ZERO, xSol );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, cjtAlunos, r, 0, 0, Solver::PRE_TAT_SAB_ZERO, xSol );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		// GENERATES SOLUTION
		status = lp->optimize( METHOD_MIP );
		lp->getX(xSol);
						
		writeSolBin( campusId, prioridade, cjtAlunos, r, 0, 0, Solver::PRE_TAT_SAB_ZERO, xSol );
		writeSolTxt( campusId, prioridade, cjtAlunos, r, 0, 0, Solver::PRE_TAT_SAB_ZERO, xSol );		
	}		


	#pragma region Imprime Gap	
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";	
	outGaps.open(gapFilename, ofstream::app);
	if ( !outGaps )
	{
		std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::().\n";
	}
	else
	{
		outGaps << "Estima Turmas - Zero em Sabado de Turno Unico - campus "<< campusId << ", prioridade " << prioridade;
		outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
		outGaps << "\n\n";
		outGaps.close();
	} 
	#pragma endregion
	
	
	// -------------------------------------------------------------------
	// Volta com os bounds originais
	
	lp->chgBds(nChgUB,idxUB,bts,valOrigUB);	
    lp->updateLP();
	
	// -------------------------------------------------------------------


	std::cout << "\n===========>>";
	std::cout << "\nFixando atendimento obtido...\n"; fflush(NULL);

	int nroAlunosDemandaAtendidos=0;
	nChgUB = 0;	
	for ( vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA && xSol[vit->second] < 0.1 )
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
	  
	std::cout << "\n\nNumero de atendidos: "<< nroAlunosDemandaAtendidos <<endl<<endl;
	std::cout << "\n================================================================================";

	delete [] idxUB;
	delete [] valUB;
	delete [] valOrigUB;
	delete [] bts;

	return status;
}

int PreModelo::solveMaxAtend( int campusId, int prioridade, int cjtAlunosId, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n===============>";
	std::cout << "\nGarantindo maximo atendimento geral...\n"; fflush(NULL);
		
	int status = 0;
	
	// -------------------------------------------------------------------

    char lpName[1024];
    strcpy( lpName, getPreLpFileName( campusId, prioridade, cjtAlunosId, r ).c_str() );
	
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";	

	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);
				
	// -------------------------------------------------------------------
	// Modificando coeficientes na função objetivo

	int *idxFO = new int[lp->getNumCols()*2];
	double *valFO = new double[lp->getNumCols()*2];        
	int nChgFO = 0;

	VariablePreHash::iterator vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;

		idxN[vit->second] = vit->second;
						
		if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA && prioridade==1 )
		{
			// Calcula nro total de creditos, pq só esta sendo criada a folga para a teorica.
			int nroCreds = v.getDisciplina()->getTotalCreditos();
			Disciplina* discPT = problemData->getDisciplinaTeorPrat( v.getDisciplina() );
			if ( discPT!=NULL ) nroCreds += discPT->getTotalCreditos();

			double coef = nroCreds * v.getAluno()->getReceita( v.getDisciplina() );
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = coef;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_SLACK_PRIOR_INF )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.4;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_SLACK_PRIOR_SUP )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.45;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_FOLGA_ABRE_TURMA_SEQUENCIAL )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.1;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_CRED_SALA_F1 )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.0;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_CRED_SALA_F2 )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.005;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_CRED_SALA_F3 )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.01;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_CRED_SALA_F4 )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.1;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_ABERTURA )
		{
			//double coef = 0.05 * v.getDisciplina()->getTotalCreditos() * v.getCampus()->getCusto();
			double coef = 0.1 * v.getDisciplina()->getTotalCreditos() * v.getCampus()->getCusto(); // testa maior custo
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = coef;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND1 )	// fmd: folga de mínimo de demanda por aluno
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 100.0;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND2 )	// fmd: folga de mínimo de demanda por aluno
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 200.0;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND3 )	// fmd: folga de mínimo de demanda por aluno
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 300.0;
			nChgFO++;
		}
		else if ( v.getType() == VariablePre::V_PRE_USA_DISCIPLINA )			// minimiza o nro de discs usadas
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.1;
			nChgFO++;
		}
		else
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.0;
			nChgFO++;
		}
    }

	lp->chgObj(nChgFO,idxFO,valFO);
    lp->updateLP();
			
	// =====================================================================================
	// CALCULA MÁXIMO ATENDIMENTO POR PARTES DE DEMANDA (80-20)

	int nParticoes=1;
#ifdef PARTICIONA_PREMODELO
	nParticoes=2;
#endif
	for ( int particao=1 ; particao <= nParticoes; particao++ )
	{
		#ifdef PARTICIONA_PREMODELO		
		  std::map< int, double > v_s_HashLivresOriginais;
		  int *idxS_UB = new int[lp->getNumCols()*2];
		  double *valS_UB = new double[lp->getNumCols()*2];
		  BOUNDTYPE *bts_S = new BOUNDTYPE[lp->getNumCols()*2];
		  int nChgUB_S = 0;

		  if ( particao==1 ) // FIXA 20% DAS DEMANDAS EM NÃO-ATENDIDAS
		  {
			// -------------------------------------------------------------------
			// Fixa o ub = 0 para todas as variaveis s_{i,d,a,cp} livres cuja quantidade de demandas da
			// disciplina d está abaixo do percentual mínimo considerado.
   
			std::cout << "\n\tFixando NAO atendimento das disciplinas pouco demandadas...\n"; fflush(NULL);

			map< int/*discId*/, GGroup<Aluno*, LessPtr<Aluno>> > mapDiscZeradaAlunos;

			vit = vHashPre.begin();
			for ( ; vit != vHashPre.end(); vit++ )
			{
				VariablePre v = vit->first;

				if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ) // s_{i,d,a,cp}
   				{
					int lb = (int)(lp->getLB(vit->second)+0.5);
					int ub = (int)(lp->getUB(vit->second)+0.5);
				
					Disciplina* disciplina = v.getDisciplina();
					double perc = problemData->getPercAcumuladoDemanda(disciplina);

					if ( lb == 0 && ub != 0 && perc > 80.0 )
					{
						v_s_HashLivresOriginais[ vit->second ] = lp->getUB(vit->second);

						idxS_UB[nChgUB_S] = vit->second;
						valS_UB[nChgUB_S] = 0.0;
						bts_S[nChgUB_S] = BOUNDTYPE::BOUND_UPPER;
						nChgUB_S++;
						mapDiscZeradaAlunos[ v.getDisciplina()->getId() ].add( v.getAluno() );
					}
				}
			}
			lp->chgBds(nChgUB_S,idxS_UB,bts_S,valS_UB);

			ofstream fixaNaoAtendFile;
			std::string fixaNaoAtendFilename( this->getFixaNaoAtendFileName( campusId, prioridade, cjtAlunosId, r ) );	
			fixaNaoAtendFile.open( fixaNaoAtendFilename, ios::out );
			if (!fixaNaoAtendFile)
			{
				cerr << "Error: Can't open output file " << fixaNaoAtendFilename << endl;
			}
			else
			{
				fixaNaoAtendFile<<"Numero de variaveis s_{i,d,a} zeradas = "<<nChgUB_S;
				fixaNaoAtendFile<<"\nNumero de disciplinas zeradas = "<< mapDiscZeradaAlunos.size() << endl;
				int nZero=0;

				map< int/*discId*/, GGroup<Aluno*, LessPtr<Aluno>> >::iterator 
					itMapDiscZeradas = mapDiscZeradaAlunos.begin();
				for ( ; itMapDiscZeradas != mapDiscZeradaAlunos.end(); itMapDiscZeradas++ )
				{
					nZero += itMapDiscZeradas->second.size();
					fixaNaoAtendFile<<"\n\tDisciplina ID: "<< itMapDiscZeradas->first 
						<< "\t\tTotal de AlunoDemanda zerados: " << itMapDiscZeradas->second.size() << "\n\t\t";
					ITERA_GGROUP_LESSPTR( it_discsZeradas, itMapDiscZeradas->second, Aluno )
					{
						fixaNaoAtendFile<<"Aluno ID="<< it_discsZeradas->getAlunoId()<<"; ";
					}
				}
				fixaNaoAtendFile<<endl<<endl;
				fixaNaoAtendFile << "\n\nNumero total de atendimentos AlunoDemanda zerados = " << nZero <<endl;
				fixaNaoAtendFile.close();
			}
			// -------------------------------------------------------------------
		  }
		  else if ( particao==2 ) // FIXA ATENDIMENTO DA ITERAÇÃO ANTERIOR 
		  {
				std::cout << "\n\tFixando maximo atendimento da particao 1..."; fflush(NULL);
		
				int nroAlunosDemandaAtendidos=0;
				vit = vHashPre.begin();
				nChgUB_S = 0;
				for ( ; vit != vHashPre.end(); vit++ )
				{
					VariablePre v = vit->first;
					double upperBound = lp->getUB( vit->second );

					if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA && xS[vit->second] < 0.1 )
					{
						idxS_UB[nChgUB_S] = vit->second;
						valS_UB[nChgUB_S] = 0.0;
						bts_S[nChgUB_S] = BOUNDTYPE::BOUND_UPPER;	
						nChgUB_S++;

						nroAlunosDemandaAtendidos++;
					}
					else if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA_OFERTA && xS[vit->second] < upperBound )
					{
						double value = xS[vit->second];
						idxUB[nChgUB] = vit->second;
						valUB[nChgUB] = value;
						bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
						nChgUB++;

						nroAlunosDemandaNAOAtendidos += value;
					}
/*					else if ( v.getType() == VariablePre::V_PRE_ABERTURA && xS[vit->second] > 0.9 )
					{
						idxS_UB[nChgUB_S] = vit->second;
						valS_UB[nChgUB_S] = 1.0;
						bts_S[nChgUB_S] = BOUNDTYPE::BOUND_LOWER;	
						nChgUB_S++;
					}	*/		
				}
				std::cout << "\n\n\tNumero de atendimentos = " << nroAlunosDemandaAtendidos <<endl; fflush(NULL);
			    lp->chgBds(nChgUB_S,idxS_UB,bts_S,valS_UB);
			    lp->updateLP();	  		  
		  }
		#endif
		 
		stringstream ss; ss << particao;
		std::string sp = ss.str();

		std::string lpName3;
		lpName3 += "3_part_";
		lpName3 += string(lpName);
			
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName3.c_str() );
		#endif
				  				            
		lp->updateLP();
		
		if ( CARREGA_SOL_PARCIAL )
		{
				// procura e carrega solucao parcial
				int statusReadBin = readSolBin( campusId, prioridade, cjtAlunosId, r, 0, particao, OptimizStep::PRE_TAT3, xS );
				if ( !statusReadBin )
				{
					CARREGA_SOL_PARCIAL=false;
				}
				else{
					writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, particao, OptimizStep::PRE_TAT3, xS );
					lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );
					
				}								
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
				#ifdef SOLVER_CPLEX
					lp->setNumIntSols(0);
					lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT3) );
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
					lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT3) );
					lp->setPreSolveIntensity( OPT_LEVEL1 );
					lp->setHeurFrequency(1.0);
					lp->setMIPEmphasis(0);
					lp->setSymetry(0);
					lp->setCuts(0);
					lp->setPolishAfterTime( this->getTimeLimit(Solver::PRE_TAT3) / 3 );
					lp->setNodeLimit(10000000);
					lp->setMIPScreenLog( 4 );
					lp->setMIPRelTol( 0.0 );

					#if defined SOLVER_GUROBI && defined USAR_CALLBACK
					cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::PRE_TAT3);
					cb_data.gapMax = 15;
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
					#endif
				#endif

			    // GENERATES SOLUTION
				std::cout << "\n\nOptimize...\n\n";
				lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT3) );
				status = lp->optimize( METHOD_MIP );
				lp->getX(xS);

				#if !defined (DEBUG) && !defined (TESTE)
					#ifdef SOLVER_CPLEX
						polishPreTatico(xS, 3600*2, 90, 15);
					#elif defined SOLVER_GUROBI				
						lp->setCallbackFunc( NULL, NULL );
						polishPreTatico(xS, 3600*4, 85, 15);
									
						#if defined SOLVER_GUROBI && defined USAR_CALLBACK
							lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
						#endif
				
						lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT3) );
						lp->updateLP();
					#endif

					writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, particao, OptimizStep::PRE_TAT3, xS ); // solucao intermediaria
					writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, particao, OptimizStep::PRE_TAT3, xS ); // solucao intermediaria
				#endif
		
				// GENERATES SOLUTION
				std::cout << "\n\nOptimize...\n\n";
				status = lp->optimize( METHOD_MIP );
				lp->getX(xS);
							
		
				writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, particao, OptimizStep::PRE_TAT3, xS );
				writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, particao, OptimizStep::PRE_TAT3, xS );
		}		

		#pragma region Imprime Gap
	 	// Imprime Gap
		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::solvePreTaticoCjtAlunos().\n";
		}
		else
		{
			outGaps << "Pre-modelo (Max atendimento) - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
		} 
		#pragma endregion

		#ifdef PARTICIONA_PREMODELO
			// -------------------------------------------------------------------
			// Volta as variaveis s_{i,d,a,cp} que estavam livres
			if ( particao==1 )
			{
				std::cout << "\n\tLiberando a possibilidade de atendimento das disciplinas pouco demandadas...\n"; fflush(NULL);
				nChgUB_S = 0;         
				for ( std::map< int, double >::iterator it = v_s_HashLivresOriginais.begin();
					  it != v_s_HashLivresOriginais.end(); it++)
				{
					int col = (*it).first;
					double ub = (*it).second;
					idxS_UB[nChgUB_S] = col;
					valS_UB[nChgUB_S] = ub;
					bts_S[nChgUB_S] = BOUNDTYPE::BOUND_UPPER;
					nChgUB_S++;
				}
	 
				lp->chgBds(nChgUB_S,idxS_UB,bts_S,valS_UB);
				lp->updateLP();
			}				
			// -------------------------------------------------------------------

		    delete []idxS_UB;
		    delete []valS_UB;
		    delete []bts_S;		
		#endif

	}

		
	std::cout << "\n=======================>>>>";
	
	int *idxUB = new int[lp->getNumCols()*2];
	double *valUB = new double[lp->getNumCols()*2];
	BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()*2];
	int nChgUB = 0;

	// =====================================================================================
	if ( problemData->parametros->considerarMinPercAtendAluno )
	{
		
		std::cout << "\nFixando maxima violacao de nao-atendimento...\n"; fflush(NULL);

		vit = vHashPre.begin();
		nChgUB = 0;
		for ( ; vit != vHashPre.end(); vit++ )
		{
			VariablePre v = vit->first;
				
			if ( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND1 ||
					v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND2 ||
					v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND3 )
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = xS[vit->second];
				bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
				nChgUB++;
			}
		}

		lp->chgBds(nChgUB,idxUB,bts,valUB);
		lp->updateLP();
	}
	// =================================================================================


	std::cout << "\n==========================>>>>";
	std::cout << "\nFixando maximo atendimento obtido por oferta...\n"; fflush(NULL);

	int nroAlunosDemandaAtendidos=0;
	int nroTurmas = 0;
	vit = vHashPre.begin();
	nChgUB = 0;
	for ( ; vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA && xS[vit->second] < 0.1 )
		{
			idxUB[nChgUB] = vit->second;
			valUB[nChgUB] = 0.0;
			bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
			nChgUB++;

			nroAlunosDemandaAtendidos++;				
		}
		else if ( v.getType() == VariablePre::V_PRE_ABERTURA && xS[vit->second] > 0.1 )
		{
			nroTurmas++;
		}
	}

	lp->chgBds(nChgUB,idxUB,bts,valUB);
	lp->updateLP();
	  
	std::cout << "\n\nNumero de AlunosDemanda atendidos: "<< nroAlunosDemandaAtendidos;
	std::cout << "\nNumero de turmas abertas: "<< nroTurmas <<endl<<endl;

				  				            
	
	// -------------------------------------------------------------------
	// Volta com a função objetivo original

	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    lp->updateLP();
	
	std::cout << "\n================================================================================";

	delete [] idxUB;
	delete [] valUB;
	delete [] bts;
	delete [] idxFO;
	delete [] valFO;
	delete [] idxN;
	delete [] objOrig;

	return status;
}

int PreModelo::solveMaxAtendPorFasesDoDia( int campusId, int prioridade, int cjtAlunosId, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n==========================>>>>";
	std::cout << "\nGarantindo maximo atendimento geral por fases do dia...\n"; fflush(NULL);
		
	int status = 0;
	
	// -------------------------------------------------------------------

    char lpName[1024];
    strcpy( lpName, getPreLpFileName( campusId, prioridade, cjtAlunosId, r ).c_str() );
	
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";	

	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);
				
	// -------------------------------------------------------------------
	// Modificando coeficientes na função objetivo

	int *idxFO = new int[lp->getNumCols()*2];
	double *valFO_FC = new double[lp->getNumCols()*2];        
	double *valFO_Geral = new double[lp->getNumCols()*2];        
	int nChgFO = 0;


	// -------------------------------------------------------------------
	// FO para formandos e calouros

	#pragma region FO para formandos e calouros
	nChgFO = 0;
	for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;

		idxN[vit->second] = vit->second;
		
		double coef = 0.0;
		
		if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA )
		{
			Aluno *aluno = v.getAluno();
			if ( ( problemData->parametros->priorizarCalouros && aluno->ehCalouro() ) ||
				 ( problemData->parametros->priorizarFormandos && aluno->ehFormando() ) )
			{
				coef = v.getDisciplina()->getTotalCreditos() * aluno->getReceita( v.getDisciplina() );
			}
		}
		else if ( v.getType() == VariablePre::V_PRE_ABERTURA )
		{
			coef = 1.0;
		}

		idxFO[nChgFO] = vit->second;
		valFO_FC[nChgFO] = coef;
		nChgFO++;
    }
	#pragma endregion

	// -------------------------------------------------------------------
	// FO para geral
	
	#pragma region FO para todos os alunos
	nChgFO = 0;
	for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;
											
		double coef = 0.0;
								
		if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA && prioridade==1 )
		{
			// Calcula nro total de creditos, pq só esta sendo criada a folga para a teorica.
			int nroCreds = v.getDisciplina()->getTotalCreditos();
			Disciplina* discPT = problemData->getDisciplinaTeorPrat( v.getDisciplina() );
			if ( discPT!=NULL ) nroCreds += discPT->getTotalCreditos();

			coef = 10 * nroCreds * v.getAluno()->getReceita( v.getDisciplina() );
		}
		else if ( v.getType() == VariablePre::V_PRE_SLACK_PRIOR_INF )
		{
			coef = 0.4;
		}
		else if ( v.getType() == VariablePre::V_PRE_SLACK_PRIOR_SUP )
		{
			coef = 0.45;
		}
		else if ( v.getType() == VariablePre::V_PRE_FOLGA_ABRE_TURMA_SEQUENCIAL )
		{
			coef = 0.1;
		}
		else if ( v.getType() == VariablePre::V_PRE_CRED_SALA_F1 )
		{
			coef = 0.0;
		}
		else if ( v.getType() == VariablePre::V_PRE_CRED_SALA_F2 )
		{
			coef = 0.005;
		}
		else if ( v.getType() == VariablePre::V_PRE_CRED_SALA_F3 )
		{
			coef = 0.01;
		}
		else if ( v.getType() == VariablePre::V_PRE_CRED_SALA_F4 )
		{
			coef = 0.1;
		}
		else if ( v.getType() == VariablePre::V_PRE_ABERTURA )
		{
			//coef = 0.05 * v.getDisciplina()->getTotalCreditos() * v.getCampus()->getCusto();
			coef = 0.1 * v.getDisciplina()->getTotalCreditos() * v.getCampus()->getCusto(); // testa maior custo
		}
		else if ( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND1 )	// fmd: folga de mínimo de demanda por aluno
		{
			coef = 100.0;
		}
		else if ( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND2 )	// fmd: folga de mínimo de demanda por aluno
		{
			coef = 200.0;
		}
		else if ( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND3 )	// fmd: folga de mínimo de demanda por aluno
		{
			coef = 300.0;
		}
		else if ( v.getType() == VariablePre::V_PRE_USA_DISCIPLINA )			// minimiza o nro de discs usadas
		{
			coef = 0.1;
		}

		idxFO[nChgFO] = vit->second;
		valFO_Geral[nChgFO] = coef;
		nChgFO++;
    }
	#pragma endregion
	

	
	// =====================================================================================
	// CALCULA MÁXIMO ATENDIMENTO POR PARTES DO DIA

	int *idxUB = new int[lp->getNumCols()*2];
	double *valUB = new double[lp->getNumCols()*2];
	double *valOrigUB_0 = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB = new BOUNDTYPE[lp->getNumCols()*2];

	int *idxLB_Q = new int[lp->getNumCols()*2];
	double *valLB_Q = new double[lp->getNumCols()*2];
	double *valOrigLB_Q = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsLB_Q = new BOUNDTYPE[lp->getNumCols()*2];
	
	double *valOrigLB_S = new double[lp->getNumCols()];
	int *idxLB_S = new int[lp->getNumCols()];
	BOUNDTYPE *btsLB_S = new BOUNDTYPE[lp->getNumCols()];
	double *valLB_S = new double[lp->getNumCols()];
	
	
	int nChgLB_S = 0;
	int nChgLB_Q = 0;
	for( auto itFase = problemData->fasesDosTurnos.begin(); itFase != problemData->fasesDosTurnos.end(); itFase++ )
	{		 
		int fase = itFase->first;

		std::cout << "\n======>> Fase " << fase << endl;

		#pragma region FIXA O NÃO-ATENDIMENTO DE FASES À FRENTE
		// Fixa não-atendimentos de fases à frente.
		int nChgUB = 0;
		for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
		{
			VariablePre v = vit->first;
									
			if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO )
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
		#pragma endregion

		stringstream ss;
		ss << fase;
		std::string sf = ss.str();
		

		if ( problemData->parametros->priorizarCalouros ||
			 problemData->parametros->priorizarFormandos )
		{
			std::cout << "\n==>> Maximizacao de atendimento de calouros e formandos na fase " << fase << endl;

			std::string lpNameFormandoCalouro;
			lpNameFormandoCalouro += "maxAtendFormandoCalouro" + sf + "_";
			lpNameFormandoCalouro += string(lpName);

			#ifdef PRINT_LOGS
			lp->writeProbLP( lpNameFormandoCalouro.c_str() );
			#endif
				  				
			// Seta FO para maximizar atendimento de calouros e formandos
			lp->chgObj(nChgFO,idxFO,valFO_FC);
			lp->updateLP();
		
			#pragma region OTIMIZA OU CARREGA SOLUÇÃO PARA FORMANDOS/CALOUROS
			if ( CARREGA_SOL_PARCIAL )
			{
				// procura e carrega solucao parcial
				int statusReadBin = readSolBin( campusId, prioridade, cjtAlunosId, r, 0, fase, Solver::PRE_TAT2, xS );
				if ( !statusReadBin )
				{
					CARREGA_SOL_PARCIAL=false;
				}
				else writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, fase, Solver::PRE_TAT2, xS );
			}
			if ( !CARREGA_SOL_PARCIAL )
			{
				#ifdef SOLVER_CPLEX
					lp->setNumIntSols(0);
					lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT2) );
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
				#endif
				#ifdef SOLVER_GUROBI
					lp->setNumIntSols(0);
					lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT2) );
					lp->setPreSolveIntensity( OPT_LEVEL1 );
					lp->setHeurFrequency(1.0);
					lp->setMIPEmphasis(0);
					lp->setSymetry(0);
					lp->setCuts(0);
					lp->setPolishAfterTime( this->getTimeLimit(Solver::PRE_TAT2) / 2 );
					lp->setNodeLimit(10000000);
					lp->setMIPScreenLog( 4 );
					lp->setMIPRelTol( 0.0 );

					cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::PRE_TAT2);
					cb_data.gapMax = 40;
					#if defined (SOLVER_GUROBI) && defined (USAR_CALLBACK)
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );			
					#endif
				#endif
			
				lp->updateLP();

				// GENERATES SOLUTION
				status = lp->optimize( METHOD_MIP );
				lp->getX(xS);
						
				writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, fase, Solver::PRE_TAT2, xS );
				writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, fase, Solver::PRE_TAT2, xS );
			}
			#pragma endregion
						
			#pragma region FIXA ATENDIMENTO DE CALOUROS/FORMANDOS
			// Fixar o atendimento parcial ( fd_{d,a} = 0 )
			int nroTurmas = 0;
			int nAtendsFC = 0;
			int nAtends = 0;
			nChgUB = 0;
			for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
			{
				VariablePre v = vit->first;
									
				if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA )
				{
					double value = (int)( xS[ vit->second ] + 0.5 );
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );

					if ( value == 0.0 )
					{
						if ( v.getDisciplina()->getId() > 0 )
						{ 
							nAtends++;
							if ( v.getAluno()->ehCalouro() || v.getAluno()->ehFormando() )
								nAtendsFC++;
						}

						if ( lb != ub )
						{
							idxUB[nChgUB] = vit->second;
							valUB[nChgUB] = value;
							btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;
							nChgUB++;		
						}
					}
				}
				else if ( v.getType() == VariablePre::V_PRE_ABERTURA )
				{
					double value = (int)( xS[ vit->second ] + 0.5 );
					if ( value == 1.0 )
						nroTurmas++;
				}
			}
			lp->chgBds( nChgUB, idxUB, btsUB, valUB );			// Fixa fd = 0

			std::cout << "\n---> Total de AlunoDemanda calouros+formandos atendidos ao fim da fase " 
				<< fase << ": " << nAtendsFC;
			std::cout << "\n---> Total de AlunoDemanda atendidos ao fim da fase " 
				<< fase << ": " << nAtends;
			std::cout << "\n---> Numero de turmas p+t abertas: "<< nroTurmas <<endl<<endl;
			#pragma endregion
		}
				
		std::cout << "\n==>> Maximizacao de atendimento de todos os alunos na fase " << fase << endl;

		std::string lpName3;
		lpName3 += "maxAtend" + sf + "_";
		lpName3 += string(lpName);

		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName3.c_str() );
		#endif
				
		// Seta FO para maximizar atendimento geral
		lp->chgObj(nChgFO,idxFO,valFO_Geral);
		lp->updateLP();
		
		#pragma region OTIMIZA OU CARREGA SOLUÇÃO PARA TODOS OS ALUNOS
		if ( CARREGA_SOL_PARCIAL )
		{
				// procura e carrega solucao parcial
				int statusReadBin = readSolBin( campusId, prioridade, cjtAlunosId, r, 0, fase, OptimizStep::PRE_TAT3, xS );
				if ( !statusReadBin )
				{
					CARREGA_SOL_PARCIAL=false;
				}
				else{
					writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, fase, OptimizStep::PRE_TAT3, xS );
					lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );
					
				}								
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
				#ifdef SOLVER_CPLEX
					lp->setNumIntSols(0);

					double runtime = 3600;					
					if (fase==1) runtime = this->getTimeLimit(Solver::PRE_M);
					if (fase==2) runtime = this->getTimeLimit(Solver::PRE_T);
					if (fase==3) runtime = this->getTimeLimit(Solver::PRE_N);
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

					double runtime = 3600.0;					
					if (fase==1) runtime = (double) this->getTimeLimit(Solver::PRE_M);
					if (fase==2) runtime = (double) this->getTimeLimit(Solver::PRE_T);
					if (fase==3) runtime = (double) this->getTimeLimit(Solver::PRE_N);
					lp->setTimeLimit( runtime );
					
					lp->setPreSolveIntensity( OPT_LEVEL1 );
					lp->setHeurFrequency(1.0);
					lp->setMIPEmphasis(0);
					lp->setSymetry(1);
					lp->setCuts(0);
					lp->setPolishAfterTime( runtime * 0.5 );
					lp->setNodeLimit(10000000);
					lp->setMIPScreenLog( 4 );
					lp->setMIPRelTol( 0.0 );

					#if defined SOLVER_GUROBI && defined USAR_CALLBACK

					if (fase==1) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::PRE_M);
					if (fase==2) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::PRE_T);
					if (fase==3) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::PRE_N);
					cb_data.gapMax = 20;
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
					#endif
				#endif

				
				// GENERATES SOLUTION
				std::cout << "\n\nOptimize...\n\n";
				status = lp->optimize( METHOD_MIP );
				lp->getX(xS);
							
				writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, fase, OptimizStep::PRE_TAT3, xS ); // solucao intermediaria
				writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, fase, OptimizStep::PRE_TAT3, xS ); // solucao intermediaria

				if ( fase == 1  ||  fase == 3 ) // TODO: não deveria ser hardcoded, a dificuldade da fase deve ser calculavel de alguma forma
				{
					#if !defined (DEBUG) && !defined (TESTE)
						#ifdef SOLVER_CPLEX
							polishPreTatico(xS, 3600*2, 70, 15);
						#elif defined SOLVER_GUROBI				
							lp->setCallbackFunc( NULL, NULL );
							polishPreTatico(xS, 3600*2, 90, 50);
									
							#if defined SOLVER_GUROBI && defined USAR_CALLBACK
								lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
							#endif
				
							lp->setTimeLimit( runtime );
							lp->updateLP();
						#endif
					#endif
								
					writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, fase, OptimizStep::PRE_TAT3, xS );
					writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, fase, OptimizStep::PRE_TAT3, xS );
				}

				
				//// GENERATES SOLUTION
				//lp->setTimeLimit( runtime );
				//std::cout << "\n\nOptimize...\n\n";
				//status = lp->optimize( METHOD_MIP );	// teste para 1xn
				//lp->getX(xS);
				//
				//writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, fase, OptimizStep::PRE_TAT3, xS );
				//writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, fase, OptimizStep::PRE_TAT3, xS );
		}	
		#pragma endregion
				
		// Volta bounds originais de variaveis q_{i,d,s,g} que foram fixadas como NÃO atendidas.
		lp->chgBds( nChgUB, idxUB, btsUB, valOrigUB_0 );

		#pragma region FIXA ATENDIMENTO PARA TODOS OS ALUNOS
		// Fixar o atendimento parcial ( fd_{a,d} = 0 ;  s_{i,d,a,g} = 1 ;  q_{i,d,s,g} = 1 )
		int nAtends = 0;
		nChgUB = 0;
		for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
		{
			VariablePre v = vit->first;
									
			if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA )
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
			else if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
			{
				double value = (int)( xS[ vit->second ] + 0.5 );				
				double ub = (int)( lp->getUB( vit->second ) + 0.5 );
				double lb = (int)( lp->getLB( vit->second ) + 0.5 );

				if ( value == 1.0 )
				if ( lb != ub )
				{
					valOrigLB_S[nChgLB_S] = lp->getLB( vit->second );
					idxLB_S[nChgLB_S] = vit->second;
					valLB_S[nChgLB_S] = 1.0;
					btsLB_S[nChgLB_S] = BOUNDTYPE::BOUND_LOWER;					
					nChgLB_S++;
				}
			}
			else if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO )
			if ( v.getTurno() <= fase )
			{
				double value = (int)( xS[ vit->second ] + 0.5 );				
				double ub = (int)( lp->getUB( vit->second ) + 0.5 );
				double lb = (int)( lp->getLB( vit->second ) + 0.5 );

				if ( value == 1.0 )
				if ( lb != ub )		// Fixa q = 1
				{
					valOrigLB_Q[nChgLB_Q] = lp->getLB( vit->second );
					idxLB_Q[nChgLB_Q] = vit->second;
					valLB_Q[nChgLB_Q] = 1.0;
					btsLB_Q[nChgLB_Q] = BOUNDTYPE::BOUND_LOWER;
					nChgLB_Q++;
				}

				if ( value == 0.0 )
				if ( lb != ub ) 	// Fixa q = 0
				{
					valOrigLB_Q[nChgLB_Q] = lp->getUB( vit->second );
					idxLB_Q[nChgLB_Q] = vit->second;
					valLB_Q[nChgLB_Q] = 0.0;
					btsLB_Q[nChgLB_Q] = BOUNDTYPE::BOUND_UPPER;
					nChgLB_Q++;
				}
			}
		}
		lp->chgBds( nChgUB, idxUB, btsUB, valUB );			// Fixa fd = 0
		lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valLB_Q );	// Fixa q = 1  e  q = 0
		lp->chgBds( nChgLB_S, idxLB_S, btsLB_S, valLB_S );	// Fixa s = 1

		std::cout << "\n---> Total de alunos-demanda atendidos ao fim da fase " << fase << ": " << nAtends << endl;
		#pragma endregion

		#pragma region Imprime Gap
	 	// Imprime Gap
		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::solvePreTaticoCjtAlunos().\n";
		}
		else
		{
			outGaps << "Pre-modelo (Max atendimento) - fase do dia " << fase << " - campus "<< campusId 
				<< ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
		} 
		#pragma endregion
		
	}

	// Volta lower bounds originais de variaveis q_{i,d,s,g}
	//lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valOrigLB_Q );

	// Volta lower bounds originais de variaveis s_{i,d,a,g}
	lp->chgBds( nChgLB_S, idxLB_S, btsLB_S, valOrigLB_S );

	
	#pragma region CONSIDERA MINIMO PERCENTUAL DE ATENDIMENTO POR ALUNO
	// =====================================================================================
	if ( problemData->parametros->considerarMinPercAtendAluno )
	{
		std::cout << "\n=======================>>>>";	
		std::cout << "\nFixando maxima violacao de nao-atendimento...\n"; fflush(NULL);
				
		int nChgUB = 0;
		for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
		{
			VariablePre v = vit->first;
				
			if ( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND1 ||
					v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND2 ||
					v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND3 )
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = xS[vit->second];
				btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
				nChgUB++;
			}
		}

		lp->chgBds(nChgUB,idxUB,btsUB,valUB);
		lp->updateLP();
	}
	// =================================================================================
	#pragma endregion

	#pragma region FIXA O ATENDIMENTO PARA TODOS OS ALUNOS E A NAO ABERTURA DE TURMAS
	std::cout << "\n==========================>>>>";
	std::cout << "\nFixando maximo atendimento obtido e NAO abertura de turmas...\n"; fflush(NULL);

	int nroAlunosDemandaAtendidos=0;
	int nroTurmas = 0;
	int nChgUB = 0;
	for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;
		double value = (int)( xS[ vit->second ] + 0.5 );

		if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA && value < 0.1 )
		{					
			nroAlunosDemandaAtendidos++;
			double ub = (int)( lp->getUB( vit->second ) + 0.5 );
			double lb = (int)( lp->getLB( vit->second ) + 0.5 );

			if ( lb != ub )
			{
				// Fixa que o aluno foi atendido
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 0.0;
				btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
				nChgUB++;
			}
		}
		else if ( v.getType() == VariablePre::V_PRE_ABERTURA && value < 0.1 )
		{
			double ub = (int)( lp->getUB( vit->second ) + 0.5 );
			double lb = (int)( lp->getLB( vit->second ) + 0.5 );

			if ( lb != ub )
			{
				// Fixa turma NÃO aberta
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 0.0;
				btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
				nChgUB++;
			}
		}
		else if ( v.getType() == VariablePre::V_PRE_ABERTURA && value > 0.1 )
		{
			// Conta nro de turmas abertas
			nroTurmas++;
		}
	}

	lp->chgBds(nChgUB,idxUB,btsUB,valUB);	// Fixa fd = 0  &  z = 0
	lp->updateLP();
	  
	std::cout << "\n\nNumero de AlunosDemanda atendidos: "<< nroAlunosDemandaAtendidos;
	std::cout << "\nNumero de turmas abertas: "<< nroTurmas <<endl<<endl;
	#pragma endregion
				  				            
	
	// -------------------------------------------------------------------
	// Volta com a função objetivo original

	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    lp->updateLP();
	
	std::cout << "\n================================================================================";
	
	delete [] valOrigLB_S;
	delete [] valLB_S;
	delete [] btsLB_S;
	delete [] idxLB_S;

	delete [] idxLB_Q;
	delete [] valLB_Q;
	delete [] btsLB_Q;
	delete [] valOrigLB_Q;

	delete [] idxUB;
	delete [] valUB;
	delete [] btsUB;
	delete [] valOrigUB_0;
	
	delete [] idxFO;
	delete [] valFO_FC;
	delete [] valFO_Geral;
	delete [] idxN;
	delete [] objOrig;

	return status;
}

int PreModelo::solveMinTurmasConflitos( int campusId, int prioridade, int cjtAlunosId, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	  std::cout << "\n====================>>>";
	  std::cout << "\nMinimizando 'z', 'fd', 'w' e 'as'...\n"; fflush(NULL);
		
	  int status = 0;
	  
	  // -------------------------------------------------------------------

	  char lpName[1024];
	  strcpy( lpName, getPreLpFileName( campusId, prioridade, cjtAlunosId, r ).c_str() );
	
	  ofstream outGaps;
	  std::string gapFilename( "gap_input" );
	  gapFilename += problemData->getInputFileName();
	  gapFilename += ".txt";	

	  // -------------------------------------------------------------------
	  // Salvando função objetivo original
	  
	  int *idxN = new int[lp->getNumCols()*2];
	  double *objOrig = new double[lp->getNumCols()*2];
	  lp->getObj(0,lp->getNumCols()-1,objOrig);


	  // -------------------------------------------------------------------
	  // Função Obj só com variaveis w, as, fpi, fps, ft
		
	  int *idxFO = new int[lp->getNumCols()*2];
	  double *valFO = new double[lp->getNumCols()*2];
	  int nChgFO = 0;
	        
	  VariablePreHash::iterator vit;
	  for ( vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
	  {
			VariablePre v = vit->first;
		
			idxN[vit->second] = vit->second;
						
			if ( v.getType() == VariablePre::V_PRE_ALUNO_SALA  ) // as
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 2.0;     
				nChgFO++;
			}
			else if ( v.getType() == VariablePre::V_PRE_TURMAS_COMPART  ) // w
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 5.0;		
				nChgFO++;
			}
			else if ( v.getType() == VariablePre::V_PRE_SLACK_PRIOR_INF )
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 0.2;
				nChgFO++;
			}
			else if ( v.getType() == VariablePre::V_PRE_SLACK_PRIOR_SUP )
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 0.25;
				nChgFO++;
			}
			else if ( v.getType() == VariablePre::V_PRE_FOLGA_ABRE_TURMA_SEQUENCIAL )
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 0.1;
				nChgFO++;
			}
			if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA ) // fd
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 1.0;
				nChgFO++;				
			}
			else if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA_OFERTA ) // fd
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 1.0;
				nChgFO++;
			}
			else if ( v.getType() == VariablePre::V_PRE_ABERTURA ) // z
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 0.1;
				nChgFO++;
			}
			else if ( v.getType() == VariablePre::V_PRE_USA_DISCIPLINA ) // k
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 0.01;
				nChgFO++;
			}
			else
			{
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = 0.0;
				nChgFO++;
			}			
	  }
      lp->chgObj(nChgFO,idxFO,valFO);
      lp->updateLP();

	  // ----------------------------------------


	  std::string lpName4;
	  lpName4 += "4";
	  lpName4 += string(lpName);
	  
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName4.c_str() );
		#endif
	  		
	  if ( CARREGA_SOL_PARCIAL )
	  {
			// procura e carrega solucao parcial
			int statusReadBin = readSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT4, xS );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else{
				writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT4, xS );
				lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
			}
	  }
	  if ( !CARREGA_SOL_PARCIAL )
	  {
		#ifdef SOLVER_CPLEX
		 lp->setNumIntSols(0);
		 lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT4) );
	     if ( prioridade>1 ) lp->setTimeLimit(3600);
		 lp->setPreSolve(OPT_TRUE);
		 lp->setHeurFrequency(1.0);
		 lp->setMIPEmphasis(0);
		 lp->setSymetry(0);
		 lp->setPolishAfterTime(2800);
	     lp->setNodeLimit(100000000);
		 lp->setCuts(0);
		 lp->setMIPScreenLog( 4 );
		 lp->setMIPRelTol(0.0);
		#endif
		#ifdef SOLVER_GUROBI
		  lp->setPolishAfterIntSol(1);
		  lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT4) );
		  if ( prioridade>1 ) lp->setTimeLimit(3600);
		  lp->setPreSolveIntensity( OPT_LEVEL_AUTO );
		  lp->setHeurFrequency(1.0);
		  lp->setMIPEmphasis(0);
		  lp->setSymetry(0);
		  lp->setPolishAfterTime( this->getTimeLimit(Solver::PRE_TAT4) / 2 );
		  lp->setNodeLimit(10000000);
		  lp->setCuts(0);
		  lp->setMIPScreenLog( 4 );
		  lp->setMIPRelTol(0.0);
		#endif
		  
		    lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);

			// GENERATES SOLUTION 		 
			this->nroPreSolAvaliadas = 0;
			status = lp->optimize( METHOD_MIP );
			lp->getX(xS);

			writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT4, xS );
			writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, 0, OptimizStep::PRE_TAT4, xS );
	  }	

	#pragma region Imprime Gap
	  // Imprime Gap
	  outGaps.open(gapFilename, ofstream::app);
	  if ( !outGaps )
	  {
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::solvePreTaticoCjtAlunos().\n";
	  }
	  else
	  {
			outGaps << "Pre-modelo (Min Conflitos) - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
	  } 
	#pragma endregion
			  
	  
	  // ------------------------------------------------------
	  // Fixa w e as, e volta com todo o restante na FO
	  
	  std::cout << "\n====================>>>>>";
	  std::cout << "\nFixando 'fd', 'z', 'w' e 'as'...\n"; fflush(NULL);

	  int *idxUB = new int[lp->getNumCols()*2];
	  double *valUB = new double[lp->getNumCols()*2];
	  BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()*2];

      int nChgUB = 0;
      vit = vHashPre.begin();
	  for ( ; vit != vHashPre.end(); vit++ )
	  {
			VariablePre v = vit->first;

			if ( v.getType() == VariablePre::V_PRE_TURMAS_COMPART && xS[vit->second] > 0.1 )
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 1.0;
				bts[nChgUB] = BOUNDTYPE::BOUND_LOWER;				
				nChgUB++;
			}
			else if ( v.getType() == VariablePre::V_PRE_ALUNO_SALA && xS[vit->second] > 0.1 )
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 1.0;
				bts[nChgUB] = BOUNDTYPE::BOUND_LOWER;				
				nChgUB++;
			}
			else if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA && xS[vit->second] < 0.1 ) // fd
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 0.0;
				bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;				
				nChgUB++;				
			}
			else if ( v.getType() == VariablePre::V_PRE_ABERTURA && xS[vit->second] > 0.1 ) // z
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 1.0;
				bts[nChgUB] = BOUNDTYPE::BOUND_LOWER;				
				nChgUB++;
			}
	  }
      lp->chgBds(nChgUB,idxUB,bts,valUB);
      lp->updateLP();	  
	  	  
	  
      // -------------------------------------------------------------------
	  // Volta com a FO original
	  lp->chgObj(lp->getNumCols(),idxN,objOrig);
      lp->updateLP();  
	  
	  delete [] idxUB;
	  delete [] valUB;
	  delete [] bts;
	  delete [] idxFO;
	  delete [] valFO;
	  delete [] idxN;
	  delete [] objOrig;

	  return status;
}

int PreModelo::solveMaxAtendCalourosFormandos( int campusId, int prioridade, int cjtAlunosId, int r, bool& CARREGA_SOL_PARCIAL, double *xSol )
{
	std::cout << "\n===============>";
	std::cout << "\nGarantindo maximo atendimento para calouros e formandos...\n"; fflush(NULL);
		
	int status = 0;

	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);
				
	// -------------------------------------------------------------------
	// Modificando coeficientes na função objetivo

	int *idxFO = new int[lp->getNumCols()*2];
	double *valFO = new double[lp->getNumCols()*2];        
	int nChgFO = 0;

	VariablePreHash::iterator vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;

		idxN[vit->second] = vit->second;
			
		bool nuloNaFO = true;

		if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA )
		{
			Aluno *aluno = v.getAluno();
			if ( ( problemData->parametros->priorizarCalouros && aluno->ehCalouro() ) ||
				 ( problemData->parametros->priorizarFormandos && aluno->ehFormando() ) )
			{
				nuloNaFO = false;

				double coef = v.getDisciplina()->getTotalCreditos() * aluno->getReceita( v.getDisciplina() );
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = coef;
				nChgFO++;
			}
		}
		else if ( v.getType() == VariablePre::V_PRE_ABERTURA )
		{
				nuloNaFO = false;

				double coef = 1.0;
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = coef;
				nChgFO++;
		}

		if ( nuloNaFO )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.0;
			nChgFO++;
		}
    }

	lp->chgObj(nChgFO,idxFO,valFO);
    lp->updateLP();
	  
	// -------------------------------------------------------------------
	
    char lpName[1024];
    strcpy( lpName, getPreLpFileName( campusId, prioridade, cjtAlunosId, r ).c_str() );

	std::string lpName2;
	lpName2 += "2_";
	lpName2 += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName2.c_str() );
	#endif

				  				            
	#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT2) );
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
	#endif

	#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT2) );
		lp->setPreSolveIntensity( OPT_LEVEL1 );
		lp->setHeurFrequency(1.0);
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(0);
		lp->setPolishAfterTime( this->getTimeLimit(Solver::PRE_TAT2) / 2 );
		lp->setNodeLimit(10000000);
		lp->setMIPScreenLog( 4 );
		lp->setMIPRelTol( 0.0 );

		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::PRE_TAT2);
		cb_data.gapMax = 70;
		#if defined (SOLVER_GUROBI) && defined (USAR_CALLBACK)
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );			
		#endif
	#endif
			
	lp->updateLP();
		
	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolBin( campusId, prioridade, cjtAlunosId, r, 0, 1, Solver::PRE_TAT2, xSol );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, 1, Solver::PRE_TAT2, xSol );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		// GENERATES SOLUTION
		status = lp->optimize( METHOD_MIP );
		lp->getX(xSol);
						
		writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, 1, Solver::PRE_TAT2, xSol );
		writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, 1, Solver::PRE_TAT2, xSol );

#if !defined (DEBUG) && !defined (TESTE)
#ifdef SOLVER_CPLEX
		polishPreTatico(xS, 3600*2, 90, 15);
#elif defined SOLVER_GUROBI								
		lp->setCallbackFunc( NULL, NULL );
		polishPreTatico(xSol, 3600*2, 90, 20);
		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif
		lp->setTimeLimit( this->getTimeLimit(Solver::PRE_TAT2) );
		lp->updateLP();
#endif
		writeSolBin( campusId, prioridade, cjtAlunosId, r, 0, 1, Solver::PRE_TAT2, xSol );
		writeSolTxt( campusId, prioridade, cjtAlunosId, r, 0, 1, Solver::PRE_TAT2, xSol );
#endif
		
	}		


	#pragma region Imprime Gap	
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";	
	outGaps.open(gapFilename, ofstream::app);
	if ( !outGaps )
	{
		std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em PreModelo::solvePreTaticoCjtAlunos().\n";
	}
	else
	{
		outGaps << "Pre-modelo (Max atendimento) - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
		outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
		outGaps << "\n\n";
		outGaps.close();
	} 
	#pragma endregion
	
	// -------------------------------------------------------------------


	std::cout << "\n===========>>";
	std::cout << "\nFixando atendimento de calouros e formandos obtido...\n"; fflush(NULL);

	int *idxUB = new int[lp->getNumCols()];
	double *valUB = new double[lp->getNumCols()];
	BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()];

	int nroCalourosAtendidos=0;
	int nroFormandosAtendidos=0;
	int nroAtendidos=0;
	int nChgUB = 0;	
	for ( vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA && xSol[vit->second] < 0.1 )
		{
			idxUB[nChgUB] = vit->second;
			valUB[nChgUB] = 0.0;
			bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
			nChgUB++;
			
			if ( v.getDisciplina()->getId() > 0 )
			{
				nroAtendidos++;
				if ( v.getAluno()->ehCalouro() ) nroCalourosAtendidos++;
				if ( v.getAluno()->ehFormando() ) nroFormandosAtendidos++;
			}
		}
	}

	lp->chgBds(nChgUB,idxUB,bts,valUB);
	lp->updateLP();
	  
	std::cout << "\n\nNumero de calouros atendidos: "<< nroCalourosAtendidos;
	std::cout << "\nNumero de formandos atendidos: "<< nroFormandosAtendidos;
	std::cout << "\nNumero total de AlunoDemanda atendido: "<< nroAtendidos <<endl<<endl;
	
	// -------------------------------------------------------------------
	// Volta com a função objetivo original

	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    lp->updateLP();
	
	std::cout << "\n================================================================================";

	delete [] idxUB;
	delete [] valUB;
	delete [] bts;
	delete [] idxFO;
	delete [] valFO;
	delete [] idxN;
	delete [] objOrig;

	return status;
}


void PreModelo::preencheMapAtendimentoAluno( VariablePre *&var )
{
	// Resultado da alocação de alunos no pre-modelo:

	if ( var->getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
	{
		Aluno *aluno = var->getAluno();
		int turma = var->getTurma();
		Disciplina *disciplina = var->getDisciplina();
		Campus *cp = var->getCampus();

		Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
		trio.set( cp->getId(), turma, disciplina );
				

		// Verificando consistência -------------------
			
		AlunoDemanda *alunoDemanda;
		#ifdef EQUIVALENCIA_DESDE_O_INICIO
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
				alunoDemanda = aluno->getAlunoDemandaEquiv( disciplina );
			else alunoDemanda = aluno->getAlunoDemanda( disciplina->getId() );
		#else
			alunoDemanda = aluno->getAlunoDemanda( disciplina->getId() );
		#endif

		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > atendimentos = 
			problemData->mapAluno_CampusTurmaDisc[ aluno ];
			
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator
			itAtend = atendimentos.begin();

		bool repetido = false;

		for ( ; itAtend != atendimentos.end(); itAtend++ )
		{
			Disciplina *d = (*itAtend).third;
			int i = (*itAtend).second;
			if ( d->getId() == disciplina->getId() && i != turma )
			{
				if ( alunoDemanda!=NULL && alunoDemanda->getPrioridade() == 1 )
				{
					std::cout<<"\nErro em PreModelo::preencheMapAtendimentoAluno(): "
							<<"Aluno " << aluno->getAlunoId() << " ja esta alocado em i"
							<< i << " d"<< d->getId() << ". Alocacao repetida: i"
							<< turma << " d" << disciplina->getId() << "\n";
				}
				repetido = true;
			}
		}
		// --------------------------------------------
		if ( !repetido )
		{
			problemData->mapAluno_CampusTurmaDisc[ aluno ].add( trio );

			GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > ggroup = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ];
			if ( ggroup.find( alunoDemanda ) == ggroup.end() )
				problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].add( alunoDemanda );
		}
	}
	
}


void PreModelo::atualizarDemandasEquiv( int campusId, int prioridade, int r )
{	
	#ifdef PRINT_LOGS
	ofstream outEquiv;
	std::string fileName( this->getEquivPreFileName(campusId, prioridade, r) );
	outEquiv.open( fileName, ofstream::app);
	if ( !outEquiv )
	{
		std::cerr<<"\nAbertura do arquivo " << fileName
			<< " falhou em TaticoIntAlunoHor::atualizarDemandasEquiv().\n";
	}
	#endif

	int idAlDemanda = problemData->retornaMaiorIdAlunoDemandas();

	ITERA_GGROUP_LESSPTR ( itVar, this->solVarsPreS, VariablePre )
	{
		if ( (*itVar)->getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{
			VariablePre *v = *itVar; // s_{a,i,d,cp}
			Aluno *aluno = v->getAluno();
			Disciplina *disciplina = v->getDisciplina();
			int turma = v->getTurma();

			if ( v->getCampus()->getId() != campusId )
				continue;
			
			// Continue, caso seja alocação de prioridade passada
			if ( prioridade==2 && problemData->procuraAlunoDemanda(disciplina->getId(), aluno->getAlunoId(), 1) != NULL )
				continue;
			
			AlunoDemanda *alDemOrig = problemData->procuraAlunoDemanda( disciplina->getId(), aluno->getAlunoId(), prioridade );
			
			if ( alDemOrig==NULL ) // disciplina substituta
			{				
				AlunoDemanda *alDem = problemData->atualizaAlunoDemandaEquiv( turma, disciplina, campusId, aluno, prioridade );
				
				if ( alDem==NULL )
				{
					std::cout<<"\nErro em void PreModelo::atualizarDemandasEquiv( int campusId, int prioridade, int r ). AlunoDemanda null.";
					continue;
				}
				#ifdef PRINT_LOGS
				if ( outEquiv )
				{
					outEquiv << "\nAluno "<<aluno->getAlunoId()<<" Disc original "<< alDem->demandaOriginal->getDisciplinaId()
						<<" Disc substituta "<<disciplina->getId()<<" Prioridade "<< alDem->getPrioridade();
				}
				#endif
			}
		}
	}

	#ifdef PRINT_LOGS
	if ( outEquiv )
	{
		outEquiv.close();
	}
	#endif

	problemData->preencheMapDisciplinaAlunosDemanda(false);
	problemData->imprimeResumoDemandasPorAlunoPosEquiv();
}

void PreModelo::imprimeSolVarsPre( int campusId, int prioridade, int cjtAlunosId )
{

   // teste
   char solPosFilename[1024];
   strcpy( solPosFilename, getSolVarsPreFileName( campusId, prioridade, cjtAlunosId, 0 ).c_str() );
   FILE * fout = fopen( solPosFilename, "wt" );
   
   ITERA_GGROUP_LESSPTR ( itSolX, solVarsXPre, VariablePre )
   {      
	   VariablePre *v = *itSolX;
	   
	   fprintf( fout, "%s = %f\n", v->toString().c_str(), v->getValue() );
		   
   }
   fclose( fout );

}


int PreModelo::localBranchingPre( double * xSol, double maxTime )
{
   // Adiciona restrição de local branching
   int status = 0;
   int nIter = 0;
   int * idxSol = new int[ lp->getNumCols() ];
   CPUTimer tempoLB;

   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      idxSol[ i ] = i;
   }



   /*FILE * fin = fopen( "solLBAtualPre.bin", "rb" );
   if ( fin == NULL )
   {
      std::cout << "\nErro em PreModelo::solvePreTaticoCjtAlunos( int campusId, int prioridade, int cjtAlunosId ):"
         << "\nArquivo " << "solLBAtualPre.bin" << " nao pode ser aberto.\n";
   }
   else
   {
      int nCols = 0;

      fread( &nCols, sizeof( int ), 1, fin );

      for ( int i = 0; i < nCols; i++ )
      {
         double auxR;
         fread( &auxR, sizeof( double ), 1, fin );

         xSol[ i ] = auxR;
      }

      fclose( fin );
   }

   lp->copyMIPStartSol(lp->getNumCols(),idxSol,xSol);*/

   double objAtual = 100000000000.0;
   double okIter = true;

   int k = -100;

   tempoLB.reset();
   tempoLB.start();

   int tempoIter = 600;

   while (okIter)
   {
      VariablePreHash::iterator vit = vHashPre.begin();

      OPT_ROW nR( 50, OPT_ROW::GREATER, 0.0, "LOCBRANCH" );
      double rhsLB = k;

      while ( vit != vHashPre.end() )
      {
         if ( vit->first.getType() == VariablePre::V_PRE_OFERECIMENTO )
         {
            if ( xSol[vit->second] > 0.1 )
            {
               rhsLB += 1.0;
               nR.insert( vit->second,1.0 );
            }
            else
            {
               nR.insert( vit->second, -1.0 );
            }
         }

         vit++;
      }

      nR.setRhs( rhsLB );

      lp->addRow( nR );
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      //lp->setMIPRelTol( 0.02 );
      //lp->setNodeLimit( 1 );
      lp->setPolishAfterNode(101);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 4 );
      lp->setCuts(4);
      //lp->setNoCuts();
      lp->setVarSel(4);
      lp->setHeurFrequency( 1.0 );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );

      status = lp->optimize( METHOD_MIP );

      lp->getX( xSol );

      FILE * fout = fopen( "solLBAtualPre.bin", "wb" );
		if ( fout == NULL )
		{
			std::cout << "\nErro em PreModelo::localBranchingPre( ):"
					<< "\nArquivo " << "solLBAtualPre.bin" << " nao pode ser aberto.\n";
		}
		else
		{
			int nCols = lp->getNumCols();

			fwrite( &nCols, sizeof( int ), 1, fout );
			for ( int i = 0; i < lp->getNumCols(); i++ )
			{
				fwrite( &( xSol[ i ] ), sizeof( double ), 1, fout );
			}

			fclose( fout );
		}

      double objN = lp->getObjVal();

      if ( fabs(objN-objAtual) < 0.0001 && k <= -1000 )
      {
         okIter = false;
      }
      else if (fabs(objN-objAtual) < 0.0001 )
      {
         k -= 50;
         tempoIter += 300;
      }

      if ( k == -50 )
      {
          tempoIter += 300;
          k+=30;
      }

      if ( k == -100 )
          k += 50;

      objAtual = objN;

      if ( objAtual < 0.001 )
         okIter = false;

      tempoLB.stop();
      double tempoAtual = tempoLB.getCronoTotalSecs();
      tempoLB.start();
      if ( tempoAtual >= maxTime )
      {
         okIter = false;
         tempoLB.stop();
      }

      int * idxs = new int[ 1 ];
      idxs[ 0 ] = lp->getNumRows() - 1;
      lp->delSetRows( 1, idxs );
      lp->updateLP();
      delete [] idxs;
   }

   delete [] idxSol;

   return status;
}

void PreModelo::polishPreTatico(double *xSol, double maxTime, int percIniFixado, int percMinFixado)
{
	std::cout<<"\n\nIniciando polishPreTatico...\n"; fflush(0);

   int status = 0;
   int * idxSol = new int[ lp->getNumCols() ];
   double *ubVars = new double[ lp->getNumCols() ];
   double *lbVars = new double[ lp->getNumCols() ];

   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      idxSol[ i ] = i;
   }

   double objAtual = 100000000000.0;
   double okIter = true;
   CPUTimer tempoPol;

   tempoPol.reset();
   tempoPol.start();

   srand(123);

   lp->getUB(0,lp->getNumCols()-1,ubVars);
   lp->getLB(0,lp->getNumCols()-1,lbVars);

   int tempoIter = 500;
   int perc = percIniFixado;

   int nIter = 1;

   int *idxBds = new int[lp->getNumCols()*2];
   double *valBds = new double[lp->getNumCols()*2];
   BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()*2];
   int nChgBds = 0;



// ------------------------------------------------------------
// Procura rapidamente a solução exata, caso já se esteja perto do ótimo

//#ifdef SOLVER_CPLEX
//      lp->updateLP();
//      lp->setNodeLimit( 100000000 );
//      lp->setTimeLimit( 1800 );
//      lp->setMIPRelTol( 0.01 );
//      lp->setPolishAfterNode(1);
//      lp->setPolishAfterTime(100000000);
//      lp->setMIPEmphasis( 4 );
//      lp->setNoCuts();
//      lp->setVarSel(4);
//      lp->setNodeLimit(200);
//      lp->setHeurFrequency( 1.0 );
//#endif
//#ifdef SOLVER_GUROBI
//      lp->setNodeLimit( 100000000 );
//      lp->setTimeLimit( 1800 );
//      lp->setMIPRelTol( 0.1 );
//      lp->setMIPEmphasis( 0 );
//      lp->setNoCuts();
//      lp->setVarSel(4);
//      lp->setNodeLimit(200);
//      lp->setHeurFrequency( 1.0 );
//      lp->updateLP();
//#endif
//
//      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
//      lp->updateLP();
//	  
//      status = lp->optimize( METHOD_MIP );
//	  
//      lp->getX( xSol );
//	  
//	  if ( lp->getMIPGap() * 100 < 5.0 )
//	  {
//		  okIter = false;
//		  cout << "\nPolish desnecessario, gap =" << lp->getMIPGap() * 100 << std::endl;
//	  }

// ------------------------------------------------------------



#ifdef SOLVER_CPLEX
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setPolishAfterIntSol(100000000);
      lp->setPolishAfterNode(100000000);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 4 );
      lp->setCuts(0);
      lp->setVarSel(4);
      lp->setHeurFrequency( 1.0 );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
      lp->updateLP();
#endif

#ifdef SOLVER_GUROBI
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 0 );
	  lp->setNoCuts();
      lp->setHeurFrequency( 1.0 );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
	  lp->setPreSolveIntensity( OPT_LEVEL_AUTO );
      lp->updateLP();
#endif


   while (okIter)
   {
	   fflush(NULL);
	   std::cout << "\n------>\nPerc = " << perc << "\nTempoIter = " << tempoIter << endl;
	   fflush(NULL);

      VariablePreHash::iterator vit = vHashPre.begin();

      vit = vHashPre.begin();

      nChgBds = 0;
      while ( vit != vHashPre.end() )
      {
         if ( vit->first.getType() == VariablePre::V_PRE_ABERTURA )
         {
            if ( rand() % 100 >= perc  )
            {
               vit++;
               continue;
            }

            if ( xSol[vit->second] > 0.1 )
            {
               idxBds[nChgBds] = vit->second;
               valBds[nChgBds] = (int)(xSol[vit->second]+0.5);
               bts[nChgBds] = BOUNDTYPE::BOUND_LOWER;
               nChgBds++;
            }
            else
            {
               idxBds[nChgBds] = vit->second;
               valBds[nChgBds] = 0.0;
               bts[nChgBds] = BOUNDTYPE::BOUND_UPPER;
               nChgBds++;
            }
         }

         vit++;
      }

      lp->chgBds(nChgBds,idxBds,bts,valBds);

#ifdef SOLVER_CPLEX
      lp->setTimeLimit( tempoIter );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
#elif SOLVER_GUROBI
      lp->setTimeLimit( tempoIter );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
	  
	  //if ( tempoIter > 800 ) lp->setPreSolveIntensity( OPT_LEVEL1 );
	  //else lp->setPreSolveIntensity( OPT_LEVEL2 );
#endif

	  lp->updateLP();

      status = lp->optimize( METHOD_MIP );

      double objN = lp->getObjVal();

      lp->getX( xSol );

      if ( fabs(objN-objAtual) < 0.0001 && perc < percMinFixado )
      {
   //      if ( nIter == 2 )
   //      {
			//std::cout << "\nNenhuma melhora na solucao atual e percentual minimo no polish alcancado = " << perc << endl;
   //         okIter = false;
   //         perc = (percIniFixado + percMinFixado) / 2;
   //         tempoIter = 500;
   //      }
   //      else
   //      {
            perc = (percIniFixado + percMinFixado) / 2;
            tempoIter = 500;
         //}
      }
	  else if ( fabs(objN-objAtual) < 0.0001 && perc >= percMinFixado )
      {
         perc -= 5;
         tempoIter += 40;
		 lp->setCuts(1);
      }

      //if ( perc < percMinFixado && nIter == 1 )
      //{
      //   nIter++;
      //}

      if ( objN < 0.001 )
         okIter = false;

      objAtual = objN;

      tempoPol.stop();
      double tempoAtual = tempoPol.getCronoTotalSecs();
      tempoPol.start();
      if ( tempoAtual >= maxTime )
      {
		  std::cout << "\nTempo maximo de polish alcancado = " << tempoAtual << endl;
          okIter = false;
          tempoPol.stop();
      }

      // Volta bounds
      vit = vHashPre.begin();
      nChgBds = 0;

      while ( vit != vHashPre.end() )
      {
         if ( vit->first.getType() == VariablePre::V_PRE_ABERTURA )
         {
            idxBds[nChgBds] = vit->second;
            valBds[nChgBds] = lbVars[vit->second];
            bts[nChgBds] = BOUNDTYPE::BOUND_LOWER;
            nChgBds++;
            idxBds[nChgBds] = vit->second;
            valBds[nChgBds] = ubVars[vit->second];
            bts[nChgBds] = BOUNDTYPE::BOUND_UPPER;
            nChgBds++;
            //lp->chgLB(vit->second,lbVars[vit->second]);
            //lp->chgUB(vit->second,ubVars[vit->second]);
         }
         vit++;
      }
      lp->chgBds(nChgBds,idxBds,bts,valBds);
      lp->updateLP();
   }

   
    int atends=0;
    for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
    {
		if ( vit->first.getType() == VariablePre::V_PRE_SLACK_DEMANDA )
        {
			double value = (int) (xSol[vit->second] + 0.5);
			if ( value == 0 )
				atends++;
		}
	}
	std::cout << "\nNro de aluno-demanda atendidos ao fim do polish = " << atends << endl;

   
	// -------------------------------------------------------------
    // Garante que não dará erro se houver um getX depois desse polish,
    // já que o lp sobre alteração nos bounds no final.
    lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
	lp->setTimeLimit( 50 );
	status = lp->optimize( METHOD_MIP );
	lp->getX(xSol);
	// -------------------------------------------------------------


    atends=0;
    for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
    {
		if ( vit->first.getType() == VariablePre::V_PRE_SLACK_DEMANDA )
        {
			double value = (int) (xSol[vit->second] + 0.5);
			if ( value == 0 )
				atends++;
		}
	}
	std::cout << "\nNro de aluno-demanda atendidos ao fim do polish = " << atends << endl;


   delete [] idxSol;
   delete [] ubVars;
   delete [] lbVars;
   delete [] idxBds;
   delete [] valBds;
   delete [] bts;
}


void PreModelo::solveAbreTurmasPorFaixasPreTatico(double *xSol, const double maxTime, const int percIni)
{
	std::cout<<"\n\nIniciando solveAbreTurmasPorFaixasPreTatico...\n"; fflush(0);

	// percIni indica a porcentagem inicial do nro de turmas por disciplina que é LIVRE.
	// Essa porcentagem (perc) aumenta aos poucos, até que 100% do problema esteja livre.

   int status = 0;
   int * idxSol = new int[ lp->getNumCols() ];
   double *ubVars = new double[ lp->getNumCols() ];

   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      idxSol[ i ] = i;
   }

   double objAtual = 100000000000.0;
   double okIter = true;
   CPUTimer tempoPol;

   tempoPol.reset();
   tempoPol.start();

   srand(123);

   lp->getUB(0,lp->getNumCols()-1,ubVars);

   double tempoIter = 500.0;
   int perc = percIni;

   int nIter = 1;

   int *idxBds = new int[lp->getNumCols()*2];
   double *valBds = new double[lp->getNumCols()*2];
   BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()*2];
   int nChgBds = 0;



// ------------------------------------------------------------
// Procura rapidamente a solução exata, caso já se esteja perto do ótimo

#ifdef SOLVER_CPLEX
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( 1800 );
      lp->setMIPRelTol( 0.01 );
      lp->setPolishAfterNode(1);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 4 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(200);
      lp->setHeurFrequency( 1.0 );
	  lp->setNumIntSols(0);
#endif
#ifdef SOLVER_GUROBI
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( 1800 );
      lp->setMIPRelTol( 0.1 );
      lp->setMIPEmphasis( 0 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(200);
      lp->setHeurFrequency( 1.0 );
	  lp->setNumIntSols(0);
#endif

      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
      lp->updateLP();
	  
      status = lp->optimize( METHOD_MIP );
	  
      lp->getX( xSol );
	  
	  if ( lp->getMIPGap() * 100 < 5.0 )
	  {
		  okIter = false;
		  cout << "\nsolveAbreTurmasPorFaixasPreTatico desnecessario, gap =" << lp->getMIPGap() * 100 << std::endl;
	  }

// ------------------------------------------------------------
	  
   int deltaPerc = 20;											// deltaPerc	= acréscimo de porcentagem por iteração
   int nit = 1 + (int) ( (100-percIni) / deltaPerc );			// nit			= numero de iterações
   double t = maxTime / (2*nit);								// t			= tempo máximo da primeira iteração
   double deltaTime=0;											// deltaTime	= acréscimo de tempo máximo por iteração
   if ( nit > 1 )
   {
	   // Acréscimo de tempo máximo por iteração calculado por progressão aritmética (PA).
	   // Para ser uma PA e respeitar o tempo máximo 'maxTime' e o incremento percentual 'deltaPerc',
	   // a única variante possível é o tempo máximo da 1a iteração 't'. Não mexer no cálculo das outras variáveis.

	   // termo inicial:	t
	   // razão:			deltaTime
	   // soma:				maxTime
	   // nro de termos:	nit
	   deltaTime = (2*maxTime - 2*t*nit) / (nit*nit - nit);
   }

   cout << "\n----Numero de iteracoes: " << nit
	    << "\nAcrescimo percentual por iteracao: " << deltaPerc
	    << "\nAcrescimo de tempo por iteracao: " << deltaTime
		<< "\nTempo total maximo: " << maxTime
	    << "\nPercentual inicial: " << percIni 
		<< "\n----" << std::endl;

   
   tempoIter = t;

#ifdef SOLVER_CPLEX
      lp->updateLP();
	  lp->setNumIntSols(0);
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setPolishAfterTime( (0.66)*tempoIter );
      lp->setMIPEmphasis( 4 );
      lp->setCuts(0);
      lp->setVarSel(4);
      lp->setHeurFrequency( 1.0 );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
      lp->updateLP();
#endif
#ifdef SOLVER_GUROBI
      lp->updateLP();
	  lp->setNumIntSols(0);
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setPolishAfterTime( (0.66)*tempoIter );
      lp->setMIPEmphasis( 0 );
	  lp->setNoCuts();
      lp->setHeurFrequency( 1.0 );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
	  lp->setPreSolveIntensity( OPT_LEVEL_AUTO );
      lp->updateLP();
#endif

	  if (okIter)
	  {
		 std::cout << "\n\n----------------------------------------------------";
		 std::cout << "\nInicializacao:\ntempoIter = " << tempoIter << "\nPerc = " << perc << endl << endl; 
	  }


	  int iter_id = 0;
	  bool fim100 = false;

   while (okIter)
   {
	   iter_id++;

      nChgBds = 0;

	  VariablePreHash::iterator vit = vHashPre.begin();	  
      for ( ; vit != vHashPre.end(); vit++ )
      {
         if ( vit->first.getType() == VariablePre::V_PRE_ABERTURA )
         {
			 VariablePre v = vit->first;
			 double totalTurmas = (double) v.getDisciplina()->getNumTurmas();
			 double turma = (double) v.getTurma();
			 
			 int value = (int)(xSol[vit->second]+0.5);

			 if ( ( turma/totalTurmas ) * 100 > perc  &&  value==0 )
             {
				 // Fixa em zero (100-perc) turmas de cada disciplina, a não ser que ela já esteja aberta

				 idxBds[nChgBds] = vit->second;
				 valBds[nChgBds] = 0.0;
				 bts[nChgBds] = BOUNDTYPE::BOUND_UPPER;
				 nChgBds++;
             }
         }
      }

      lp->chgBds(nChgBds,idxBds,bts,valBds);

#ifdef SOLVER_CPLEX
      lp->setTimeLimit( tempoIter );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
#elif SOLVER_GUROBI
      lp->setTimeLimit( tempoIter );
	  lp->setPolishAfterTime( (0.66)*tempoIter );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
#endif

	  lp->updateLP();

	
	  // ----------------------------------------------------------------------------
	  // Print bounds and partial results
	  std::stringstream ss;
	  ss << "preModelo-BoundTurmas" << iter_id << "-"
		 << "Time" << tempoIter << "Perc" << perc << ".txt";

	  ofstream fout( ss.str(), ios_base::out );
	  if ( fout == NULL ){
		std::cout << "\nErro. Arquivo " << ss.str() << " nao pode ser aberto.\n";
	  }
	  
	  int nTurmas = 0;
	  int nAtends = 0;
	  int nAtendsPT = 0;
      for ( VariablePreHash::iterator vit = vHashPre.begin(); vit != vHashPre.end(); vit++ )
      {
         if ( vit->first.getType() == VariablePre::V_PRE_ABERTURA )
         {
			 VariablePre v = vit->first;
			 int value = (int)(xSol[vit->second]+0.5);
			 if ( value == 1 )
				 nTurmas++;

			 fout << "\n" << lp->getLB( vit->second ) << " <= " << v.toString().c_str() << " <= " << lp->getUB( vit->second );			 
		 }
	  }
	  fout.close();
	  // ----------------------------------------------------------------------------
	  

	  if ( perc >= 100 )
	  {
	  	  std::cout << "\n100 \% de turmas livres alcancado" << endl;
		  fim100 = true;
	  }


      status = lp->optimize( METHOD_MIP );

      lp->getX( xSol );

      double objN = lp->getObjVal();


	  // --------------------------------
	  // Print solution
	  string solName("solPreFases-");
	  
	  stringstream ss2;
	  ss2 << solName;
	  ss2 << "Time" << tempoIter << "Perc" << perc;

	  writeSolTxt( ss2.str() + ".txt", xSol, false );
	  writeSolBin( ss2.str() + ".bin", xSol );
	  // --------------------------------
	  

      if ( objN < 0.0001 )
         okIter = false;

      objAtual = objN;

      tempoPol.stop();
      double tempoAtual = tempoPol.getCronoTotalSecs();
      tempoPol.start();
      if ( tempoAtual >= maxTime )
      {
		  std::cout << "\nTempo maximo de otimizacao por fases alcancado = " << tempoAtual << endl;		  
      }

	  if ( fim100 && tempoAtual >= maxTime )
	  {
		  std::cout << "\n\n----Fim da otimizacao por fases. Tempo total = " << tempoAtual << endl << endl;
		  tempoPol.stop();
		  okIter = false;
	  }

	  if ( fim100 && fabs(objN-objAtual) < 0.01 )
	  {
		  std::cout << "\n\n----Sem melhorias.";
		  std::cout << "\n----Fim da otimizacao por fases. Tempo total = " << tempoAtual << endl << endl;
		  tempoPol.stop();
		  okIter = false;
	  }

	  if ( okIter )
	  {
		  perc += deltaPerc;
		  if ( perc > 100) perc = 100;

		  tempoIter += deltaTime;
		//  lp->setCuts(1);
	  
		  std::cout << "\n\n----------------------------------------------------";
		  std::cout << "\nAtualizacao:\ntempoIter = " << tempoIter << "\nPerc = " << perc << endl << endl;
	  }

      // Volta bounds
      vit = vHashPre.begin();
      nChgBds = 0;

      while ( vit != vHashPre.end() )
      {
         if ( vit->first.getType() == VariablePre::V_PRE_ABERTURA )
         {
            idxBds[nChgBds] = vit->second;
            valBds[nChgBds] = ubVars[vit->second];
            bts[nChgBds] = BOUNDTYPE::BOUND_UPPER;
            nChgBds++;
         }
         vit++;
      }
      lp->chgBds(nChgBds,idxBds,bts,valBds);
      lp->updateLP();
   }

   delete [] idxSol;
   delete [] ubVars;
   delete [] idxBds;
   delete [] valBds;
   delete [] bts;
}





 /*******************************************************************************************************************
								CRIAÇÃO DE VARIAVEIS DO PRE-MODELO   
  ******************************************************************************************************************/



int PreModelo::cria_preVariaveis(  int campusId, int prioridade, int grupoAlunosId, int r )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif

	
	timer.start();
	num_vars += cria_preVariavel_alunos( campusId, grupoAlunosId, prioridade, r );  // a_{i,d,oft,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"a\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	
   #ifndef SALA_UNICA_POR_TURMA  
	timer.start();
	num_vars += cria_preVariavel_creditos( campusId, grupoAlunosId, prioridade, r );   // x_{i,d,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
   #endif

	if ( PREMODELO_POR_TURNO )
	{
	timer.start();
	num_vars += cria_preVariavel_oferecimentos_turno( campusId, grupoAlunosId, prioridade, r ); // q_{i,d,s,g}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"q\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	

	timer.start();
	#ifdef EQUIVALENCIA_DESDE_O_INICIO
		num_vars += cria_preVariavel_aloca_aluno_turma_disc_turno_equiv( campusId, prioridade ); // s
	#else
		num_vars += cria_preVariavel_aloca_aluno_turma_disc_turno( campusId, grupoAlunosId, prioridade, r ); // s
	#endif
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"s\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	}

	timer.start();
	num_vars += cria_preVariavel_oferecimentos( campusId, grupoAlunosId, prioridade, r ); // o_{i,d,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_abertura( campusId, grupoAlunosId, prioridade, r );   // z_{i,d}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_aloc_alunos( campusId, grupoAlunosId, prioridade, r );   // b_{i,d,c}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"b\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	/*
	timer.start();
	num_vars += cria_preVariavel_folga_compartilhamento_incomp( campusId, grupoAlunosId, prioridade ); // bs_{d,oft}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"bs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	*/

	timer.start();
	num_vars += cria_preVariavel_folga_proibe_compartilhamento( campusId, grupoAlunosId, prioridade, r ); // fc_{i,d,c1,c2}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fc\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	
	timer.start();
	num_vars += cria_preVariavel_limite_sup_creds_sala( campusId ); // Hs
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"Hs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	
	timer.start();
	num_vars += cria_preVariavel_usa_periodo_unid( campusId ); // Hs
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"Hs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	// Exclusivas para modelo por aluno:

	if ( problemData->parametros->otimizarPor == "ALUNO" )
	{


		timer.start();
		num_vars += cria_preVariavel_folga_prioridade_inf( campusId, prioridade, grupoAlunosId ); // fpi
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fpi\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif
		

		timer.start();
		num_vars += cria_preVariavel_folga_prioridade_sup( campusId, prioridade, grupoAlunosId ); // fps
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fps\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif
		

		timer.start();
		num_vars += cria_preVariavel_folga_abre_turma_sequencial( campusId, grupoAlunosId, prioridade, r ); // ft
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"ft\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif

		/*timer.start();
		num_vars += cria_preVariavel_folga_distribuicao_aluno( campusId, grupoAlunosId, prioridade ); // fda
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fda\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif*/
		
	#ifndef UNIT
	#ifndef UNIRITTER
	#ifndef KROTON
		timer.start();
		num_vars += cria_preVariavel_turmas_compartilhadas( campusId, grupoAlunosId, prioridade ); // w_{i,d,i',d'}
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_variaveis
			std::cout << "numVars \"w\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
			numVarsAnterior = num_vars;
		#endif
	#endif
	#endif
	#endif


		timer.start();
		num_vars += cria_preVariavel_folga_turma_mesma_disc_sala_dif( campusId, grupoAlunosId, prioridade, r ); // fs_{d,s}
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif	

		timer.start();
		num_vars += cria_preVariavel_formandosNaTurma( campusId, grupoAlunosId, prioridade, r ); // f_{i,d,cp}
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"f\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif	


		#if defined(ALUNO_UNICO_CALENDARIO) || defined(ALUNO_TURNOS_DA_DEMANDA)
		timer.start();
		num_vars += cria_preVariavel_turmaTurno( campusId, grupoAlunosId, prioridade, r ); // v_{i,d,tt,s}
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"v\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif	

		timer.start();
		num_vars += cria_preVariavel_turma_so_sabado( campusId, grupoAlunosId, prioridade, r ); // sab_{i,d,s}
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"sab\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif
		#endif

	#ifndef PREMODELO_AGRUPA_ALUNO_POR_CURSO

		#ifndef UNIT
		#ifndef UNIRITTER
		if ( FIXA_SALAS_ESTIMATURMAS )
		{
			timer.start();
			num_vars += cria_preVariavel_aluno_sala( campusId, grupoAlunosId, prioridade, r ); // as_{a,s}
			timer.stop();
			dif = timer.getCronoCurrSecs();
			#ifdef PRINT_cria_variaveis
			std::cout << "numVars \"as\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
			numVarsAnterior = num_vars;
			#endif	
		}
		#endif
		#endif

		timer.start();
		num_vars += cria_preVariavel_folga_demanda_disciplina_aluno( campusId, grupoAlunosId, prioridade ); // fd_{d,a}
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif

		
		if ( !PREMODELO_POR_TURNO )
		{
		timer.start();
		#ifdef EQUIVALENCIA_DESDE_O_INICIO
			num_vars += cria_preVariavel_aloca_aluno_turma_disc_equiv( campusId, prioridade ); // s
		#else
			num_vars += cria_preVariavel_aloca_aluno_turma_disc( campusId, grupoAlunosId, prioridade, r ); // s
		#endif
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"s\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif
		}
		
		timer.start();
		if ( ! PREMODELO_SEM_VAR_N )
			num_vars += cria_preVariavel_nroAlunos_turma( campusId, grupoAlunosId, prioridade, r ); // n_{i,d,cp}
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"n\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif	

		timer.start();
		if ( PREMODELO_POR_TURNO ) num_vars += cria_preVariavel_soma_cred_sala_por_turno( campusId ); // xcs_{s,g}	
		else num_vars += cria_preVariavel_soma_cred_sala( campusId ); // xcs_{s}
		timer.stop();
		dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"xcs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif
		
		timer.start();
		num_vars += cria_preVariavel_alunos_mesma_turma_pratica( campusId, prioridade ); // ss
		timer.stop();
		dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"ss\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif
		
		timer.start();
		num_vars += cria_preVariavel_folga_minimo_demanda_por_aluno( campusId, prioridade ); // fmd
		timer.stop();
		dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fmd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif	
		
		timer.start();
		num_vars += cria_preVariavel_folga_ocupacao_sala( campusId, prioridade ); // fos
		timer.stop();
		dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fos\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif			
		

	#endif

	#ifdef PREMODELO_AGRUPA_ALUNO_POR_CURSO

		// usando variavel "a_{i,d,s,oft}"
		/*timer.start();
		num_vars += cria_preVariavel_nroAlunos_turma_oferta( campusId, grupoAlunosId, prioridade, r ); // no_{i,d,oft}
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"no\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif*/	
		
		timer.start();
		num_vars += cria_preVariavel_folga_demanda_oferta( campusId, grupoAlunosId, prioridade, r ); // fd_{d,oft}
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif		


	#endif


	}

	timer.start();
	num_vars += cria_preVariavel_usa_disciplina( campusId, prioridade ); // k_{d}
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"k\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif	
	
	timer.start();
	num_vars += cria_preVariavel_usa_periodo_unid( campusId ); // uu_{u,oft,p}
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"uu\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif	

	timer.start();
	num_vars += cria_preVariavel_usa_periodo_sala( campusId ); // us_{s,oft,p}
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"us\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif	

	timer.start();
	num_vars += cria_preVariavel_folga_periodo_unid( campusId, prioridade ); // fuu_{oftt,tp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fuu\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif	

	timer.start();
	num_vars += cria_preVariavel_folga_periodo_sala( campusId, prioridade ); // fus_{oft,p}
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fus\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif	

	timer.start();
	num_vars += cria_preVariavel_distrib_alunos( campusId ); // da_{cp,d}
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"da\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif


	timer.start();
	//num_vars += cria_preVariavel_folga_disponib_prof( campusId ); // fp_{d,g}
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fp\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif
	

	return num_vars;
}

int PreModelo::add_preVariaveis(  int campusId, int prioridade, int grupoAlunosId, int r )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif

	#ifdef PREMODELO_AGRUPA_ALUNO_POR_CURSO
			
		//#ifdef EQUIVALENCIA_DESDE_O_INICIO
		//	timer.start();
		//	num_vars += cria_preVariavel_aloca_aluno_turma_disc_equiv( campusId, grupoAlunosId, prioridade, r ); // s_{i,d,a}
		//	timer.stop();
		//	dif = timer.getCronoCurrSecs();
		//	#ifdef PRINT_cria_variaveis
		//	std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		//	numVarsAnterior = num_vars;
		//	#endif				
		//#else
			timer.start();
			num_vars += cria_preVariavel_aloca_aluno_turma_disc( campusId, grupoAlunosId, prioridade, r ); // s_{i,d,a}
			timer.stop();
			dif = timer.getCronoCurrSecs();
			#ifdef PRINT_cria_variaveis
			std::cout << "numVars \"s\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
			numVarsAnterior = num_vars;
			#endif
		//#endif
		
		timer.start();
		num_vars += cria_preVariavel_soma_cred_sala( campusId ); // xcs
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"xcs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
		#endif

		#ifndef UNIT
		#ifndef UNIRITTER
			timer.start();
			num_vars += cria_preVariavel_aluno_sala( campusId, grupoAlunosId, prioridade, r ); // as_{a,s}
			timer.stop();
			dif = timer.getCronoCurrSecs();
			#ifdef PRINT_cria_variaveis
			std::cout << "numVars \"as\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
			numVarsAnterior = num_vars;
			#endif	
		#endif
		#endif
	#endif

	return num_vars;
}


// q_{i,d,s,g}
int PreModelo::cria_preVariavel_oferecimentos_turno( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;

   Disciplina * disciplina = NULL;
      
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  Campus *cp = *itCampus;

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 Sala *sala = itCjtSala->salas.begin()->second;

            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disc );

			    if ( P_ATUAL==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
			    {
					if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
						continue;
			    }
			   
				int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );

 				if ( discGrupoAlunosId > grupoAlunosId || 
					discGrupoAlunosId == 0 )
				{
					continue;
				}

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {

				    // -----------------
				    // Solução Inicial
				   if ( problemData->existeSolTaticoInicial() )
				    {
						bool fixarSala = false;
						Sala *salaSolInic = problemData->getSolTaticoInicial()->getSala( campusId, disciplina, turma, fixarSala );
						if ( salaSolInic != NULL && salaSolInic != sala && fixarSala )
				 			 continue;
				    }
				    // -----------------

					for ( int turno = 1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
					{
						if ( ! disciplina->possuiTurno( turno ) ) continue;
						//if ( ! itCjtSala->salas.begin()->second->possuiTurno( turno ) ) continue;					

						 VariablePre v;
						 v.reset();
						 v.setType( VariablePre::V_PRE_OFERECIMENTO_TURNO );
						 v.setTurma( turma );            // i
						 v.setDisciplina( disciplina );  // d
						 v.setUnidade( *itUnidade );     // u
						 v.setSubCjtSala( *itCjtSala );  // tps
						 v.setTurno( turno );			 // g

						 if ( vHashPre.find( v ) == vHashPre.end() )
						 {
							double custo = 0.0;

							vHashPre[ v ] = lp->getNumCols();
						 
							double lowerBound = 0.0;
							double upperBound = 1.0;
						
							OPT_COL col( OPT_COL::VAR_BINARY, custo, lowerBound, upperBound, ( char* )v.toString().c_str() );
                          
							lp->newCol( col );

							num_vars++;						  
						 }
					}
                }
            }
         }
      }
   }

	return num_vars;
}


// x_{i,d,u,s}
int PreModelo::cria_preVariavel_creditos( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  Campus *cp = *itCampus;

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disc );
				
				int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );

 				if ( discGrupoAlunosId > grupoAlunosId || 
					 discGrupoAlunosId == 0 )
				{
					continue;
				}

                for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
                {
                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_CREDITOS );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps

                     if ( vHashPre.find( v ) == vHashPre.end() )
                     {
						 double coef = 0.0;

						 int upperBound = disciplina->getCredTeoricos() + disciplina->getCredPraticos();						 
						 double lowerBound = 0.0;

						 if ( upperBound == 0 )
						 {
							std::cout<<"\nAtencao em cria_preVariavel_creditos: disciplina "
								<<disciplina->getId()<<" possui 0 creditos associados.\n";
						 }

						 if ( FIXAR_P1 && P_ATUAL > 1 )
						 {
							int value = this->fixaLimitesVariavelPre( &v );
							if ( value > 0 ) // fixa!
							{
						 		lowerBound = value;
								upperBound = lowerBound;
							}
						 }
						 else if ( discGrupoAlunosId < grupoAlunosId )
						 {
							lowerBound = this->fixaLimitesVariavelPre( &v );
							upperBound = lowerBound;
						 }

						 if (r>1)
						 {
							int value = this->fixaLimitesVariavelPre( &v );
							if ( value > 0 ) // fixa!
							{
						 		lowerBound = value;
								upperBound = lowerBound;
							}
						 }

                         vHashPre[ v ] = lp->getNumCols();

                         OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
                          
						 lp->newCol( col );						 

						 num_vars++;						  
                     }
                 }
             }
         }
      }
   }

	return num_vars;
}

// o_{i,d,s}
int PreModelo::cria_preVariavel_oferecimentos( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;

   Disciplina * disciplina = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  Campus *cp = *itCampus;

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 Sala *sala = itCjtSala->salas.begin()->second;

            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disc );
			   int discId = disciplina->getId();

			   if ( P_ATUAL==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
			   {
					if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
						continue;
			   }
			   
				int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );

 				if ( discGrupoAlunosId > grupoAlunosId || 
					discGrupoAlunosId == 0 )
				{
					continue;
				}

				if ( disciplina->getNumTurmas() <= 0 )
					std::cout<<"";

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_OFERECIMENTO );
                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps

                     if ( vHashPre.find( v ) == vHashPre.end() )
                     {
                        double custo = 0.0;
						double lowerBound = 0.0;
						double upperBound = 1.0;
						
						 if ( FIXAR_P1 && P_ATUAL > 1 )
						 {
							int value = this->fixaLimitesVariavelPre( &v );
							if ( value > 0 ) // fixa!
							{
						 		lowerBound = value;
								upperBound = lowerBound;
							}
						 }
						 else if ( discGrupoAlunosId < grupoAlunosId )
						 {
							lowerBound = this->fixaLimitesVariavelPre( &v );
							upperBound = lowerBound;
						 }

						 if (r>1)
						 {
							int value = this->fixaLimitesVariavelPre( &v );
							if ( value > 0 ) // fixa!
							{
						 		lowerBound = value;
								upperBound = lowerBound;
							}
						 }

						 // -----------------
						 // Solução Inicial
						 bool fixarSala = false;
						 if ( problemData->existeSolTaticoInicial() )
						 {						 
							 bool criar = problemData->getSolTaticoInicial()->criar(v, fixarSala);
							 if ( !criar ) continue;					
							 if ( fixarSala ) lowerBound = upperBound;	 
						 }
						 // -----------------
				   		
                        vHashPre[ v ] = lp->getNumCols();
						 
                        OPT_COL col( OPT_COL::VAR_BINARY, custo, lowerBound, upperBound, ( char* )v.toString().c_str() );
                          
						lp->newCol( col );

						num_vars++;						  
                     }
                }
            }
         }
      }
   }

	return num_vars;
}

// z_{i,d,cp}
int PreModelo::cria_preVariavel_abertura( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	
   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_disciplina, it_CpDisc->second, int )
      {
		 Disciplina *disciplina = problemData->refDisciplinas[ *it_disciplina ];
		 		 	
		 int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );
		 
 		 if ( discGrupoAlunosId > grupoAlunosId || 
			  discGrupoAlunosId == 0 )
		 {
			 continue;
		 }
		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {		 
            VariablePre v;
            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );	    // cp

            if ( vHashPre.find(v) == vHashPre.end() )
            {               
			    double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					double custo = cp->getCusto();
							 
					coef = -10 * custo * disciplina->getTotalCreditos();
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					double custo = cp->getCusto();

					coef = custo * disciplina->getTotalCreditos();
				}

#ifdef UNIT
            coef = 5.0;
#endif
						                
						 
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( FIXAR_P1 && P_ATUAL > 1 )
				{
					int value = this->fixaLimitesVariavelPre( &v );
					if ( value > 0 ) // fixa!
					{
						lowerBound = value;
						upperBound = lowerBound;
					}
				}
				else if ( discGrupoAlunosId < grupoAlunosId )
				{
					lowerBound = this->fixaLimitesVariavelPre( &v );
					upperBound = lowerBound;
				}

				if (r>1)
				{
					int value = this->fixaLimitesVariavelPre( &v );
					if ( value > 0 ) // fixa!
					{
						lowerBound = value;
						upperBound = lowerBound;
					}
				}


				// -----------------
				// Solução Inicial
				if ( problemData->existeSolTaticoInicial() )
				{
					bool fixarAbertura = false;
					bool existe = problemData->getSolTaticoInicial()->existeTurma( campusId, disciplina, turma, fixarAbertura );
					if ( existe && fixarAbertura )
				 		lowerBound = upperBound;
				}
				// -----------------

				vHashPre[v] = lp->getNumCols();

				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				num_vars++;
            }
         }
      }
   }

	return num_vars;
}

// a_{i,d,oft,s}
int PreModelo::cria_preVariavel_alunos( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	
	std::map< Disciplina*, std::map< Oferta*, int /*qtdMaxDem*/, LessPtr<Oferta> >, LessPtr<Disciplina> > mapDiscOftQtd;

	ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
	{
		Disciplina* disciplina = *it_disc;
					
		// Listando todas as ofertas que contêm uma disciplina especificada.
		GGroup< Oferta *, LessPtr< Oferta > > ofertas;
		#ifdef EQUIVALENCIA_DESDE_O_INICIO
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
				ofertas = problemData->discOfertasEquiv[ it_disc->getId() ];
			else
				ofertas = problemData->ofertasDisc[ it_disc->getId() ];
		#else	
			ofertas = problemData->ofertasDisc[ it_disc->getId() ];
		#endif

		ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
		{
			Oferta* oft = *itOferta;

			if ( oft->campus->getId() != campusId )
				continue;
						
			int qtdMaxDem=0;
			#ifdef EQUIVALENCIA_DESDE_O_INICIO
				if ( problemData->parametros->considerar_equivalencia_por_aluno )
				qtdMaxDem = problemData->maxDemandaDiscNoCjtAlunosPorOfertaComEquiv( disciplina, oft->getId(), grupoAlunosId );
				else
				qtdMaxDem = problemData->haDemandaDiscNoCjtAlunosPorOferta( disciplina->getId(), oft->getId(), grupoAlunosId );
			#else
				qtdMaxDem = problemData->haDemandaDiscNoCjtAlunosPorOferta( disciplina->getId(), oft->getId(), discGrupoAlunosId );
			#endif

			mapDiscOftQtd[disciplina][oft] = qtdMaxDem;
		}
	}


	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;
	  
	    if ( cp->getId() != campusId )
	    {
		    continue;
	    }

		ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				Sala *sala = itCjtSala->salas.begin()->second;

				ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					Disciplina* disciplina = *it_disc;
					
					if ( P_ATUAL==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
					{
						if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
							continue;
					}

					int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );
					if ( discGrupoAlunosId > grupoAlunosId || 
						 discGrupoAlunosId == 0 )
					{
						continue;
					}

					// Listando todas as ofertas que contêm uma disciplina especificada.
					GGroup< Oferta *, LessPtr< Oferta > > ofertas;
					#ifdef EQUIVALENCIA_DESDE_O_INICIO
						if ( problemData->parametros->considerar_equivalencia_por_aluno )
							ofertas = problemData->discOfertasEquiv[ it_disc->getId() ];
						else
							ofertas = problemData->ofertasDisc[ it_disc->getId() ];
					#else	
						ofertas = problemData->ofertasDisc[ it_disc->getId() ];
					#endif

					ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
					{
						Oferta* oft = *itOferta;

						if ( oft->campus != cp )
							continue;
						
						int qtdMaxDem = mapDiscOftQtd[disciplina][oft];
						if ( qtdMaxDem == 0 )
						{
							continue;
						}

						for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
						{
							
							// -----------------
							// Solução Inicial
						    if ( problemData->existeSolTaticoInicial() )
							{
								bool fixarSala = false;
								Sala *salaSolInic = problemData->getSolTaticoInicial()->getSala( campusId, disciplina, turma, fixarSala );
								if ( salaSolInic != NULL && salaSolInic != sala && fixarSala )
				 					 continue;
							}
							// -----------------
							
							VariablePre v;
							v.reset();
							v.setType( VariablePre::V_PRE_ALUNOS );

							v.setTurma( turma );               // i
							v.setDisciplina( disciplina );     // d
							v.setOferta( *itOferta );          // oft
							v.setUnidade( *itUnidade );		   // u
							v.setSubCjtSala( *itCjtSala );	   // s
								
							if ( vHashPre.find(v) == vHashPre.end() )
							{
								vHashPre[v] = lp->getNumCols();
									
								double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									double valorCredito = itOferta->getReceita();
									int numCreditos = ( disciplina->getCredTeoricos() + disciplina->getCredPraticos() );
				
									coef = 100 * numCreditos * valorCredito;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = 0.0;
								}
						 
								int lowerBound = 0;
								int upperBound = qtdMaxDem;
						
								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
									( char * )v.toString().c_str() );

								lp->newCol( col );

								num_vars += 1;
							}
						}
				    }
				}
			}
		}
   }

	return num_vars;
}

// b_{i,d,c}
int PreModelo::cria_preVariavel_aloc_alunos( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
		
	ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
	{		
		Disciplina* disciplina = *itDisc;

		if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			 problemData->cp_discs[campusId].end() )
		{
			continue;
		}

		 int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );

 		 if ( discGrupoAlunosId > grupoAlunosId || 
			  discGrupoAlunosId == 0 )
		 {
			 continue;
		 }

		ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
		{
			Curso *curso = *itCurso;

			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			 if ( problemData->parametros->considerar_equivalencia_por_aluno ){
				if ( !problemData->haDemandaDiscNoCursoEquiv( disciplina, curso->getId() ) ) continue;
			 }
			 else if ( !problemData->haDemandaDiscNoCurso( disciplina->getId(), curso->getId() ) ) continue;
			#else
			 if ( !problemData->haDemandaDiscNoCurso( disciplina->getId(), curso->getId() ) )
			 {
				 continue;
			 }
			#endif

			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOC_ALUNO );

				v.setTurma( turma );               // i
				v.setDisciplina( disciplina );     // d
				v.setCurso( curso );			   // c
				
				if ( vHashPre.find(v) == vHashPre.end() )
				{
					double coef = 0.0;
					
					vHashPre[v] = lp->getNumCols();

					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = -1.0;
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{						
						coef = 50.0;
					}
					
					// Depois de maximizado o atendimento, min sum[] b deve ajudar a agrupar os alunos por curso,
					// o que deve diminuir o nro de conflitos futuros de horários.

					double lowerBound = 0.0;
					double upperBound = 1.0;
					
					if ( FIXAR_P1 && P_ATUAL > 1 )
					{
						int value = this->fixaLimitesVariavelPre( &v );
						if ( value > 0 ) // fixa!
						{
							lowerBound = value;
							upperBound = lowerBound;
						}
					}
					else if ( discGrupoAlunosId < grupoAlunosId )
					{
						lowerBound = this->fixaLimitesVariavelPre( &v );
						upperBound = lowerBound;
					}

					if (r>1)
					{
						int value = this->fixaLimitesVariavelPre( &v );
						if ( value > 0 ) // fixa!
						{
							lowerBound = value;
							upperBound = lowerBound;
						}
					}

					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
												
					lp->newCol( col );
					
					num_vars += 1;
				}
			}
		}
   }

	return num_vars;
}

// fd_{d,a}
int PreModelo::cria_preVariavel_folga_demanda_disciplina_aluno( int campusId, int grupoAlunosAtualId, int P_ATUAL )
{
    int num_vars = 0;

	Campus *cp = problemData->refCampus[campusId];

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();
	
	// Para cada conjunto de alunos cp com id menor ou igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;

		if ( grupoAlunosId > grupoAlunosAtualId )
		{
			break;
		}

		GGroup< Aluno *, LessPtr< Aluno > > cjtAlunos = problemData->cjtAlunos[ grupoAlunosId ];
	
		ITERA_GGROUP_LESSPTR( itAluno, cjtAlunos, Aluno )
		{
			Aluno *aluno = *itAluno;


			ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
			{
				if ( itAlDemanda->demanda->oferta->getCampusId() != campusId )
				{
					continue;
				}

				Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

				// Não crio fd para disciplina pratica. O atendimento ja será
				// garantido em sum[i] s_{i,dp,a} = sum[i] s_{i,dt,a}
				#ifdef EQUIVALENCIA_DESDE_O_INICIO
				if ( disciplina->getId() < 0 && 
					 problemData->parametros->considerar_equivalencia_por_aluno )
					 continue;
				#endif

				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_SLACK_DEMANDA );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );

				// Evita criação da variavel caso o aluno já possua o total de creditos pedidos em P1 atendidos
				// e a disciplina corrente não for uma das já atendidas.
				if (P_ATUAL>1)
				{
					int turmaAlocada = problemData->retornaTurmaDiscAluno( aluno, disciplina );
					double tempoTotalP1 = aluno->getCargaHorariaOrigRequeridaP1();
					double tempoJaAtendido = problemData->cargaHorariaJaAtendida( aluno );
					if ( tempoJaAtendido >= tempoTotalP1 &&
						 turmaAlocada == -1 )
						continue;
				}

				// Coeficiente na funcao objetivo
				double coef = 0.0;
				if ( itAlDemanda->getPrioridade() > 1 )
				{
					coef = 0.0; // para P2, quem controla o custo da folga de demanda são fpi e fps
				}
				else
				{
					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = 0.0;
					}
					else if( problemData->parametros->funcao_objetivo == 1 )
					{						
						//	coef = 100.0 * cp->getCusto() * disciplina->getTotalCreditos();
						
						coef = 100.0 * aluno->getReceita(disciplina) * disciplina->getTotalCreditos();
					}
				}

#ifdef UNIT
     //       coef = 10000.0;
#endif
				
				// Limites da variavel						 
				double lowerBound = 0.0;
				double ub = 1.0;

#ifdef TATICO_COM_HORARIOS
				if ( FIXAR_P1 )
				{
					int turmaAlocada = problemData->retornaTurmaDiscAluno( aluno, disciplina );
					if ( turmaAlocada != -1 ) // se o aluno esta alocado
					{
						ub = 0.0; // fixa atendimento
					}
					else if ( (*itAlDemanda)->getPrioridade() < P_ATUAL ||
							  grupoAlunosId < grupoAlunosAtualId ) // se o aluno não esta alocado e é de cjtAluno ou P anterior
					{
						lowerBound = 1.0; // fixa nao-atendimento
					}					
				}
				else
				{
					if ( grupoAlunosId < grupoAlunosAtualId ) // só fixa atendimento se for cjt anterior
					{
						int turmaAlocada = problemData->retornaTurmaDiscAluno( aluno, disciplina );
						if ( turmaAlocada != -1 ) ub = 0.0;	// se o aluno esta alocado												
						else lowerBound = 1.0;
					}
				}
#else
				if ( FIXAR_P1 )
				{
					int turmaAlocada = problemData->retornaTurmaDiscAluno( aluno, disciplina );
					if ( turmaAlocada != -1 ) // se o aluno esta alocado
					{
						ub = 0.0; // fixa atendimento
					}
					else if ( (*itAlDemanda)->getPrioridade() < P_ATUAL ||
							  grupoAlunosId < grupoAlunosAtualId ) // se o aluno não esta alocado e é de cjtAluno ou P anterior
					{
						lowerBound = 1.0; // fixa nao-atendimento
					}					
				}
				else
				{
					if ( grupoAlunosId < grupoAlunosAtualId ) // só fixa atendimento se for cjt anterior
					{
						int turmaAlocada = problemData->retornaTurmaDiscAluno( aluno, disciplina );
						if ( turmaAlocada != -1 ) ub = 0.0;	// se o aluno esta alocado												
						else lowerBound = 1.0;
					}
				}
#endif
				if ( vHashPre.find( v ) == vHashPre.end() )
				{
					vHashPre[v] = lp->getNumCols();

					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, ub, ( char * )v.toString().c_str() );
					lp->newCol(col);
				
					num_vars++;
				}
			}
		}
	
	}

	return num_vars;
}

// Não esta sendo mais usada, a restrição é forte
// bs_{i,d,c1,c2}
int PreModelo::cria_preVariavel_folga_compartilhamento_incomp( int campusId, int grupoAlunosId, int P_ATUAL )
{
	int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
		Campus* cp = *itCampus;
	  
	    if ( cp->getId() != campusId )
	    {
		    continue;
	    }

	   ITERA_GGROUP_LESSPTR( itCurso1, cp->cursos, Curso )
	   {
		   Curso* c1 = *itCurso1;
		   
		   ITERA_GGROUP_INIC_LESSPTR( itCurso2, itCurso1, cp->cursos, Curso )
		   {
				Curso* c2 = *itCurso2;
			    
			    if ( problemData->cursosCompativeis(c1, c2) || c1 == c2 )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;
					
					// A disciplina deve estar no conjunto de alunos atual ou anterior
					int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );
 					if ( discGrupoAlunosId > grupoAlunosId || 
						 discGrupoAlunosId == 0 )
					{
						continue;
					}

					// A disciplina deve ser ofertada no campus especificado
					if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
						 problemData->cp_discs[campusId].end() )
					{
						continue;
					}

					// A disciplina deve pertencer aos dois cursos c1 e c2
					if ( !c1->possuiDisciplina(disciplina) || !c2->possuiDisciplina(disciplina) )
						continue;

					// Calculando P_{d,curso}					
					if ( problemData->haDemandaDiscNoCjtAlunosPorCurso( disciplina->getId(), c1->getId(), discGrupoAlunosId ) == 0 ||
						 problemData->haDemandaDiscNoCjtAlunosPorCurso( disciplina->getId(), c2->getId(), discGrupoAlunosId ) == 0 )
					{
						continue;
					}

					for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					{
					   VariablePre v;
					   v.reset();
					   v.setType( VariablePre::V_PRE_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );
					   v.setTurma( turma );							// i
					   v.setDisciplina( disciplina );   			// d
					   v.setParCursos( std::make_pair(c1, c2) );	// c1 c2

					   if ( vHashPre.find( v ) == vHashPre.end() )
					   {
						  vHashPre[v] = lp->getNumCols();
						  
						  double coef = 0.0;

						  if ( problemData->parametros->funcao_objetivo == 0 )
						  {
							  coef = -cp->getCusto();
						  }
						  else if( problemData->parametros->funcao_objetivo == 1 )
						  {
							  coef = cp->getCusto();
						  }
						 
						 
						  double lowerBound = 0.0;
						  double upperBound = 1.0;
					
						  if ( FIXAR_P1 && P_ATUAL > 1 )
						  {
							  int value = this->fixaLimitesVariavelPre( &v );
						  	  if ( value > 0 ) // fixa!
							  {
								  lowerBound = value;
								  upperBound = lowerBound;
							  }
						  }
						  else if ( discGrupoAlunosId < grupoAlunosId )
						  {
							  lowerBound = this->fixaLimitesVariavelPre( &v );
							  upperBound = lowerBound;
						  }

						  OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

						  lp->newCol( col );
						  num_vars++;
					   }
					}
				}
		    }
		}
   }

	return num_vars;
}

// fc_{i,d,c1,c2}
int PreModelo::cria_preVariavel_folga_proibe_compartilhamento( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
		return num_vars;
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   Campus* cp = *itCampus;
	  
	   if ( cp->getId() != campusId )
	   {
		   continue;
	   }

	   ITERA_GGROUP_LESSPTR( itCurso1, cp->cursos, Curso )
	   {
		   Curso* c1 = *itCurso1;
		   
		   ITERA_GGROUP_INIC_LESSPTR( itCurso2, itCurso1, cp->cursos, Curso )
		   {
				Curso* c2 = *itCurso2;
			    
				// A variavel de folga só é criada para cursos compativeis e diferentes entre si
				// Ofertas para mesmo curso sempre poderão compartilhar
				// Ofertas de cursos distintos só poderão compartilhar se forem compativeis e o compartilhamento estiver permitido
			    if ( c1 == c2 || !problemData->cursosCompativeis(c1, c2) )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					// A disciplina deve estar no conjunto de alunos atual ou anterior
					int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );
 					if ( discGrupoAlunosId > grupoAlunosId || 
						 discGrupoAlunosId == 0 )
					{
						continue;
					}
					
					// A disciplina deve ser ofertada no campus especificado
					if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
						 problemData->cp_discs[campusId].end() )
					{
						continue;
					}

					// A disciplina deve pertencer aos dois cursos c1 e c2
					if ( !c1->possuiDisciplina(disciplina) || !c2->possuiDisciplina(disciplina) )
						continue;

					// Calculando P_{d,curso}					
					if ( problemData->haDemandaDiscNoCjtAlunosPorCurso( disciplina->getId(), c1->getId(), discGrupoAlunosId ) == 0 ||
						 problemData->haDemandaDiscNoCjtAlunosPorCurso( disciplina->getId(), c2->getId(), discGrupoAlunosId ) == 0 )
					{
						continue;
					}

					for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					{
					   VariablePre v;
					   v.reset();
					   v.setType( VariablePre::V_PRE_SLACK_COMPARTILHAMENTO );
					   v.setTurma( turma );							// i
					   v.setDisciplina( disciplina );   			// d
					   v.setParCursos( std::make_pair(c1, c2) );	// c1 c2

					   if ( vHashPre.find( v ) == vHashPre.end() )
					   {
						  vHashPre[v] = lp->getNumCols();
						  
						  double coef = 0.0;

						  if ( problemData->parametros->funcao_objetivo == 0 )
						  {
							  coef = -200*cp->getCusto();
						  }
						  else if( problemData->parametros->funcao_objetivo == 1 )
						  {
							  coef = 200*cp->getCusto();
						  }
						  						 
						  double lowerBound = 0.0;
						  double upperBound = 1.0;
					
#ifdef TATICO_COM_HORARIOS
						  if ( FIXAR_P1 && P_ATUAL > 1 )
						  {
							  int value = this->fixaLimitesVariavelPre( &v );
							  if ( value > 0 ) // fixa!
							  {
							  	  lowerBound = value;
								  upperBound = lowerBound;
							  }
						  }
						  else if ( discGrupoAlunosId < grupoAlunosId )
						  {
							  lowerBound = this->fixaLimitesVariavelPre( &v );
							  upperBound = lowerBound;
						  }

						  if (r>1)
						  {
								int value = this->fixaLimitesVariavelPre( &v );
								if ( value > 0 ) // fixa!
								{
						 			lowerBound = value;
									upperBound = lowerBound;
								}
						  }

#endif
						  OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

						  lp->newCol( col );
						  num_vars++;
					   }
					}
				}
		    }
		}
   }

	return num_vars;
}

// fs_{d,s}
int PreModelo::cria_preVariavel_folga_turma_mesma_disc_sala_dif( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	    
	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;

		if ( cp->getId() != campusId )
	    {
		    continue;
	    }
		
		ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					Disciplina* disciplina = *itDisc;

					if ( P_ATUAL==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
					{
						if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
							continue;
					}

					// A disciplina deve estar no conjunto de alunos atual ou anterior
					int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );
 					if ( discGrupoAlunosId > grupoAlunosId || 
						 discGrupoAlunosId == 0 )
					{
						continue;
					}
				
					// Verificando existencia de demanda					
					//if ( !problemData->haDemandaDiscNoCampus( disciplina->getId(), cp->getId() ) )
					//{
					//	continue;
					//}
						
					if ( itDisc->getNumTurmas() > 1 )
					{
						VariablePre v;
						v.reset();
						v.setType( VariablePre::V_PRE_SLACK_DISC_SALA);
						v.setDisciplina( *itDisc );
						v.setUnidade( *itUnidade );
						v.setSubCjtSala( *itCjtSala );

						if ( vHashPre.find( v ) == vHashPre.end() )
						{
							vHashPre[v] = lp->getNumCols();

							double coef = 0.0;

							if ( problemData->parametros->funcao_objetivo == 0 )
							{
								coef = -5*cp->getCusto();
							}
							else if( problemData->parametros->funcao_objetivo == 1 )
							{
								coef = 5*cp->getCusto();
							}

#ifdef UNIT
                     coef = 100.0;
#endif
						  
							double upperBound = itDisc->getNumTurmas()-1;						 
							double lowerBound = 0.0;
					
#ifdef TATICO_COM_HORARIOS
							if ( FIXAR_P1 && P_ATUAL > 1 )
							{
								lowerBound = this->fixaLimitesVariavelPre( &v );									
							}
							else if ( discGrupoAlunosId < grupoAlunosId )
							{
								lowerBound = this->fixaLimitesVariavelPre( &v );
								upperBound = lowerBound;
							}

							if (r>1)
							{
								int value = this->fixaLimitesVariavelPre( &v );
								if ( value > 0 ) // fixa!
								{
						 			lowerBound = value;
									upperBound = lowerBound;
								}
							}

#endif
							OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

							lp->newCol( col );
							num_vars++;
						}
					}
				}
			}
		}
	}
	return num_vars;
}

// Hs_{cp}
int PreModelo::cria_preVariavel_limite_sup_creds_sala( int campusId )
{
	int num_vars = 0;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus *cp = *itCampus;

	    if ( cp->getId() != campusId )
	    {
		   continue;
	    }

		VariablePre v;
		v.reset();
		v.setType( VariablePre::V_PRE_LIM_SUP_CREDS_SALA);
		v.setCampus( cp );

		if ( vHashPre.find( v ) == vHashPre.end() )
		{
			vHashPre[v] = lp->getNumCols();

			double upperbound = 0;

			ITERA_GGROUP_N_PT( itDisc, problemData->cp_discs[ cp->getId() ], int )
			{
				Disciplina* disciplina = problemData->refDisciplinas[ *itDisc ];
			
				int nCreds = disciplina->getCredTeoricos() + disciplina->getCredPraticos();

				upperbound += nCreds * disciplina->getNumTurmas();				
			}

			double coef = 0.0;

			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				coef = -cp->getCusto();
			}
			else if( problemData->parametros->funcao_objetivo == 1 )
			{
				coef = cp->getCusto();
			}
			
			double lowerBound;		
#ifdef TATICO_COM_HORARIOS
			lowerBound = this->fixaLimitesVariavelPre( &v );

#endif
			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperbound, ( char * )v.toString().c_str() );

			lp->newCol( col );
			num_vars++;
		}
	}

	return num_vars;
}

// xcs1_{s}, xcs2_{s}, xcs3_{s}
int PreModelo::cria_preVariavel_soma_cred_sala( int campusId )
{
	int num_vars = 0;
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
				double minTime = itCjtSala->minLimiteTempoPermitidoNaSemana();
				double maxTime = itCjtSala->maxTempoPermitidoNaSemana(problemData->mapDiscSubstituidaPor);
				double htps = 0;

				if ( maxTime >= 2*minTime ) // Se há diferença significativa entre tamanho das semanas letivas
					htps = ( maxTime + minTime ) / 3;
				else
					htps = ( maxTime + minTime ) / 2;


				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_CRED_SALA_F1 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s

				if ( vHashPre.find(v) == vHashPre.end() )
				{
				   vHashPre[v] = lp->getNumCols();

				   double coef = 0.0;

				   double lowerBound = 0.0;
                 
				   upperBound1 = (int)(htps * 0.5 + 0.5);	// 50% do tempo
               
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound1,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   num_vars ++;
				}

				v.reset();
				v.setType( VariablePre::V_PRE_CRED_SALA_F2 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s

				if ( vHashPre.find(v) == vHashPre.end() )
				{
				   vHashPre[v] = lp->getNumCols();

				   double coef = 50.0;

				   double lowerBound = 0.0;

				   upperBound2 = (int)(htps * 0.3 + 0.5);	// 30% do tempo
               
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound2,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   num_vars ++;
				}

				v.reset();
				v.setType( VariablePre::V_PRE_CRED_SALA_F3 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s

				if ( vHashPre.find(v) == vHashPre.end() )
				{
				   vHashPre[v] = lp->getNumCols();

				   double coef = 200.0;

				   double lowerBound = 0.0;

				   upperBound3 = (int)(htps * 0.1 + 0.5);	// 10% do tempo
               
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound3,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   num_vars ++;
				}

				v.reset();
				v.setType( VariablePre::V_PRE_CRED_SALA_F4 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s

				if ( vHashPre.find(v) == vHashPre.end() )
				{
				   vHashPre[v] = lp->getNumCols();

				   double coef = 500.0;

				   double lowerBound = 0.0;
               
				   double upperBound = htps - upperBound1 - upperBound2 - upperBound3;	// 10% do tempo

				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   num_vars ++;
				}
         }
      }
   }

	return num_vars;
}


// xcs1_{s,g}, xcs2_{s,g}, xcs3_{s,g}
int PreModelo::cria_preVariavel_soma_cred_sala_por_turno( int campusId )
{
	int num_vars = 0;
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
			std::map< int, GGroup<Calendario*,LessPtr<Calendario>> >::iterator
				itTurno = problemData->mapTurnoSemanasLetivas.begin();
			for ( ; itTurno != problemData->mapTurnoSemanasLetivas.end(); itTurno++ )
			{
				int turno = itTurno->first;
			
				double maxTime = itCjtSala->maxLimiteTempoPermitidoNaSemana( turno );
				if ( maxTime <= 0 )
					continue;

				double htps = maxTime;

				//double minTime = itCjtSala->minLimiteTempoPermitidoNaSemana();
				//double maxTime = itCjtSala->maxTempoPermitidoNaSemana(problemData->mapDiscSubstituidaPor);
				//double htps = 0;

				//if ( maxTime >= 2*minTime ) // Se há diferença significativa entre tamanho das semanas letivas
				//	htps = ( maxTime + minTime ) / 3;
				//else
				//	htps = ( maxTime + minTime ) / 2;

				//if ( htps <= 0 ) continue;


				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_CRED_SALA_F1 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s
				v.setTurno( turno );			   // g

				if ( vHashPre.find(v) == vHashPre.end() )
				{
				   vHashPre[v] = lp->getNumCols();

				   double coef = 0.0;

				   double lowerBound = 0.0;
                 
				   upperBound1 = (int)(htps * 0.5 + 0.5);	// 50% do tempo
               
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound1,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   num_vars ++;
				}

				v.reset();
				v.setType( VariablePre::V_PRE_CRED_SALA_F2 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s
				v.setTurno( turno );			   // g

				if ( vHashPre.find(v) == vHashPre.end() )
				{
				   vHashPre[v] = lp->getNumCols();

				   double coef = 5.0;

				   double lowerBound = 0.0;

				   upperBound2 = (int)(htps * 0.3 + 0.5);	// 30% do tempo
               
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound2,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   num_vars ++;
				}

				v.reset();
				v.setType( VariablePre::V_PRE_CRED_SALA_F3 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s
				v.setTurno( turno );			   // g

				if ( vHashPre.find(v) == vHashPre.end() )
				{
				   vHashPre[v] = lp->getNumCols();

				   double coef = 10.0;

				   double lowerBound = 0.0;

				   upperBound3 = (int)(htps * 0.1 + 0.5);	// 10% do tempo
               
				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound3,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   num_vars ++;
				}

				v.reset();
				v.setType( VariablePre::V_PRE_CRED_SALA_F4 );
				v.setCampus( *itCampus );          // c
				v.setUnidade( *itUnidade );		   // u
				v.setSubCjtSala( *itCjtSala );	   // s
				v.setTurno( turno );			   // g

				if ( vHashPre.find(v) == vHashPre.end() )
				{
				   vHashPre[v] = lp->getNumCols();

				   double coef = 30.0;

				   double lowerBound = 0.0;
               
				   double upperBound = htps - upperBound1 - upperBound2 - upperBound3;	// 10% do tempo

				   OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );

				   num_vars ++;
				}
			}
         }
      }
   }

	return num_vars;
}

// s_{i,d,a,cp}
int PreModelo::cria_preVariavel_aloca_aluno_turma_disc( int campusId, int grupoAlunosAtualId, int P_ATUAL, int r )
{
	int num_vars = 0;

	Campus *cp = problemData->refCampus[campusId];

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();
	
	// Para cada conjunto de alunos com id menor ou igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;

		if ( grupoAlunosId > grupoAlunosAtualId )
		{
			break;
		}

		// Para cada aluno do conjunto
		GGroup< Aluno *, LessPtr< Aluno > > cjtAlunos = problemData->cjtAlunos[ grupoAlunosId ];	
		ITERA_GGROUP_LESSPTR( itAluno, cjtAlunos, Aluno )
		{
			Aluno *aluno = *itAluno;


			ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
			{
				Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

				int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );
				
				if ( itAlDemanda->getCampus()->getId() != campusId )
				{
					continue;
				}

				for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
				{
					VariablePre v;
					v.reset();
					v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setCampus( cp );
					

					// -----------------
					// Solução Inicial
					bool fixarAlocacao = false;
					if ( problemData->existeSolTaticoInicial() )
					{						 
						bool criar = problemData->getSolTaticoInicial()->criar(v, fixarAlocacao);
						if ( !criar ) continue;
					}
					// -----------------


					if ( vHashPre.find( v ) == vHashPre.end() )
					{						
						// Se o aluno já estiver alocado, evita a criação da variavel para outras turmas
						if ( turmaAluno != -1 && turmaAluno != turma )
						{
							continue;
						}

						// passou: aluno não alocado em nenhuma turma ou alocado nesse turma corrente

						// Se não for permitido ao modelo inserir aluno de prioridade 2 em turma de p1,
						// e o aluno não tiver sido alocado pela heuristica, não cria a variavel
						if ( FIXAR_P1 && P_ATUAL > 1 && itAlDemanda->getPrioridade()==P_ATUAL )
						{							
							if ( turmaAluno == -1 ) // se não foi alocado pela heuristica
							{							
								int nAlunosTurmaDeP1 = problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId );
								if ( nAlunosTurmaDeP1 > 0 && !PERMITIR_INSERCAO_ALUNODEMANDAP2_EM_TURMAP1 )
								{
									continue;
								}
							}
						}					

						// Evita criação da variavel caso o aluno já possua o total de creditos pedidos em P1 atendidos
						// e a disciplina corrente não for uma das já atendidas.
						if ( P_ATUAL>1 )
						{
							double tempoTotalP1 = aluno->getCargaHorariaOrigRequeridaP1();
							double tempoJaAtendido = problemData->cargaHorariaJaAtendida( aluno );
							if ( tempoJaAtendido >= tempoTotalP1 &&
								 turmaAluno != turma )
								continue;
						}

						vHashPre[v] = lp->getNumCols();

						double coef = 0.0;												 
						double lowerBound = 0.0;
						double upperBound = 1.0;
					
						if ( FIXAR_P1 && P_ATUAL > 1 )
						{
							lowerBound = this->fixaLimitesVariavelPre( &v );
							if ( grupoAlunosId < grupoAlunosAtualId )
							{
								upperBound = lowerBound;
							}
						}
						else if ( grupoAlunosId < grupoAlunosAtualId )
						{
							lowerBound = this->fixaLimitesVariavelPre( &v );
							upperBound = lowerBound;
						}

						if (r>1)
						{
							if ( turmaAluno != -1 ) // alocado, fixa
							{
								lowerBound = 1.0 - 1e-5;
								upperBound = 1.0 + 1e-5;
							}
							else // não alocado, deixa livre para novas turmas e fixa igual a 0 para turmas existentes
							{
								int nAlunosTurmaDeP1 = problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId );
								if ( nAlunosTurmaDeP1 > 0 )
								{	// turma existente
									lowerBound = 0.0 - 1e-5;
									upperBound = 0.0 + 1e-5;
								}							
							}
						}

						// -------------------------						
						// Solução inicial
						if ( fixarAlocacao )
							lowerBound = upperBound;	 
						// -------------------------

						OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

						lp->newCol( col );
						num_vars++;
					}
				}
			}
		}
	}

	return num_vars;
}

// s_{i,d,a,cp}
int PreModelo::cria_preVariavel_aloca_aluno_turma_disc_equiv( int campusId, int P )
{
	int numVars = 0;
	
	Campus *cp = problemData->refCampus[campusId];
	
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

			if ( itAlDemanda->getCampus()->getId() != campusId )
			{
				continue;
			}

			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );
			
			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );
		
			if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
			{
				int chRequerida = aluno->getCargaHorariaOrigRequeridaP1();
				int chAtendida = problemData->cargaHorariaJaAtendida( aluno );
				if ( chRequerida - chAtendida <= 0 )
						continue;

				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{
					if ( problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, *itDisc ) )
					{
						disciplinasPorAlDem.add( *itDisc );
					}
				}
			}

			ITERA_GGROUP_LESSPTR( itDiscEq, disciplinasPorAlDem, Disciplina )
			{
				Disciplina *disciplinaEquiv = (*itDiscEq);

				for ( int turma=1; turma <= disciplinaEquiv->getNumTurmas(); turma++ )
				{
					// Se o aluno já estiver alocado, evita a criação da variavel para outras turmas
					if ( turmaAluno != -1 && turmaAluno != turma )
					{
						continue;
					}
										

					VariablePre v;
					v.reset();
					v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( disciplinaEquiv );
					v.setTurma( turma );
					v.setCampus( cp );
										
					// -----------------
					// Solução Inicial
					bool fixarAlocacao = false;
					if ( problemData->existeSolTaticoInicial() )
					{						 
						bool criar = problemData->getSolTaticoInicial()->criar(v, fixarAlocacao);
						if ( !criar ) continue;
					}
					// -----------------


					if ( vHashPre.find( v ) == vHashPre.end() )
					{
						vHashPre[ v ] = lp->getNumCols();
									
						double lowerBound = 0.0;
						double upperBound = 1.0;
						double coef = 0.0;		
					
						if ( fixarAlocacao )
							lowerBound = upperBound;

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


// s_{i,d,a,g,cp}
int PreModelo::cria_preVariavel_aloca_aluno_turma_disc_turno( int campusId, int P )
{
	int numVars = 0;
	
	Campus *cp = problemData->refCampus[campusId];
	
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;
			
			if ( itAlDemanda->getCampus()->getId() != campusId )
			{
				continue;
			}
				
			std::map<Disciplina*, GGroup<int>> mapDiscTurnos;
			mapDiscTurnos[disciplina] = problemData->getTurnosComunsViaveis( disciplina, (*itAlDemanda)->demanda->getTurnoIES() );
			
			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );			

			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );
		
			if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
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
					for ( int turma=1; turma <= disciplinaEquiv->getNumTurmas(); turma++ )
					{
						// Se o aluno já estiver alocado, evita a criação da variavel para outras turmas
						if ( turmaAluno != -1 && turmaAluno != turma )
						{
							continue;
						}										

						VariablePre v;
						v.reset();
						v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
						v.setAluno( aluno );
						v.setDisciplina( disciplinaEquiv );
						v.setTurma( turma );
						v.setTurno( *itTurno );
						v.setCampus( cp );
										
						// -----------------
						// Solução Inicial
						bool fixarAlocacao = false;
						if ( problemData->existeSolTaticoInicial() )
						{						 
							bool criar = problemData->getSolTaticoInicial()->criar(v, fixarAlocacao);
							if ( !criar ) continue;
						}
						// -----------------


						if ( vHashPre.find( v ) == vHashPre.end() )
						{
							vHashPre[ v ] = lp->getNumCols();
									
							double lowerBound = 0.0;
							double upperBound = 1.0;
							double coef = 0.0;		
					
							if ( fixarAlocacao )
								lowerBound = upperBound;

							OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

							lp->newCol( col );
							numVars++;
						}
					}
				}
			}
		}
	}

	return numVars;
}

// s_{i,d,a,g,cp}
int PreModelo::cria_preVariavel_aloca_aluno_turma_disc_turno_equiv( int campusId, int P )
{
	int numVars = 0;
	
	Campus *cp = problemData->refCampus[campusId];
	
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;
			
			if ( itAlDemanda->getCampus()->getId() != campusId )
			{
				continue;
			}
				
			std::map<Disciplina*, GGroup<int>> mapDiscTurnos;
			mapDiscTurnos[disciplina] = problemData->getTurnosComunsViaveis( disciplina, (*itAlDemanda)->demanda->getTurnoIES() );
			
			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );			

			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );
		
			if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
			{
				// Se já houve substituição: não permite transitividade de equivalências
				if ( (*itAlDemanda)->demandaOriginal == NULL )
				{
					int chRequerida = aluno->getCargaHorariaOrigRequeridaP1();
					int chAtendida = problemData->cargaHorariaJaAtendida( aluno );
					if ( chRequerida - chAtendida <= 0 )
							continue;

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
			}
			
			ITERA_GGROUP_LESSPTR( itDiscEq, disciplinasPorAlDem, Disciplina )
			{
				Disciplina *disciplinaEquiv = (*itDiscEq);

				GGroup<int> turnos = mapDiscTurnos[disciplinaEquiv];
				ITERA_GGROUP_N_PT( itTurno, turnos, int )
				{
					for ( int turma=1; turma <= disciplinaEquiv->getNumTurmas(); turma++ )
					{
						// Se o aluno já estiver alocado, evita a criação da variavel para outras turmas
						if ( turmaAluno != -1 && turmaAluno != turma )
						{
							continue;
						}										

						VariablePre v;
						v.reset();
						v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
						v.setAluno( aluno );
						v.setDisciplina( disciplinaEquiv );
						v.setTurma( turma );
						v.setTurno( *itTurno );
						v.setCampus( cp );
										
						// -----------------
						// Solução Inicial
						bool fixarAlocacao = false;
						if ( problemData->existeSolTaticoInicial() )
						{						 
							bool criar = problemData->getSolTaticoInicial()->criar(v, fixarAlocacao);
							if ( !criar ) continue;
						}
						// -----------------


						if ( vHashPre.find( v ) == vHashPre.end() )
						{
							vHashPre[ v ] = lp->getNumCols();
									
							double lowerBound = 0.0;
							double upperBound = 1.0;
							double coef = 0.0;		
					
							if ( fixarAlocacao )
								lowerBound = upperBound;

							OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

							lp->newCol( col );
							numVars++;
						}
					}
				}
			}
		}
	}

	return numVars;
}



// fda_{}
int PreModelo::cria_preVariavel_folga_distribuicao_aluno( int campusId, int grupoAlunosAtualId, int P_ATUAL )
{
	int num_vars = 0;

	Campus *cp = problemData->refCampus[campusId];

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();

	// Para cada conjunto de alunos cp com id menor ou igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;

		if ( grupoAlunosId > grupoAlunosAtualId )
		{
			break;
		}

		// Para cada aluno do conjunto
		GGroup< Aluno *, LessPtr< Aluno > > cjtAlunos = problemData->cjtAlunos[ grupoAlunosId ];	
		ITERA_GGROUP_LESSPTR( itAluno, cjtAlunos, Aluno )
		{
			Aluno *aluno = *itAluno;

			ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
			{
				Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

				if ( itAlDemanda->getCampus()->getId() != campusId )
				{
					continue;
				}

				GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >::iterator itAlDemanda2 = itAlDemanda;
				itAlDemanda2++;

				for(; itAlDemanda2 != aluno->demandas.end(); itAlDemanda2++)
				{
					Disciplina *disciplina2 = (*itAlDemanda2)->demanda->disciplina;

					for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					{
						for ( int turma2 = 1; turma2 <= disciplina2->getNumTurmas(); turma2++ )
						{
							VariablePre v;
							v.reset();
							v.setType( VariablePre::V_PRE_FOLGA_DISTR_ALUNOS );
							v.setDisciplina( disciplina );
							v.setTurma( turma );
							v.setDisciplina2( disciplina2 );
							v.setTurma2( turma2 );
							//v.setCampus( cp );

							if ( vHashPre.find( v ) == vHashPre.end() )
							{
								vHashPre[v] = lp->getNumCols();

								double coef = 100.0;

								double lowerBound = 0.0;
								double upperBound = 1.0;
														

								OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

								lp->newCol( col );
								num_vars++;
							}
						}
					}
				}
			}
		}
	}

	return num_vars;
}


// fpi_{a}
// Só para P2 em diante
int PreModelo::cria_preVariavel_folga_prioridade_inf( int campusId, int prior, int grupoAlunosAtualId )
{
	int num_vars = 0;

	if ( prior < 2 )
	{
	   return num_vars;
	}

	Campus *cp = problemData->refCampus[campusId];

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();
	
	// Para cada conjunto de alunos cp com id menor ou igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;

		if ( grupoAlunosId > grupoAlunosAtualId )
		{
			break;
		}

		// Para cada aluno do conjunto
		GGroup< Aluno *, LessPtr< Aluno > > cjtAlunos = problemData->cjtAlunos[ grupoAlunosId ];	
		ITERA_GGROUP_LESSPTR( itAluno, cjtAlunos, Aluno )
		{
			Aluno *aluno = *itAluno;

			if ( !aluno->hasCampusId(campusId) )
			{
				continue;
			}

			VariablePre v;
			v.reset();
			v.setType( VariablePre::V_PRE_SLACK_PRIOR_INF );
			v.setAluno( aluno );
			v.setCampus( cp );

			if ( vHashPre.find( v ) == vHashPre.end() )
			{
				vHashPre[v] = lp->getNumCols();					

				double upperBound = 0.0;						 
				double lowerBound = 0.0;

				if ( grupoAlunosId < grupoAlunosAtualId )
				{
#ifdef TATICO_COM_HORARIOS
					lowerBound = this->fixaLimitesVariavelPre( &v );

#endif
				}
								
				double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( 1, aluno->getAlunoId() );	
				if ( cargaHorariaNaoAtendida == 0 ) // se não houver folga de demanda de P1
				{
					continue;
				}

				int tempoMaxCred=0;
				int totalCreditosP2 = 0;
				ITERA_GGROUP_LESSPTR( itAlunoDem, aluno->demandas, AlunoDemanda )
				{
					if ( itAlunoDem->getPrioridade() != prior )
						continue;

					int nCreds = itAlunoDem->demanda->disciplina->getTotalCreditos();
					int tempo = itAlunoDem->demanda->disciplina->getTempoCredSemanaLetiva();
					totalCreditosP2 += nCreds;

					if ( tempoMaxCred < tempo )
						tempoMaxCred=tempo;
				}							
				if ( totalCreditosP2 == 0 ) // se não houver demanda de P2 para o aluno
					continue;
				
				// Limita o tanto de carga horária do aluno que pode ser excedido em P2 em 2 créditos.
				upperBound = tempoMaxCred*2;

				double coef = 0.0;
				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					double custo = cp->getCusto();
							 
					coef = -50 * custo * totalCreditosP2;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					double custo = cp->getCusto();

					coef = 5 * custo * totalCreditosP2;
				}	

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
		}
	}

	return num_vars;
}

// fps_{a}
// Só para P2 em diante
int PreModelo::cria_preVariavel_folga_prioridade_sup( int campusId, int prior, int grupoAlunosAtualId )
{
	int num_vars = 0;
	
	if ( prior < 2 )
	{
	   return num_vars;
	}

	Campus *cp = problemData->refCampus[campusId];

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();
	
	// Para cada conjunto de alunos cp com id menor ou igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;

		if ( grupoAlunosId > grupoAlunosAtualId )
		{
			break;
		}

		// Para cada aluno do conjunto
		GGroup< Aluno *, LessPtr< Aluno > > cjtAlunos = problemData->cjtAlunos[ grupoAlunosId ];	
		ITERA_GGROUP_LESSPTR( itAluno, cjtAlunos, Aluno )
		{
			Aluno *aluno = *itAluno;

			if ( !aluno->hasCampusId(campusId) )
			{
				continue;
			}

			VariablePre v;
			v.reset();
			v.setType( VariablePre::V_PRE_SLACK_PRIOR_SUP );
			v.setAluno( aluno );
			v.setCampus( cp );

			if ( vHashPre.find( v ) == vHashPre.end() )
			{
				vHashPre[v] = lp->getNumCols();
				
				double upperBound = 0.0;						 
				double lowerBound = 0.0;

				if ( grupoAlunosId < grupoAlunosAtualId )
				{
					lowerBound = this->fixaLimitesVariavelPre( &v );
				}

				int totalCreditos = aluno->getNroCreditosNaoAtendidos();
				double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( 1, aluno->getAlunoId() );	
				if ( cargaHorariaNaoAtendida == 0 ) // se não houver folga de demanda de P1
				{
					continue;
				}

				upperBound = cargaHorariaNaoAtendida;

				double coef = 0.0;
				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					double custo = cp->getCusto();
							 
					coef = -60 * custo * totalCreditos;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					double custo = cp->getCusto();

					coef = 6 * custo * totalCreditos;
				}	

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
		}
	}

	return num_vars;
}


// ft_{i,d,cp}
// Só para P2 em diante
int PreModelo::cria_preVariavel_folga_abre_turma_sequencial( int campusId, int cjtAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	
	if ( P_ATUAL < 2 && r==1 )
	{
	   return num_vars;
	}

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_disciplina, it_CpDisc->second, int )
      {
		 Disciplina *disciplina = problemData->refDisciplinas[ *it_disciplina ];
		 	 
		 int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );

 		 if ( discGrupoAlunosId > cjtAlunosId || 
			  discGrupoAlunosId == 0 )
		 {
			 continue;
		 }
		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            VariablePre v;
            v.reset();
            v.setType( VariablePre::V_PRE_FOLGA_ABRE_TURMA_SEQUENCIAL );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );				// cp

            if ( vHashPre.find(v) == vHashPre.end() )
            {
                lp->getNumCols();
                vHashPre[v] = lp->getNumCols();

			    double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{							 
					coef = -10 * (turma+1);
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{					
					coef = 10 * (turma+1);
				}						                
						 
				double lowerBound = 0.0;
				double upperBound = 1.0;

				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				num_vars++;
            }
         }
      }
   }

	return num_vars;

}


// w_{i,d,i',d'}
/* // Usada só para p2
int PreModelo::cria_preVariavel_turmas_compartilhadas( int campusId, int cjtAlunosId, int P_ATUAL )
{
	int num_vars = 0;
	
	if ( P_ATUAL < 2 )
	{
	   return num_vars;
	}

   std::map< int, GGroup< int > >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( itDisc1, it_CpDisc->second, int )
      {
		 Disciplina *disciplina1 = problemData->refDisciplinas[ *itDisc1 ];
		 	 
		 int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina1->getId() );

 		 if ( discGrupoAlunosId > cjtAlunosId || 
			  discGrupoAlunosId == 0 )
		 {
			 continue;
		 }
		 
		 bool haDemandaDisc1P2 = problemData->haDemandaP2DiscNoCampus( campusId, P_ATUAL, disciplina1 );

         for ( int turma1 = 0; turma1 < disciplina1->getNumTurmas(); turma1++ )
         {
			std::pair<ConjuntoSala*, GGroup<int> > atends1 = retornaSalaEDiasDeAtendimentoTaticoAnterior( turma1, disciplina1, cp );
						
			ConjuntoSala* cjtSala1 = atends1.first;
			GGroup<int> dias1 = atends1.second;

			ITERA_GGROUP_INIC_N_PT( itDisc2, itDisc1, it_CpDisc->second, int )
			{
				Disciplina *disciplina2 = problemData->refDisciplinas[ *itDisc2 ];
		 
				if ( disciplina2 == disciplina1 )
					continue;

				bool haDemandaDisc2P2 = problemData->haDemandaP2DiscNoCampus( campusId, P_ATUAL, disciplina2 );

				// Só cria a variavel se pelo menos 1 das disciplinas tiver demanda de prioridade 2
				if ( !haDemandaDisc1P2 && !haDemandaDisc2P2 )
				{
					continue;
				}

				discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina2->getId() );

 				if ( discGrupoAlunosId > cjtAlunosId || 
					discGrupoAlunosId == 0 )
				{
					continue;
				}
		 
				for ( int turma2 = 0; turma2 < disciplina2->getNumTurmas(); turma2++ )
				{
					std::pair<ConjuntoSala*, GGroup<int> > atends2 = retornaSalaEDiasDeAtendimentoTaticoAnterior( turma2, disciplina2, cp );
						
					ConjuntoSala* cjtSala2 = atends2.first;
					GGroup<int> dias2 = atends2.second;

					// Se os dois atendimentos já existirem: 
					// Esta variavel por enquanto só é usada para atendimentos em salas diferentes e no mesmo dia
					if ( cjtSala1 != NULL && cjtSala2 != NULL )
					{
						bool diaEmComum = false;
						ITERA_GGROUP_N_PT( itDia1, dias1, int )
						{
							if ( dias2.find( *itDia1 ) != dias2.end() )
							{
								diaEmComum = true;
							}
						}

						if ( cjtSala1 == cjtSala2 || !diaEmComum )
						{
							continue;
						}
					}

					// Esta variavel por enquanto só sera usada se pelo menos uma das turmas já existir de p1
					if ( cjtSala1 == NULL && cjtSala2 == NULL )
					{					
						continue;
					}										

					VariablePre v;
					v.reset();
					v.setType( VariablePre::V_PRE_TURMAS_COMPART );
					v.setTurma1( turma1 );            // i1
					v.setDisciplina1( disciplina1 );  // d1
					v.setTurma2( turma2 );            // i2
					v.setDisciplina2( disciplina2 );  // d2					


					if ( vHashPre.find(v) == vHashPre.end() )
					{			
						lp->getNumCols();
						vHashPre[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{							 
							coef = -1.0;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{					
							coef = 1.0;
						}						                
						 
						double lowerBound = 0.0;
						double upperBound = 1.0;

						// Se as turmas já estão alocadas com alunos em comum, já fixa a variavel
						if ( problemData->possuiAlunosEmComum( turma1, disciplina1,cp, turma2, disciplina2, cp ) )						
						{
							lowerBound = 1.0 - 1e-8;
							upperBound = 1.0 + 1e-8;						
						}

						OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

						lp->newCol( col ); 
                
						num_vars++;
					}
				}
			}
         }
      }
   }

	return num_vars;
}
*/

// w_{i,d,i',d'}
int PreModelo::cria_preVariavel_turmas_compartilhadas( int campusId, int cjtAlunosId, int P_ATUAL )
{
	int num_vars = 0;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( itDisc1, it_CpDisc->second, int )
      {
		 Disciplina *disciplina1 = problemData->refDisciplinas[ *itDisc1 ];
		 	 
		 int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina1->getId() );

 		 if ( discGrupoAlunosId > cjtAlunosId || 
			  discGrupoAlunosId == 0 )
		 {
			 continue;
		 }

		 ITERA_GGROUP_INIC_N_PT( itDisc2, itDisc1, it_CpDisc->second, int )
		 {
			Disciplina *disciplina2 = problemData->refDisciplinas[ *itDisc2 ];
		 
			if ( disciplina2->getId() == disciplina1->getId() )
				continue;

			discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina2->getId() );

 			if ( discGrupoAlunosId > cjtAlunosId || 
				discGrupoAlunosId == 0 )
			{
				continue;
			}
		 	
			bool demandasAlunoEmComum = problemData->possuiDemandasAlunoEmComum( disciplina1, disciplina2, campusId, P_ATUAL );
			
			if ( !demandasAlunoEmComum )
			{
				continue;
			}

			for ( int turma1 = 1; turma1 <= disciplina1->getNumTurmas(); turma1++ )
			{		 
				for ( int turma2 = 1; turma2 <= disciplina2->getNumTurmas(); turma2++ )
				{
					VariablePre v;
					v.reset();
					v.setType( VariablePre::V_PRE_TURMAS_COMPART );
					v.setTurma1( turma1 );            // i1
					v.setDisciplina1( disciplina1 );  // d1
					v.setTurma2( turma2 );            // i2
					v.setDisciplina2( disciplina2 );  // d2					
					
					if ( vHashPre.find(v) == vHashPre.end() )
					{			
						lp->getNumCols();
						vHashPre[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{							 
							coef = -400.0;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{					
							coef = 400.0;
						}						                
						 
						double lowerBound = 0.0;
						double upperBound = 1.0;

						// Se as turmas já estão alocadas com alunos em comum, já fixa a variavel
						if ( problemData->possuiAlunosEmComum( turma1, disciplina1, cp, turma2, disciplina2, cp ) )						
						{
							lowerBound = 1.0 - 1e-8;
							upperBound = 1.0 + 1e-8;						
						}

						OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

						lp->newCol( col ); 
                
						num_vars++;												
					}
				}
			}
         }
      }
   }

	return num_vars;	
}

// as_{a,s}
int PreModelo::cria_preVariavel_aluno_sala( int campusId, int grupoAlunosAtualId, int P_ATUAL, int r )
{
	int num_vars = 0;
	
	Campus* cp = problemData->refCampus[campusId];

	// Filtro: para cada aluno, lista as salas associadas a disciplinas que ele pede

	std::map< int/*alunoId*/, GGroup<ConjuntoSala*, LessPtr<ConjuntoSala>> > mapAlunoCjtSalas;
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDem, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDem->demanda->disciplina;
					
			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );
			
			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			 int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );		
			 if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
			 {
				int chRequerida = aluno->getCargaHorariaOrigRequeridaP1();
				int chAtendida = problemData->cargaHorariaJaAtendida( aluno );
				if ( chRequerida - chAtendida <= 0 )
						continue;

				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{
					if ( problemData->alocacaoEquivViavel( (*itAlDem)->demanda, *itDisc ) )
						disciplinasPorAlDem.add( *itDisc );
				}
			 }
			#endif

			ITERA_GGROUP_LESSPTR( itDiscEq, disciplinasPorAlDem, Disciplina )
			{
				Disciplina *disciplinaEquiv = (*itDiscEq);
				std::map< int, ConjuntoSala* >::iterator itMapCjtSala = disciplinaEquiv->cjtSalasAssociados.begin();
				for( ; itMapCjtSala != disciplinaEquiv->cjtSalasAssociados.end(); itMapCjtSala++ )
				{
					if ( P_ATUAL==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
					{
						if ( disciplinaEquiv->getNumTurmasNaSala( itMapCjtSala->second->getId() ) == 0 )
						{	
							// Se não tiver criando opções de sala fora da solução do estima turmas,
							// não incluir essa sala
							continue;
						}
					}

					if ( itMapCjtSala->second->salas.begin()->second->getIdCampus() == campusId )
						mapAlunoCjtSalas[aluno->getAlunoId()].add( itMapCjtSala->second );		
				}
			}
		}
	}
			
	std::map< int/*alunoId*/, GGroup<ConjuntoSala*, LessPtr<ConjuntoSala>> >::iterator itMapAluno = mapAlunoCjtSalas.begin();
	for ( ; itMapAluno != mapAlunoCjtSalas.end(); itMapAluno++ )
	{
		Aluno *aluno = problemData->retornaAluno( itMapAluno->first );

		ITERA_GGROUP_LESSPTR( itCjtSala, itMapAluno->second, ConjuntoSala )
		{
			if ( !aluno->hasCampusId(campusId) )
			{
				continue;
			}

			Unidade *unidade = problemData->refUnidade[ itCjtSala->salas.begin()->second->getIdUnidade() ];

			VariablePre v;
			v.reset();
			v.setType( VariablePre::V_PRE_ALUNO_SALA);
			v.setAluno( aluno );
			v.setUnidade( unidade );
			v.setSubCjtSala( *itCjtSala );

			if ( vHashPre.find( v ) == vHashPre.end() )
			{
				vHashPre[v] = lp->getNumCols();

				double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -cp->getCusto()*2;
				}
				else if( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = cp->getCusto()*2;
				}
						  
				double upperBound = 1.0;						 
				double lowerBound = 0.0;
					
				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
		}
	}
	
	return num_vars;
}

// f_{i,d,cp} -> indica se há alunos formandos na turma
int PreModelo::cria_preVariavel_formandosNaTurma( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->violar_min_alunos_turmas_formandos )
		return num_vars;

	if ( P_ATUAL==1 && r==1 ) // só considera formandos a partir da segunda rodada
		return num_vars;

	std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_disciplina, it_CpDisc->second, int )
      {
		 Disciplina *disciplina = problemData->refDisciplinas[ *it_disciplina ];
		 		 
		 int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );
		 
 		 if ( discGrupoAlunosId > grupoAlunosId || 
			  discGrupoAlunosId == 0 )
		 {
			 continue;
		 }
		 
		 if ( ! problemData->haDemandaPorFormandos( disciplina, cp, P_ATUAL ) )
			 continue;

         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
			 if ( !problemData->possuiAlunoFormando( turma, disciplina, cp ) &&
				  !problemData->haAlunoFormandoNaoAlocado( disciplina, campusId, P_ATUAL ) )
			 {
				 continue;
			 }

            VariablePre v;
            v.reset();
            v.setType( VariablePre::V_PRE_FORMANDOS_NA_TURMA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );				// cp

            if ( vHashPre.find(v) == vHashPre.end() )
            {
                lp->getNumCols();
                vHashPre[v] = lp->getNumCols();

			    double coef = 0.0;					                
						 
				double lowerBound = 0.0;
				double upperBound = 1.0;

				int value = this->fixaLimitesVariavelPre( &v );
				if ( value > 0 ) // fixa!
				{
					lowerBound = value;
					upperBound = lowerBound;
				}

				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				num_vars++;
            }
         }
      }
   }

	return num_vars;
}

// v_{i,d,sl,s}
int PreModelo::cria_preVariavel_turmaCalendario( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

     ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               Disciplina *disciplina = *it_disc;
				
			   if ( P_ATUAL==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
			   {
					if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
						continue;
			   }
		 
               int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );		 
               if ( discGrupoAlunosId > grupoAlunosId || 
                  discGrupoAlunosId == 0 )
               {
                  continue;
               }
		 
               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
                  GGroup<Calendario*, LessPtr<Calendario>> discCalends = disciplina->getCalendarios();
                  ITERA_GGROUP_LESSPTR( itCalendario, discCalends, Calendario )
                  {
                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_TURMA_CALENDARIO );
                     v.setTurma( turma );				// i
                     v.setDisciplina( disciplina );		// d
                     v.setCalendario( *itCalendario );   // sl
                     v.setSubCjtSala(*itCjtSala);		//s

                     if ( vHashPre.find(v) == vHashPre.end() )
                     {
                        lp->getNumCols();
                        vHashPre[v] = lp->getNumCols();

                        double coef = 0.0;										              						 
                        double lowerBound = 0.0;
                        double upperBound = 1.0;

                        OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                           ( char * )v.toString().c_str() );

                        lp->newCol( col );                
                        num_vars++;
                     }
                  }
               }
            }
         }
      }
   }

	return num_vars;
}

// substituiu a de cima, que usa calendario
// v_{i,d,tt,s}
int PreModelo::cria_preVariavel_turmaTurno( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

     ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               Disciplina *disciplina = *it_disc;
				
			   if ( P_ATUAL==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
			   {
					if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
						continue;
			   }
		 
               int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );		 
               if ( discGrupoAlunosId > grupoAlunosId || 
                  discGrupoAlunosId == 0 )
               {
                  continue;
               }
		 
               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
                  ITERA_GGROUP_LESSPTR( itTurnoIES, problemData->turnosIES, TurnoIES )
                  {
					  if ( ! disciplina->possuiTurnoIES( *itTurnoIES ) ) continue;

                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_TURMA_TURNOIES );
                     v.setTurma( turma );				// i
                     v.setDisciplina( disciplina );		// d
                     v.setTurnoIES( *itTurnoIES );		// tt
                     v.setSubCjtSala(*itCjtSala);		// s

                     if ( vHashPre.find(v) == vHashPre.end() )
                     {
                        lp->getNumCols();
                        vHashPre[v] = lp->getNumCols();

                        double coef = 0.0;										              						 
                        double lowerBound = 0.0;
                        double upperBound = 1.0;

                        OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                           ( char * )v.toString().c_str() );

                        lp->newCol( col );                
                        num_vars++;
                     }
                  }
               }
            }
         }
      }
   }

	return num_vars;
}

// sab_{i,d,s}
int PreModelo::cria_preVariavel_turma_so_sabado( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	   
   ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
   {
	  Campus *cp = *itCp;

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

     ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               Disciplina* disciplina = ( *it_disc );
			            
               if ( P_ATUAL==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
               {
                  if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
                     continue;
               }
		 
               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {			
				  if ( disciplina->diasLetivos.find( 7 ) == disciplina->diasLetivos.end() ||
					  itCjtSala->salas.begin()->second->diasLetivos.find( 7 ) == 
					  itCjtSala->salas.begin()->second->diasLetivos.end() )
					  continue;

                  VariablePre v;
                  v.reset();
                  v.setType( VariablePre::V_PRE_TURMA_SAB );
                  v.setTurma( turma );				// i
                  v.setDisciplina( disciplina );	// d
                  v.setSubCjtSala( *itCjtSala );    // s

                  if ( vHashPre.find(v) == vHashPre.end() )
                  {
                     lp->getNumCols();
                     vHashPre[v] = lp->getNumCols();

                     double coef = 0.0;										              						 
                     double lowerBound = 0.0;
                     double upperBound = 1.0;
					 
                     OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                        ( char * )v.toString().c_str() );

                     lp->newCol( col );                
                     num_vars++;
                  }
               }
            }
         }
      }
   }

	return num_vars;
}

// r_{a,k} // Não estou usando, vamos tentar primeiro tratar co-requisito como pos processamento
int PreModelo::cria_preVariavel_aluno_correquisito( int campusId, int grupoAlunosAtualId, int P_ATUAL )
{
    int num_vars = 0;

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();
	
	// Para cada conjunto de alunos cp com id menor ou igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;

		if ( grupoAlunosId > grupoAlunosAtualId )
		{
			break;
		}

		GGroup< Aluno *, LessPtr< Aluno > > cjtAlunos = problemData->cjtAlunos[ grupoAlunosId ];
	
		ITERA_GGROUP_LESSPTR( itAluno, cjtAlunos, Aluno )
		{
			Aluno *aluno = *itAluno;

			if ( !aluno->hasCampusId(campusId) )
			{
				continue;
			}

			std::map< int /*tuplaId*/, GGroup< int/*discId*/ > > mapCorrequisitos = aluno->getCorrequisitos();
			std::map< int /*tuplaId*/, GGroup< int/*discId*/ > >::iterator 
				itMap = mapCorrequisitos.begin();
			for ( ; itMap != mapCorrequisitos.end(); itMap++ )
			{		
				int tupla = itMap->first;

				VariablePre v;
				v.reset();
		//		v.setType( VariablePre::V_PRE_ALUNO_CORREQUISITO );
				v.setAluno( aluno );
		//		v.setTupla( tupla );

				// Coeficiente na funcao objetivo
				double coef = 0.0;
				
				// Limites da variavel						 
				double lowerBound = 0.0;
				double ub = 1.0;

				if ( vHashPre.find( v ) == vHashPre.end() )
				{
					vHashPre[v] = lp->getNumCols();

					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, ub, ( char * )v.toString().c_str() );
					lp->newCol(col);
				
					num_vars++;
				}				
			}
		}	
	}

	return num_vars;
}

// n_{i,d,cp}
int PreModelo::cria_preVariavel_nroAlunos_turma( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
	
   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_disciplina, it_CpDisc->second, int )
      {
		 Disciplina *disciplina = problemData->refDisciplinas[ *it_disciplina ];
		 		 
		 int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );
		 
 		 if ( discGrupoAlunosId > grupoAlunosId || 
			  discGrupoAlunosId == 0 )
		 {
			 continue;
		 }
		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            VariablePre v;
            v.reset();
            v.setType( VariablePre::V_PRE_NRO_ALUNOS );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );				// cp

            if ( vHashPre.find(v) == vHashPre.end() )
            {
                lp->getNumCols();
                vHashPre[v] = lp->getNumCols();

			    int maxP = disciplina->getMaxAlunosP();
			    int maxT = disciplina->getMaxAlunosT();
			    int maxAlunosTurma = ( (maxP > maxT) ? maxP : maxT );
			   
			    double coef = 0.0;
				double lowerBound = 0.0;
				double upperBound = maxAlunosTurma;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				num_vars++;
            }
         }
      }
   }

	return num_vars;
}

// no_{i,d,oft}
int PreModelo::cria_preVariavel_nroAlunos_turma_oferta( int campusId, int grupoAlunosId, int P_ATUAL, int r )
{
	int num_vars = 0;
		
	ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
	{		
		Disciplina* disciplina = *itDisc;

		if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			 problemData->cp_discs[campusId].end() )
		{
			continue;
		}

		 int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );

 		 if ( discGrupoAlunosId > grupoAlunosId || 
			  discGrupoAlunosId == 0 )
		 {
			 continue;
		 }

		 ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
		{
			Oferta *oft = *itOft;

			if ( !oft->possuiDisciplina( disciplina ) )
				continue;

			// Calculando P_{d,oft}
			int qtdDem = problemData->haDemandaDiscNoCjtAlunosPorOferta( disciplina->getId(), oft->getId(), discGrupoAlunosId );
			
			if ( qtdDem == 0 )
			{
				continue;
			}

			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_NRO_ALUNOS_OFT );

				v.setTurma( turma );               // i
				v.setDisciplina( disciplina );     // d
				v.setOferta( oft );				   // c
				
				if ( vHashPre.find(v) == vHashPre.end() )
				{					
					vHashPre[v] = lp->getNumCols();
					
					double coef = 0.0;	
					double lowerBound = 0.0;
					double upperBound = qtdDem;

					OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
												
					lp->newCol( col );
					
					num_vars += 1;
				}
			}
		}
   }

	return num_vars;
}

// fd_{d,oft}


// ss_{a1,a2,dp}
int PreModelo::cria_preVariavel_alunos_mesma_turma_pratica( int campusId, int P )
{
	int numVars = 0;
	
	// Por enquanto só a Unit usa essa relação 1xN
	if ( ! problemData->parametros->discPratTeor1xN_antigo )
	{
		return numVars;
	}
	
	Campus *cp = problemData->refCampus[campusId];
	
	std::map< Disciplina*, GGroup<Aluno*, LessPtr<Aluno>>, LessPtr<Disciplina> > mapDiscPratAlunos;

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

			if ( itAlDemanda->getCampus()->getId() != campusId )
			{
				continue;
			}

			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;

			if ( disciplina->getId() < 0 )
				mapDiscPratAlunos[ disciplina ].add( aluno );
			
			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );		
			if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
			{
				int chRequerida = aluno->getCargaHorariaOrigRequeridaP1();
				int chAtendida = problemData->cargaHorariaJaAtendida( aluno );
				if ( chRequerida - chAtendida <= 0 )
						continue;

				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{
					if ( itDisc->getId() < 0 )
					if ( problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, *itDisc ) )
					{
						mapDiscPratAlunos[ *itDisc ].add( aluno );
					}
				}
			}
			#endif
		}
	}

	std::map< Disciplina*, GGroup<Aluno*, LessPtr<Aluno>>, LessPtr<Disciplina> >::iterator
		itMap = mapDiscPratAlunos.begin();
	for ( ; itMap != mapDiscPratAlunos.end(); itMap++ )
	{
		Disciplina *discPratica = itMap->first;
		GGroup<Aluno*, LessPtr<Aluno>> alunos = itMap->second;

		ITERA_GGROUP_LESSPTR( itAluno1, alunos, Aluno )
		{
			ITERA_GGROUP_LESSPTR( itAluno2, alunos, Aluno )
			{
				if ( itAluno1->getAlunoId() >= itAluno2->getAlunoId() )
					continue;

				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALUNOS_MESMA_TURMA_PRAT );
				v.setParAlunos( *itAluno1, *itAluno2 );
				v.setDisciplina( discPratica );
					
				if ( vHashPre.find( v ) == vHashPre.end() )
				{
					vHashPre[ v ] = lp->getNumCols();
									
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

	return numVars;
}

// fmd1_{a}, fmd2_{a}, fmd3_{a}
int PreModelo::cria_preVariavel_folga_minimo_demanda_por_aluno( int campusId, int P_ATUAL )
{
    int num_vars = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return num_vars;
   }

	Campus *cp = problemData->refCampus[campusId];

	double coef1 = 100;
	double coef2 = 500;
	double coef3 = 1000;

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;
		
		double ub = (double) aluno->getNroCredsOrigRequeridosP1();
		
		double ub1 = ub/3;
		double ub2 = ub/3;
		double ub3 = ub/3;

		VariablePre v;
		v.reset();
		v.setType( VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND1 );
		v.setAluno( aluno );
		
		if ( vHashPre.find( v ) == vHashPre.end() )
		{
			vHashPre[v] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef1, 0.0, ub1, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			num_vars++;
		}		

		v.reset();
		v.setType( VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND2 );
		v.setAluno( aluno );
		
		if ( vHashPre.find( v ) == vHashPre.end() )
		{
			vHashPre[v] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef2, 0.0, ub2, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			num_vars++;
		}		

		v.reset();
		v.setType( VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND3 );
		v.setAluno( aluno );
		
		if ( vHashPre.find( v ) == vHashPre.end() )
		{
			vHashPre[v] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef3, 0.0, ub3, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			num_vars++;
		}		
	}

	return num_vars;

}

// fos_{i,d,cp}
int PreModelo::cria_preVariavel_folga_ocupacao_sala( int campusId, int P_ATUAL )
{
	int num_vars = 0;
	
	if ( ! problemData->parametros->min_folga_ocupacao_sala )
	{
		return num_vars;
	}

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_disciplina, it_CpDisc->second, int )
      {
		 Disciplina *disciplina = problemData->refDisciplinas[ *it_disciplina ];
		 		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            VariablePre v;
            v.reset();
            v.setType( VariablePre::V_PRE_FOLGA_OCUPA_SALA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );				// cp

            if ( vHashPre.find(v) == vHashPre.end() )
            {
                lp->getNumCols();
                vHashPre[v] = lp->getNumCols();

			    int maxP = disciplina->getMaxAlunosP();
			    int maxT = disciplina->getMaxAlunosT();
			    int maxAlunosTurma = ( (maxP > maxT) ? maxP : maxT );
			   
			    double coef = 2.0;
				double lowerBound = 0.0;
				double upperBound = maxAlunosTurma;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				num_vars++;
            }
         }
      }
   }

	return num_vars;
}

// k_{d}
int PreModelo::cria_preVariavel_usa_disciplina( int campusId, int P )
{
	int numVars=0;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCpDisc = problemData->cp_discs.begin();
   for ( ; itCpDisc != problemData->cp_discs.end(); itCpDisc++ )
   {
	   int cpId = itCpDisc->first;
	   if ( cpId != campusId ) continue;

	   Campus * cp = problemData->refCampus[campusId];

	   // Para cada disciplina do campus
	   GGroup< int >::iterator itDisc = itCpDisc->second.begin();
	   for ( ; itDisc != itCpDisc->second.end(); itDisc++ )
	   {
		    int discId = *itDisc;
		    Disciplina *disciplina = problemData->refDisciplinas[discId];
		   		   				
			VariablePre v;
			v.reset();
			v.setType( VariablePre::V_PRE_USA_DISCIPLINA );
			v.setDisciplina( disciplina );	 // d
				
			if ( vHashPre.find( v ) == vHashPre.end() )
			{
				vHashPre[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;
		
				OPT_COL col( OPT_COL::VAR_BINARY, 1.0, lowerBound, upperBound,
					( char * )v.toString().c_str());

				lp->newCol( col );
				numVars++;
			}
	    }
	}

	return numVars;
}

// uu_{u,oft,p}
int PreModelo::cria_preVariavel_usa_periodo_unid( int campusId )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->limitar_unidades_por_periodo )
	{
		return num_vars;
	}

	Campus* cp = problemData->refCampus[campusId];
	  
	ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
	{
		ITERA_GGROUP_LESSPTR( itOferta, cp->ofertas, Oferta )
		{
			Oferta* oft = *itOferta;
						
			ITERA_GGROUP_N_PT( itPeriodo, oft->curriculo->periodos, int )
			{
				int periodo = *itPeriodo;

				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_UNID_PERIODO );
				v.setPeriodo( periodo );				// p
				v.setOferta( oft );						// oft
				v.setUnidade( *itUnidade );				// u
								
				if ( vHashPre.find(v) == vHashPre.end() )
				{
					vHashPre[v] = lp->getNumCols();
									
					double coef = 0.0;
					int lowerBound = 0;
					int upperBound = 1;
						
					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
						( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				}
			}
		}
   }

	return num_vars;
}

// us_{s,oft,p}
int PreModelo::cria_preVariavel_usa_periodo_sala( int campusId )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->limitar_salas_por_periodo )
	{
		return num_vars;
	}

	Campus* cp = problemData->refCampus[campusId];
	  
	ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
	{
		ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
		{
			ITERA_GGROUP_LESSPTR( itOferta, cp->ofertas, Oferta )
			{
				Oferta* oft = *itOferta;
					
				ITERA_GGROUP_N_PT( itPeriodo, oft->curriculo->periodos, int )
				{
					int periodo = *itPeriodo;

					VariablePre v;
					v.reset();
					v.setType( VariablePre::V_PRE_SALA_PERIODO );
					v.setPeriodo( periodo );				// p
					v.setOferta( oft );						// oft
					v.setSubCjtSala( *itCjtSala );			// cjtSala

					if ( vHashPre.find(v) == vHashPre.end() )
					{
						vHashPre[v] = lp->getNumCols();
									
						double coef = 0.0;
						int lowerBound = 0;
						int upperBound = 1;
						
						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
							( char * )v.toString().c_str() );

						lp->newCol( col );
						num_vars++;
					}
				}
			}
		}
   }

	return num_vars;
}

// fus_{oft,p}
int PreModelo::cria_preVariavel_folga_periodo_sala( int campusId, int prioridade )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->limitar_salas_por_periodo )
	{
		return num_vars;
	}

	Campus* cp = problemData->refCampus[campusId];
	  
	ITERA_GGROUP_LESSPTR( itOferta, cp->ofertas, Oferta )
	{
		Oferta* oft = *itOferta;
					
		ITERA_GGROUP_N_PT( itPeriodo, oft->curriculo->periodos, int )
		{
			int periodo = *itPeriodo;
			
			if ( ! problemData->possibilidCompartNoPeriodo( oft, periodo, campusId, prioridade ) )
				continue;

			VariablePre v;
			v.reset();
			v.setType( VariablePre::V_PRE_FOLGA_SALA_PERIODO );
			v.setPeriodo( periodo );				// p
			v.setOferta( oft );						// oft

			if ( vHashPre.find(v) == vHashPre.end() )
			{
				vHashPre[v] = lp->getNumCols();
									
				double coef = 100.0;
				int lowerBound = 0;
				int upperBound = max( lowerBound, cp->getTotalSalas() - 1 );
						
				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
					( char * )v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
		}
	}

	return num_vars;
}

// fuu_{oft,p}
int PreModelo::cria_preVariavel_folga_periodo_unid( int campusId, int prioridade )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->limitar_unidades_por_periodo )
	{
		return num_vars;
	}

	Campus* cp = problemData->refCampus[campusId];
	  
	ITERA_GGROUP_LESSPTR( itOferta, cp->ofertas, Oferta )
	{
		Oferta* oft = *itOferta;
					
		ITERA_GGROUP_N_PT( itPeriodo, oft->curriculo->periodos, int )
		{
			int periodo = *itPeriodo;
			
			if ( ! problemData->possibilidCompartNoPeriodo( oft, periodo, campusId, prioridade ) )
				continue;

			VariablePre v;
			v.reset();
			v.setType( VariablePre::V_PRE_FOLGA_UNID_PERIODO );
			v.setPeriodo( periodo );				// p
			v.setOferta( oft );						// oft

			if ( vHashPre.find(v) == vHashPre.end() )
			{
				vHashPre[v] = lp->getNumCols();
									
				double coef = 200.0;
				int lowerBound = 0;
				int upperBound = max( lowerBound, cp->unidades.size() - 1 );
						
				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
					( char * )v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
		}
	}

	return num_vars;
}

// da_{cp,d}
int PreModelo::cria_preVariavel_distrib_alunos( int campusId )
{
	int numVars=0;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCpDisc = problemData->cp_discs.begin();
   for ( ; itCpDisc != problemData->cp_discs.end(); itCpDisc++ )
   {
	   int cpId = itCpDisc->first;
	   if ( cpId != campusId ) continue;

	   Campus * cp = problemData->refCampus[campusId];

	   // Para cada disciplina do campus
	   GGroup< int >::iterator itDisc = itCpDisc->second.begin();
	   for ( ; itDisc != itCpDisc->second.end(); itDisc++ )
	   {
		    int discId = *itDisc;
		    Disciplina *disciplina = problemData->refDisciplinas[discId];
		   		   				
			VariablePre v;
			v.reset();
			v.setType( VariablePre::V_PRE_DISTRIB_ALUNOS );
			v.setDisciplina( disciplina );		// d
			v.setCampus( cp );					// cp

			if ( vHashPre.find( v ) == vHashPre.end() )
			{
				vHashPre[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = cp->getMaiorSala();
		
				OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.5, lowerBound, upperBound,
					( char * )v.toString().c_str());

				lp->newCol( col );
				numVars++;
			}
	    }
	}

	return numVars;
}

// fp_{d,g}
int PreModelo::cria_preVariavel_folga_disponib_prof( int campusId )
{
	int numVars=0;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCpDisc = problemData->cp_discs.begin();
   for ( ; itCpDisc != problemData->cp_discs.end(); itCpDisc++ )
   {
	   int cpId = itCpDisc->first;
	   if ( cpId != campusId ) continue;

	   Campus * cp = problemData->refCampus[campusId];

	   // Para cada disciplina do campus
	   GGroup< int >::iterator itDisc = itCpDisc->second.begin();
	   for ( ; itDisc != itCpDisc->second.end(); itDisc++ )
	   {
		    int discId = *itDisc;
		    Disciplina *disciplina = problemData->refDisciplinas[discId];

			for( auto itFase = problemData->fasesDosTurnos.begin(); 
				itFase != problemData->fasesDosTurnos.end(); itFase++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_SLACK_PROF );
				v.setDisciplina( disciplina );		// d
				v.setTurno( itFase->first );		// g

				if ( vHashPre.find( v ) == vHashPre.end() )
				{
					vHashPre[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = 100000;
		
					OPT_COL col( OPT_COL::VAR_INTEGRAL, 50.0, lowerBound, upperBound,
						( char * )v.toString().c_str());

					lp->newCol( col );
					numVars++;
				}
			}
	    }
	}

	return numVars;
}


 /*******************************************************************************************************************
								CRIAÇÃO DE RESTRIÇÕES DO PRE-MODELO   
  ******************************************************************************************************************/



int PreModelo::cria_preRestricoes( int campusId, int prioridade, int cjtAlunosId, int r )
{
	int restricoes = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	timer.start();
	restricoes += cria_preRestricao_carga_horaria( campusId, cjtAlunosId );					// Restrição 1.1
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_carga_horaria: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
		
	// **********************************************************
	if ( !PREMODELO_POR_TURNO )
	{
	timer.start();
	restricoes += cria_preRestricao_max_cred_sala_sl( campusId, cjtAlunosId );				// Restrição 1.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_max_cred_sala_sl: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	}

	// **********************************************************
	if ( PREMODELO_POR_TURNO )
	{
	timer.start();
	restricoes += cria_preRestricao_max_cred_sala_turno( campusId, cjtAlunosId );			// Restrição 1.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "Restricao cria_preRestricao_max_cred_sala_turno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_preRestricao_set_turno_por_turma( campusId, cjtAlunosId );			// Restrição 1.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "Restricao cria_preRestricao_set_turno_por_turma: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


//	timer.start();
//	restricoes += cria_preRestricao_limita_turno_por_turma( campusId, cjtAlunosId );			// Restrição 1.2
//	timer.stop();
//	dif = timer.getCronoCurrSecs();
//
//#ifdef PRINT_cria_restricoes
//	std::cout << "Restricao cria_preRestricao_limita_turno_por_turma: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
//	numRestAnterior = restricoes;
//#endif


	timer.start();
	restricoes += cria_preRestricao_turnoIES_viavel_por_turma( campusId, cjtAlunosId );			// Restrição 1.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "Restricao cria_preRestricao_turnoIES_viavel_por_turma: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_tempo_max_aluno_sabado( campusId );			// Restrição 1.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "Restricao cria_preRestricao_tempo_max_aluno_sabado: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_preRestricao_tempo_max_aluno_turno( campusId );			// Restrição 1.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "Restricao cria_preRestricao_tempo_max_aluno_turno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_preRestricao_associa_aluno_turno( campusId );			// Restrição 1.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "Restricao cria_preRestricao_associa_aluno_turno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	}
	// **********************************************************


	timer.start();
	restricoes += cria_preRestricao_ativacao_var_o( campusId, cjtAlunosId );				// Restrição 1.3
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_ativacao_var_o: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_evita_mudanca_de_sala( campusId, cjtAlunosId );			// Restrição 1.4
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_evita_mudanca_de_sala: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_compartilhamento_incompat( campusId, cjtAlunosId );		// Restrição 1.8
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_compartilhamento_incompat: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif    

	timer.start();
	restricoes += cria_preRestricao_proibe_compartilhamento( campusId, cjtAlunosId );		// Restrição 1.9
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_proibe_compartilhamento: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_ativacao_var_z( campusId, cjtAlunosId );				// Restrição 1.10
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_ativacao_var_z: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_evita_turma_disc_camp_d( campusId, cjtAlunosId );		// Restrição 1.11
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_evita_turma_disc_camp_d: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_abre_turmas_em_sequencia( campusId, cjtAlunosId, prioridade, r );		// Restrição 1.13
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_abre_turmas_em_sequencia: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   


	timer.start();
	restricoes += cria_preRestricao_limite_sup_creds_sala( campusId, cjtAlunosId );		// Restrição 1.15
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_limite_sup_creds_sala: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	
	timer.start();
//	restricoes += cria_preRestricao_turma_mesma_disc_sala_dif( campusId, cjtAlunosId );		// Restrição 1.14
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_turma_mesma_disc_sala_dif: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	// Não é mais necessaria se não for usar a variavel "a"
	// IGUAL A cria_preRestricao_nro_alunos_por_turma2()

	//timer.start();
	//restricoes += cria_preRestricao_atendimento_aluno_peloHash( campusId, cjtAlunosId );			// Restrição 1.18
	//timer.stop();
	//dif = timer.getCronoCurrSecs();

	//#ifdef PRINT_cria_restricoes
	//std::cout << "numRest \"1.18\": " << (restricoes - numRestAnterior) << std::endl;
	//numRestAnterior = restricoes;
	//#endif   

	if ( !PREMODELO_POR_TURNO ) // modelo com turno tem restrição de tempo por aluno por turno, substitui essa
	{
    timer.start();
	restricoes += cria_preRestricao_limite_cred_aluno( campusId, cjtAlunosId, prioridade ); // Restrição 1.27
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_limite_cred_aluno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif  
	}

	/* // Não precisa dessa restrição, caso a restrição de folga de demanda já
		// garanta a soma dos "s" menor ou igual a 1

	restricoes += cria_preRestricao_aluno_unica_turma_disc( campusId, cjtAlunosId );		// Restrição 1.19

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.19\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
	#endif
	*/


	timer.start();
	restricoes += cria_preRestricao_prioridadesDemanda( campusId, prioridade, cjtAlunosId );		// Restrição 1.21
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_prioridadesDemanda: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
	
#ifndef KROTON
#ifndef UNIT
#ifndef UNIRITTER
	timer.start();
	restricoes += cria_preRestricao_ativa_var_compart_turma( campusId, cjtAlunosId, prioridade );		// Restrição 1.23
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_ativa_var_compart_turma: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
		
	// Só cria a restrição se estiver fixando salas definidas no EstimaTurmas. Caso contrario, fica inviável o nro de restrições
	if ( FIXA_SALAS_ESTIMATURMAS )
	{
		timer.start();
		restricoes += cria_preRestricao_aluno_sala( campusId, cjtAlunosId );		// Restrição 1.23
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest cria_preRestricao_aluno_sala: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
	}
#endif
#endif   
#endif   

	timer.start();
	restricoes += cria_preRestricao_formandos( campusId, cjtAlunosId, prioridade, r );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_formandos: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
		   
	#if defined(ALUNO_UNICO_CALENDARIO) || defined(ALUNO_TURNOS_DA_DEMANDA)

		timer.start();
		restricoes += cria_preRestricao_turma_turnosIES_com_intersecao( campusId, cjtAlunosId, prioridade, r );
			//cria_preRestricao_turma_calendarios_com_intersecao( campusId, cjtAlunosId, prioridade, r );
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest cria_preRestricao_turma_turnosIES_com_intersecao: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif

	#endif


	// o que é isso??
//	   timer.start();
//	   restricoes += cria_preRestricao_distribuicao_aluno( campusId, cjtAlunosId );		// Restrição 1.25
//	   timer.stop();
//	   dif = timer.getCronoCurrSecs();
//
//#ifdef PRINT_cria_restricoes
//	   std::cout << "numRest \"1.26\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
//	   numRestAnterior = restricoes;
//#endif

#ifndef PREMODELO_AGRUPA_ALUNO_POR_CURSO

	timer.start();
	if ( !PREMODELO_SEM_VAR_N )
		restricoes += cria_preRestricao_aloca_aluno_turma_aberta( campusId, cjtAlunosId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aloca_aluno_turma_aberta: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	if ( PREMODELO_SEM_VAR_N )
		restricoes += cria_preRestricao_nro_alunos_por_turma2( campusId, cjtAlunosId );
	else
		restricoes += cria_preRestricao_nro_alunos_por_turma( campusId, cjtAlunosId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_nro_alunos_por_turma2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	if ( PREMODELO_SEM_VAR_N )
		restricoes += cria_preRestricao_cap_sala_peloHash2( campusId, cjtAlunosId );					// Restrição 1.7
	else
		restricoes += cria_preRestricao_cap_sala_peloHash( campusId, cjtAlunosId );						// Restrição 1.7
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_cap_sala_peloHash2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif   

	timer.start();
	restricoes += cria_preRestricao_aluno_curso_disc_peloHash( campusId, cjtAlunosId );				// Restrição 1.6
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aluno_curso_disc_peloHash: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	if ( PREMODELO_SEM_VAR_N )	
		restricoes += cria_preRestricao_limita_abertura_turmas_peloHash2( campusId, cjtAlunosId );		// Restrição 1.12
	else
		restricoes += cria_preRestricao_limita_abertura_turmas_peloHash( campusId, cjtAlunosId );		// Restrição 1.12
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_limita_abertura_turmas_peloHash2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif   

	timer.start();
	restricoes += cria_preRestricao_aluno_discPraticaTeorica_hash_MxN( campusId );		// Restrição 1.20
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aluno_discPraticaTeorica_hash_MxN: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
		
		
	timer.start();
	restricoes += cria_preRestricao_aluno_discPraticaTeorica_1x1( campusId );		// Restrição 1.20
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aluno_discPraticaTeorica_1x1: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

		
	timer.start();
	restricoes += cria_preRestricao_aluno_discPraticaTeorica_1xN( campusId );		// Restrição 1.20
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aluno_discPraticaTeorica_1xN: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	/*
	timer.start();
	restricoes += cria_preRestricao_aluno_discPraticaTeorica_1xN_antiga( campusId );		// Restrição 1.20
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aluno_discPraticaTeorica_1xN_antiga: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_preRestricao_alunos_mesma_turma_pratica( campusId );		// Restrição 1.20
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_alunos_mesma_turma_pratica: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif	
	*/
		

	timer.start();
	restricoes += cria_preRestricao_cap_aloc_dem_disc_aluno( campusId, cjtAlunosId );				// Restrição 1.5
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_cap_aloc_dem_disc_aluno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
		   
	#if defined(ALUNO_UNICO_CALENDARIO) || defined(ALUNO_TURNOS_DA_DEMANDA)
		timer.start();
		restricoes += cria_preRestricao_setV_TurnoIES( campusId, cjtAlunosId );
			//cria_preRestricao_setV2( campusId, cjtAlunosId );
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest cria_preRestricao_setV_TurnoIES: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
				
		//#ifdef UNIT
        timer.start();
        restricoes += cria_preRestricao_aloca_turma_sab_turnoIES( campusId, cjtAlunosId );
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest cria_preRestricao_aloca_turma_sab_turnoIES: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
        timer.start();
        restricoes += cria_preRestricao_max_cred_sab( campusId, cjtAlunosId );
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest cria_preRestricao_max_cred_sab: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
		//#endif
	#endif
			
	timer.start();
	if ( PREMODELO_POR_TURNO ) restricoes += cria_preRestricao_soma_cred_sala_por_turno( campusId, cjtAlunosId );		// Restrição 1.26
	else restricoes += cria_preRestricao_soma_cred_sala( campusId, cjtAlunosId );										// Restrição 1.26
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_soma_cred_sala_por_turno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
		
	timer.start();
	restricoes += cria_preRestricao_aloc_min_aluno( campusId, cjtAlunosId );		// Restrição 1.26
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aloc_min_aluno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

		
	timer.start();
	restricoes += cria_preRestricao_folga_ocupacao_sala( campusId, cjtAlunosId );		// Restrição 1.26
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_folga_ocupacao_sala: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
		

#endif

#ifdef PREMODELO_AGRUPA_ALUNO_POR_CURSO

	timer.start();
	restricoes += cria_preRestricao_aloca_aluno_turma_aberta2( campusId, cjtAlunosId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aloca_aluno_turma_aberta2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_preRestricao_cap_sala_peloHash2( campusId, cjtAlunosId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_cap_sala_peloHash2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_preRestricao_aluno_curso_disc_peloHash2( campusId, cjtAlunosId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aluno_curso_disc_peloHash2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_preRestricao_limita_abertura_turmas_peloHash2( campusId, cjtAlunosId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_limita_abertura_turmas_peloHash2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_preRestricao_aluno_discPraticaTeorica_oferta( campusId, cjtAlunosId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_aluno_discPraticaTeorica_oferta: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_preRestricao_cap_aloc_dem_disc_aluno2( campusId, cjtAlunosId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_cap_aloc_dem_disc_aluno2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
				   
	#if defined(ALUNO_UNICO_CALENDARIO) || defined(ALUNO_TURNOS_DA_DEMANDA)
		timer.start();
		restricoes += cria_preRestricao_setV_TurnoIES( campusId, cjtAlunosId );
				//cria_preRestricao_setV2( campusId, cjtAlunosId );
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest cria_preRestricao_setV_TurnoIES: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif

		//#ifdef UNIT
        timer.start();
        restricoes += cria_preRestricao_aloca_turma_sab_turnoIES( campusId, cjtAlunosId );
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest cria_preRestricao_aloca_turma_sab_turnoIES: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
        timer.start();
        restricoes += cria_preRestricao_max_cred_sab( campusId, cjtAlunosId );
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest cria_preRestricao_max_cred_sab: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
		//#endif
	#endif

#endif
		

	timer.start();
	restricoes += cria_preRestricao_usa_disciplina( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_usa_disciplina: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
	
	timer.start();
	restricoes += cria_preRestricao_usa_unid_periodo( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_usa_unid_periodo: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
	
	timer.start();
	restricoes += cria_preRestricao_limita_unid_periodo( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_limita_unid_periodo: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_preRestricao_usa_sala_periodo( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_usa_sala_periodo: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_preRestricao_limita_sala_periodo( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_limita_sala_periodo: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
	
	timer.start();
	restricoes += cria_preRestricao_distrib_alunos( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_distrib_alunos: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif


	timer.start();
	//restricoes += cria_preRestricao_disponib_profs( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest cria_preRestricao_disponib_profs: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

		////timer.start();
		////restricoes += cria_preRestricao_forca_min_atendimento( campusId, cjtAlunosId );		// Restrição 1.30
		////timer.stop();
		////dif = timer.getCronoCurrSecs();

		////#ifdef PRINT_cria_restricoes
	 ////  std::cout << "numRest \"1.30\": " << (restricoes - numRestAnterior) << std::endl;
	 ////  numRestAnterior = restricoes;
		////#endif   

	return restricoes;
}

int PreModelo::add_preRestricoes( int campusId, int prioridade, int cjtAlunosId, int r )
{
	int restricoes = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	#ifdef PREMODELO_AGRUPA_ALUNO_POR_CURSO

		timer.start();
		restricoes += cria_preRestricao_nro_alunos_por_turma2( campusId, cjtAlunosId );
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.30\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif

		timer.start();
		restricoes += cria_preRestricao_aluno_discPraticaTeorica_MxN( campusId, cjtAlunosId );		// Restrição 1.20
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.34\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif

		timer.start();
		restricoes += cria_preRestricao_soma_cred_sala( campusId, cjtAlunosId );		// Restrição 1.26
		timer.stop();
		dif = timer.getCronoCurrSecs();
		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.26\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif

      timer.start();
      restricoes += cria_preRestricao_aluno_unica_turma_disc( campusId, cjtAlunosId );		// Restrição 1.19
      timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
	   std::cout << "numRest \"1.19\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif

	#endif

	return restricoes;
}

// Restrição 1.1
int PreModelo::cria_preRestricao_carga_horaria( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;     
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );
		 		 
		  map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
		    itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

		  if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
		  {
			  if ( itMapDiscCjt->second	!= cjtAlunosId &&
				   NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
				  continue;
		  }
		  else
		  {
			  if ( problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
				std::cout<<"\nERROR: disciplina "<<disciplina->getId()<<"nao pertence a nenhum cjt\n";
			  continue;
		  }
		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {	 

            c.reset();
            c.setType( ConstraintPre::C_PRE_CARGA_HORARIA );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHashPre.find( c ) != cHashPre.end() )
            {
               continue;
            }

			nnz = disciplina->getNSalasAptas();

            OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );

            // ---

            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
				    if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
					     itCjtSala->disciplinas_associadas.end() )
					{
						continue;
				    }

					v.reset();

				   #ifdef SALA_UNICA_POR_TURMA 
					v.setType( VariablePre::V_PRE_OFERECIMENTO );
				   #else
					v.setType( VariablePre::V_PRE_CREDITOS );
				   #endif

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );             

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
               }
            }

            // ---

            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );

            v.setCampus( *itCampus );
            v.setDisciplina( disciplina );
            v.setTurma( turma );

            it_v = vHashPre.find( v );
            if( it_v != vHashPre.end() )
            {
				#ifdef SALA_UNICA_POR_TURMA 
				 row.insert( it_v->second, -1.0 );
				#else
				 row.insert( it_v->second, -( disciplina->getCredPraticos() + 
											disciplina->getCredTeoricos() ) );
				#endif
            }

            // ---

            if ( row.getnnz() != 0 )
            {
               cHashPre[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

	return restricoes;
}


// Restrição 1.2
int PreModelo::cria_preRestricao_max_cred_sala_turno( int campusId, int cjtAlunosId  )
{   
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariablePre v;
	ConstraintPre c;

	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	vit = vHashPre.begin();
	for (; vit != vHashPre.end(); vit++)
	{
		if( vit->first.getType() != VariablePre::V_PRE_OFERECIMENTO_TURNO )
		{
			continue;
		}

		VariablePre v = vit->first;
		
		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		c.reset();
		c.setType( ConstraintPre::C_PRE_MAX_CRED_SALA_TURNO );
		c.setUnidade( v.getUnidade() );
		c.setSubCjtSala( v.getSubCjtSala() );
		c.setTurno( v.getTurno() );

		double coef = v.getDisciplina()->getTempoCredSemanaLetiva() * v.getDisciplina()->getTotalCreditos();

		cit = cHashPre.find(c);
		if(cit == cHashPre.end())
		{
			double maxTime = v.getSubCjtSala()->maxLimiteTempoPermitidoNaSemana( v.getTurno() );
			double rhs = maxTime;

			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS , rhs , name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);		
		}
	}

	lp->updateLP();
	lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
	lp->updateLP();

	idxC.clear();
	idxR.clear();
	valC.clear();

	return restricoes;
}

// Restrição 1.2
int PreModelo::cria_preRestricao_set_turno_por_turma( int campusId, int cjtAlunosId  )
{   
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariablePre v;
	ConstraintPre c;

	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	vit = vHashPre.begin();
	for( ; vit != vHashPre.end(); vit++ )
	{
		double coef = 0.0;

		if( vit->first.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO )
		{
			coef = 1.0;
		}
		else if( vit->first.getType() == VariablePre::V_PRE_OFERECIMENTO )
		{
			coef = -1.0;
		}
		else
		{
			continue;
		}

		VariablePre v = vit->first;
		
		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		c.reset();
		c.setType( ConstraintPre::C_PRE_SET_TURNO_POR_TURMA );
		c.setTurma( v.getTurma() );
		c.setDisciplina( v.getDisciplina() );
		c.setSubCjtSala( v.getSubCjtSala() );
				
		cit = cHashPre.find(c);
		if(cit == cHashPre.end())
		{			
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 4, OPT_ROW::EQUAL, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

	lp->updateLP();
	lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
	lp->updateLP();

	idxC.clear();
	idxR.clear();
	valC.clear();

	return restricoes;
}

// Restrição 1.2: desativada
int PreModelo::cria_preRestricao_limita_turno_por_turma( int campusId, int cjtAlunosId  )
{   
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariablePre v;
	ConstraintPre c;

	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	Campus *campus = problemData->refCampus[ campusId ];

	vit = vHashPre.begin();
	for( ; vit != vHashPre.end(); vit++ )
	{
		double coef = 0.0;

		if( vit->first.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO )
		{
			coef = 1.0;
		}
		else
		{
			continue;
		}

		VariablePre v = vit->first;
		
		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		c.reset();
		c.setType( ConstraintPre::C_PRE_LIMITA_TURNO_POR_TURMA );
		c.setTurma( v.getTurma() );
		c.setDisciplina( v.getDisciplina() );
		c.setCampus( campus );
				
		cit = cHashPre.find(c);
		if(cit == cHashPre.end())
		{			
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 3, OPT_ROW::LESS, 1.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

	lp->updateLP();
	lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
	lp->updateLP();

	idxC.clear();
	idxR.clear();
	valC.clear();

	return restricoes;
}

// Restrição 1.2: desativada
int PreModelo::cria_preRestricao_turno_viavel_por_turma( int campusId, int cjtAlunosId  )
{   
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariablePre v;
	ConstraintPre c;

	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		double coef = 0.0;

		if( vit->first.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO )
		{		
			coef = 1.0;

			VariablePre v = vit->first;
		
			if ( v.getUnidade()->getIdCampus() != campusId )
				continue;
			
			GGroup<Calendario*, LessPtr<Calendario>> calendarios = v.getDisciplina()->getCalendarios();
			ITERA_GGROUP_LESSPTR( itCalend, calendarios, Calendario )
			{
				Calendario *calendario = *itCalend;

				int turno = v.getTurno();
								
				GGroup<int> dias = calendario->dias(turno);
				
				bool turnoDiaComum = false;
				ITERA_GGROUP_N_PT( itDia, dias, int )
				{
					if ( v.getDisciplina()->possuiHorarioNoTurnoDia( *itDia, turno ) )
					{							
						turnoDiaComum = true;
						break;
					}
				}
				if ( turnoDiaComum ) continue;
					
				c.reset();
				c.setType( ConstraintPre::C_PRE_TURNO_VIAVEL_POR_TURMA );
				c.setTurma( v.getTurma() );
				c.setDisciplina( v.getDisciplina() );
				c.setSemanaLetiva( calendario );
				
				cit = cHashPre.find(c);
				if(cit == cHashPre.end())
				{			
					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS, 1.0, name );

					row.insert( vit->second, coef);

					cHashPre[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
				else
				{
					idxC.push_back(vit->second);
					idxR.push_back(cit->second);
					valC.push_back(coef);
				}
			}			
		}
		else if( vit->first.getType() == VariablePre::V_PRE_TURMA_CALENDARIO )
		{
			coef = 1.0;

			VariablePre v = vit->first;
		
			if ( v.getSubCjtSala()->getCampusId() != campusId )
				continue;

			c.reset();
			c.setType( ConstraintPre::C_PRE_TURNO_VIAVEL_POR_TURMA );
			c.setTurma( v.getTurma() );
			c.setDisciplina( v.getDisciplina() );
			c.setSemanaLetiva( v.getCalendario() );
				
			cit = cHashPre.find(c);
			if(cit == cHashPre.end())
			{			
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
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
	lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
	lp->updateLP();

	idxC.clear();
	idxR.clear();
	valC.clear();

	return restricoes;
}

// Restrição 1.2
int PreModelo::cria_preRestricao_turnoIES_viavel_por_turma( int campusId, int cjtAlunosId  )
{   
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariablePre v;
	ConstraintPre c;

	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		double coef = 0.0;

		if( vit->first.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO )
		{		
			coef = 1.0;

			VariablePre v = vit->first;
		
			if ( v.getUnidade()->getIdCampus() != campusId )
				continue;
			
			int turno = v.getTurno();

			ITERA_GGROUP_LESSPTR( itTurnoIES, problemData->turnosIES, TurnoIES )
			{
				TurnoIES *turnoIES = *itTurnoIES;

				GGroup< Horario*, LessPtr<Horario> > horsDiscTurno = v.getDisciplina()->getHorariosOuCorrespondentes( turnoIES );
				
				bool turnoDiaComum = false;
				ITERA_GGROUP_LESSPTR( itHor, horsDiscTurno, Horario )
				{
					if ( problemData->getFaseDoDia( itHor->horario_aula->getInicio() ) == turno )
					{							
						turnoDiaComum = true;
						break;
					}
				}
				if ( turnoDiaComum ) continue;
					
				c.reset();
				c.setType( ConstraintPre::C_PRE_TURNO_VIAVEL_POR_TURMA );
				c.setTurma( v.getTurma() );
				c.setDisciplina( v.getDisciplina() );
				c.setTurnoIES( turnoIES );
				
				cit = cHashPre.find(c);
				if(cit == cHashPre.end())
				{			
					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS, 1.0, name );

					row.insert( vit->second, coef);

					cHashPre[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
				else
				{
					idxC.push_back(vit->second);
					idxR.push_back(cit->second);
					valC.push_back(coef);
				}
			}			
		}
		else if( vit->first.getType() == VariablePre::V_PRE_TURMA_TURNOIES )
		{
			coef = 1.0;

			VariablePre v = vit->first;
		
			if ( v.getSubCjtSala()->getCampusId() != campusId )
				continue;

			c.reset();
			c.setType( ConstraintPre::C_PRE_TURNO_VIAVEL_POR_TURMA );
			c.setTurma( v.getTurma() );
			c.setDisciplina( v.getDisciplina() );
			c.setTurnoIES( v.getTurnoIES() );
				
			cit = cHashPre.find(c);
			if(cit == cHashPre.end())
			{			
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
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
	lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
	lp->updateLP();

	idxC.clear();
	idxR.clear();
	valC.clear();

	return restricoes;
}



// Restrição 1.2
int PreModelo::cria_preRestricao_max_cred_sala_sl( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;   
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

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
			ConstraintPre c;
			c.reset();
			c.setType( ConstraintPre::C_PRE_MAX_CRED_SALA );
			
			c.setUnidade( *itUnidade );
			c.setSubCjtSala( *itCjtSala );

			sprintf( name, "%s", c.toString().c_str() ); 
			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			double minTime = itCjtSala->minLimiteTempoPermitidoNaSemana();
			double maxTime = itCjtSala->maxTempoPermitidoNaSemana(problemData->mapDiscSubstituidaPor);
			double htps = 0;

			if ( maxTime >= 2*minTime ) // Se há diferença significativa entre tamanho das semanas letivas
				htps = ( maxTime + minTime ) / 3;
			else
				htps = ( maxTime + minTime ) / 2;

									
			nnz = itCjtSala->disciplinas_associadas.size() * 5; // estimando em media 5 turmas por disciplina

			OPT_ROW row( nnz, OPT_ROW::LESS , htps, name );

            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disc );
			   
               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
				   #ifndef SALA_UNICA_POR_TURMA 
                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_CREDITOS );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps

					 it_v = vHashPre.find( v );
					 if ( it_v != vHashPre.end() )
					 {
						 row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() );
					 }       
					#else
                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_OFERECIMENTO );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps

					 it_v = vHashPre.find( v );
					 if ( it_v != vHashPre.end() )
					 {
						 row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos() );
					 }     
					#endif
                }
            }

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
         }
      }
   }

	return restricoes;
}

// Restrição 1.3: desativada
int PreModelo::cria_preRestricao_ativacao_var_o( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   
   #ifdef SALA_UNICA_POR_TURMA  
		return restricoes;
   #endif

   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

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
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disc );
				
				map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
				itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

				if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
				{
					if ( itMapDiscCjt->second	!= cjtAlunosId &&
						NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
						continue;
				}
				else
				{
					continue;
				}

                for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
                {
					c.reset();
					c.setType( ConstraintPre::C_PRE_VAR_O );

					c.setUnidade( *itUnidade );
					c.setSubCjtSala( *itCjtSala );
					c.setDisciplina( disciplina );
					c.setTurma( turma );

					sprintf( name, "%s", c.toString().c_str() ); 
					if ( cHashPre.find( c ) != cHashPre.end() )
					{
						continue;
					}

					nnz = 2;

					OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );

					v.reset();
					v.setType( VariablePre::V_PRE_OFERECIMENTO );

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );

					it_v = vHashPre.find( v );
					if( it_v != vHashPre.end() )
					{
                  row.insert( it_v->second, - ( disciplina->getCredPraticos() + 
													  disciplina->getCredTeoricos() ) );
					}

					v.reset();
					v.setType( VariablePre::V_PRE_CREDITOS );

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}

					if ( row.getnnz() != 0 )
					{
						cHashPre[ c ] = lp->getNumRows();

						lp->addRow( row );
						restricoes++;
					}
				}
            }
         }
      }
   }

	return restricoes;
}

// Restrição 1.4
int PreModelo::cria_preRestricao_evita_mudanca_de_sala( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   
   #ifdef SALA_UNICA_POR_TURMA  
		return restricoes;
   #endif

   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		  map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
		    itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

		  if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
		  {
			  if ( itMapDiscCjt->second	!= cjtAlunosId &&
				   NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
				  continue;
		  }
		  else
		  {
			  continue;
		  }

         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintPre::C_EVITA_MUDANCA_DE_SALA );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHashPre.find( c ) != cHashPre.end() )
            {
               continue;
            }

			nnz = disciplina->getNSalasAptas();

            OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );
			
            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
				    if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
					     itCjtSala->disciplinas_associadas.end() )
					{
						continue;
				    }

					v.reset();
					v.setType( VariablePre::V_PRE_OFERECIMENTO );

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );             

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
               }
            }
			
            if ( row.getnnz() != 0 )
            {
               cHashPre[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}


/*
	Alocação de demanda por aluno
*/
int PreModelo::cria_preRestricao_cap_aloc_dem_disc_aluno2( int campusId, int cjtAlunosId  )
{
  int restricoes = 0;
   char name[ 100 ];
   int nnz=0;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
			
		Oferta *oferta;

		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA_OFERTA )
		{
			oferta = v.getOferta();
			coef = 1.0;		
		}
      else if( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			oferta = v.getOferta();
			coef = 1.0;
		}
		else
		{
			continue;
		}
		
		Disciplina *disciplina = v.getDisciplina();

		c.reset();
        c.setType( ConstraintPre::C_PRE_ALOC_DEM_DISC_OFERTA );
		c.setOferta( oferta );
		c.setDisciplina( disciplina );
						
		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			double rhs = problemData->haDemandaDiscNoCjtAlunosPorOferta( v.getDisciplina()->getId(), oferta->getId(), cjtAlunosId );
			int nnz = 100;

			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( nnz, OPT_ROW::EQUAL, rhs, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;
}


/*
	Alocação de demanda por aluno
*/
int PreModelo::cria_preRestricao_cap_aloc_dem_disc_aluno( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
      
   char name[ 200 ];
   int nnz=0;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( !aluno->hasCampusId(campusId) )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			#ifdef EQUIVALENCIA_DESDE_O_INICIO		
				if ( disciplina->getId() < 0 )
					continue;
			#endif

			// Pula a restrição caso a disciplina seja de conjuntoAluno anterior

			map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
			itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

			if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
			{
				if ( itMapDiscCjt->second != cjtAlunosId &&
					NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
					continue;
			}
			else
			{
				continue;
			}

			c.reset();
			c.setType( ConstraintPre::C_PRE_CAP_ALOC_DEM_DISC );
			c.setAluno( aluno );
			c.setDisciplina( disciplina );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}
			
			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );
				
			#ifdef EQUIVALENCIA_DESDE_O_INICIO		
				int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );		
				if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
				{
					ITERA_GGROUP_LESSPTR( itequiv, disciplina->discEquivSubstitutas, Disciplina )
					{
						if ( problemData->alocacaoEquivViavel( itAlDemanda->demanda, *itequiv ) )
						{
							nnz += itequiv->getNumTurmas();
							disciplinasPorAlDem.add( *itequiv );
						}
					}
				}
			#endif
				
			nnz += disciplina->getNumTurmas() + 1;
			OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0 , name );

			ITERA_GGROUP_LESSPTR( itDiscEq, disciplinasPorAlDem, Disciplina )
			{
				Disciplina *disciplinaEquiv = (*itDiscEq);

				#ifdef EQUIVALENCIA_DESDE_O_INICIO
					if ( disciplinaEquiv->getId() < 0 )
						continue;
				#endif

				for ( int turma=1; turma <= disciplinaEquiv->getNumTurmas(); turma++ )
				{
					for ( int turno=1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
					{
						VariablePre v;
						v.reset();
						v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
						v.setAluno( aluno );
						v.setDisciplina( disciplinaEquiv );
						v.setTurma( turma );
						v.setTurno( turno );
						v.setCampus( campus );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, 1.0 );
						}
					}
				}	
			}

			v.reset();
			v.setType( VariablePre::V_PRE_SLACK_DEMANDA );
			v.setDisciplina( disciplina );
			v.setAluno( aluno );

			it_v = vHashPre.find( v );
			if( it_v != vHashPre.end() )
			{
				row.insert( it_v->second, 1.0 );
			}

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;
}

// Restrição 1.6
int PreModelo::cria_preRestricao_aluno_curso_disc_peloHash2( int campusId, int cjtAlunosId  )
{
  int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
			
		Curso *curso;
		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_ALOC_ALUNO ) // b_{i,d,c}
		{
			curso = v.getCurso();

			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			 if ( problemData->parametros->considerar_equivalencia_por_aluno )
				coef = - problemData->maxDemandaDiscNoCursoEquiv( v.getDisciplina(), curso->getId() );			
			 else
				coef = - problemData->haDemandaDiscNoCurso( v.getDisciplina()->getId(), curso->getId() );			
			#else
				coef = - problemData->haDemandaDiscNoCurso( v.getDisciplina()->getId(), curso->getId() );			
			#endif		
		}
		//else if( v.getType() == VariablePre::V_PRE_NRO_ALUNOS_OFT )
      else if( v.getType() == VariablePre::V_PRE_ALUNOS ) // a_{i,d,s,oft}
		{
			curso = v.getOferta()->curso;
			coef = 1.0;
		}
		else
		{
			continue;
		}
		
		c.reset();
        c.setType( ConstraintPre::C_ALUNO_OFT_DISC );
		c.setCurso( curso );
		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
				
		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;
}


// Restrição 1.6
int PreModelo::cria_preRestricao_aluno_curso_disc_peloHash( int campusId, int cjtAlunosId  )
{
  int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
			
		Curso *curso;
		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_ALOC_ALUNO )					// b_{i,d,c}
		{
			curso = v.getCurso();
			
			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			 if ( problemData->parametros->considerar_equivalencia_por_aluno )
				coef = - problemData->maxDemandaDiscNoCursoEquiv( v.getDisciplina(), curso->getId() );	
			 else
				coef = - problemData->haDemandaDiscNoCurso( v.getDisciplina()->getId(), curso->getId() );			
			#else
				coef = - problemData->haDemandaDiscNoCurso( v.getDisciplina()->getId(), curso->getId() );			
			#endif	
		}
		else if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )	// s_{i,d,a}
		{			
			#ifdef EQUIVALENCIA_DESDE_O_INICIO
				AlunoDemanda *alDem = problemData->procuraAlunoDemOrigEquiv( v.getDisciplina(), v.getAluno(), 1 );
				if (alDem==NULL)
					alDem = problemData->procuraAlunoDemOrigEquiv( v.getDisciplina(), v.getAluno(), 2 );
			#else
				AlunoDemanda *alDem = problemData->procuraAlunoDemanda( v.getDisciplina()->getId(), v.getAluno()->getAlunoId(), 1 );
				if (alDem==NULL)
					alDem = problemData->procuraAlunoDemOrigEquiv( v.getDisciplina(), v.getAluno(), 2 );
			#endif	
			if ( alDem==NULL ){ std::cout<<"Erro! AlunoDemanda nao encontrado. Disc"
				<< v.getDisciplina()->getId() << " Aluno" << v.getAluno()->getAlunoId(); continue; }

			curso = alDem->getOferta()->curso;
			coef = 1.0;
		}
		else
		{
			continue;
		}
		
		c.reset();
        c.setType( ConstraintPre::C_ALUNO_OFT_DISC );
		c.setCurso( curso );
		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
				
		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;

	/*
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
			
		Curso *curso;
		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_ALOC_ALUNO )
		{
			curso = v.getCurso();
			coef = - problemData->haDemandaDiscNoCurso( v.getDisciplina()->getId(), curso->getId() );			
		}
		else if( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			curso = v.getOferta()->curso;
			coef = 1.0;		
		}
		else
		{
			continue;
		}
		
		c.reset();
        c.setType( ConstraintPre::C_ALUNO_OFT_DISC );
		c.setCurso( curso );
		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
				
		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;
   */
}

int PreModelo::cria_preRestricao_cap_sala_peloHash2( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   std::map< int /*turma*/, std::map< Disciplina*, GGroup<int /*column*/>, LessPtr<Disciplina>> > mapConstraintHash;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;
		double bigM = 0.0;
		
		if( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
		{
			coef = - ( v.getSubCjtSala()->capTotalSalas() - bigM );
		}
		else
		{
			continue;
		}

		if ( v.getUnidade()->getIdCampus() != campusId ) continue;

		c.reset();
        c.setType( ConstraintPre::C_CAP_SALA );
		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
		c.setUnidade( v.getUnidade() );
		c.setSubCjtSala( v.getSubCjtSala() );

		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, bigM, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			
			lp->addRow( row );
			restricoes++;

			//mapConstraintHash[v.getTurma()][v.getDisciplina()].add( cHashPre[ c ] );
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}
   
	/*vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;   
		if( v.getType() == VariablePre::V_PRE_NRO_ALUNOS_OFT )
		{
			coef = 1.0;
			if ( v.getOferta()->getCampusId() != campusId ) continue;
		}
		else
		{
			continue;
		}		

		int turma = v.getTurma();
		Disciplina *disciplina = v.getDisciplina();
		int idxColumn = vit->second;

		GGroup<int> ggroupConstraint = mapConstraintHash[turma][disciplina];
		ITERA_GGROUP_N_PT( it, ggroupConstraint, int )
		{
			int idxConstraint = *it;
		
			idxC.push_back(idxColumn);
			idxR.push_back(idxConstraint);
			valC.push_back(coef);
		}
	}*/
   vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;
		double bigM = 0.0;

		if( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			coef = 1.0;
			if ( v.getOferta()->getCampusId() != campusId ) continue;
		}
		else
		{
			continue;
		}		

      c.reset();
      c.setType( ConstraintPre::C_CAP_SALA );
		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
		c.setUnidade( v.getUnidade() );
		c.setSubCjtSala( v.getSubCjtSala() );

		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, bigM, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();
   
   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;
}


// Restrição 1.7
int PreModelo::cria_preRestricao_cap_sala_peloHash( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   std::map< int /*turma*/, std::map< Disciplina*, GGroup<int /*column*/>, LessPtr<Disciplina>> > mapConstraintHash;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;
		double bigM = 0.0;
		
		if( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
		{   
			int maxP = v.getDisciplina()->getMaxAlunosP();
		    int maxT = v.getDisciplina()->getMaxAlunosT();
		    int maxAlunos = ( (maxP > maxT) ? maxP : maxT );
						
			if (maxAlunos == 0)
				std::cout<<"\nATENCAO: maximo de alunos por turma" 
				" igual a zero para a disciplina "<< v.getDisciplina()->getId();

			bigM = maxAlunos;
			
			coef = - ( v.getSubCjtSala()->capTotalSalas() - bigM );
		}
		else
		{
			continue;
		}

		if ( v.getUnidade()->getIdCampus() != campusId ) continue;

		c.reset();
        c.setType( ConstraintPre::C_CAP_SALA );
		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
		c.setUnidade( v.getUnidade() );
		c.setSubCjtSala( v.getSubCjtSala() );

		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, bigM, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			
			lp->addRow( row );
			restricoes++;

			mapConstraintHash[v.getTurma()][v.getDisciplina()].add( cHashPre[ c ] );
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}
   
	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;   
		if( v.getType() == VariablePre::V_PRE_NRO_ALUNOS )
		{
			coef = 1.0;
			if ( v.getCampus()->getId() != campusId ) continue;
		}
		else
		{
			continue;
		}		

		int turma = v.getTurma();
		Disciplina *disciplina = v.getDisciplina();
		int idxColumn = vit->second;

		GGroup<int /*column*/> ggroupConstraint = mapConstraintHash[turma][disciplina];
		ITERA_GGROUP_N_PT( it, ggroupConstraint, int )
		{
			int idxConstraint = *it;
		
			idxC.push_back(idxColumn);
			idxR.push_back(idxConstraint);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();
   
   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;


	/* // Com variavel a
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
		{
			coef = - v.getSubCjtSala()->capTotalSalas();
		}
		else if( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			coef = 1.0;		
		}
		else
		{
			continue;
		}

		if ( v.getUnidade()->getIdCampus() != campusId ) continue;

		c.reset();
        c.setType( ConstraintPre::C_CAP_SALA );

		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
		c.setUnidade( v.getUnidade() );
		c.setSubCjtSala( v.getSubCjtSala() );

		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;
   */
}

// Restrição 1.8
int PreModelo::cria_preRestricao_compartilhamento_incompat( int campusId, int cjtAlunosId  )
{
       int restricoes = 0;
       char name[ 100 ];
       int nnz;

	   ConstraintPre c;
	   VariablePre v;
	   VariablePreHash::iterator it_v;	
	   Campus *cp = NULL;

	   cp = problemData->refCampus[ campusId ];

	   if ( cp==NULL )
	   {
		   std::cout<<"\nATENCAO: PreModelo::cria_preRestricao_compartilhamento_incompat( int campusId ): ";
		   std::cout<<"\nCampus" << campusId << "nao encontrado.\n";
		   return restricoes;
	   }

	   ITERA_GGROUP_LESSPTR( itCurso1, cp->cursos, Curso )
	   {
		   Curso* c1 = *itCurso1;

		   ITERA_GGROUP_INIC_LESSPTR( itCurso2, itCurso1, cp->cursos, Curso )
		   {
				Curso* c2 = *itCurso2;
				
			    if ( problemData->cursosCompativeis(c1, c2) || c1 == c2 )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
						 problemData->cp_discs[ campusId ].end() )
					{
						continue;
					}

					// Pula a restrição caso a disciplina seja de conjuntoAluno anterior

					map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
					itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

					if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
					{
						if ( itMapDiscCjt->second != cjtAlunosId &&
							NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
							continue;
					}
					else
					{
						continue;
					}

					#ifdef EQUIVALENCIA_DESDE_O_INICIO
					if ( problemData->parametros->considerar_equivalencia_por_aluno ){
					 if ( !problemData->haDemandaDiscNoCursoEquiv( disciplina, c1->getId() ) ||
						  !problemData->haDemandaDiscNoCursoEquiv( disciplina, c2->getId() ) )					
						 continue;
					}
					else if ( !problemData->haDemandaDiscNoCurso( disciplina->getId(), c1->getId() ) ||
							  !problemData->haDemandaDiscNoCurso( disciplina->getId(), c2->getId() ) )
						 continue;
					#else
					if ( !problemData->haDemandaDiscNoCurso( disciplina->getId(), c1->getId() ) ||
						 !problemData->haDemandaDiscNoCurso( disciplina->getId(), c2->getId() ) )
					{
						continue;
					}
					#endif
					
					for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintPre::C_PRE_ALUNOS_CURSOS_INCOMP );
						c.setParCursos( std::make_pair( c1, c2 ) );
						c.setDisciplina( disciplina );
						c.setTurma( turma );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHashPre.find( c ) != cHashPre.end() )
						{
							continue;
						}

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						v.reset();
						v.setType( VariablePre::V_PRE_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c1 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, 1 );
						}

						v.reset();
						v.setType( VariablePre::V_PRE_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c2 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert(it_v->second, 1);
						}

						if ( row.getnnz() != 0 )
						{
							cHashPre[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}

		return restricoes;
}

// Restrição 1.9
int PreModelo::cria_preRestricao_proibe_compartilhamento( int campusId, int cjtAlunosId  )
{
	int restricoes = 0;

    if ( problemData->parametros->permite_compartilhamento_turma_sel )
    {
	 	return restricoes;
    }

    char name[ 100 ];
    int nnz;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;
	   
	Campus *cp = NULL;

	cp = problemData->refCampus[ campusId ];

	if ( cp==NULL )
	{
		 std::cout<<"\nATENCAO: PreModelo::cria_preRestricao_compartilhamento_incompat( int campusId ): ";
		 std::cout<<"\nCampus" << campusId << "nao encontrado.\n";
		 return restricoes;
	}
	
	   ITERA_GGROUP_LESSPTR( itCurso1, cp->cursos, Curso )
	   {
		   Curso* c1 = *itCurso1;
		   
		   ITERA_GGROUP_INIC_LESSPTR( itCurso2, itCurso1, cp->cursos, Curso )
		   {
				Curso* c2 = *itCurso2;
			    
			    if ( !problemData->cursosCompativeis(c1, c2) || c1 == c2 )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
						 problemData->cp_discs[ campusId ].end() )
					{
						continue;
					}
												
					#ifdef EQUIVALENCIA_DESDE_O_INICIO
					 if ( problemData->parametros->considerar_equivalencia_por_aluno ){
						if ( !problemData->haDemandaDiscNoCursoEquiv( disciplina, c1->getId() ) ||
							 !problemData->haDemandaDiscNoCursoEquiv( disciplina, c2->getId() ) )					
							continue;
					 }
					 else if ( !problemData->haDemandaDiscNoCurso( disciplina->getId(), c1->getId() ) ||
							   !problemData->haDemandaDiscNoCurso( disciplina->getId(), c2->getId() ) )					
						continue;
					#else
					if ( !problemData->haDemandaDiscNoCurso( disciplina->getId(), c1->getId() ) ||
						 !problemData->haDemandaDiscNoCurso( disciplina->getId(), c2->getId() ) )
					{
						continue;
					}
					#endif

					// Pula a restrição caso a disciplina seja de conjuntoAluno anterior

					map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
					itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

					if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
					{
						if ( itMapDiscCjt->second != cjtAlunosId &&
							NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
							continue;
					}
					else
					{
						continue;
					}

					for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintPre::C_PRE_PROIBE_COMPARTILHAMENTO );
						c.setParCursos( std::make_pair( c1, c2 ) );
						c.setDisciplina( disciplina );
						c.setTurma( turma );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHashPre.find( c ) != cHashPre.end() )
						{
							continue;
						}

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						v.reset();
						v.setType( VariablePre::V_PRE_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c1 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, 1 );
						}

						v.reset();
						v.setType( VariablePre::V_PRE_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c2 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert(it_v->second, 1);
						}

						VariablePre v;
						v.reset();
						v.setType( VariablePre::V_PRE_SLACK_COMPARTILHAMENTO );
						v.setTurma( turma );				
						v.setDisciplina( disciplina );  
						v.setParCursos( std::make_pair(c1, c2) );	

						it_v = vHashPre.find( v );
						if ( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, -1 );
						}

						if ( row.getnnz() != 0 )
						{
							cHashPre[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}

		return restricoes;
}

// Restricao 1.10
int PreModelo::cria_preRestricao_ativacao_var_z( int campusId, int cjtAlunosId  )
{
	int restricoes = 0;
   
#ifdef SALA_UNICA_POR_TURMA 
   return restricoes;
#endif	

	char name[ 100 ];
    int nnz;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;

    Disciplina * disciplina = NULL;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_disciplina, it_CpDisc->second, int )
      {
		 Disciplina *disciplina = problemData->refDisciplinas[ *it_disciplina ];

		 // Pula a restrição caso a disciplina seja de conjuntoAluno anterior

 		 map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
		 itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

		 if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
		 {
			if ( itMapDiscCjt->second != cjtAlunosId &&
				NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
				continue;
		 }
		 else
		 {
			continue;
		 }

		 double bigM = disciplina->getNSalasAptas();

         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
			c.reset();
			c.setType( ConstraintPre::C_PRE_ATIVA_Z );
			c.setCampus( cp );
			c.setDisciplina( disciplina );
			c.setTurma( turma );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			nnz = disciplina->getNSalasAptas() + 1;

			OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

            VariablePre v;
            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );
            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );				// cp

			it_v = vHashPre.find( v );
			if ( it_v != vHashPre.end() )
			{
				row.insert( it_v->second, -bigM );
			}
			
			ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
			{
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
				{							
					if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
						itCjtSala->disciplinas_associadas.end() )
					{
						continue;
					}

                    VariablePre v;
                    v.reset();
                    v.setType( VariablePre::V_PRE_OFERECIMENTO );
                    v.setTurma( turma );            // i
                    v.setDisciplina( disciplina );  // d
                    v.setUnidade( *itUnidade );     // u
                    v.setSubCjtSala( *itCjtSala );  // tps

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
            
         }
      }
   }

   return restricoes;

}

// Restricao 1.11
int PreModelo::cria_preRestricao_evita_turma_disc_camp_d( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;

   if ( problemData->campi.size() <= 1 ) 
	   return restricoes;

   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
		   problemData->cp_discs[ campusId ].end() )
	  {
		  continue;
 	  }

		// Pula a restrição caso a disciplina seja de conjuntoAluno anterior

		map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
		itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

		if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
		{
			if ( itMapDiscCjt->second != cjtAlunosId &&
				NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
				continue;
		}
		else
		{
			continue;
		}

      for ( int i = 1; i <= disciplina->getNumTurmas(); ++i )
      {
         c.reset();
         c.setType( ConstraintPre::C_PRE_EVITA_TURMA_DISC_CAMP_D );

         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHashPre.find( c ) != cHashPre.end() )
         {
            continue;
         }

         nnz = problemData->totalSalas;
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
         {
			if ( it_campus->getId() != campusId )
			{
				continue;
			}

            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );

            v.setTurma( i );
            v.setDisciplina( disciplina );
            v.setCampus( *it_campus );

            it_v = vHashPre.find( v );
            if( it_v != vHashPre.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() != 0 )
         {
            cHashPre[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}   

// Restricao 1.12
int PreModelo::cria_preRestricao_limita_abertura_turmas_peloHash2( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   int minalunos;
   if ( problemData->parametros->min_alunos_abertura_turmas )
   {
		minalunos = problemData->parametros->min_alunos_abertura_turmas_value;
		if ( minalunos <= 0 ) minalunos = 1;
   }
   else
   {
	   minalunos = 1;
   }

   Campus *campus = problemData->refCampus[campusId];

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_ABERTURA )
		{
			coef = minalunos;
			if ( v.getCampus()->getId() != campusId ) continue;
		}
		//else if( v.getType() == VariablePre::V_PRE_NRO_ALUNOS_OFT )
      else if( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			coef = -1.0;		
			if ( v.getOferta()->getCampusId() != campusId ) continue;
		}
		else if( v.getType() == VariablePre::V_PRE_FORMANDOS_NA_TURMA )
		{
			coef = - minalunos;
			if ( v.getCampus()->getId() != campusId ) continue;
		}
		else 
		{
			continue;
		}
		
		c.reset();
        c.setType( ConstraintPre::C_PRE_LIMITA_ABERTURA_TURMAS );

		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
		c.setCampus( campus );

		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;
}


// Restricao 1.12
int PreModelo::cria_preRestricao_limita_abertura_turmas_peloHash( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   int minalunos;
   if ( problemData->parametros->min_alunos_abertura_turmas )
   {
		minalunos = problemData->parametros->min_alunos_abertura_turmas_value;
		if ( minalunos <= 0 ) minalunos = 1;
   }
   else
   {
	   minalunos = 1;
   }

   Campus *campus = problemData->refCampus[campusId];

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_ABERTURA )
		{
			coef = minalunos;
			if ( v.getCampus()->getId() != campusId ) continue;
		}
		else if( v.getType() == VariablePre::V_PRE_NRO_ALUNOS )
		{
			coef = -1.0;		
			if ( v.getCampus()->getId() != campusId ) continue;
		}
		else if( v.getType() == VariablePre::V_PRE_FORMANDOS_NA_TURMA )
		{
			coef = - minalunos;
			if ( v.getCampus()->getId() != campusId ) continue;
		}
		else 
		{
			continue;
		}
		
		c.reset();
        c.setType( ConstraintPre::C_PRE_LIMITA_ABERTURA_TURMAS );

		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
		c.setCampus( campus );

		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;

/* // Com variavel a

   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   int minalunos;
   if ( problemData->parametros->min_alunos_abertura_turmas )
   {
		minalunos = problemData->parametros->min_alunos_abertura_turmas_value;
		if ( minalunos <= 0 ) minalunos = 1;
   }
   else
   {
	   minalunos = 1;
   }

   Campus *campus = problemData->refCampus[campusId];

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_ABERTURA )
		{
			coef = minalunos;
			if ( v.getCampus()->getId() != campusId ) continue;
		}
		else if( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			coef = -1.0;		
			if ( v.getUnidade()->getIdCampus() != campusId ) continue;
		}
		else if( v.getType() == VariablePre::V_PRE_FORMANDOS_NA_TURMA )
		{
			coef = - minalunos;		
			if ( v.getCampus()->getId() != campusId ) continue;
		}
		else 
		{
			continue;
		}
		
		c.reset();
        c.setType( ConstraintPre::C_PRE_LIMITA_ABERTURA_TURMAS );

		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
		c.setCampus( campus );

		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;
   */

}

// Restricao 1.13
int PreModelo::cria_preRestricao_abre_turmas_em_sequencia( int campusId, int cjtAlunosId, int prioridade, int r )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   Campus *campus = problemData->refCampus[ campusId ];
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
		   problemData->cp_discs[ campusId ].end() )
	  {
		  continue;
 	  }
	  	  
	  if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
	  {
		  continue;
	  }

	  map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
		  itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

	  if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
	  {
		  if ( itMapDiscCjt->second	!= cjtAlunosId )
			  //&& NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
			  continue;
	  }
	  else
	  {
		  if ( problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
		  {
			  std::cout<<"\nAtencao em cria_preRestricao_abre_turmas_em_sequencia: disciplina "
						<<disciplina->getId() <<" nao pertence a nenhum conjunto\n";
		  }
	  }

      if ( disciplina->getNumTurmas() > 1 )
      {
         for ( int turma = 1; turma <= ( disciplina->getNumTurmas() - 1 ); turma++ )
         {
            c.reset();
            c.setType( ConstraintPre::C_PRE_ABRE_TURMAS_EM_SEQUENCIA );

            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() ); 
            if ( cHashPre.find( c ) != cHashPre.end() )
            {
               continue;
            }

			nnz = 2*disciplina->getNSalasAptas();
            OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

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
                        v.reset();
                        v.setType( VariablePre::V_PRE_OFERECIMENTO );

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );

                        it_v = vHashPre.find( v );
                        if ( it_v != vHashPre.end() )
                        {
                           row.insert( it_v->second, 1.0 );
                        }

                        v.reset();
                        v.setType( VariablePre::V_PRE_OFERECIMENTO );

                        int turmaSuc = turma + 1;
                        v.setTurma(turmaSuc);
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );

                        it_v = vHashPre.find( v );
                        if ( it_v != vHashPre.end() )
                        {
                           row.insert( it_v->second, -1.0 );
                        }
                  }
               }
            }

			if ( ( prioridade > 1 && FIXAR_P1 ) || r>1 )
			{
                v.reset();
                v.setType( VariablePre::V_PRE_FOLGA_ABRE_TURMA_SEQUENCIAL );

                v.setTurma( turma );
                v.setDisciplina( disciplina );
				v.setCampus( campus );

                it_v = vHashPre.find( v );
                if ( it_v != vHashPre.end() )
                {
                    row.insert( it_v->second, 1.0 );
                }				
			}

            if ( row.getnnz() != 0 )
            {
               cHashPre[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}


// Restrição 1.14
int PreModelo::cria_preRestricao_turma_mesma_disc_sala_dif( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;
				
		if ( cp->getId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					  Disciplina* disciplina = ( *itDisc );
					  					  
						// Pula a restrição caso a disciplina seja de conjuntoAluno anterior

						map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
						itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

	 					if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
	 					{
							if ( itMapDiscCjt->second != cjtAlunosId &&
								NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
								continue;
						}
						else
						{
							continue;
						}

					  if ( disciplina->getNumTurmas() > 1 )
					  {
							if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
							{
								continue;
							}

							c.reset();
							c.setType( ConstraintPre::C_PRE_TURMA_MESMA_DISC_SALA_DIF );
							c.setDisciplina( disciplina );
							c.setUnidade( *itUnidade );
							c.setSubCjtSala( *itCjtSala );
								  
							sprintf( name, "%s", c.toString().c_str() ); 
							if ( cHashPre.find( c ) != cHashPre.end() )
							{
								continue;
							}

							nnz = disciplina->getNumTurmas() + 1;

							OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
						  
							for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
							{
								v.reset();
								v.setType( VariablePre::V_PRE_OFERECIMENTO );
								v.setTurma( turma );
								v.setDisciplina( *itDisc );
								v.setUnidade( *itUnidade );
								v.setSubCjtSala( *itCjtSala );

								it_v = vHashPre.find( v );
								if ( it_v != vHashPre.end() )
								{
									row.insert( it_v->second, 1.0 );
								}
							}

							v.reset();
							v.setType( VariablePre::V_PRE_SLACK_DISC_SALA);
							v.setDisciplina( *itDisc );
							v.setUnidade( *itUnidade );
							v.setSubCjtSala( *itCjtSala );

							it_v = vHashPre.find( v );
							if ( it_v != vHashPre.end() )
							{
								row.insert( it_v->second, -1.0 );
							}

							if ( row.getnnz() != 0 )
							{
								cHashPre[ c ] = lp->getNumRows();

  								lp->addRow( row );
								restricoes++;
							}
					  }
				}
			}
		}
	}

	return restricoes;
}

// Restrição 1.15
int PreModelo::cria_preRestricao_limite_sup_creds_sala( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

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
				// --------------------------------------------------------
				c.reset();
				c.setType( ConstraintPre::C_PRE_LIM_SUP_CREDS_SALA );
				c.setCampus( *itCampus );
				c.setUnidade( *itUnidade );
				c.setSubCjtSala( *itCjtSala );

				sprintf( name, "%s", c.toString().c_str() ); 
				if ( cHashPre.find( c ) != cHashPre.end() )
				{
					continue;
				}

				nnz = 100 + 1;

				OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );
						  
				v.reset();
				v.setType( VariablePre::V_PRE_LIM_SUP_CREDS_SALA);
				v.setCampus( *itCampus );

				it_v = vHashPre.find( v );
				if ( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -1.0 );
				}

				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					  Disciplina* disciplina = ( *itDisc );
					  
					  for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					  {		
						#ifndef SALA_UNICA_POR_TURMA   						  
						  v.reset();
						  v.setType( VariablePre::V_PRE_CREDITOS);
						  v.setUnidade( *itUnidade );
						  v.setSubCjtSala( *itCjtSala );
						  v.setDisciplina( disciplina );
						  v.setTurma( turma );

						  it_v = vHashPre.find( v );
						  if ( it_v != vHashPre.end() )
						  {
							  row.insert( it_v->second, 1.0 );
						  }						  
						#else
						  v.reset();
						  v.setType( VariablePre::V_PRE_OFERECIMENTO );
						  v.setUnidade( *itUnidade );
						  v.setSubCjtSala( *itCjtSala );
						  v.setDisciplina( disciplina );
						  v.setTurma( turma );

						  it_v = vHashPre.find( v );
						  if ( it_v != vHashPre.end() )
						  {
							  row.insert( it_v->second, disciplina->getTotalCreditos() );
						  }
						#endif
					  }
				}

				if ( row.getnnz() != 0 )
				{
					cHashPre[ c ] = lp->getNumRows();

  					lp->addRow( row );
					restricoes++;
				}
				// --------------------------------------------------------
			}
		}
	}

	return restricoes;
}

// Restrição 1.26
int PreModelo::cria_preRestricao_soma_cred_sala( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

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
				// --------------------------------------------------------
				c.reset();
            c.setType( ConstraintPre::C_PRE_SOMA_CRED_SALA );
				c.setCampus( *itCampus );
				c.setUnidade( *itUnidade );
				c.setSubCjtSala( *itCjtSala );

				sprintf( name, "%s", c.toString().c_str() ); 
				if ( cHashPre.find( c ) != cHashPre.end() )
				{
					continue;
				}

				nnz = 100 + 1;

				OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );
						  
				v.reset();
            v.setType( VariablePre::V_PRE_CRED_SALA_F1);
				v.setCampus( *itCampus );
            v.setUnidade(*itUnidade);
            v.setSubCjtSala(*itCjtSala);

				it_v = vHashPre.find( v );
				if ( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -1.0 );
				}

            v.reset();
            v.setType( VariablePre::V_PRE_CRED_SALA_F2);
				v.setCampus( *itCampus );
            v.setUnidade(*itUnidade);
            v.setSubCjtSala(*itCjtSala);

				it_v = vHashPre.find( v );
				if ( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -1.0 );
				}

            v.reset();
            v.setType( VariablePre::V_PRE_CRED_SALA_F3);
				v.setCampus( *itCampus );
            v.setUnidade(*itUnidade);
            v.setSubCjtSala(*itCjtSala);

				it_v = vHashPre.find( v );
				if ( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -1.0 );
				}

            v.reset();
            v.setType( VariablePre::V_PRE_CRED_SALA_F4);
				v.setCampus( *itCampus );
            v.setUnidade(*itUnidade);
            v.setSubCjtSala(*itCjtSala);

				it_v = vHashPre.find( v );
				if ( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -1.0 );
				}

				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					  Disciplina* disciplina = ( *itDisc );
					  					  
					  for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					  {
						 #ifndef SALA_UNICA_POR_TURMA
						  v.reset();
						  v.setType( VariablePre::V_PRE_CREDITOS);
						  v.setUnidade( *itUnidade );
						  v.setSubCjtSala( *itCjtSala );
						  v.setDisciplina( disciplina );
						  v.setTurma( turma );

						  it_v = vHashPre.find( v );
						  if ( it_v != vHashPre.end() )
						  {
							  row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() );
						  }							
						 #else
						  v.reset();
						  v.setType( VariablePre::V_PRE_OFERECIMENTO);
						  v.setUnidade( *itUnidade );
						  v.setSubCjtSala( *itCjtSala );
						  v.setDisciplina( disciplina );
						  v.setTurma( turma );

						  it_v = vHashPre.find( v );
						  if ( it_v != vHashPre.end() )
						  {
							  row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos() );
						  }							
						 #endif

					  }
				}

				if ( row.getnnz() != 0 )
				{
					cHashPre[ c ] = lp->getNumRows();

  					lp->addRow( row );
					restricoes++;
				}
				// --------------------------------------------------------
			}
		}
	}

	return restricoes;
}

// Restrição 1.26
int PreModelo::cria_preRestricao_soma_cred_sala_por_turno( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

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
				std::map< int, GGroup<Calendario*,LessPtr<Calendario>> >::iterator
					itTurno = problemData->mapTurnoSemanasLetivas.begin();
				for ( ; itTurno != problemData->mapTurnoSemanasLetivas.end(); itTurno++ )
				{
					int turno = itTurno->first;

					// --------------------------------------------------------
					c.reset();
					c.setType( ConstraintPre::C_PRE_SOMA_CRED_SALA );
					c.setCampus( *itCampus );
					c.setUnidade( *itUnidade );
					c.setSubCjtSala( *itCjtSala );
					c.setTurno( turno );

					sprintf( name, "%s", c.toString().c_str() ); 
					if ( cHashPre.find( c ) != cHashPre.end() )
					{
						continue;
					}

					nnz = 100 + 1;

					OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );
						  
					v.reset();
					v.setType( VariablePre::V_PRE_CRED_SALA_F1);
					v.setCampus( *itCampus );
					v.setUnidade(*itUnidade);
					v.setSubCjtSala(*itCjtSala);
					v.setTurno( turno );

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, -1.0 );
					}

					v.reset();
					v.setType( VariablePre::V_PRE_CRED_SALA_F2);
					v.setCampus( *itCampus );
					v.setUnidade(*itUnidade);
					v.setSubCjtSala(*itCjtSala);
					v.setTurno( turno );

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, -1.0 );
					}

					v.reset();
					v.setType( VariablePre::V_PRE_CRED_SALA_F3);
					v.setCampus( *itCampus );
					v.setUnidade(*itUnidade);
					v.setSubCjtSala(*itCjtSala);
					v.setTurno( turno );

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, -1.0 );
					}

					v.reset();
					v.setType( VariablePre::V_PRE_CRED_SALA_F4);
					v.setCampus( *itCampus );
					v.setUnidade(*itUnidade);
					v.setSubCjtSala(*itCjtSala);
					v.setTurno( turno );

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, -1.0 );
					}

					ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
					{
						  Disciplina* disciplina = ( *itDisc );
					  					  
						  for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
						  {
							 #ifndef SALA_UNICA_POR_TURMA
							  v.reset();
							  v.setType( VariablePre::V_PRE_CREDITOS);
							  v.setUnidade( *itUnidade );
							  v.setSubCjtSala( *itCjtSala );
							  v.setDisciplina( disciplina );
							  v.setTurma( turma );

							  it_v = vHashPre.find( v );
							  if ( it_v != vHashPre.end() )
							  {
								  row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() );
							  }							
							 #else
							  v.reset();
							  v.setType( VariablePre::V_PRE_OFERECIMENTO_TURNO);
							  v.setUnidade( *itUnidade );
							  v.setSubCjtSala( *itCjtSala );
							  v.setDisciplina( disciplina );
							  v.setTurma( turma );
							  v.setTurno( turno );

							  it_v = vHashPre.find( v );
							  if ( it_v != vHashPre.end() )
							  {
								  row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos() );
							  }							
							 #endif

						  }
					}

					if ( row.getnnz() != 0 )
					{
						cHashPre[ c ] = lp->getNumRows();

  						lp->addRow( row );
						restricoes++;
					}
					// --------------------------------------------------------
				}
			}
		}
	}

	return restricoes;
}



// Restricao 1.18
/*
    Metodo somente utilizado para o modelo Tatico_Aluno

	Define qual aluno foi atendido em cada atendimento feito.
*/
int PreModelo::cria_preRestricao_atendimento_aluno_peloHash( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
		
		Oferta *oferta;

		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{
			coef = - 1.0;
			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			AlunoDemanda *alDem = NULL;
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
			  alDem = v.getAluno()->getAlunoDemandaEquiv( v.getDisciplina() );
			else
			  alDem = v.getAluno()->getAlunoDemanda( v.getDisciplina()->getId() );
			#else	
			  AlunoDemanda *alDem = v.getAluno()->getAlunoDemanda( v.getDisciplina()->getId() );
			#endif
			if ( alDem == NULL ){
				std::cout<<"\nErro: AlunoDemanda nao encontrado. Disc " 
					<< v.getDisciplina()->getId() << ", Aluno" << v.getAluno()->getAlunoId();
				continue;
			}

			oferta = alDem->getOferta();
		}
		else if( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			coef = 1.0;
			oferta = v.getOferta();			
		}
		else
		{
			continue;
		}

		if ( oferta->getCampusId() != campusId ) continue;

		c.reset();
        c.setType( ConstraintPre::C_ATENDIMENTO_ALUNO );
		c.setOferta( oferta );
		c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
        
		cit = cHashPre.find(c);

		if ( cit == cHashPre.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::EQUAL, 0.0, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;
}


// Restricao 1.19
/*
    Metodo somente utilizado para o modelo Tatico_Aluno
   
	Garante que cada aluno esteja em apenas 1 turma de uma disciplina
*/
int PreModelo::cria_preRestricao_aluno_unica_turma_disc( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;      
   char name[ 200 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;


		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;							  

			if ( itAlDemanda->getOferta()->getCampusId() != campusId )
			{
				continue;
			}

			// Pula a restrição caso a disciplina seja de conjuntoAluno anterior 
			map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
			itMapDiscCjt = problemData->cjtDisciplinas.find( disciplina );

			if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
	 		{
				if ( itMapDiscCjt->second != cjtAlunosId &&
					NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
					continue;
			}
			else
			{
				continue;
	 		}

			c.reset();
			c.setType( ConstraintPre::C_ALUNO_UNICA_TURMA_DISC );
			c.setAluno( aluno );
			c.setDisciplina( disciplina );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			nnz = disciplina->getNumTurmas();

			OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			{
				for ( int turno=1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
				{
					VariablePre v;
					v.reset();
					v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setCampus( campus );
					v.setTurno( turno );

					it_v = vHashPre.find( v );
					if( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;
}


int PreModelo::cria_preRestricao_aluno_discPraticaTeorica_oferta( int campusId, int cjtAlunosId )
{
    int restricoes = 0;		
    char name[ 100 ];
    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		// no_{i,d,oft}
		//if( vit->first.getType() == VariablePre::V_PRE_NRO_ALUNOS_OFT )
      if( vit->first.getType() == VariablePre::V_PRE_ALUNOS )
		{			
			VariablePre v = vit->first;
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

			Oferta* oferta = v.getOferta();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_DISC_PRATICA_TEORICA_OFERTA );
			c.setDisciplina( disciplinaPratica );
			c.setOferta( oferta );
			
			double coef = 0.0;
			if ( v.getDisciplina()->getId() < 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz = disciplinaPratica->getNumTurmas() * 2;

				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}
	}
	
	return restricoes;
}


int PreModelo::cria_preRestricao_aluno_discPraticaTeorica_MxN( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
    
	if ( !problemData->parametros->discPratTeorMxN )
	{
		return restricoes;
	}
  
   char name[ 200 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *discPratica = itAlDemanda->demanda->disciplina;
			
			if ( itAlDemanda->getOferta()->getCampusId() != campusId )
			{
				continue;
			}						  

			// Pula a restrição caso a disciplina seja de conjuntoAluno anterior
 
			map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
			itMapDiscCjt = problemData->cjtDisciplinas.find( discPratica );

			if ( itMapDiscCjt != problemData->cjtDisciplinas.end() )
	 		{
				if ( itMapDiscCjt->second != cjtAlunosId &&
					NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
					continue;
			}
			else
			{
				continue;
	 		}

			// Pula disciplina teorica
			if ( discPratica->getId() > 0 )
				continue;

			Disciplina *discTeorica = problemData->refDisciplinas[ - discPratica->getId() ];

			if ( discTeorica == NULL )
			{
				std::cout<<"\nErro em cria_preRestricao_aluno_discPraticaTeorica: disciplina teorica nao encontrada.\n";
				continue;
			}
			
			c.reset();
			c.setType( ConstraintPre::C_ALUNO_DISC_PRATICA_TEORICA );
			c.setAluno( aluno );
			c.setDisciplina( discPratica );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			nnz = discPratica->getNumTurmas() + discTeorica->getNumTurmas() + 1;

			OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

			for ( int turma = 1; turma <= discPratica->getNumTurmas(); turma++ )
			{
				for ( int turno=1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
				{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( discPratica );
				v.setTurma( turma );
				v.setCampus( campus );
				v.setTurno( turno );

				it_v = vHashPre.find( v );
				if( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, 1.0 );
				}
				}
			}

			for ( int turma = 1; turma <= discTeorica->getNumTurmas(); turma++ )
			{
				for ( int turno=1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
				{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( discTeorica );
				v.setTurma( turma );
				v.setCampus( campus );
				v.setTurno( turno );

				it_v = vHashPre.find( v );
				if( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -1.0 );
				}
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;

}

int PreModelo::cria_preRestricao_aluno_discPraticaTeorica_hash_MxN( int campusId )
{
	int numRest=0;

	if ( !problemData->parametros->discPratTeorMxN )
	{
		return numRest;
	}

    char name[ 100 ];

	VariablePre v;
	ConstraintPre c;
	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		v = vit->first;
	
		if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
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
			c.setType( ConstraintPre::C_ALUNO_DISC_PRATICA_TEORICA );
			c.setDisciplina( disciplinaPratica );
			c.setAluno( v.getAluno() );

			double coef = 0.0;
			if ( v.getDisciplina()->getId() < 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz = 2;

				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				numRest++;
			}
		}
	}

	return numRest;
}

int PreModelo::cria_preRestricao_aluno_discPraticaTeorica_1x1( int campusId )
{
	int numRest=0;

	if ( ! problemData->parametros->discPratTeor1x1 )
	{
		return numRest;
	}

    char name[ 100 ];

	VariablePre v;
	ConstraintPre c;
	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		v = vit->first;
	
		if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{						
			Disciplina *disciplinaPratica = NULL;
						
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{			
				// Só para disciplinas praticas/teoricas
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
			c.setType( ConstraintPre::C_ALUNO_DISC_PRATICA_TEORICA_1x1 );
			c.setDisciplina( disciplinaPratica );
			c.setTurma( v.getTurma() );
			c.setAluno( v.getAluno() );

			double coef = 0.0;
			if ( v.getDisciplina()->getId() < 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz = 2;

				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				numRest++;
			}
		}
	}

	return numRest;
}

int PreModelo::cria_preRestricao_aluno_discPraticaTeorica_1xN( int campusId )
{
	int numRest=0;

	// Se der certo isso, dá para generalizar essa restrição para as outras relacoes PT.

	if ( ! problemData->parametros->discPratTeor1xN )
	{
		return numRest;
	}

    char name[ 100 ];

	VariablePre v;
	ConstraintPre c;
	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		v = vit->first;
	
		if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{						
			Disciplina *disciplinaPratica = NULL;
			Disciplina *disciplinaTeorica = NULL;
			int turmaTeorica = -1;

			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{
				// Só para disciplinas praticas/teoricas
				std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
				if ( itMapDisc == problemData->refDisciplinas.end() )
					continue;

				disciplinaPratica = itMapDisc->second;				
				disciplinaTeorica = v.getDisciplina();
				turmaTeorica = v.getTurma();
			}
			else
			{
				disciplinaPratica = v.getDisciplina();
				disciplinaTeorica = problemData->retornaDisciplina( -discId );

				GGroup<int> tt = disciplinaPratica->getTurmasAssociadas( v.getTurma() );
				if ( tt.size() > 1 )
					std::cout<<"\nErro: relacao 1xN e disc pratica com mais de uma turma teorica associada.";

				turmaTeorica = *(tt.begin());					
			}
		
			c.reset();
			c.setType( ConstraintPre::C_ALUNO_DISC_PRATICA_TEORICA_1xN );
			c.setDisciplina( disciplinaTeorica );	// disciplina TEORICA que tenha pratica associada
			c.setTurma( turmaTeorica );				// turma TEORICA
			c.setAluno( v.getAluno() );				// aluno

			double coef = 0.0;
			if ( v.getDisciplina()->getId() > 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int totalTurmas = ( 1 + disciplinaTeorica->getTurmasAssociadas(turmaTeorica).size() );				
				int nnz = totalTurmas * problemData->getNroTotalDeFasesDoDia();

				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				numRest++;
			}
		}
	}

	return numRest;
}


int PreModelo::cria_preRestricao_aluno_discPraticaTeorica_1xN_antiga( int campusId )
{
	int numRest=0;

	// Por enquanto só a Unit usa essa relação 1xN
	if ( ! problemData->parametros->discPratTeor1xN_antigo )
	{
		return numRest;
	}

    char name[ 100 ];

	VariablePre v;
	ConstraintPre c;
	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;

	map< int /*turma*/, map< Disciplina*, map<int, VariablePre>, LessPtr<Disciplina> > >
		mapTurmaDiscTeorAlunos;

	map< Disciplina*, map< Aluno*, map<Aluno*, int, LessPtr<Aluno>>, LessPtr<Aluno> >, LessPtr<Disciplina> >
		mapDiscTeorAluno1Aluno2VarSS;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		v = vit->first;

		if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ) // s_{i,d,a}
		{				
			int discId = v.getDisciplina()->getId();
			if ( discId < 0 ) continue;

			// Só para disciplinas praticas/teoricas
			std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
			if ( itMapDisc == problemData->refDisciplinas.end() )
				continue;
		
			int turmaTeor = v.getTurma();
			Disciplina *discTeor = v.getDisciplina();

			mapTurmaDiscTeorAlunos[turmaTeor][discTeor][vit->second] = v;
		}
		else if( v.getType() == VariablePre::V_PRE_ALUNOS_MESMA_TURMA_PRAT ) // ss_{a1,a2,dp}
		{				
			int discPratId = v.getDisciplina()->getId();
			
			Disciplina *discTeor = problemData->refDisciplinas[ -discPratId ];

			Aluno* aluno1 = v.getParAlunos().first;
			Aluno* aluno2 = v.getParAlunos().second;

			if ( aluno1->getAlunoId() < aluno2->getAlunoId() )
				mapDiscTeorAluno1Aluno2VarSS[discTeor][aluno1][aluno2] = vit->second;
			else
				mapDiscTeorAluno1Aluno2VarSS[discTeor][aluno2][aluno1] = vit->second;
		}
	}

	map< int /*turma*/, map< Disciplina*, map<int, VariablePre>, LessPtr<Disciplina> > >::iterator
		itMapTurma = mapTurmaDiscTeorAlunos.begin();
	for ( ; itMapTurma != mapTurmaDiscTeorAlunos.end(); itMapTurma++ )
	{
		int turmaTeor = itMapTurma->first;

		map< Disciplina*, map<int, VariablePre>, LessPtr<Disciplina> >::iterator
			itMapDisc = itMapTurma->second.begin();
		for ( ; itMapDisc != itMapTurma->second.end(); itMapDisc++ )
		{
			Disciplina *discTeor = itMapDisc->first;

			map<int, VariablePre> variables = itMapDisc->second;

			map<int, VariablePre>::iterator itMapVar1 = variables.begin();
			for( ; itMapVar1 != variables.end(); itMapVar1++ )
			{
				VariablePre v1 = itMapVar1->second; // s_{i,d,a1}
				int indx1 = itMapVar1->first;

				map<int, VariablePre>::iterator itMapVar2 = variables.begin();
				for( ; itMapVar2 != variables.end(); itMapVar2++ )
				{
					VariablePre v2 = itMapVar2->second; // s_{i,d,a2}
					int indx2 = itMapVar2->first;

					if ( v1.getAluno()->getAlunoId() >= v2.getAluno()->getAlunoId() ) continue;

					std::pair<Aluno*, Aluno*> parAlunos( v1.getAluno(), v2.getAluno() );

					int indx3 = mapDiscTeorAluno1Aluno2VarSS[discTeor][v1.getAluno()][v2.getAluno()]; // ss_{dp,a1,a2}

					c.reset();
					c.setType( ConstraintPre::C_ALUNO_DISC_PRATICA_TEORICA_1xN );
					c.setDisciplina( discTeor );
					c.setTurma( turmaTeor );
					c.setParAlunos( parAlunos );

					cit = cHashPre.find(c);
					if(cit == cHashPre.end())
					{
						int nnz = 3;

						sprintf( name, "%s", c.toString().c_str() ); 
						OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

						row.insert(indx1, -1.0);
						row.insert(indx2, -1.0);
						row.insert(indx3, 2.0);

						cHashPre[ c ] = lp->getNumRows();

						lp->addRow( row );
						numRest++;
					}
				}
			}
		}
	}

	return numRest;
}


int PreModelo::cria_preRestricao_alunos_mesma_turma_pratica( int campusId )
{
	int numRest=0;

	// Por enquanto só a Unit usa a relação 1xN
	if ( ! problemData->parametros->discPratTeor1xN_antigo )
	{
		return numRest;
	}

    char name[ 100 ];

	VariablePre v;
	ConstraintPre c;
	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;

	map< int /*turma*/, map< Disciplina*, map<int, VariablePre>, LessPtr<Disciplina> > >
		mapTurmaDiscPratAlunos;

	map< Disciplina*, map< Aluno*, map<Aluno*, int, LessPtr<Aluno>>, LessPtr<Aluno> >, LessPtr<Disciplina> >
		mapDiscPratAluno1Aluno2VarSS;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		v = vit->first;

		if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ) // s_{i,d,a}
		{				
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 ) continue;
					
			int turmaPrat = v.getTurma();
			Disciplina *discPrat = v.getDisciplina();

			mapTurmaDiscPratAlunos[turmaPrat][discPrat][vit->second] = v;
		}
		else if( v.getType() == VariablePre::V_PRE_ALUNOS_MESMA_TURMA_PRAT ) // ss_{a1,a2,dp}
		{				
			Disciplina *discPrat = v.getDisciplina();			
			Aluno* aluno1 = v.getParAlunos().first;
			Aluno* aluno2 = v.getParAlunos().second;

			if ( aluno1->getAlunoId() < aluno2->getAlunoId() )
				mapDiscPratAluno1Aluno2VarSS[discPrat][aluno1][aluno2] = vit->second;
			else
				mapDiscPratAluno1Aluno2VarSS[discPrat][aluno2][aluno1] = vit->second;
		}
	}

	map< int /*turma*/, map< Disciplina*, map<int, VariablePre>, LessPtr<Disciplina> > >::iterator
		itMapTurma = mapTurmaDiscPratAlunos.begin();
	for ( ; itMapTurma != mapTurmaDiscPratAlunos.end(); itMapTurma++ )
	{
		int turmaPrat = itMapTurma->first;

		map< Disciplina*, map<int, VariablePre>, LessPtr<Disciplina> >::iterator
			itMapDisc = itMapTurma->second.begin();
		for ( ; itMapDisc != itMapTurma->second.end(); itMapDisc++ )
		{
			Disciplina *discPrat = itMapDisc->first;

			map<int, VariablePre> variables = itMapDisc->second;

			map<int, VariablePre>::iterator itMapVar1 = variables.begin();
			for( ; itMapVar1 != variables.end(); itMapVar1++ )
			{
				VariablePre v1 = itMapVar1->second; // s_{i,d,a1}
				int indx1 = itMapVar1->first;

				map<int, VariablePre>::iterator itMapVar2 = variables.begin();
				for( ; itMapVar2 != variables.end(); itMapVar2++ )
				{
					VariablePre v2 = itMapVar2->second; // s_{i,d,a2}
					int indx2 = itMapVar2->first;

					if ( v1.getAluno()->getAlunoId() >= v2.getAluno()->getAlunoId() ) continue;

					std::pair<Aluno*, Aluno*> parAlunos( v1.getAluno(), v2.getAluno() );

					int indx3 = mapDiscPratAluno1Aluno2VarSS[discPrat][v1.getAluno()][v2.getAluno()]; // ss_{dp,a1,a2}

					c.reset();
					c.setType( ConstraintPre::C_ALUNOS_MESMA_TURMA_PRAT );
					c.setDisciplina( discPrat );
					c.setTurma( turmaPrat );
					c.setParAlunos( parAlunos );

					cit = cHashPre.find(c);
					if(cit == cHashPre.end())
					{
						int nnz = 3;

						sprintf( name, "%s", c.toString().c_str() ); 
						OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

						row.insert(indx1, 1.0);
						row.insert(indx2, 1.0);
						row.insert(indx3, -1.0);

						cHashPre[ c ] = lp->getNumRows();

						lp->addRow( row );
						numRest++;
					}
				}
			}
		}
	}

	return numRest;
}

int PreModelo::cria_preRestricao_prioridadesDemanda( int campusId, int prior, int cjtAlunosId  )
{
    int restricoes = 0;

    if ( prior < 2 )
	   return restricoes;

    char name[ 200 ];
    int nnz;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

    GGroup< Aluno *, LessPtr< Aluno > > alunos;
	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator 
		itMapAlunoCjt = problemData->cjtAlunos.find( cjtAlunosId );	
	if ( itMapAlunoCjt != problemData->cjtAlunos.end() )
	{
		alunos = itMapAlunoCjt->second;
	}

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( ! aluno->hasCampusId( campusId ) )
		{
			continue;
		}

		if ( NAO_CRIAR_RESTRICOES_CJT_ANTERIORES )
		{
			if ( alunos.find( aluno ) == alunos.end() )
				continue;
		}

		c.reset();
		c.setType( ConstraintPre::C_ALUNO_PRIORIDADES_DEMANDA );
		c.setAluno( aluno );
		c.setCampus( campus );

		sprintf( name, "%s", c.toString().c_str() ); 

		if ( cHashPre.find( c ) != cHashPre.end() )
		{
			continue;
		}

		nnz = aluno->demandas.size()*3 + 2;
		
		double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( prior-1, aluno->getAlunoId() );		
		double cargaHorariaP2 = problemData->cargaHorariaAtualRequeridaPorPrioridade( prior, aluno );

		double rhs = cargaHorariaNaoAtendida;
		if (rhs>cargaHorariaP2)
			rhs=cargaHorariaP2;

		OPT_ROW row( nnz, OPT_ROW::EQUAL , rhs, name );
		
		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			if ( itAlDemanda->getPrioridade() != prior )
				continue;

			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			{
				for ( int turno=1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
				{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( campus );
				v.setTurno( turno );

				it_v = vHashPre.find( v );
				if( it_v != vHashPre.end() )
				{
					double tempo = disciplina->getTotalCreditos() * disciplina->getTempoCredSemanaLetiva();

					row.insert( it_v->second, tempo );
				}
				}
			}
		}

		VariablePre v;
		v.reset();
		v.setType( VariablePre::V_PRE_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashPre.find( v );
		if( it_v != vHashPre.end() )
		{
			row.insert( it_v->second, -1.0 );
		}

		v.reset();
		v.setType( VariablePre::V_PRE_SLACK_PRIOR_SUP );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashPre.find( v );
		if( it_v != vHashPre.end() )
		{
			row.insert( it_v->second, 1.0 );
		}


		if ( row.getnnz() >= 3 )
		{
			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
	}

	return restricoes;

}

int PreModelo::cria_preRestricao_limite_cred_aluno(int campusId, int cjtAlunosId, int prior)
{
   int restricoes = 0;

    char name[ 200 ];
    int nnz;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;   

	Campus *cp = problemData->refCampus[campusId];

   std::map<Aluno*,std::list<VariablePreHash::iterator> > mapAlunoVarS;

   for (it_v = vHashPre.begin(); it_v != vHashPre.end(); it_v++)
   {
      v = it_v->first;

      if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
      {
         mapAlunoVarS[v.getAluno()].push_back(it_v);
      }
   }

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();
	
	// Para cada conjunto de alunos cp com id menor ou igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;

		if ( grupoAlunosId > cjtAlunosId )
		{
			break;
		}

		// Para cada aluno do conjunto
		GGroup< Aluno *, LessPtr< Aluno > > cjtAlunos = problemData->cjtAlunos[ grupoAlunosId ];	
		ITERA_GGROUP_LESSPTR( itAluno, cjtAlunos, Aluno )
		{
			Aluno *aluno = *itAluno;

			if ( ! aluno->hasCampusId( campusId ) )
			{
				continue;
			}

			c.reset();
			c.setType( ConstraintPre::C_PRE_MAX_CREDS_ALUNO_SEMANA );
			c.setAluno( aluno );
			c.setCampus( cp );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			int maxTempoSemana = 0;

			OPT_ROW row( 100, OPT_ROW::LESS , maxTempoSemana, (char*)c.toString().c_str() );

			std::list<VariablePreHash::iterator> listVars = mapAlunoVarS[aluno];
			for (std::list<VariablePreHash::iterator>::iterator itVars = listVars.begin();
				itVars != listVars.end();
				itVars++)
			{
				v = (*itVars)->first;
				int col = (*itVars)->second;

				Disciplina *disciplina = v.getDisciplina();
				
				#ifdef EQUIVALENCIA_DESDE_O_INICIO
				AlunoDemanda* alDem=NULL;
				if ( problemData->parametros->considerar_equivalencia_por_aluno )
					alDem = aluno->getAlunoDemandaEquiv( disciplina );	
				else
					alDem = aluno->getAlunoDemanda( disciplina->getId() );
				#else
					AlunoDemanda* alDem = aluno->getAlunoDemanda( disciplina->getId() );
				#endif	
									
				if ( alDem==NULL ) std::cout<<"Erro! AlDem nao encontrado em cria_preRestricao_limite_cred_aluno";
				
				Calendario *calendario = alDem->getOferta()->curriculo->calendario;

				int ttSem = disciplina->getTempoTotalSemana( calendario );

				if ( ttSem > maxTempoSemana )
					maxTempoSemana = ttSem;
									
				double coef = disciplina->getTotalCreditos();
				coef *= disciplina->getTempoCredSemanaLetiva();

				row.insert(col,coef);
			}

			row.setRhs(maxTempoSemana);

			if ( row.getnnz() >= 1 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
       }
   }

	return restricoes;
}


// Usada agora para p1 também. Tentativa de minimizar os conflitos
// entre turmas a serem enviadas ao tático
int PreModelo::cria_preRestricao_ativa_var_compart_turma( int campusId, int cjtAlunosId, int prior  )
{
    int restricoes = 0;
    char name[ 200 ];
    int nnz;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

    GGroup< Aluno *, LessPtr< Aluno > > alunos;
	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator 
		itMapAlunoCjt = problemData->cjtAlunos.find( cjtAlunosId );	
	if ( itMapAlunoCjt != problemData->cjtAlunos.end() )
	{
		alunos = itMapAlunoCjt->second;
	}

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( !aluno->hasCampusId( campusId ) )
		{
			continue;
		}
		
		ITERA_GGROUP_LESSPTR( it1AlDemanda, aluno->demandas, AlunoDemanda )
		{
			if ( it1AlDemanda->getPrioridade() > prior )
				continue;

			Disciplina *disciplina1 = it1AlDemanda->demanda->disciplina;

			for ( int turma1 = 1; turma1 <= disciplina1->getNumTurmas(); turma1++ )
			{
				ITERA_GGROUP_INIC_LESSPTR( it2AlDemanda, it1AlDemanda, aluno->demandas, AlunoDemanda )
				{
					if ( it2AlDemanda->getPrioridade() > prior )
						continue;

					Disciplina *disciplina2 = it2AlDemanda->demanda->disciplina;

					if ( disciplina1->getId() == disciplina2->getId() )
						continue;

					for ( int turma2 = 1; turma2 <= disciplina2->getNumTurmas(); turma2++ )
					{
						c.reset();
						c.setType( ConstraintPre::C_PRE_ATIVA_VAR_COMPART_TURMA );
						c.setAluno( aluno );
						c.setTurma1( turma1 );
						c.setTurma2( turma2 );
						c.setDisciplina1( disciplina1 );
						c.setDisciplina2( disciplina2 );
						c.setCampus( campus );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHashPre.find( c ) != cHashPre.end() )
						{
							continue;
						}

						nnz = 3;
		
						OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );
					
						for ( int turno=1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
						{
							VariablePre v;
							v.reset();
							v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
							v.setAluno( aluno );
							v.setDisciplina( disciplina1 );
							v.setTurma( turma1 );
							v.setCampus( campus );
							v.setTurno(turno);

							it_v = vHashPre.find( v );
							if( it_v != vHashPre.end() )
							{
								row.insert( it_v->second, 1.0 );
							}

							v.reset();
							v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
							v.setAluno( aluno );
							v.setDisciplina( disciplina2 );
							v.setTurma( turma2 );
							v.setCampus( campus );
							v.setTurno(turno);

							it_v = vHashPre.find( v );
							if( it_v != vHashPre.end() )
							{
								row.insert( it_v->second, 1.0 );
							}
						}
				
						v.reset();
						v.setType( VariablePre::V_PRE_TURMAS_COMPART );				
						v.setDisciplina1( disciplina1 );
						v.setTurma1( turma1 );
						v.setDisciplina2( disciplina2 );
						v.setTurma2( turma2 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, -1.0 );
						}
						else
						{	// Analoga
							v.reset();
							v.setType( VariablePre::V_PRE_TURMAS_COMPART );				
							v.setDisciplina2( disciplina1 );
							v.setTurma2( turma1 );
							v.setDisciplina1( disciplina2 );
							v.setTurma1( turma2 );

							it_v = vHashPre.find( v );
							if( it_v != vHashPre.end() )
							{
								row.insert( it_v->second, -1.0 );
							}						
						}
						
						if ( row.getnnz() == 3 )
						{
							cHashPre[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}
	}

	return restricoes;

}

int PreModelo::cria_preRestricao_distribuicao_aluno( int campusId, int cjtAlunosId  )
{
    int restricoes = 0;

	//map<Disciplina *, map<int, map<Disciplina *, map<int, set<Aluno*, LessPtr< Aluno > > > ,LessPtr< Disciplina > > >,LessPtr< Disciplina > > mapAlunos;
   
    char name[ 100 ];

    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();

	while(vit != vHashPre.end())
	{
		if(vit->first.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC)
		{
			VariablePre v = vit->first;

			Aluno *aluno = v.getAluno();
			Disciplina *disc = v.getDisciplina();
			int turma = v.getTurma();

			ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
			{
				Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

				if(*disciplina < *disc)
				{
					for ( int turma2 = 1; turma2 <= disciplina->getNumTurmas(); turma2++ )
					{
						c.reset();
						c.setType(ConstraintPre::C_PRE_DISTR_ALUNOS);
						c.setDisciplina(disciplina);
						c.setTurma(turma2);
						c.setDisciplina2(disc);
						c.setTurma2(turma);
						c.setAluno(aluno);

						cit = cHashPre.find(c);

						if(cit != cHashPre.end())
							lp->chgCoef(cit->second, vit->second, 1.0);
						else
						{
							sprintf( name, "%s", c.toString().c_str() ); 
							OPT_ROW row( 3 , OPT_ROW::LESS, 1.0 , name );

							row.insert(vit->second, 1.0);

							cHashPre[ c ] = lp->getNumRows();

							lp->addRow( row );
							restricoes++;

							//mapAlunos[disciplina][turma2][disc][turma].insert(aluno);
						}
					}
				}
				else if(*disc < *disciplina)
				{
					for ( int turma2 = 1; turma2 <= disciplina->getNumTurmas(); turma2++ )
					{
						c.reset();
						c.setType(ConstraintPre::C_PRE_DISTR_ALUNOS);
						c.setDisciplina(disc);
						c.setTurma(turma);
						c.setDisciplina2(disciplina);
						c.setTurma2(turma2);
						c.setAluno(aluno);

						cit = cHashPre.find(c);

						if(cit != cHashPre.end())
							lp->chgCoef(cit->second, vit->second, 1.0);
						else
						{
							sprintf( name, "%s", c.toString().c_str() ); 
							OPT_ROW row( 3 , OPT_ROW::LESS, 1.0 , name );

							row.insert(vit->second, 1.0);

							cHashPre[ c ] = lp->getNumRows();

							lp->addRow( row );
							restricoes++;

							//mapAlunos[disc][turma][disciplina][turma2].insert(aluno);
						}
					}
				}
			}
		}

		vit++;
	}

	cit = cHashPre.begin();

	while(cit != cHashPre.end())
	{
      if(cit->first.getType() == ConstraintPre::C_PRE_DISTR_ALUNOS)
		{
			ConstraintPre c = cit->first;

			Disciplina *disciplina = c.getDisciplina();
			int turma = c.getTurma();
			Disciplina *disciplina2 = c.getDisciplina2();
			int turma2 = c.getTurma2();

         VariablePre v2;
			
	      v2.reset();
		   v2.setType(VariablePre::V_PRE_FOLGA_DISTR_ALUNOS);
			v2.setDisciplina(disciplina);
		   v2.setTurma(turma);
			v2.setDisciplina2(disciplina2);
			v2.setTurma2(turma2);

         vit = vHashPre.find(v2);

         if(vit != vHashPre.end())
            lp->chgCoef(cit->second, vit->second, -1.0);
         else
         {
            v2.reset();
            v2.setType(VariablePre::V_PRE_FOLGA_DISTR_ALUNOS);
            v2.setDisciplina2(disciplina);
            v2.setTurma2(turma);
            v2.setDisciplina(disciplina2);
            v2.setTurma(turma2);

            vit = vHashPre.find(v2);

            if(vit != vHashPre.end())
               lp->chgCoef(cit->second, vit->second, -1.0);
         }
      }

		cit++;
	}

	return restricoes;
}

int PreModelo::cria_preRestricao_aluno_sala( int campusId, int grupoAlunosAtualId )
{
    int restricoes = 0;

    char name[ 200 ];
    int nnz;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;   

	Campus *cp = problemData->refCampus[campusId];

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();
	
	// Para cada conjunto de alunos com id menor ou igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;

		if ( grupoAlunosId > grupoAlunosAtualId )
		{
			break;
		}

		// Para cada aluno do conjunto
		GGroup< Aluno *, LessPtr< Aluno > > cjtAlunos = problemData->cjtAlunos[ grupoAlunosId ];	
		ITERA_GGROUP_LESSPTR( itAluno, cjtAlunos, Aluno )
		{
			Aluno *aluno = *itAluno;

			if ( ! aluno->hasCampusId(campusId) )
			{
				continue;
			}

			// Itera as salas
			ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
			{
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
				{
					// Itera as disciplinas
					ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
					{
						Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

						if ( (*itCjtSala)->disciplinas_associadas.find( disciplina ) ==  
							 (*itCjtSala)->disciplinas_associadas.end() )
						{
							continue;
						}

						for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
						{
							c.reset();
							c.setType( ConstraintPre::C_PRE_ALUNO_SALA );
							c.setAluno( aluno );
							c.setDisciplina( disciplina );
							c.setTurma( turma );
							c.setUnidade( *itUnidade );
							c.setSubCjtSala( *itCjtSala );

							sprintf( name, "%s", c.toString().c_str() ); 

							if ( cHashPre.find( c ) != cHashPre.end() )
							{
								std::cout<<"\nERRO: nao deveria ter repetido, como??\n";
								continue;
							}

							nnz = 3;

							OPT_ROW row( nnz, OPT_ROW::GREATER , -1.0 , name );

							v.reset();
							v.setType( VariablePre::V_PRE_ALUNO_SALA ); // as_{a,s}
							v.setAluno( aluno );
							v.setSubCjtSala( *itCjtSala );
							v.setUnidade( *itUnidade );
							it_v = vHashPre.find( v );
							if( it_v != vHashPre.end() )
							{
								row.insert( it_v->second, 1.0 );
							}
						
							for ( int turno=1; turno <= problemData->getNroTotalDeFasesDoDia(); turno++ )
							{
								VariablePre v;
								v.reset();
								v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ); // s_{i,d,a}
								v.setAluno( aluno );
								v.setDisciplina( disciplina );
								v.setTurma( turma );
								v.setCampus( cp );
								v.setTurno(  turno );

								it_v = vHashPre.find( v );
								if( it_v != vHashPre.end() )
								{
									row.insert( it_v->second, -1.0 );
								}

							}

							v.reset();
							v.setType( VariablePre::V_PRE_OFERECIMENTO ); // o_{i,d,u,s}
							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setSubCjtSala( *itCjtSala );
							v.setUnidade( *itUnidade );
								
							it_v = vHashPre.find( v );
							if( it_v != vHashPre.end() )
							{
								row.insert( it_v->second, -1.0 );
							}

							if ( row.getnnz() == 3 )
							{
								cHashPre[ c ] = lp->getNumRows();
								lp->addRow( row );
								restricoes++;
							}
						}
					}
				}
			}
		}
	}

	return restricoes;
}


int PreModelo::cria_preRestricao_formandos( int campusId, int cjtAlunosId, int prioridade, int r )
{
    int restricoes = 0;

	if ( !problemData->parametros->violar_min_alunos_turmas_formandos )
		return restricoes;		

	if ( prioridade==1 && r==1 ) // só considera formandos na segunda rodada
		return restricoes;
		
    char name[ 100 ];

    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		// s_{i,d,a}
		if( vit->first.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{			
			VariablePre v = vit->first;

			Aluno *aluno = v.getAluno();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus *campus = v.getCampus();

			if ( !aluno->ehFormando() )
				continue;

			// -----------------------------------------
			// Constraint 1
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_FORMANDOS1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
			
			c.reset();
			c.setType( ConstraintPre::C_PRE_FORMANDOS2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// f_{i,d,cp}
		else if( vit->first.getType() == VariablePre::V_PRE_FORMANDOS_NA_TURMA )
		{			
			VariablePre v = vit->first;

			Campus *campus = v.getCampus();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();

			// -----------------------------------------
			// Constraint 1

			double M = problemData->getNroDemandaPorFormandos( disciplina, campus, prioridade );

			c.reset();
			c.setType( ConstraintPre::C_PRE_FORMANDOS1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, M);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, M);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
						
			c.reset();
			c.setType( ConstraintPre::C_PRE_FORMANDOS2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, 1.0);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, 1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}		
		}
	}
	
	return restricoes;
}


int PreModelo::cria_preRestricao_turma_calendarios_com_intersecao( int campusId, int cjtAlunosId, int prioridade, int r )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;
   int min;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;
   ConstraintPreHash::iterator it_c;
   
		
   std::map< Disciplina*, std::map< Calendario*, GGroup<int>, LessPtr<Calendario> >, LessPtr<Disciplina> > mapDiscCalendTurnos;
   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
        Disciplina *disciplina = *it_disc;
		GGroup<Calendario*, LessPtr<Calendario>> calends = disciplina->getCalendarios();
        ITERA_GGROUP_LESSPTR( itSl1, calends, Calendario )
		{
			mapDiscCalendTurnos[disciplina][*itSl1] = problemData->getTurnosComunsViaveis( disciplina, *itSl1 );
		}
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      Campus *cp = *itCampus;
	  if ( itCampus->getId() != campusId )
	  {
		  continue;
	  }

     ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               Disciplina *disciplina = *it_disc;
		                
			   if ( prioridade==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
			   {
					if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
						continue;
			   }

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
                  GGroup<Calendario*, LessPtr<Calendario>> calends = disciplina->getCalendarios();
                  ITERA_GGROUP_LESSPTR( itSl1, calends, Calendario )
                  {					 
					  GGroup<int> turnosSl1 = mapDiscCalendTurnos[disciplina][*itSl1];

                     ITERA_GGROUP_INIC_LESSPTR( itSl2, itSl1, calends, Calendario )
                     {
						GGroup<int> turnosSl2 = mapDiscCalendTurnos[disciplina][*itSl2];

						#ifdef ALUNO_TURNOS_DA_DEMANDA
						bool foundCommon=false;
						ITERA_GGROUP_N_PT( itTurno, turnosSl1, int )
						{
							if ( turnosSl2.find( *itTurno ) != turnosSl2.end() ) foundCommon=true;
						}
						if( foundCommon ) continue;
						#endif

						#ifdef ALUNO_UNICO_CALENDARIO
                        GGroup<HorarioDia*, LessPtr<HorarioDia>> horariosDiaComuns = 
                           problemData->getHorariosDiaEmComum( *itSl1, *itSl2 );
                        int nCredsMax = problemData->getNroCredsMaxComum( disciplina, horariosDiaComuns );
                        if ( nCredsMax >= disciplina->getTotalCreditos() ) continue;
						#endif

                        c.reset();
                        c.setType( ConstraintPre::C_PRE_TURMA_CALENDARIOS_INTERSECAO );
                        c.setCampus( *itCampus );
                        c.setDisciplina( disciplina );
                        c.setTurma( turma );
                        c.setParCalendarios( *itSl1, *itSl2 );

                        sprintf( name, "%s", c.toString().c_str() );

                        it_c = cHashPre.find( c );

                        if ( it_c == cHashPre.end() )
                        {
                           nnz = 100;
                           OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

                           v.reset();
						   v.setType( VariablePre::V_PRE_TURMA_SAB );
                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setSubCjtSala(*itCjtSala);

                           it_v = vHashPre.find( v );
                           if ( it_v != vHashPre.end() )
                           {
                              row.insert( it_v->second, -1.0 );
                           }

                           v.reset();
                           v.setType( VariablePre::V_PRE_TURMA_CALENDARIO );
                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setCalendario( *itSl1 );
                           v.setSubCjtSala(*itCjtSala);

                           it_v = vHashPre.find( v );
                           if ( it_v != vHashPre.end() )
                           {
                              row.insert( it_v->second, 1.0 );
                           }

                           v.reset();
                           v.setType( VariablePre::V_PRE_TURMA_CALENDARIO );
                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setCalendario( *itSl2 );
                           v.setSubCjtSala(*itCjtSala);

                           it_v = vHashPre.find( v );
                           if ( it_v != vHashPre.end() )
                           {
                              row.insert( it_v->second, 1.0 );
                           }

                           if ( row.getnnz() != 0 )
                           {
                              cHashPre[ c ] = lp->getNumRows();
                              lp->addRow( row );
                              restricoes++;
                           }
                        }
                        else
                        {
                           v.reset();
                           v.setType( VariablePre::V_PRE_TURMA_CALENDARIO );
                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setCalendario( *itSl1 );
                           v.setSubCjtSala(*itCjtSala);

                           it_v = vHashPre.find( v );
                           if ( it_v != vHashPre.end() )
                           {
                              lp->chgCoef(it_c->second, it_v->second, 1.0 );
                           }

                           v.reset();
                           v.setType( VariablePre::V_PRE_TURMA_CALENDARIO );
                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setCalendario( *itSl2 );
                           v.setSubCjtSala(*itCjtSala);

                           it_v = vHashPre.find( v );
                           if ( it_v != vHashPre.end() )
                           {
                              lp->chgCoef(it_c->second, it_v->second, 1.0 );
                           }
                        }
                     }
                  }
               }
			}
		 }
      }
   }

   return restricoes;
}

int PreModelo::cria_preRestricao_turma_turnosIES_com_intersecao( int campusId, int cjtAlunosId, int prioridade, int r )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;
   int min;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;
   ConstraintPreHash::iterator it_c;
   
		
   std::map< Disciplina*, GGroup< TurnoIES*, LessPtr<TurnoIES> >, LessPtr<Disciplina> > mapDiscTurnosIES;
   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
        Disciplina *disciplina = *it_disc;
        ITERA_GGROUP_LESSPTR( itTurnoIES, problemData->turnosIES, TurnoIES )
		{			
			if ( disciplina->possuiTurnoIES( *itTurnoIES ) )
			{
				mapDiscTurnosIES[disciplina].add( *itTurnoIES );
			}
		}
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      Campus *cp = *itCampus;
	  if ( itCampus->getId() != campusId )
	  {
		  continue;
	  }

     ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               Disciplina *disciplina = *it_disc;
		                
			   if ( prioridade==1 && r==1 && MODELO_ESTIMA_TURMAS && FIXA_SALAS_ESTIMATURMAS )
			   {
					if ( disciplina->getNumTurmasNaSala( itCjtSala->getId() ) == 0 )
						continue;
			   }

			   GGroup< TurnoIES*, LessPtr<TurnoIES> > turnosIESdisc = mapDiscTurnosIES[disciplina];
			   ITERA_GGROUP_LESSPTR( itMapTurnoIES1, turnosIESdisc, TurnoIES )
			   {					
				    TurnoIES * tt1 = *itMapTurnoIES1;

				    ITERA_GGROUP_INIC_LESSPTR( itMapTurnoIES2, itMapTurnoIES1, turnosIESdisc, TurnoIES )
				    {	
						TurnoIES * tt2 = *itMapTurnoIES2;

						GGroup< TurnoIES*, LessPtr<TurnoIES> > turnosIESDisc;
						turnosIESDisc.add( tt1 );
						turnosIESDisc.add( tt2 );
						GGroup< HorarioDia*, LessPtr<HorarioDia> > hs = problemData->retornaHorariosDiaComuns( turnosIESDisc );

						if ( hs.size() >= disciplina->getTotalCreditos() )
							continue;

						for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
						{
							c.reset();
							c.setType( ConstraintPre::C_PRE_TURMA_CALENDARIOS_INTERSECAO );
							c.setCampus( *itCampus );
							c.setDisciplina( disciplina );
							c.setTurma( turma );
							c.setParTurnosIES( tt1, tt2 );

							sprintf( name, "%s", c.toString().c_str() );

							it_c = cHashPre.find( c );

							if ( it_c == cHashPre.end() )
							{
							   nnz = 100;
							   OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

							   v.reset();
							   v.setType( VariablePre::V_PRE_TURMA_SAB );
							   v.setTurma( turma );
							   v.setDisciplina( disciplina );
							   v.setSubCjtSala(*itCjtSala);

							   it_v = vHashPre.find( v );
							   if ( it_v != vHashPre.end() )
							   {
								  row.insert( it_v->second, -1.0 );
							   }

							   v.reset();
							   v.setType( VariablePre::V_PRE_TURMA_TURNOIES );
							   v.setTurma( turma );
							   v.setDisciplina( disciplina );
							   v.setTurnoIES( tt1 );
							   v.setSubCjtSala(*itCjtSala);

							   it_v = vHashPre.find( v );
							   if ( it_v != vHashPre.end() )
							   {
								  row.insert( it_v->second, 1.0 );
							   }

							   v.reset();
							   v.setType( VariablePre::V_PRE_TURMA_TURNOIES );
							   v.setTurma( turma );
							   v.setDisciplina( disciplina );
							   v.setTurnoIES( tt2 );
							   v.setSubCjtSala(*itCjtSala);

							   it_v = vHashPre.find( v );
							   if ( it_v != vHashPre.end() )
							   {
								  row.insert( it_v->second, 1.0 );
							   }

							   if ( row.getnnz() != 0 )
							   {
								  cHashPre[ c ] = lp->getNumRows();
								  lp->addRow( row );
								  restricoes++;
							   }
							}
							else
							{
							   v.reset();
							   v.setType( VariablePre::V_PRE_TURMA_TURNOIES );
							   v.setTurma( turma );
							   v.setDisciplina( disciplina );
							   v.setTurnoIES( tt1 );
							   v.setSubCjtSala(*itCjtSala);

							   it_v = vHashPre.find( v );
							   if ( it_v != vHashPre.end() )
							   {
								  lp->chgCoef(it_c->second, it_v->second, 1.0 );
							   }

							   v.reset();
							   v.setType( VariablePre::V_PRE_TURMA_TURNOIES );
							   v.setTurma( turma );
							   v.setDisciplina( disciplina );
							   v.setTurnoIES( tt2 );
							   v.setSubCjtSala(*itCjtSala);

							   it_v = vHashPre.find( v );
							   if ( it_v != vHashPre.end() )
							   {
								  lp->chgCoef(it_c->second, it_v->second, 1.0 );
							   }
							}
						}
					}
				}
			}
		 }
      }
   }

   return restricoes;
}
     
int PreModelo::cria_preRestricao_setV_TurnoIES( int campusId, int cjtAlunosId )
{
    int restricoes = 0;
		
    char name[ 100 ];

    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		if( vit->first.getType() == VariablePre::V_PRE_ALUNOS )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			TurnoIES* turnoIES = problemData->refTurnos[ v.getOferta()->getTurnoId() ];
						
			// -----------------------------------------
			// Constraint 1
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setTurnoIES(turnoIES);
			c.setSubCjtSala(v.getSubCjtSala());

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}

         // -----------------------------------------
			// Constraint 2
			
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setTurnoIES(turnoIES);
			c.setSubCjtSala(v.getSubCjtSala());

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
      }

		// v_{i,d,tt,s}
		else if( vit->first.getType() == VariablePre::V_PRE_TURMA_TURNOIES )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			TurnoIES* turnoIES = v.getTurnoIES();
						
			// -----------------------------------------
			// Constraint 1

			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setTurnoIES(turnoIES);
			c.setSubCjtSala(v.getSubCjtSala());		  

 		    int maxP = disciplina->getMaxAlunosP();
		    int maxT = disciplina->getMaxAlunosT();
		    int maxAlunos = ( (maxP > maxT) ? maxP : maxT );
						
			double bigM = maxAlunos;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{				
				lp->chgCoef(cit->second, vit->second, bigM);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, bigM);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
						
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setTurnoIES(turnoIES);
	        c.setSubCjtSala(v.getSubCjtSala());

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, 1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, 1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}		
		}
	}
	
	return restricoes;
}

int PreModelo::cria_preRestricao_setV2( int campusId, int cjtAlunosId )
{
    int restricoes = 0;
		
    char name[ 100 ];

    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		// no_{i,d,oft}
		/*if( vit->first.getType() == VariablePre::V_PRE_NRO_ALUNOS_OFT )
		{			
			VariablePre v = vit->first;

			Calendario *sl = v.getOferta()->curriculo->calendario;
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
						
			// -----------------------------------------
			// Constraint 1
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
			
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}*/
      // a_{i,d,s,oft}
		if( vit->first.getType() == VariablePre::V_PRE_ALUNOS )
		{			
			VariablePre v = vit->first;

			Calendario *sl = v.getOferta()->curriculo->calendario;
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
						
			// -----------------------------------------
			// Constraint 1
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);
         c.setSubCjtSala(v.getSubCjtSala());

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}

         // -----------------------------------------
			// Constraint 2
			
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);
         c.setSubCjtSala(v.getSubCjtSala());

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
      }

		// v_{i,d,sl}
		else if( vit->first.getType() == VariablePre::V_PRE_TURMA_CALENDARIO )
		{			
			VariablePre v = vit->first;

			Calendario *sl = v.getCalendario();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();

			// -----------------------------------------
			// Constraint 1

			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);
			c.setSubCjtSala(v.getSubCjtSala());		  

 		    int maxP = disciplina->getMaxAlunosP();
		    int maxT = disciplina->getMaxAlunosT();
		    int maxAlunos = ( (maxP > maxT) ? maxP : maxT );
						
			double bigM = maxAlunos;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{				
				lp->chgCoef(cit->second, vit->second, bigM);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, bigM);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
						
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);
         c.setSubCjtSala(v.getSubCjtSala());

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, 1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, 1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}		
		}
	}
	
	return restricoes;
}

// Não está sendo usada. Usamos cria_preRestricao_setV2
int PreModelo::cria_preRestricao_setV( int campusId, int cjtAlunosId )
{
    int restricoes = 0;
		
    char name[ 100 ];

    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		// s_{i,d,a}
		if( vit->first.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{			
			VariablePre v = vit->first;
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			
			#ifdef EQUIVALENCIA_DESDE_O_INICIO 
			AlunoDemanda *alDemanda = NULL;
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
				alDemanda = v.getAluno()->getAlunoDemandaEquiv( disciplina );
			else
				alDemanda = v.getAluno()->getAlunoDemanda( disciplina->getId() );
			#else
			AlunoDemanda *alDemanda = v.getAluno()->getAlunoDemanda( disciplinaId );
			#endif			
							
			if ( alDemanda==NULL ) std::cout<<"Erro! AlDem nao encontrado em cria_preRestricao_setV";
				
			Calendario *sl = v.getAluno()->getCalendario( alDemanda->demanda );
			
			// -----------------------------------------
			// Constraint 1
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
			
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// v_{i,d,sl}
		else if( vit->first.getType() == VariablePre::V_PRE_TURMA_CALENDARIO )
		{			
			VariablePre v = vit->first;

			Calendario *sl = v.getCalendario();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();

			// -----------------------------------------
			// Constraint 1

			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);

			double bigM = 200;
			
			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{				
				lp->chgCoef(cit->second, vit->second, bigM);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, bigM);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
						
			c.reset();
			c.setType( ConstraintPre::C_PRE_SET_V_2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSemanaLetiva(sl);

			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, 1.0);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, 1.0);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}		
		}
	}
	
	return restricoes;
}

int PreModelo::cria_preRestricao_aloca_turma_sab( int campusId, int cjtAlunosId )
{
    int restricoes = 0;
		
    char name[ 100 ];

    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

    std::vector<std::pair<Calendario*,Calendario*> > paresCalend;

   ITERA_GGROUP_LESSPTR( it_cal1, problemData->calendarios, Calendario )
   {
      Calendario *cal1 = *it_cal1;

      ITERA_GGROUP_INIC_LESSPTR( it_cal2, it_cal1, problemData->calendarios, Calendario )
      {
         Calendario *cal2 = *it_cal2;

		 if ( cal1 == cal2 ) continue;
		 
         GGroup<Calendario*,LessPtr<Calendario>> gCal;
         gCal.add(cal2);

         std::map< int, GGroup<HorarioAula*, LessPtr<HorarioAula>> > diasHorCom = cal1->retornaDiaHorariosEmComum(gCal);    
         bool soSab = false;

         for (int i=0; i <= 7; i++)
         {
            if ( i < 7 && (diasHorCom.find(i) != diasHorCom.end()) )
            {
               if ( diasHorCom[i].size() != 0 )
               {
                  soSab = false;
                  break;
               }
            }
            else if ( i == 7 && (diasHorCom.find(i) != diasHorCom.end()) )
            {
               if ( diasHorCom[i].size() != 0 )
               {
                  soSab = true;
                  break;
               }
            }
         }

         if ( soSab ) 
         {
            std::pair<Calendario*,Calendario*> auxPair;
            auxPair.first = cal1;
            auxPair.second = cal2;
            paresCalend.push_back(auxPair);
         }
      }
   }

   vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		// v_{i,d,sl}
		if( vit->first.getType() == VariablePre::V_PRE_TURMA_CALENDARIO )
		{			
			VariablePre v = vit->first;

			Calendario *sl = v.getCalendario();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALOC_TURMA_SAB );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSubCjtSala(v.getSubCjtSala());

			for (int i=0; i < (int)paresCalend.size(); i++)
			{
				if ( paresCalend[i].first->getId() != sl->getId() && paresCalend[i].second->getId() != sl->getId()  )
					continue;

				c.setParCalendarios(paresCalend[i].first,paresCalend[i].second);

				cit = cHashPre.find(c);

				if(cit != cHashPre.end())
				{
					lp->chgCoef(cit->second, vit->second, 1.0);
				}
				else
				{
					int nnz=100;
					sprintf( name, "%s", c.toString().c_str() ); 
					OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

					row.insert(vit->second, 1.0);

					cHashPre[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}						
			}
		}
   }

   vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		// sab_{i,d,s}
		if( vit->first.getType() == VariablePre::V_PRE_TURMA_SAB )
		{			
			VariablePre v = vit->first;

			ConjuntoSala *s = v.getSubCjtSala();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALOC_TURMA_SAB );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSubCjtSala(s);

			for (int i=0; i < (int)paresCalend.size(); i++)
			{
				c.setParCalendarios(paresCalend[i].first,paresCalend[i].second);

				cit = cHashPre.find(c);

				if(cit != cHashPre.end())
				{
					lp->chgCoef(cit->second, vit->second, -1.0);
				}					
			}
      }
   }
		
	
	return restricoes;
}

int PreModelo::cria_preRestricao_aloca_turma_sab_turnoIES( int campusId, int cjtAlunosId )
{
    int restricoes = 0;
		
    char name[ 100 ];

    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

    std::vector<std::pair<TurnoIES*,TurnoIES*> > paresTurnosIES;

   ITERA_GGROUP_LESSPTR( it_turnoIES1, problemData->turnosIES, TurnoIES )
   {
      TurnoIES *turnoIES1 = *it_turnoIES1;

      ITERA_GGROUP_INIC_LESSPTR( it_turnoIES2, it_turnoIES1, problemData->turnosIES, TurnoIES )
      {
         TurnoIES *turnoIES2 = *it_turnoIES2;

		 if ( turnoIES1 == turnoIES2 ) continue;
		 
         GGroup<TurnoIES*,LessPtr<TurnoIES>> gTurnosIES;
         gTurnosIES.add(turnoIES1);
		 gTurnosIES.add(turnoIES2);

         GGroup<HorarioDia*, LessPtr<HorarioDia>> diasHorDiaCom = problemData->retornaHorariosDiaComuns(gTurnosIES);    
         bool outros = false, sab = false;

		 ITERA_GGROUP_LESSPTR( it_hd, diasHorDiaCom, HorarioDia )
		 {
			 if ( it_hd->getDia() == 7 )
			 {
				sab = true;
			 }
			 else if ( it_hd->getDia() != 7 )
			 {
				 outros = true;
				 break;
			 }
		 }

         if ( sab && !outros ) // só sabado
         {
            std::pair<TurnoIES*,TurnoIES*> auxPair;
            auxPair.first = turnoIES1;
            auxPair.second = turnoIES2;
            paresTurnosIES.push_back(auxPair);
         }
      }
   }

   vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		// v_{i,d,tt,s}
		if( vit->first.getType() == VariablePre::V_PRE_TURMA_TURNOIES )
		{			
			VariablePre v = vit->first;

			TurnoIES *turnoIES = v.getTurnoIES();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALOC_TURMA_SAB );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSubCjtSala(v.getSubCjtSala());

			for (int i=0; i < (int)paresTurnosIES.size(); i++)
			{
				if ( paresTurnosIES[i].first->getId() != turnoIES->getId() && paresTurnosIES[i].second->getId() != turnoIES->getId()  )
					continue;

				c.setParTurnosIES(paresTurnosIES[i].first,paresTurnosIES[i].second);

				cit = cHashPre.find(c);

				if(cit != cHashPre.end())
				{
					lp->chgCoef(cit->second, vit->second, 1.0);
				}
				else
				{
					int nnz=100;
					sprintf( name, "%s", c.toString().c_str() ); 
					OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

					row.insert(vit->second, 1.0);

					cHashPre[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}						
			}
		}
   }

   vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		// sab_{i,d,s}
		if( vit->first.getType() == VariablePre::V_PRE_TURMA_SAB )
		{			
			VariablePre v = vit->first;

			ConjuntoSala *s = v.getSubCjtSala();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALOC_TURMA_SAB );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setSubCjtSala(s);

			for (int i=0; i < (int)paresTurnosIES.size(); i++)
			{
				c.setParTurnosIES(paresTurnosIES[i].first,paresTurnosIES[i].second);

				cit = cHashPre.find(c);

				if(cit != cHashPre.end())
				{
					lp->chgCoef(cit->second, vit->second, -1.0);
				}					
			}
      }
   }
		
	
	return restricoes;
}

int PreModelo::cria_preRestricao_max_cred_sab( int campusId, int cjtAlunosId )
{
    int restricoes = 0;
		
    char name[ 100 ];

    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	bool alertaImpresso = false;

   vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		// sab_{i,d,s}
		if( vit->first.getType() == VariablePre::V_PRE_TURMA_SAB )
		{			
			VariablePre v = vit->first;

			ConjuntoSala *s = v.getSubCjtSala();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_MAX_CRED_SAB );
			c.setSubCjtSala(s);

			double coef = disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos();

			double rhs = s->salas.begin()->second->getTempoDispPorDia( 7 );
			
#ifdef KROTON
			rhs = 250;
			if ( !alertaImpresso ){
				std::cout << "\nATENCAO: Codigo com valores de tempos maximo no sabado fixados!!\n";
				alertaImpresso = true;
			}
#endif

			//double horasSab = s->salas.begin()->second->getMaxCredsPossiveis( 7, disciplina ); //300;
		 
			cit = cHashPre.find(c);

			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=100;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}
   }
		
	return restricoes;
}


int PreModelo::cria_preRestricao_aloca_aluno_turma_aberta( int campusId, int cjtAlunosId )
{
    int restricoes = 0;		
    char name[ 100 ];
    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		//// s_{i,d,a}
		//if( vit->first.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		//{			
		//	VariablePre v = vit->first;

		//	Disciplina *disciplina = v.getDisciplina();
		//	int turma = v.getTurma();
		//
		//	c.reset();
		//	c.setType( ConstraintPre::C_PRE_ALUNO_EM_TURMA_ABERTA );
		//	c.setDisciplina(disciplina);
		//	c.setTurma(turma);
		//	
		//	double coef = 1.0;

		//	cit = cHashPre.find(c);
		//	if(cit != cHashPre.end())
		//	{
		//		lp->chgCoef(cit->second, vit->second, coef);
		//	}
		//	else
		//	{
		//		int nnz=150;
		//		sprintf( name, "%s", c.toString().c_str() ); 
		//		OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

		//		row.insert(vit->second, coef);

		//		cHashPre[ c ] = lp->getNumRows();

		//		lp->addRow( row );
		//		restricoes++;
		//	}
		//}

		// n_{i,d}
		
		/*if( vit->first.getType() == VariablePre::V_PRE_NRO_ALUNOS )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALUNO_EM_TURMA_ABERTA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			
			double coef = 1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}*/
      if( vit->first.getType() == VariablePre::V_PRE_ALUNOS )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALUNO_EM_TURMA_ABERTA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			
			double coef = 1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// z_{i,d,cp}
		else if( vit->first.getType() == VariablePre::V_PRE_ABERTURA )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALUNO_EM_TURMA_ABERTA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);

		    int maxP = disciplina->getMaxAlunosP();
		    int maxT = disciplina->getMaxAlunosT();
		    int maxAlunos = ( (maxP > maxT) ? maxP : maxT );
						
			double M = - maxAlunos;
						
			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, M);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, M);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}
	}
	
	return restricoes;
}
int PreModelo::cria_preRestricao_aloca_aluno_turma_aberta2( int campusId, int cjtAlunosId )
{
    int restricoes = 0;		
    char name[ 100 ];
    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		// no_{i,d,oft}
		/*if( vit->first.getType() == VariablePre::V_PRE_NRO_ALUNOS_OFT ) 
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALUNO_EM_TURMA_ABERTA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			
			double coef = 1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}*/
      if( vit->first.getType() == VariablePre::V_PRE_ALUNOS ) 
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALUNO_EM_TURMA_ABERTA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			
			double coef = 1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// z_{i,d,cp}
		else if( vit->first.getType() == VariablePre::V_PRE_ABERTURA )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			
			c.reset();
			c.setType( ConstraintPre::C_PRE_ALUNO_EM_TURMA_ABERTA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			
		    int maxP = disciplina->getMaxAlunosP();
		    int maxT = disciplina->getMaxAlunosT();
		    int maxAlunos = ( (maxP > maxT) ? maxP : maxT );

			double M = - maxAlunos;
						
			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, M);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, M);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}
	}
	
	return restricoes;
}

int PreModelo::cria_preRestricao_nro_alunos_por_turma( int campusId, int cjtAlunosId )
{
    int restricoes = 0;		
    char name[ 100 ];
    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		// s_{i,d,a}
		if( vit->first.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintPre::C_PRE_NRO_ALUNOS_POR_TURMA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );

			double coef = 1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// n_{i,d,cp}
		else if( vit->first.getType() == VariablePre::V_PRE_NRO_ALUNOS )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintPre::C_PRE_NRO_ALUNOS_POR_TURMA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );

			double coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}
	}
	
	return restricoes;
}
int PreModelo::cria_preRestricao_nro_alunos_por_turma2( int campusId, int cjtAlunosId )
{
    int restricoes = 0;		
    char name[ 100 ];
    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		// s_{i,d,a}
		if( vit->first.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{			
			VariablePre v = vit->first;
			
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();
						
			if ( turma == 1 && disciplina->getId() == 5937 && v.getAluno()->getAlunoId() == 8096 )
				std::cout<<"aqui";

			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			AlunoDemanda *alDemanda = NULL;
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
				alDemanda = v.getAluno()->getAlunoDemandaEquiv( disciplina );
			else
				alDemanda = v.getAluno()->getAlunoDemanda( disciplina->getId() );
			#else
				AlunoDemanda *alDemanda = v.getAluno()->getAlunoDemanda( disciplina->getId() );
			#endif			
			
			Demanda *demanda = alDemanda->demanda;
			Oferta* oferta = v.getAluno()->getOferta(demanda);

			c.reset();
			c.setType( ConstraintPre::C_PRE_NRO_ALUNOS_POR_TURMA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );
			c.setOferta( oferta );

			double coef = 1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// no_{i,d,oft}
		/*else if( vit->first.getType() == VariablePre::V_PRE_NRO_ALUNOS_OFT )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getOferta()->campus;
			Oferta* oferta = v.getOferta();

			c.reset();
			c.setType( ConstraintPre::C_PRE_NRO_ALUNOS_POR_TURMA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );
			c.setOferta( oferta );

			double coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}*/
      // a
      else if( vit->first.getType() == VariablePre::V_PRE_ALUNOS )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getOferta()->campus;
			Oferta* oferta = v.getOferta();

			c.reset();
			c.setType( ConstraintPre::C_PRE_NRO_ALUNOS_POR_TURMA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );
			c.setOferta( oferta );

			double coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}
	}
	
	return restricoes;
}

/*
	Alocação minima de demanda por aluno

	sum[d] nCreds_{d} * (1 - fd_{d,a}) >= MinAtendPerc * TotalDemanda_{a} - fmd_{a}		, para cada aluno a
	
	min sum[a] fmd{a}
*/
int PreModelo::cria_preRestricao_aloc_min_aluno( int campusId, int cjtAlunosId  )
{
   int restricoes = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return restricoes;
   }

   char name[ 200 ];
   int nnz=0;

   ConstraintPre c;
   ConstraintPreHash::iterator cit;
   VariablePre v;
   VariablePreHash::iterator vit;   

   std::map<int, int> mapConstraintSomaCredFD;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		VariablePre v = vit->first;
		
		int nCreds=0;
	
		double coef;

		if( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA ) // fd_{d,a}
		{
			nCreds = v.getDisciplina()->getTotalCreditos();
			coef = - nCreds;

			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			 if ( problemData->parametros->considerar_equivalencia_por_aluno ) // Só está sendo criadas folgas fd para as teóricas
			 {
				 std::map< int, Disciplina * >::iterator itMap 
					 = problemData->refDisciplinas.find( - v.getDisciplina()->getId() );
				 if ( itMap != problemData->refDisciplinas.end() )
				 {
					 nCreds += itMap->second->getTotalCreditos(); // add os creditos praticos
				 }
			 }
			#endif

		}
		else if( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND1 ) // fmd1_{a}
		{ 
			coef = 1.0;
		}
		else if( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND2 ) // fmd2_{a}
		{ 
			coef = 1.0;
		}
		else if( v.getType() == VariablePre::V_PRE_FOLGA_ALUNO_MIN_ATEND3 ) // fmd3_{a}
		{ 
			coef = 1.0;
		}
		else continue;

		Aluno *aluno = v.getAluno();
		
		if ( !aluno->hasCampusId(campusId) )
		{
			continue;
		}
				
		c.reset();
		c.setType( ConstraintPre::C_PRE_ALOC_MIN_ALUNO );
		c.setAluno( aluno );
		
		sprintf( name, "%s", c.toString().c_str() ); 

		cit = cHashPre.find( c ); 
		if ( cit != cHashPre.end() )
		{
			lp->chgCoef( cit->second, vit->second, coef );
			
			if( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA ) // fd_{d,a}
			{	
				std::map<int, int>::iterator itMap = mapConstraintSomaCredFD.find( cit->second );
				if ( itMap == mapConstraintSomaCredFD.end() )
					mapConstraintSomaCredFD[ cit->second ] = nCreds;
				else
					itMap->second += nCreds;
			}
		}
		else
		{
			double init_rhs = problemData->parametros->minAtendPercPorAluno * aluno->getNroCredsOrigRequeridosP1();
		
			nnz += aluno->demandas.size() + 1;
			OPT_ROW row( nnz, OPT_ROW::GREATER, init_rhs , name );
		
			row.insert( vit->second, coef );
			cHashPre[ c ] = lp->getNumRows();
			
			if( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA ) // fd_{d,a}
			{
				std::map<int, int>::iterator itMap = mapConstraintSomaCredFD.find( lp->getNumRows() );
				if ( itMap == mapConstraintSomaCredFD.end() )
					mapConstraintSomaCredFD[ lp->getNumRows() ] = nCreds;
				else
					itMap->second += nCreds;
			}
			
			lp->addRow( row );
			restricoes++;
		}
	}

	std::map<int, int>::iterator itMap = mapConstraintSomaCredFD.begin();			
	for ( ; itMap != mapConstraintSomaCredFD.end(); itMap++ )
	{		
		int rowId = itMap->first;
		int somaCredsFd = itMap->second;
		double init_rhs = lp->getRHS( rowId );
		double rhs = init_rhs - somaCredsFd;

		lp->chgRHS( rowId, rhs );
	}

	return restricoes;
}

int PreModelo::cria_preRestricao_folga_ocupacao_sala( int campusId, int cjtAlunosId )
{
    int restricoes = 0;		

	if ( ! problemData->parametros->min_folga_ocupacao_sala )
	{
		return restricoes;
	}

    char name[ 100 ];
    ConstraintPre c;
	ConstraintPreHash::iterator cit;
    VariablePre v;
    VariablePreHash::iterator vit;

	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++ )
	{
		// o_{i,d,s}
		if( vit->first.getType() == VariablePre::V_PRE_OFERECIMENTO )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = problemData->refCampus[ v.getUnidade()->getIdCampus() ];

			c.reset();
			c.setType( ConstraintPre::C_PRE_FOLGA_OCUPACAO_SALA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );

			double coef = v.getSubCjtSala()->salas.begin()->second->getCapacidade();

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// n_{i,d,cp}
		else if( vit->first.getType() == VariablePre::V_PRE_NRO_ALUNOS )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintPre::C_PRE_FOLGA_OCUPACAO_SALA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );

			double coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}

		// a_{i,d,s,oft}
		else if( vit->first.getType() == VariablePre::V_PRE_ALUNOS  &&  PREMODELO_SEM_VAR_N )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintPre::C_PRE_FOLGA_OCUPACAO_SALA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );

			double coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}

		// fos_{i,d,cp}
		else if( vit->first.getType() == VariablePre::V_PRE_FOLGA_OCUPA_SALA )
		{			
			VariablePre v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintPre::C_PRE_FOLGA_OCUPACAO_SALA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );

			double coef = -1.0;

			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=150;
				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}
	}
	
	return restricoes;
}


// Restrição para ser usada no caso da UNIT, que tem disponibilidade
// limitada nos sábados para algumas disciplinas
int PreModelo::cria_preRestricao_tempo_max_aluno_sabado( int campusId )
{
   int numRest=0;

#ifndef UNIT
	return numRest;
#endif

   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   bool printMsg=true;

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
   {		
      VariablePre v = vit->first;

      double coef = 0.0;
      if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
	  {		  
         Disciplina* disciplina = v.getDisciplina();
		 int turno = v.getTurno();
		 Aluno *aluno = v.getAluno();
		 AlunoDemanda *alunoDem = aluno->getAlunoDemandaEquiv(disciplina); 

		 // ---------------------------------------
		 // Verifica se o unico dia no turno é sábado
		 bool turnoSoNoSabado=true;
		 GGroup< HorarioDia*, LessPtr<HorarioDia> > horsDia = 
			 problemData->getHorariosDiaPorTurno( disciplina, alunoDem->demanda->getTurnoIES(), turno );
		 ITERA_GGROUP_LESSPTR( itHorarioDia, horsDia, HorarioDia )
		 {
			if ( itHorarioDia->getDia() != 7 ) // dia 7 = sábado
			{
				turnoSoNoSabado = false; break;
			}
		 }		 
		 if ( !turnoSoNoSabado ) continue;
		 // ---------------------------------------
		 
         c.reset();
         c.setType( ConstraintPre::C_PRE_TEMPO_MAX_ALUNO_SAB );
         c.setTurno( turno );
		 c.setAluno( aluno );
		 
         double coef = disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos();
		 
         cit = cHashPre.find(c);
         if ( cit == cHashPre.end() )
         {
			 if (printMsg)
			 {
				std::cout<<"\nATENCAO: TEMPO MAX NO SAB PARA ALUNO FIXO EM PREMODELO!!";
				printMsg=false;
			 }

            double htps=0;
#ifdef KROTON
				htps = 225;
			else // noite
				htps = 200;
#elif defined UNIT
			if ( v.getTurno() == 1 || v.getTurno() == 2 ) // 1. Manha ou 2. Tarde
				htps = 300;
			else // noite
				htps = 200;
#endif
			nnz = aluno->demandas.size(); // estimativa

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::LESS, htps, name );

            row.insert( vit->second, coef );

            cHashPre[ c ] = lp->getNumRows();
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


// Restrição para casos multi-turno
int PreModelo::cria_preRestricao_tempo_max_aluno_turno( int campusId )
{
   int numRest=0;
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
	bool alertaImpresso = false;

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
   {		
      VariablePre v = vit->first;

      double coef = 0.0;
      if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
	  {		  
         Disciplina* disciplina = v.getDisciplina();
		 int turno = v.getTurno();
		 Aluno *aluno = v.getAluno();
		 AlunoDemanda *alunoDem = aluno->getAlunoDemandaEquiv(disciplina); 
		 
		 // ---------------------------------------
		 // Pula os casos em que o unico dia no turno é sábado.
		 // Há tratamento especial para tais casos.
		 bool turnoSoNoSabado=true;
		 GGroup< HorarioDia*, LessPtr<HorarioDia> > horsDia = 
			 problemData->getHorariosDiaPorTurno( disciplina, alunoDem->demanda->getTurnoIES(), turno );
		 ITERA_GGROUP_LESSPTR( itHorarioDia, horsDia, HorarioDia )
		 {
			if ( itHorarioDia->getDia() != 7 ) // dia 7 = sábado
			{
				turnoSoNoSabado = false; break;
			}
		 }		 
		 if ( turnoSoNoSabado ) continue;
		 // ---------------------------------------

         c.reset();
         c.setType( ConstraintPre::C_PRE_TEMPO_MAX_ALUNO_TURNO);
         c.setTurno( turno );
		 c.setAluno( aluno );
		 
         double coef = disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos();
		 
         cit = cHashPre.find(c);
         if ( cit == cHashPre.end() )
         {
			 if ( !alertaImpresso ){
				std::cout<<"\nATENCAO: TEMPO MAX PARA ALUNO FIXO EM PREMODELO!!";
				alertaImpresso = true;
			 }

            double htps = 225*5;
#ifdef KROTON
			if ( v.getTurno() == 1 || v.getTurno() == 2 ) // 1. Manha ou 2. Tarde
				htps = 250*5;
			else // noite
				htps = 250*5;
#elif defined UNIT
			if ( v.getTurno() == 1 || v.getTurno() == 2 ) // 1. Manha ou 2. Tarde
				htps = 300*5;
			else // noite
				htps = 200*5;
#endif
			nnz = 5 * aluno->demandas.size(); // estimativa

            sprintf( name, "%s", c.toString().c_str() );
            OPT_ROW row( nnz, OPT_ROW::LESS, htps, name );

            row.insert( vit->second, coef );

            cHashPre[ c ] = lp->getNumRows();
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

// Restrição para casos multi-turno
int PreModelo::cria_preRestricao_associa_aluno_turno( int campusId )
{
   int numRest=0;
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
   {		
		VariablePre v = vit->first;

		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{		 
			coef = 1.0;
		}
		else if( v.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO )
		{
			coef = - v.getSubCjtSala()->getCapacidadeRepr();
		}
		else continue;

		Disciplina* disciplina = v.getDisciplina();
		int turno = v.getTurno();
		int turma = v.getTurma();

		c.reset();
		c.setType( ConstraintPre::C_PRE_ALUNOS_TURNO);
		c.setTurno( turno );
		c.setDisciplina( disciplina );
		c.setTurma( turma );
		 		 
		cit = cHashPre.find(c);
		if ( cit == cHashPre.end() )
		{			
			nnz = 200; // estimativa

			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef );

			cHashPre[ c ] = lp->getNumRows();
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

   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

int PreModelo::cria_preRestricao_usa_disciplina( int campusId )
{
   int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;

		if( v.getType() == VariablePre::V_PRE_ABERTURA )
		{
			 c.reset();
			 c.setType( ConstraintPre::C_PRE_USA_DISC );
			 c.setDisciplina(v.getDisciplina());
			 			 
			 double coef = -1.0;

			 cit = cHashPre.find(c);
			 if ( cit == cHashPre.end() )
			 {				 
				 nnz = v.getDisciplina()->getNumTurmas() + 1;
				 
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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
		else if( v.getType() == VariablePre::V_PRE_USA_DISCIPLINA )
		{
			 c.reset();
			 c.setType( ConstraintPre::C_PRE_USA_DISC );
			 c.setDisciplina(v.getDisciplina());
			 			 
			 double coef = problemData->campi.size() * v.getDisciplina()->getNumTurmas();	// big M

			 cit = cHashPre.find(c);
			 if ( cit == cHashPre.end() )
			 {			
				 nnz = v.getDisciplina()->getNumTurmas() + 1;
				 
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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


int PreModelo::cria_preRestricao_usa_unid_periodo( int campusId )
{
   int numRest=0; 
   
   if ( !problemData->parametros->limitar_unidades_por_periodo )
   {
		return numRest;
   }

   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   nnz = problemData->refCampus[campusId]->unidades.size();

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;

		if( v.getType() == VariablePre::V_PRE_UNID_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintPre::C_PRE_USA_UNID_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 c.setUnidade( v.getUnidade() );
			 			 
			 double coef = -100000;

			 cit = cHashPre.find(c);
			 if ( cit == cHashPre.end() )
			 {
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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
		else if ( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			Disciplina *disciplina = v.getDisciplina();

			// Não restringe unidade única em caso de disciplina prática
			if ( disciplina->getId() < 0 )
				continue;

			Oferta *oferta = v.getOferta();

			int periodo;
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
				periodo = oferta->curriculo->getPeriodoEquiv( disciplina );
			else
				periodo = oferta->curriculo->getPeriodo( disciplina );

			if ( periodo < 0 )
			{
				std::cout<<"\nErro em PreModelo::cria_preRestricao_usa_unid_periodo(), periodo de disciplina "
					<< disciplina->getId() << " nao encontrado na oferta " << oferta->getId();		
			}

			c.reset();
			c.setType( ConstraintPre::C_PRE_USA_UNID_PERIODO );
			c.setOferta( oferta );
			c.setPeriodo( periodo );
			c.setUnidade( v.getUnidade() );
			 
			double coef = 1.0;

			cit = cHashPre.find(c);
			if ( cit == cHashPre.end() )
			{
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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

int PreModelo::cria_preRestricao_limita_unid_periodo( int campusId )
{
   int numRest=0; 
   
   if ( !problemData->parametros->limitar_unidades_por_periodo )
   {
		return numRest;
   }

   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   nnz = problemData->refCampus[campusId]->unidades.size();

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;

		if( v.getType() == VariablePre::V_PRE_UNID_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintPre::C_PRE_UNID_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 			 
			 double coef = 1.0;

			 cit = cHashPre.find(c);
			 if ( cit == cHashPre.end() )
			 {
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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
		else if( v.getType() == VariablePre::V_PRE_FOLGA_UNID_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintPre::C_PRE_UNID_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 			 
			 double coef = 1.0;

			 cit = cHashPre.find(c);
			 if ( cit == cHashPre.end() )
			 {
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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

int PreModelo::cria_preRestricao_usa_sala_periodo( int campusId )
{
   int numRest=0; 
   
   if ( !problemData->parametros->limitar_salas_por_periodo )
   {
		return numRest;
   }

   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   nnz = problemData->refCampus[campusId]->getTotalSalas();

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;

		if( v.getType() == VariablePre::V_PRE_SALA_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintPre::C_PRE_USA_SALA_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 c.setSubCjtSala( v.getSubCjtSala() );
			 			 
			 double coef = -100000;

			 cit = cHashPre.find(c);
			 if ( cit == cHashPre.end() )
			 {
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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
		else if ( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			Disciplina *disciplina = v.getDisciplina();

			// Não restringe unidade única em caso de disciplina prática
			if ( disciplina->getId() < 0 )
				continue;

			Oferta *oferta = v.getOferta();

			int periodo;
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
				periodo = oferta->curriculo->getPeriodoEquiv( disciplina );
			else
				periodo = oferta->curriculo->getPeriodo( disciplina );

			if ( periodo < 0 )
			{
				std::cout<<"\nErro em PreModelo::cria_preRestricao_usa_sala_periodo(), periodo de disciplina "
					<< disciplina->getId() << " nao encontrado na oferta " << oferta->getId();		
			}

			c.reset();
			c.setType( ConstraintPre::C_PRE_USA_SALA_PERIODO );
			c.setOferta( oferta );
			c.setPeriodo( periodo );
			c.setSubCjtSala( v.getSubCjtSala() );
			 
			double coef = 1.0;

			cit = cHashPre.find(c);
			if ( cit == cHashPre.end() )
			{
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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

int PreModelo::cria_preRestricao_limita_sala_periodo( int campusId )
{
   int numRest=0; 
   
   if ( !problemData->parametros->limitar_salas_por_periodo )
   {
		return numRest;
   }

   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   nnz = problemData->refCampus[campusId]->unidades.size();

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;

		if( v.getType() == VariablePre::V_PRE_SALA_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintPre::C_PRE_SALA_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 			 
			 double coef = 1.0;

			 cit = cHashPre.find(c);
			 if ( cit == cHashPre.end() )
			 {
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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
		else if( v.getType() == VariablePre::V_PRE_FOLGA_SALA_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintPre::C_PRE_SALA_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 			 
			 double coef = 1.0;

			 cit = cHashPre.find(c);
			 if ( cit == cHashPre.end() )
			 {
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef );

				cHashPre[ c ] = lp->getNumRows();
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

int PreModelo::cria_preRestricao_distrib_alunos( int campusId )
{
   int numRest=0; 
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator vit;
   ConstraintPreHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

   // Organiza as variáveis em maps por restrição

   std::map< Campus*, std::map< Disciplina*, std::map< int, GGroup< std::pair<int,double> > >,
	   LessPtr<Disciplina> >, LessPtr<Campus> > mapCpDiscTurma_VarCoef;

   vit = vHashPre.begin();
   for ( ; vit != vHashPre.end(); vit++)
   {		
		VariablePre v = vit->first;
		
		if( v.getType() == VariablePre::V_PRE_DISTRIB_ALUNOS )
		{
			if ( v.getCampus()->getId() == campusId )
			{
				for ( int i=1; i <= v.getDisciplina()->getNumTurmas(); i++ )
				{
					std::pair<int, double> varCoef(vit->second, -1.0);
					mapCpDiscTurma_VarCoef[ v.getCampus() ][ v.getDisciplina() ][ i ].add( varCoef );
				}
			}
		}
		else if ( v.getType() == VariablePre::V_PRE_ALUNOS )
		{
			if ( v.getUnidade()->getIdCampus() == campusId )
			{
				Campus *campus = problemData->refCampus[ v.getUnidade()->getIdCampus() ];

				std::pair<int, double> varCoef(vit->second, 1.0);
				mapCpDiscTurma_VarCoef[ campus ][ v.getDisciplina() ][ v.getTurma() ].add( varCoef );
			}
		}
   }

   // Cria as restrições

   std::map< Campus*, std::map< Disciplina*, std::map< int, GGroup< std::pair<int,double> > >,
	   LessPtr<Disciplina> >, LessPtr<Campus> >::iterator itMapCp = mapCpDiscTurma_VarCoef.begin();  

   for ( ; itMapCp != mapCpDiscTurma_VarCoef.end(); itMapCp++ )
   {
	   Campus *campus = itMapCp->first;

	   std::map< Disciplina*, std::map< int, GGroup< std::pair<int,double> > >,
		   LessPtr<Disciplina> > *mapDisc = & itMapCp->second;

	   std::map< Disciplina*, std::map< int, GGroup< std::pair<int,double> > >,
		   LessPtr<Disciplina> >::iterator itMapDisc = (*mapDisc).begin();
	   for ( ; itMapDisc != (*mapDisc).end(); itMapDisc++ )
	   {
		   Disciplina* disciplina = itMapDisc->first;

		   std::map< int, GGroup< std::pair<int,double> > > *mapTurma = & itMapDisc->second;

		   std::map< int, GGroup< std::pair<int,double> > >::iterator itMapTurma = (*mapTurma).begin();
		   for ( ; itMapTurma != (*mapTurma).end(); itMapTurma++ )
		   {
			    int turma = itMapTurma->first;
			   
				GGroup< std::pair<int,double> > *pares = & itMapTurma->second;
			    nnz = (*pares).size();

				c.reset();
				c.setType( ConstraintPre::C_PRE_DISTRIB_ALUNOS );
				c.setDisciplina( disciplina );
				c.setCampus( campus );
				c.setTurma( turma );
			 			 
				cit = cHashPre.find(c);
				if ( cit == cHashPre.end() )
				{
					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				    GGroup< std::pair<int,double> >::iterator itVars = (*pares).begin();
				    for ( ; itVars != (*pares).end(); itVars++ )
				    {
						row.insert( (*itVars).first, (*itVars).second );				
				    }

					cHashPre[ c ] = lp->getNumRows();
					lp->addRow( row );
					numRest++;
				}
				else
				{				   
				   GGroup< std::pair<int,double> >::iterator itVars = (*pares).begin();
				   for ( ; itVars != (*pares).end(); itVars++ )
				   {
					   idxC.push_back( (*itVars).first );
					   idxR.push_back( cit->second );
					   valC.push_back( (*itVars).second );
				   }
				}
		   }
	   }
   }
   
   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}


// Restricao 1.12
int PreModelo::cria_preRestricao_disponib_profs( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   ConstraintPreHash::iterator cit;
   VariablePreHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   Campus *campus = problemData->refCampus[campusId];

	std::map< Disciplina*, std::map<int /*dia*/, std::map< DateTime, 
		GGroup<Professor*, LessPtr<Professor>> >>, LessPtr<Disciplina> >	mapDiscDiaHorProfs;

    GGroup< Professor *, LessPtr< Professor > > professores = problemData->getProfessores();
    ITERA_GGROUP_LESSPTR( itProf, professores, Professor )
    {
		Professor *prof = *itProf;

		ITERA_GGROUP_LESSPTR( itHorDia, prof->horariosDia, HorarioDia )
		{
			int dia = itHorDia->getDia();
			HorarioAula *horAula = itHorDia->getHorarioAula();
					
			ITERA_GGROUP_LESSPTR( itMagist, itProf->magisterio, Magisterio )
			{
				Disciplina *disciplina = itMagist->disciplina;
				mapDiscDiaHorProfs[disciplina][dia][horAula->getInicio()].add(prof);;
			}
		}
	}
	
   std::map< Disciplina*, std::map<int /*turno*/, int /*nHorsProfs*/>, LessPtr<Disciplina> > mapDiscTurnoN;

	std::map< Disciplina*, std::map<int /*dia*/, std::map< DateTime, 
		GGroup<Professor*, LessPtr<Professor>> >>, LessPtr<Disciplina> >::iterator 
		itMap1 = mapDiscDiaHorProfs.begin();
	for ( ; itMap1 != mapDiscDiaHorProfs.end(); itMap1++ )
	{
		Disciplina* disciplina = itMap1->first;

		std::map<int /*dia*/, std::map< DateTime, 
			GGroup<Professor*, LessPtr<Professor>> >> *map2 = & itMap1->second;

		std::map<int /*dia*/, std::map< DateTime, 
			GGroup<Professor*, LessPtr<Professor>> >>::iterator 
			itMap2 = (*map2).begin();
		for ( ; itMap2 != (*map2).end(); itMap2++ )
		{
			std::map< DateTime, GGroup<Professor*, LessPtr<Professor>> > *map3 = & itMap2->second;

			std::map< DateTime, GGroup<Professor*, LessPtr<Professor>> >::iterator
				itMap3 = (*map3).begin();
			for ( ; itMap3 != (*map3).end(); itMap3++ )
			{
				int turno = problemData->getFaseDoDia( itMap3->first );

				mapDiscTurnoN[disciplina];
				if ( mapDiscTurnoN[disciplina].find(turno) == mapDiscTurnoN[disciplina].end() )
					mapDiscTurnoN[disciplina][turno] = itMap3->second.size();
				else
					mapDiscTurnoN[disciplina][turno] += itMap3->second.size();
			}
		}
	}


	vit = vHashPre.begin();
	for ( ; vit != vHashPre.end(); vit++)
	{		
		VariablePre v = vit->first;
				
		double coef = 0.0;
		if( v.getType() == VariablePre::V_PRE_OFERECIMENTO_TURNO )
		{
			coef = v.getDisciplina()->getTotalCreditos();
			if ( v.getSubCjtSala()->getCampusId() != campusId ) continue;
		}
		else if( v.getType() == VariablePre::V_PRE_SLACK_PROF )
		{
			coef = -1.0;
		}
		else 
		{
			continue;
		}
		
		c.reset();
        c.setType( ConstraintPre::C_PRE_DISPONIB_PROF );
		c.setDisciplina( v.getDisciplina() );
		c.setTurno( v.getTurno() );

		cit = cHashPre.find(c);
		if ( cit == cHashPre.end() )
		{
			double rhs = 0.0;
			
			if ( mapDiscTurnoN.find( v.getDisciplina() ) != mapDiscTurnoN.end() );
				if ( mapDiscTurnoN[v.getDisciplina()].find(v.getTurno()) != mapDiscTurnoN[v.getDisciplina()].end() )
					rhs = mapDiscTurnoN[v.getDisciplina()][v.getTurno()];


			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, rhs, name );

			row.insert( vit->second, coef);

			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   
   return restricoes;

}


/*
	Usada para caso de teste. Força um numero minimo de atendimentos AlunoDemanda,
	para verificar se há inviabilidade no número possível total de atendimentos.
*/
int PreModelo::cria_preRestricao_forca_min_atendimento( int campusId, int cjtAlunosId )
{
	int numRest=0;
    char name[ 100 ];

#ifndef UNIRITTER
	return numRest;
#endif

	VariablePre v;
	ConstraintPre c;
	VariablePreHash::iterator vit;
	ConstraintPreHash::iterator cit;

	vit = vHashPre.begin();

	for ( ; vit != vHashPre.end(); vit++ )
	{
		v = vit->first;
	
		if( v.getType() == VariablePre::V_PRE_SLACK_DEMANDA )
		{	
			// Só conta disciplinas teoricas
			int discId = v.getDisciplina()->getId();
			if ( discId < 0 )
			{
				continue;
			}
		
			c.reset();
			c.setType( ConstraintPre::C_PRE_FORCA_MIN_ATEND );
			
			double coef = 1.0;
			
			cit = cHashPre.find(c);
			if(cit != cHashPre.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz = 17149;

				sprintf( name, "%s", c.toString().c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 1149.0, name );

				row.insert(vit->second, coef);

				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				numRest++;
			}
		}
	}

	return numRest;
}

