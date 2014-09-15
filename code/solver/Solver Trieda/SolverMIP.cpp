#include "SolverMIP.h"

#include <math.h>
#include <hash_set>

#include "SolverTaticoHeur\SolverTaticoHeur.h"
#include "SolverTaticoHeur\GUtil.h"

#include "NaoAtendimento.h"

#include "AtendimentoHorarioAula.h"
#include "AtendimentoTurno.h"

#ifdef SOLVER_CPLEX
#include "opt_cplex.cpp"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.cpp"
#endif

using namespace std;

#define MODELO_INTEGRADO_P2
#ifndef MODELO_INTEGRADO_P2
#define HEURISTICA_P2
#endif

bool CRIA_NOVAS_VARIAVEIS_TAT_HOR2 = false;

bool ETAPA2_ASSOCIA_ALUNOS=false;


#pragma region DOC
/*==================================================================/
%DocBegin TRIEDA_LOAD_MODEL
%Title Módulo Tático

%ProbSense MIN

%Set CP 
%Desc 
Conjunto de campus. Os elementos desse conjunto são denotados por $cp$.

%Set U
%Desc 
Conjunto de unidades. Os elementos desse conjunto são denotados por $u$.

%Set S_{u}
%Desc 
Conjunto de salas da unidade $u$. Os elementos desse conjunto são 
denotados por $s$.

%Set SCAP_{u}
%Desc 
Conjunto de salas da unidade $u$ classificadas de acordo com as suas 
capacidades. Os elementos desse conjunto são denotados por $tps$.

%Set T 
%Desc 
Conjunto de dias letivos da semana. Os elementos desse conjunto 
são denotados por $t$.

%Set C
%Desc 
Conjunto de cursos. Os elementos desse conjunto são denotados por $c$.

%Set CC
%Desc 
Conjunto de cursos compatíveis.

%Set D
%Desc 
Conjunto de disciplinas. Os elementos desse conjunto são denotados por $d$.

%Set B
%Desc 
Conjunto de blocos curriculares. Os elementos desse conjunto são 
denotados por $bc$.

%Set D_{bc}
%Desc 
Conjunto de disciplinas que pertencem ao bloco curricular $bc$. 
Os elementos desse conjunto são denotados por $d_{bc}$.

%Set I_{bc}
%Desc 
Conjunto de turmas de um bloco curricular $bc$. 
Os elementos desse conjunto são denotados por $i_{bc}$.

%Set O
%Desc 
Conjunto de ofertas de cursos. Os elementos desse conjunto são 
denotados por $oft$.

%Set D_{oft}
%Desc 
Conjunto de disciplinas de uma oferta $oft$. 
Os elementos desse conjunto são denotados por $d_{oft}$.

%Set O_{d}
%Desc 
Conjunto de ofertas de uma uma disciplina $d$. 
Os elementos desse conjunto são denotados por $oft_{d}$.

%Set K_{d}
%Desc 
Conjunto de combinações possíveis de divisão de créditos de uma uma disciplina $d$. 
Os elementos desse conjunto são denotados por $k$.

%DocEnd
/===================================================================*/

/*==================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Data A_{u,s}
%Desc
capacidade da sala $s$ da unidade $u$.


%Data C_{d}
%Desc
Total de créditos da disciplina $d$.

%Data \overline{H_{d}}
%Desc 
máximo de créditos diários da disciplina $d$.

%Data \underline{H_{d}}
%Desc 
mínimo de créditos diários da disciplina $d$.

%Data I_{d}
%Desc 
máximo de turmas que podem ser abertas da disciplina $d$.

%Data P_{d,c,cp}
%Desc 
demanda da disciplina $d$ no campus $cp$ para o curso $c$.

%Data P_{d,oft}
%Desc
demanda da disciplina $d$ da oferta $oft$.

%Data Pmax_{d}
%Desc 
maior demanda da disciplina $d$.

%Data H_{t}
%Desc
máximo de créditos permitidos por dia $t$.

%Data A_{u,s}
%Desc 
capacidade da sala $s$ da unidade $u$.

%Data HTPS_{t,tps}
%Desc 
máximo de créditos permitidos por dia $t$ para o conjunto de salas do tipo (capacidade) $tps$.

%Data A_{u,tps}
%Desc 
capacidade total das salas de um conjunto de salas do tipo (capacidade) $tps$ da unidade $u$.

%Data O_{cp}
%Desc
conjunto de ofertas de um campus $cp$.

%Data FC_{d,t}
%Desc 
número de créditos fixados para a disciplina $d$ no dia $t$.

%Data N_{d,k,t}
%Desc 
número de créditos determinados para a disciplina $d$ no dia $t$ na combinação de divisão de crédito $k$.

%Data M
%Desc 
big $M$.

%Data \alpha
%Desc 
peso associado a função objetivo.
%Data \beta
%Desc 
peso associado a função objetivo.
%Data \gamma
%Desc 
peso associado a função objetivo.
%Data \delta
%Desc 
peso associado a função objetivo.
%Data \lambda
%Desc 
peso associado a função objetivo.
%Data \rho
%Desc 
peso associado a função objetivo.
%Data \xi
%Desc 
pesos associados a cada item da função objetivo.
%Data \psi
%Desc 
peso associado a função objetivo.
%Data \theta
%Desc 
peso associado a função objetivo.
%Data \omega
%Desc 
peso associado a função objetivo.
%Data \tau
%Desc 
peso associado a função objetivo.
%Data \eta
%Desc 
peso associado a função objetivo.

%DocEnd
/===================================================================*/
#pragma endregion


#ifndef WIN32
struct ordenaDiscSalas {
   bool operator()(const std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & left,
                   const std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & right) const
   {
      return ( left.second < right.second );
   }
};
#else
bool ordenaDiscSalas( std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & left,
                      std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & right )
{
   return ( left.second < right.second );
}
#endif

bool ordenaVarsX( Variable * left, Variable * right )
{
   if ( left->getUnidade() > right->getUnidade() )
   {
      return false;
   }
   else
   {
      if ( left->getTurma() > right->getTurma() )
      {
         return false;
      }
      else
      {
         if ( left->getDia() > right->getDia() )
         {
            return false;
         }
      }
   }

   return true;
}


// comparison function.
bool compare_dif_func ( std::pair< AlunoDemanda*, int > first, std::pair< AlunoDemanda*, int > second )
{
  if ( abs(first.second) < abs(second.second) )
	  return true;
  else if ( abs(first.second) > abs(second.second) )
	  return false;
  else if ( first.second < second.second )
	  return true;
  else if ( first.second > second.second )
	  return false;
  else if ( abs( first.first->demanda->getDisciplinaId() ) == abs( second.first->demanda->getDisciplinaId() ) )
  {	  
	  // first/second são um par de pratica-teorica
	  if ( first.first->getId() < second.first->getId() )
		 return true;
	  else
		  return false;
  }
  else if ( first.first->demanda->getDisciplinaId() < second.first->demanda->getDisciplinaId() )
	  return true;
  else
	  return false;
}


SolverMIP::SolverMIP( ProblemData * aProblemData,
  ProblemSolution * _ProblemSolution, ProblemDataLoader * _problemDataLoader, bool solucaoHeuristica )
   : Solver( aProblemData )
{

#ifdef USA_ESTIMA_TURMAS
   MODELO_ESTIMA_TURMAS = true;
#else
   MODELO_ESTIMA_TURMAS = false;
#endif

if ( CRIA_NOVAS_VARIAVEIS_TAT_HOR2 )
	std::cout << "\nCRIA_NOVAS_VARIAVEIS_TAT_HOR2";
if ( ETAPA2_ASSOCIA_ALUNOS )
	std::cout << "\nETAPA2_ASSOCIA_ALUNOS";
if ( MODELO_ESTIMA_TURMAS )
	std::cout << "\nMODELO_ESTIMA_TURMAS";

   problemSolution = _ProblemSolution;
   problemDataLoader = _problemDataLoader;

   usaSolInicialHeur = solucaoHeuristica;

   if ( solucaoHeuristica )
   {
	   carregaSolucaoHeuristicaInicial();
   }

   alpha = 5.0;
   beta = 10.0;
   gamma = 0;
   delta = 1.0;
   lambda = 10.0;
   epsilon = 1.0;
  // M = 1.0;
   rho = 5;

   // Verificar o valor
   psi = 5.0;
   tau = 1.0;

   TEMPO_TATICO = 3600;//450;

#ifdef TATICO_CJT_ALUNOS
   NAO_CRIAR_RESTRICOES_CJT_ANTERIORES = true;
   FIXAR_P1 = true;
   FIXAR_TATICO_P1 = true;
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

SolverMIP::~SolverMIP()
{
   if ( lp != NULL )
   {
      delete lp;
   }

	// -----------------------------------------------------
	// Deleta as variaveis em solVarsXPre 
	ITERA_GGROUP_LESSPTR ( it, solVarsXPre, VariablePre )
	{
		delete *it;    
	}
	solVarsXPre.clear();

}


int SolverMIP::retornaTempoDeExecucaoTatico( int campusId, int cjtAlunosId, int prioridade )
{
	if ( campusId == 19 ) // campus 2, mais dificil
	{
		if ( prioridade == 1 )
		{
			if ( cjtAlunosId == 1 )
			{
				return TEMPO_TATICO*7;
			}
			else if ( cjtAlunosId == 2 )
			{
				return TEMPO_TATICO*6;
			}
			else
			{
				return TEMPO_TATICO*4;
			}
		}
		else // prioridade 2 em diante
		{
			if ( cjtAlunosId == 1 )
			{
				return TEMPO_TATICO*6;
			}
			else
			{
				return TEMPO_TATICO;
			}		
		}
	}
	else if ( campusId == 20 ) // campus 28, mais facil
	{
		if ( prioridade == 1 )
		{
			if ( cjtAlunosId == 1 )
			{
				return TEMPO_TATICO*5;
			}
			else if ( cjtAlunosId == 2 )
			{
				return TEMPO_TATICO*4;
			}
			else
			{
				return TEMPO_TATICO*3;
			}
		}
		else // prioridade 2 em diante
		{
			if ( cjtAlunosId == 1 )
			{
				return TEMPO_TATICO*6;
			}
			else
			{
				return TEMPO_TATICO;
			}		
		}
	}
	else // geral
	{
		return TEMPO_TATICO;	
	}
}

std::string SolverMIP::getTaticoLpFileName( int campusId, int prioridade, int cjtAlunosId, int r, int tatico )
{
	std::string solName;
	solName += "SolverTrieda";
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
 
   if ( tatico != 0 )
   {
		stringstream ss;
		ss << tatico;
		solName += "_T"; 
		solName += ss.str();   		
   }        
   return solName;
}

std::string SolverMIP::getSolHeuristBinFileName( int campusId, int prioridade, int cjtAlunosId )
{
   std::string solName;
	solName += "solHeuristBin";
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

   solName += ".bin";
      
   return solName;
}

std::string SolverMIP::getSolBinFileName( int campusId, int prioridade, int cjtAlunosId, int r, int tatico )
{
	std::string solName;
	solName += "solBin";
	solName +=problemData->inputIdToString();
   
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
   if ( tatico != 0 )
   {
		stringstream ss;
		ss << tatico;
		solName += "_T"; 
		solName += ss.str();   		
   }
   solName += ".bin";
      
   return solName;
}

std::string SolverMIP::getSolucaoTaticoFileName( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int fase )
{
	std::string solName;
	solName += "solucaoTatico";
	solName +=problemData->inputIdToString();
   
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
   if ( tatico != 0 )
   {
		stringstream ss;
		ss << tatico;
		solName += "_T"; 
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

std::string SolverMIP::getFixaNaoAtendFileName( int campusId, int prioridade, int cjtAlunosId, int r )
{
	std::string solName;
	solName += "fixaNaoAtendPre";
	solName +=problemData->inputIdToString();
      
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


std::string SolverMIP::getAulasSolInitFileName( int campusId  )
{
	string fileName("aulasSolInicial");
      
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 fileName += "_Cp"; 
		 fileName += ss.str();
   }
   
    fileName += "_";
	fileName += problemData->inputIdToString();
	fileName += "_";
	fileName += problemData->getInputFileName();
    fileName += ".txt";
    return fileName;
}

std::string SolverMIP::getSolXInitFileName( int campusId )
{
	string fileName("solInicialX");  

   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 fileName += "_Cp"; 
		 fileName += ss.str();
   }
   
    fileName += "_";
	fileName += problemData->inputIdToString();
	fileName += "_";
	fileName += problemData->getInputFileName();
    fileName += ".txt";
    return fileName;
}

std::string SolverMIP::getSolSInitFileName( int campusId )
{
	string fileName("solInicialS");      

   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 fileName += "_Cp"; 
		 fileName += ss.str();
   }
   
    fileName += "_";
	fileName += problemData->inputIdToString();
	fileName += "_";
	fileName += problemData->getInputFileName();
    fileName += ".txt";
    return fileName;
}

std::string SolverMIP::getSolNaoAtendInitFileName( int campusId )
{
	string fileName("solInicialNaoAtend");      

   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 fileName += "_Cp"; 
		 fileName += ss.str();
   }
   
    fileName += "_";
	fileName += problemData->inputIdToString();
	fileName += "_";
	fileName += problemData->getInputFileName();
    fileName += ".txt";
    return fileName;
}

void SolverMIP::writeSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type, double *xSol )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (TAT_HOR_BIN):
			strcpy( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );			
			break;
		case (TAT_HOR_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
			break;
		case (TAT_HOR_BIN2):
			strcpy( solName, "2" );
			strcat( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
			break;
		case (TAT_HOR_BIN3):
			strcpy( solName, "3" );
			strcat( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
			break;
		case (TAT_HOR_CALOURO_BIN):
			strcpy( solName, "calouros" );
			strcat( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	FILE * fout = fopen( solName, "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em SolverMIP::writeSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type ):"
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

void SolverMIP::writeSolBinAux( char *fileName,double *xSol )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	// WRITES SOLUTION
		
	FILE * fout = fopen( fileName, "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em SolverMIP::writeSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type ):"
				<< "\nArquivo " << fileName << " nao pode ser aberto.\n";
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

int SolverMIP::readSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type, double *xSol )
{
	char solName[1024]="\0";

	switch (type)
	{
		case (TAT_HOR_BIN):
			strcpy( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );			
			break;
		case (TAT_HOR_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
			break;
		case (TAT_HOR_BIN2):
			strcpy( solName, "2" );
			strcat( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
			break;
		case (TAT_HOR_BIN3):
			strcpy( solName, "3" );
			strcat( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
			break;
		case (TAT_HOR_CALOURO_BIN):
			strcpy( solName, "calouros" );
			strcat( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
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

int SolverMIP::readSolBinAux(char *fileName,double *xSol)
{
	// READS THE SOLUTION
		
	cout<<"====================> carregando solucao auxiliar" <<fileName <<endl; fflush(NULL);
	FILE* fin = fopen( fileName,"rb");

	if ( fin == NULL )
	{
		std::cout << "<============ Arquivo " << fileName << " nao encontrado. Fim do carregamento de solucao.\n\n"; fflush(NULL);
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

int SolverMIP::readSolTxtTat( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type, double *xSol, int fase )
{
   char solName[1024]="\0";

	switch (type)
	{
		case (TAT_HOR_BIN):
			strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );			
			break;
		case (TAT_HOR_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );
			break;
		case (TAT_HOR_BIN2):
			strcpy( solName, "2" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );
			break;
		case (TAT_HOR_BIN3):
			strcpy( solName, "3" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );
			break;
		case (TAT_HOR_CALOURO_BIN):
			strcpy( solName, "calouros" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );
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

      VariableTaticoHash::iterator vit = vHashTatico.begin();
      while ( vit != vHashTatico.end() )
      {
         VariableTatico v = vit->first;
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


void SolverMIP::writeSolTxt( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type, double *xSol, int fase )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (TAT_HOR_BIN):
			strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );			
			break;
		case (TAT_HOR_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );
			break;
		case (TAT_HOR_BIN2):
			strcpy( solName, "2" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );
			break;
		case (TAT_HOR_BIN3):
			strcpy( solName, "3" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );
			break;
		case (TAT_HOR_CALOURO_BIN):
			strcpy( solName, "calouros" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, fase ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	ofstream fout( solName, ios_base::out );
	if ( fout == NULL )
	{
		std::cout << "\nErro em SolverMIP::writeSolTxt( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{		
		int nAtend = 0, nAtendSemPT = 0;

		VariableTaticoHash::iterator vit = vHashTatico.begin();
		while ( vit != vHashTatico.end() )
		{
				VariableTatico v = vit->first;
				int col = vit->second;
				double value = (int)( xSol[ col ] + 0.5 );
		  
				fout << v.toString() << " = " << value << endl;
				  
				if ( v.getType() == VariableTatico::V_ABERTURA )
				{
					nAtend += value;
					if ( v.getDisciplina()->getId() > 0 )
					nAtendSemPT += value;
				}				  

				vit++;
		}
		fout << "\n\nTotal de turmas atendidas = " << nAtend << endl;
		fout << "\n\nTotal de turmas atendidas sem divisao PT = " << nAtendSemPT << endl;

		fout.close();
	}
}


bool SolverMIP::turmaAtendida( int turma, Disciplina *disciplina, Campus* campus )
{		
	VariableTatico *v = new VariableTatico();
	v->reset();
    v->setType( VariableTatico::V_ABERTURA );
    v->setTurma( turma );				// i
    v->setDisciplina( disciplina );		// d
    v->setCampus( campus );				// cp

	GGroup< VariableTatico *, LessPtr<VariableTatico> >::iterator itSol = solVarsTatico.find(v);

	delete v;

	if(itSol != solVarsTatico.end())
		return true;
	return false;
}

bool SolverMIP::SolVarsPreFound( VariablePre v )
{		
	GGroup< VariablePre *, LessPtr<VariablePre> >::iterator itPreSol = solVarsXPre.find(&v);

	if(itPreSol != solVarsXPre.end())
		return true;
	else
		return false;
}

bool SolverMIP::criaVariavelTatico_Anterior( VariableTatico *v )
{
	// pesquisa se existe outra x com a turma e disciplina (dia diferente por ex).
	// se existir, não cria a variavel.
	// se não existir, então é pq é turma nova. Cria.

	if ( v->getType() == VariableTatico::V_CREDITOS )  //  x_{i,d,u,s,hi,hf,t} 
	{
		bool turmaExiste=false;
		ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
		{
			VariableTatico *vSol = *it_Vars_x;
			if ( vSol->getTurma() == v->getTurma() &&
				 vSol->getDisciplina() == v->getDisciplina() && 
				 vSol->getUnidade()->getIdCampus() == v->getUnidade()->getIdCampus() )
			{				
				turmaExiste=true;

				if ( vSol->getUnidade() == v->getUnidade() &&
				     vSol->getSubCjtSala() == v->getSubCjtSala() )
				{
					if ( vSol->getDia() == v->getDia() &&
						vSol->getHorarioAulaInicial() == v->getHorarioAulaInicial() &&
						vSol->getHorarioAulaFinal() == v->getHorarioAulaFinal() )
					{
						return true;
					}
				}
				else
				{	// sala diferente, não pode!
					return false;
				}
			}
		}

		if ( turmaExiste ) 
			return false;

		// turma nova. Não existe em iteração tática anterior.
		return true;				
	}

	return true;
}

bool SolverMIP::criaVariavelTatico( VariableTatico *v )
{

#ifndef SALA_UNICA_POR_TURMA
	VariablePre::VariableType type = VariablePre::V_PRE_CREDITOS;
#else
	VariablePre::VariableType type = VariablePre::V_PRE_OFERECIMENTO;
#endif

	VariablePre preV;
		   
	switch( v->getType() )
	{
		
		 case VariableTatico::V_ERROR:
		 {
			 return true;
		 }
		 case VariableTatico::V_CREDITOS:  //  x_{i,d,u,s,hi,hf,t} 
		 {
			 preV.reset();
			 preV.setType( type ); // x_{i,d,u,s}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );
			 preV.setUnidade( v->getUnidade() );
			 preV.setSubCjtSala( v->getSubCjtSala() );

			 if ( SolVarsPreFound(preV) )
				return true;
			 else
				return false;
		 }
		case VariableTatico::V_ABERTURA: // z_{i,d,cp}
		 {
			 // z_{i,d,cp}
			 if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
				return true;
			 
			 return false;

			 //preV.reset();
			 //preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 //preV.setTurma( v->getTurma() );
			 //preV.setDisciplina( v->getDisciplina() );
			 //preV.setCampus( v->getCampus() );
		 	//	 
			 //if ( SolVarsPreFound(preV) )
				//return true;
			 //else
				//return false;
		 }
		 case VariableTatico::V_DIAS_CONSECUTIVOS: // c_{i,d,t}
		 {
			 // z_{i,d,cp}
			 if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
				return true;
			 
			 return false;

			 //preV.reset();
			 //preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 //preV.setTurma( v->getTurma() );
			 //preV.setDisciplina( v->getDisciplina() );
			 //preV.setCampus( v->getCampus() );		 // cp

			 //if ( SolVarsPreFound( preV ) )
				// return true;
			 //else
				//return false;
		 }
		 case VariableTatico::V_MAX_CRED_SEMANA: // H_{a}
		 {
			 Aluno *aluno = v->getAluno();
	
			 if ( problemData->mapAluno_CampusTurmaDisc.find(aluno) !=
				  problemData->mapAluno_CampusTurmaDisc.end() )
			 {
				 if ( problemData->mapAluno_CampusTurmaDisc[aluno].size() > 0 )
					return true;			
			 }

			 return false;
		 }
		 case VariableTatico::V_MIN_CRED_SEMANA: // h_{a}
		 {
			 Aluno *aluno = v->getAluno();
	
			 if ( problemData->mapAluno_CampusTurmaDisc.find(aluno) !=
				  problemData->mapAluno_CampusTurmaDisc.end() )
			 {
				 if ( problemData->mapAluno_CampusTurmaDisc[aluno].size() > 0 )
					return true;			
			 }

			 return false;
		 }
		 case VariableTatico::V_ALUNO_UNID_DIA:// y_{a,u,t}
		 {
			 Aluno *aluno = v->getAluno();
			 Unidade *unid = v->getUnidade();
			 Campus *cp = problemData->retornaCampus( unid->getId() );
	
			 if ( problemData->mapAluno_CampusTurmaDisc.find(aluno) !=
				  problemData->mapAluno_CampusTurmaDisc.end() )
			 {
				 GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator
					 itTrio = problemData->mapAluno_CampusTurmaDisc[aluno].begin();

				 for ( ; itTrio != problemData->mapAluno_CampusTurmaDisc[aluno].end(); itTrio++ )
				 {
					 if ( (*itTrio).first != cp->getId() ) continue;

					 int turma = (*itTrio).second;
					 Disciplina *disc = (*itTrio).third;
					 
					 ITERA_GGROUP_LESSPTR( itCjtSala, unid->conjutoSalas, ConjuntoSala )
					 {
						preV.reset();
						preV.setType( type ); //x_{i,d,u,s}
						preV.setTurma( turma );
						preV.setDisciplina( disc );
						preV.setUnidade( unid );
						preV.setSubCjtSala( *itCjtSala );

						if ( SolVarsPreFound( preV ) )
							return true;
					 }					
				 }						 
			 }
			 return false;
		 }
		 case VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR: // fcp_{i,d,s,t}
		 {
			 preV.reset();
			 preV.setType( type ); // x_{i,d,u,s}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );
			 preV.setUnidade( v->getUnidade() );
			 preV.setSubCjtSala( v->getSubCjtSala() );

			 if ( SolVarsPreFound(preV) )
				return true;
			 else
				return false;
		 }
		 case VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR: // fcm_{i,d,s,t}
		 {
			 preV.reset();
			 preV.setType( type ); // x_{i,d,u,s}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );
			 preV.setUnidade( v->getUnidade() );
			 preV.setSubCjtSala( v->getSubCjtSala() );

			 if ( SolVarsPreFound(preV) )
				return true;
			 else
				return false;
		 }
		 case VariableTatico::V_SLACK_DEMANDA: // fd_{i,d,cp}
		 {	
			 // Só cria se a turma existir, criando a possibilidade de desaloca-la, caso não seja fixada depois.
			 if ( !problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
			 	return false;

			 return true;			
		 }
		 case VariableTatico::V_SLACK_SLACKDEMANDA_PT: // ffd_{i,d,i',d',cp}
		 {	
			 // Só cria se as duas turmas existirem, criando a possibilidade de desaloca-las, caso não sejam fixadas depois.
			 if ( !problemData->existeTurmaDiscCampus( v->getTurma1(), v->getDisciplina1()->getId(), v->getCampus()->getId() ) )
				return false;
			 if ( !problemData->existeTurmaDiscCampus( v->getTurma2(), v->getDisciplina2()->getId(), v->getCampus()->getId() ) )
			 	return false;

			 return true;			
		 }
		 case VariableTatico::V_COMBINACAO_DIVISAO_CREDITO: // m{i,d,k}
		 {
			 // z_{i,d,cp}
			 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 {
				 int cpId = (*itCp)->getId();
				 if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), cpId ) )
				 	return true;
			 }			 
			 return false;

			 //preV.reset();
			 //preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 //preV.setTurma( v->getTurma() );
			 //preV.setDisciplina( v->getDisciplina() );

			 //ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 //{
				// preV.setCampus( *itCp );
				// if ( SolVarsPreFound( preV ) )
				//	 return true;
			 //}
			 //return false;
		 }
		 case VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M: // fkm{i,d,k}
		 {
			 // z_{i,d,cp}
			 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 {
				 int cpId = (*itCp)->getId();
				 if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), cpId ) )
				 	return true;
			 }			 
			 return false;

			 //preV.reset();
			 //preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 //preV.setTurma( v->getTurma() );
			 //preV.setDisciplina( v->getDisciplina() );

			 //ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 //{
				// preV.setCampus( *itCp );
				// if ( SolVarsPreFound( preV ) )
				//	 return true;
			 //}
			 //return false;
		 }
		 case VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P: // fkp{i,d,t}
		 {
			 // z_{i,d,cp}
			 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 {
				 int cpId = (*itCp)->getId();
				 if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), cpId ) )
				 	return true;
			 }			 
			 return false;

			 //preV.reset();
			 //preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 //preV.setTurma( v->getTurma() );
			 //preV.setDisciplina( v->getDisciplina() );

			 //ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 //{
				// preV.setCampus( *itCp );
				// if ( SolVarsPreFound( preV ) )
				//	 return true;
			 //}
			 //return false;
		 }
		 case VariableTatico::V_ABERTURA_COMPATIVEL: // zc_{d,t,cp}
		 {
			 // z_{i,d,cp}
			 for ( int turma = 1; turma <= v->getDisciplina()->getNumTurmas(); turma++ )
			 {
				 if ( problemData->existeTurmaDiscCampus( turma, v->getDisciplina()->getId(), v->getCampus()->getId() ) )
				 	return true;
			 }			 
			 return false;

			 //preV.reset();
			 //preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 //preV.setDisciplina( v->getDisciplina() );
			 //preV.setCampus( v->getCampus() );

			 //for ( int turma = 0; turma < v->getDisciplina()->getNumTurmas(); turma++ )
			 //{
				//preV.setTurma( turma );
				//if	( SolVarsPreFound( preV ) )
				//	return true;
			 //}			 
			 //return false;
		 }		 		 
		 case VariableTatico::V_ALUNO_VARIAS_UNID_DIA: // w_{a,t}
		 {
			 Aluno *aluno = v->getAluno();
	
			 if ( problemData->mapAluno_CampusTurmaDisc.find(aluno) !=
				  problemData->mapAluno_CampusTurmaDisc.end() )
			 {
				 if ( problemData->mapAluno_CampusTurmaDisc[aluno].size() > 0 )
					return true;			
			 }

			 return false;
		 }
		
		 default:
		 {
			return true;
			break;
		 }
	}
	
}

GGroup< VariableTatico*, LessPtr<VariableTatico> > SolverMIP::retornaVariableTaticoCreditosAnterior( int turma, Disciplina* disciplina, Campus* campus )
{	
	GGroup< VariableTatico*, LessPtr<VariableTatico> > variaveis;
	VariableTatico v;

	v.reset();
	v.setType( VariableTatico::V_CREDITOS ); // x_{i,d,u,s,hi,hf,t}
	v.setTurma( turma );
	v.setDisciplina( disciplina );

	ITERA_GGROUP_LESSPTR( itUnid, campus->unidades, Unidade )
	{
		v.setUnidade( *itUnid );

		ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
		{			
			v.setSubCjtSala( (*itCjtSala) );
			Sala* s = (*itCjtSala)->salas.begin()->second;
			v.setSala( s );

			ITERA_GGROUP_N_PT( itDia, disciplina->diasLetivos, int )
			{
				v.setDia( *itDia );
				
				ITERA_GGROUP( itHorInic, s->horarios_disponiveis, Horario )
				{			
					v.setHorarioAulaInicial( (*itHorInic)->horario_aula );

					ITERA_GGROUP( itHorFinal, s->horarios_disponiveis, Horario )
					{			
						v.setHorarioAulaFinal( (*itHorFinal)->horario_aula );		

						GGroup< VariableTatico*, LessPtr<VariableTatico> >::iterator
							it = solVarsTatico.find( &v );
						if ( it != solVarsTatico.end() )
						{
							variaveis.add( *it );
						}
					}
				}
			}
		}
	}
	
	return variaveis;
}

Unidade* SolverMIP::retornaUnidadeDeAtendimento( int turma, Disciplina* disciplina, Campus* campus )
{
	Unidade *unid = NULL;
	
#ifndef SALA_UNICA_POR_TURMA
	VariablePre::VariableType TYPE = VariablePre::V_PRE_CREDITOS;
#else
	VariablePre::VariableType TYPE = VariablePre::V_PRE_OFERECIMENTO;
#endif

	VariablePre preV;		   
	preV.reset();
	preV.setType( TYPE ); // x_{i,d,u,s}
	preV.setTurma( turma );
	preV.setDisciplina( disciplina );

	ITERA_GGROUP_LESSPTR( itUnid, campus->unidades, Unidade )
	{
		unid = *itUnid;
		preV.setUnidade( unid );

		ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
		{
			preV.setSubCjtSala( *itCjtSala );

			if ( SolVarsPreFound(preV) )
				return unid;
		}
	}
	
	return NULL;
}

ConjuntoSala* SolverMIP::retornaSalaDeAtendimento( int turma, Disciplina* disciplina, Campus* campus )
{
	ConjuntoSala *s = NULL;

#ifndef SALA_UNICA_POR_TURMA
	VariablePre::VariableType TYPE = VariablePre::V_PRE_CREDITOS;
#else
	VariablePre::VariableType TYPE = VariablePre::V_PRE_OFERECIMENTO;
#endif

	VariablePre preV;		   
	preV.reset();
	preV.setType( TYPE ); // x_{i,d,u,s}
	preV.setTurma( turma );
	preV.setDisciplina( disciplina );

	ITERA_GGROUP_LESSPTR( itUnid, campus->unidades, Unidade )
	{
		preV.setUnidade( *itUnid );

		ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
		{
			s = *itCjtSala;
			preV.setSubCjtSala( s );

			if ( SolVarsPreFound(preV) )
				return s;
		}
	}
	
	return NULL;
}



GGroup< std::pair< int,Disciplina* > > SolverMIP::retornaAtendEmCjtSala( ConjuntoSala * cjtSala )
{
	GGroup< std::pair<int,Disciplina*>  > turmaDiscsAtendidas;
		
#ifndef SALA_UNICA_POR_TURMA
	VariablePre::VariableType type = VariablePre::V_PRE_CREDITOS;
#else
	VariablePre::VariableType type = VariablePre::V_PRE_OFERECIMENTO;
#endif

	ITERA_GGROUP_LESSPTR ( it, solVarsXPre, VariablePre )
	{
		VariablePre *var = *it;
		if ( var->getType() == type )
		{
			ConjuntoSala *subCjtSala = var->getSubCjtSala();

			if ( subCjtSala == cjtSala )
			{
				int turma = var->getTurma();
				Disciplina * disc = var->getDisciplina();

				std::pair< int, Disciplina* > par = std::make_pair( turma, disc );

				turmaDiscsAtendidas.add( par );
			}
		}
	}

	return turmaDiscsAtendidas;
}




void SolverMIP::limpaMapAtendimentoAlunoPrioridadeAnterior( int campusId )
{
	// Limpa a solução obtida na iteração de PRIORIDADE de demanda anterior, caso exista, do campusId

	std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator
		itMapAtendeAluno =	problemData->mapAluno_CampusTurmaDisc.begin();

	while ( itMapAtendeAluno != problemData->mapAluno_CampusTurmaDisc.end() )
	{
		Aluno *a = itMapAtendeAluno->first;
		if ( a->hasCampusId(campusId) )
		{
			GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator
			itMapTurmas = itMapAtendeAluno->second.begin();

			for ( ; itMapTurmas != itMapAtendeAluno->second.end(); itMapTurmas++ )
			{
				Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio = *itMapTurmas;
				if ( trio.first == campusId )
					problemData->mapCampusTurmaDisc_AlunosDemanda.erase( trio );
			}
			if ( problemData->mapAluno_CampusTurmaDisc[a].size() == 0 ){
				problemData->mapAluno_CampusTurmaDisc.erase( a );
				itMapAtendeAluno = problemData->mapAluno_CampusTurmaDisc.begin();
			}else itMapAtendeAluno++;
		}
		else
			itMapAtendeAluno++;
	}
}

int SolverMIP::removeAtendimentosParciais( double *xSol, char solFilename[1024], int prioridade )
{	
	int nroAtendsRemovidosAlunoDemanda = 0;
	
	GGroup< Trio< int, int, Disciplina* > > remocao; // campusId, turma, disc

	// -----------------------------------------------------------------------------
	// Retira dos maps mapAluno_CampusTurmaDisc e mapCampusTurmaDisc_AlunosDemanda
	// os atendimentos incompletos ( atendeu só disc pratica ou só teorica ), e os
	// acrescenta em listSlackDemandaAluno.
	// Deleta os atendimentos em vars_x que não tiverem nenhum aluno alocado.
	#pragma region Retira Atendimentos Incompletos
	
	ofstream folgasOuRemocoesFile;
	std::string folgasOuRemocoesFilename( "folgasOuRemocoes_" );
	folgasOuRemocoesFilename += problemData->getInputFileName();
	folgasOuRemocoesFilename += ".txt";
	folgasOuRemocoesFile.open(folgasOuRemocoesFilename, ios::app);
	if (!folgasOuRemocoesFile)
	{
		cerr << "Error: Can't open output file " << folgasOuRemocoesFilename << endl;		
	}

	int itera=1;
	while ( 1 )
	{
		GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > novasFolgasAlDem;
		ITERA_GGROUP_LESSPTR( itSlack, problemData->listSlackDemandaAluno, AlunoDemanda )
		{		
			Aluno *aluno = problemData->retornaAluno( itSlack->getAlunoId() );
			int campusId = itSlack->demanda->oferta->getCampusId();
			int discId = -itSlack->demanda->getDisciplinaId();
			int prior = itSlack->getPrioridade();

			if ( prior != prioridade )
			{
				continue;
			}

			// Se existir a disciplina teorica/pratica correspondente
			if ( problemData->refDisciplinas.find( discId ) != 
				 problemData->refDisciplinas.end() )
			{
				Disciplina *disciplina = problemData->refDisciplinas[ discId ];

				// Se o aluno estiver alocado em alguma turma da disciplina
				// retira-o, eliminando atendimento parcial
				int turma = problemData->retornaTurmaDiscAluno( aluno, disciplina );
				if ( turma != -1 )
				{
					AlunoDemanda *ad = problemData->procuraAlunoDemanda( discId, aluno->getAlunoId(), prioridade );

					if ( ad == NULL )
					{
						std::cout<<"\nErro em carregaVariaveisSolucaoTaticoPorAluno: AlunoDemanda nao encontrado.\n";
						std::cout<<"Aluno"<<aluno->getAlunoId()<<" Disc"<<discId<<"\n";
					}

					Trio< int, int, Disciplina* > trio;
					trio.set( campusId, turma, disciplina );

					problemData->mapAluno_CampusTurmaDisc[aluno].remove( trio );				
					problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].remove( ad );
				
					problemData->mapSlackAluno_CampusTurmaDisc[aluno].add( trio );
					problemData->mapSlackCampusTurmaDisc_AlunosDemanda[ trio ].add( ad );

					folgasOuRemocoesFile<<"\nRemove "<<itera<<" (1): Aluno"<<aluno->getAlunoId()<<" AlunoDemanda"
						<<ad->getId()<<" Prior"<<ad->getPrioridade()<<" da turma"<<turma<<" disc"<<discId;

					novasFolgasAlDem.add( ad );

					if ( alunoDemandaAtendsHeuristica.find(ad) != alunoDemandaAtendsHeuristica.end() )
						alunoDemandaAtendsHeuristica.remove( ad );

					nroAtendsRemovidosAlunoDemanda++;

					int nroAlunos = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].size();

					if ( nroAlunos == 0 )
					{
						remocao.add( trio );
						problemData->mapCampusTurmaDisc_AlunosDemanda.erase( trio );
					}
					else if ( problemData->violaMinTurma( trio ) )
					{
						folgasOuRemocoesFile<<"\nViolou o min de alunos:";
						GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > >
							alunosDemanda = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ];
						ITERA_GGROUP_LESSPTR( itAlDem, alunosDemanda, AlunoDemanda )
						{
							novasFolgasAlDem.add( *itAlDem );
							folgasOuRemocoesFile<<" "<<itAlDem->getId();
						}
					}
				}
			}

			discId = itSlack->demanda->getDisciplinaId();

			// Se existir a disciplina teorica/pratica correspondente
			if ( problemData->refDisciplinas.find( discId ) != 
				 problemData->refDisciplinas.end() )
			{
				Disciplina *disciplina = problemData->refDisciplinas[ discId ];

				// Se o aluno estiver alocado em alguma turma da disciplina
				// retira-o, eliminando atendimento parcial
				int turma = problemData->retornaTurmaDiscAluno( aluno, disciplina );
				if ( turma != -1 )
				{
					AlunoDemanda *ad = *itSlack;

					Trio< int, int, Disciplina* > trio;
					trio.set( campusId, turma, disciplina );
				
					problemData->mapAluno_CampusTurmaDisc[aluno].remove( trio );				
					problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].remove( ad );
								
					problemData->mapSlackAluno_CampusTurmaDisc[aluno].add( trio );
					problemData->mapSlackCampusTurmaDisc_AlunosDemanda[ trio ].add( ad );
				
					folgasOuRemocoesFile<<"\nRemove "<<itera<<" (2): Aluno"<<aluno->getAlunoId()<<" AlunoDemanda"
						<<ad->getId()<<" Prior"<<ad->getPrioridade()<<" da turma"<<turma<<" disc"<<discId;

					if ( alunoDemandaAtendsHeuristica.find(ad) != alunoDemandaAtendsHeuristica.end() )
						alunoDemandaAtendsHeuristica.remove( ad );
										
					int nroAlunos = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].size();

					if ( nroAlunos == 0 )
					{
						remocao.add( trio );
						problemData->mapCampusTurmaDisc_AlunosDemanda.erase( trio );
					}
					else if ( problemData->violaMinTurma( trio ) )
					{
						folgasOuRemocoesFile<<"\nViolou o min de alunos:";
						GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > >
							alunosDemanda = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ];
						ITERA_GGROUP_LESSPTR( itAlDem, alunosDemanda, AlunoDemanda )
						{
							novasFolgasAlDem.add( *itAlDem );
							folgasOuRemocoesFile<<" "<<itAlDem->getId();
						}
					}
				}
			}
		}

		ITERA_GGROUP_LESSPTR( itNovasFolgas, novasFolgasAlDem, AlunoDemanda )
		{
			problemData->listSlackDemandaAluno.add( *itNovasFolgas );
		}		

		if ( novasFolgasAlDem.size() == 0 )
			break;
		itera++;
	}

   if ( folgasOuRemocoesFile )
   {
	   folgasOuRemocoesFile<< std::endl;
	   folgasOuRemocoesFile.close();
   }
	#pragma endregion
	// -----------------------------------------------------------------------------


	// -----------------------------
	// CONFERÊNCIA (pode deletar se nao estiver havendo problema)
    ITERA_GGROUP_LESSPTR( itSlack, problemData->listSlackDemandaAluno, AlunoDemanda )
	{
		Aluno *aluno = problemData->retornaAluno( itSlack->getAlunoId() );
		int campusId = itSlack->demanda->oferta->getCampusId();
		int discId = itSlack->demanda->getDisciplinaId();
		int prior = itSlack->getPrioridade();
		if ( prior != prioridade )
			continue;

		Disciplina *disciplina = problemData->refDisciplinas[ discId ];

		int turma = problemData->retornaTurmaDiscAluno( aluno, disciplina );
		if ( turma != -1 )
		{
			std::cout<<"\nErro: aluno "<<aluno->getAlunoId()<<" nao removido corretamente da turma "<<turma<<" disc "<<discId<<"\n"; fflush(NULL);
			Trio< int, int, Disciplina* > trio;
			trio.set( campusId, turma, disciplina );	
			AlunoDemanda *ad = problemData->procuraAlunoDemanda( discId, aluno->getAlunoId(), prioridade );
			problemData->mapAluno_CampusTurmaDisc[aluno].remove( trio );				
			problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].remove( ad );

			problemData->mapSlackAluno_CampusTurmaDisc[aluno].add( trio );
			problemData->mapSlackCampusTurmaDisc_AlunosDemanda[ trio ].add( ad );

			int nroAlunos = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].size();
			if ( nroAlunos == 0 )
			{
				remocao.add( trio );
				problemData->mapCampusTurmaDisc_AlunosDemanda.erase( trio );
			}

			turma = problemData->retornaTurmaDiscAluno( aluno, disciplina );
			if ( turma != -1 )
			{ std::cout<<"\nErro DE NOVO!! Erro eh no remove entao"; fflush(NULL); }
		}
	}
	// -----------------------------
	
   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
		(*itAluno)->clearHorariosDiaOcupados();
   }

   #pragma region Preenche solVars e vars_x
   VariableTaticoHash::iterator vit;
      
   problemData->mapCreditosSalas.clear();

   vit = vHashTatico.begin();
   while ( vit != vHashTatico.end() )
   {
      VariableTatico* v = new VariableTatico( vit->first );
      int col = vit->second;
	  double value = xSol[ col ];
      v->setValue( value );

      if ( v->getValue() > 0.00001 )
      {

         switch( v->getType() )
         {
			 case VariableTatico::V_ERROR:
			 {
				 delete v;
				 break;
			 }
			 case VariableTatico::V_CREDITOS:
			 {
				  Trio< int, int, Disciplina* > trio;
				  trio.set( v->getUnidade()->getIdCampus(), v->getTurma(), v->getDisciplina() );

				  // Insere
				  if ( remocao.find( trio ) == remocao.end() )
				  {
					  solVarsTatico.add( v );
					  vars_xh.add( v );	
						
					  // Acha horariosDias usados
					  GGroup<HorarioDia*> horariosDias;
					  int dia = v->getDia();
					  HorarioAula *hi = v->getHorarioAulaInicial();
					  HorarioAula *hf = v->getHorarioAulaFinal();
					  int nCreds = hi->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
					  HorarioAula *ha = hi;
					  for ( int i = 1; i <= nCreds; i++ )
					  {
							HorarioDia *hd = problemData->getHorarioDiaCorrespondente( ha, dia );
							horariosDias.add( hd );							
							ha = hi->getCalendario()->getProximoHorario( ha );				
					  }

					  GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > 
						  alunosDemandaAlocados = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ];
					  ITERA_GGROUP_LESSPTR( itAlDem, alunosDemandaAlocados, AlunoDemanda )
					  {
						  Aluno *aluno = problemData->retornaAluno( (*itAlDem)->getAlunoId() );
						  aluno->addHorarioDiaOcupado( horariosDias );
					  }

					  Sala *sala = v->getSubCjtSala()->salas.begin()->second;
					  sala->addHorarioDiaOcupado( horariosDias );

					  hi = v->getHorarioAulaInicial();
					  hf = v->getHorarioAulaFinal();

					  int nAlunos=0;
					  if ( problemData->mapCampusTurmaDisc_AlunosDemanda.find(trio) !=
						   problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
						 nAlunos = problemData->mapCampusTurmaDisc_AlunosDemanda[trio].size();
					  else
						  std::cout<<"\nERRO! Turma aberta sem alunos: ";

/*					  std::cout << std::endl << " " << nAlunos << " vagas para a aula de "
							<< problemData->retornaNroCreditos( hi, hf, sala, v->getDisciplina(), dia )
							<< " creditos da disciplina " << v->getDisciplina()->getCodigo() << " id" <<v->getDisciplina()->getId()
							<< " para a turma " << v->getTurma()
							<< " no dia " << dia
							<< " para a sala " << sala->getId();	*/						
				  }
				  else
				  {
					  delete v;
				  }
				  break;
			 }
			 case VariableTatInt::V_OFERECIMENTO:
			 {
				 if ( problemData->mapCreditosSalas.find( v->getSubCjtSala() ) == problemData->mapCreditosSalas.end() )
					  problemData->mapCreditosSalas[ v->getSubCjtSala() ] = v->getDisciplina()->getTotalTempo();
				 else problemData->mapCreditosSalas[ v->getSubCjtSala() ] += v->getDisciplina()->getTotalTempo();
				 break;
			 }
			 case VariableTatico::V_ABERTURA:
			 {
				  Trio< int, int, Disciplina* > trio;
				  trio.set( v->getCampus()->getId(), v->getTurma(), v->getDisciplina() );

				  if ( remocao.find( trio ) == remocao.end() )
				  {
					  solVarsTatico.add( v );
				  }
				  else
				  {
					  delete v;
				  }
				  break;
			 }
			 case VariableTatico::V_ABERTURA_COMPATIVEL:
			 {
				 bool inseriu = false;
				 std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
					 itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.begin();
				 for ( ; itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end(); itMapAtend++ )
				 {
					 int discId = itMapAtend->first.third->getId();

					 if ( discId == v->getDisciplina()->getId() )
					 {
						 solVarsTatico.add( v );
						 inseriu = true;
						 break;
					 }
				 }
				 if ( !inseriu )
				 {
					 delete v;
				 }
				 break;
			 }
			 case VariableTatico::V_DIAS_CONSECUTIVOS:
			 {
				 bool inseriu = false;
				 std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
					 itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.begin();
				 for ( ; itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end(); itMapAtend++ )
				 {
					 int discId = itMapAtend->first.third->getId();
					 int turma = itMapAtend->first.second;
					 int cpId = itMapAtend->first.first;

					 if ( discId == v->getDisciplina()->getId() &&
						  turma == v->getTurma() &&
						  cpId == v->getCampus()->getId() )
					 {
						 solVarsTatico.add( v );
						 inseriu = true;
						 break;
					 }
				 }
				 if ( !inseriu )
				 {
					 delete v;
				 }
				 break;
			 }
			 case VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR:			// fcp_{i,d,tps,t}
			 case VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR:			// fcm_{i,d,tps,t}
			 case VariableTatico::V_COMBINACAO_DIVISAO_CREDITO:			// m_{i,d,k}
			 case VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M:	// fkm{i,d,t}
			 case VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P:	// fkp{i,d,t}
			 {
				 bool inseriu = false;
				 std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
					 itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.begin();
				 for ( ; itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end(); itMapAtend++ )
				 {
					 int discId = itMapAtend->first.third->getId();
					 int turma = itMapAtend->first.second;

					 if ( discId == v->getDisciplina()->getId() &&
						  turma == v->getTurma() )
					 {
						 solVarsTatico.add( v );
						 inseriu = true;
						 break;
					 }
				 }
				 if ( !inseriu )
				 {
					 delete v;
				 }
				 break;
			 }
			 case VariableTatico::V_SLACK_ALUNO_VARIAS_UNID_DIA:	// fu{i1,d1,i2,d2,t}
			 {
				 bool inseriu = false;
				 bool achou1 = false, achou2 = false;

				 std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
					 itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.begin();
				 for ( ; itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end(); itMapAtend++ )
				 {
					 int discId = itMapAtend->first.third->getId();
					 int turma = itMapAtend->first.second;

					 if ( discId == v->getDisciplina1()->getId() && turma == v->getTurma1() )
					 {
						 achou1 = true;
					 }
					 if ( discId == v->getDisciplina2()->getId() && turma == v->getTurma2() )
					 {
						 achou2 = true;						 
					 }
					 if ( achou1 && achou2 )
					 {
						 solVarsTatico.add( v );
						 inseriu = true;
						 break;					 
					 }
				 }
				 if ( !inseriu )
				 {
					 delete v;
				 }
				 break;
			 }
			 case VariableTatico::V_ALUNO_DIA:
			 {	 // Essa variavel será tratada logo depois, pois preciso saber todas as
				 // variaveis de crédito "x" primeiro, para conseguir filtrar esta "du"
				 delete v;
				 break;
			 }
			 default:
			 {
				  // Acho que não tem problema adicionar todo o resto
				  solVarsTatico.add( v );
			 }
		  }
	  }
	  else
	  {
		  delete v;
	  }

	  vit++;
   }

   vit = vHashTatico.begin();
   while ( vit != vHashTatico.end() )
   {
      VariableTatico* v = new VariableTatico( vit->first );
      int col = vit->second;
	  double value = xSol[ col ];
      v->setValue( value );

      if ( v->getValue() > 0.00001 )
      {
         switch( v->getType() )
         {
			 case VariableTatico::V_ALUNO_DIA:
			 {
				 bool inseriu = false;
				 Aluno* aluno = v->getAluno();
				 int dia = v->getDia();

				 GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >	trios = problemData->mapAluno_CampusTurmaDisc[aluno];
				 GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator it = trios.begin();
				 for ( ; it != trios.end(); it++ )
				 {
					 int campusId = (*it).first;
					 int turma = (*it).second;
					 int discId = (*it).third->getId();

					 ITERA_GGROUP_LESSPTR ( it, vars_xh, VariableTatico )
					 {
						 if ( (*it)->getDisciplina()->getId() == discId &&
							  (*it)->getTurma() == turma &&
							  (*it)->getDia() == dia &&
							  (*it)->getUnidade()->getIdCampus() == campusId )
						 {
							 solVarsTatico.add( v );
							 inseriu = true;
							 break;
						 }
					 }
					 if (inseriu) break;
				 }
				 if ( !inseriu )
				 {
					 delete v;
				 }
				 break;
			 }
			 default:
			 {
				delete v;
			 }
		 }
	  }
	  else
	  {
		  delete v;
	  }

	  vit++;
	}

   // CRIA FOLGAS e insere em solVars
   GGroup< Trio< int, int, Disciplina* > >::iterator itRemove = remocao.begin();
   for ( ; itRemove != remocao.end(); itRemove++ )
   {
	    Campus *campus = problemData->refCampus[ (*itRemove).first ];
		int turma = (*itRemove).second;
		Disciplina *d = (*itRemove).third;

		VariableTatico *slackVar = new VariableTatico;
		slackVar->setType( VariableTatico::V_SLACK_DEMANDA );
		slackVar->setTurma( turma );
		slackVar->setDisciplina( d );
		slackVar->setCampus( campus );
		slackVar->setValue( 1 );

		solVarsTatico.add( slackVar );
   }
	#pragma endregion	    
   
	  
   #ifdef PRINT_LOGS

   char solPosFilename[1024];
   strcpy( solPosFilename, "pos_" );
   strcat( solPosFilename, solFilename );
   FILE * fout = fopen( solPosFilename, "wt" );
   ITERA_GGROUP_LESSPTR( itSol, solVarsTatico, VariableTatico )
   {      
	   VariableTatico *v = *itSol;
	   
	   fprintf( fout, "%s = %f\n", v->toString().c_str(), v->getValue() );
		   
   }
   fprintf( fout, "\nAlunosDemanda removidos = %d", nroAtendsRemovidosAlunoDemanda );
   fclose( fout );
   
   #endif

   return nroAtendsRemovidosAlunoDemanda;
}

void SolverMIP::testeCarregaSol( int campusId, int prioridade, int cjtAlunosId, int r, int tatico )
{
	std::cout<<"\nCarregamento de teste...\n"; fflush(NULL);

	double * xSol = NULL;
   
	lp->updateLP();
	int nroColsLP = lp->getNumCols();
	xSol = new double[ nroColsLP ];
   
	char solName[1024];

	strcpy( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
	cout<<"====================> TESTE => carregando tatico " <<solName <<endl; fflush(NULL);
	FILE* fin = fopen( solName,"rb");
	if ( fin == NULL )
	{
		std::cout << "\nErro em SolverMIP::testeCarregaSol(int campusId, int prioridade, int cjtAlunosId, int r): arquivo "
					<< solName << " nao encontrado.\n";fflush(NULL);
		return;
	}

	std::cout<<"\nLendo nro de variaveis...\n"; fflush(NULL);
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
		std::cout << "\nErro em testeCarregaSol(int campusAtualId, int prioridade, int cjtAlunos, int r): "
					<< " \nNumero diferente de variaveis: " << nCols << " != " << nroColsLP; fflush(NULL);
		return;
	}
	fclose(fin);

	std::cout<<"\nCriando arrows...\n"; fflush(NULL);

	int nChg = 0;
	int *idx = new int[lp->getNumCols()*2];
	double *valOrigUB = new double[lp->getNumCols()*2];
	double *valOrigLB = new double[lp->getNumCols()*2];
	double *valUB = new double[lp->getNumCols()*2];
	double *valLB = new double[lp->getNumCols()*2];
	BOUNDTYPE *bts1 = new BOUNDTYPE[lp->getNumCols()*2];
	BOUNDTYPE *bts2 = new BOUNDTYPE[lp->getNumCols()*2];
	
	std::cout<<"\nFixando a solucao lida...\n"; fflush(NULL);

	// Fixação da solucao:
    VariableTaticoHash::iterator vit;
	vit = vHashTatico.begin();
	while ( vit != vHashTatico.end() )
	{
		VariableTatico v = vit->first;
		int col = vit->second;

		// Alteração especifica:
		if ( v.getType() == VariableTatico::V_DESALOCA_ALUNO ||
			 v.getType() == VariableTatico::V_DESALOCA_ALUNO_DIA )
		{
			if ( v.getDisciplina()->getId() == 7369 &&
				 v.getTurma() == 0 )
				continue;
		}
		if ( v.getType() == VariableTatico::V_SLACK_DEMANDA && 
			 v.getTurma() == 0 && 
			 v.getDisciplina()->getId() == 7369 )
		{			
			double ub = 0.0;
			double lb = 0.0;
			idx[nChg] = col;
			valOrigUB[nChg] = lp->getUB(col);
			valOrigLB[nChg] = lp->getLB(col);
			valUB[nChg] = ub + 1e-5;
			valLB[nChg] = lb - 1e-5;
			bts1[nChg] = BOUNDTYPE::BOUND_UPPER;
			bts2[nChg] = BOUNDTYPE::BOUND_LOWER;
			nChg++;
			continue;
		}

		if ( ( xSol[col] > 0.1 &&
				v.getType() != VariableTatico::V_SLACK_DEMANDA &&
				v.getType() != VariableTatico::V_SLACK_SLACKDEMANDA_PT ) ||
			 ( xSol[col] < 0.1 &&
				v.getType() == VariableTatico::V_SLACK_DEMANDA ) )
		{
			double ub = xSol[ col ];
			double lb = xSol[ col ];
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

	std::cout<<"\nAlterando bounds...\n"; fflush(NULL);

	lp->chgBds(nChg,idx,bts1,valUB);
	lp->chgBds(nChg,idx,bts2,valLB);
	lp->updateLP();

	lp->updateLP();
	lp->setTimeLimit( 60 );
	lp->setPreSolve(OPT_TRUE);
	lp->setHeurFrequency(1.0);
	lp->setMIPScreenLog( 4 );
	lp->setMIPEmphasis(0);
	lp->setSymetry(0);
	lp->setCuts(3);
	lp->setNumIntSols(1);	
	lp->updateLP();
	
	char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
	strcat( lpName, "_teste" );

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName );
	#endif
	
	std::cout<<"\nOtimizando o teste..."; fflush(NULL);

	int status = lp->optimize( METHOD_MIP );	

	std::cout<<" otimizado!\n"; fflush(NULL);

	lp->chgBds(nChg,idx,bts1,valOrigUB);
	lp->chgBds(nChg,idx,bts2,valOrigLB);
	lp->updateLP();

	std::cout<<"\nFim do carregamento de teste!\n"; fflush(NULL);
}

void SolverMIP::teste(double *& xSol, int n )
{
	stringstream ss;
	ss << n;
	std::string fileName( "teste_" );
	fileName += ss.str();
	fileName += ".txt";

	ofstream fout;
	fout.open( fileName, ios::app );
	if ( fout == NULL )
	{
		std::cout << "\nErro em SolverMIP::teste(): Arquivo teste.txt nao pode ser aberto.\n";
	}
	else
	{
		int size = vHashTatico.size();
		int nCols = lp->getNumCols();
		
		fout << "nCols: " << nCols;
		fout << "\nhash size: " << size;
		 
		VariableTaticoHash::iterator vit = vHashTatico.begin();
		for ( int i = 0; i < lp->getNumCols(); i++ )
		{
			VariableTatico v = vit->first;

			fout << "\n" << i << ": " << v.toString() << " = " << xSol[ i ];
			  
			vit++;
		}

		fout.close();
	}

}
void SolverMIP::teste( int n )
{
	stringstream ss;
	ss << n;
	std::string fileName( "testeSimples_" );
	fileName += ss.str();
	fileName += ".txt";

	ofstream fout;
	fout.open( fileName, ios::app );
	if ( fout == NULL )
	{
		std::cout << "\nErro em SolverMIP::teste(): Arquivo teste.txt nao pode ser aberto.\n";
	}
	else
	{
		int size = vHashTatico.size();
		int nCols = lp->getNumCols();
		
		fout << "nCols: " << nCols;
		fout << "\nhash size: " << size;
		 
		VariableTaticoHash::iterator vit = vHashTatico.begin();
		for ( int i = 0; i < lp->getNumCols(); i++ )
		{
			VariableTatico v = vit->first;

			fout << "\n" << i << ": " << v.toString();
			  
			vit++;
		}

		fout.close();
	}

}

void SolverMIP::deletaPreSol()
{
	// -----------------------------------------------------
	// Deleta as variaveis em solVarsXPre 
	ITERA_GGROUP_LESSPTR ( it, solVarsXPre, VariablePre )
	{
		delete *it;    
	}
	solVarsXPre.clear();
}

double SolverMIP::fixaLimitesVariavelTaticoComHorAnterior( VariableTatico *v, bool &FOUND )
{
	if ( v->getType() == VariableTatico::V_SLACK_DEMANDA )
	{
		ITERA_GGROUP_LESSPTR ( itVar, solVarsTatico, VariableTatico )
		{
			VariableTatico vSol = **itVar;		

			if ( vSol.getType() == VariableTatico::V_ABERTURA &&
				 vSol.getTurma() == v->getTurma() &&
				 vSol.getDisciplina() == v->getDisciplina() &&		
				 vSol.getCampus() == v->getCampus() )
			{
				FOUND = true;
				return (0.0);				
			}
		}
	}
	else if ( v->getType() == VariableTatico::V_CREDITOS )
	{
		// A variavel x adquire o campo "sala" em solVars, mas v ainda não o tem,
		// por isso x tem que ser comparada separada, por atributo
		ITERA_GGROUP_LESSPTR ( itVar, solVarsTatico, VariableTatico )
		{
			VariableTatico vSol = **itVar;		

			if ( vSol.getTurma() == v->getTurma() &&
				 vSol.getDisciplina() == v->getDisciplina() &&
				 vSol.getUnidade() == v->getUnidade() &&
				 vSol.getSubCjtSala() == v->getSubCjtSala() &&
				 vSol.getDia() == v->getDia() &&
				 vSol.getHorarioAulaInicial() == v->getHorarioAulaInicial() &&
				 vSol.getHorarioAulaFinal() == v->getHorarioAulaFinal() &&
				 vSol.getType() == VariableTatico::V_CREDITOS )
			{
				FOUND = true;
				return (1.0);				
			}
		}
	}
	else
	{
		GGroup< VariableTatico *, LessPtr<VariableTatico> >::iterator itVar = solVarsTatico.find( v );
		if ( itVar != solVarsTatico.end() )
		{
			FOUND = true;
			int intValue = (int) ( (*itVar)->getValue() + 0.5 );
			return (double) intValue;
		}
	}
	
	FOUND = false;
	return 0.0;
}

int SolverMIP::carregaVariaveisSolucaoTaticoPorAluno_CjtAlunos( int campusAtualId, int prioridade, int cjtAlunos, int r, int tatico )
{

   std::cout << "\n\nCarregando solucao tatica...\n";

	problemData->mapSlackCampusTurmaDisc_AlunosDemanda.clear();
	problemData->mapSlackAluno_CampusTurmaDisc.clear();

   double * xSol = NULL;
   
   SolutionLoader sLoader( problemData, problemSolution );
   lp->updateLP();

   int nroColsLP = lp->getNumCols();
   xSol = new double[ nroColsLP ];
	
	#pragma region Carrega solucao
	if ( this->CARREGA_SOLUCAO )
	{
		//int status = readSolBin( campusAtualId, prioridade, cjtAlunos, r, tatico, 0, OutPutFileType::TAT_HOR_BIN, xSol );
      int status = readSolTxtTat(campusAtualId, prioridade, cjtAlunos, r, tatico, OutPutFileType::TAT_HOR_BIN, xSol, 0 );
		if ( !status )
		{
		   std::cout << "\nErro em SolverMIP::carregaVariaveisSolucaoTaticoPorAluno_CjtAlunos( int campusId, int prioridade, int cjtAlunos, int r, int tatico ): arquivo "
					" nao encontrado.\n";
			exit(0);
		}
	}
	else
	{
		lp->getX( xSol );
	}
	#pragma endregion

   std::map< std::pair<Disciplina*, Oferta*>, int > mapSlackDemanda;
   std::map< std::pair<Disciplina*, Oferta*>, int >::iterator itMapSlackDemanda;
   

   char solFilename[1024];
   strcpy( solFilename, getSolucaoTaticoFileName( campusAtualId, prioridade, cjtAlunos, r, tatico, 0 ).c_str() );

   ofstream fout( solFilename, std::ios_base::out );
   if ( fout == NULL )
   {
	   std::cout << "\nErro em SolverMIP::carregaVariaveisSolucaoTaticoPorAluno_CjtAlunos( int campusAtualId, int prioridade, int cjtAlunos, int tatico )"
				 << "\nArquivo " << solFilename << " nao pode ser aberto\n";
	   ofstream fout( "solSubstituto.txt", std::ios_base::out );
	   if ( fout == NULL )
	   {
			std::cout <<"\nErro de novo. Finalizando execucao...\n";
			exit(0);
	   }
   }

	ofstream folgasOuRemocoesFile;
	std::string folgasOuRemocoesFilename( "folgasOuRemocoes_" );
	folgasOuRemocoesFilename += problemData->getInputFileName();
	folgasOuRemocoesFilename += ".txt";
	folgasOuRemocoesFile.open(folgasOuRemocoesFilename, ios::app);
	if (!folgasOuRemocoesFile)
	{
		cerr << "Error: Can't open output file " << folgasOuRemocoesFilename << endl;		
	}
	else
	{
		folgasOuRemocoesFile <<"\n\n===============================================================================================";
		folgasOuRemocoesFile <<"\nCampusAtualId " << campusAtualId<< ", Prioridade" << prioridade <<", R="<< r <<", Tatico "<<tatico;
	}

   int nroNaoAtendimentoAlunoDemanda = 0;
   int nroAtendimentoAlunoDemanda = 0;
   Aluno *vAluno;
   AlunoDemanda *alunoDem;
   Disciplina *vDisc;
   int vDia;
   int vTurma;
   int vCampusId;


   #ifdef TATICO_COM_HORARIOS
   VariableTaticoHash::iterator vit = vHashTatico.begin();
   while ( vit != vHashTatico.end() )
   {
	  VariableTatico* v = new VariableTatico( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         //#ifdef DEBUG
         char auxName[100];
         lp->getColName( auxName, col, 100 );
         fout << auxName << " = " << v->getValue() << endl;
         //#endif
		 		 
		 Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;

         switch( v->getType() )
         {
			 case VariableTatico::V_ERROR:
				std::cout << "Variável inválida " << std::endl;
				break;
			 case VariableTatico::V_CREDITOS:				 					 
				break;
			 case VariableTatico::V_ABERTURA:
				 vDisc = v->getDisciplina();
				 vTurma = v->getTurma();
				 vCampusId = v->getCampus()->getId();

				 trio.set(vCampusId, vTurma, vDisc);
				 nroAtendimentoAlunoDemanda += problemData->mapCampusTurmaDisc_AlunosDemanda[trio].size();
				 break;
			 case VariableTatico::V_DESALOCA_ALUNO:

				 vDisc = v->getDisciplina();
				 vTurma = v->getTurma();
				 vAluno = v->getAluno();
				 				 
				 alunoDem = vAluno->getAlunoDemanda( vDisc->getId() );

				 nroNaoAtendimentoAlunoDemanda++;
				 problemData->listSlackDemandaAluno.add( alunoDem );				 

				 itMapSlackDemanda = mapSlackDemanda.find( std::make_pair( vDisc, alunoDem->demanda->oferta ) );
				 if ( itMapSlackDemanda != mapSlackDemanda.end() )
				 	itMapSlackDemanda->second = itMapSlackDemanda->second + 1;
				 else
					mapSlackDemanda[ std::make_pair( vDisc, alunoDem->demanda->oferta ) ] = 1;			 
				
				 break;
			 case VariableTatico::V_SLACK_DEMANDA:

				 // ------------ Preenche mapSlackDemanda ---------------------

				 vDisc = v->getDisciplina();
				 vTurma = v->getTurma();
				 vCampusId = v->getCampus()->getId();

				 trio.set(vCampusId, vTurma, vDisc);

				 std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator 
					 itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
				 
				 if ( itMap != problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
				 {
					 folgasOuRemocoesFile <<"\n\nFolga i"<<vTurma<<" Disc"<<vDisc->getId()<<":";

					 GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda = itMap->second;					
				 
					 ITERA_GGROUP_LESSPTR( itAlDem, alunosDemanda, AlunoDemanda )
					 {
						 nroNaoAtendimentoAlunoDemanda++;
						 problemData->listSlackDemandaAluno.add( *itAlDem );				 
					 
						 folgasOuRemocoesFile <<"\n   Aluno"<<(*itAlDem)->getAlunoId()<<" AlunoDemanda"
							 <<(*itAlDem)->getId()<<" Prior"<<(*itAlDem)->getPrioridade();
						 
						 itMapSlackDemanda = mapSlackDemanda.find( std::make_pair( vDisc, itAlDem->demanda->oferta ) );
						 if ( itMapSlackDemanda != mapSlackDemanda.end() )
							 itMapSlackDemanda->second = itMapSlackDemanda->second + 1;
						 else
							mapSlackDemanda[ std::make_pair( vDisc, itAlDem->demanda->oferta ) ] = 1;			 
					 }
				 }
				 // ------------------------------------------------------------

				 break;

           }
      }

	  delete v;
	 
      vit++;
   }
   #endif

   if ( folgasOuRemocoesFile )
   {
	   folgasOuRemocoesFile<< std::endl;
	   folgasOuRemocoesFile.close();
   }

   // Escreve um resumo do não-atendimento das demandas, juntando
   // as demandas não atendidas por oferta.
	itMapSlackDemanda = mapSlackDemanda.begin();
	for ( ; itMapSlackDemanda != mapSlackDemanda.end() ; itMapSlackDemanda++ )
	{
		fout << "FD_{_Disc" << itMapSlackDemanda->first.first->getId() 
			<< "_Oft" << itMapSlackDemanda->first.second->getId() 
			<< "} = " << itMapSlackDemanda->second << endl;
	}
	fout << "\nAlunosDemanda nao atendidos = " << nroNaoAtendimentoAlunoDemanda << endl;
	fout << "AlunosDemanda atendidos = " << nroAtendimentoAlunoDemanda << endl;

	std::cout << std::endl;

    if ( fout )
    {
		fout.close();
    }	
    
	
//   if ( tatico==2 && prioridade==1 )
//	  teste( xSol, 2 );

	int nar = removeAtendimentosParciais( xSol, solFilename, prioridade );

#ifndef TATICO_COM_HORARIOS
   vit = vHash.begin();
   for (; vit != vHash.end(); ++vit )
   {
      Variable * v = new Variable( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         sLoader.setFolgas( v );
      }

      delete v;
   }
#endif

#ifdef TATICO_COM_HORARIOS
   vit = vHashTatico.begin();
   for (; vit != vHashTatico.end(); ++vit )
   {
      VariableTatico * v = new VariableTatico( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         sLoader.setFolgas( v );
      }

      delete v;
   }
#endif


//   if ( tatico==2 && prioridade==1 )
//	  teste( xSol, 3 );

  
   if ( tatico==2 || CRIA_NOVAS_VARIAVEIS_TAT_HOR2 ) // para a segunda rodada do tatico, aproveita-se as variaveis. Logo, não posso deletar o primeiro.
   {
	   if ( vHashTatico.size() > 0 )
	   {
		   vHashTatico.clear();
	   }
   }    

   if ( cHashTatico.size() > 0 )
   {
	   cHashTatico.clear();
   }

   if ( xSol )
   {
      delete [] xSol;
   }
   
   bool violou = problemData->violaMinTurma( campusAtualId );


   std::cout << "\nSolucao tatica carregada com sucesso!\n";
   std::cout << "\n-----------------------------------------------------------------\n";
   std::cout << "-----------------------------------------------------------------\n\n";

   return ( nar + nroNaoAtendimentoAlunoDemanda );
}


int SolverMIP::solveTaticoPorCampusComSolHeur()
{
	int status = 1;

	std::cout<<"\n\n--------------------------------------------------------------------------------";
	std::cout<<"\n\n--------------------------------------------------------------------------------";
	std::cout << "\nIniciando MIP com solucao inicial heuristica..." << endl;

	if ( ! problemData->existeSolTaticoInicial() )
	{
		std::cout << "\nErro: nao ha solucao inicial. Saindo..." << endl;
		exit(1);
	}

	if ( this->CARREGA_SOLUCAO )
	{
		std::cout << "\nCARREGA_SOLUCAO = true\n" << endl;
	}
	
    ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
    {		
		int campusId = ( *itCampus )->getId();
		this->campusAtualId = campusId;
		
		int n_prioridades = problemData->nPrioridadesDemanda[campusId];
		int P = 1;
		
		// ============================================ P1 =======================================================


		std::cout<<"\n-------------------------- Campus " << campusId << "----------------------------\n";		
		std::cout<<"\n-------------------------- Prioridade " << P << "---------------------------\n";
		std::cout << "\nNumero total de niveis de prioridade no campus: " << n_prioridades << endl;


		std::cout << "\nExiste solucao inicial! " << endl;
		std::cout << "Calcula numero de turmas ja abertas..." << endl;
		problemData->getSolTaticoInicial()->calculaNroTurmasAbertas( campusId );

		std::cout << "Cria variaveis de solucao inicial..." << endl;
		criaVariaveisSolInicial( campusId );

		std::cout << "Preenche map de turmas e turnos..." << endl;
		problemData->preencheMapsTurmasTurnosIES();							
		problemData->imprimeAlocacaoAlunos( campusId, 1, -1, false, -1, toString(SOL_INIT), this->getRunTime() );

					
		// Cria e ordena os conjuntos de alunos
		problemData->criaCjtAlunos( campusId, P, this->FIXAR_P1 );
			
		problemData->imprimeCjtAlunos( campusId );

		problemData->constroiHistogramaDiscDemanda();
		
		if ( problemData->cjtAlunos.size() != 1 )
		{
			std::cout << "\nSomente 1 conjunto de alunos eh esperado aqui. Saindo...\n";
			exit(1);
		}

		int grupoId = problemData->cjtAlunos.begin()->first;
		
		int r = 1;
		if ( problemData->parametros->min_alunos_abertura_turmas )
			r = 2;


		// ==================================
		// MODELO INTEGRADO	SEM NOVAS TURMAS
		int NOVAS_TURMAS = 0;

		TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( 
			this->problemData, &(this->solVarsTatico), &(this->vars_xh), &this->CARREGA_SOLUCAO, true, NOVAS_TURMAS );
			
		solverTaticoInt->solveTaticoIntegrado( campusId, P, r );
			
		delete solverTaticoInt;


		// ==================================
		// MODELO INTEGRADO	COM NOVAS TURMAS	
#ifndef TATICO_SO_HEURN
		NOVAS_TURMAS = 1;
		if ( NOVAS_TURMAS )
		{
			bool EQUIV=true;
			for ( int it=1; it<=1; it++ )
			{
				TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( this->problemData, &(this->solVarsTatico), &(this->vars_xh),
																			&this->CARREGA_SOLUCAO, EQUIV, NOVAS_TURMAS );
				solverTaticoInt->solveTaticoIntegrado( campusId, P, r );
				delete solverTaticoInt;
				NOVAS_TURMAS++;						
			}
		}	
#endif
				
		verificaNaoAtendimentos( campusId, P );
		
		// ============================================ P2 =======================================================

		P++;

		if ( problemData->parametros->utilizarDemandasP2 )
		if ( P <= n_prioridades )
		{
			std::cout << "\nAtualizacao de demandas de prioridade " << P <<"..."; fflush(NULL);
			problemData->atualizaDemandas( P, campusId );
			std::cout << "  atualizadas!\n"; fflush(NULL);

			// ==================================
			// MODELO INTEGRADO	PARA P2
			if ( problemData->listSlackDemandaAluno.size() != 0 )
			{
				TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( 
					this->problemData, &(this->solVarsTatico), &(this->vars_xh), &this->CARREGA_SOLUCAO, true, 0 );
				solverTaticoInt->solveTaticoIntegrado( campusId, P, 1 );
				delete solverTaticoInt;
			}
			
			
#ifndef TATICO_SO_HEURN
			if ( problemData->listSlackDemandaAluno.size() != 0 )
			{
				TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( 
					this->problemData, &(this->solVarsTatico), &(this->vars_xh), &this->CARREGA_SOLUCAO, true, 3 );
				solverTaticoInt->solveTaticoIntegrado( campusId, P, 1 );
				delete solverTaticoInt;
			}
#endif
			// ==================================
		}
		
		verificaNaoAtendimentos( campusId, P );
		
		// ========================================= FIM DO CAMPUS =================================================

		problemData->confereExcessoP2( campusId );		
		problemData->listSlackDemandaAluno.clear();
		mudaCjtSalaParaSala();			
		getSolutionTaticoPorAlunoComHorario();

	}
	
	return (status);
}


int SolverMIP::solveTaticoPorCampusCjtAlunos()
{
	int statusPre = 1, statusTatico = 1, status = 1;

	if ( this->CARREGA_SOLUCAO )
	{
		std::cout << "\nCARREGA_SOLUCAO = true\n" << endl;
	}


    ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
    {		
		int campusId = ( *itCampus )->getId();

		this->campusAtualId = campusId;
		
		int n_prioridades = problemData->nPrioridadesDemanda[campusId];
		int P = 1;
		
		// ================================================ P1 ======================================================

		if ( problemData->existeSolTaticoInicial() )
		{
			std::cout << "\nExiste solucao inicial! " << endl;
			std::cout << "Calcula numero de turmas ja abertas..." << endl;
			problemData->getSolTaticoInicial()->calculaNroTurmasAbertas( campusId );

			std::cout << "Cria variaveis de solucao inicial..." << endl;
			criaVariaveisSolInicial( campusId );

			problemData->preencheMapsTurmasTurnosIES();							
			problemData->imprimeAlocacaoAlunos( campusId, 1, -1, false, -1, toString(SOL_INIT), this->getRunTime() );
		}

		std::cout<<"\n\n--------------------------------------------------------------------------------";		
		std::cout<<"\n-------------------------- Campus " << campusId << "----------------------------\n";		
		std::cout<<"\n-------------------------- Prioridade " << P << "---------------------------\n";			
		
		std::cout << "\nNumero total de niveis de prioridade no campus: " << n_prioridades << endl;
			
		// Cria e ordena os conjuntos de alunos
		problemData->criaCjtAlunos( campusId, P, this->FIXAR_P1 );
			
		problemData->imprimeCjtAlunos( campusId );

		problemData->constroiHistogramaDiscDemanda();

		// Modelo EstimaTurmas
		if ( MODELO_ESTIMA_TURMAS && P==1 && problemData->cjtAlunoDemanda.size()>0 )
		{
			EstimaTurmas * solverEstimaTurmas = new EstimaTurmas( this->problemData, &this->CARREGA_SOLUCAO );
			solverEstimaTurmas->solveEstimaTurmas( campusId, P );
			delete solverEstimaTurmas;
		}

		// Resolve pre-modelo e tatico para cada conjunto
		map< int, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
			itMapCjtAlunos = problemData->cjtAlunos.begin();			
		for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
		{
			int grupoId = itMapCjtAlunos->first;
			
			int nRodadas;
			if ( P==1 && problemData->parametros->min_alunos_abertura_turmas ) nRodadas = 2;
			else nRodadas = 1;

			std::cout << "\nNumero total de rodadas: " << nRodadas << endl;

			for ( int r=1; r<=nRodadas ;r++ )
			{
					
				if ( P == 1 && r==1 && problemData->passarPorPreModeloETatico(campusId) )
				{

					// -------------------------------------
					// PreModelo
					PreModelo * solverPreModelo = new PreModelo( this->problemData, &(this->solVarsXPre), &(this->vars_xh), &(this->CARREGA_SOLUCAO) );
					solverPreModelo->solvePreTaticoMain( campusId, P, grupoId, r );
					delete solverPreModelo;
					// -------------------------------------


					int nTaticos=2;
					for ( int tatico=1; tatico<=nTaticos; tatico++ )
					{					
						int na = solveTaticoMain( campusId, P, grupoId, r, tatico );

						if ( na == 0 ) break;
						
						if ( problemData->pularParaTaticoIntegrado(campusId) ) break;

						if (tatico < nTaticos) 
						{
							voltaComAlunosNaoAlocados();							
						}
					}					
					
					deletaPreSol();

					statusTatico = ( statusTatico && statusPre );
					
					vHashTatico.clear();
				}												
				else if ( P == 1 && r==1 )
				{
					atualizaFolgasAlunoDemanda( campusId );
				}
					
				// ==================================
				// MODELO INTEGRADO	SEM NOVAS TURMAS
				int NOVAS_TURMAS = 0;
				if ( P == 1 && r==1 )
				if ( problemData->passarPorPreModeloETatico(campusId) || problemData->existeSolTaticoInicial() )
				{
					TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( this->problemData, &(this->solVarsTatico), &(this->vars_xh),
																				&this->CARREGA_SOLUCAO, true, NOVAS_TURMAS );
					solverTaticoInt->solveTaticoIntegrado( campusId, P, r );
					delete solverTaticoInt;
				}
				// ==================================


				// ==================================
				// MODELO INTEGRADO	COM NOVAS TURMAS	
				NOVAS_TURMAS = 1;
				if ( NOVAS_TURMAS )
				{
					bool EQUIV=false;
					for ( int it=1; it<=2; it++ )
					{
						//if ( problemData->listSlackDemandaAluno.size() != 0 )
						//{
							//if ( NOVAS_TURMAS == 3 ) NOVAS_TURMAS++; // pula o 3 (equiv..), vai direto para teste que deixa livre todas as salas

							TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( this->problemData, &(this->solVarsTatico), &(this->vars_xh),
																						&this->CARREGA_SOLUCAO, EQUIV, NOVAS_TURMAS );
							solverTaticoInt->solveTaticoIntegrado( campusId, P, r );
							delete solverTaticoInt;
							NOVAS_TURMAS++;
						//}
						//else std::cout << "\nSem folga de demandas! ---- 0\n"; fflush(NULL);
					}
				}

				// ==================================	
				// BRANCH DE SALAS
					
				//TaticoIntAlunoHor * solverBranchSala = new TaticoIntAlunoHor( this->problemData, &(this->solVarsTatico), &(this->vars_xh),
				//															&this->CARREGA_SOLUCAO, false, 4 );					
				//solverBranchSala->solveTaticoIntegradoBranchSalas( campusId, P, r );
				//delete solverBranchSala;
					
				// ==================================	
					

				if ( problemData->listSlackDemandaAluno.size() == 0 )
				{
					std::cout << "\nSem folga de demandas! ---- 1\n"; fflush(NULL);
					break;
				}
					
				// ==================================
				// MODELO INTEGRADO COM EQUIVALENCIAS					
				#ifndef EQUIVALENCIA_DESDE_O_INICIO
					if ( P==1 )
					if ( r==1 )
					if ( problemData->parametros->considerar_equivalencia_por_aluno )					
					if ( problemData->listSlackDemandaAluno.size() != 0 )
					{
						TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( this->problemData, &(this->solVarsTatico), &(this->vars_xh), 
																					&this->CARREGA_SOLUCAO, true, true );
						solverTaticoInt->solveTaticoIntegrado( campusId, P, r );						
						delete solverTaticoInt;
					}
				#endif
				// ==================================

				if ( problemData->listSlackDemandaAluno.size() == 0 )
				{
					std::cout << "\nSem folga de demandas! ---- 2\n"; fflush(NULL);
					break;
				}
					
				// Rodada dupla somente se estiverem sendo considerados formandos e minimo de alunos em turmas
				if ( ( !problemData->parametros->violar_min_alunos_turmas_formandos ||
						!problemData->parametros->min_alunos_abertura_turmas ||
						!(*itCampus)->getPossuiAlunoFormando() ) )
				{
					std::cout << "\nRodada simples!\nViolar formandos = "
						<< problemData->parametros->violar_min_alunos_turmas_formandos 
						<< "; MinAlunos = " << problemData->parametros->min_alunos_abertura_turmas
						<< "; Possui Formandos = " << (*itCampus)->getPossuiAlunoFormando();
					fflush(NULL);
					std::cout << "\nFim de R" << r << "!\n"; fflush(NULL);
					break;
				}

				if ( ! problemData->haAlunoFormandoNaoAlocado(campusId, P) )
				{
					std::cout << "\nRodada simples! Sem formandos nao-atendidos."; fflush(NULL);
					break;
				}

				std::cout << "\nFim de R" << r << "!\n"; fflush(NULL);
			}
		}
		
		verificaNaoAtendimentos( campusId, P );
			
		// ================================================ P2 ======================================================

		if ( ! problemData->parametros->utilizarDemandasP2 )
		{
			std::cout << "\nNao usa P2!\n"; fflush(NULL);
		}
		else if ( problemData->listSlackDemandaAluno.size() == 0 )
		{
			std::cout << "\nSem folga de demandas! ---- 3\n"; fflush(NULL);
		}
		else if ( P + 1 <= n_prioridades )
		{
			std::cout << "\nAtualizacao de demandas de prioridade " << P + 1<<"..."; fflush(NULL);
			problemData->atualizaDemandas( P+1, campusId );
			std::cout << "  atualizadas!\n"; fflush(NULL);

			// ==================================
			// MODELO INTEGRADO	PARA P2
#ifdef MODELO_INTEGRADO_P2
			if ( problemData->listSlackDemandaAluno.size() != 0 )
			{
				TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( this->problemData, &(this->solVarsTatico), &(this->vars_xh),
																			&this->CARREGA_SOLUCAO, false, 0 );
				solverTaticoInt->solveTaticoIntegrado( campusId, P+1, 1 );
				delete solverTaticoInt;
			}

			if ( problemData->listSlackDemandaAluno.size() != 0 )
			{
				TaticoIntAlunoHor * solverTaticoInt = new TaticoIntAlunoHor( this->problemData, &(this->solVarsTatico), &(this->vars_xh),
																			&this->CARREGA_SOLUCAO, false, 3 );
				solverTaticoInt->solveTaticoIntegrado( campusId, P+1, 1 );
				delete solverTaticoInt;
			}
#endif
			// ==================================
		}			
				
		verificaNaoAtendimentos( campusId, P );
		
		// ============================================== FIM DO CAMPUS ===============================================
		
		problemData->confereExcessoP2( campusId );
		
		problemData->listSlackDemandaAluno.clear(); // Caso seja util ter isso depois, entao tem que fazer um map com campus

		mudaCjtSalaParaSala();			
		
		getSolutionTaticoPorAlunoComHorario();

		status = ( status && statusTatico );
	}
	
	return (status);
}



int SolverMIP::solveTaticoMain( int campusId, int prioridade, int cjtAlunosId, int r, int tatico )
{
	std::cout<<"\n---------------------Rodada "<< r <<" -----------------------\n";
	std::cout<<"\n------- Campus "<< campusId << " , Conjunto-Aluno "<< cjtAlunosId << ", Prior " << prioridade << "----------\n";
	std::cout<<"\n------------------------------Tatico "<<tatico<<"------------------------------\n"; fflush(NULL);

	CPUTimer timer;
	timer.start();

	int statusTatico = solveTaticoBasicoCjtAlunos( campusId, prioridade, cjtAlunosId, r, tatico );

	int na = carregaVariaveisSolucaoTaticoPorAluno_CjtAlunos( campusId, prioridade, cjtAlunosId, r, tatico );

	timer.stop();
	double runtime = timer.getCronoCurrSecs();
						
	problemData->preencheMapsTurmasTurnosIES();

	std::cout<<"\nCarregou solucao tatica\n";
				
	if ( tatico == 1 )
		problemData->imprimeAlocacaoAlunos( campusId, prioridade, cjtAlunosId, false, r, toString(TATICO1), runtime );
	else
		problemData->imprimeAlocacaoAlunos( campusId, prioridade, cjtAlunosId, false, r, toString(TATICO2), runtime );

	problemData->imprimeDiscNTurmas( campusId, prioridade, cjtAlunosId, false, r, tatico );

	// retorna numero de não-atendimentos
	return na;
}


int SolverMIP::solveTaticoBasicoCjtAlunos( int campusId, int prioridade, int cjtAlunosId, int r, int tatico )
{
	int status = 0;

	bool CARREGA_SOL_PARCIAL = this->CARREGA_SOLUCAO;
	bool CARREGA_TESTE = false;

	//if ( tatico==2 && prioridade==1 ) 
	//	CARREGA_TESTE = true;

	if ( CARREGA_TESTE ) CARREGA_SOLUCAO = false;

   if ( this->CARREGA_SOLUCAO )
   {
	   char solName[1024];
	   //strcpy( solName, getSolBinFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
	   
	   strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, cjtAlunosId, r, tatico, 0 ).c_str() );
	   
	   FILE* fin = fopen( solName,"rb");
	   if ( fin == NULL )
	   {
		   std::cout << "\nA partir de " << solName << " , nao foram lidas mais solucoes.\n";
		   CARREGA_SOLUCAO = false;
	   }
	   else
	   {
		  fclose(fin);
	   }
   }

   int varNum = 0;
   int constNum = 0;
   
   if ( lp != NULL && ( tatico==1 || CRIA_NOVAS_VARIAVEIS_TAT_HOR2 ) )
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
   else if ( tatico!=1 && !CRIA_NOVAS_VARIAVEIS_TAT_HOR2 )
   {
	   lp->delRows( 0, lp->getNumRows()-1 );
	   std::cout<<"\nNumero de restricoes apos remocao: "<<lp->getNumRows()<<endl; fflush(NULL);
   }

   if ( tatico==1 || CRIA_NOVAS_VARIAVEIS_TAT_HOR2 ) // para a segunda rodada do tatico, aproveita-se as variaveis
   {
	   if ( vHashTatico.size() > 0 )
	   {
		   vHashTatico.clear();
	   }
   }
   
   if ( cHashTatico.size() > 0 )
   {
	   cHashTatico.clear();
   }

   char lpName[1024], id[100];
   strcpy( lpName, getTaticoLpFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );

   if ( tatico==1 || CRIA_NOVAS_VARIAVEIS_TAT_HOR2 )
   {
	   if ( problemData->parametros->funcao_objetivo == 0
		  || problemData->parametros->funcao_objetivo == 2 )
	   {
		  lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
	   }
	   else if( problemData->parametros->funcao_objetivo == 1 )
	   {
		  lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
	   }
   }
#ifdef DEBUG
   printf( "Creating LP...\n" );
#endif


   if ( problemData->parametros->otimizarPor == "ALUNO" )
   {

   #ifdef TATICO_COM_HORARIOS_HEURISTICO // Resolução do Modelo Tático com Horários por Heurística
      solverTaticoHeur = new SolverTaticoHeur(problemData, problemData->getCampus(campusId));
   #endif // TATICO_COM_HORARIOS_HEURISTICO

      // Variable creation
      varNum = criaVariaveisTatico( campusId, prioridade, r, tatico );
		
      lp->updateLP();

		if ( tatico==2 && prioridade==1 ) teste( 0 );

		#ifdef PRINT_cria_variaveis
         printf( "Total of Variables: %i\n\n", varNum );
		#endif

		// -----------------------------------------------------
		// Deleta todas as variaveis referenciadas em solVarsTatico (e em vars_xh)
		ITERA_GGROUP_LESSPTR ( it, solVarsTatico, VariableTatico )
		{
			delete *it;    
		}
		vars_xh.clear();
		solVarsTatico.clear();
		// -----------------------------------------------------
		
		if ( ! this->CARREGA_SOLUCAO )
		{
		   // Constraint creation
		   constNum = criaRestricoesTatico( campusId, prioridade, r, tatico );

		   lp->updateLP();

			#ifdef PRINT_cria_restricoes
		   printf( "Total of Constraints: %i\n\n", constNum );
			#endif
		   		   
			std::string lpName0;
			lpName0 += "0_";
			lpName0 += string(lpName);

			#ifdef PRINT_LOGS
			lp->writeProbLP( lpName0.c_str() );
			#endif
			
			if ( CARREGA_TESTE ) testeCarregaSol( campusId, prioridade, cjtAlunosId, r, tatico );

		}
   }
   else
   {
		std::cerr<<"\nErro: Parametro otimizarPor deve ser ALUNO!\n";
		exit(1);
   }  

   if ( ! this->CARREGA_SOLUCAO )
   {   
		ofstream outGaps;
		std::string gapFilename( "gap_input" );
		gapFilename += problemData->getInputFileName();
		gapFilename += ".txt";

	   std::set< int > vHashLivresOriginais;
	   
		int *idxN = new int[lp->getNumCols()];
		int *idxs = new int[lp->getNumCols()*2];
		double *vals = new double[lp->getNumCols()*2];
		BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
		int nBds = 0;

		std::cout << "\n=========================================";
	    std::cout << "\nGarantindo solucao...\n"; fflush(NULL);

		VariableTaticoHash::iterator vit = vHashTatico.begin();
		for ( ; vit != vHashTatico.end(); vit++ )
		{
			VariableTatico v = vit->first;
	   
			idxN[vit->second] = vit->second;

			if ( v.getType() == VariableTatico::V_ABERTURA )
			{
				int lb = (int)(lp->getLB(vit->second) + 0.5);
				int ub = (int)(lp->getUB(vit->second) + 0.5);

				if ( lb != ub ) // se for variavel livre
				{
					vHashLivresOriginais.insert( vit->second );

				   idxs[nBds] = vit->second;
				   vals[nBds] = 0.0;
				   bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				   nBds++;
				}
			}
		}
		lp->chgBds(nBds,idxs,bds,vals);
	    
#ifdef SOLVER_CPLEX
		lp->updateLP();
		lp->setTimeLimit( 1e10 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(3);
		lp->setNumIntSols(1);
		lp->setNodeLimit(10000000);
		lp->updateLP();
#elif SOLVER_GUROBI
		lp->updateLP();
		lp->setTimeLimit( 1e10 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(3);
		lp->setNumIntSols(1);
		lp->setNodeLimit(10000000);
		
		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit =  this->getMaxTimeNoImprov(Solver::TAT_HOR1);
		cb_data.gapMax = 80;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif

		lp->updateLP();
#endif

		std::string lpName1;
		lpName1 += "1_";
		lpName1 += string(lpName);

		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName1.c_str() );
		#endif
		
	    double *xS = new double[lp->getNumCols()];
		
		if (tatico==2 && !CRIA_NOVAS_VARIAVEIS_TAT_HOR2) lp->setAdvance(0);

		// GENERATES SOLUTION 
		status = lp->optimize( METHOD_MIP );
		std::cout<<"\nStatus TAT_HOR_BIN1 = "<<status; fflush(NULL);
		lp->getX(xS);
	  	
		if (tatico==2 && !CRIA_NOVAS_VARIAVEIS_TAT_HOR2) lp->setAdvance(1);

		writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN1, xS );
		writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN1, xS, 0 );       
		

		// -------------------------------------------------------------------
		// Volta as variaveis z_{i,d,cp} que estavam livres
		nBds = 0;

		for ( std::set< int >::iterator it = vHashLivresOriginais.begin();
			  it != vHashLivresOriginais.end(); it++)
		{
			idxs[nBds] = *it;
			vals[nBds] = 1.0;
			bds[nBds] = BOUNDTYPE::BOUND_UPPER;
			nBds++;
		}

		lp->chgBds(nBds,idxs,bds,vals);
		lp->updateLP();				
		

	    // -------------------------------------------------------------------
	    // Priorizar atendimento de alunos entrantes (calouros)

		// Já incluso na maximização de atendimento por fases do dia
		//if ( problemData->parametros->priorizarCalouros || problemData->parametros->priorizarFormandos )
		//{
		//	solveMaxAtendCalourosFormandos( campusId, prioridade, cjtAlunosId, r, tatico, CARREGA_SOL_PARCIAL, xS );
		//}
		
	    // -------------------------------------------------------------------
	    
		fflush(NULL);
		

		//solveMaxAtend( campusId, prioridade, cjtAlunosId, r, tatico, CARREGA_SOL_PARCIAL, xS );

		solveMaxAtendPorFasesDoDia( campusId, prioridade, cjtAlunosId, r, tatico, CARREGA_SOL_PARCIAL, xS );


		if ( problemData->parametros->considerar_disponibilidade_prof_em_tatico )
		{
			std::cout << "\n=========================================";
			std::cout << "\nConsiderando a disponibilidade de professores...\n"; fflush(NULL);

			
			// -------------------------------------------------------------------
			// Salvando função objetivo original

			double *objN = new double[lp->getNumCols()];
			lp->getObj(0,lp->getNumCols()-1,objN);
	

			// FUNÇÃO OBJETIVO COM A FOLGA QUE CONSIDERA PROFESSORES -----------------------

			nBds = 0;
			vit = vHashTatico.begin();
			for ( ; vit != vHashTatico.end(); vit++ )
			{
				VariableTatico v = vit->first;
			
				if ( v.getType() != VariableTatico::V_FOLGA_HOR_PROF )
				{            
					idxs[nBds] = vit->second;
					vals[nBds] = 0.0;
					nBds++;
				}
				else
				{
					idxs[nBds] = vit->second;
					vals[nBds] = 1.0;
					nBds++;
				}
			}
			lp->chgObj(nBds,idxs,vals);
			// ------------------------------------------------------------------------------------

			lp->updateLP();
					
			std::string lpName3;
			lpName3 += "3_";
			lpName3 += string(lpName);

			#ifdef PRINT_LOGS
			lp->writeProbLP( lpName3.c_str() );
			#endif
			
			lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);

	#ifdef SOLVER_CPLEX
			lp->setNumIntSols(0);
			lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR3) );			 
			//lp->setMIPRelTol( 0.01 );
			lp->setPreSolve(OPT_TRUE);
			lp->setHeurFrequency(1.0);
			lp->setMIPScreenLog( 4 );
			lp->setPolishAfterTime(100000000);
			lp->setPolishAfterIntSol(100000000);
			lp->setMIPEmphasis(0);
			lp->setPolishAfterNode(1);
			lp->setSymetry(0);
			lp->setCuts(0);
			lp->setPreSolve(OPT_TRUE);
			lp->updateLP();
	#endif
	#ifdef SOLVER_GUROBI
			lp->setNumIntSols(0);
			lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR3) );
			lp->setPreSolve(OPT_TRUE);
			lp->setHeurFrequency(1.0);
			lp->setMIPScreenLog( 4 );
			lp->setPolishAfterTime( 3600 );
			lp->setMIPEmphasis(0);
			lp->setPolishAfterNode(1);
			lp->setSymetry(0);
			lp->setCuts(1);

			#if defined SOLVER_GUROBI && defined USAR_CALLBACK
			cb_data.gapMax = 80;
			cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_HOR3);
			lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
			#endif

			lp->updateLP();
	#endif
				
			if ( CARREGA_SOL_PARCIAL )
			{
				// procura e carrega solucao parcial
				int statusReadBin = readSolTxtTat( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN3, xS, 0 );
				if ( !statusReadBin )
				{
					CARREGA_SOL_PARCIAL=false;
				}
				else writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN3, xS, 0 );
			}
			if ( !CARREGA_SOL_PARCIAL )
			{
				// GENERATES SOLUTION 		 
				status = lp->optimize( METHOD_MIP );
				std::cout<<"\nStatus TAT_HOR_BIN3 = "<<status; fflush(NULL);
				lp->getX(xS);

				writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN3, xS );
				writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN3, xS, 0 );
			}      

			// FIXA A SOLUÇÃO OBTIDA ANTERIORMENTE -------------------------------------------------

			nBds = 0;
			vit = vHashTatico.begin();
			for ( ; vit != vHashTatico.end(); vit++ )
			{
				VariableTatico v = vit->first;

				if (  v.getType() == VariableTatico::V_FOLGA_HOR_PROF )
				{
					idxs[nBds] = vit->second;
					vals[nBds] = xS[vit->second];
					bds[nBds] = BOUNDTYPE::BOUND_UPPER;
					nBds++;
				}			
			}
			
			lp->chgBds(nBds,idxs,bds,vals);
			
			// ------------------------------------------------------------------------------------		
			// Volta a FO original
			lp->chgObj(lp->getNumCols(),idxN,objN);
			lp->updateLP();

			delete[] objN;
		}

		std::cout << "\n=========================================";
	    std::cout << "\nGarantindo o resto dos parametros...\n"; fflush(NULL);

		
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName );
		#endif
		
		lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);

		delete[] idxN;
		delete[] idxs;
		delete[] bds;
		delete[] vals;

#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		//lp->setTimeLimit( tempoDeExecucao );
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR) );
		//lp->setMIPRelTol( 0.01 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime(900);
		lp->setPolishAfterIntSol(100000000);
		lp->setMIPEmphasis(0);
		lp->setPolishAfterNode(10000000000);
		lp->setSymetry(0);
		lp->setCuts(0);
		lp->setPreSolve(OPT_TRUE);
		lp->updateLP();
#endif
#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR) );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime(900);
		lp->setPolishAfterIntSol(100000000);
		lp->setPolishAfterNode(10000000000);
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(1);

		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_HOR);		 
		cb_data.gapMax = 80;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif

		lp->updateLP();
#endif
				
		//polishTaticoHor(xS, 3600, 50, 25,1800);

		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readSolTxtTat( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN, xS, 0 );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN, xS, 0 );
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
			// GENERATES SOLUTION 		 
			status = lp->optimize( METHOD_MIP );
			std::cout<<"\nStatus TAT_HOR_BIN = "<<status; fflush(NULL);
			lp->getX(xS);
	  
			writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN, xS );
			writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN, xS, 0 );
		}      
			  		
		fflush(NULL);

		delete[] xS;	
						
	    // Imprime Gap
		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em SolverMIP::solveTaticoBasicoCjtAlunos().\n";
		}
		else
		{
			outGaps << "Tatico - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
			outGaps << ", r "<< r << ", tatico " << tatico;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n\n";
			outGaps.close();
		}
			
		lp->updateLP();
   }

   return status;
}


int SolverMIP::solveMaxAtendCalourosFormandos( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n=========================================";
	std::cout << "\nGarantindo maximo atendimento para calouros e formandos...\n"; fflush(NULL);
		
	int status = 0;

	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);
				
	// -------------------------------------------------------------------
	// Modificando coeficientes na função objetivo

	bool db=false;

	if ( db )
	std::cout << "\nModificando coeficientes na função objetivo...";fflush(NULL);

	int *idxFO = new int[lp->getNumCols()];
	double *valFO = new double[lp->getNumCols()];
	int nChgFO = 0;

	VariableTaticoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;

		idxN[vit->second] = vit->second;
			
		bool nuloNaFO = true;

		if ( v.getType() == VariableTatico::V_SLACK_DEMANDA )
		{
			if ( db )
			std::cout << "\nv = " << v.toString(); fflush(NULL);

			int turma = v.getTurma();
			Disciplina *disciplina = v.getDisciplina();
			Campus *cp = v.getCampus();

			if ( db )
			std::cout << "\t getting nrs"; fflush(NULL);

			int nCalouros = 0;
			if ( problemData->parametros->priorizarCalouros )
				nCalouros = problemData->getNroCalouros( turma, disciplina, cp->getId() );
		
			if ( db )
			std::cout << "\tnr calouros = " << nCalouros; fflush(NULL);

			int nFormandos = 0;
			if ( problemData->parametros->priorizarFormandos )
				nFormandos = problemData->getNroFormandos( turma, disciplina, cp );

			if ( db )
			std::cout << "\tnr formandos = " << nFormandos; fflush(NULL);

			double coef = 0;
			if ( problemData->parametros->priorizarCalouros ) 
				coef += nCalouros;
			if ( problemData->parametros->priorizarFormandos )
				coef += nFormandos;
						
			if ( coef>0 )
			{
				nuloNaFO = false;
				
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = coef;
				nChgFO++;
			}

			if ( db )
			std::cout << "\t End"; fflush(NULL);
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
	
	if ( db )
	std::cout << "\nCoeficientes modificados!"; fflush(NULL);

	// -------------------------------------------------------------------
	
    char lpName[1024];
    strcpy( lpName, this->getTaticoLpFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );

	std::string lpName2;
	lpName2 += "2_";
	lpName2 += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName2.c_str() );
	#endif
				  				            
	#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR_CALOURO) );
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
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR_CALOURO) );
		lp->setPreSolveIntensity( OPT_LEVEL1 );
		lp->setHeurFrequency(1.0);
		lp->setMIPEmphasis(1);
		lp->setSymetry(0);
		lp->setCuts(0);
		lp->setPolishAfterTime(3600);
		lp->setNodeLimit(10000000);
		lp->setMIPScreenLog( 4 );
		lp->setMIPRelTol( 0.0 );

		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_HOR_CALOURO);
		cb_data.gapMax = 70;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );			
		#endif
	#endif
			
	lp->updateLP();
		
	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolTxtTat( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS, 0 );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS, 0 );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{

#if !defined (DEBUG) && !defined (TESTE)
#ifdef SOLVER_CPLEX
		polishTaticoHor(xS, 3600*2, 90, 15, 1800);
#elif defined SOLVER_GUROBI								
		lp->setCallbackFunc( NULL, NULL );
		polishTaticoHor(xS, 3600*3, 90, 10, 1800);
		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR_CALOURO) );
		lp->updateLP();
#endif
		writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS ); // solucao intermediaria
		writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS, 0 ); // solucao intermediaria
#endif
		
		// GENERATES SOLUTION
		status = lp->optimize( METHOD_MIP );
		lp->getX(xS);
						
		writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS );
		writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS, 0 );
	}		


	#pragma region Imprime Gap	
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";	
	outGaps.open(gapFilename, ofstream::app);
	if ( !outGaps )
	{
		std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em SolverMIP::solveMaxAtendCalourosFormandos().\n";
	}
	else
	{
		outGaps << "Tatico " << tatico << " (Max atendimento calouros e formandos) - campus "
			<< campusId << ", prioridade " << prioridade;
		outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
		outGaps << "\n\n";
		outGaps.close();
	} 
	#pragma endregion
	
	// -------------------------------------------------------------------


	std::cout << "\n=========================================";
	std::cout << "\nFixando atendimento de calouros e formandos obtido...\n"; fflush(NULL);

	int *idxUB = new int[lp->getNumCols()];
	double *valUB = new double[lp->getNumCols()];
	BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()];
	
	int nroFormandosAtendidos=0;
	int nroCalourosAtendidos=0;
	int nroAlunosDemandaAtendidos=0;
	int nChgUB = 0;	
	for ( vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariableTatico::V_SLACK_DEMANDA && xS[vit->second] < 0.1 )
		{
			idxUB[nChgUB] = vit->second;
			valUB[nChgUB] = 0.0;
			bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
			nChgUB++;			

			if ( v.getDisciplina()->getId() > 0 )
			{
				Campus *cp = v.getCampus();
				Disciplina *disciplina = v.getDisciplina();
				int turma = v.getTurma();

				if ( problemData->parametros->priorizarCalouros )
					nroCalourosAtendidos += problemData->getNroCalouros( turma, disciplina, cp->getId() );
				
				if ( problemData->parametros->priorizarFormandos )
					nroFormandosAtendidos += problemData->getNroFormandos( turma, disciplina, cp );

				Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
				trio.set( v.getCampus()->getId(), v.getTurma(), v.getDisciplina() );
				nroAlunosDemandaAtendidos += problemData->mapCampusTurmaDisc_AlunosDemanda[trio].size();
			}
		}
	}

	lp->chgBds(nChgUB,idxUB,bts,valUB);
	  
	if ( problemData->parametros->priorizarCalouros )
		std::cout << "\n\nTotal de calouros-demanda atendidos: "<< nroCalourosAtendidos;
	if ( problemData->parametros->priorizarFormandos )
		std::cout << "\nTotal de formandos-demanda atendidos: "<< nroFormandosAtendidos;

	std::cout << "\nTotal de alunos-demanda atendidos: "<< nroAlunosDemandaAtendidos <<endl<<endl;
	

	// -------------------------------------------------------------------
	// Volta com a função objetivo original

	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    lp->updateLP();
	

	delete[] idxUB;
	delete[] valUB;
	delete[] bts;
	delete[] idxFO;
	delete[] valFO;
	delete[] idxN;
	delete[] objOrig;
	
	return status;
}

int SolverMIP::solveMaxAtend( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n=========================================";
	std::cout << "\nGarantindo maximo atendimento...\n"; fflush(NULL);
		
	int status = 0;

	// -------------------------------------------------------------------
	// Salvando função objetivo original

	double *objN = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objN);
	

	// FUNÇÃO OBJETIVO SOMENTE COM AS FOLGAS DE DEMANDA -----------------------------------

	int *idxN = new int[lp->getNumCols()];
	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
	int nBds = 0;

	#pragma region Modifica FO
    nBds = 0;
	VariableTaticoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		idxN[vit->second] = vit->second;

		VariableTatico v = vit->first;
			
		if ( v.getType() == VariableTatico::V_SLACK_DEMANDA )
		{
			idxs[nBds] = vit->second;
			int nroAlunos = problemData->existeTurmaDiscCampus( v.getTurma(), v.getDisciplina()->getId(), v.getCampus()->getId() );
			vals[nBds] = nroAlunos;
			nBds++;
		}
		else if ( v.getType() == VariableTatico::V_SLACK_SLACKDEMANDA_PT )
		{
			idxs[nBds] = vit->second;
			GGroup<Aluno*, LessPtr<Aluno>> alunosEmComum = problemData->alunosEmComum( v.getTurma1(), v.getDisciplina1(),
																		v.getTurma2(), v.getDisciplina2(), v.getCampus() );           
			vals[nBds] = alunosEmComum.size();
			nBds++;
		}
		else if ( v.getType() == VariableTatico::V_DESALOCA_ALUNO )
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 1;
			nBds++;
		}
		else if ( v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND1 )
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 3;
			nBds++;
		}
		else if ( v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND2 )
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 6;
			nBds++;
		}
		else if ( v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND3 )
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 9;
			nBds++;
		}
		else
		{            
			idxs[nBds] = vit->second;
			vals[nBds] = 0.0;
			nBds++;		
		}
	}

    lp->chgObj(nBds,idxs,vals);
	#pragma endregion

	// ------------------------------------------------------------------------------------

    lp->updateLP();
		
    char lpName[1024];
    strcpy( lpName, this->getTaticoLpFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );

	std::string lpName2;
	lpName2 += "2_";
	lpName2 += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName2.c_str() );
	#endif

				
#ifdef SOLVER_CPLEX
	lp->setNumIntSols(0);
	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR2) );
	lp->setPreSolve(OPT_TRUE);
	lp->setHeurFrequency(1.0);
	lp->setMIPScreenLog( 4 );
	lp->setMIPEmphasis(4);
	//lp->setPolishAfterNode(1);
	lp->setPolishAfterTime(900);
	lp->setSymetry(0);
	lp->setCuts(0);
	lp->updateLP();
#endif
#ifdef SOLVER_GUROBI
	lp->setNumIntSols(0);
	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR2) );
	lp->setPreSolve(OPT_TRUE);
	lp->setHeurFrequency(1.0);
	lp->setMIPScreenLog( 4 );
	lp->setMIPEmphasis(1);
	lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_HOR2) / 2 );
	lp->setSymetry(0);
	lp->setCuts(1);

	#if defined SOLVER_GUROBI && defined USAR_CALLBACK
	cb_data.gapMax = 80;
	cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_HOR2);
	lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
	#endif
#endif

	lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);

	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolTxtTat( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_BIN2, xS, 0 );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_BIN2, xS, 0 );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		// GENERATES SOLUTION 	
         
		if ( tatico == 1 )
		{
		#if !defined (DEBUG) && !defined (TESTE)
			#ifdef SOLVER_GUROBI
				lp->setCallbackFunc( NULL, NULL );
			#endif
			polishTaticoHor(xS,(3 * 3600.0),90.0,25.0,1800);		 
		#endif
		}
#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);	
		#ifndef TESTE			
		if ( tatico == 1 )
		{
			lp->setTimeLimit( 1800 );
			lp->setPolishAfterTime(3600);
		}
		else
		{
		lp->setPolishAfterTime(1800);
		lp->setTimeLimit( 3 * 3600 );//teste //3600 * 3 );
		}
		#endif
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(4);
		//lp->setPolishAfterNode(1);
		lp->setSymetry(0);
		lp->setVarSel(4);
		//lp->setNoCuts();
		lp->setCuts(0);
		lp->updateLP();
#elif defined SOLVER_GUROBI
		lp->setNumIntSols(0);	
	#ifndef TESTE			
		if ( tatico == 1 )
		{
			lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR2) );
			lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_HOR2) / 2 );
		}
		else
		{
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_HOR2) / 2 );
		lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_HOR2) / 3 );
		}
	#endif
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(1);
		lp->setSymetry(0);
		lp->setVarSel(4);
		lp->setCuts(0);		 
		 		 
		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_HOR2);
		cb_data.gapMax = 80;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif

		lp->updateLP();
#endif

#ifdef TATICO_COM_HORARIOS_HEURISTICO // Resolução do Modelo Tático com Horários por Heurística

		CPUTimer timer;
		double dif = 0.0;

		std::cout << "[BUILD] TATICO_COM_HORARIOS_HEURISTICO\n"; fflush(NULL);

		timer.start();
		solverTaticoHeur->build();
		timer.stop();
		dif = timer.getCronoCurrSecs();

		std::cout << "[BUILD] TATICO_COM_HORARIOS_HEURISTICO (" << dif << " sec)\n";

		std::cout << "[SOLVING] TATICO_COM_HORARIOS_HEURISTICO\n"; fflush(NULL);
		solverTaticoHeur->useRandomSeed();
		std::cout << "Using random seed: " << solverTaticoHeur->getSeed() << "\n";  fflush(NULL);

		std::string filename = "instanceHeur" + GUtil::intToString(solverTaticoHeur->getSeed());
		std::cout << "Writing instance: " << filename << "...\n"; fflush(NULL);
		solverTaticoHeur->writeInstance(filename);

		timer.start();
		double timeLimit = 5400.0;
         
#ifdef HEUR_MIP_LOCAL_OPTIMIZATION
		solverTaticoHeur->setVariableTaticoHash(&vHashTatico);
		solverTaticoHeur->setOptLP(lp);

		bool statusHeur = solverTaticoHeur->solve2(timeLimit);

		timer.stop();
		dif = timer.getCronoCurrSecs();

		std::cout << "[SOLVING] TATICO_COM_HORARIOS_HEURISTICO (" << dif << " sec)\n";

		if (statusHeur)
		{
		double objVal = solverTaticoHeur->getObjVal();
		double* xSolHeur = solverTaticoHeur->getX();

		std::cout << "[GET SOLUTION] TATICO_COM_HORARIOS_HEURISTICO\n"; fflush(NULL);

		std::vector<int> indicesHeurMIP;
		std::vector<double> valuesHeurMIP;

		for (VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); ++vit)
		{
			int col = vit->second;
			double value = xSolHeur[col];
			if (value > 0.0001)
			{
				indicesHeurMIP.push_back(col);
				valuesHeurMIP.push_back(value);
			}
		}

		solverTaticoHeur->delX();

		int cntHeur = (int)indicesHeurMIP.size();
		int* indicesHeur = new int[cntHeur];
		double* valuesHeur = new double[cntHeur];

		for (int k=0; k<cntHeur; k++)
		{
			indicesHeur[k] = indicesHeurMIP[k];
			valuesHeur[k] = valuesHeurMIP[k];
		}

		indicesHeurMIP.clear();
		valuesHeurMIP.clear();

		lp->copyMIPStartSol(cntHeur, indicesHeur, valuesHeur);
            
		delete [] indicesHeur;
		delete [] valuesHeur;
		}
#else
		bool statusHeur = solverTaticoHeur->solve(timeLimit);

		timer.stop();
		dif = timer.getCronoCurrSecs();

		std::cout << "[SOLVING] TATICO_COM_HORARIOS_HEURISTICO (" << dif << " sec)\n";

		// read solution
		std::cout << "[GET SOLUTION] TATICO_COM_HORARIOS_HEURISTICO\n"; fflush(NULL);
		std::vector<VariableTatico> solution;
		solverTaticoHeur->writeSolution(solution);

		int cntHeur = (int)solution.size();
		if (cntHeur > 0)
		{
		bool fixaAtendimento = true; // fixa o atendimento da heurística

		int* indicesHeur = NULL;
		double* valuesHeur = NULL;
		if (!fixaAtendimento || !statusHeur)
		{
			indicesHeur = new int[cntHeur];
			valuesHeur = new double[cntHeur];
		}

		//std::cout << "[ATENDIMENTO]\n"; fflush(NULL);

		for (int vth=0; vth < cntHeur; vth++)
		{
			VariableTatico& vh = solution[vth];
			int turma = vh.getTurma();                                 // i
			Disciplina* disciplina = vh.getDisciplina();               // d
			Unidade* unidade = vh.getUnidade();                        // u
			ConjuntoSala* cjtSala = vh.getSubCjtSala();                // tps
			int dia = vh.getDia();                                     // t
			HorarioAula* horarioInicial = vh.getHorarioAulaInicial();  // hi
			HorarioAula* horarioFinal = vh.getHorarioAulaFinal();      // hf

			VariableTaticoHash::iterator vhit = vHashTatico.find(vh);
			if (vhit != vHashTatico.end())
			{
				int col = vhit->second;
				double value = 1.0;
				if (fixaAtendimento && statusHeur)
				{
					lp->chgLB(col, 1.0);
					lp->chgUB(col, 1.0);
				}
				else
				{
					indicesHeur[vth] = col;
					valuesHeur[vth] = value;
				}
			}
			else
			{
				std::cout << "[ERROR] Inconsistencia na hash 'vHashTatico' detectada - Variavel nao encontrada: " << vh.toString() << "\n"; fflush(NULL);
			}

			//std::cout << "Variable { turma = " << turma << "\t disciplina = " << disciplina->getId() << "\t dia = " << dia << "\t inicio = " << horarioInicial->getInicio() << "(" << horarioInicial->getId() << ")\t fim = " << horarioFinal->getInicio() << "(" << horarioFinal->getId() << ")\t tempo = " << horarioInicial->getTempoAula() << " }\n";
				fflush(NULL);
   		}

		if (!fixaAtendimento || !statusHeur)
		{
			lp->copyMIPStartSol(cntHeur, indicesHeur, valuesHeur);
            
			delete [] indicesHeur;
			delete [] valuesHeur;
		}

		lp->writeProbLP("tatico_heur"); fflush(NULL);

		//std::cout << "\nPress enter to continue...\n"; getchar();

		// Otimizando com as variáveis fixadas para acelerar a convegência
		status = lp->optimize(METHOD_MIP);
		lp->getX(xS);

		std::vector<int> indicesHeurMIP;
		std::vector<double> valuesHeurMIP;

		for (VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); ++vit)
		{
			int col = vit->second;
			double value = xS[col];
			if (value > 0.0001 && vit->first.getType() == VariableTatico::V_CREDITOS)
			{
				indicesHeurMIP.push_back(col);
				valuesHeurMIP.push_back(value);
				lp->chgLB(col, 0.0);
				lp->chgUB(col, 1.0);
			}
		}

		int cntHeur = (int)indicesHeurMIP.size();
		indicesHeur = new int[cntHeur];
		valuesHeur = new double[cntHeur];

		for (int k=0; k<cntHeur; k++)
		{
			indicesHeur[k] = indicesHeurMIP[k];
			valuesHeur[k] = valuesHeurMIP[k];
		}

		indicesHeurMIP.clear();
		valuesHeurMIP.clear();

		lp->copyMIPStartSol(cntHeur, indicesHeur, valuesHeur);
            
		delete [] indicesHeur;
		delete [] valuesHeur;
		}
#endif

		// delete solver
		delete solverTaticoHeur;

#endif // TATICO_COM_HORARIOS_HEURISTICO
		 
		status = lp->optimize( METHOD_MIP );	
		std::cout<<"\nStatus TAT_HOR_BIN2 = "<<status; fflush(NULL);
		lp->getX(xS);
			
		writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN2, xS );
		writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, OutPutFileType::TAT_HOR_BIN2, xS, 0 );

#ifdef TATICO_COM_HORARIOS_HEURISTICO
#ifndef HEUR_MIP_LOCAL_OPTIMIZATION
		/*if (cntHeur > 0)
		{
		//std::cout << "[SOLUCAO - MIP]\n"; fflush(NULL);

		for (VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); ++vit)
		{
			int col = vit->second;
			double value = xS[col];
			if (value > 0.0001 && vit->first.getType() == VariableTatico::V_CREDITOS)
			{
				const VariableTatico& vh = vit->first;
				int turma = vh.getTurma();                                 // i
				Disciplina* disciplina = vh.getDisciplina();               // d
				Unidade* unidade = vh.getUnidade();                        // u
				ConjuntoSala* cjtSala = vh.getSubCjtSala();                // tps
				int dia = vh.getDia();                                     // t
				HorarioAula* horarioInicial = vh.getHorarioAulaInicial();  // hi
				HorarioAula* horarioFinal = vh.getHorarioAulaFinal();      // hf

				//std::cout << "Variable { turma = " << turma << "\t disciplina = " << disciplina->getId() << "\t dia = " << dia << "\t inicio = " << horarioInicial->getInicio() << "(" << horarioInicial->getId() << ")\t fim = " << horarioFinal->getInicio() << "(" << horarioFinal->getId() << ")\t tempo = " << horarioInicial->getTempoAula() << " }\n";
				fflush(NULL);
			}
		}
		//std::cout << "\nPress enter to continue...\n"; getchar();
		}*/
		 
		// libera os bounds das variáveis fixadas
		if (cntHeur > 0)
		{
		for (int vth=0; vth < cntHeur; vth++)
		{
			VariableTatico& vh = solution[vth];
			int turma = vh.getTurma();                                 // i
			Disciplina* disciplina = vh.getDisciplina();               // d
			Unidade* unidade = vh.getUnidade();                        // u
			ConjuntoSala* cjtSala = vh.getSubCjtSala();                // tps
			int dia = vh.getDia();                                     // t
			HorarioAula* horarioInicial = vh.getHorarioAulaInicial();  // hi
			HorarioAula* horarioFinal = vh.getHorarioAulaFinal();      // hf

			VariableTaticoHash::iterator vhit = vHashTatico.find(vh);
			if (vhit != vHashTatico.end())
			{
				int col = vhit->second;
				double value = 1.0;
				lp->chgLB(col, 0.0);
				lp->chgUB(col, 1.0);
			}
			else
			{
				std::cout << "[ERROR] Inconsistencia na hash 'vHashTatico' detectada - Variavel nao encontrada: " << vh.toString() << "\n";
				fflush(NULL);
			}
		}
		}
#endif // HEUR_MIP_LOCAL_OPTIMIZATION
#endif // TATICO_COM_HORARIOS_HEURISTICO
	}      
		


	#pragma region Imprime Gap
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";
	outGaps.open(gapFilename, ofstream::app);
	if ( !outGaps )
	{
		std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em SolverMIP::solveTaticoBasicoCjtAlunos().\n";
	}
	else
	{
		outGaps << "Tatico (Max Atend) - campus "<< campusId << ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
		outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
		outGaps << "\n\n";
		outGaps.close();
	} 
	#pragma endregion

	fflush(NULL);
		
	// FIXA SOLUÇÃO OBTIDA ANTERIORMENTE --------------------------------------------------
	#pragma region Fixa solução
    nBds = 0;
	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;

		if (  v.getType() == VariableTatico::V_SLACK_DEMANDA && xS[vit->second] < 0.1 ) // atendido
		{
			int discId = v.getDisciplina()->getId();
			int turma = v.getTurma();
			bool ffdUsada=false;

			VariableTaticoHash::iterator vit2 = vHashTatico.begin();
			for ( ; vit2 != vHashTatico.end(); vit2++ )
			{
				VariableTatico v_ffd = vit2->first;
				if ( v_ffd.getType() == VariableTatico::V_SLACK_SLACKDEMANDA_PT && xS[vit2->second] > 0.1 )
					if ( ( v_ffd.getDisciplina1()->getId() == discId && v_ffd.getTurma1() == turma ) ||
							( v_ffd.getDisciplina2()->getId() == discId && v_ffd.getTurma2() == turma ) )
				{
					ffdUsada = true; break;
				}
			}
			if (!ffdUsada) // Só fixa o atendimento de fd (fd=0) se NÃO existir ffd=1 com o mesmo par turma/disc
			{
				idxs[nBds] = vit->second;
				vals[nBds] = 0.0;
				bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}	
		}
		else if (  v.getType() == VariableTatico::V_SLACK_SLACKDEMANDA_PT && xS[vit->second] < 0.1 ) // atendido
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 0.0;
			bds[nBds] = BOUNDTYPE::BOUND_UPPER;
			nBds++;
		}
		else if (  v.getType() == VariableTatico::V_DESALOCA_ALUNO && xS[vit->second] < 0.1 ) // atendido
		{
			int discId = v.getDisciplina()->getId();
			int turma = v.getTurma();
			int campusId = v.getCampus()->getId();
			bool fdUsada=false;

			VariableTaticoHash::iterator vit2 = vHashTatico.begin();
			for ( ; vit2 != vHashTatico.end(); vit2++ )
			{
				VariableTatico v_fd = vit2->first;
				if ( v_fd.getType() == VariableTatico::V_SLACK_DEMANDA && xS[vit2->second] > 0.1 )
					if ( v_fd.getDisciplina()->getId() == discId &&
							v_fd.getTurma() == turma &&
							v_fd.getCampus()->getId() == campusId )
				{
					fdUsada = true; break;
				}
			}
			if (!fdUsada) // Só fixa o atendimento de fa (fa=0) se NÃO existir fd=1 com o mesmo par turma/disc
			{
				idxs[nBds] = vit->second;
				vals[nBds] = 0.0;
				bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
		}
	}

    lp->chgBds(nBds,idxs,bds,vals);
	#pragma endregion
	// ------------------------------------------------------------------------------------

	

	// -------------------------------------------------------------------
	// Volta com a função objetivo original
	
	lp->chgObj(lp->getNumCols(),idxN,objN);
    lp->updateLP();
	

	delete [] idxs;
	delete [] vals;
	delete [] bds;
	delete [] idxN;
	delete [] objN;
	
	return status;
}


int SolverMIP::solveMaxAtendPorFasesDoDia( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n==========================>>>>";
	std::cout << "\nGarantindo maximo atendimento geral por fases do dia...\n"; fflush(NULL);
		
	int status = 0;
	
	// -------------------------------------------------------------------

    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, cjtAlunosId, r, tatico ).c_str() );
	
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
	double *valFO_Geral = new double[lp->getNumCols()*2];      
	double *valFO_FC = new double[lp->getNumCols()*2];	
	int nChgFO = 0;


	// -------------------------------------------------------------------
	// FO para formandos e calouros

	#pragma region FO para formandos e calouros
	nChgFO = 0;
	for ( VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;

		idxN[vit->second] = vit->second;
		
		double coef = 0.0;
		
		if ( v.getType() == VariableTatico::V_SLACK_DEMANDA )
		{
			int nCalouros = 0;
			if ( problemData->parametros->priorizarCalouros )
				nCalouros = problemData->getNroCalouros( v.getTurma(), v.getDisciplina(), v.getCampus()->getId() );
		
			int nFormandos = 0;
			if ( problemData->parametros->priorizarFormandos )
				nFormandos = problemData->getNroFormandos( v.getTurma(), v.getDisciplina(), v.getCampus() );

			coef = 0;
			if ( problemData->parametros->priorizarCalouros ) 
				coef += nCalouros;
			if ( problemData->parametros->priorizarFormandos )
				coef += nFormandos;			
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
	for ( VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;
											
		double coef = 0.0;
		if ( v.getType() == VariableTatico::V_SLACK_DEMANDA )
		{
			int nroAlunos = problemData->existeTurmaDiscCampus( v.getTurma(), v.getDisciplina()->getId(), v.getCampus()->getId() );	
			coef = nroAlunos;
		}
		else if ( v.getType() == VariableTatico::V_SLACK_SLACKDEMANDA_PT )
		{
			// Penaliza mais o uso de fdd, visto que a turma inteira será deslocada caso essa seja usada.
			int alunos1 = problemData->existeTurmaDiscCampus( v.getTurma1(), v.getDisciplina1()->getId(), v.getCampus()->getId() );
			int alunos2 = problemData->existeTurmaDiscCampus( v.getTurma2(), v.getDisciplina2()->getId(), v.getCampus()->getId() );
			coef = alunos1 + alunos2;
		}
		else if ( v.getType() == VariableTatico::V_DESALOCA_ALUNO )
		{
			coef = 1.0;
		}
		else if ( v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND1 )
		{
			coef = 3.0;
		}
		else if ( v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND2 )
		{
			coef = 6.0;
		}
		else if ( v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND3 )
		{
			coef = 9.0;
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
	
	
	int nChgLB_Q = 0;
	for( auto itFase = problemData->fasesDosTurnos.begin(); itFase != problemData->fasesDosTurnos.end(); itFase++ )
	{		 
		int fase = itFase->first;
		std::cout << "\n======>> Fase " << fase << endl;

		// Fixa não-atendimentos de fases à frente.
		int nChgUB = 0;
		for ( VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
		{
			VariableTatico v = vit->first;
									
			if ( v.getType() == VariableTatico::V_CREDITOS )
			{
				HorarioAula *ha = v.getHorarioAulaInicial();
				int turno = problemData->getFaseDoDia( ha->getInicio() );

				if ( turno > fase )
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
		}
		lp->chgBds( nChgUB, idxUB, btsUB, valUB );

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
				int statusReadBin = readSolTxtTat( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS, fase );
				if ( !statusReadBin )
				{
					CARREGA_SOL_PARCIAL=false;
				}
				else{
					writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS, fase );
					lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );					
				}								
			}
			if ( !CARREGA_SOL_PARCIAL )
			{
					#ifdef SOLVER_CPLEX
						lp->setNumIntSols(0);

						double runtime = 3600.0;					
						if (fase==1) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
						if (fase==2) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
						if (fase==3) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
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
						double runtime = 3600.0;
						if (fase==1) runtime = (double) this->getTimeLimit(Solver::TAT_HOR_CALOURO);
						if (fase==2) runtime = (double) this->getTimeLimit(Solver::TAT_HOR_CALOURO);
						if (fase==3) runtime = (double) this->getTimeLimit(Solver::TAT_HOR_CALOURO);
						lp->setTimeLimit( runtime );
						
						lp->setNumIntSols(0);
						lp->setPreSolveIntensity( OPT_LEVEL1 );
						lp->setHeurFrequency(1.0);
						lp->setMIPEmphasis(1);
						lp->setSymetry(0);
						lp->setCuts(0);
						lp->setPolishAfterTime( runtime * 0.66 );
						lp->setNodeLimit(10000000);
						lp->setMIPScreenLog( 4 );
						lp->setMIPRelTol( 0.0 );

						#if defined SOLVER_GUROBI && defined USAR_CALLBACK
						if (fase==1) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_HOR_CALOURO);
						if (fase==2) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_HOR_CALOURO);
						if (fase==3) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_HOR_CALOURO);
						cb_data.gapMax = 10;
						lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
						#endif
					#endif

					lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );
							
					// GENERATES SOLUTION
					std::cout << "\n\nOptimize...\n\n";
					status = lp->optimize( METHOD_MIP );
					lp->getX(xS);							
		
					writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS );
					writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_CALOURO_BIN, xS, fase );
			}		
			#pragma endregion
						
			#pragma region FIXA ATENDIMENTO DE CALOUROS/FORMANDOS
			// Fixar o atendimento parcial ( fd_{i,d,cp} = 0 )
			int nroFormandosAtendidos=0;
			int nroCalourosAtendidos=0;
			int nroAlunosDemandaAtendidos=0;
			nChgUB = 0;	
			for ( VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
			{
				VariableTatico v = vit->first;
				double upperBound = lp->getUB( vit->second );

				if ( v.getType() == VariableTatico::V_SLACK_DEMANDA && xS[vit->second] < 0.1 )
				{
					idxUB[nChgUB] = vit->second;
					valUB[nChgUB] = 0.0;
					btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
					nChgUB++;			

					if ( v.getDisciplina()->getId() > 0 )
					{
						if ( problemData->parametros->priorizarCalouros )
							nroCalourosAtendidos += problemData->getNroCalouros( v.getTurma(), v.getDisciplina(), v.getCampus()->getId() );
				
						if ( problemData->parametros->priorizarFormandos )
							nroFormandosAtendidos += problemData->getNroFormandos( v.getTurma(), v.getDisciplina(), v.getCampus() );

						Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
						trio.set( v.getCampus()->getId(), v.getTurma(), v.getDisciplina() );
						nroAlunosDemandaAtendidos += problemData->mapCampusTurmaDisc_AlunosDemanda[trio].size();
					}
				}
			}
			lp->chgBds( nChgUB, idxUB, btsUB, valUB );			// Fixa fd = 0

			if ( problemData->parametros->priorizarCalouros )
				std::cout << "\n\nTotal de calouros-demanda atendidos: "<< nroCalourosAtendidos;
			if ( problemData->parametros->priorizarFormandos )
				std::cout << "\nTotal de formandos-demanda atendidos: "<< nroFormandosAtendidos;
			std::cout << "\nTotal de alunos-demanda atendidos: "<< nroAlunosDemandaAtendidos <<endl<<endl;
			#pragma endregion
		}


		std::cout << "\n==>> Maximizacao de atendimento de todos os alunos na fase " << fase << endl;

		std::string maxAtendLpName;
		maxAtendLpName += "maxAtend" + sf + "_";
		maxAtendLpName += string(lpName);

		#ifdef PRINT_LOGS
		lp->writeProbLP( maxAtendLpName.c_str() );
		#endif
				  		
		// Seta FO para maximizar atendimento geral
		lp->chgObj(nChgFO,idxFO,valFO_Geral);
		lp->updateLP();

		#pragma region OTIMIZA OU CARREGA SOLUÇÃO PARA TODOS OS ALUNOS
		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readSolTxtTat( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_BIN2, xS, fase );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else{
				writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_BIN2, xS, fase );
				lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );
					
			}								
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
				#ifdef SOLVER_CPLEX
					lp->setNumIntSols(0);

					double runtime = 3600.0;					
					if (fase==1) runtime = (double) this->getTimeLimit(Solver::TAT_M);
					if (fase==2) runtime = (double) this->getTimeLimit(Solver::TAT_T);
					if (fase==3) runtime = (double) this->getTimeLimit(Solver::TAT_N);
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
					if ( tatico == 1 ){
						if (fase==1) runtime = (double) this->getTimeLimit(Solver::TAT_M);
						if (fase==2) runtime = (double) this->getTimeLimit(Solver::TAT_T);
						if (fase==3) runtime = (double) this->getTimeLimit(Solver::TAT_N);
					}
					if ( tatico == 2 ){
						if (fase==1) runtime = 3600;
						if (fase==2) runtime = 1000;
						if (fase==3) runtime = 3500 * 1.5;
					}
					lp->setTimeLimit( runtime );
					
					lp->setPreSolveIntensity( OPT_LEVEL2 );
					lp->setHeurFrequency(1.0);
					lp->setMIPEmphasis(0);
					lp->setSymetry(0);
					lp->setCuts(0);
					lp->setPolishAfterTime( runtime * 0.66 );
					lp->setNodeLimit(10000000);
					lp->setMIPScreenLog( 4 );
					lp->setMIPRelTol( 0.0 );

					if ( fase==3 ) lp->setPolishAfterTime( 2500 );

					#if defined SOLVER_GUROBI && defined USAR_CALLBACK
					if (fase==1) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_M);
					if (fase==2) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_T);
					if (fase==3) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_N);
					cb_data.gapMax = 10;
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
					#endif
				#endif

				lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );

		
				// GENERATES SOLUTION
				std::cout << "\n\nOptimize...\n\n";
				status = lp->optimize( METHOD_MIP );
				lp->getX(xS);							
		
				writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_BIN2, xS );
				writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_BIN2, xS, fase );

				if ( fase == 3 )
				//if ( ( (fase == 1 || fase == 3) && tatico == 1 ) ||
				//	 ( fase == 3 && tatico == 2 ) )
					 // TODO: não deveria ser hardcoded, a dificuldade da fase deve ser calculavel de alguma forma
				{
					double runtime = 3600;
					if ( fase == 1 && tatico == 1 ) runtime = 3600 * 1;
					if ( fase == 3 && tatico == 1 ) runtime = 3600 * 2;
					if ( fase == 3 && tatico == 2 ) runtime = 3600 * 1;

				// POLISHES THE SOLUTION
				#if !defined (DEBUG) && !defined (TESTE)
				#ifdef SOLVER_CPLEX
					polishTaticoHor(xS, runtime, 60, 15, 1800);
				#elif defined SOLVER_GUROBI								
					lp->setCallbackFunc( NULL, NULL );
					polishTaticoHor(xS, runtime, 70, 20, 1800);
					#if defined SOLVER_GUROBI && defined USAR_CALLBACK
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
					lp->updateLP();
					#endif
				#endif
					writeSolBin( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_BIN2, xS ); // solucao intermediaria
					writeSolTxt( campusId, prioridade, cjtAlunosId, r, tatico, SolverMIP::TAT_HOR_BIN2, xS, fase ); // solucao intermediaria
				#endif
				}
		}		
		#pragma endregion

		// Volta bounds originais de variaveis x_{i,d,s,t,hi,hf} que foram fixadas como NÃO atendidas.
		lp->chgBds( nChgUB, idxUB, btsUB, valOrigUB_0 );

		#pragma region FIXA ATENDIMENTO PARA TODOS OS ALUNOS
		// Fixar o atendimento parcial ( fd_{i,d,cp} = 0  ;  x_{i,d,s,t,hi,hf} = 1 )
		int nAtends = 0;
		int nAtendsSemFfd = 0;
		int nAtendsTotal = 0;
		int nAlDemAtends = 0;
		int nAlDemAtendsPT = 0;
		nChgUB = 0;
		for ( VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
		{
			VariableTatico v = vit->first;
									
			if ( v.getType() == VariableTatico::V_SLACK_DEMANDA )
			{
				double value = (int)( xS[ vit->second ] + 0.5 );
				double ub = (int)( lp->getUB( vit->second ) + 0.5 );
				double lb = (int)( lp->getLB( vit->second ) + 0.5 );

				if ( value == 0.0 )
				{
					nAtendsTotal++;

					if ( lb != ub )
					{
						nAtends++;
						int discId = v.getDisciplina()->getId();
						int turma = v.getTurma();
						bool ffdUsada=false;

						VariableTaticoHash::iterator vit2 = vHashTatico.begin();
						for ( ; vit2 != vHashTatico.end(); vit2++ )
						{
							VariableTatico v_ffd = vit2->first;
							if ( v_ffd.getType() == VariableTatico::V_SLACK_SLACKDEMANDA_PT && xS[vit2->second] > 0.1 )
							if ( ( v_ffd.getDisciplina1()->getId() == discId && v_ffd.getTurma1() == turma ) ||
								 ( v_ffd.getDisciplina2()->getId() == discId && v_ffd.getTurma2() == turma ) )
							{
								ffdUsada = true; break;
							}
						}
						if (!ffdUsada) // Só fixa o atendimento de fd (fd=0) se NÃO existir ffd=1 com o mesmo par turma/disc
						{
							idxUB[nChgUB] = vit->second;
							valUB[nChgUB] = value;
							btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;
							nChgUB++;
							nAtendsSemFfd++;
							
							int nAlunos = problemData->existeTurmaDiscCampus( v.getTurma(), v.getDisciplina()->getId(), v.getCampus()->getId() );
							nAlDemAtendsPT += nAlunos;
							if ( v.getDisciplina()->getId() > 0 )
								nAlDemAtends += nAlunos;
						}					
					}
				}
			}
			else if ( v.getType() == VariableTatico::V_CREDITOS )
			{
				HorarioAula *ha = v.getHorarioAulaInicial();
				int turno = problemData->getFaseDoDia( ha->getInicio() );

				if ( turno <= fase )
				{
					double value = (int)( xS[ vit->second ] + 0.5 );				
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );

					if ( value == 1 )
					if ( lb != ub )
					{
						valOrigLB_Q[nChgLB_Q] = lp->getLB( vit->second );
						idxLB_Q[nChgLB_Q] = vit->second;
						valLB_Q[nChgLB_Q] = value;
						btsLB_Q[nChgLB_Q] = BOUNDTYPE::BOUND_LOWER;
						nChgLB_Q++;
					}
					if ( value == 0 )
					if ( lb != ub )
					{
						valOrigLB_Q[nChgLB_Q] = lp->getUB( vit->second );
						idxLB_Q[nChgLB_Q] = vit->second;
						valLB_Q[nChgLB_Q] = value;
						btsLB_Q[nChgLB_Q] = BOUNDTYPE::BOUND_UPPER;
						nChgLB_Q++;
					}
				}
			}
		}
		lp->chgBds( nChgUB, idxUB, btsUB, valUB );			// Fixa fd = 0
		lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valLB_Q );	// Fixa x = 1  e  x = 0

		std::cout << "\n---> Total de novas turmas p+t sem ffd atendidas ao fim da fase " << fase << ": " << nAtendsSemFfd;
		std::cout << "\n---> Total de novas turmas p+t atendidas ao fim da fase " << fase << ": " << nAtends;
		std::cout << "\n---> Total de turmas p+t atendidas ao fim da fase " << fase << ": " << nAtendsTotal;
		std::cout << "\n\n---> Total de aluno-demanda p+t atendidos ao fim da fase " << fase << ": " << nAlDemAtendsPT;
		std::cout << "\n---> Total de aluno-demanda atendidos ao fim da fase " << fase << ": " << nAlDemAtends << endl;
		#pragma endregion

		#pragma region Imprime Gap
	 	// Imprime Gap
		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em SolverMIP::solveMaxAtendPorFasesDoDia().\n";
		}
		else
		{
			outGaps << "Tatico (Max atendimento) - fase do dia " << fase << " - campus "<< campusId 
				<< ", cjtAlunos " << cjtAlunosId << ", prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
		} 
		#pragma endregion
		
	}

	// Volta lower bounds originais de variaveis x_{i,d,s,t,hi,hf}
	// Se não voltar, talvez tenha que mudar para só fixar quando fdd for zero
	lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valOrigLB_Q );

	// Mantem as fixações fd_{i,d,cp} = 0

	std::cout << "\n==========================>>>>";
	std::cout << "\nFixando maximo atendimento obtido e abertura de turmas...\n"; fflush(NULL);

	int nroTurmas = 0;
	int nChgUB = 0;
	for ( VariableTaticoHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;
		double value = (int)( xS[ vit->second ] + 0.5 );

		if ( v.getType() == VariableTatico::V_SLACK_SLACKDEMANDA_PT && value < 0.1 ) // fdd: atendido
		{
			double ub = (int)( lp->getUB( vit->second ) + 0.5 );
			double lb = (int)( lp->getLB( vit->second ) + 0.5 );

			if ( lb != ub )
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 0.0;
				btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
				nChgUB++;
			}
		}
		else if (  v.getType() == VariableTatico::V_DESALOCA_ALUNO && value < 0.1 ) // fa: atendido
		{
			int discId = v.getDisciplina()->getId();
			int turma = v.getTurma();
			int campusId = v.getCampus()->getId();
			bool fdUsada=false;

			VariableTaticoHash::iterator vit2 = vHashTatico.begin();
			for ( ; vit2 != vHashTatico.end(); vit2++ )
			{
				VariableTatico v_fd = vit2->first;
				if ( v_fd.getType() == VariableTatico::V_SLACK_DEMANDA && xS[vit2->second] > 0.1 )
				if ( v_fd.getDisciplina()->getId() == discId &&
					 v_fd.getTurma() == turma &&
					 v_fd.getCampus()->getId() == campusId )
				{
					fdUsada = true; break;
				}
			}
			if (!fdUsada) // Só fixa o atendimento de fa (fa=0) se NÃO existir fd=1 com o mesmo par turma/disc
			{
				idxUB[nChgUB] = vit->second;
				valUB[nChgUB] = 0.0;
				btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
				nChgUB++;
			}
		}
		else if ( v.getType() == VariableTatico::V_ABERTURA && value > 0.1 )
		{
			// Conta nro de turmas abertas
			nroTurmas++;
		}
	}



	lp->chgBds(nChgUB,idxUB,btsUB,valUB);	// Fixa ffd = 0  &  fa = 0
	lp->updateLP();
	  
	std::cout << "\nNumero de turmas abertas: "<< nroTurmas <<endl<<endl;

				  				            
	
	// -------------------------------------------------------------------
	// Volta com a função objetivo original

	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    lp->updateLP();
	
	std::cout << "\n================================================================================";
	
	
	delete [] idxLB_Q;
	delete [] valLB_Q;
	delete [] btsLB_Q;
	delete [] valOrigLB_Q;

	delete [] idxUB;
	delete [] valUB;
	delete [] btsUB;
	delete [] valOrigUB_0;
	
	delete [] idxFO;
	delete [] valFO_Geral;
	delete [] valFO_FC;
	delete [] idxN;
	delete [] objOrig;

	return status;
}


void SolverMIP::atualizaFolgasAlunoDemanda( int campusId )
{
	ITERA_GGROUP_LESSPTR( itAlDem, problemData->alunosDemanda, AlunoDemanda )
	{
		Aluno *aluno = itAlDem->getAluno();
		Disciplina *disciplina = itAlDem->demanda->disciplina;

		if ( itAlDem->getOferta()->getCampusId() == campusId )
		{
			if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) == -1 )
			{
				problemData->listSlackDemandaAluno.add( *itAlDem );
			}
		}
	}
}

/*
Algoritmo para, a partir de uma solução do modo Tático do Trieda, determinar
o motivo de não atendimento de um par [aluno, disciplina].
*/
void SolverMIP::verificaNaoAtendimentos( int campusIdAtual, int P )
{
	std::cout<<"\nVerificando nao atendimentos para prioridade " << P << " e campus " << campusIdAtual << "...\n";
	fflush(NULL);	

	/* 		
	Possiveis restricoes:
	* Salas lotadas
	* Choque de horarios
	* Disciplina criada um outro turno
	* Demanda minima nao atingida
	* Nao ha sala disponivel para criar a disciplina
	*/
	
	// ------------------------------------------------------------------------------------------
	// Conjunto de solucoes para cada turma criada
	map< int /*campusId*/, map< Disciplina*, map< int /*turma*/, vars__X__i_d_u_s_hi_hf_t >, LessPtr<Disciplina> > >::iterator itMapSol;
	map< int /*campusId*/, map< Disciplina*, map< int /*turma*/, vars__X__i_d_u_s_hi_hf_t >, LessPtr<Disciplina> > > mapDisciplina_Solucoes;
	ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
	{
		int cpId = (*it_Vars_x)->getUnidade()->getIdCampus();
		int turma = (*it_Vars_x)->getTurma();
		Disciplina *disciplina = (*it_Vars_x)->getDisciplina();

		mapDisciplina_Solucoes[cpId][disciplina][turma].add( (*it_Vars_x) );
	}

	// ------------------------------------------------------------------------------------------
	// Alunos-Demanda não atendidos
	map< Disciplina*, map< TurnoIES*, GGroup<AlunoDemanda*, 
		LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >, LessPtr<Disciplina> > mapDiscTurnoAlDemNaoAtend;

	ITERA_GGROUP_LESSPTR(it_alunos_demanda, problemData->alunosDemanda, AlunoDemanda)
	{
		// Solicitacao do aluno
		AlunoDemanda* ad = (*it_alunos_demanda);		

		if ( ad->getCampus()->getId() != campusIdAtual )
			continue;
		
		if ( P>1 && ad->getAluno()->totalmenteAtendido() )
			continue;

		int nTurma = problemData->retornaTurmaDiscAluno(ad->getAluno(), ad->demanda->disciplina);
		
		if(nTurma == -1)//n foi atendido
		{
			mapDiscTurnoAlDemNaoAtend[ad->demanda->disciplina][ad->getOferta()->turno].add( ad );
		}
	}

	// ------------------------------------------------------------------------------------------
	
	std::map<int, std::vector<string> > aluno_saida_em_txt;
	
	map< Disciplina*, map< TurnoIES*, GGroup<AlunoDemanda*, 
		LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >, LessPtr<Disciplina> >::iterator itDisc = mapDiscTurnoAlDemNaoAtend.begin();
	for ( ; itDisc != mapDiscTurnoAlDemNaoAtend.end(); itDisc++ )
	{	
		map< TurnoIES*, GGroup<AlunoDemanda*, 
			LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > *mapTurnoAlDemNaoAtend = & itDisc->second;

		map< TurnoIES*, GGroup<AlunoDemanda*, 
			LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >::iterator itTurno = (*mapTurnoAlDemNaoAtend).begin();
		for ( ; itTurno != (*mapTurnoAlDemNaoAtend).end(); itTurno++ )
		{
			GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> *alDemNaoAtend = & itTurno->second;
			ITERA_GGROUP_LESSPTR(itAlDem, *alDemNaoAtend, AlunoDemanda)
			{
				AlunoDemanda* ad = (*itAlDem);

			//	std::cout << "\nVerificando nao atendimento do AlunoDemanda id " 
			//		<< ad->getId() << ", aluno " << ad->getAlunoId() << ", disc " << ad->demanda->disciplina->getId();

				bool bd = false;
				if ( ad->getAlunoId() == 53254 && ad->demanda->disciplina->getId() == -16004 ){
					//bd = true;
				}

				int campusAlunoId = ad->getCampus()->getId();
				Aluno *aluno = ad->getAluno();
				int alunoId = ad->getAluno()->getAlunoId();
				int alDemId = ad->getId();
				int turnoIESid = ad->demanda->getTurnoIES()->getId();
				TurnoIES* turnoIES = ad->demanda->getTurnoIES();
				string turnoIESname = ad->demanda->getTurnoIES()->getNome();

				if (bd) std::cout<<"\n1"; fflush(NULL);

				aluno_saida_em_txt[ alunoId ].push_back("\n\n********* ALUNO: " + 
					ad->getAluno()->getNomeAluno() + "\t id = " + std::to_string( (long double) alunoId ) + " **********\n");
			
				NaoAtendimento* naoAtendimento = new NaoAtendimento( alDemId );

				if (bd) std::cout<<"\n2"; fflush(NULL);

				// Verifica se alguma turma para o campus foi criada
				itMapSol = mapDisciplina_Solucoes.find( campusAlunoId );
				if ( itMapSol == mapDisciplina_Solucoes.end() )
				{
					stringstream ss;
					ss << "-- NENHUMA TURMA NO CAMPUS " << campusAlunoId << endl;
					aluno_saida_em_txt[ alunoId ].push_back( ss.str() );

					stringstream ss2;
					ss2 << "Nenhuma turma de nenhuma disciplina criada no campus " << ad->getCampus()->getCodigo() << ".";
					naoAtendimento->addMotivo( ss2.str() );
				
					problemSolution->nao_atendimentos->add( naoAtendimento );

					continue;
				}

				if (bd) std::cout<<"\n3"; fflush(NULL);

				GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas;
				disciplinas.add(ad->demanda->disciplina);

				if ( problemData->parametros->considerar_equivalencia_por_aluno )
				{
					if ( ad->demandaOriginal==NULL )
					{
						ITERA_GGROUP_LESSPTR(itDiscEquiv, ad->demanda->disciplina->discEquivSubstitutas, Disciplina)
						{	
							if ( problemData->alocacaoEquivViavel( ad->demanda, *itDiscEquiv ) )
								disciplinas.add( *itDiscEquiv );
						}
					}
					else
						disciplinas.add(ad->demandaOriginal->disciplina);
				}

				if (bd) std::cout<<"\n4"; fflush(NULL);

				ITERA_GGROUP_LESSPTR(it_disciplina, disciplinas, Disciplina)
				{				
					Disciplina *discEquiv = *it_disciplina;

					bool discEhEquiv;
					if ( ad->demandaOriginal==NULL )
						discEhEquiv = discEquiv->getId() != ad->demanda->getDisciplinaId();
					else
						discEhEquiv = discEquiv->getId() != ad->demandaOriginal->getDisciplinaId();

					if (bd) std::cout<<"\n\t4.1"; fflush(NULL);

					// Verifica se alguma turma no campus para a disciplina foi criada
					map< Disciplina*, map< int /*turma*/, vars__X__i_d_u_s_hi_hf_t >, LessPtr<Disciplina> > *mapDiscsSol = & itMapSol->second;
					map< Disciplina*, map< int /*turma*/, vars__X__i_d_u_s_hi_hf_t >, LessPtr<Disciplina> >::iterator
						itDiscSol = (*mapDiscsSol).find( discEquiv );
					if ( itDiscSol != (*mapDiscsSol).end() )
					{
				if (bd) std::cout<<"\n\t4.2"; fflush(NULL);

						/* 
						Disciplina Foi Criada
						- Capacidade da sala
						- Se houve choque de horario
						- Verificar se foi em outro turno
						*/
						#pragma region Disciplina_Criada

						// Indica se alguma turma foi aberta no turno da oferta
						bool haTurmaMesmoTurno = false;

						// Conjunto de turmas que estao com salas lotadas
						GGroup< std::pair<int /*turma*/, Sala*> > salas_lotadas;

						// Conjunto de turmas em que houve choque de horario
						map< int /*turma existente*/, map< Trio< int , int , Disciplina* >, vars__X__i_d_u_s_hi_hf_t > > turmas_choque_horarios;
					
						GGroup< int /*turmas existentes*/ > turmasForaDoTurno;

						int totalTurmasCriadas = itDiscSol->second.size();

						// Itera sobre todas as turmas da disciplina nao alocada do aluno
						map< int /*turma*/, vars__X__i_d_u_s_hi_hf_t > *turmasCriadas = & itDiscSol->second;
						map< int /*turma*/, vars__X__i_d_u_s_hi_hf_t >::iterator itTurmas = (*turmasCriadas).begin();
						for ( ; itTurmas != (*turmasCriadas).end(); itTurmas++ )
						{
							int turma = itTurmas->first;						
						
							Trio< int , int , Disciplina* > trio_alocado(campusAlunoId, turma, discEquiv);
							int turmaSize = problemData->mapCampusTurmaDisc_AlunosDemanda[trio_alocado].size();

							ITERA_GGROUP_LESSPTR( it_solucao, itTurmas->second, VariableTatico )
							{
								HorarioAula *hi = (*it_solucao)->getHorarioAulaInicial();
								HorarioAula *hf = (*it_solucao)->getHorarioAulaFinal();
								DateTime inicio = hi->getInicio();
								DateTime fim = hf->getFinal();
								int dia = (*it_solucao)->getDia();
								Sala* sala = (*it_solucao)->getSubCjtSala()->getSala();

								if ( ad->getOferta()->turno->possuiHorarioDiaOuCorrespondente(hi,hf,dia) )
								{								
									if((unsigned) sala->getCapacidade() > turmaSize )
									{
										/*  Ha vaga na sala;
											Checar se houve choque de horario 
										*/

										// Turmas em que o aluno foi alocado
										GGroup< Trio< int , int , Disciplina* > > trios_aluno = problemData->mapAluno_CampusTurmaDisc[ad->getAluno()];

										GGroup< Trio< int , int , Disciplina* > >::iterator it_trios = trios_aluno.begin();
										for( ; it_trios != trios_aluno.end(); it_trios++ )
										{
											Trio< int , int , Disciplina* > trio_aluno = (*it_trios);
											int campus_aluno = trio_aluno.first;
											int turma_aluno = trio_aluno.second;
											Disciplina* disc_aluno = trio_aluno.third;
				
											vars__X__i_d_u_s_hi_hf_t aulasAluno = mapDisciplina_Solucoes[campus_aluno][disc_aluno][turma_aluno];
										
											ITERA_GGROUP_LESSPTR( it_aula_aluno, aulasAluno, VariableTatico )
											{
												int dia_aula = (*it_aula_aluno)->getDia();

												if( dia_aula == dia)									
												{
													//encontrou uma disciplina do aluno no mesmo dia

													DateTime inicio_aluno = (*it_aula_aluno)->getHorarioAulaInicial()->getInicio();
													DateTime fim_aluno = (*it_aula_aluno)->getHorarioAulaFinal()->getFinal();

													if( (inicio_aluno >= inicio && inicio_aluno < fim) ||
														(inicio >= inicio_aluno && inicio < fim_aluno ) )
													{
														turmas_choque_horarios[turma][trio_aluno].add( (*it_aula_aluno) );
													}
												}
											}
										}
									}
									else
									{
										/* Sala lotada */
										std::pair<int /*turma*/, Sala*> lotacao(turma,sala);
										salas_lotadas.add(lotacao);
									}
								}
								else
								{
									turmasForaDoTurno.add( turma );
								}
							}
						}
					
						if( turmasForaDoTurno.size() == totalTurmasCriadas )
						{
							// TODAS AS TURMAS CRIADAS ESTÃO EM TURNOS DIFERENTES
							stringstream ss;
							ss << "-- TODAS AS TURMAS CRIADAS ESTAO EM TURNOS DIFERENTES PARA A DISCIPLINA ID " 
								<< std::to_string( (long double) discEquiv->getId()) 
								<< ( discEhEquiv ? " (EQUIVALENTE)":"" ) << "\n";
							aluno_saida_em_txt[alunoId].push_back( ss.str() );

							stringstream ss2;
							ss2 << "Todas as turmas da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente) ":" ")
									<< "criadas estão em turnos diferentes do turno do aluno.";
							naoAtendimento->addMotivo( ss2.str() );
						}
						else
						{
							#pragma region Salas lotadas
							int nroSalasLotadas = salas_lotadas.size();
							if ( nroSalasLotadas == 1 )
							{																
								stringstream ss;
								ss << "A turma " << (*salas_lotadas.begin()).first 
									<< " da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ") 
									<< " está lotada na sala " << (*salas_lotadas.begin()).second->getCodigo() << ".";
								naoAtendimento->addMotivo( ss.str() );
							}
							else if ( nroSalasLotadas > 1 )
							{
								stringstream ss;
								ss << "As turmas ";
								int counter=0;
								GGroup< std::pair<int /*turma*/, Sala*> >::iterator it_salas_lotadas = salas_lotadas.begin();
								for(;it_salas_lotadas != salas_lotadas.end(); it_salas_lotadas++)
								{
									counter++;
									ss << (*it_salas_lotadas).first;
									if ( counter + 1 < nroSalasLotadas ) ss << ", ";			// antes do penultimo
									else if ( counter + 1 == nroSalasLotadas ) ss << " e ";		// penultimo
								}

								ss << " da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ") 
									<< " estão lotadas.";					
							
								naoAtendimento->addMotivo( ss.str() );
							}
							#pragma endregion

							#pragma region Turmas fora do turno do aluno
							int nroTurmasForaDoTurno = turmasForaDoTurno.size();
							if ( nroTurmasForaDoTurno == 1 )
							{																
								stringstream ss;
								ss << "A turma " << *(turmasForaDoTurno.begin())
									<< " da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ")
									<< " está em turno diferente do turno do aluno.";
								naoAtendimento->addMotivo( ss.str() );
							}
							else if ( nroTurmasForaDoTurno > 1 )
							{
								stringstream ss;
								ss << "As turmas ";
								int counter=0;
								ITERA_GGROUP_N_PT( itTurma, turmasForaDoTurno, int )
								{
									counter++;
									ss << *itTurma;
									if ( counter + 1 < nroTurmasForaDoTurno ) ss << ", ";			// antes do penultimo
									else if ( counter + 1 == nroTurmasForaDoTurno ) ss << " e ";	// penultimo
								}

								ss << "da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ")
									<< " estão em turno diferente do turno do aluno.";								
							
								naoAtendimento->addMotivo( ss.str() );
							}
							#pragma endregion

							map< int, map< Trio< int , int , Disciplina* >, vars__X__i_d_u_s_hi_hf_t > >::iterator 
								it_turma_criada = turmas_choque_horarios.begin();
							for(;it_turma_criada != turmas_choque_horarios.end(); it_turma_criada++)
							{
								int turmaCriada = it_turma_criada->first;

								map< Trio< int , int , Disciplina* >, vars__X__i_d_u_s_hi_hf_t > ::iterator 
									it_turmas_choque_horarios = it_turma_criada->second.begin();
								for(;it_turmas_choque_horarios != it_turma_criada->second.end(); it_turmas_choque_horarios++)
								{
									// HOUVE CHOQUE DE HORARIO COM OUTRAS AULAS DO ALUNO
									stringstream ss2;
									ss2 << "A turma " << turmaCriada << " da disciplina " << discEquiv->getCodigo()
										<< " tem choque nos horários do aluno com a turma " << (*it_turmas_choque_horarios).first.second
										<< " da disciplina " << (*it_turmas_choque_horarios).first.third->getCodigo()
										<< " no campus " << (*it_turmas_choque_horarios).first.first << ".";
									naoAtendimento->addMotivo( ss2.str() );
				
									stringstream ss;
									ss << "-- CHOQUE DE HORARIO COM A TURMA " << (*it_turmas_choque_horarios).first.second
										<< " DA DISCIPLINA " << (*it_turmas_choque_horarios).first.third->getId()
										<< " NO CAMPUS " << (*it_turmas_choque_horarios).first.first << "\n";
									aluno_saida_em_txt[alunoId].push_back(ss.str());
									ITERA_GGROUP_LESSPTR( it_aula_aluno, it_turmas_choque_horarios->second, VariableTatico )
									{
										stringstream ss;
										ss 	<< "\tNO DIA " << (*it_aula_aluno)->getDia()
											<< " DE " << (*it_aula_aluno)->getHorarioAulaInicial()->getInicio()
											<< " ATE " << (*it_aula_aluno)->getHorarioAulaFinal()->getFinal() << "\n";
										aluno_saida_em_txt[alunoId].push_back(ss.str());
									}
								}
							}
						}

						if ( totalTurmasCriadas == discEquiv->getNumTurmas() )
						{
							// FOI CRIADO O MAXIMO DE TURMAS
							stringstream ss;
							ss << "-- MAXIMO DE TURMAS ATINGIDO PARA A DISCIPLINA ID " 
								<< std::to_string( (long double) discEquiv->getId())
								<< ( discEhEquiv ? " (EQUIVALENTE)":"" )
								<< ": " << totalTurmasCriadas << "TURMAS CRIADAS\n";
							aluno_saida_em_txt[alunoId].push_back( ss.str() );						
						
							stringstream ss2;
							ss2 << "Não é possível abrir mais turmas para a disciplina " << discEquiv->getCodigo()
								<< ( discEhEquiv ? " (equivalente)":"" ) << ".";
							naoAtendimento->addMotivo( ss2.str() );
						}
						else
						{
							// NAO FOI CRIADO O MAXIMO DE TURMAS
						
							map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >							
								mapSalaDiaHorsLivres = this->procuraCombinacaoLivreEmSalas( discEquiv, ad->demanda->getTurnoIES(), campusAlunoId );
			
							// Sem salas com horários livres suficientes
							if ( mapSalaDiaHorsLivres.size() == 0 )
							{
								stringstream ss;						
								stringstream ss2;
								ss2 << "Não existe sala com horários livres suficientes para abrir mais turmas da disciplina " 
								<< discEquiv->getCodigo() << ( discEhEquiv ? " (equivalente)":"" ) << " no turno " <<
								ad->demanda->getTurnoIES()->getNome() << ".";
								naoAtendimento->addMotivo( ss2.str() );
													
								ss << "-- NAO EXISTE SALA COM HORARIOS LIVRES SUFICIENTES PARA MAIS TURMAS DA DISCIPLINA ID " 
								<< std::to_string( (long double) discEquiv->getId())
								<< ( discEhEquiv ? " (EQUIVALENTE)":"" ) << " no turno " <<	ad->demanda->getTurnoIES()->getNome() << ".";
								aluno_saida_em_txt[alunoId].push_back( ss.str() );
							}
							// Há salas com horários livres suficientes
							else
							{
								// Verifica para cada sala com horário livre se há choque de horários com o aluno
								
								stringstream ss1SemChoque;
								ss1SemChoque << "Provavelmente não há alunos com horários livres em comum suficientes "
											<< "para abrir nova turma da disciplina " << discEquiv->getCodigo()
											<< ( discEhEquiv ? " (equivalente)":"" );
								stringstream ss2SemChoque;
								int opcaoSemChoque=0;

								stringstream ss1ComChoque;
								stringstream ss2ComChoque;
								ss1ComChoque << "Não há horários livres em comum com o aluno suficientes "
											<< "para abrir mais turmas da disciplina " << discEquiv->getCodigo()
											<< ( discEhEquiv ? " (equivalente)":"" );
								int opcaoComChoque=0;

								map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >::iterator
									itSala = mapSalaDiaHorsLivres.begin();
								for ( ; itSala != mapSalaDiaHorsLivres.end(); itSala++ )
								{
									Sala *sala = itSala->first;

									map< int, GGroup< Trio<int,int,Disciplina*> > > choques =
										procuraChoqueDeHorariosAluno( aluno, itSala->second );

									if ( choques.size() == itSala->second.size() )	// Todas as opcoes da sala têm choques
									{
										opcaoComChoque++;
										ss2ComChoque << sala->getCodigo() << "; ";
									}
									else											// Há opção sem choque
									{		
										opcaoSemChoque++;
										ss2SemChoque << sala->getCodigo() << "; ";		
									}
								}

								if ( opcaoSemChoque )
								{
									stringstream ss;
									ss << ss1SemChoque.str();

									if ( opcaoSemChoque == 1 ) ss << " na sala ";
									else ss << " nas salas ";
									
									ss << ss2SemChoque.str();

									naoAtendimento->addMotivo( ss.str() );
								}
								if ( opcaoComChoque )
								{
									stringstream ss;
									ss << ss1ComChoque.str();

									if ( opcaoComChoque == 1 ) ss << " na sala ";
									else ss << " nas salas ";
									
									ss << ss2ComChoque.str();

									naoAtendimento->addMotivo( ss.str() );
								}
							}							
						}
						#pragma endregion
					}
					else
					{
						if (bd) std::cout<<"\n\t4.3"; fflush(NULL);

						/* 
						Disciplina Nao Foi Criada
						- verificar se a disciplina poderia ser criada no turno ofertado
						- Verificar se houve demanda suficiente
						- Verificar se havia sala disponivel para criar a disciplina
						*/
						#pragma region Disciplina_Nao_Criada
					
						if ( !discEquiv->possuiTurnoIES( turnoIES ) )
						{
							// DISCIPLINA COM HORARIO DISPONIVEL EM TURNO DIFERENTE DA OFERTA
							stringstream ss;
							ss << "-- A DISCIPLINA " 
								<< discEquiv->getCodigo() 
								<< ( discEhEquiv ? " (EQUIVALENTE)":" " )
								<< ", ID" << std::to_string( (long double) discEquiv->getId()) << ", "
								<< "NAO PODE SER MINSTRADA NO TURNO DO ALUNO-DEMANDA: "
								<< ad->demanda->getTurnoIES()->getNome() << "\n";
							aluno_saida_em_txt[ad->getAluno()->getAlunoId()].push_back( ss.str() );
						
							stringstream ss2;
							ss2 << "A disciplina " 
								<< discEquiv->getCodigo() 
								<< ( discEhEquiv ? " (EQUIVALENTE)":" " )
								<< ", não tem horários suficientes disponíveis no turno "
								<< ad->demanda->getTurnoIES()->getNome() << " associado ao aluno.";
							naoAtendimento->addMotivo( ss2.str() );
				
							continue;		
						}
					
						if (bd) std::cout<<"\n\t\t4.31"; fflush(NULL);

						GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alsDem 
							= problemData->retornaDemandasDiscNoCampus( discEquiv->getId(), ad->getCampus()->getId(), ad->getPrioridade() );

						int demandaSizeNoTurnoIES = 0;
						if ( ( problemData->parametros->min_alunos_abertura_turmas && !discEquiv->eLab() ) ||
							 ( problemData->parametros->min_alunos_abertura_turmas_praticas && discEquiv->eLab() ) )
						{
							bool formando = false;
								
							if (bd) std::cout<<"\n\t\t4.32"; fflush(NULL);

							ITERA_GGROUP_LESSPTR(it_als_dem, alsDem, AlunoDemanda)
							{							
								if ( (*it_als_dem)->demanda->getTurnoIES()->getId() == turnoIESid )
								{
									demandaSizeNoTurnoIES++;

									if ( (*it_als_dem)->getAluno()->ehFormando() )
									{
										formando = true;								
									}
								}
							}
		
							if (bd) std::cout<<"\n\t\t4.33"; fflush(NULL);

							if ( ( !discEquiv->eLab() && demandaSizeNoTurnoIES < problemData->parametros->min_alunos_abertura_turmas_value ) ||
								 ( discEquiv->eLab() && demandaSizeNoTurnoIES < problemData->parametros->min_alunos_abertura_turmas_praticas_value ) )
							if ( ( problemData->parametros->violar_min_alunos_turmas_formandos && !formando ) ||
								 ( !problemData->parametros->violar_min_alunos_turmas_formandos ) )
							{
								// DEMANDA MINIMA NAO ATINGIDA
								stringstream ss;
								ss << "-- DEMANDA MINIMA NAO ATIGIDA PELA DISCIPLINA " 
									<< discEquiv->getCodigo() 
									<< ( discEhEquiv ? " (EQUIVALENTE)":" " )
									<< ", ID" << std::to_string( (long double) discEquiv->getId())
									<< ", NO TURNO " << turnoIESid << "\n";	
								aluno_saida_em_txt[alunoId].push_back( ss.str() );

								stringstream ss2;
								ss2 << "Demanda mínima não atingida pela disciplina " 
									<< discEquiv->getCodigo()
									<< ( discEhEquiv ? " (equivalente)":" " )
									<< ", no turno " << turnoIESname << ".";
								naoAtendimento->addMotivo( ss2.str() );
				
								problemSolution->nao_atendimentos->add( naoAtendimento );

								continue;
							}
						}
		
						if (bd) std::cout<<"\n\t\t4.34"; fflush(NULL);

						// VERIFICA SALAS
						
						map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >							
							mapSalaDiaHorsLivres = this->procuraCombinacaoLivreEmSalas( discEquiv, ad->demanda->getTurnoIES(), campusAlunoId );
									
						if ( mapSalaDiaHorsLivres.size() == 0 )		// Não há salas com horários livres suficientes
						{
							stringstream ss;						
							stringstream ss2;
							ss2 << "Não existe sala com horários livres suficientes para abrir turmas da disciplina " 
							<< discEquiv->getCodigo() << ( discEhEquiv ? " (equivalente)":"" ) << ".";
							naoAtendimento->addMotivo( ss2.str() );
													
							ss << "-- NAO EXISTE SALA COM HORARIOS LIVRES SUFICIENTES PARA TURMAS DA DISCIPLINA ID " 
							<< std::to_string( (long double) discEquiv->getId())
							<< ( discEhEquiv ? " (EQUIVALENTE)":"" );
							aluno_saida_em_txt[alunoId].push_back( ss.str() );
						}						
						else										// Há salas com horários livres suficientes
						{
							// Verifica para cada sala com horário livre se há choque de horários com o aluno
								
							stringstream ss1SemChoque;
							ss1SemChoque << "Provavelmente não há alunos com horários livres em comum suficientes "
										<< "para abrir nova turma da disciplina " << discEquiv->getCodigo()
										<< ( discEhEquiv ? " (equivalente)":"" );
							stringstream ss2SemChoque;
							int opcaoSemChoque=0;

							stringstream ss1ComChoque;
							stringstream ss2ComChoque;
							ss1ComChoque << "Não há horários livres em comum com o aluno suficientes "
										<< "para abrir mais turmas da disciplina " << discEquiv->getCodigo()
										<< ( discEhEquiv ? " (equivalente)":"" );
							int opcaoComChoque=0;

							map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >::iterator
								itSala = mapSalaDiaHorsLivres.begin();
							for ( ; itSala != mapSalaDiaHorsLivres.end(); itSala++ )
							{
								Sala *sala = itSala->first;

								map< int, GGroup< Trio<int,int,Disciplina*> > > choques =
									procuraChoqueDeHorariosAluno( aluno, itSala->second );

								if ( choques.size() == itSala->second.size() )	// Todas as opcoes da sala têm choques
								{
									opcaoComChoque++;
									ss2ComChoque << sala->getCodigo() << "; ";
								}
								else											// Há opção sem choque
								{		
									opcaoSemChoque++;
									ss2SemChoque << sala->getCodigo() << "; ";	
								}
							}

							if ( opcaoSemChoque )
							{
								stringstream ss;
								ss << ss1SemChoque.str();

								if ( opcaoSemChoque == 1 ) ss << " na sala ";
								else ss << " nas salas ";
									
								ss << ss2SemChoque.str();

								naoAtendimento->addMotivo( ss.str() );
							}
							if ( opcaoComChoque )
							{
								stringstream ss;
								ss << ss1ComChoque.str();

								if ( opcaoComChoque == 1 ) ss << " na sala ";
								else ss << " nas salas ";
									
								ss << ss2ComChoque.str();

								naoAtendimento->addMotivo( ss.str() );
							}
						}		
											
						if (bd) std::cout<<"\n\t\t4.35"; fflush(NULL);

						#pragma endregion
					}
				}
			
				if (bd) std::cout<<"\n5"; fflush(NULL);

				problemSolution->nao_atendimentos->add( naoAtendimento );

				if (bd) std::cout<<"\n6"; fflush(NULL);
			}
		}
	}
		  
		
	#ifndef PRINT_LOGS
		return;
	#endif
	
	// ------------------------------------------------------------------------------------------
	// Print

	stringstream ssFileName;
	ssFileName << "nao-atendimentos-tatico" << P << ".txt";
	ofstream fout( ssFileName.str(), ios::app );
	if ( !fout )
	{
		std::cout<<"\nErro ao abrir arquivo nao-atendimentos-tatico.txt";
		return;
	}

	fout << "\n==========================================================="
		<< "==============================================================\n"
		<< "\nCampus " << campusIdAtual
		<< "\n\n==========================================================="
		<< "==============================================================\n";
	map<int, vector<string> >::iterator it_aluno_saida = aluno_saida_em_txt.begin();
	for (; it_aluno_saida != aluno_saida_em_txt.end(); it_aluno_saida++)
	{
		for ( int at = 0; at < (*it_aluno_saida).second.size(); at++ )
		{
			fout << (*it_aluno_saida).second[at];
		}
	}
	fout.close();
}

map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >
	SolverMIP::procuraCombinacaoLivreEmSalas( Disciplina *disciplina, TurnoIES* turno, int campusId )
{
	map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >
		mapSalaCombinacoesLivres;

	std::map< int, ConjuntoSala* >::iterator it_conjunto_sala = disciplina->cjtSalasAssociados.begin();
	for ( ; it_conjunto_sala != disciplina->cjtSalasAssociados.end(); it_conjunto_sala++ )
	{
		Sala *sala = (*it_conjunto_sala).second->getSala();

		if ( sala->getIdCampus() != campusId )
			continue;

		vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > 
			opcoesLivresNaSala = this->procuraCombinacaoLivreNaSala( disciplina, turno, sala );
		if ( opcoesLivresNaSala.size() != 0 )
		{
			mapSalaCombinacoesLivres[sala] = opcoesLivresNaSala;
		}
	}

	return mapSalaCombinacoesLivres;
}

vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > 
	SolverMIP::procuraCombinacaoLivreNaSala( Disciplina *disciplina, TurnoIES* turno, Sala *sala )
{
	bool haSalaDisponivel = false;

	bool debug=false;
	if ( disciplina->getId() == 14290 && sala->getId() == 2016 && turno->getId() == 38 )
	{
		//std::cout<<"\n\nDEBUG\n";
		//debug = true;
	}

	vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > mapCombinacoesLivres;

	std::map<int, std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > > > 
		map_dia_horarios_vagos = sala->retornaHorariosAulaVagos();

	if ( debug )
	{
		if ( map_dia_horarios_vagos.size() == 0 ) 
			std::cout<<"\nSala cheia.";
		else
			std::cout<<"\nHors livres da sala:";
			std::map<int, std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > > >::iterator 
				itMap1 = map_dia_horarios_vagos.begin();
			for ( ; itMap1 != map_dia_horarios_vagos.end(); itMap1++ )
			{
				cout<< "\nDia " << itMap1->first;
				std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > >::iterator
					itMapDti = itMap1->second.begin();
				for ( ; itMapDti != itMap1->second.end(); itMapDti++ )
				{
					DateTime dti = itMapDti->first;
					std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator
						itMapDtf = itMapDti->second.begin();
					for ( ; itMapDtf != itMapDti->second.end(); itMapDtf++ )
					{
						DateTime dtf = itMapDtf->first;
						cout<< "   " << dti.getHour() << ":" << dti.getMinute();
						cout<< " - " << dtf.getHour() << ":" << dtf.getMinute();
					}
				}
			}
	}

	std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > >::iterator
		it_combinacao_creditos = disciplina->combinacao_divisao_creditos.begin();
	for ( ; it_combinacao_creditos != disciplina->combinacao_divisao_creditos.end() ; it_combinacao_creditos++ )
	{
		bool combinacaoOK = true;
		map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > opcaoValida;
		
		if ( debug )
			std::cout<<"\n\nCombinacao: ";

		std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > >::iterator it_creditos = (*it_combinacao_creditos).begin();
		for ( ; it_creditos != (*it_combinacao_creditos).end() ; it_creditos++ )
		{
			int dia_credito = (*it_creditos).first;
			int n_credito = (*it_creditos).second;
			bool blocoCred = false;
			
			if ( debug )
				std::cout<<"\ndia " << dia_credito << "- n = " << n_credito;

			if ( n_credito > 0 )
			{
				// PERCORRE HORARIOS LIVRES DA SALA NO DIA

				std::map<int, std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > > >::iterator 
					itMap1 = map_dia_horarios_vagos.find( dia_credito );
				if ( itMap1 != map_dia_horarios_vagos.end() )
				{
					std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > >::iterator
						itMapDti = itMap1->second.begin();
					for ( ; itMapDti != itMap1->second.end(); itMapDti++ )
					{
						DateTime dti = itMapDti->first;
						std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator
							itMapDtf = itMapDti->second.begin();
						for ( ; itMapDtf != itMapDti->second.end(); itMapDtf++ )
						{
							DateTime dtf = itMapDtf->first;
							HorarioAula *horarioAula = *(itMapDtf->second.begin());
								
							GGroup<HorarioAula*, LessPtr<HorarioAula>> horsValidosAPartirDeHi;
							
							if ( debug ) std::cout<<"\n-->Hi = (" << horarioAula->getInicio() << " - " << horarioAula->getFinal() << ")";

							if ( turno->possuiHorarioDiaOuCorrespondente(horarioAula,dia_credito) )
							{
								horsValidosAPartirDeHi.add( horarioAula );
							}
							else
							{
								if ( debug ) std::cout<<"\n\tTurno NAO possui " << horarioAula->getInicio() << " no dia " << dia_credito;
								continue;
							}

							bool hInicialComNCreditosOK = true;

							#pragma region CONFERE SE OS N HORÁRIOS DO BLOCO SÃO VÁLIDOS NO DIA NOS HORÁRIOS LIVRES DA SALA E NO TURNO
							HorarioAula* h = horarioAula;
							for ( int i=2; i<=n_credito && hInicialComNCreditosOK; i++ )
							{
								h = h->getCalendario()->getProximoHorario( h );

								if ( h == NULL )
								{
									hInicialComNCreditosOK = false;
									break;
								}

								DateTime dti_next = h->getInicio();
								DateTime dtf_next = h->getFinal();

								// CONFERE SE O HORARIO-DIA ESTÁ DISPONIVEL NA SALA

								bool achouNaSala=false;
								std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > >::iterator
									itMapDtiProx = itMapDti;
								for ( ; itMapDtiProx != itMap1->second.end(); itMapDtiProx++ )
								{
									if ( itMapDtiProx->first > dti_next )
										break;
									if ( itMapDtiProx->first == dti_next )
									{
										std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator
											itMapDtfProx = itMapDtiProx->second.begin();
										for ( ; itMapDtfProx != itMapDtiProx->second.end(); itMapDtfProx++ )
										{
											if ( itMapDtfProx->first > dtf_next )
												break;
											if ( itMapDtfProx->first == dtf_next )
											{
												achouNaSala = true;
												break;
											}
										}
									}
								}

								if ( !achouNaSala ){ hInicialComNCreditosOK = false; if ( debug ) std::cout<<"\n\tNao achou na sala";}
									
								// CONFERE SE O HORARIO-DIA ESTÁ DISPONIVEL NO TURNO

								if ( hInicialComNCreditosOK )
								{
									if ( turno->possuiHorarioDiaOuCorrespondente(h,dia_credito) )
									{
										horsValidosAPartirDeHi.add( horarioAula );
									}
									else
									{
										hInicialComNCreditosOK = false;
										if ( debug ) 
											std::cout<<"\n\tTurno NAO possui " << horarioAula->getInicio() << " no dia " << dia_credito;
									}
								}
							}
							#pragma endregion

							// SE OS HORARIOS-DIAS ESTÃO DISPONIVEIS NA SALA E NO TURNO, CONFERE SE É UMA AULA VÁLIDA NA DISCIPLINA
							if ( hInicialComNCreditosOK )
							{
								if ( disciplina->inicioTerminoValidos( horarioAula, h, dia_credito ) )
								{
									if ( debug ) std::cout<<"\n\tInicio fim validos!";
									blocoCred = true;
									opcaoValida[dia_credito].add( horsValidosAPartirDeHi );
								}
								else if ( debug ) std::cout<<"\n\tInicio fim nao validos";
							}
						}
					}
				}
			}
			else
			{
				blocoCred = true;
			}

			if ( !blocoCred )
			{
				combinacaoOK = false;
				break;
			}
		}
		if(combinacaoOK)
		{
			mapCombinacoesLivres.push_back( opcaoValida );
			haSalaDisponivel = true;
		}
	}

	return mapCombinacoesLivres;
}

map< int, GGroup< Trio<int,int,Disciplina*> > > SolverMIP::procuraChoqueDeHorariosAluno( Aluno *aluno, 
	vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > &opcoes )
{
	/*
		- Recebe em 'opcoes' um vetor com combinacoes de horários-dia, e um aluno.
		- Verifica para cada combinacao se há choque de horários com as turmas já alocadas do aluno.
		- Retorna, para cada combinacao, as turmas do aluno que possuem choques.
	*/

	map< int /*opcao*/, GGroup< Trio<int,int,Disciplina*> > > turmas_choque_horarios;

	// Turmas em que o aluno foi alocado
	GGroup< Trio< int , int , Disciplina* > > *trios_aluno = &problemData->mapAluno_CampusTurmaDisc[aluno];

	// Conjunto de solucoes para cada turma alocada do aluno
	map< int /*campusId*/, map< Disciplina*, map< int /*turma*/, vars__X__i_d_u_s_hi_hf_t >, LessPtr<Disciplina> > >::iterator itMapSol;
	map< int /*campusId*/, map< Disciplina*, map< int /*turma*/, vars__X__i_d_u_s_hi_hf_t >, LessPtr<Disciplina> > > mapSolucoes;
	ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
	{
		int cpId = (*it_Vars_x)->getUnidade()->getIdCampus();
		int turma = (*it_Vars_x)->getTurma();
		Disciplina *disciplina = (*it_Vars_x)->getDisciplina();
		
		Trio<int,int,Disciplina*> trio(cpId, turma, disciplina);
		if ( trios_aluno->find( trio ) != trios_aluno->end() )
		{
			mapSolucoes[cpId][disciplina][turma].add( (*it_Vars_x) );
		}
	}

	// Verifica conflitos entre horários-dia alocados do aluno e os horários-dia em 'opcoes'
	for ( int at = 0; at != opcoes.size(); at++ )
	{
		map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > *opcao = & opcoes[at];
		map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator
			itDiaHors = (*opcao).begin();
		for ( ; itDiaHors != (*opcao).end(); itDiaHors++ )
		{
			int dia = itDiaHors->first;

			ITERA_GGROUP_LESSPTR( itHorAula, itDiaHors->second, HorarioAula )
			{
				DateTime dti = itHorAula->getInicio();
				DateTime dtf = itHorAula->getFinal();

				GGroup< Trio< int , int , Disciplina* > >::iterator it_trios = (*trios_aluno).begin();
				for( ; it_trios != (*trios_aluno).end(); it_trios++ )
				{
					Trio< int , int , Disciplina* > trio_aluno = (*it_trios);
					int campus_aluno = trio_aluno.first;
					int turma_aluno = trio_aluno.second;
					Disciplina* disc_aluno = trio_aluno.third;
					
					vars__X__i_d_u_s_hi_hf_t *aulasAluno = &mapSolucoes[campus_aluno][disc_aluno][turma_aluno];
										
					ITERA_GGROUP_LESSPTR( it_aula_aluno, (*aulasAluno), VariableTatico )
					{
						int dia_aula = (*it_aula_aluno)->getDia();

						if( dia_aula == dia)									
						{
							//encontrou uma disciplina do aluno no mesmo dia

							DateTime inicio_aluno = (*it_aula_aluno)->getHorarioAulaInicial()->getInicio();
							DateTime fim_aluno = (*it_aula_aluno)->getHorarioAulaFinal()->getFinal();

							if( (inicio_aluno >= dti && inicio_aluno < dtf) ||
								(dti >= inicio_aluno && dti < fim_aluno ) )
							{
								turmas_choque_horarios[at].add(trio_aluno);
							}
						}
					}
				}
			}
		}
	}

	return turmas_choque_horarios;
}

void SolverMIP::fixaAtendimentosVariaveisCreditosAnterior()
{
	int nBds=0;
	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	BOUNDTYPE *bdsl = new BOUNDTYPE[lp->getNumCols()*2];
	BOUNDTYPE *bdsu = new BOUNDTYPE[lp->getNumCols()*2];

	VariableTaticoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;
	   
		if ( v.getType() == VariableTatico::V_CREDITOS )
		{
			int lb = (int)(lp->getLB(vit->second) + 0.5);
			int ub = (int)(lp->getUB(vit->second) + 0.5);

			if ( lb != ub ) // se for variavel livre
			{				
				bool FOUND=false;
				bool FOUND_ANOTHER=false;
				
				// Procura a variavel x nos atendimentos anteriores.
				// Se for encontrada, fixa-a igual a 1. Se ela não for encontrada, mas
				// outra para a mesma turma/disciplina for (logo, ja existe atendimento),
				// fixa-a igual a 0.
				ITERA_GGROUP_LESSPTR ( itVar, solVarsTatico, VariableTatico )
				{
					VariableTatico vSol = **itVar;		

					if ( vSol.getType() == VariableTatico::V_CREDITOS &&
						 vSol.getTurma() == v.getTurma() &&
						 vSol.getDisciplina() == v.getDisciplina() &&
						 vSol.getUnidade()->getIdCampus() == v.getUnidade()->getIdCampus() )
					{
						if ( vSol.getUnidade() == v.getUnidade() &&
							 vSol.getSubCjtSala() == v.getSubCjtSala() &&
							 vSol.getDia() == v.getDia() &&
							 vSol.getHorarioAulaInicial() == v.getHorarioAulaInicial() &&
							 vSol.getHorarioAulaFinal() == v.getHorarioAulaFinal() )
						{
							FOUND = true;
							break;
						}
						else FOUND_ANOTHER = true;
					}
				}

				if (FOUND || FOUND_ANOTHER)
				{
					idxs[nBds] = vit->second;
					if (FOUND) vals[nBds] = 1.0;
					else if (!FOUND && FOUND_ANOTHER) vals[nBds] = 0.0;
					bdsl[nBds] = BOUNDTYPE::BOUND_LOWER;
					bdsu[nBds] = BOUNDTYPE::BOUND_UPPER;
					nBds++;
				}
			}
		}
		else if ( v.getType() == VariableTatico::V_ABERTURA )
		{
			int lb = (int)(lp->getLB(vit->second) + 0.5);
			int ub = (int)(lp->getUB(vit->second) + 0.5);

			if ( lb != ub ) // se for variavel livre
			{
				bool found=false;
				double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );				
				if (found) 
				{
					idxs[nBds] = vit->second;
					vals[nBds] = 1.0;
					bdsl[nBds] = BOUNDTYPE::BOUND_LOWER;
					bdsu[nBds] = BOUNDTYPE::BOUND_UPPER;
					nBds++;
				}
			}
		}
	}
	
	lp->chgBds(nBds,idxs,bdsl,vals);
	lp->chgBds(nBds,idxs,bdsu,vals);
}

void SolverMIP::liberaBoundsVariaveis_FFD_FD_FP()
{
	int nBds=0;
	int *idxs = new int[lp->getNumCols()*2];
	double *vall = new double[lp->getNumCols()*2];
	double *valu = new double[lp->getNumCols()*2];
	BOUNDTYPE *bdsl = new BOUNDTYPE[lp->getNumCols()*2];
	BOUNDTYPE *bdsu = new BOUNDTYPE[lp->getNumCols()*2];

	VariableTaticoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;

		if ( v.getType() == VariableTatico::V_SLACK_SLACKDEMANDA_PT || 
			 v.getType() == VariableTatico::V_SLACK_DEMANDA ) // necessario nos casos de atendimento parcial, por pratica+teorica
		{
			int lb = 0.0;
			int ub = 1.0;
			idxs[nBds] = vit->second;
			vall[nBds] = lb;
			valu[nBds] = ub;
			bdsl[nBds] = BOUNDTYPE::BOUND_LOWER;
			bdsu[nBds] = BOUNDTYPE::BOUND_UPPER;
			nBds++;			
		}
		else if ( v.getType() == VariableTatico::V_FOLGA_HOR_PROF )
		{
			int lb = 0.0;
			int ub = 50.0;
			idxs[nBds] = vit->second;
			vall[nBds] = lb;
			valu[nBds] = ub;
			bdsl[nBds] = BOUNDTYPE::BOUND_LOWER;
			bdsu[nBds] = BOUNDTYPE::BOUND_UPPER;
			nBds++;			
		}		
	}

	lp->chgBds(nBds,idxs,bdsl,vall);
	lp->chgBds(nBds,idxs,bdsu,valu);
}

void SolverMIP::voltaComAlunosNaoAlocados()
{	
	std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator
		it1 = problemData->mapSlackAluno_CampusTurmaDisc.begin();
    for ( ; it1 != problemData->mapSlackAluno_CampusTurmaDisc.end(); it1++ )
	{
		Aluno *aluno = it1->first;

		GGroup< Trio< int, int, Disciplina* > >::iterator
			itGGroup = it1->second.begin();
		for ( ; itGGroup!= it1->second.end(); itGGroup++ )
		{
			problemData->mapAluno_CampusTurmaDisc[aluno].add( *itGGroup );		
		}
	}

	std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
		it2 = problemData->mapSlackCampusTurmaDisc_AlunosDemanda.begin();
    for ( ; it2 != problemData->mapSlackCampusTurmaDisc_AlunosDemanda.end(); it2++ )
	{
		Trio< int, int, Disciplina* > trio = it2->first;

		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > >::iterator
			itGGroup = it2->second.begin();
		for ( ; itGGroup!= it2->second.end(); itGGroup++ )
		{
			problemData->mapCampusTurmaDisc_AlunosDemanda[trio].add( *itGGroup );
			problemData->listSlackDemandaAluno.remove( *itGGroup );
		}
	}
					
}

void SolverMIP::validaCorrequisitos()
{
	std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator
		itMapAlocacao = problemData->mapAluno_CampusTurmaDisc.begin();

	for ( ; itMapAlocacao != problemData->mapAluno_CampusTurmaDisc.end(); itMapAlocacao++ )
	{
		Aluno *aluno = itMapAlocacao->first;
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > aluno_trios = itMapAlocacao->second;
		
		std::map< int /*tuplaId*/, GGroup< int/*discId*/ > > mapAlunoCorrequisitos = aluno->getCorrequisitos();	
		std::map< int /*tuplaId*/, GGroup< int/*discId*/ > >::iterator itMapCorreq = mapAlunoCorrequisitos.begin();
		for( ; itMapCorreq != mapAlunoCorrequisitos.end(); itMapCorreq++ )
		{
			int nAlocacoes = 0;
			map<int, bool> discAlocado;
			ITERA_GGROUP_N_PT( itCorreq, itMapCorreq->second, int )
			{
				int discCorreqId = *itCorreq;				
				discAlocado[discCorreqId]=false;

				GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itTrioAlocado = aluno_trios.begin();
				for( ; itTrioAlocado != aluno_trios.end(); itTrioAlocado++ )
				{
					int discAlocadaId = (*itTrioAlocado).third->getId();
					if ( discCorreqId == discAlocadaId )
					{
						nAlocacoes++;
						discAlocado[discCorreqId]=true;
						break;
					}
				}
			}
			if ( nAlocacoes != itMapCorreq->second.size() && nAlocacoes != 0 )
			{
				// remover o aluno das parciais
				std::cout<<"\n\nRemover o aluno id" << aluno->getAlunoId() << " das turmas: ";
				map<int, bool>::iterator itMapDisc = discAlocado.begin();
				for ( ; itMapDisc!=discAlocado.end(); itMapDisc++ )
				{
					if ( itMapDisc->second )
						std::cout<<"d="<<itMapDisc->first<<"; ";
				}
				std::cout<<"\nDevido à nao alocacao em: ";
				itMapDisc = discAlocado.begin();
				for ( ; itMapDisc!=discAlocado.end(); itMapDisc++ )
				{
					if ( !itMapDisc->second )
						std::cout<<"d="<<itMapDisc->first<<"; ";
				}
			}
		}
	}
}


void SolverMIP::criaVariaveisSolInicial( int campusId )
{
	//ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
	//{
		//std::cout << "Aula " << itAula->toString() << endl;

	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
	   LessPtr<Disciplina> > > *ptMapCpAulas = problemData->getSolTaticoInicial()->getPtMapAulas();

	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
	   LessPtr<Disciplina> > >::iterator itMapCp = (*ptMapCpAulas).begin();
	for ( ; itMapCp != (*ptMapCpAulas).end(); itMapCp++ )
	{		
		if ( itMapCp->first != campusId )
			continue;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		   LessPtr<Disciplina> > *ptMapDiscAulas = & itMapCp->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		   LessPtr<Disciplina> >::iterator itMapDisc = (*ptMapDiscAulas).begin();
		for ( ; itMapDisc != (*ptMapDiscAulas).end(); itMapDisc++ )
		{
			Disciplina* disciplina = itMapDisc->first;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> > *ptMapTurma = & itMapDisc->second;
			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >::iterator itTurma = (*ptMapTurma).begin();
	
			for ( ; itTurma != (*ptMapTurma).end(); itTurma++ )
			{
				int turma = itTurma->first;
				
				bool fixadoCompleto = false;

				ITERA_GGROUP_LESSPTR( itAula, itTurma->second, Aula )
				{					
					if ( itAula->fixaAbertura() &&
						itAula->fixaSala() &&
						itAula->fixaDia() &&
						itAula->fixaHi() &&
						itAula->fixaHf() )
					{		
						fixadoCompleto = true;

						// ---------------------------------------------------------------
						// Cria variavel correspondente à aula

						VariableTatico *v = new VariableTatico();			
						v->reset();
						v->setType( VariableTatico::V_CREDITOS );
						v->setTurma( itAula->getTurma() );							 // i
						v->setDisciplina( itAula->getDisciplina() );				 // d
						v->setUnidade( itAula->getUnidade() );						 // u
						v->setSubCjtSala( itAula->getCjtSala() );					 // tps  
						v->setDia( itAula->getDiaSemana() );						 // t
						v->setHorarioAulaInicial( itAula->getHorarioAulaInicial() ); // hi
						v->setHorarioAulaFinal( itAula->getHorarioAulaFinal() );	 // hf
						v->setDateTimeInicial( itAula->getDateTimeInicial() );		 // dti
						v->setDateTimeFinal( itAula->getDateTimeFinal() );			 // dtf
					
						this->solVarsTatico.add(v);
						this->vars_xh.add(v);
				
						// ---------------------------------------------------------------
						// Acha horariosDias usados
						GGroup<HorarioDia*> horariosDias;
						int dia = v->getDia();
						HorarioAula *hi = v->getHorarioAulaInicial();
						HorarioAula *hf = v->getHorarioAulaFinal();
						int nCreds = hi->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
			
						HorarioAula *ha = hi;
						for ( int i = 1; i <= nCreds; i++ )
						{
							HorarioDia *hd = problemData->getHorarioDiaCorrespondente( ha, dia );
							horariosDias.add( hd );							
							ha = hi->getCalendario()->getProximoHorario( ha );				
						}

						itAula->getSala()->addHorarioDiaOcupado( horariosDias );
											
					}
				}

				// ---------------------------------------------------------------
				// Aloca alunos

				if (fixadoCompleto)
				{
					GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> *alunosDemAlocados = 
						problemData->getSolTaticoInicial()->getAlunosDemanda( campusId, disciplina, turma );
					ITERA_GGROUP_LESSPTR( itAlDem, (*alunosDemAlocados), AlunoDemanda)
					{
						Trio< int, int, Disciplina* > trio;
						trio.set( campusId, turma, disciplina );

						problemData->mapAluno_CampusTurmaDisc[ itAlDem->getAluno() ].add( trio );					 						
						problemData->mapCampusTurmaDisc_AlunosDemanda[trio].add( *itAlDem );
					}			
				}
			}
		}
	}
}


#ifndef HEURISTICA

bool SolverMIP::readSolHeurInic()
{
	return false;

	//std::string fileName = this->getSolXInitFileName( problemData->campi.begin()->getId() );

	//ifstream fin( fileName, ios_base::in );
	//if ( fin == NULL )
	//{
	//	std::cout << "\n==> Arquivo " << fileName << " nao encontrado. Fim do carregamento de solucao heuristica.\n\n"; fflush(NULL);
	//    return false;
	//}
	//else
	//{      
	//	double *xSol;
	//	
 //     std::map<std::string,double> varVals;

	//  while ( ! fin.eof() )
 //     {
 //        char buf[2048];
 //        std::string varName = "";
 //        fin >> buf;
 //        varName = buf;
	//	 
 //        varVals[varName] = 1;
 //     }

 //     std::map<std::string,double>::iterator itStrX = varVals.begin();
 //     while ( itStrX != varVals.end() )
 //     {
	//	  // x_{}
	//	  std::string strVarX = itStrX->first;
	//	  		  
	//	  std::string::iterator itStr = strVarX.begin();
	//	  while ( itStr != strVarX.end() )
	//	  {
	//		  std::string strParc;

	//		  for ( std::string::iterator itStrParc = itStr; itStrParc != strVarX.end() ; itStrParc++ )
	//		  {
	//			strParc

	//		  }

	//		  itStr = itStrParc;
	//		  if ( itStr != strVarX.end() )
	//			  itStr++;
	//	  }

 //        VariableTatico v = vit->first;
 //        int col = vit->second;
 //        double value = (int)( xSol[ col ] + 0.5 );
 //        std::string varName = v.toString();

 //        if ( varVals.find(varName) != varVals.end() )
 //        {
 //           xSol[col] = varVals[varName];
 //        }

 //        itStrX++;
 //     }

 //     fin.close();
 //  }

	//return true;
	//return false;
}

void SolverMIP::carregaSolucaoHeuristicaInicial()
{
	bool leuArquivoSol = false;

	leuArquivoSol = readSolHeurInic();

	if ( !leuArquivoSol )
	{
	   reenumeraTurmasSolInicProblemSolution();
	   loadProblemSolution();
	}
}

void SolverMIP::reenumeraTurmasSolInicProblemSolution()
{
	std::cout<<"\nReenumerando turmas da Sol Inic do ProblemSolution..."; fflush(NULL);
	
	bool debugging=false;

	std::map< Campus*, std::map< Disciplina*, std::map< int, int >, LessPtr<Disciplina> >, LessPtr<Campus> > mapCpDiscTurma;
	
    ITERA_GGROUP( it_At_Campus,
      ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   {
       // Campus do atendimento	   
	   Campus *campus = it_At_Campus->campus;
	   
       ITERA_GGROUP_LESSPTR( it_At_Unidade,
         ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
       {
		   ITERA_GGROUP_LESSPTR( it_At_Sala,
            ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
		   {
			   ITERA_GGROUP_LESSPTR( it_At_DiaSemana, 
				( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
			   {
				   ITERA_GGROUP_LESSPTR( it_At_Turno,
					( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
				   {
					   ITERA_GGROUP_LESSPTR( it_At_Hor_Aula,
						( *it_At_Turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
					   {
						   bool ehTeorica = it_At_Hor_Aula->getCreditoTeorico();
						   
						   ITERA_GGROUP_LESSPTR( it_At_Oft,
							( *it_At_Hor_Aula->atendimentos_ofertas ), AtendimentoOferta )
						   {
							    int discOrigId = it_At_Oft->getDisciplinaId();
								int discRealId = it_At_Oft->getDisciplinaSubstitutaId();

								// Se não houve substituição por equivalência, a original é igual à real
								if ( discRealId == NULL )
									discRealId = discOrigId;

								Disciplina* discOrig = problemData->refDisciplinas[discOrigId];
								Disciplina* discReal = problemData->refDisciplinas[discRealId];
								
								if (!ehTeorica)
								{
									Disciplina* discTemp;
									int discTempId = -discRealId;
									discTemp = problemData->getDisciplinaTeorPrat(discReal);
									if ( discTemp != NULL )
									{
										discReal = discTemp;
										discRealId = discTempId;
									}
									else if ( discReal->getCredPraticos() <= 0 )
									{
										std::cout<<"\nDisciplina pratica nao encontrada: id "
												<< discTempId << ". Saindo...";
										exit(1);
									}
								}
																
								int turma = it_At_Oft->getTurma();
								
								// Cadastra a turma
								mapCpDiscTurma[campus][discReal][turma] = 0;
							}
						}
					}
				}
            }
        }
    }
	
	std::cout<<"\nRemapeando as turmas..."; fflush(NULL);

	std::string fileName("reenumeracaoTurmasPosHeur.txt");
	ofstream reenumeraFile;
	reenumeraFile.open( fileName, ofstream::out );
	if ( !reenumeraFile )
	{
		std::cout << "\nErro em void SolverMIP::reenumeraTurmasSolInicProblemSolution():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}
	std::map< Campus*, std::map< Disciplina*, std::map< int, int >, LessPtr<Disciplina> >, LessPtr<Campus> >::iterator
		itCp = mapCpDiscTurma.begin();
	for ( ; itCp != mapCpDiscTurma.end(); itCp++ )
	{			
		reenumeraFile << "\nCp " << itCp->first->getId();
		std::map< Disciplina*, std::map< int, int >, LessPtr<Disciplina> >::iterator
			itDisc = itCp->second.begin();
		for ( ; itDisc != itCp->second.end(); itDisc++ )
		{
			reenumeraFile << "\n\tDisc " << itDisc->first->getId();
			int i = 1;
			std::map< int, int >::iterator itTurma = itDisc->second.begin();
			for ( ; itTurma != itDisc->second.end(); itTurma++ )
			{
				itTurma->second = i;
				i++;

				reenumeraFile << "\n\t\tTurma " << itTurma->first << " -> " << itTurma->second;
			}
		}
	}
	reenumeraFile.close();
	
	std::cout<<"\nReenumerando as turmas no Problem Solution..."; fflush(NULL);

    ITERA_GGROUP( it_At_Campus,
      ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   { 
	   Campus *campus = it_At_Campus->campus;
	   
       ITERA_GGROUP_LESSPTR( it_At_Unidade,
         ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
       {
		   ITERA_GGROUP_LESSPTR( it_At_Sala,
            ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
		   {
			   ITERA_GGROUP_LESSPTR( it_At_DiaSemana, 
				( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
			   {
				   ITERA_GGROUP_LESSPTR( it_At_Turno,
					( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
				   {
					   ITERA_GGROUP_LESSPTR( it_At_Hor_Aula,
						( *it_At_Turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
					   {
						   bool ehTeorica = it_At_Hor_Aula->getCreditoTeorico();
						   
						   ITERA_GGROUP_LESSPTR( it_At_Oft,
							( *it_At_Hor_Aula->atendimentos_ofertas ), AtendimentoOferta )
						   {
							    int discOrigId = it_At_Oft->getDisciplinaId();
								int discRealId = it_At_Oft->getDisciplinaSubstitutaId();

								// Se não houve substituição por equivalência, a original é igual à real
								if ( discRealId == NULL )
									discRealId = discOrigId;

								Disciplina* discOrig = problemData->refDisciplinas[discOrigId];
								Disciplina* discReal = problemData->refDisciplinas[discRealId];
								
								if (!ehTeorica)
								{
									Disciplina* discTemp;
									int discTempId = -discRealId;
									discTemp = problemData->getDisciplinaTeorPrat(discReal);
									if ( discTemp != NULL )
									{
										discReal = discTemp;
										discRealId = discTempId;
									}
									else if ( discReal->getCredPraticos() <= 0 )
									{
										std::cout<<"\nDisciplina pratica nao encontrada: id "
												<< discTempId << ". Saindo...";
										exit(1);
									}
								}

								int turma = it_At_Oft->getTurma();
								
								it_At_Oft->setTurma( mapCpDiscTurma[campus][discReal][turma] );								
							}
						}
					}
				}
            }
        }
    }
	
	std::cout<<" Fim!\n"; fflush(NULL);
}

void SolverMIP::loadProblemSolution()
{
	std::cout<<"\nLoading problem solution..."; fflush(NULL);
	
	bool debugging=false;

	std::map< Aluno*, std::map< Disciplina*, int, LessPtr<Disciplina> >, LessPtr<Aluno> > mapAlunoDiscTurma;
	
    ITERA_GGROUP( it_At_Campus,
      ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   {
       // Campus do atendimento	   
	   Campus *campus = it_At_Campus->campus;
	   int campusId = campus->getId();

	   if (debugging)
	   std::cout<<"\nCampus " << campusId; fflush(NULL);

       ITERA_GGROUP_LESSPTR( it_At_Unidade,
         ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
       {
		   // Unidade do atendimento
		   Unidade *unidade = problemData->refUnidade[ it_At_Unidade->getId() ];
	   
		   if (debugging)		   
		   std::cout<<"\n\tUnid " << unidade->getId(); fflush(NULL);

		   ITERA_GGROUP_LESSPTR( it_At_Sala,
            ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
		   {
			   // Sala do atendimento
			   Sala *sala = problemData->refSala[ it_At_Sala->getId() ];
				   
			   if (debugging)   
			   std::cout<<"\n\t\tsala " << sala->getId(); fflush(NULL);

			   ITERA_GGROUP_LESSPTR( it_At_DiaSemana, 
				( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
			   {
				   // Dia da semana do atendimento
				   int dia = it_At_DiaSemana->getDiaSemana();
			    
				   if (debugging)
				   std::cout<<"\n\t\t\tDia " << dia; fflush(NULL);

				   ITERA_GGROUP_LESSPTR( it_At_Turno,
					( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
				   {	
					   // Turno do atendimento
					   TurnoIES* turno = it_At_Turno->turno;
					   	   
					   if (debugging)
					    std::cout<<"\n\t\t\t\tturno " << turno->getId(); fflush(NULL);

					   ITERA_GGROUP_LESSPTR( it_At_Hor_Aula,
						( *it_At_Turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
					   {
						   // Horario do atendimento
						   int horAulaId = it_At_Hor_Aula->getHorarioAulaId();
						   HorarioAula* horAula = it_At_Hor_Aula->horario_aula;
						   bool ehTeorica = it_At_Hor_Aula->getCreditoTeorico();
						   
						   if (debugging)
						   std::cout<<"\n\t\t\t\t\thorAula " << horAulaId; fflush(NULL);
						   if (debugging)
						   std::cout<<"\n\t\t\t\t\tteorica " << ehTeorica; fflush(NULL);

						   ITERA_GGROUP_LESSPTR( it_At_Oft,
							( *it_At_Hor_Aula->atendimentos_ofertas ), AtendimentoOferta )
						   {
							    int discOrigId = it_At_Oft->getDisciplinaId();
								int discRealId = it_At_Oft->getDisciplinaSubstitutaId();

								// Se não houve substituição por equivalência, a original é igual à real
								if ( discRealId == NULL )
									discRealId = discOrigId;

								Disciplina* discOrig = problemData->refDisciplinas[discOrigId];
								Disciplina* discReal = problemData->refDisciplinas[discRealId];
								
								if (!ehTeorica)
								{
									Disciplina* discTemp;
									int discTempId = -discRealId;
									discTemp = problemData->getDisciplinaTeorPrat(discReal);
									if ( discTemp != NULL )
									{
										discReal = discTemp;
										discRealId = discTempId;
									}
									else if ( discReal->getCredPraticos() <= 0 )
									{
										std::cout<<"\nDisciplina pratica nao encontrada: id "
												<< discTempId << ". Saindo...";
										exit(1);
									}
								}

								if (debugging)
								std::cout<<"\n\t\t\t\t\t\tdisc orig " << discOrig->getId(); fflush(NULL);
								if (debugging)
								std::cout<<"\n\t\t\t\t\t\tdisc subst " << discReal->getId(); fflush(NULL);
								
								int turma = it_At_Oft->getTurma();
								int qtd = it_At_Oft->getQuantidade();
								GGroup<int> alDemAtend = it_At_Oft->alunosDemandasAtendidas;

								if (debugging)
								std::cout<<"\n\t\t\t\t\t\tturma " << turma; fflush(NULL);
								if (debugging)
								std::cout<<"\n\t\t\t\t\t\tqtd " << qtd; fflush(NULL);

								string strOft = it_At_Oft->getOfertaCursoCampiId();
								int oftId = atoi( strOft.c_str() );
								Oferta *oferta = problemData->refOfertas[ oftId ];

								if (debugging)
								std::cout<<"\n\t\t\t\t\t\toft " << oftId; fflush(NULL);
								if (debugging)
								std::cout<<"\n\t\t\t\t\t\talunos (" << alDemAtend.size() << "): "; fflush(NULL);

								ITERA_GGROUP_N_PT( itAlDemId, alDemAtend, int )
								{
									int alDemId = *itAlDemId;

									if (debugging)
									std::cout << alDemId <<"; "; fflush(NULL);

									bool fixar = false;
									#ifdef TATICO_SO_HEURN
										fixar = true;
									#endif

									AlunoDemanda *alunoDemanda = problemData->retornaAlunoDemanda( alDemId );
									Aluno *aluno = alunoDemanda->getAluno();									
									problemData->getSolTaticoInicial()->addAlunoDem( campus, discReal, turma, alunoDemanda, fixar );									
									problemData->getSolTaticoInicial()->addAlunoTurma( campus->getId(), discReal, turma, aluno, fixar );

									mapAlunoDiscTurma[aluno][discReal] = turma;
								}

								 bool novaAula = true;

								 if (debugging)
								 std::cout<<"\n\t\t\t\t\t\tProcurando aula.."; fflush(NULL);

								 Aula *aulaAntiga = problemData->getSolTaticoInicial()->getAula( campusId, discReal, turma, dia, sala );
								 if ( aulaAntiga!=NULL )
								 {
									novaAula = false;
									problemData->getSolTaticoInicial()->removeAula( campusId, discReal, turma, aulaAntiga );							 
								 }

								 if (debugging)
								 std::cout<<"\n\t\t\t\t\t\tnovaAula " << novaAula; fflush(NULL);

								 if( novaAula )
								 {
									// Monta o objeto 'aula'
									Aula * aula = new Aula();
									aula->ofertas.add( oferta );

									int nCredTeor = ( ehTeorica? 1 : 0 );
									int nCredPrat = ( ehTeorica? 0 : 1 );

									aula->setTurma( turma );
									aula->setDisciplina( discReal );
									aula->setCampus( campus );
									aula->setSala( sala );
									aula->setUnidade( unidade );
									aula->setDiaSemana( dia );
									aula->setCreditosTeoricos( nCredTeor );
									aula->setCreditosPraticos( nCredPrat );
									aula->setQuantPorOferta( qtd, oferta );
									aula->addCursoAtendido( oferta->curso, qtd );
									aula->setDisciplinaSubstituida( discOrig, oferta );
									aula->addAlunoDemanda( oferta, discOrig, alDemAtend );

									HorarioAula *hi = horAula;
									HorarioAula *hf = horAula;
									aula->setHorarioAulaInicial( hi );	
									aula->setHorarioAulaFinal( hf );	

									DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
									DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;
									aula->setDateTimeInicial( dti );
									aula->setDateTimeFinal( dtf );	

									// Fixações para solução inicial						
									aula->fixaSala( true );
									aula->fixaDia( true );
									aula->fixaCredsTeor( true );
									aula->fixaCredsPrat( true );
									aula->fixaHi( true );
									aula->fixaHf( true );
									aula->fixaAbertura( true );									
									
									if ( turma > discReal->getNumTurmas() )
										discReal->setNumTurmas( turma );
									
									problemData->getSolTaticoInicial()->addAula(campusId, discReal, turma, aula );
								 }
								 else
								 {
									aulaAntiga->ofertas.add( oferta );
									aulaAntiga->setQuantPorOferta( qtd, oferta );
									aulaAntiga->addCursoAtendido( oferta->curso, qtd );
									aulaAntiga->setDisciplinaSubstituida( discOrig, oferta );
									aulaAntiga->addAlunoDemanda( oferta, discOrig, alDemAtend );

									HorarioAula *hi = aulaAntiga->getHorarioAulaInicial();
									HorarioAula *hf = aulaAntiga->getHorarioAulaFinal();

									bool temHorario = true;									
									bool horAulaMenorQueHi = horAula->comparaMenor( *hi ); 
									bool horAulaMaiorQueHf = hf->comparaMenor( *horAula ); 
									
									// Se horAula não está entre hi e hf, então horAula ainda não está sendo englobado na aula
									if ( horAulaMenorQueHi || horAulaMaiorQueHf )
										temHorario = false;
									
									if ( !temHorario )
									{
										if ( horAulaMenorQueHi )
										{
											hi = horAula;
											aulaAntiga->setHorarioAulaInicial( hi );
											DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
											aulaAntiga->setDateTimeInicial( dti );
										}
										if ( horAulaMaiorQueHf )
										{
											hf = horAula;
											aulaAntiga->setHorarioAulaFinal( hf );
											DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;
											aulaAntiga->setDateTimeFinal( dtf );
										}

										Calendario *calendario = hi->getCalendario();
										int nCreds = calendario->retornaNroCreditosEntreHorarios(hi,hf);

										if ( ehTeorica )
											aulaAntiga->setCreditosTeoricos( nCreds );
										else
											aulaAntiga->setCreditosPraticos( nCreds );
									}

									problemData->getSolTaticoInicial()->addAula(campusId, discReal, turma, aulaAntiga );
								 }
							}
						}
					}
				}
            }
        }
	
		string aulasFileName = this->getAulasSolInitFileName(campusId);
		problemData->getSolTaticoInicial()->imprimeAulas( aulasFileName );
	
		string xFileName = this->getSolXInitFileName(campusId);
		problemData->getSolTaticoInicial()->imprimeSolucaoX( xFileName );
	
		string sFileName = this->getSolSInitFileName(campusId);
		problemData->getSolTaticoInicial()->imprimeSolucaoS( sFileName );

		string naFileName = this->getSolNaoAtendInitFileName(campusId);
		problemData->getSolTaticoInicial()->imprimeSolucaoNaoAtendimentos( naFileName );
    }
	

//#ifndef TATICO_SO_HEURN // SÓ HEURISTICA
	this->problemSolution->resetProblemSolution();
//#endif

	this->setNumTurmasPratPorTeor( mapAlunoDiscTurma );
	problemData->getSolTaticoInicial()->confereSolucao();

}

void SolverMIP::setNumTurmasPratPorTeor( std::map< Aluno*, std::map< Disciplina*, int, LessPtr<Disciplina> >, LessPtr<Aluno> > &mapAlunoDiscTurma )
{	
	std::map< Disciplina*, std::map< int /*turma prat*/, GGroup<int /*turmas teor*/ > >, LessPtr<Disciplina> > mapDiscPratTurmaPratTurmasTeorAssoc;


	std::map< Aluno*, std::map< Disciplina*, int, LessPtr<Disciplina> >, LessPtr<Aluno> >::iterator
		itAluno = mapAlunoDiscTurma.begin();
	for ( ; itAluno != mapAlunoDiscTurma.end(); itAluno++ )
	{
		Aluno* aluno = itAluno->first;

		std::map< Disciplina*, int /*turma alocada*/, LessPtr<Disciplina> > *mapDiscTurma = & itAluno->second;
		std::map< Disciplina*, int /*turma alocada*/, LessPtr<Disciplina> >::iterator
			itDisc = mapDiscTurma->begin();
		for ( ; itDisc != mapDiscTurma->end(); itDisc++ )
		{
			if ( itDisc->first->getId() > 0 ) continue;

			Disciplina *discP = itDisc->first;
			int turmaP = itDisc->second;
			Disciplina *discT = problemData->retornaDisciplina( - discP->getId() );
			int turmaT = mapAlunoDiscTurma[aluno][discT];

			mapDiscPratTurmaPratTurmasTeorAssoc[discP][turmaP].add( turmaT );
		}
	}

	std::map< Disciplina*, std::map< int /*turma prat*/, GGroup<int /*turmas teor*/> >, LessPtr<Disciplina> >::iterator
		itDiscPrat = mapDiscPratTurmaPratTurmasTeorAssoc.begin();
	for ( ; itDiscPrat != mapDiscPratTurmaPratTurmasTeorAssoc.end(); itDiscPrat++ )
	{ 
		Disciplina *discP = itDiscPrat->first;

		std::map< int /*turma prat*/, GGroup<int /*turmas teor*/> > *mapTurmaP = & itDiscPrat->second;
		std::map< int /*turma prat*/, GGroup<int /*turmas teor*/> >::iterator itTurmaP = mapTurmaP->begin();
		for ( ; itTurmaP != mapTurmaP->end(); itTurmaP++ )
		{
			int turmaP = itTurmaP->first;
			GGroup<int> turmasT = itTurmaP->second;

			if ( problemData->parametros->discPratTeor1xN && turmasT.size() != 1 )
				std::cout<<"\nErro: relacao 1xN, porem ha turma pratica com mais de uma teorica associada.";
			
			if ( problemData->parametros->discPratTeor1x1 && turmasT.size() != 1 )
				std::cout<<"\nErro: relacao 1x1, porem ha turma pratica com mais de uma teorica associada.";
			
			Disciplina *discT = problemData->getDisciplinaTeorPrat( discP );

			ITERA_GGROUP_N_PT( itTurmaT, turmasT, int )
			{
				discP->setTurmaTeorAssociada( turmaP, *itTurmaT );
				discT->setTurmaPratAssociada( *itTurmaT, turmaP );
			}
		}

		// Pode ter buraco => não posso setar o total n assim, se eu tiver as turmas em um for iterativo de 1 a n
		//int totalPrat = mapTurmaP->size();
		//discP->setNumTurmas( totalPrat );
	}

}
#endif

void SolverMIP::mudaCjtSalaParaSala()
{
	std::cout<<"\nMudando de CjtSala para Sala...\n";
	
	if ( problemData->parametros->otimizarPor == "ALUNO" )
	{
		#ifndef TATICO_COM_HORARIOS

		   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
		   {
			  if ( ( *it_Vars_x )->getSubCjtSala()->salas.size() > 0 )
			  {
				 Sala *auxSala = (( *it_Vars_x )->getSubCjtSala()->salas.begin())->second;
				 ( *it_Vars_x )->setSala(auxSala);
			  }
		   }

		   // Imprimindo as variáveis x_{i,d,u,s,t} convertidas.

		   std::cout << "\n\n\n";
		   std::cout << "x\t\ti\td\t\t\tu\ts\t\tt\n";

		   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
		   {
			  if ( ( *it_Vars_x )->getSala() == NULL )
			  {
				 printf( "\nOPA. Variavel x (x_i(%d)_d(%d)_u(%d)_tps(%d)_t(%d))nao convertida.\n\n",
						 ( *it_Vars_x )->getTurma(),
						 ( *it_Vars_x )->getDisciplina()->getId(),
						 ( *it_Vars_x )->getUnidade()->getId(),
						 ( *it_Vars_x )->getSubCjtSala()->getId(),
						 ( *it_Vars_x )->getDia() );

				 exit( 1 );
			  }

			  std::cout << (*it_Vars_x)->getValue() << "\n\t\t"
						<< ( *it_Vars_x )->getTurma() << "\t"
						<< ( *it_Vars_x )->getDisciplina()->getCodigo() << " - id"
						<< ( *it_Vars_x )->getDisciplina()->getId() << "\t"
						<< ( *it_Vars_x )->getUnidade()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getSala()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getDia() << "\n\n";
		   }
		#endif

		#ifdef TATICO_COM_HORARIOS
		   		   
		   ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
		   {
			  if ( ( *it_Vars_x )->getSubCjtSala()->salas.size() > 0 )
			  {
				 Sala *auxSala = (( *it_Vars_x )->getSubCjtSala()->salas.begin())->second;
				 ( *it_Vars_x )->setSala(auxSala);
			  }
		   }

		   std::cout<<"\nSolucao:";

		   // Imprimindo as variáveis x_{i,d,u,s,hi,hf,t} convertidas.
		   std::cout << "\n\n\n";
		   std::cout << "x\t\ti\td\tu\ts\t\thi\thf\tt\n\n";

		   ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
		   {
			  if ( ( *it_Vars_x )->getSala() == NULL )
			  {
				 printf( "\nOPA. Variavel x (x_i(%d)_d(%d)_u(%d)_tps(%d)_t(%d))nao convertida.\n\n",
						 ( *it_Vars_x )->getTurma(),
						 ( *it_Vars_x )->getDisciplina()->getId(),
						 ( *it_Vars_x )->getUnidade()->getId(),
						 ( *it_Vars_x )->getSubCjtSala()->getId(),
						 ( *it_Vars_x )->getHorarioAulaInicial()->getId(),
						 ( *it_Vars_x )->getHorarioAulaFinal()->getId(),
						 ( *it_Vars_x )->getDia() );

				 exit( 1 );
			  }

			  std::cout << (*it_Vars_x)->getValue() << "\t\t"
						<< ( *it_Vars_x )->getTurma() << "\t"
						<< ( *it_Vars_x )->getDisciplina()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getUnidade()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getSala()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getHorarioAulaInicial()->getId() << "\t"
						<< ( *it_Vars_x )->getHorarioAulaFinal()->getId() << "\t"
						<< ( *it_Vars_x )->getDia() << "\n\n";
			  fflush(NULL);
		   }
		#endif
	}
}

// Função usada para Tatico - Aluno - Com Horario
void SolverMIP::getSolutionTaticoPorAlunoComHorario()
{
	std::cout<<"\nPreenchendo a estrutura atendimento_campus com a saida.\n"; fflush(NULL);		

   // POVOANDO AS CLASSES DE SAIDA

   int at_Tatico_Counter = 0;

   // Iterando sobre as variáveis do tipo x.
   ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
   {
	  int dia = ( *it_Vars_x )->getDia();
	  Disciplina *d = ( *it_Vars_x )->getDisciplina();
	  int turma = ( *it_Vars_x )->getTurma();
	  HorarioAula *hi = ( *it_Vars_x )->getHorarioAulaInicial();
	  HorarioAula *hf = ( *it_Vars_x )->getHorarioAulaFinal();
  	  int nCreds = hi->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
	  Sala *sala = ( *it_Vars_x )->getSala();
      Unidade * unidade = ( *it_Vars_x )->getUnidade();
	  
      // Descobrindo qual Campus a variável x em questão pertence.
      Campus * campus = problemData->refCampus[ ( *it_Vars_x )->getUnidade()->getIdCampus() ];
	
      bool novo_Campus = true;
      ITERA_GGROUP( it_At_Campus, ( *problemSolution->atendimento_campus ), AtendimentoCampus )
      {
         if ( it_At_Campus->getId() == campus->getId() )
         {
            if ( it_At_Campus->atendimentos_unidades->size() == 0 )
            {
               std::cout << "Achei que nao era pra cair aqui <dbg1>" << std::endl;

               // NOVA UNIDADE
               // exit( 1 );
            }
            else
            {
               bool nova_Unidade = true;
               ITERA_GGROUP_LESSPTR( it_At_Unidade, ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
               {
                  if ( it_At_Unidade->getId() == unidade->getId() )
                  {
                     if ( it_At_Unidade->atendimentos_salas->size() == 0 )
                     {
                        std::cout << "Achei que nao era pra cair aqui <dbg2>" << std::endl;

                        // NOVA SALA
                        // exit( 1 );
                     }
                     else
                     {
                        bool nova_Sala = true;
                        ITERA_GGROUP_LESSPTR( it_At_Sala, ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
                        {
                           if ( it_At_Sala->getId() == sala->getId() )
                           {
							    AtendimentoDiaSemana *at_Dia_Semana = NULL;
								bool novo_Dia = false;

                                if ( it_At_Sala->atendimentos_dias_semana->size() == 0 )
                                {
                                   std::cout << "Achei que nao era pra cair aqui <dbg3>" << std::endl;

                                   // NOVO DIA SEMANA
                                   // exit( 1 );
                                }
                                else
                                {
									// Verifica se o AtendimentoDiaSemana já existe
									ITERA_GGROUP_LESSPTR( it_At_Dia, ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
									{
										if ( it_At_Dia->getDiaSemana() == dia )
										{
											if ( it_At_Dia->atendimentos_tatico->size() == 0 )
											{
												std::cout << "Achei que nao era pra cair aqui <dbg4>" << std::endl;
												// NOVO ATENDIMENTO
												// exit( 1 );
											}

											at_Dia_Semana = *it_At_Dia;
											break;
										}
									}

									if ( at_Dia_Semana == NULL )
									{
										novo_Dia = true;

										// Cadastrando o dia da semana
										at_Dia_Semana = new AtendimentoDiaSemana(
											this->problemSolution->getIdAtendimentos() );

										at_Dia_Semana->setDiaSemana( dia );
									}

									#pragma region CADASTRO DE ATENDIMENTO TATICO

									Trio<int,int,Disciplina*> trio;
									trio.set(campus->getId(), turma, d );

									std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
										itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
							
									if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
									{
										std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
										std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
										std::cout << "\nNao encontrado AlunosDemanda em: Disciplina: " << d->getId();
										std::cout << "\tTurma: " << turma;
										continue;
									}

									// Todas as ofertas atendidas
									map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> > mapOftDiscOriginais;
									ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMap->second, AlunoDemanda )
									{									
										Demanda *demOrig = itAlunoDemanda->demandaOriginal;
										if ( demOrig != NULL )
											mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ demOrig->disciplina ].add( *itAlunoDemanda );
										else
											mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ itAlunoDemanda->demanda->disciplina ].add( *itAlunoDemanda );
									}
																							
									map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> >::iterator
											itMapOft_DiscOrig = mapOftDiscOriginais.begin();
									for ( ; itMapOft_DiscOrig != mapOftDiscOriginais.end() ; itMapOft_DiscOrig++ )
									{
										Oferta *oferta = itMapOft_DiscOrig->first;

										map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> > 
											mapDisc_Alunos = itMapOft_DiscOrig->second;

										map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >::iterator
											itMap_DiscOrig = mapDisc_Alunos.begin();
										for ( ; itMap_DiscOrig != mapDisc_Alunos.end(); itMap_DiscOrig++ )
										{	
											Disciplina* discOrig = itMap_DiscOrig->first;																																		
										
											GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > alunosDemanda = itMap_DiscOrig->second;

											AtendimentoTatico * at_Tatico = new AtendimentoTatico(
											this->problemSolution->getIdAtendimentos(),
											this->problemSolution->getIdAtendimentos() );

											// Verificando se a disciplina é de carater prático ou teórico.
											if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
											{
												at_Tatico->setQtdCreditosTeoricos( nCreds );
											}
											else
											{
												at_Tatico->setQtdCreditosPraticos( nCreds );
											}

											AtendimentoOferta * at_Oferta = at_Tatico->atendimento_oferta;
										
											ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemanda, AlunoDemanda )
											{
												if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
												{
													int alunoId = itAlunoDemanda->getAlunoId();
													int discId = - itAlunoDemanda->demanda->getDisciplinaId();
													Aluno* aluno = problemData->retornaAluno( alunoId );

													// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
													// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
													AlunoDemanda* alunoDemanda = aluno->getAlunoDemanda( discId );
													if ( alunoDemanda != NULL )
													{
														at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
													}
													else std::cout<<"\nERROR: alunodemanda teorico nao encontrado\n";
												}
												else
												{
													at_Oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
												}
											}
																																	 
											stringstream str;
											str << oferta->getId();
											at_Oferta->setOfertaCursoCampiId( str.str() );

											int id_disc = d->getId();
											
											if ( problemData->parametros->considerar_equivalencia &&
												! problemData->parametros->considerar_equivalencia_por_aluno )
											{
												std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
												Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
												if ( discOriginal != NULL )
												{
													at_Oferta->setDisciplinaSubstitutaId( id_disc );
													at_Oferta->setDisciplinaId( discOriginal->getId() );
													at_Oferta->disciplina = d;
												}
												else
												{
													at_Oferta->setDisciplinaId( id_disc );
													at_Oferta->disciplina = d;
												}
											}
											else
											{
												at_Oferta->setDisciplinaId( discOrig->getId() );
												at_Oferta->disciplina = d;

												if ( id_disc != discOrig->getId() )
												{
													at_Oferta->setDisciplinaSubstitutaId( id_disc );
												}
											}

											at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
											at_Oferta->setTurma( turma );
											at_Oferta->oferta = oferta;

											HorarioAula *h = hi;
											while (1)
											{
												at_Tatico->addHorarioAula( h->getId() );
												if ( h->getInicio() == hf->getInicio() ) break;
												h = h->getCalendario()->getProximoHorario( h );
												if ( h == NULL )
												{
													std::cout<<"\nErro 1!!! horario NULL antes de encontrar hf\n";
													break;
												}
											}

											at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

											++at_Tatico_Counter;
										}

									}

									#pragma endregion
                                }

                                if ( novo_Dia )
                                {
                                    it_At_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                                }
                               
								nova_Sala = false;
                                break;
                           }
                        }

                        if ( nova_Sala )
                        {
                            // Cadastrando a Sala
                            AtendimentoSala * at_Sala = new AtendimentoSala(
                              this->problemSolution->getIdAtendimentos() );

                            at_Sala->setId( sala->getId() );
                            at_Sala->setSalaId( sala->getCodigo() );
                            at_Sala->sala = sala;

                            // Cadastrando o dia da semana
                            AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
                              this->problemSolution->getIdAtendimentos() );

                            at_Dia_Semana->setDiaSemana( dia );
							
							#pragma region CADASTRO DE ATENDIMENTO TATICO PARA NOVA SALA

							Trio<int,int,Disciplina*> trio;
							trio.set(campus->getId(), turma, d );

							std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
								itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
							
							if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
							{
								std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
								std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
								std::cout << "\nNao encontrado AlunosDemanda em: Disciplina: " << d->getId();
								std::cout << "\tTurma: " << turma;
								continue;
							}

							// Todas as ofertas atendidas
							map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> > mapOftDiscOriginais;
							ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMap->second, AlunoDemanda )
							{									
								Demanda *demOrig = itAlunoDemanda->demandaOriginal;
								if ( demOrig != NULL )
									mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ demOrig->disciplina ].add( *itAlunoDemanda );
								else
									mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ itAlunoDemanda->demanda->disciplina ].add( *itAlunoDemanda );
							}
																							
							map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> >::iterator
									itMapOft_DiscOrig = mapOftDiscOriginais.begin();
							for ( ; itMapOft_DiscOrig != mapOftDiscOriginais.end() ; itMapOft_DiscOrig++ )
							{
								Oferta *oferta = itMapOft_DiscOrig->first;

								map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> > 
									mapDisc_Alunos = itMapOft_DiscOrig->second;

								map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >::iterator
									itMap_DiscOrig = mapDisc_Alunos.begin();
								for ( ; itMap_DiscOrig != mapDisc_Alunos.end(); itMap_DiscOrig++ )
								{	
									Disciplina* discOrig = itMap_DiscOrig->first;																																		
										
									GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > alunosDemanda = itMap_DiscOrig->second;

									AtendimentoTatico * at_Tatico = new AtendimentoTatico(
									this->problemSolution->getIdAtendimentos(),
									this->problemSolution->getIdAtendimentos() );

									// Verificando se a disciplina é de carater prático ou teórico.
									if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
									{
										at_Tatico->setQtdCreditosTeoricos( nCreds );
									}
									else
									{
										at_Tatico->setQtdCreditosPraticos( nCreds );
									}

									AtendimentoOferta * at_Oferta = at_Tatico->atendimento_oferta;
										
									ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemanda, AlunoDemanda )
									{
										if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
										{
											int alunoId = itAlunoDemanda->getAlunoId();
											int discId = - itAlunoDemanda->demanda->getDisciplinaId();
											Aluno* aluno = problemData->retornaAluno( alunoId );

											// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
											// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
											AlunoDemanda* alunoDemanda = aluno->getAlunoDemanda( discId );
											if ( alunoDemanda != NULL )
											{
												at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
											}
											else std::cout<<"\nERROR: alunodemanda teorico nao encontrado\n";
										}
										else
										{
											at_Oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
										}
									}
																																	 
									stringstream str;
									str << oferta->getId();
									at_Oferta->setOfertaCursoCampiId( str.str() );

									int id_disc = d->getId();
											
									if ( problemData->parametros->considerar_equivalencia &&
										! problemData->parametros->considerar_equivalencia_por_aluno )
									{
										std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
										Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
										if ( discOriginal != NULL )
										{
											at_Oferta->setDisciplinaSubstitutaId( id_disc );
											at_Oferta->setDisciplinaId( discOriginal->getId() );
											at_Oferta->disciplina = d;
										}
										else
										{
											at_Oferta->setDisciplinaId( id_disc );
											at_Oferta->disciplina = d;
										}
									}
									else
									{		
										at_Oferta->setDisciplinaId( discOrig->getId() );
										at_Oferta->disciplina = d;

										if ( id_disc != discOrig->getId() )
										{
											at_Oferta->setDisciplinaSubstitutaId( id_disc );
										}																					
									}

									at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
									at_Oferta->setTurma( turma );
									at_Oferta->oferta = oferta;
									
									HorarioAula *h = hi;
									while (1)
									{
										at_Tatico->addHorarioAula( h->getId() );
										if ( h->getInicio() == hf->getInicio() ) break;
										h = h->getCalendario()->getProximoHorario( h );
										if ( h == NULL )
										{
											std::cout<<"\nErro 2!!! horario NULL antes de encontrar hf\n";
											break;
										}
									}

									at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

									++at_Tatico_Counter;
								}

							}

							#pragma endregion
							
                           at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                           it_At_Unidade->atendimentos_salas->add( at_Sala );
                        }
                     }

                     nova_Unidade = false;
                     break;

                  }
               }

               if ( nova_Unidade )
               {
				   // Cadastrando a Unidade
                   AtendimentoUnidade * at_Unidade = new AtendimentoUnidade(
                     this->problemSolution->getIdAtendimentos() );

                   at_Unidade->setId( unidade->getId() );
                   at_Unidade->setCodigoUnidade( unidade->getCodigo() );
                   at_Unidade->unidade = unidade;

                   // Cadastrando a Sala
                   AtendimentoSala * at_Sala = new AtendimentoSala(
                     this->problemSolution->getIdAtendimentos() );

                   at_Sala->setId( sala->getId() );
                   at_Sala->setSalaId( sala->getCodigo() );
                   at_Sala->sala = sala;

                   // Cadastrando o dia da semana
                   AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
                     this->problemSolution->getIdAtendimentos() );

                   at_Dia_Semana->setDiaSemana( dia );
 										
					#pragma region CADASTRO DE ATENDIMENTO TATICO PARA NOVA UNIDADE

					Trio<int,int,Disciplina*> trio;
					trio.set(campus->getId(), turma, d );

					std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
						itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
							
					if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
					{
						std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
						std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
						std::cout << "\nNao encontrado AlunosDemanda em: Disciplina: " << d->getId();
						std::cout << "\tTurma: " << turma;
						continue;
					}

					// Todas as ofertas atendidas
					map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> > mapOftDiscOriginais;
					ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMap->second, AlunoDemanda )
					{									
						Demanda *demOrig = itAlunoDemanda->demandaOriginal;
						if ( demOrig != NULL )
							mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ demOrig->disciplina ].add( *itAlunoDemanda );
						else
							mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ itAlunoDemanda->demanda->disciplina ].add( *itAlunoDemanda );
					}
																							
					map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> >::iterator
							itMapOft_DiscOrig = mapOftDiscOriginais.begin();
					for ( ; itMapOft_DiscOrig != mapOftDiscOriginais.end() ; itMapOft_DiscOrig++ )
					{
						Oferta *oferta = itMapOft_DiscOrig->first;

						map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> > 
							mapDisc_Alunos = itMapOft_DiscOrig->second;

						map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >::iterator
							itMap_DiscOrig = mapDisc_Alunos.begin();
						for ( ; itMap_DiscOrig != mapDisc_Alunos.end(); itMap_DiscOrig++ )
						{	
							Disciplina* discOrig = itMap_DiscOrig->first;																																		
										
							GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > alunosDemanda = itMap_DiscOrig->second;

							AtendimentoTatico * at_Tatico = new AtendimentoTatico(
							this->problemSolution->getIdAtendimentos(),
							this->problemSolution->getIdAtendimentos() );

							// Verificando se a disciplina é de carater prático ou teórico.
							if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
							{
								at_Tatico->setQtdCreditosTeoricos( nCreds );
							}
							else
							{
								at_Tatico->setQtdCreditosPraticos( nCreds );
							}

							AtendimentoOferta * at_Oferta = at_Tatico->atendimento_oferta;
										
							ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemanda, AlunoDemanda )
							{
								if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
								{
									int alunoId = itAlunoDemanda->getAlunoId();
									int discId = - itAlunoDemanda->demanda->getDisciplinaId();
									Aluno* aluno = problemData->retornaAluno( alunoId );

									// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
									// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
									AlunoDemanda* alunoDemanda = aluno->getAlunoDemanda( discId );
									if ( alunoDemanda != NULL )
									{
										at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
									}
									else std::cout<<"\nERROR: alunodemanda teorico nao encontrado\n";
								}
								else
								{
									at_Oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
								}
							}
																																	 
							stringstream str;
							str << oferta->getId();
							at_Oferta->setOfertaCursoCampiId( str.str() );

							int id_disc = d->getId();
											
							if ( problemData->parametros->considerar_equivalencia &&
								! problemData->parametros->considerar_equivalencia_por_aluno )
							{
								std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
								Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
								if ( discOriginal != NULL )
								{
									at_Oferta->setDisciplinaSubstitutaId( id_disc );
									at_Oferta->setDisciplinaId( discOriginal->getId() );
									at_Oferta->disciplina = d;
								}
								else
								{
									at_Oferta->setDisciplinaId( id_disc );
									at_Oferta->disciplina = d;
								}
							}
							else
							{		
								at_Oferta->setDisciplinaId( discOrig->getId() );
								at_Oferta->disciplina = d;

								if ( id_disc != discOrig->getId() )
								{
									at_Oferta->setDisciplinaSubstitutaId( id_disc );
								}																				
							}

							at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
							at_Oferta->setTurma( turma );
							at_Oferta->oferta = oferta;
														
							HorarioAula *h = hi;
							while (1)
							{
								at_Tatico->addHorarioAula( h->getId() );
								if ( h->getInicio() == hf->getInicio() ) break;
								h = h->getCalendario()->getProximoHorario( h );
								if ( h == NULL )
								{
									std::cout<<"\nErro 3!!! horario NULL antes de encontrar hf\n";
									break;
								}
							}

							at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

							++at_Tatico_Counter;
						}

					}

					#pragma endregion
							

                    at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                    at_Unidade->atendimentos_salas->add( at_Sala );
                    it_At_Campus->atendimentos_unidades->add( at_Unidade );
               }
            }

            novo_Campus = false;
            break;
         }
      }

      if ( novo_Campus )
      {
			AtendimentoCampus * at_Campus = new AtendimentoCampus(
			this->problemSolution->getIdAtendimentos() );

			at_Campus->setId( campus->getId() );
			at_Campus->setCampusId( campus->getCodigo() );
			at_Campus->campus = campus;

			// Cadastrando a Unidade
			AtendimentoUnidade * at_Unidade = new AtendimentoUnidade(
			this->problemSolution->getIdAtendimentos() );

			at_Unidade->setId( unidade->getId() );
			at_Unidade->setCodigoUnidade( unidade->getCodigo() );
			at_Unidade->unidade = unidade;

			// Cadastrando a Sala
			AtendimentoSala * at_Sala = new AtendimentoSala(
			this->problemSolution->getIdAtendimentos() );

			at_Sala->setId( sala->getId() );
			at_Sala->setSalaId( sala->getCodigo() );
			at_Sala->sala = sala;

			// Cadastrando o dia da semana
			AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
			this->problemSolution->getIdAtendimentos() );

			at_Dia_Semana->setDiaSemana( dia );
         						
			#pragma region CADASTRO DE ATENDIMENTO TATICO PARA NOVO CAMPUS

			Trio<int,int,Disciplina*> trio;
			trio.set(campus->getId(), turma, d );

			std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
				itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
							
			if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
			{
				std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
				std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
				std::cout << "\nNao encontrado AlunosDemanda em: Disciplina: " << d->getId();
				std::cout << "\tTurma: " << turma;
				continue;
			}

			// Todas as ofertas atendidas
			map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> > mapOftDiscOriginais;
			ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMap->second, AlunoDemanda )
			{									
				Demanda *demOrig = itAlunoDemanda->demandaOriginal;
				if ( demOrig != NULL )
					mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ demOrig->disciplina ].add( *itAlunoDemanda );
				else
					mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ itAlunoDemanda->demanda->disciplina ].add( *itAlunoDemanda );
			}
																							
			map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> >::iterator
					itMapOft_DiscOrig = mapOftDiscOriginais.begin();
			for ( ; itMapOft_DiscOrig != mapOftDiscOriginais.end() ; itMapOft_DiscOrig++ )
			{
				Oferta *oferta = itMapOft_DiscOrig->first;

				map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> > 
					mapDisc_Alunos = itMapOft_DiscOrig->second;

				map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >::iterator
					itMap_DiscOrig = mapDisc_Alunos.begin();
				for ( ; itMap_DiscOrig != mapDisc_Alunos.end(); itMap_DiscOrig++ )
				{	
					Disciplina* discOrig = itMap_DiscOrig->first;																																		
										
					GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > alunosDemanda = itMap_DiscOrig->second;

					AtendimentoTatico * at_Tatico = new AtendimentoTatico(
					this->problemSolution->getIdAtendimentos(),
					this->problemSolution->getIdAtendimentos() );

					// Verificando se a disciplina é de carater prático ou teórico.
					if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
					{
						at_Tatico->setQtdCreditosTeoricos( nCreds );
					}
					else
					{
						at_Tatico->setQtdCreditosPraticos( nCreds );
					}

					AtendimentoOferta * at_Oferta = at_Tatico->atendimento_oferta;
										
					ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemanda, AlunoDemanda )
					{
						if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
						{
							int alunoId = itAlunoDemanda->getAlunoId();
							int discId = - itAlunoDemanda->demanda->getDisciplinaId();
							Aluno* aluno = problemData->retornaAluno( alunoId );

							// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
							// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
							AlunoDemanda* alunoDemanda = aluno->getAlunoDemanda( discId );
							if ( alunoDemanda != NULL )
							{
								at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
							}
							else std::cout<<"\nERROR: alunodemanda teorico nao encontrado\n";
						}
						else
						{
							at_Oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
						}
					}
																																	 
					stringstream str;
					str << oferta->getId();
					at_Oferta->setOfertaCursoCampiId( str.str() );

					int id_disc = d->getId();
											
					if ( problemData->parametros->considerar_equivalencia &&
						! problemData->parametros->considerar_equivalencia_por_aluno )
					{
						std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
						Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
						if ( discOriginal != NULL )
						{
							at_Oferta->setDisciplinaSubstitutaId( id_disc );
							at_Oferta->setDisciplinaId( discOriginal->getId() );
							at_Oferta->disciplina = d;
						}
						else
						{
							at_Oferta->setDisciplinaId( id_disc );
							at_Oferta->disciplina = d;
						}
					}
					else
					{		
						at_Oferta->setDisciplinaId( discOrig->getId() );
						at_Oferta->disciplina = d;

						if ( id_disc != discOrig->getId() )
						{
							at_Oferta->setDisciplinaSubstitutaId( id_disc );
						}
					}

					at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
					at_Oferta->setTurma( turma );
					at_Oferta->oferta = oferta;
					
					HorarioAula *h = hi;
					while (1)
					{
						at_Tatico->addHorarioAula( h->getId() );
						if ( h->getInicio() == hf->getInicio() ) break;
						h = h->getCalendario()->getProximoHorario( h );
						if ( h == NULL )
						{
							std::cout<<"\nErro 4!!! horario NULL antes de encontrar hf\n";
							break;
						}
					}

					at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

					++at_Tatico_Counter;
				}
			}

			#pragma endregion							

			at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
			at_Unidade->atendimentos_salas->add( at_Sala );
			at_Campus->atendimentos_unidades->add( at_Unidade );

			problemSolution->atendimento_campus->add( at_Campus );
      }

	  // todo: posso deletar isso aqui msm? Acrescentei qdo comecei a testar solucao inicial fixada, indo direto pro TatInt
	  delete (*it_Vars_x);
   }

   // todo: posso deletar isso aqui msm? Acrescentei qdo comecei a testar solucao inicial fixada, indo direto pro TatInt
   vars_xh.clear();
}

int SolverMIP::solve()
{
   int status = 0;

#ifdef READ_SOLUTION
	this->CARREGA_SOLUCAO = true;
#endif
#ifndef READ_SOLUTION
	this->CARREGA_SOLUCAO = false;
#endif

   if ( problemData->parametros->modo_otimizacao == "TATICO" )
   {
//		#ifndef TATICO_SO_HEURN // SÓ HEURISTICA
		if ( usaSolInicialHeur )
			status = solveTaticoPorCampusComSolHeur();
		else
			status = solveTaticoPorCampusCjtAlunos();
//		#endif
   }
   else if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
   {
	  std::cout<<"\n------------------------------Operacional------------------------------\n";

      problemSolution->atendimento_campus;

      if ( problemData->atendimentosTatico != NULL
            && problemData->atendimentosTatico->size() > 0 )
      {
		  ITERA_GGROUP_LESSPTR( itAtTat, ( *problemData->atendimentosTatico ), AtendimentoCampusSolucao )
         { 
            Campus * campus = problemData->refCampus[ itAtTat->getCampusId() ];

            AtendimentoCampus * atCampus = new AtendimentoCampus( this->problemSolution->getIdAtendimentos() );

            atCampus->setId( campus->getId() );
            atCampus->setCampusId( campus->getCodigo() );
            atCampus->campus = campus;

            ITERA_GGROUP( itAtUnd, itAtTat->atendimentosUnidades, AtendimentoUnidadeSolucao )
            {
               Unidade * unidade = problemData->refUnidade[ itAtUnd->getUnidadeId() ];

               AtendimentoUnidade * atUnidade = new AtendimentoUnidade(
                  this->problemSolution->getIdAtendimentos() );

               atUnidade->setId( unidade->getId() );
               atUnidade->setCodigoUnidade( unidade->getCodigo() );
               atUnidade->unidade = unidade;

               ITERA_GGROUP( itAtSala,
                  itAtUnd->atendimentosSalas, AtendimentoSalaSolucao )
               {
                  Sala * sala = problemData->refSala[ itAtSala->getSalaId() ];

                  AtendimentoSala * atSala = new AtendimentoSala(
                     this->problemSolution->getIdAtendimentos() );

                  atSala->setId( sala->getId() );
                  atSala->setSalaId( sala->getCodigo() );
                  atSala->sala = sala;

                  ITERA_GGROUP( itAtDiaSemana,
                     itAtSala->atendimentosDiasSemana, AtendimentoDiaSemanaSolucao )
                  {
                     AtendimentoDiaSemana * atDiaSemana = new AtendimentoDiaSemana(
                        this->problemSolution->getIdAtendimentos() );

                     atDiaSemana->setDiaSemana( itAtDiaSemana->getDiaSemana() );

                     atSala->atendimentos_dias_semana->add( atDiaSemana );
                  }

                  atUnidade->atendimentos_salas->add( atSala );
               }

               atCampus->atendimentos_unidades->add( atUnidade );
            }

            problemSolution->atendimento_campus->add( atCampus );
         }

         // Resolvendo o modelo operacional

		 // -----------------------
		 Operacional * solverOp = new Operacional( this->problemData, &this->CARREGA_SOLUCAO, &(this->solVarsOp), this->problemSolution );
		 status = solverOp->solveOperacionalEtapas();
		 delete solverOp;
		 // -----------------------

      }
      else
      {
         // Neste caso, primeiro deve-se gerar uma saída para
         // o modelo tático. Em seguida, deve-se resolver o
         // modelo operacional com base na saída do modelo tático gerada.

			if ( usaSolInicialHeur )
				status = solveTaticoPorCampusComSolHeur();
			else
				status = solveTaticoPorCampusCjtAlunos();
			

			relacionaAlunosDemandas();
			
			// Write output

			std::cout<<"\nImprimindo output tatico... ";

			std::string outputFile;
			outputFile = "output_tat_";
			outputFile += problemData->getInputFileName().c_str();
			outputFile += "F";

			std::ofstream file;
			file.open( outputFile.c_str(), ios::out );
			if ( file )
			{
				file << ( *problemSolution );
				file.close();
			}
			else std::cout<<"Erro ao abrir arquivo "<< outputFile;

			std::cout<<" fim!\n";

			// Preenchendo a estrutura "atendimentosTatico".
			problemData->atendimentosTatico
				= new GGroup< AtendimentoCampusSolucao *, LessPtr< AtendimentoCampusSolucao > >();

			ITERA_GGROUP( it_At_Campus,
			( *problemSolution->atendimento_campus ), AtendimentoCampus )
			{
				std::cout<<"\nit_At_Campus..."; fflush(0);
				problemData->atendimentosTatico->add(
					new AtendimentoCampusSolucao( **it_At_Campus ) );
			}

			std::cout<<"\nRemovendo referencias para atend tatico..."; fflush(0);
			// Remove a referência para os atendimentos tático (que pertencem ao output tático)
			ITERA_GGROUP( it_At_Campus,
			( *problemSolution->atendimento_campus ), AtendimentoCampus )
			{
				ITERA_GGROUP_LESSPTR( it_At_Unidade,
					( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
				{
					ITERA_GGROUP_LESSPTR( it_At_Sala,
						( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
					{
						ITERA_GGROUP_LESSPTR( it_At_DiaSemana,
							( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
						{
							GGroup< AtendimentoTatico*, LessPtr<AtendimentoTatico> > * atendimentos_tatico
								= it_At_DiaSemana->atendimentos_tatico;

							atendimentos_tatico->clear();

							GGroup< AtendimentoTurno*, LessPtr<AtendimentoTurno> > * atendimentos_turno
								= it_At_DiaSemana->atendimentos_turno;

							atendimentos_turno->clear();
						}
					}
				}
			}
	    
			std::cout<<"\n\n\n------------------------------Operacional------------------------------\n\n";

			// Criando as aulas que serão utilizadas
			// para resolver o modelo operacional
			problemDataLoader->criaAulas();

			// Resolvendo o modelo operacional
			// -----------------------
			Operacional * solverOp = new Operacional( this->problemData, &this->CARREGA_SOLUCAO, &(this->solVarsOp), this->problemSolution );
			status = solverOp->solveOperacionalEtapas();
			delete solverOp;
			// -----------------------
      }	  

   }
      
   relacionaAlunosDemandas();
   
   int runtime = this->getRunTime();
   int hours = runtime / 3600;						// h
   int min = (int) ( (int) runtime % 3600 ) / 60;	// min
   std::stringstream runtimess;
   runtimess << hours << "h" << min;

   std::cout << "\n\nTotal Run Time = " << runtimess.str() << endl << endl;


   return status;
}

// Método que relaciona cada demanda atendida aos
// correspondentes alunos que assistirão as aulas 
void SolverMIP::relacionaAlunosDemandas()
{
	std::cout<<"\nRelacionando AlunosDemanda...";

   Campus * campus = NULL;
   Unidade * unidade = NULL;
   Sala * sala = NULL;
   int dia_semana = 0;

   // Lendo os atendimentos oferta da solução
   GGroup< AtendimentoOferta * > atendimentosOferta;

   ITERA_GGROUP( it_At_Campus,
      ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      // Campus do atendimento
      campus = it_At_Campus->campus;

      ITERA_GGROUP_LESSPTR( it_At_Unidade,
         ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
      {
         // Unidade do atendimento
         unidade = problemData->refUnidade[ it_At_Unidade->getId() ];

         ITERA_GGROUP_LESSPTR( it_At_Sala,
            ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
         {
            // Sala do atendimento
            sala = problemData->refSala[ it_At_Sala->getId() ];

            ITERA_GGROUP_LESSPTR( it_At_DiaSemana,
               ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               // Dia da semana do atendimento
               dia_semana = it_At_DiaSemana->getDiaSemana();

               // Modelo Tático
               if ( it_At_DiaSemana->atendimentos_tatico != NULL
                  && it_At_DiaSemana->atendimentos_tatico->size() > 0 )
               {
                  ITERA_GGROUP_LESSPTR( it_at_tatico,
                     ( *it_At_DiaSemana->atendimentos_tatico ), AtendimentoTatico )
                  {
                     AtendimentoTatico * at_tatico = ( *it_at_tatico );

                     atendimentosOferta.add( at_tatico->atendimento_oferta );
                  }
               }
               // Modelo Operacional
               else if ( it_At_DiaSemana->atendimentos_turno != NULL
                  && it_At_DiaSemana->atendimentos_turno->size() > 0 )
               {
                  ITERA_GGROUP_LESSPTR( it_at_turno,
                     ( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
                  {
                     AtendimentoTurno * at_turno = ( *it_at_turno );

                     ITERA_GGROUP_LESSPTR( it_horario_aula,
                        ( *at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        AtendimentoHorarioAula * horario_aula = ( *it_horario_aula );

                        ITERA_GGROUP_LESSPTR( it_oferta,
                           ( *it_horario_aula->atendimentos_ofertas ), AtendimentoOferta )
                        {
                           AtendimentoOferta * oferta = ( *it_oferta );

                           atendimentosOferta.add( oferta );
                        }
                     }
                  }
               }
            }
         }
      }
   }
   // Fim da leitura dos atendimentos oferta

   // Armazenando a informação de quantos alunos
   // de cada demanda foram atendidos pela solução
   std::map< Demanda *, int > quantidadeAlunosAtendidosDemanda;

   ITERA_GGROUP( it_at_oferta, atendimentosOferta, AtendimentoOferta )
   {
      AtendimentoOferta * at_oferta = ( *it_at_oferta );

      int id_oferta = atoi( at_oferta->getOfertaCursoCampiId().c_str() );
      int id_disciplina = at_oferta->getDisciplinaId();

      Demanda * demanda = this->problemData->buscaDemanda( id_oferta, id_disciplina );

      quantidadeAlunosAtendidosDemanda[ demanda ] += at_oferta->getQuantidade();
   }

   // Preenchendo alunosDemanda com cada demanda atendida de cada aluno
   if ( problemData->parametros->otimizarPor == "ALUNO" )
   {
	   ITERA_GGROUP_LESSPTR( it_alunosDemanda, problemData->alunosDemanda, AlunoDemanda )
	   {
		   AlunoDemanda * aluno_demanda = ( *it_alunosDemanda );

		   Disciplina * disc = aluno_demanda->demanda->disciplina;
		   int alunoId = aluno_demanda->getAlunoId();

		   Aluno* a = problemData->retornaAluno( alunoId );

		   GGroup< Trio<int, int, Disciplina*> > campusTurmaDiscAluno = problemData->mapAluno_CampusTurmaDisc[a];
		   for ( GGroup< Trio<int, int, Disciplina*> >::iterator it_at_aluno = campusTurmaDiscAluno.begin();
				 it_at_aluno != campusTurmaDiscAluno.end(); it_at_aluno++ )
		   {
			   if ( (*it_at_aluno).third->getId() == disc->getId() )
			   {
					this->problemSolution->alunosDemanda->add( aluno_demanda );
			   }
		   }
	   }
   }

   // Preenchendo alunosDemanda de acordo com o numero de demandas atendidas
   // (Escolhe os alunos que foram atendidos 'aleatoriamente' )
   else if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
   {
	   std::cout<<"\nNot done. Exiting...\n";
	   exit(1);
   }
   std::cout<<" done!\n";
}


bool SolverMIP::aulaAlocada( Aula * aula, Campus * campus,
   Unidade * unidade, Sala * sala, int dia_semana ) 
{
   if ( aula == NULL || aula->eVirtual() )
   {
      return false;
   }

   bool aula_alocada = true;

   int id_unidade_aula = aula->getSala()->getIdUnidade();
   int id_campus_aula = problemData->refUnidade[ id_unidade_aula ]->getIdCampus();
   int id_sala_aula = aula->getSala()->getId();
   int dia_semana_aula = aula->getDiaSemana();

   if ( id_campus_aula != campus->getId() || id_unidade_aula != unidade->getId()
      || id_sala_aula != sala->getId() || dia_semana_aula != dia_semana )
   {
      aula_alocada = false;
   }

   return aula_alocada;
}


int SolverMIP::localBranchingHor( double * xSol, double maxTime )
{
   // Adiciona restrição de local branching
   int status = 0;
   int nIter = 0;
   int * idxSol = new int[ lp->getNumCols() ];

   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      idxSol[ i ] = i;
   }

   double objAtual = 100000000000.0;
   double okIter = true;

   int k = -10;

   FILE * fin = fopen( "solPolAtual.bin", "rb" );
   if ( fin == NULL )
   {
      std::cout << "\nErro em SolverMIP::solveTaticoBasicoCjtAlunos( int campusId, int prioridade, int cjtAlunosId ):"
         << "\nArquivo " << "solLBAtual.bin" << " nao pode ser aberto.\n";
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

   int tempoIter = 600;
   k = -3;

   while (okIter)
   {
      VariableTaticoHash::iterator vit = vHashTatico.begin();

      OPT_ROW nR( 50, OPT_ROW::GREATER, 0.0, "LOCBRANCH" );
      double rhsLB = k;

      while ( vit != vHashTatico.end() )
      {
         if ( vit->first.getType() == VariableTatico::V_ABERTURA )
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
      lp->setNodeLimit( 100000000 );
      lp->setPolishAfterNode(1);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 1 );
      lp->setCuts(4);
      //lp->setNoCuts();
      lp->setVarSel(4);
      lp->setHeurFrequency( 1.0 );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );

      status = lp->optimize( METHOD_MIP );

      double objN = lp->getObjVal();

      if ( fabs(objAtual-objN) < 100.0  )
      {
         lp->setNodeLimit(10000000);
         lp->setPolishAfterNode(1);
         lp->setTimeLimit( tempoIter );
         status = lp->optimize( METHOD_MIP );

         if ( k == -100 )
            k = -50;

         if ( k == -150 )
            k = -100;
      }

      lp->getX( xSol );

      FILE * fout = fopen( "solLBAtual.bin", "wb" );
		if ( fout == NULL )
		{
			std::cout << "\nErro em SolverMIP::solveTaticoBasicoCjtAlunos( int campusId, int prioridade, int cjtAlunosId ):"
					<< "\nArquivo " << "solLBAtual.bin" << " nao pode ser aberto.\n";
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

      if ( fabs(objN-objAtual) < 0.0001 && k >= -10 )
      {
         okIter = false;
      }
      else if (fabs(objN-objAtual) < 0.0001 && k < -10 )
      {
         k += 10;
         tempoIter += 300;
      }

      objAtual = objN;

      int * idxs = new int[ 1 ];
      idxs[ 0 ] = lp->getNumRows() - 1;
      lp->delSetRows( 1, idxs );
      lp->updateLP();
      delete [] idxs;
   }

   delete [] idxSol;

   return status;
}

void SolverMIP::fixaVarsMutacaoTaticoHor(double *xSol, int perc, int strateg)
{
   int *idxs = new int[lp->getNumCols()*2];
   double *vals = new double[lp->getNumCols()*2];
   BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
   int nBds = 0;

   VariableTaticoHash::iterator vit;
   
   switch(strateg)
   {
   case 1: // fixa variaveis z
      {
         vit= vHashTatico.begin();
         nBds = 0;
         while ( vit != vHashTatico.end() )
         {
            if ( vit->first.getType() == VariableTatico::V_ABERTURA )
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
               }
               else
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = 0.0;
                  bds[nBds] = BOUNDTYPE::BOUND_UPPER;
                  nBds++;
               }
            }

            vit++;
         }
         lp->chgBds(nBds,idxs,bds,vals);
         lp->updateLP();
         break;
      }
   /*case 2: // fixa variaveis x
      {
         vit= vHashTatico.begin();
         nBds = 0;
         while ( vit != vHashTatico.end() )
         {
            if ( vit->first.getType() == VariableTatico::V_CREDITOS )
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
               }
               else
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = 0.0;
                  bds[nBds] = BOUNDTYPE::BOUND_UPPER;
                  nBds++;
               }
            }

            vit++;
         }
         lp->chgBds(nBds,idxs,bds,vals);
         lp->updateLP();
         break;
      }*/

   case 2: // fixa variaveis x e z
      {
         std::set<std::pair<int,Disciplina*> > paraFixarUm;
         std::set<std::pair<int,Disciplina*> > paraFixarZero;
         vit= vHashTatico.begin();
         nBds = 0;
         while ( vit != vHashTatico.end() )
         {
            if ( vit->first.getType() == VariableTatico::V_ABERTURA )
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
         lp->chgBds(nBds,idxs,bds,vals);
         lp->updateLP();

         vit = vHashTatico.begin();
         nBds = 0;
         while ( vit != vHashTatico.end() )
         {
            if ( vit->first.getType() == VariableTatico::V_CREDITOS )
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

            vit++;
         }
         lp->chgBds(nBds,idxs,bds,vals);
         lp->updateLP();
         break;
      }
   }

   delete[] idxs;
   delete[] vals;
   delete[] bds;
}

void SolverMIP::fixaVarsCombTaticoHor(double *xSol1, double *xSol2)
{
   int *idxs = new int[lp->getNumCols()*2];
   double *vals = new double[lp->getNumCols()*2];
   BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
   int nBds = 0;

   VariableTaticoHash::iterator vit;

   vit= vHashTatico.begin();
   nBds = 0;
   while ( vit != vHashTatico.end() )
   {
      if ( vit->first.getType() == VariableTatico::V_CREDITOS )
      {
         if (xSol1[vit->second] > 0.9 && fabs(xSol1[vit->second]-xSol2[vit->second]) < 0.001 )
         {
            
            idxs[nBds] = vit->second;
            vals[nBds] = (int)(xSol1[vit->second]+0.5);
            bds[nBds] = BOUNDTYPE::BOUND_LOWER;
            nBds++;
            idxs[nBds] = vit->second;
            vals[nBds] = (int)(xSol1[vit->second]+0.5);
            bds[nBds] = BOUNDTYPE::BOUND_UPPER;
            nBds++;
         }
      }
      vit++;
   }
   lp->chgBds(nBds,idxs,bds,vals);
   lp->updateLP();      

   delete[] idxs;
   delete[] vals;
   delete[] bds;
}

void SolverMIP::desfixaVarsTaticoHor(double *lbs, double *ubs)
{
   int *idxs = new int[lp->getNumCols()*2];
   double *vals = new double[lp->getNumCols()*2];
   BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
   int nBds = 0;

   for (int i=0; i < lp->getNumCols(); i++)
   {
      idxs[nBds] = i;
      vals[nBds] = ubs[i];
      bds[nBds] = BOUNDTYPE::BOUND_UPPER;
      nBds++;
      idxs[nBds] = i;
      vals[nBds] = lbs[i];
      bds[nBds] = BOUNDTYPE::BOUND_LOWER;
      nBds++;
   }

   lp->chgBds(nBds,idxs,bds,vals);
   lp->updateLP();

   delete[] idxs;
   delete[] vals;
   delete[] bds;
}

bool SolverMIP::mutacaoTaticoHor(double* &xSol, double &objSol, int perc, double tempoLim, bool paradaMelhora,SolutionPool* &pool)
{
   double objAtual = objSol;
   CPUTimer tempoMut;
   bool okMut = true;
   int tempoIter = 180.0; 
   int estrategia = 1;
   bool melhorouSol = false;
   int *idxSol = new int[lp->getNumCols()];
   double *xSolAtual = new double[lp->getNumCols()];
   double *ubVars = new double[ lp->getNumCols() ];
   double *lbVars = new double[ lp->getNumCols() ];

   // Armazena bounds originais
   lp->getUB(0,lp->getNumCols()-1,ubVars);
   lp->getLB(0,lp->getNumCols()-1,lbVars);

   for (int i=0; i < lp->getNumCols(); i++)
   {
      idxSol[i] = i;
      xSolAtual[i] = xSol[i];
   }

   // Inicia contagem de tempo
   tempoMut.reset();
   tempoMut.start();

   while(okMut)
   {
      // Seleciona estrategia
      estrategia = (estrategia % 2) + 1;

      printf("Fixando mutacao...\n");
      fixaVarsMutacaoTaticoHor(xSolAtual,perc,estrategia);

      bool melhorouIter = false;

      // Parametros para rodar
#ifdef SOLVER_CPLEX
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setMIPRelTol( 0.0 );
      lp->setPolishAfterNode(100000000);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 4 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(1000);
      lp->setHeurFrequency( 1.0 );
#endif
#ifdef SOLVER_GUROBI
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setMIPRelTol( 0.0 );
      lp->setMIPEmphasis( 0 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(1000);
      lp->setHeurFrequency( 1.0 );
      lp->updateLP();
#endif
      printf("Carregando sol inic mutacao...\n");
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSolAtual );
      lp->updateLP();

      printf("Otimizando mutacao...\n");
      int status = lp->optimize( METHOD_MIP );

      double objN = lp->getObjVal();

      if ( objN <= (objAtual - 0.1)) 
      {
         objAtual = objN;
         lp->getX( xSolAtual );
         melhorouSol = true;
         melhorouIter = true;

         // Insere no pool
         pool->addSolution(xSolAtual,objAtual);
      }

      tempoMut.stop();
      double tempoAtual = tempoMut.getCronoTotalSecs();
      tempoMut.start();
      if ( tempoAtual >= tempoLim )
      {
         okMut = false;
         tempoMut.stop();
      }
      else if ( paradaMelhora && !melhorouIter )
      {
         okMut = false;
         tempoMut.stop();
      }
      // Desfixa variaveis
       printf("Desfixando mutacao...\n");
      desfixaVarsTaticoHor(lbVars,ubVars);
   }

   if ( melhorouSol )
   {
      objSol = objAtual;
      for (int i=0; i < lp->getNumCols(); i++)
      {
         xSol[i] = xSolAtual[i];
      }
   }

   delete[] idxSol;
   delete[] xSolAtual;
   delete[] lbVars;
   delete[] ubVars;

   return melhorouSol;
}

bool SolverMIP::combTaticoHor(double* &xSol, double &objSol, double tempoLim, bool paradaMelhora, SolutionPool* &pool)
{
   double objAtual = objSol;
   CPUTimer tempoComb;
   bool okComb = true;
   int tempoIter = 180.0; 
   int estrategia = 1;
   bool melhorouSol = false;
   int *idxSol = new int[lp->getNumCols()];
   double *xSolAtual = new double[lp->getNumCols()];
   double *ubVars = new double[ lp->getNumCols() ];
   double *lbVars = new double[ lp->getNumCols() ];
   int *mBeg = new int[2];
   int *idxsSols = new int[lp->getNumCols()*2];
   double *valsSols = new double[lp->getNumCols()*2];

   // Armazena bounds originais
   lp->getUB(0,lp->getNumCols()-1,ubVars);
   lp->getLB(0,lp->getNumCols()-1,lbVars);

   for (int i=0; i < lp->getNumCols(); i++)
   {
      idxSol[i] = i;
      xSolAtual[i] = xSol[i];
   }

   // Inicia contagem de tempo
   tempoComb.reset();
   tempoComb.start();

   if ( pool->getPoolSize() <= 1 )
   {
      okComb = false;
   }

   while(okComb)
   {
      // Seleciona solucoes no pool
      int idx1 = rand() % pool->getPoolSize();
      int idx2 = rand() % pool->getPoolSize();

      if ( idx1 == idx2  )
      {
         tempoComb.stop();
         double tempoAtual = tempoComb.getCronoTotalSecs();
         tempoComb.start();
         if ( tempoAtual >= tempoLim )
         {
            okComb = false;
            tempoComb.stop();
         }
         continue;
      }

      printf("Fixando combinacao...\n");
      fixaVarsCombTaticoHor(pool->getSolution(idx1),pool->getSolution(idx2));

      double *x1 = pool->getSolution(idx1);
      double *x2 = pool->getSolution(idx2);

      // Parametros para rodar
#ifdef SOLVER_CPLEX
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setMIPRelTol( 0.0 );
      lp->setPolishAfterNode(1);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 4 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(1000);
      lp->setHeurFrequency( 1.0 );
#endif
#ifdef SOLVER_GUROBI
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setMIPRelTol( 0.0 );
      lp->setMIPEmphasis( 0 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(1000);
      lp->setHeurFrequency( 1.0 );
      lp->updateLP();
#endif
      // Copia solucoes iniciais
      for (int j=0; j < lp->getNumCols(); j++)
      {
         idxsSols[j] = j;
         valsSols[j] = x1[j];
      }
      for (int j=0; j < lp->getNumCols(); j++)
      {
         idxsSols[j+lp->getNumCols()] = j;
         valsSols[j+lp->getNumCols()] = x2[j];
      }
      mBeg[0] = 0;
      mBeg[1] = lp->getNumCols();
      lp->copyMIPStartSols (2, 2*lp->getNumCols(), mBeg, idxsSols, valsSols, NULL);
      lp->updateLP();

      printf("Otimizando combinacao...\n");
      int status = lp->optimize( METHOD_MIP );
      double objN = lp->getObjVal();
      printf("Otimizado combinacao: status=%i, obj=%f\n",status,objN);

      if ( objN <= (objAtual-0.1) )
      {
         objAtual = objN;
         lp->getX( xSolAtual );
         melhorouSol = true;

         objSol = objAtual;
         for (int i=0; i < lp->getNumCols(); i++)
         {
            xSol[i] = xSolAtual[i];
         }

         // Insere no pool
         pool->addSolution(xSolAtual,objAtual);
      }

      tempoComb.stop();
      double tempoAtual = tempoComb.getCronoTotalSecs();
      tempoComb.start();
      if ( tempoAtual >= tempoLim )
      {
         okComb = false;
         tempoComb.stop();
      }
      else if ( paradaMelhora && melhorouSol )
      {
         okComb = false;
         tempoComb.stop();
      }
      // Desfixa variaveis
      printf("Desfixando combinacao...\n");
      desfixaVarsTaticoHor(lbVars,ubVars);
   }

   delete[] idxSol;
   delete[] xSolAtual;
   delete[] lbVars;
   delete[] ubVars;
   delete[] mBeg;
   delete[] idxsSols;
   delete[] valsSols;

   return melhorouSol;
}

void SolverMIP::polishMutacaoCombTaticoHor(double *xSol, double maxTime, int percIni, int percMin)
{
   CPUTimer tempoPol; // tempo atual de polish
   bool okIter = true; // controla fim do polish
   SolutionPool *pool = new SolutionPool(10,lp->getNumCols());
   int perc = percIni;
   double objAtual = 100000000000.0;

   // Inicia numero aleatorio
   srand(123);

   // Inicia contagem de tempo
   tempoPol.reset();
   tempoPol.start();

   while (okIter)
   {
      printf("Mutacao...\n");
      bool melhorou1 = mutacaoTaticoHor(xSol,objAtual,perc,600,false,pool);

      tempoPol.stop();
      double tempoAtual = tempoPol.getCronoTotalSecs();
      tempoPol.start();
      if ( tempoAtual >= maxTime )
      {
         okIter = false;
         tempoPol.stop();
         continue;
      }

      printf("Combinacao...\n");
      bool melhorou2 = combTaticoHor(xSol,objAtual,300.0,false,pool);

      if ( !melhorou1 )
      {
         perc-= 5;
         if ( perc < percMin )
            perc = percIni;
      }

      tempoPol.stop();
      tempoAtual = tempoPol.getCronoTotalSecs();
      tempoPol.start();
      if ( tempoAtual >= maxTime )
      {
         okIter = false;
         tempoPol.stop();
         continue;
      }
   }
   
   delete pool;
}

void SolverMIP::polishTaticoHor(double *xSol, double maxTime, int percIni, int percMin, double maxTempoSemMelhora)
{
	std::cout << "\n\n--------------\nIniciando polish manual...\n";

   // Adiciona restrição de local branching
   int status = 0;
   int nIter = 0;
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

   CPUTimer tempoSemMelhora;
   tempoSemMelhora.reset();
   tempoSemMelhora.start();

   srand(123);

   lp->getUB(0,lp->getNumCols()-1,ubVars);
   lp->getLB(0,lp->getNumCols()-1,lbVars);

   int tempoIter = 300;
   int perc = percIni;
   int *idxs = new int[lp->getNumCols()*2];
   double *vals = new double[lp->getNumCols()*2];
   BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
   int nBds = 0;

   int fixType = 2;


// Comentei isso pq estava gastando tempo inutil na raiz. Quando esse trecho é necessario,
// dado que o polish ja não é chamado para teste?
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
//      lp->updateLP();
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


   while (okIter)
   {
	   fflush(NULL);
	   std::cout << "\n------>\nPerc = " << perc << "\nTempoIter = " << tempoIter << endl;
	   fflush(NULL);

      VariableTaticoHash::iterator vit = vHashTatico.begin();

      // Seleciona turmas e disciplinas para fixar
      std::set<std::pair<int,Disciplina*> > paraFixarUm;
      std::set<std::pair<int,Disciplina*> > paraFixarZero;
      std::map<Trio<int,Disciplina*,int>,int > paraFixarDia;

      if ( fixType == 1 )
      {
         nBds = 0;
         while ( vit != vHashTatico.end() )
         {
            if ( vit->first.getType() == VariableTatico::V_ABERTURA )
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
         lp->chgBds(nBds,idxs,bds,vals);
         lp->updateLP();
      }

      vit = vHashTatico.begin();

      nBds = 0;
      while ( vit != vHashTatico.end() )
      {
         if ( vit->first.getType() == VariableTatico::V_CREDITOS )
         {
            if ( fixType == 1 )
            {
               std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
               if ( paraFixarUm.find(auxPair) != paraFixarUm.end() )
               {
                  if ( xSol[vit->second] > 0.1 )
                  {
                     //lp->chgLB(vit->second,(int)(xSol[vit->second]+0.5));
                     //lp->chgUB(vit->second,(int)(xSol[vit->second]+0.5));
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
                     //lp->chgUB(vit->second,0.0);
                  }
               }
               else if ( paraFixarZero.find(auxPair) != paraFixarZero.end() )
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = 0.0;
                  bds[nBds] = BOUNDTYPE::BOUND_UPPER;
                  nBds++;
                  //lp->chgUB(vit->second,0.0);
               }
            }
            else
            {
               if ( rand() % 100 >= perc  )
               {
                  vit++;
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
                  //lp->chgLB(vit->second,(int)(xSol[vit->second]+0.5));
                  //lp->chgUB(vit->second,(int)(xSol[vit->second]+0.5));
               }
               else
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = 0.0;
                  bds[nBds] = BOUNDTYPE::BOUND_UPPER;
                  nBds++;
                  lp->chgUB(vit->second,0.0);
               }
            }
         }

         vit++;
      }

      lp->chgBds(nBds,idxs,bds,vals);
      lp->updateLP();

#ifdef SOLVER_CPLEX
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setMIPRelTol( 0.01 );
      lp->setPolishAfterNode(1);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 4 );
      //lp->setCuts(1);
      //if ( perc < percIni )
      //   lp->setCuts(1);
      //lp->setNoCuts();
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(200);
      lp->setHeurFrequency( 1.0 );
#endif
#ifdef SOLVER_GUROBI
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setMIPRelTol( 0.1 );
      lp->setMIPEmphasis( 0 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(200);
      lp->setHeurFrequency( 1.0 );
      lp->updateLP();
#endif
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
      lp->updateLP();
	  
      status = lp->optimize( METHOD_MIP );

      double objN = lp->getObjVal();

      lp->getX( xSol );

      if ( fabs(objN-objAtual) < 0.0001 && perc < percMin )
      {
         if ( fixType == 2 )
         {
            perc = percIni-40;
            fixType = 2;
            tempoIter = 300;
         }
         else
         {
            perc = percIni-40;
            fixType = 2;
            tempoIter = 300;
         }
      }
      else if ( fabs(objN-objAtual) < 0.0001 && perc >= percMin)
      {
         if ( fixType == 2 )
         {
            perc -= 5;
            lp->setCuts(1);
            tempoIter += 20;
         }
         else
         {
            perc -= 5;
            lp->setCuts(1);
            tempoIter += 20;
         }
      }

	  if ( fabs(objN-objAtual) < 0.0001 )
	  {
		  /* no improvement */
		  tempoSemMelhora.stop();
		  double tempoAtual = tempoSemMelhora.getCronoTotalSecs();
		  tempoSemMelhora.start();
		  if ( tempoAtual >= maxTempoSemMelhora )
		  {
			 /* if there is too much time without any improvement, then quit */
			 okIter = false;
			 tempoSemMelhora.stop();
			 tempoPol.stop();
			 cout << "Abort by timeWithoutChange. Limit of time without improvement" << tempoAtual << ", BestObj " << objN;
		  }
	  }
	  else
	  {
		  tempoSemMelhora.stop();
		  tempoSemMelhora.reset();
		  tempoSemMelhora.start();		
	  }

      objAtual = objN;
	  
	  if ( okIter )
      {
		  tempoPol.stop();
		  double tempoAtual = tempoPol.getCronoTotalSecs();
		  tempoPol.start();
		  if ( tempoAtual >= maxTime )
		  {
			 okIter = false;
			 tempoPol.stop();
			 tempoSemMelhora.stop();
		  }
	  }
	  
      // Volta bounds
      vit = vHashTatico.begin();

      nBds = 0;
      while ( vit != vHashTatico.end() )
      {
         if ( vit->first.getType() == VariableTatico::V_CREDITOS || vit->first.getType() == VariableTatico::V_ABERTURA )
         {
            idxs[nBds] = vit->second;
            vals[nBds] = ubVars[vit->second];
            bds[nBds] = BOUNDTYPE::BOUND_UPPER;
            nBds++;
            idxs[nBds] = vit->second;
            vals[nBds] = lbVars[vit->second];
            bds[nBds] = BOUNDTYPE::BOUND_LOWER;
            nBds++;
            //lp->chgLB(vit->second,lbVars[vit->second]);
            //lp->chgUB(vit->second,ubVars[vit->second]);
         }
         vit++;
      }
	  
      lp->chgBds(nBds,idxs,bds,vals);
      lp->updateLP();

#ifdef SOLVER_CPLEX
      lp->setPolishAfterNode(1);
      lp->setMIPRelTol( 0.0 );
#endif
   }
   
	// -------------------------------------------------------------
    // Garante que não dará erro se houver um getX depois desse polish,
    // já que o lp sobre alteração nos bounds no final.
    lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
	lp->setTimeLimit( 50 );
	status = lp->optimize( METHOD_MIP );
	lp->getX(xSol);
	// -------------------------------------------------------------


   delete [] idxSol;
   delete [] ubVars;
   delete [] lbVars;
   delete [] idxs;
   delete [] vals;
   delete [] bds;
}






   /********************************************************************
   **    CRIAÇÃO DE VARIAVEIS E RESTRIÇÕES DO TATICO COM HORARIOS     **
   *********************************************************************/ 


/* ----------------------------------------------------------------------------------
							VARIAVEIS TATICO POR ALUNO COM HORARIOS
 ---------------------------------------------------------------------------------- */

int SolverMIP::criaVariaveisTatico( int campusId, int P, int r, int tatico )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif

	std::cout << "\n\n";

	if ( tatico==1 || CRIA_NOVAS_VARIAVEIS_TAT_HOR2 )
	{

	timer.start();
	num_vars += criaVariavelTaticoCreditosAtravesDePreSol( campusId, P, r, tatico );	// x
	//num_vars += criaVariavelTaticoCreditos( campusId, P, r, tatico );					// x
   timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += criaVariavelTaticoAbertura( campusId, P, r ); // z
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	/*
	timer.start();
	//num_vars += criaVariavelTaticoConsecutivos( campusId, P ); // c
	num_vars += criaVariavelTaticoConsecutivosAPartirDeX( campusId, P ); // c
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"c\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	*/

	timer.start();
	num_vars += criaVariavelTaticoCombinacaoDivisaoCredito( campusId, P ); // m_{i,d,k}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"m\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

   /*timer.start();
	num_vars += criaVariavelTaticoSalaTurma( campusId, P, r, tatico ); // ys
   timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"ys\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif*/

	timer.start();
	num_vars += criaVariavelTaticoFolgaCombinacaoDivisaoCredito( campusId, P ); // fk_{i,d,k}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fkp e fkm\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
//	num_vars += criaVariavelTaticoFolgaDistCredDiaSuperior( campusId, P ); // fcp_{d,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcp\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
   		

	timer.start();
//	num_vars += criaVariavelTaticoFolgaDistCredDiaInferior( campusId, P ); // fcm_{d,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcm\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	

	timer.start();
	num_vars += criaVariavelTaticoAberturaCompativel( campusId, P ); // zc
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"zc\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += criaVariavelTaticoFolgaDemandaDisc( campusId, P, r ); // fd
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
		
	/*
	timer.start();
	num_vars += criaVariavelTaticoAlunoUnidDia( campusId ); // y_{a,u,t} 
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"y\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
		

	timer.start();
	num_vars += criaVariavelTaticoAlunoUnidadesDifDia( campusId ); // w_{a,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"w\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	*/

	timer.start();
	num_vars += criaVariavelTaticoFolgaAlunoUnidDifDia( campusId, P ); // fu_{i1,d1,i2,d2,t,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fu\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += criaVariavelTaticoFolgaFolgaDemandaPT( campusId, P ); // ffd_{i1,-d,i2,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"ffd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += criaVariavelTaticoDiaUsadoPeloAluno( campusId, P ); // du_{a,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"du\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
		

	timer.start();
	num_vars += criaVariavelTaticoFolgaProfAPartirDeX( campusId, P ); // fp_{d,t,h}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fp\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += criaVariavelTaticoFolgaMinimoDemandaAtendPorAluno( campusId, P ); // fmd_{a}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fmd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	}

	// Se é tático 2, então estamos reaproveitando as variaveis, e só temos que criar as variaveis fa, fad e f,
	// e fixar algumas atendidas.
	if ( tatico==2 )
	{
		num_vars = lp->getNumCols();
		numVarsAnterior = num_vars;

		// TODO
	//	timer.start();
	//	num_vars += criaVariavelTaticoCreditosAtravesDePreSol( campusId, P, r, tatico );	// x
	//	timer.stop();
	//	dif = timer.getCronoCurrSecs();

	//#ifdef PRINT_cria_variaveis
	//	std::cout << "numVars \"x adicionadas\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	//	numVarsAnterior = num_vars;
	//#endif


		timer.start();
		num_vars += criaVariavelTaticoDesalocaAluno( campusId, P, tatico ); // fa_{i,d,a}
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fa\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif

		timer.start();
		num_vars += criaVariavelTaticoDesalocaAlunoDiaHor( campusId, P, tatico ); // fad_{i,d,a,t,h}
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fad\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif

		timer.start();
		num_vars += criaVariavelTaticoFormandosNaTurma( campusId, P, r, tatico ); // f_{i,d,cp}
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"f\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif
		
		//if ( !CRIA_NOVAS_VARIAVEIS_TAT_HOR2 )
		//{
			fixaAtendimentosVariaveisCreditosAnterior();
			liberaBoundsVariaveis_FFD_FD_FP();
		//}
	}
	
	return num_vars;

}

// x_{i,d,u,s,hi,hf,t}
int SolverMIP::criaVariavelTaticoCreditosAtravesDePreSol( int campusId, int P, int r, int tatico )
{
	int numVars = 0;
	
	Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itPreVar, solVarsXPre, VariablePre )
	{
		VariablePre *v = *itPreVar;

		if ( v->getType() != VariablePre::V_PRE_OFERECIMENTO )
			continue;
		
		int turma = v->getTurma();
		Disciplina * disciplina = v->getDisciplina();
		ConjuntoSala *cjtSala = v->getSubCjtSala();
		int cpId = cjtSala->getCampusId();

		if ( cpId != campusId )
			continue;	
		
		// -------------------------------------------------------------
		// Se estiver no tatico 2, só cria possíveis novas variaveis para turmas não atendidas,
		// possivelmente incluindo horarios que não são comuns a todos os alunos, mas somente
		// a um subconjunto dos alunos.
		
		// TODO: como excluir o aluno da turma automaticamente se o horario da turma escolhido nao estiver em seu turno?
		// conferir no modelo do tatico 2

		if ( tatico == 2 )
		if ( turmaAtendida(turma, disciplina, campus) )
			continue;


		// -------------------------------------------------------------
		// Horarios comuns entre os alunos da turma

		Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio;
		trio.set( campusId, turma, disciplina->getId() );
		
		GGroup< HorarioDia*, LessPtr<HorarioDia> > horariosDiaComunsAosAlunos;						
		if ( tatico==2 )
			horariosDiaComunsAosAlunos = problemData->retornaHorariosDiaTurnoIESUniao(trio);
		else
			horariosDiaComunsAosAlunos = problemData->retornaHorariosDiaTurnoIESIntersecao(trio);
					
		if ( horariosDiaComunsAosAlunos.size()==0 )
			std::cout<<"\nErro em SolverMIP::criaVariavelTaticoCreditosAtravesDePreSol(...): "
				<< "turma"<< turma <<" disc" << disciplina->getId() << " sem horarios em comum para todos os alunos\n";
		
		// -------------------------------------------------------------
		// Horarios comuns entre a disciplina e a sala

		int salaId = cjtSala->salas.begin()->first;
		Sala *sala = cjtSala->salas.begin()->second;
		Unidade *unidade = problemData->refUnidade[ sala->getIdUnidade() ];
		std::pair< int, int > parDiscSala = std::make_pair( disciplina->getId(), salaId );

		std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, 
				  std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > >::iterator
			it_Disc_Sala_Dias_HorariosAula = problemData->disc_Salas_Dias_HorariosAula.find( parDiscSala );

		if ( it_Disc_Sala_Dias_HorariosAula == problemData->disc_Salas_Dias_HorariosAula.end() )
		{			
			std::cout<<"\nErro em SolverMIP::criaVariavelTaticoCreditosAtravesDePreSol(...): "
				<< "disc " << disciplina->getId() << " com horarios vazios com sala "<< salaId;
			continue;
		}

		std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > 
			dia_horariosAula_comuns = (it_Disc_Sala_Dias_HorariosAula->second);

		// -------------------------------------------------------------
		// Faz a interseção entre os horarios comuns dos alunos e os horarios comuns da disc-sala

		GGroup< HorarioDia *, LessPtr< HorarioDia > > remover;
		std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
			it_Dia_HorarioAula = dia_horariosAula_comuns.begin();					
		for ( ; it_Dia_HorarioAula != dia_horariosAula_comuns.end(); it_Dia_HorarioAula++ )
		{
			int dia = it_Dia_HorarioAula->first;
			ITERA_GGROUP_LESSPTR( itHorAula, it_Dia_HorarioAula->second, HorarioAula )
			{
				HorarioDia *hd = problemData->getHorarioDiaCorrespondente( *itHorAula, dia );
				bool contem = false;
				ITERA_GGROUP_LESSPTR( itHD, horariosDiaComunsAosAlunos, HorarioDia )
				{
					if ( itHD->igual( *hd ) )
						contem = true;
				}
				if ( !contem )
				{
					remover.add( hd );
				}
			}
		}
		ITERA_GGROUP_LESSPTR( itRemover, remover, HorarioDia )
		{
			dia_horariosAula_comuns[itRemover->getDia()].remove(itRemover->getHorarioAula());
			if ( dia_horariosAula_comuns[itRemover->getDia()].size() == 0 )
				dia_horariosAula_comuns.erase( itRemover->getDia() );
		}
		if ( dia_horariosAula_comuns.size() == 0 )
		{
			std::cout<<"\nErro em SolverMIP::criaVariavelTaticoCreditosAtravesDePreSol(...): "
				<< "sem intersecao entre os horarios comuns dos alunos e os horarios comuns da disc-sala."
				<< "\nDisc " << disciplina->getId() << " Turma " << turma << " Sala "<< salaId;
			continue;	
		}
		// -------------------------------------------------------------

		bool debug=false;

		it_Dia_HorarioAula = dia_horariosAula_comuns.begin();					
		for ( ; it_Dia_HorarioAula != dia_horariosAula_comuns.end(); it_Dia_HorarioAula++ )
		{
			int dia = it_Dia_HorarioAula->first;  
				
			ITERA_GGROUP_LESSPTR( itHorarioI, it_Dia_HorarioAula->second, HorarioAula )
			{
				HorarioAula *hi = *itHorarioI;

				ITERA_GGROUP_INIC_LESSPTR( itHorarioF, itHorarioI, it_Dia_HorarioAula->second, HorarioAula )
				{
					HorarioAula *hf = *itHorarioF;
								 
					if ( debug )
						std::cout<<"\n\tDEBUG  testando: dia " << dia << " hi " << hi->getId() << " hf " << hf->getId();
 
					if ( ! disciplina->inicioTerminoValidos( hi, hf, dia, it_Dia_HorarioAula->second ) )
						continue;								 						 

					if ( debug )
						std::cout<<"\n\t\tinicio-fim ok";

					VariableTatico v;
					v.reset();
					v.setType( VariableTatico::V_CREDITOS );

					v.setTurma( turma );            // i
					v.setDisciplina( disciplina );  // d
					v.setUnidade( unidade );		 // u
					v.setSubCjtSala( cjtSala );     // tps  
					v.setDia( dia );				 // t
					v.setHorarioAulaInicial( hi );	 // hi
					v.setHorarioAulaFinal( hf );	 // hf
					std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
					v.setDateTimeInicial(auxP.first);
					auxP = problemData->horarioAulaDateTime[hf->getId()];
					v.setDateTimeFinal(auxP.first);

					if ( vHashTatico.find( v ) == vHashTatico.end() )
					{				
						if ( P==1 && r>1 )
						if ( !criaVariavelTatico_Anterior( &v ) )
							continue;

						double lowerBound = 0.0;
						double upperBound = 1.0;

						if ( ( P > 1 && FIXAR_TATICO_P1 ) ||
								( P==1 && r > 1 ) )
						{
							bool found = false;
							double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
							if ( found ) // fixa!
							{
								lowerBound = value;
								upperBound = lowerBound;
							}
						}
																		
						// -----------------
						// Solução Inicial
						if ( problemData->existeSolTaticoInicial() )
						{
							bool fixar = false;
							bool criar = problemData->getSolTaticoInicial()->criar( v, fixar );
							if ( !criar ) continue;
							if ( fixar ) lowerBound = upperBound;
						}
						// -----------------
									
							    
						if ( debug )
							std::cout<<"... criando!";

						vHashTatico[ v ] = lp->getNumCols();
									
						OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound, ( char * )v.toString().c_str());

						lp->newCol( col );
						numVars++;

						#ifdef TATICO_COM_HORARIOS_HEURISTICO // Resolução do Modelo Tático com Horários por Heurística
						solverTaticoHeur->addVariable(v);
						#endif // TATICO_COM_HORARIOS_HEURISTICO

					}
				}
			}
		}
	}         

	return numVars;
}


// x_{i,d,u,s,hi,hf,t}
int SolverMIP::criaVariavelTaticoCreditos( int campusId, int P, int r, int tatico )
{
	int numVars = 0;
	Disciplina * disciplina = NULL;

    ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
    {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

       ITERA_GGROUP_LESSPTR( itDisciplina, problemData->disciplinas, Disciplina )
       {
            disciplina = ( *itDisciplina );	
			
			if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
				continue;
        
			map< int, GGroup< HorarioDia*, LessPtr<HorarioDia> > > turma_horariosDiaComunsAosAlunos;
			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
            {
				Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio;
				trio.set( campusId, turma, disciplina->getId() );
								
				if ( tatico==2 && CRIA_NOVAS_VARIAVEIS_TAT_HOR2 )
					turma_horariosDiaComunsAosAlunos[turma] = problemData->retornaHorariosDiaTurnoIESUniao(trio);
				else
					turma_horariosDiaComunsAosAlunos[turma] = problemData->retornaHorariosDiaTurnoIESIntersecao(trio);
					
				if ( problemData->existeTurmaDiscCampus(turma, disciplina->getId(), campusId) &&
					 turma_horariosDiaComunsAosAlunos[turma].size()==0 )
					std::cout<<"\nErro em SolverMIP::criaVariavelTaticoCreditos( int campusId, int P, int r, int tatico )"
					<< ", turma"<< turma <<" disc" << disciplina->getId() << " sem horarios em comum para todos os alunos\n";
			}

			if ( disciplina->cjtSalasAssociados.size() == 0 ) std::cout<<"\nATENCAO, disc"<<disciplina->getId()<<" sem sala!!";

			map< int, ConjuntoSala* >::iterator itCjtSala = disciplina->cjtSalasAssociados.begin();
			for ( ; itCjtSala != disciplina->cjtSalasAssociados.end(); itCjtSala++ )
			{
				ConjuntoSala *cjtSala = itCjtSala->second;
				
				if ( cjtSala->salas.size() > 1 )
				{
					std::cout<<"\nATENCAO em criaVariavelTaticoCreditos: conjunto sala deve ter somente 1 sala! \n";
				}

				int salaId = cjtSala->salas.begin()->first;
				Sala *sala = cjtSala->salas.begin()->second;

				Unidade *unidade = problemData->refUnidade[ sala->getIdUnidade() ];

			   std::pair< int, int > parDiscSala = std::make_pair( disciplina->getId(), salaId );

			   std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, 
						 std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > >::iterator

			   it_Disc_Sala_Dias_HorariosAula = problemData->disc_Salas_Dias_HorariosAula.find( parDiscSala );

			   if ( it_Disc_Sala_Dias_HorariosAula == 
				    problemData->disc_Salas_Dias_HorariosAula.end() )
			   {
				   if ( disciplina->getId() == 20187 && cjtSala->getId() == 106 )
					   std::cout<<"\nDEBUG: disc 17886 ===>  horarios vazios para disc com sala "<< parDiscSala.second;

				   continue;
			   }
			   			   
			   for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
				   	bool debug = false;
					if ( disciplina->getId() == 20187 && cjtSala->getId() == 106 && turma == 0)
					{
						//debug = true;
					}
				    if ( debug )
					   std::cout<<"\n\nDEBUG 1: i0,  disc 20187, cjtSalaId 106";

				    GGroup< HorarioDia*, LessPtr<HorarioDia> > horariosDiaComunsAosAlunos = turma_horariosDiaComunsAosAlunos[turma];

					std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > 
						dia_horariosAula_comuns = it_Disc_Sala_Dias_HorariosAula->second;

					// -------------------------------------------------------------
					// Faz a interseção entre os dois conjuntos de horarios comuns
					GGroup< HorarioDia *, LessPtr< HorarioDia > > remover;
					std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
						it_Dia_HorarioAula = dia_horariosAula_comuns.begin();					
					for ( ; it_Dia_HorarioAula != dia_horariosAula_comuns.end(); it_Dia_HorarioAula++ )
					{
						int dia = it_Dia_HorarioAula->first;
						ITERA_GGROUP_LESSPTR( itHorAula, it_Dia_HorarioAula->second, HorarioAula )
						{
							HorarioDia *hd = problemData->getHorarioDiaCorrespondente( *itHorAula, dia );
							if ( horariosDiaComunsAosAlunos.find( hd ) == horariosDiaComunsAosAlunos.end() )
							{
								remover.add( hd );
																	 
							    if ( debug )
								    std::cout<<"\n\tDEBUG  REMOVER: dia " << hd->getDia() << " ha " << hd->getHorarioAulaId();
							}
						}
					}
					ITERA_GGROUP_LESSPTR( itRemover, remover, HorarioDia )
					{
						dia_horariosAula_comuns[itRemover->getDia()].remove(itRemover->getHorarioAula());
						if ( dia_horariosAula_comuns[itRemover->getDia()].size() == 0 )
							dia_horariosAula_comuns.erase( itRemover->getDia() );
					}
					// -------------------------------------------------------------
										

					it_Dia_HorarioAula = dia_horariosAula_comuns.begin();					
					for ( ; it_Dia_HorarioAula != dia_horariosAula_comuns.end(); it_Dia_HorarioAula++ )
					{
						int dia = it_Dia_HorarioAula->first;  
				
						ITERA_GGROUP_LESSPTR( itHorarioI, it_Dia_HorarioAula->second, HorarioAula )
						{
							HorarioAula *hi = *itHorarioI;

							ITERA_GGROUP_INIC_LESSPTR( itHorarioF, itHorarioI, it_Dia_HorarioAula->second, HorarioAula )
							{
								 HorarioAula *hf = *itHorarioF;
								 
							    if ( debug )
									std::cout<<"\n\tDEBUG  testando: dia " << dia << " hi " 
									<< hi->getId() << " hf " << hf->getId();;
 
								 if ( ! disciplina->inicioTerminoValidos( hi, hf, dia, it_Dia_HorarioAula->second ) )
									 continue;								 						 

							    if ( debug )
								   std::cout<<"\n\t\tinicio-fim ok";

								 VariableTatico v;
								 v.reset();
								 v.setType( VariableTatico::V_CREDITOS );

								 v.setTurma( turma );            // i
								 v.setDisciplina( disciplina );  // d
								 v.setUnidade( unidade );		 // u
								 v.setSubCjtSala( cjtSala );     // tps  
								 v.setDia( dia );				 // t
								 v.setHorarioAulaInicial( hi );	 // hi
								 v.setHorarioAulaFinal( hf );	 // hf
								 std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
								 v.setDateTimeInicial(auxP.first);
								 auxP = problemData->horarioAulaDateTime[hf->getId()];
								 v.setDateTimeFinal(auxP.first);

								 if ( vHashTatico.find( v ) == vHashTatico.end() )
								 {			
									if ( !criaVariavelTatico( &v ) )
										continue;
							    
									if ( debug )
										std::cout<<"\n\t\t\tDEBUG 4: i0,  disc 20187, cjtSalaId 106";
									
									if ( P==1 && r>1 )
									if ( !criaVariavelTatico_Anterior( &v ) )
										continue;

									double lowerBound = 0.0;
									double upperBound = 1.0;

									if ( ( P > 1 && FIXAR_TATICO_P1 ) ||
										 ( P==1 && r > 1 ) )
									{
										bool found = false;
										double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
										if ( found ) // fixa!
										{
											lowerBound = value;
											upperBound = lowerBound;
										}
									}
																		
									// -----------------
									// Solução Inicial
									if ( problemData->existeSolTaticoInicial() )
									{
										bool fixar = false;
										bool criar = problemData->getSolTaticoInicial()->criar( v, fixar );
										if ( !criar ) continue;
										if ( fixar ) lowerBound = upperBound;
									}
									// -----------------
									
							    
									if ( debug )
										std::cout<<"... criando!";

									vHashTatico[ v ] = lp->getNumCols();
									
									OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
									   ( char * )v.toString().c_str());

									lp->newCol( col );
									numVars++;

                           #ifdef TATICO_COM_HORARIOS_HEURISTICO // Resolução do Modelo Tático com Horários por Heurística
                              solverTaticoHeur->addVariable(v);
                           #endif // TATICO_COM_HORARIOS_HEURISTICO

								 }
							}
						}
					}
               }
            }
        }
    }

	return numVars;
}

int SolverMIP::criaVariavelTaticoSalaTurma( int campusId, int P, int r, int tatico )
{
	int numVars = 0;
	Disciplina * disciplina = NULL;
   VariableTaticoHash::iterator vit = vHashTatico.begin();

   GGroup<Trio<Disciplina*,int,ConjuntoSala*> > auxVarsToCreate;

   while (vit != vHashTatico.end())
   {
      VariableTatico v = vit->first;

      if ( v.getType() == VariableTatico::V_CREDITOS )
      {
         Trio<Disciplina*,int,ConjuntoSala*> trio;

         trio.first = v.getDisciplina();
         trio.second = v.getTurma();
         trio.third = v.getSubCjtSala();

         auxVarsToCreate.add(trio);
      }

      vit++;
   }

   for (GGroup<Trio<Disciplina*,int,ConjuntoSala*> >::iterator itVars = auxVarsToCreate.begin();
      itVars != auxVarsToCreate.end();
      itVars++)
   {
      Trio<Disciplina*,int,ConjuntoSala*> trio = *itVars;

      VariableTatico v;
      v.reset();
      v.setType( VariableTatico::V_SALA_TURMA );

      v.setTurma( trio.second );            // i
      v.setDisciplina( trio.first );  // d
      v.setSubCjtSala( trio.third );     // tps 

      if ( vHashTatico.find( v ) == vHashTatico.end() )
      {								
         vHashTatico[ v ] = lp->getNumCols();

         double lowerBound = 0.0;
         double upperBound = 1.0;

         if ( ( P > 1 && FIXAR_TATICO_P1 ) ||
            ( P==1 && r > 1 ) )
         {
            bool found = false;
            double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
            if ( found ) // fixa!
            {
               lowerBound = value;
               upperBound = lowerBound;
            }
         }

         OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
            ( char * )v.toString().c_str());

         lp->newCol( col );
         numVars++;
      }
   }

	return numVars;
}
 
   
// z_{i,d,cp}
int SolverMIP::criaVariavelTaticoAbertura( int campusId, int P, int r )
{
	int numVars = 0;
   
   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
		   continue;
	   }
	     
	  GGroup< int > disciplinas = problemData->cp_discs[ campusId ];

      ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
      {
		  Disciplina * disciplina = problemData->refDisciplinas[ *it_disciplina ];

		 if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			continue;

         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_ABERTURA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( *it_campus );	    // cp

            std::pair< int, int > dc
               = std::make_pair( disciplina->getId(), it_campus->getId() );

            if ( problemData->demandas_campus.find( dc )
               == problemData->demandas_campus.end() )
            {
               problemData->demandas_campus[ dc ] = 0;
            }

            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
				if ( !criaVariavelTatico( &v ) )
					continue;

				lp->getNumCols();
				vHashTatico[v] = lp->getNumCols();

				double coef = 0.0;			                     
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( ( P > 1 && FIXAR_TATICO_P1 ) ||
					 ( P==1 && r > 1 ) )
				{
					bool found = false;
					double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
					if ( found ) // fixa!
					{
						lowerBound = value;
						upperBound = lowerBound;
					}
				}

				// -----------------
				// Solução Inicial
				if ( problemData->existeSolTaticoInicial() )
				{
					bool fixar = false;
					bool criar = problemData->getSolTaticoInicial()->criar( v, fixar );
					if ( !criar ) continue;
					if ( fixar ){ upperBound = 1.0; lowerBound = upperBound; }
				}
				// -----------------

			   OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

               lp->newCol( col );

               numVars++;
            }
         }
      }
   }
	
	return numVars;
}
   
/*   
// b_{i,d,c,cp}
int SolverMIP::criaVariavelTaticoCursoAlunos( int campusId )
{
	int numVars = 0;
   
	Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
   {
       Campus * pt_Campus = it_Oferta->campus;
       Curso * pt_Curso = it_Oferta->curso;

	   if ( pt_Campus->getId() != campusId )
	   {
		   continue;
	   }

       map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();
      for(; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end();
         it_Prd_Disc++ )
      {
		  disciplina = it_Prd_Disc->first;
		  
         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_ALOC_ALUNO );

            v.setTurma( turma );           // i
            v.setDisciplina( disciplina ); // d
            v.setCurso( pt_Curso );        // c
            v.setCampus( pt_Campus );	    // cp

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHashTatico[v] = lp->getNumCols();
				
			   double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -1.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 1.0;
				}

               OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );
               numVars++;
            }
         }
      }
   }
	
	return numVars;
}
   
*/

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var c_{i,d,t} 

%Desc 
indica se houve abertura de turma $i$ da disciplina $d$ em dias consecutivos.

%ObjCoef
\delta \cdot \sum\limits_{d \in D} 
\sum\limits_{i \in I_{d}} \sum\limits_{t \in T-{1}} c_{i,d,t}

%Data \delta
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/
int SolverMIP::criaVariavelTaticoConsecutivos( int campusId, int P )
{
	int numVars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	  if ( it_campus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );
			   
			    if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
					continue;

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
                  GGroup< int > dias_letivos
                     = it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     VariableTatico v;
                     v.reset();
                     v.setType( VariableTatico::V_DIAS_CONSECUTIVOS );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );     // t
					 v.setCampus( *it_campus );		 // cp

                     if ( vHashTatico.find( v ) == vHashTatico.end() )
                     {
						if ( !criaVariavelTatico( &v ) )
							continue;

                        vHashTatico[v] = lp->getNumCols();
					    
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

						if ( P > 1 && FIXAR_TATICO_P1 )
						{
							bool found = false;
							double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
							if ( found ) // fixa!
							{
								lowerBound = value;
								upperBound = lowerBound;
							}
						}

						OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
						                           
						lp->newCol( col );

                        numVars++;
                     }
                  }
               }
            }
         }
      }
   }

	return numVars;
}

// c_{i,d,t,cp}
int SolverMIP::criaVariavelTaticoConsecutivosAPartirDeX( int campusId, int P )
{
	int numVars = 0;
	VariableTaticoHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTaticoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatico x = vit->first;

		if( x.getType() != VariableTatico::V_CREDITOS )
		{
			continue;
		}

		Campus *campus = problemData->refCampus[ x.getUnidade()->getIdCampus() ];
		Disciplina* disciplina = x.getDisciplina();
		int dia = x.getDia();
		int turma = x.getTurma();

		VariableTatico v;
		v.reset();
		v.setType( VariableTatico::V_DIAS_CONSECUTIVOS );
		v.setTurma( turma );			 // i
		v.setDisciplina( disciplina );   // d  
		v.setDia( dia );				 // t
		v.setCampus( campus );			 // cp	

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;
			if ( !criaVariavelTatico( &v ) )
			{
				continue;
			}
						
			int id = numVars;
			varHashAux[ v ] = id;
				
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
								
			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			varId_Bounds[ id ] = trio;
				
			numVars++;
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatico v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;

}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var m_{d,i,k} 

%Desc 
variável binária que indica se a combinação de divisão de créditos 
$k$ foi escolhida para a turma $i$ da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::criaVariavelTaticoCombinacaoDivisaoCredito( int campusId, int P )
{
   int numVars = 0;

   Disciplina * disciplina = NULL;
  
   GGroup<int> disciplinas = problemData->cp_discs[campusId];

   ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
   {
	   disciplina = problemData->refDisciplinas[ *it_disciplina ];

	  if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
		  continue;

      for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
      {
         for ( unsigned k = 0; k < disciplina->combinacao_divisao_creditos.size(); k++ )
         { 
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_COMBINACAO_DIVISAO_CREDITO );

            v.setTurma( turma );           // i
            v.setDisciplina( disciplina ); // d
            v.setK( k );	                // k

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHashTatico[ v ] = lp->getNumCols();

				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( P > 1 && FIXAR_TATICO_P1 )
				{
					bool found = false;
					double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
					if ( found ) // fixa!
					{
						lowerBound = value;
						upperBound = lowerBound;
					}
				}

               OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound, ( char * )v.toString().c_str() );

               lp->newCol( col );
               numVars++;
            }
         }
      }
   }

   return numVars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fkp_{d,i,t} 

%Desc 
variável de folga superior para a restrição de combinação de divisão de créditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fkp_{d,i,k}

%Data \psi
%Desc
peso associado a função objetivo.

%Var fkm_{d,i,t} 

%Desc 
variável de folga inferior para a restrição de combinação de divisão de créditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fkm_{d,i,k}

%Data \psi
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::criaVariavelTaticoFolgaCombinacaoDivisaoCredito( int campusId, int P )
{
   int numVars = 0;

   Disciplina * disciplina = NULL;
   Campus *campus = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
			continue;
	   }
      campus = *it_campus;
   }

   if ( campus == NULL )
      return 0;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
		disciplina = ( *it_disciplina );
	  
		// A disciplina deve ser ofertada no campus especificado
		if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 		{
			continue;
		}

		if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			continue;
		 
		for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
		{
			if ( ! problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId ) )
				continue;

			ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
			{
				int dia = *itDiasLetDisc;

				VariableTatico v;
				v.reset();
				v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );

				v.setTurma( turma );            // i
				v.setDisciplina( disciplina );  // d
				v.setDia( dia );	    // t

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					if ( criaVariavelTatico( &v ) )
					{
						vHashTatico[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
						coef = -2*campus->getCusto();
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
						coef = 2*campus->getCusto();
						}

						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10000.0,
						( char * )v.toString().c_str() );

						lp->newCol( col );

						numVars++;
					}
				}

				v.reset();
				v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );

				v.setTurma( turma );            // i
				v.setDisciplina( disciplina );  // d
				v.setDia( dia );	    // t

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					if ( criaVariavelTatico( &v ) )
					{
						vHashTatico[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
						coef = -2*campus->getCusto();
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
						coef = 2*campus->getCusto();
						}

						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10000.0,
						( char * )v.toString().c_str() );

						lp->newCol( col );

						numVars++;
					}
				}
			}
		}
   }
            

   /*
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );
			   
			   if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
					continue;

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
					if ( ! problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId ) )
						continue;

                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
                  GGroup< int > dias_letivos =
                     it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     VariableTatico v;
                     v.reset();
                     v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );	    // t

                     if ( vHashTatico.find( v ) == vHashTatico.end() )
                     {
						if ( criaVariavelTatico( &v ) )
						{
							vHashTatico[v] = lp->getNumCols();

							double coef = 0.0;

							if ( problemData->parametros->funcao_objetivo == 0 )
							{
								coef = -2*it_campus->getCusto();
							}
							else if ( problemData->parametros->funcao_objetivo == 1 )
							{
								coef = 2*it_campus->getCusto();
							}
                           
							OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10000.0,
								  ( char * )v.toString().c_str() );

							lp->newCol( col );

							numVars++;
						}
                     }

                     v.reset();
                     v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );	    // t

                     if ( vHashTatico.find( v ) == vHashTatico.end() )
                     {
						if ( criaVariavelTatico( &v ) )
						{
							vHashTatico[v] = lp->getNumCols();

							double coef = 0.0;

							if ( problemData->parametros->funcao_objetivo == 0 )
							{
								coef = -2*it_campus->getCusto();
							}
							else if ( problemData->parametros->funcao_objetivo == 1 )
							{
								coef = 2*it_campus->getCusto();
							}
                           
							OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10000.0,
								  ( char * )v.toString().c_str() );
						                           
							lp->newCol( col );

							numVars++;
						}
                     }
                  }
               }
            }
         }
      }
   }*/

   return numVars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fcp_{d,t}

%Desc 
variável de folga superior para a restrição de fixação da distribuição de créditos por dia.
%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcp_{d,t}

%Data \xi
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::criaVariavelTaticoFolgaDistCredDiaSuperior( int campusId, int P )
{
   int numVars = 0;

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
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				// A disciplina deve ser ofertada no campus especificado
				if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
					 problemData->cp_discs[campusId].end() )
				{
					continue;
				}

			   if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
					continue;

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
                  GGroup< int > dias_letivos
                     = itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId() 
                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) )
                        {
                           VariableTatico v;
                           v.reset();
                           v.setType( VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setDia( *itDiasLetDisc );
                           v.setSubCjtSala( *itCjtSala );

                           if ( vHashTatico.find( v ) == vHashTatico.end() )
                           {
							    if ( !criaVariavelTatico( &v ) )
									continue;

                                vHashTatico[v] = lp->getNumCols();
                                int cred_disc_dia = it_fix->disciplina->getMaxCreds();

								double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -itCampus->getCusto()/2;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = itCampus->getCusto()/2;
								}
               
								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, cred_disc_dia,
										( char * )v.toString().c_str() );
			                 
								lp->newCol( col );

                                numVars++;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return numVars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fcm_{d,t}  

%Desc 
variável de folga inferior para a restrição de fixação da distribuição de créditos por dia.

%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcm_{d,t}

%DocEnd
/====================================================================*/

int SolverMIP::criaVariavelTaticoFolgaDistCredDiaInferior( int campusId, int P )
{
   int numVars = 0;

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
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				// A disciplina deve ser ofertada no campus especificado
				if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
					 problemData->cp_discs[campusId].end() )
				{
					continue;
				}

			   if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
				   continue;

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
                  GGroup< int > dias_letivos =
                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId() 
                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) )
                        {
                           VariableTatico v;
                           v.reset();
                           v.setType( VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setDia( *itDiasLetDisc );
                           v.setSubCjtSala( *itCjtSala );

                           if ( vHashTatico.find( v ) == vHashTatico.end() )
                           {
							    if ( !criaVariavelTatico( &v ) )
									continue;

                                vHashTatico[v] = lp->getNumCols();
                                int cred_disc_dia = it_fix->disciplina->getMinCreds();

							    double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -itCampus->getCusto()/2;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = itCampus->getCusto()/2;
								}               

								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, cred_disc_dia,
											( char* )v.toString().c_str() );

								lp->newCol( col );

                                numVars += 1;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return numVars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var zc_{d,t} 

%Desc 
indica se houve abertura da disciplina $d$ no dia $t$.

%DocEnd
/====================================================================*/
int SolverMIP::criaVariavelTaticoAberturaCompativel( int campusId, int P )
{
   int numVars = 0;

   Disciplina *disciplina = NULL;

   Campus *campus = problemData->refCampus[ campusId ];

   ITERA_GGROUP_N_PT( it_disciplina, problemData->cp_discs[campusId], int )
   {
	    disciplina = problemData->refDisciplinas[ *it_disciplina ];

		if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			 continue;

        ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
        {
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_ABERTURA_COMPATIVEL );

            v.setDisciplina( disciplina );  // d
            v.setDia( *itDiasLetDisc );     // t
			v.setCampus( campus );			// cp

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
				if ( !criaVariavelTatico( &v ) )
					continue;

                vHashTatico[ v ] = lp->getNumCols();

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

				if ( P > 1 && FIXAR_TATICO_P1 )
				{
					bool found = false;
					double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
					if ( found ) // fixa!
					{
						lowerBound = value;
					}
				}

                OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                ( char * )v.toString().c_str() );

                lp->newCol( col );
                numVars++;
            }
        }
   }

   return numVars;
}

// fd_{i,d,cp}
int SolverMIP::criaVariavelTaticoFolgaDemandaDisc( int campusId, int P, int r )
{
	int numVars = 0;
   
   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
		   continue;
	   }
	     
	  GGroup< int > disciplinas = problemData->cp_discs[ campusId ];

      ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
      {
		  Disciplina * disciplina = problemData->refDisciplinas[ *it_disciplina ];
		  
		 #pragma region Equivalencias
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		 {
			continue;
		 }
		 #pragma endregion
		 		 
		 if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			  continue;

         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_SLACK_DEMANDA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( *it_campus );	    // cp

            std::pair< int, int > dc
               = std::make_pair( disciplina->getId(), it_campus->getId() );

            if ( problemData->demandas_campus.find( dc )
               == problemData->demandas_campus.end() )
            {
               problemData->demandas_campus[ dc ] = 0;
            }

            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
			    if ( !criaVariavelTatico( &v ) )
					continue;

                lp->getNumCols();
                vHashTatico[v] = lp->getNumCols();

			    double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					// Numero de aluno alocados para a turma
					int nroAlunos = problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId );
					int receitaMedia = problemData->receitaMediaTurmaDiscCampus( turma, disciplina->getId(), campusId );

					coef = - 100 * nroAlunos * disciplina->getTotalCreditos() * receitaMedia;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 10 * disciplina->getTotalCreditos() * it_campus->getCusto();
			 	}
								
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( ( P > 1 && FIXAR_TATICO_P1 ) ||
					 ( P==1 && r > 1 ) )
				{
					bool found = false;
					double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
					if ( found ) // fixa!
					{
						lowerBound = value;
						upperBound = lowerBound;
					}
				}

			    OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

                lp->newCol( col );
				 
                numVars++;
            }
         }
      }
   }
	
	return numVars;
	
}

// y_{a,u,t}
int SolverMIP::criaVariavelTaticoAlunoUnidDia( int campusId, int P )
{
   int numVars = 0;
   
   Campus *cp = problemData->refCampus[campusId];
   
   ITERA_GGROUP_LESSPTR( itUnid, cp->unidades, Unidade )
   {
	   Unidade *unid = *itUnid;

	   ITERA_GGROUP_N_PT( itDia, unid->dias_letivos, int )
	   {
			int dia = *itDia;

		   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		   {
				Aluno *aluno = *itAluno;

				if ( !aluno->hasCampusId( campusId ) )
					continue;

				VariableTatico v;
				v.reset();
				v.setType( VariableTatico::V_ALUNO_UNID_DIA );

				v.setUnidade( unid ); // u
				v.setAluno( aluno );  // a
				v.setDia( dia );	  // t

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					if ( !criaVariavelTatico( &v ) )
						continue;

					vHashTatico[ v ] = lp->getNumCols();

					double coef = 0.0;

					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = -1.0;
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{
						coef = 1.0;
			 		}

					OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
					( char * )v.toString().c_str() );

					lp->newCol( col );
					numVars++;
				}
			}
	   }
   }

   return numVars;
}


// w_{a,t}
int SolverMIP::criaVariavelTaticoAlunoUnidadesDifDia( int campusId, int P )
{
   int numVars = 0;
   
   Campus *cp = problemData->refCampus[campusId];
   
   ITERA_GGROUP_N_PT( itDia, cp->diasLetivos, int )
   {
		int dia = *itDia;
		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			if ( !aluno->hasCampusId( campusId ) )
				continue;

			VariableTatico v;
			v.reset();
			v.setType( VariableTatico::V_ALUNO_VARIAS_UNID_DIA );

			v.setAluno( aluno );  // a
			v.setDia( dia );	  // t

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				if ( !criaVariavelTatico( &v ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();

				double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -100.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 100.0;
			 	}

				OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
				( char * )v.toString().c_str() );

				lp->newCol( col );
				numVars++;
			}
		}
   }

   return numVars;
}



// fu_{i1,d1,i2,d2,t,cp}
int SolverMIP::criaVariavelTaticoFolgaAlunoUnidDifDia( int campusId, int P )
{
	int numVars = 0;
	
	if ( ! problemData->parametros->minDeslocAlunoEntreUnidadesNoDia )
		return numVars;

	Campus *campus = problemData->refCampus[campusId];
    if ( campus->unidades.size() <= 1 )
	   return numVars;
	
	std::map< VariableTatico, int /*nroAlunosEmComum*/ > variaveisFu;

	std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >,
		LessPtr< Aluno > >::iterator itMapAl = problemData->mapAluno_CampusTurmaDisc.begin();
	for ( ; itMapAl != problemData->mapAluno_CampusTurmaDisc.end(); itMapAl++ )
	{
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > alocacoesAluno = itMapAl->second;
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itAlocacoes1 = alocacoesAluno.begin();
		for ( ; itAlocacoes1 != alocacoesAluno.end(); itAlocacoes1++ )
		{
			Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio1 = *itAlocacoes1;
			int campusId1 = trio1.first;
			int turma1 = trio1.second;
			Disciplina* disciplina1 = trio1.third;

			if ( campusId1 != campusId ) continue;

			 Unidade * u1 = this->retornaUnidadeDeAtendimento( turma1, disciplina1, campus );
			 if ( u1 == NULL ){
				 std::cout<<"\nErro, turma 1 sem unidade!";
				 continue;
			 }

			GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itAlocacoes2 = itAlocacoes1;
			itAlocacoes2++;
			for ( ; itAlocacoes2 != alocacoesAluno.end(); itAlocacoes2++ )
			{
				Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio2 = *itAlocacoes2;
				int campusId2 = trio2.first;
				int turma2 = trio2.second;
				Disciplina* disciplina2 = trio2.third;

				if ( campusId2 != campusId ) continue;
			 
				Unidade * u2 = this->retornaUnidadeDeAtendimento( turma2, disciplina2, campus );
				if ( u2 == NULL ){
					std::cout<<"\nErro, turma 2 sem unidade!";
					continue;
				}

				if ( u1 == u2 )
					continue;

				GGroup<int> dias = problemData->diasComunsEntreDisciplinas( disciplina1, disciplina2 );
				ITERA_GGROUP_N_PT( it_dias, dias, int )
				{
					VariableTatico v;
					v.reset();
					v.setType( VariableTatico::V_SLACK_ALUNO_VARIAS_UNID_DIA );

					v.setTurma1( turma1 );            // i1
					v.setDisciplina1( disciplina1 );  // d1
					v.setTurma2( turma2 );            // i2
					v.setDisciplina2( disciplina2 );  // d2
					v.setCampus( campus );		      // cp
					v.setDia( *it_dias );			  // t
					
					std::map< VariableTatico, int >::iterator itFu = variaveisFu.find(v);
					if ( itFu == variaveisFu.end() )
					{					
						variaveisFu[ v ] = 1;
					}
					else
					{
						itFu->second = itFu->second + 1;
					}
				}
			}
		}
	}

	std::map< VariableTatico, int >::iterator itFu = variaveisFu.begin();
	for ( ; itFu != variaveisFu.end(); itFu++ )
	{
		VariableTatico v = itFu->first;
		int nroAlunos = itFu->second;

		if ( vHashTatico.find(v) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();

			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
				coef = - 10 * nroAlunos;
			else if ( problemData->parametros->funcao_objetivo == 1 )
				coef = 10 * nroAlunos;

			double lowerBound = 0.0;
			double upperBound = 1.0;
      
			OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
			lp->newCol( col );
				 
			numVars++;
		}		
	}

	return numVars;

}


// ffd_{i1,-d,i2,d,cp}
int SolverMIP::criaVariavelTaticoFolgaFolgaDemandaPT( int campusId, int P )
{
	int numVars = 0;
     
    Campus* campus = problemData->refCampus[ campusId ];
	     
	GGroup< int > disciplinas = problemData->cp_discs.find( campusId )->second;

	ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
	{
		Disciplina * discPratica = problemData->refDisciplinas[ *it_disciplina ];

		#pragma region Equivalencias
		if ( ( problemData->mapDiscSubstituidaPor.find( discPratica ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( discPratica ) )
		{
			continue;
		}
		#pragma endregion

		if ( discPratica->getId() > 0 )
		{
			continue;
		}
		
		std::map< int, Disciplina * >::iterator itMapDisc =
			problemData->refDisciplinas.find( - discPratica->getId() );
				
		if ( itMapDisc == problemData->refDisciplinas.end() )
		{
			continue;
		}
		
		Disciplina *discTeorica = itMapDisc->second;

		if ( ! problemData->haDemandaDiscNoCampus( discPratica->getId(), campusId ) )
		{
			continue;
		}

		for ( int turma1 = 1; turma1 <= discPratica->getNumTurmas(); turma1++ )
		{
			for ( int turma2 = 1; turma2 <= discTeorica->getNumTurmas(); turma2++ )
			{		
				GGroup<Aluno*, LessPtr<Aluno>> alunos = problemData->alunosEmComum( turma1, discPratica, turma2, discTeorica, campus );
				
				if ( alunos.size() == 0 )
				{
					continue;
				}

				VariableTatico v;
				v.reset();
				v.setType( VariableTatico::V_SLACK_SLACKDEMANDA_PT );

				v.setTurma1( turma1 );            // i1
				v.setDisciplina1( discPratica );  // -d
				v.setTurma2( turma2 );            // i2
				v.setDisciplina2( discTeorica );  // d
				v.setCampus( campus );	     // cp

				if ( vHashTatico.find(v) == vHashTatico.end() )
				{
					if ( !criaVariavelTatico( &v ) )
						continue;

					lp->getNumCols();
					vHashTatico[v] = lp->getNumCols();

					double coef = 0.0;		
				
					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = - 100.0 * alunos.size() * 
							( discPratica->getTotalCreditos() + discTeorica->getTotalCreditos() ); // TODO
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{						
						coef = 500 * campus->getCusto() * alunos.size() * 
							( discPratica->getTotalCreditos() + discTeorica->getTotalCreditos() ); // TODO
			 		}

					double lowerBound = 0.0;
					double upperBound = 1.0;
                  
					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

					lp->newCol( col );
				 
					numVars++;
				}
			}
		}
   }
	
	return numVars;
	
}
  
// du_{a,t}
int SolverMIP::criaVariavelTaticoDiaUsadoPeloAluno( int campusId, int P )
{
   int numVars = 0;
   
   if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::INDIFERENTE )
   {
		return numVars;
   }

   Campus *cp = problemData->refCampus[campusId];

   ITERA_GGROUP_N_PT( itDia, cp->diasLetivos, int )
   {
	   int dia = *itDia;
	   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	   {
			Aluno *aluno = *itAluno;

			if ( !aluno->hasCampusId( campusId ) )
			{
				continue;
			}

			VariableTatico v;
			v.reset();
			v.setType( VariableTatico::V_ALUNO_DIA );
			v.setAluno( aluno );	// a
			v.setDia( dia );		// t

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				if ( !criaVariavelTatico( &v ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();

				double obj = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 ) // MAX
				{
					if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
					{
						obj = cp->getCusto()/4;
					}
					else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
					{
						obj = -cp->getCusto()/4;
					}
				}
				else if ( problemData->parametros->funcao_objetivo == 1 ) // MIN
				{
					if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
					{
						obj = -cp->getCusto()/4;
					}
					else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
					{
						obj = cp->getCusto()/4;
					}
				}
               
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( P > 1 && FIXAR_TATICO_P1 )
				{
					bool found = false;
					double value = fixaLimitesVariavelTaticoComHorAnterior( &v, found );
					if ( found ) // fixa!
					{
						lowerBound = value;
					}
				}

				OPT_COL col( OPT_COL::VAR_BINARY, obj, lowerBound, upperBound, ( char * )v.toString().c_str() );

				lp->newCol( col );
				numVars++;
			}
		}
   }

    return numVars;
}

// fad_{i,d,a,t,hi,hf}
// É criada a partir da variavel x. Logo, só deve ser chamada após esta ser toda criada.
int SolverMIP::criaVariavelTaticoDesalocaAlunoDiaHor( int campusId, int P, int tatico )
{
    int numVars = 0;

    if ( tatico<=1 ) return numVars;
	
    char name[ 100 ];
    VariableTatico v;
    VariableTaticoHash::iterator vit;
	
	VariableTaticoHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	vit = vHashTatico.begin();
	while( vit != vHashTatico.end() )
	{
		// x_{i,d,s,t,hi,hf}
		if( vit->first.getType() == VariableTatico::V_CREDITOS )
		{
			VariableTatico v = vit->first;
						
			int dia = v.getDia();
			HorarioAula *hi = v.getHorarioAulaInicial();
			HorarioAula *hf = v.getHorarioAulaFinal();
			Disciplina *disciplina = v.getDisciplina();
         DateTime *di =v.getDateTimeInicial();
         DateTime *df = v.getDateTimeFinal();
			int turma = v.getTurma();
			int campusId = v.getUnidade()->getIdCampus();
			Campus *campus = problemData->refCampus[campusId];
			
			Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
			trio.set( campusId, turma, disciplina );

			std::map< Trio< int, int, Disciplina* >, 
						GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator 
				itMap = problemData->mapSlackCampusTurmaDisc_AlunosDemanda.find( trio );
							
			if ( itMap != problemData->mapSlackCampusTurmaDisc_AlunosDemanda.end() )
			{
				ITERA_GGROUP_LESSPTR( itAlDem, itMap->second, AlunoDemanda )
				{
					Aluno *aluno = problemData->retornaAluno( (*itAlDem)->getAlunoId() );

					if ( itAlDem->getCampus()->getId() != campusId )
					{
						continue;
					}

					VariableTatico v;
					v.reset();
					v.setType( VariableTatico::V_DESALOCA_ALUNO_DIA );
					v.setAluno( aluno );			// a
					v.setTurma( turma );			// i
					v.setDisciplina( disciplina );	// d
					v.setDia( dia );				// t
					v.setHorarioAulaInicial( hi );	// hi
					v.setHorarioAulaFinal( hf );	// hf
					v.setDateTimeInicial(di);
					v.setDateTimeFinal(df);

					if ( varHashAux.find( v ) == varHashAux.end() )
					{												
						int id = numVars;
						varHashAux[ v ] = id;
				
						double coef = 0.0;					
						double lowerBound = 0.0;
						double upperBound = 1.0;

						Trio<double, double, double> trio2;
						trio2.set( coef,lowerBound, upperBound );

						varId_Bounds[ id ] = trio2;
				
						numVars++;	
					}
				}
			}
		}

		vit++;
	}
	
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{
		VariableTatico v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}
	
	return numVars;
}

// fa_{i,d,a}
int SolverMIP::criaVariavelTaticoDesalocaAluno( int campusId, int P, int r )
{
    int numVars = 0;
		
    if ( r<=1 ) return numVars;

	VariableTaticoHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator
		it = problemData->mapSlackAluno_CampusTurmaDisc.begin();

	for ( ; it != problemData->mapSlackAluno_CampusTurmaDisc.end(); it++ )
	{
		Aluno *aluno = (*it).first;

		if ( !aluno->hasCampusId( campusId ) )
		{
			continue;
		}

		GGroup< Trio< int, int, Disciplina* > > trios = (*it).second;

		GGroup< Trio< int, int, Disciplina* > >::iterator itGGroup = trios.begin();
		for ( ; itGGroup != trios.end(); itGGroup++ )
		{
			int turma = (*itGGroup).second;
			Disciplina *disciplina = (*itGGroup).third;
			Campus *campus = problemData->refCampus[ (*itGGroup).first ];

			if ( campus->getId() != campusId ) continue;

			VariableTatico v;
			v.reset();
			v.setType( VariableTatico::V_DESALOCA_ALUNO );
			v.setAluno( aluno );			// a
			v.setTurma( turma );			// i
			v.setDisciplina( disciplina );	// d
			v.setCampus( campus );			// cp

			if ( varHashAux.find( v ) == varHashAux.end() )
			{
				int id = numVars;
				varHashAux[ v ] = id;
				
				Demanda *demanda = aluno->getAlunoDemanda( disciplina->getId() )->demanda;

				double coef = 0.0;
				if ( problemData->parametros->funcao_objetivo == 0 ) // MAX
				{
					coef = -v.getDisciplina()->getTotalCreditos() * aluno->getOferta(demanda)->getReceita();
				}
				else if ( problemData->parametros->funcao_objetivo == 1 ) // MIN
				{
					coef = v.getDisciplina()->getTotalCreditos() * aluno->getOferta(demanda)->getReceita();
				}
				double lowerBound = 0.0;
				double upperBound = 1.0;

				Trio<double, double, double> trio2;
				trio2.set( coef,lowerBound, upperBound );

				varId_Bounds[ id ] = trio2;
				
				numVars++;
			}
		}
   }
		
	// Insere todas as variaveis em vHashTatico e no lp	
	VariableTaticoHash::iterator vit;
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{
		VariableTatico v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}	

   return numVars;
}

// f_{i,d,cp} -> indica se há alunos formandos na turma
int SolverMIP::criaVariavelTaticoFormandosNaTurma( int campusId, int P_ATUAL, int r, int tatico )
{
	int numVars = 0;
	
	if ( !problemData->parametros->violar_min_alunos_turmas_formandos )
		return numVars;

	if ( tatico!=2 )
		return numVars;

	if ( P_ATUAL==1 && r==1 ) // r1 não considera formandos
		return numVars;

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
		 		 
		 #pragma region Equivalencias
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		 {
		 	continue;
		 }
		 #pragma endregion
		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
			 // como o tático não permite inserção de aluno, nem cria variavel caso já não exista formando.
			 // se existir, cria livre já que é possível a desalocação de alunos para tatico=2.
			 if ( ! problemData->possuiAlunoFormando( turma, disciplina, cp ) )
				 continue;

			 VariableTatico v;
			 v.reset();
			 v.setType( VariableTatico::V_FORMANDOS_NA_TURMA );
			 v.setTurma( turma );            // i
			 v.setDisciplina( disciplina );  // d
			 v.setCampus( cp );				// cp

			 if ( vHashTatico.find(v) == vHashTatico.end() )
			 {
			    if ( !criaVariavelTatico( &v ) )
					continue;

                lp->getNumCols();
                vHashTatico[v] = lp->getNumCols();

			    double coef = 0.0;
						 
				double lowerBound = 0.0;
				double upperBound = 1.0;

				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				numVars++;
            }
         }
      }
   }

	return numVars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fp_{d,t,h} 

%Desc 
indica se houve folga no numero minimo necessario de professores 
para ministrar a disciplina $d$ no dia $t$ no horario $h$.

%DocEnd
/====================================================================*/
// fp_{d,t,h}
int SolverMIP::criaVariavelTaticoFolgaProfAPartirDeX( int campusId, int P )
{
	int numVars = 0;

	if ( ! problemData->parametros->considerar_disponibilidade_prof_em_tatico )
		return numVars;

	VariableTaticoHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTaticoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatico v = vit->first;

		if( v.getType() != VariableTatico::V_CREDITOS )
		{
			continue;
		}

		Disciplina* disciplina = v.getDisciplina();
		int dia = v.getDia();
		HorarioAula *hi = v.getHorarioAulaInicial();
		HorarioAula *hf = v.getHorarioAulaFinal();
		
		HorarioAula *ha = hi;

		bool end=false;
		while ( !end )
		{
			VariableTatico v;
			v.reset();
			v.setType( VariableTatico::V_FOLGA_HOR_PROF );
			v.setDisciplina( disciplina );   // d  
			v.setDia( dia );				 // t
			v.setHorarioAulaInicial( ha );	 // h
         std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[ha->getId()];
         v.setDateTimeInicial(auxP.first);

			if ( varHashAux.find( v ) == varHashAux.end() )
			{
				bool fixar=false;
				if ( !criaVariavelTatico( &v ) )
				{
					continue;
				}
						
				int id = numVars;
				varHashAux[ v ] = id;
				
				double coef = 0.0;
				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -50.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 50.0;
			 	}

				double lowerBound = 0.0;
				double upperBound = 50.0;
								
				Trio<double, double, double> trio;
				trio.set( coef,lowerBound, upperBound );

				varId_Bounds[ id ] = trio;
				
				numVars++;
			}

			if ( *ha == *hf ) end=true;
			else ha = ha->getCalendario()->getProximoHorario( ha );

			if ( ha==NULL )
			{
				std::cout<<"\n\nErro em SolverMIP::criaVariavelTaticoFolgaProfAPartirDeX! horario eh null antes de chegar em hf";
				std::cout<<"\nDisc"<<disciplina->getId()<<" dia"<<dia;
				end=true;
			}			
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatico v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;

}

// fmd_{a}
int SolverMIP::criaVariavelTaticoFolgaMinimoDemandaAtendPorAluno( int campusId, int P )
{
    int numVars = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return numVars;
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

		VariableTatico v;
		v.reset();
		v.setType( VariableTatico::V_FOLGA_ALUNO_MIN_ATEND1 );
		v.setAluno( aluno );
		
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef1, 0.0, ub1, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			numVars++;
		}	

		v.reset();
		v.setType( VariableTatico::V_FOLGA_ALUNO_MIN_ATEND2 );
		v.setAluno( aluno );
		
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef2, 0.0, ub2, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			numVars++;
		}		

		v.reset();
		v.setType( VariableTatico::V_FOLGA_ALUNO_MIN_ATEND3 );
		v.setAluno( aluno );
		
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef3, 0.0, ub3, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			numVars++;
		}	
	}

	return numVars;

}




/* ----------------------------------------------------------------------------------
	
							RESTRICOES TATICO POR ALUNO COM HORARIOS
 ---------------------------------------------------------------------------------- */


int SolverMIP::criaRestricoesTatico( int campusId, int prioridade, int r, int tatico )
{
	int restricoes = 0;

	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	timer.start();
	restricoes += criaRestricaoTaticoCargaHoraria( campusId, prioridade, r, tatico );				// Restricao 1.2.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.2\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += criaRestricaoTaticoUsoDeSalaParaCadaHorario( campusId, prioridade, r, tatico );				// Restricao 1.2.3
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.3\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( campusId, prioridade, r, tatico );				// Restricao 1.2.4
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.4\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoAtendeDemanda( campusId, prioridade, r, tatico );						// Restricao 1.2.5
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.5\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	/* // Ainda não está sendo usada, então tirei para agilizar a criação e solução do modelo
	timer.start();
	restricoes += criaRestricaoTaticoTurmaDiscDiasConsec( campusId, prioridade, r, tatico );				// Restricao 1.2.6
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.6\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	*/


	/*
	timer.start();
	restricoes += criaRestricaoTaticoLimitaAberturaTurmas( campusId, prioridade );			// Restricao 1.2.7
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.7\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	*/


	timer.start();
	//restricoes += criaRestricaoTaticoDivisaoCredito( campusId, prioridade, r, tatico );			// Restricao 1.2.8
	restricoes += criaRestricaoTaticoDivisaoCredito_hash( campusId, prioridade, r, tatico );			// Restricao 1.2.8
	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.8\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoCombinacaoDivisaoCredito( campusId, prioridade, r, tatico );			// Restricao 1.2.9
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.9\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
      

	timer.start();
	restricoes += criaRestricaoTaticoAtivacaoVarZC( campusId, prioridade, r, tatico );			// Restricao 1.2.10
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.10\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

   /*timer.start();
	restricoes += criaRestricaoTaticoSalaTurma( campusId, prioridade, r, tatico );			// Restricao 1.2.30
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.30\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

   timer.start();
	restricoes += criaRestricaoTaticoSalaUnicaTurma( campusId, prioridade, r, tatico );			// Restricao 1.2.30
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.31\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif*/


  	timer.start();
	restricoes += criaRestricaoTaticoDisciplinasIncompativeis( campusId, prioridade, r, tatico );			// Restricao 1.2.11
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.11\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	if ( tatico==1 )
	{
  		timer.start();
		restricoes += criaRestricaoTaticoAlunoHorario( campusId, prioridade, r, tatico );					// Restricao 1.2.12
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.2.12\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
	}
	else
	{
  		timer.start();
		restricoes += criaRestricaoTaticoDesalocaAlunoTurmaHorario( campusId, prioridade, r, tatico );			// Restricao 1.2.13.1
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.2.13.1\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
		
  		timer.start();
		restricoes += criaRestricaoTaticoDesalocaAlunoHorario( campusId, prioridade, r, tatico );			// Restricao 1.2.13.2
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.2.13.2\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
		
  		timer.start();
		restricoes += criaRestricaoTaticoSumDesalocaAlunoFolgaDemanda( campusId, prioridade, r, tatico );			// Restricao 1.2.13.3
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.2.13.3\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif

  		timer.start();
		restricoes += criaRestricaoTaticoSumDesalocaAluno( campusId, prioridade, r, tatico );			// Restricao 1.2.13.4
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.2.13.4\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
	

  		timer.start();
		restricoes += criaRestricaoTaticoGaranteMinAlunosTurma( campusId, prioridade, r, tatico );			// Restricao 1.2.13.5
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.2.13.5\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif


  		timer.start();
		restricoes += criaRestricaoTaticoDesalocaPT( campusId, prioridade, r, tatico );			// Restricao 1.2.13.6
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.2.13.6\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif

		
  		timer.start();
		restricoes += criaRestricaoTaticoFormandos( campusId, prioridade, r, tatico );			// Restricao 1.2.13.7
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.2.13.7\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
		#endif
			
	}

/*	// substituidas por criaRestricaoTaticoAlunoUnidDifDia()

  	timer.start();
	restricoes += criaRestricaoTaticoAtivaY( campusId );						// Restricao 1.2.13
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.13\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoAlunoUnidadesDifDia( campusId );			// Restricao 1.2.14
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.14\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
*/


  	timer.start();
	restricoes += criaRestricaoTaticoAlunoUnidDifDia( campusId, prioridade, r, tatico );			// Restricao 1.2.14
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.14\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
	/* // RESTRIÇÕES ERRADAS, FORAM SUBSTITUIDAS POR criaRestricaoTaticoMinDiasAluno e criaRestricaoTaticoMaxDiasAluno
	   // DELETAR
  	timer.start();
	restricoes += criaRestricaoTaticoMinCreds( campusId );			// Restricao 1.2.15
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.15\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoMaxCreds( campusId );			// Restricao 1.2.16
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.16\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	*/

  	timer.start();
	restricoes += criaRestricaoTaticoDiscPraticaTeorica( campusId, prioridade, r, tatico );	// Restricao 1.2.17
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoDiscPraticaTeorica: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoTaticoDiscPraticaTeorica1xN( campusId, prioridade, r, tatico );	// Restricao 1.2.17
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoDiscPraticaTeorica1xN: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoMinDiasAluno( campusId, prioridade, r, tatico );	// Restricao 1.2.18
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.18\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoMaxDiasAluno( campusId, prioridade, r, tatico );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.19\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
	restricoes += criaRestricaoTaticoConsideraHorariosProfs( campusId, prioridade, r, tatico );	// Restricao 1.2.20
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.20\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif  
	
	timer.start();
	restricoes += criaRestricaoTaticoDiscPTAulasContinuas( campusId, prioridade, r, tatico );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.21\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
		
	timer.start();
	restricoes += criaRestricaoTaticoAlocMinPercAluno( campusId, prioridade, r, tatico );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.22\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();

	return restricoes;

}

int SolverMIP::criaRestricaoTaticoCargaHoraria( int campusId, int prioridade, int r, int tatico )
{
	int restricoes = 0;

	int nnz;
	char name[ 100 ];

	VariableTatico v;
	ConstraintTatico c;

	VariableTaticoHash::iterator vit;
	ConstraintTaticoHash::iterator cit;

	Campus *campus = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatico::V_CREDITOS && vit->first.getType() != VariableTatico::V_ABERTURA)
		{
			vit++;
			continue;
		}

		VariableTatico v = vit->first;

		if(v.getType() == VariableTatico::V_CREDITOS)
		{
         if (v.getUnidade()->getIdCampus() != campusId)
         {
				vit++;
				continue;
			}

			HorarioAula *hi = v.getHorarioAulaInicial();
			HorarioAula *hf = v.getHorarioAulaFinal();
			Sala *sala = v.getSubCjtSala()->salas.begin()->second;
			Disciplina *disciplina = v.getDisciplina();
			int dia = v.getDia();
			int turma = v.getTurma();

			c.reset();
			c.setType( ConstraintTatico::C_CARGA_HORARIA );
			c.setCampus( campus );
			c.setTurma( turma );
			c.setDisciplina( disciplina );

			cit = cHashTatico.find(c);
			
			if(cit == cHashTatico.end())
			{
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
				OPT_ROW row( 100, OPT_ROW::EQUAL , 0 , name );

				//int NCH = v.getDisciplina()->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
				int NCH = problemData->retornaNroCreditos( hi, hf, sala, disciplina, dia );

				row.insert( vit->second, NCH );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
			else
			{
				//int NCH = v.getDisciplina()->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
				int NCH = problemData->retornaNroCreditos( hi, hf, sala, disciplina, dia );
								
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(NCH);
				//lp->chgCoef(cit->second, vit->second,  NCH);
			}
		}
		else
		{
			if(campus->getId() != v.getCampus()->getId())
			{
				vit++;
				continue;
			}

			c.reset();
			c.setType( ConstraintTatico::C_CARGA_HORARIA );
			c.setCampus( campus );
			c.setTurma( v.getTurma() );
			c.setDisciplina( v.getDisciplina() );

			cit = cHashTatico.find(c);
			if(cit == cHashTatico.end())
			{
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
				OPT_ROW row( 100, OPT_ROW::EQUAL , 0 , name );

				row.insert( vit->second, -( v.getDisciplina()->getCredPraticos() + v.getDisciplina()->getCredTeoricos() ) );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
			else
			{
            idxC.push_back(vit->second);
            idxR.push_back(cit->second);
            valC.push_back(-( v.getDisciplina()->getCredPraticos() + v.getDisciplina()->getCredTeoricos() ));
				//lp->chgCoef(cit->second, vit->second,  -( v.getDisciplina()->getCredPraticos() + v.getDisciplina()->getCredTeoricos() ));
			}
		}


		vit++;
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();

	return restricoes;
}

int SolverMIP::criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId, int prioridade, int r, int tatico )
{
	int restricoes = 0;

	int nnz;
	char name[ 100 ];

	VariableTatico v;
	ConstraintTatico c;

	VariableTaticoHash::iterator vit;
	ConstraintTaticoHash::iterator cit;

	Campus *campus = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatico::V_CREDITOS)
		{
			vit++;
			continue;
		}

		VariableTatico v = vit->first;

		if ( campus->unidades.find(v.getUnidade()) == campus->unidades.end())
		{
			vit++;
			continue;
		}

		Sala *sala = v.getSubCjtSala()->salas.begin()->second;

      //Elimina repetidos
      GGroup<DateTime*,LessPtr<DateTime> > horariosDifSala;

		ITERA_GGROUP( itHor, sala->horariosDia, HorarioDia )
		{
			HorarioAula* h = itHor->getHorarioAula();

			if ( itHor->getDia() != v.getDia() )					
				continue;

         std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[h->getId()];

         if (horariosDifSala.find(auxP.first) != horariosDifSala.end())
         {
            continue;
         }
         else
         {
            horariosDifSala.add(auxP.first);
         }

			DateTime fimF = v.getHorarioAulaFinal()->getInicio();
			fimF.addMinutes( v.getHorarioAulaFinal()->getTempoAula() );

			DateTime fimH = h->getInicio(); // controle
			fimH.addMinutes( h->getTempoAula() );
			
			if ( ( v.getHorarioAulaInicial()->getInicio() <= h->getInicio() ) && ( fimF >  h->getInicio() ) )
			{
				c.reset();
				c.setType( ConstraintTatico::C_SALA_HORARIO );
				c.setCampus( campus );
				c.setUnidade( v.getUnidade() );
				c.setSubCjtSala( v.getSubCjtSala() );
				c.setDia( v.getDia() );
				c.setHorarioAula( h );
            c.setDateTime(auxP.first);

				cit = cHashTatico.find(c);

				if(cit == cHashTatico.end())
				{
					sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS , 1 , name );

					row.insert( vit->second, 1.0);

					cHashTatico[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
				else
				{
               idxC.push_back(vit->second);
               idxR.push_back(cit->second);
               valC.push_back(1.0);
					//lp->chgCoef(cit->second, vit->second,  1.0);
				}
			}
		}

		vit++;
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();

	return restricoes;

}

int SolverMIP::criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId, int prioridade, int r, int tatico )
{
	int restricoes = 0;

	int nnz;
	char name[ 100 ];

	VariableTatico v;
	ConstraintTatico c;

	VariableTaticoHash::iterator vit;
	ConstraintTaticoHash::iterator cit;

	Campus *campus = NULL;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{
		VariableTatico v = vit->first;

		if (v.getType() != VariableTatico::V_CREDITOS)
		{
			continue;
		}
		if ( problemData->cp_discs[campusId].find( v.getDisciplina()->getId() ) ==
			 problemData->cp_discs[campusId].end() )
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintTatico::C_UNICO_ATEND_TURMA_DISC_DIA );
		c.setCampus( campus );
		c.setTurma( v.getTurma() );
		c.setDisciplina( v.getDisciplina() );
		c.setDia( v.getDia() );

		cit = cHashTatico.find(c);

		if(cit == cHashTatico.end())
		{
			sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS , 1.0 , name );

			row.insert( vit->second, 1.0);

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(1.0);
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

int SolverMIP::criaRestricaoTaticoAtendeDemanda( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   Campus* cp = problemData->refCampus[ campusId ];

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
       disciplina = ( *it_disciplina );

		#pragma region Equivalencias
		if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
		{
			continue;
		}
		#pragma endregion	

		if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			 continue;

      for ( int i = 1; i <= disciplina->getNumTurmas(); ++i )
      {
         c.reset();
         c.setType( ConstraintTatico::C_DEMANDA_DISC );
		 c.setCampus( cp );
         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 
         if ( cHashTatico.find( c ) != cHashTatico.end() )
         {
            continue;
         }

		 nnz = 2;
         OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0, name );

		 v.reset();
		 v.setType( VariableTatico::V_ABERTURA );
		 v.setTurma( i );
		 v.setDisciplina( disciplina );
		 v.setCampus( cp );

		 it_v = vHashTatico.find( v );
		 if( it_v != vHashTatico.end() )
		 {
			row.insert( it_v->second, 1.0 );
		 }

		 v.reset();
		 v.setType( VariableTatico::V_SLACK_DEMANDA );
		 v.setTurma( i );
		 v.setDisciplina( disciplina );
		 v.setCampus( cp );

		 it_v = vHashTatico.find( v );
		 if( it_v != vHashTatico.end() )
		 {
			row.insert( it_v->second, 1.0 );
		 }         

         if ( row.getnnz() != 0 )
         {
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Evitar alocação de turmas da mesma disciplina em campus diferentes
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{cp \in CP} z_{i,d,cp}  \leq  1  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/
/*
int SolverMIP::criaRestricaoTaticoEvitaTurmaDiscCampiDif()
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
       disciplina = ( *it_disciplina );

		#pragma region Equivalencias
		if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() ) &&
			 !problemData->ehSubstituta( disciplina ) )
		{
			continue;
		}
		#pragma endregion	

      for ( int i = 0; i < disciplina->getNumTurmas(); ++i )
      {
         c.reset();
         c.setType( ConstraintTatico::C_EVITA_TURMA_DISC_CAMP_D );

         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHashTatico.find( c ) != cHashTatico.end() )
         {
            continue;
         }

		 nnz = problemData->campi.size();
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
         {
            v.reset();
            v.setType( VariableTatico::V_ABERTURA );

            v.setTurma( i );
            v.setDisciplina( disciplina );

            v.setCampus( *it_campus );

            it_v = vHashTatico.find( v );
            if( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() != 0 )
         {
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}
*/



// Restricao 1.2.8
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Contabiliza se há turmas da mesma disciplina em dias consecutivos (*)
%Desc 

%MatExp

\begin{eqnarray}
c_{i,d,t}  \geq \sum\limits_{u \in U_{cp}} \sum\limits_{s \in S_{u}}(x_{i,d,u,s,hi,hf,t} - x_{i,d,u,s,hi,hf,t-1}) - 1  \nonumber \qquad 
\forall cp \in CP \quad
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall (t \geq 2) \in T
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::criaRestricaoTaticoTurmaDiscDiasConsec( int campusId, int prioridade, int r, int tatico )
{	
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

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

		  #pragma region Equivalencias
		  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		  {
			  continue;
		  }
		  #pragma endregion	

		  // A disciplina deve ser ofertada no campus especificado
		  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			   problemData->cp_discs[campusId].end() )
		  {
			  continue;
		  }

		  if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			  continue;
		 
		  for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
		  {
			 GGroup< int, std::less<int> >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();
			 
			 // Só cria as restrições a partir do segundo dia
			 // Já que a estrutura é ordenada, pula o primeiro.
			 if ( itDiasLetDisc != disciplina->diasLetivos.end() )
				itDiasLetDisc++;

			 for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
			 {
				 int dia = *itDiasLetDisc;

				c.reset();
				c.setType( ConstraintTatico::C_TURMA_DISC_DIAS_CONSEC );

				c.setCampus( *itCampus );
				c.setDisciplina( disciplina );
				c.setTurma( turma );
				c.setDia( dia );

				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
				   continue;
				}

				nnz = ( problemData->totalSalas * 2 + 1 );
				OPT_ROW row( nnz, OPT_ROW::GREATER , -1.0 , name );

				v.reset();
				v.setType( VariableTatico::V_DIAS_CONSECUTIVOS );
				v.setTurma( turma );
				v.setDisciplina( disciplina );
				v.setDia( dia );
				v.setCampus( *itCampus );	

				it_v = vHashTatico.find( v );
				if ( it_v != vHashTatico.end() )
				{
				   row.insert( it_v->second, 1.0 );
				}

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{
						int salaId = itCjtSala->salas.begin()->first;

						GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
							problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );

						ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
						{
							HorarioAula *hi = *itHorario;

							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
							{
								HorarioAula *hf = *itHorario;

								if ( *hf < *hi )
								{
							 		continue;
								}

								v.reset();
								v.setType( VariableTatico::V_CREDITOS );

								v.setTurma( turma );
								v.setDisciplina( disciplina );
								v.setUnidade( *itUnidade );
								v.setSubCjtSala( *itCjtSala );
								v.setHorarioAulaInicial( hi );
								v.setHorarioAulaFinal( hf );
								v.setDia( dia );

								it_v = vHashTatico.find( v );
								if ( it_v != vHashTatico.end() )
								{
									row.insert( it_v->second, -1.0 );
								}

								v.setDia( dia - 1 );

								it_v = vHashTatico.find( v );
								if ( it_v != vHashTatico.end() )
								{
									row.insert( it_v->second, -1.0 );
								}
							}
						}
					}
				}

				if ( row.getnnz() != 0 )
				{
				   cHashTatico[ c ] = lp->getNumRows();

				   lp->addRow( row );
				   restricoes++;
				}
			 }
		  }
	   }
	}
 
	return restricoes;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Não permitir que alunos de cursos diferentes incompatíveis compartilhem turmas (*)
%Desc 

%MatExp

\begin{eqnarray} 
b_{i,d,c,cp} + b_{i,d,c',cp} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall c,c' \notin CC \quad
\forall cp \in CP
\end{eqnarray}

%DocEnd
/====================================================================*/

/*
int SolverMIP::criaRestricaoTaticoCursosIncompat( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( it1Cursos, problemData->cursos, Curso )
		{
			Curso* c1 = *it1Cursos;
			
			if ( itCampus->cursos.find( c1 ) == itCampus->cursos.end() )
			{
				continue;
			}

			ITERA_GGROUP_INIC_LESSPTR( it2Cursos, it1Cursos, problemData->cursos, Curso )
			{
				Curso* c2 = *it2Cursos;
			    
				if ( itCampus->cursos.find( c2 ) == itCampus->cursos.end() )
				{
					continue;
				}
				if ( problemData->cursosCompativeis(c1, c2) )
				{
					continue;
				}

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					// A disciplina deve ser ofertada no campus especificado
					if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
						 problemData->cp_discs[campusId].end() )
					{
						continue;
					}

					#pragma region Equivalencias
					if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						   problemData->mapDiscSubstituidaPor.end() ) &&
						 !problemData->ehSubstituta( disciplina ) )
					{
						continue;
					}
					#pragma endregion	
										
					if ( !c1->possuiDisciplina(disciplina) || !c2->possuiDisciplina(disciplina) )
						continue;
					

					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintTatico::C_ALUNOS_CURSOS_INCOMP );
						c.setCampus( *itCampus );
						c.setParCursos( std::make_pair( c1, c2 ) );
						c.setDisciplina( disciplina );
						c.setTurma( turma );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHashTatico.find( c ) != cHashTatico.end() )
						{
							continue;
						}

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						v.reset();
						v.setType( VariableTatico::V_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c1 );
						v.setCampus( *itCampus );

						it_v = vHashTatico.find( v );
						if( it_v != vHashTatico.end() )
						{
							row.insert( it_v->second, 1 );
						}

						v.reset();
						v.setType( VariableTatico::V_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c2 );
						v.setCampus( *itCampus );

						it_v = vHashTatico.find( v );
						if( it_v != vHashTatico.end() )
						{
							row.insert(it_v->second, 1);
						}

						if ( row.getnnz() != 0 )
						{
							cHashTatico[ c ] = lp->getNumRows();
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
*/

// Restricao 1.2.11
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Não permitir que alunos de cursos diferentes (mesmo que compativeis) compartilhem turmas.
%Desc 

%MatExp

\begin{eqnarray} 
b_{i,d,c,cp} + b_{i,d,c',cp} - fc_{i,d,c,c',cp} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall c,c' C \quad
\forall cp \in CP
\end{eqnarray}

%DocEnd

/====================================================================*/
/*
int SolverMIP::criaRestricaoTaticoProibeCompartilhamento( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
	   return restricoes;
   }

   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

	   std::map< std::pair< Curso *, Curso * >, std::vector< int > >::iterator
               it_cursoComp_disc = problemData->cursosComp_disc.begin();

        for (; it_cursoComp_disc != problemData->cursosComp_disc.end(); it_cursoComp_disc++ )
        {
			Curso *c1 = it_cursoComp_disc->first.first;
			Curso *c2 = it_cursoComp_disc->first.second;

			if ( itCampus->cursos.find( c1 ) == itCampus->cursos.end() ||
				 itCampus->cursos.find( c2 ) == itCampus->cursos.end() )
			{
				continue;
			}

            std::vector< int >::iterator it_disc = it_cursoComp_disc->second.begin();

            for (; it_disc != it_cursoComp_disc->second.end(); ++it_disc )
            {
				int discId = *it_disc;
				Disciplina * disciplina = problemData->retornaDisciplina(discId);
				  
				if (disciplina == NULL) continue;

				#pragma region Equivalencias
				if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() ) &&
						!problemData->ehSubstituta( disciplina ) )
				{
					continue;
				}
				#pragma endregion

				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
					c.reset();
					c.setType( ConstraintTatico::C_PROIBE_COMPARTILHAMENTO );
					c.setCampus( *itCampus );
					c.setParCursos( std::make_pair( c1, c2 ) );
					c.setDisciplina( disciplina );
					c.setTurma( turma );

					sprintf( name, "%s", c.toString().c_str() ); 

					if ( cHash.find( c ) != cHash.end() )
					{
						continue;
					}

					nnz = 3;

					OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

					v.reset();
					v.setType( VariableTatico::V_ALOC_ALUNO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setCurso( c1 );
					v.setCampus( *itCampus );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1 );
					}

					v.reset();
					v.setType( VariableTatico::V_ALOC_ALUNO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setCurso( c2 );
					v.setCampus( *itCampus );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert(it_v->second, 1);
					}

					v.reset();
					v.setType( VariableTatico::V_SLACK_COMPARTILHAMENTO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setParCursos( std::make_pair( c1, c2 ) );
					v.setCampus( *itCampus );

					it_v = vHashTatico.find( v );
					if ( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -1 );
					}

					if ( row.getnnz() != 0 )
					{
						cHashTatico[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
			}
		}
   }

   return restricoes;
}
*/

// Restricao 1.2.12
int SolverMIP::criaRestricaoTaticoLimitaAberturaTurmas( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
	
   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;


   int MinAlunosPrat=1;
   int MinAlunosTeor=1;
   
   if ( problemData->parametros->min_alunos_abertura_turmas )
   {
	   MinAlunosTeor = problemData->parametros->min_alunos_abertura_turmas_value;
	   MinAlunosTeor = problemData->parametros->min_alunos_abertura_turmas_praticas_value;
   }

   if ( MinAlunosPrat <= 0 ) MinAlunosPrat=1;
   if ( MinAlunosTeor <= 0 ) MinAlunosTeor=1;


   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 // A disciplina deve ser ofertada no campus especificado
		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			  problemData->cp_discs[campusId].end() )
		 {
			 continue;
		 }
		
		 if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			 continue;
		 
		 #pragma region Equivalencias
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		 {
			continue;
		 }
		 #pragma endregion

         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintTatico::C_LIMITA_ABERTURA_TURMAS );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = ( itCampus->getTotalSalas() * 7 );

            OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0 , name );

            v.reset();
            v.setType( VariableTatico::V_CREDITOS );

            // Insere variaveis Credito (x) ---

            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
				   if ( itCjtSala->disciplinas_associadas.find( disciplina) ==
					    itCjtSala->disciplinas_associadas.end() )
				   {
					   continue;
				   }
				   
				   int salaId = itCjtSala->salas.begin()->first;
					
				   ITERA_GGROUP_N_PT ( it_Dia, itCjtSala->dias_letivos_disciplinas[disciplina], int )
				   {
						int dia = *it_Dia;

						GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
							problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );
					  
						ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
						{
							HorarioAula *hi = *itHorario;

							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
							{
									HorarioAula *hf = *itHorario;

									if ( hf < hi )
									{
							 			continue;
									}
																		
									v.setTurma( turma );
									v.setDisciplina( disciplina );
									v.setUnidade( *itUnidade );
									v.setSubCjtSala( *itCjtSala );
									v.setDia( dia );                   
									v.setHorarioAulaInicial( hi );	 // hi
									v.setHorarioAulaFinal( hf );	 // hf

									it_v = vHashTatico.find( v );
									if( it_v != vHashTatico.end() )
									{
										row.insert( it_v->second, -1.0 );
									}
							}
						}
					}                  
                }
            }


			int MinAlunos;
			if (disciplina->eLab()) 
				MinAlunos = MinAlunosPrat;
			else
				MinAlunos = MinAlunosTeor;


            // Insere variaveis Abertura (z) ---

            v.reset();
            v.setType( VariableTatico::V_ABERTURA );

            v.setCampus( *itCampus );
            v.setDisciplina( disciplina );
            v.setTurma( turma );

            it_v = vHashTatico.find( v );
            if( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, MinAlunos );
            }

            // Insere restrição no Hash ---

            if ( row.getnnz() != 0 )
            {
               cHashTatico[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

	return restricoes;
}


// Restricao 1.2.12
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Abertura sequencial de turmas
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U_{cp}} \sum\limits_{s \in S_{u}} \sum\limits_{t \in T} \sum\limits_{hi \in H} \sum\limits_{hf \in H} x_{i,d,u,s,hi,hf,t} 
\geq \sum\limits_{u \in U_{cp}} \sum\limits_{s \in S_{u}} \sum\limits_{t \in T} \sum\limits_{hi \in H} \sum\limits_{hf \in H} x_{i',d,u,s,hi,hf,t}
\nonumber \qquad 

\forall cp \in CP \quad
\forall d \in D \quad
\forall i,i' \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/
/*
int SolverMIP::criaRestricaoTaticoAbreTurmasEmSequencia( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
	  {
		  continue;
	  }
	  #pragma endregion	

      if ( disciplina->getNumTurmas() > 1 )
      {
         for ( int turma = 0; turma < ( disciplina->getNumTurmas() - 1 ); turma++ )
         {
            c.reset();
            c.setType( ConstraintTatico::C_ABRE_TURMAS_EM_SEQUENCIA );

			c.setCampus( problemData->refCampus[campusId] );
            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() ); 
            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = 9999;
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
						  int salaId = itCjtSala->salas.begin()->first;
                      
						  GGroup< int > disc_sala_dias = problemData->disc_Conjutno_Salas__Dias[ std::make_pair<int,int>
																		( disciplina->getId(), itCjtSala->getId() ) ];

						  ITERA_GGROUP_N_PT( itDia, disc_sala_dias, int )
						  {
								int dia = *itDia;

								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
									problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );

								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hi = *itHorario;

									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
									{
										HorarioAula *hf = *itHorario;

										if ( hf < hi )
										{
							 				continue;
										}

										v.reset();
										v.setType( VariableTatico::V_CREDITOS );

										v.setTurma( turma );
										v.setDisciplina( disciplina );
										v.setUnidade( *itUnidade );
										v.setSubCjtSala( *itCjtSala );
										v.setHorarioAulaInicial( hi );
										v.setHorarioAulaFinal( hf );
										v.setDia( dia );

										it_v = vHashTatico.find( v );
										if ( it_v != vHashTatico.end() )
										{
											row.insert( it_v->second, 1.0 );
										}

										v.reset();
										v.setType( VariableTatico::V_CREDITOS );

										int proxTurma = turma + 1;
										v.setTurma(proxTurma);

										v.setDisciplina( disciplina );
										v.setUnidade( *itUnidade );
										v.setSubCjtSala( *itCjtSala );
										v.setHorarioAulaInicial( hi );
										v.setHorarioAulaFinal( hf );
										v.setDia( dia );

										it_v = vHashTatico.find( v );
										if ( it_v != vHashTatico.end() )
										{
											row.insert( it_v->second, -1.0 );
										}
									}
								}
							}
						}
				  }
            }

            if ( row.getnnz() != 0 )
            {
               cHashTatico[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

*/

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Regra de divisão de créditos
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{s \in S_{u}} \sum\limits_{hi \in H} \sum\limits_{hf \in H} x_{i,d,u,s,hi,hf,t} = 
 \sum\limits_{k \in K_{d}}N_{d,k,t} \cdot m_{d,i,k} + fkp_{d,i,t} - fkm_{d,i,t} \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%Data N_{d,k,t}
%Desc
número de créditos determinados para a disciplina $d$ no dia $t$ na combinação de divisão de crédito $k$.

%DocEnd
/====================================================================*/
int SolverMIP::criaRestricaoTaticoDivisaoCredito_hash( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;
   
   VariableTaticoHash::iterator vit;
   vit = vHashTatico.begin();
   while(vit != vHashTatico.end())
   {
	   VariableTatico v = vit->first;

	   Disciplina *disciplina=NULL;
	   int turma;
	   GGroup<int> dias;
	   std::vector<double> coefs;

       if(vit->first.getType() == VariableTatico::V_CREDITOS)
       {
		   Calendario *calendario = v.getHorarioAulaInicial()->getCalendario();
		   int nCreds = calendario->retornaNroCreditosEntreHorarios( v.getHorarioAulaInicial(), v.getHorarioAulaFinal() );
		   coefs.push_back( nCreds );
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P)
       {         
		   coefs.push_back( -1.0 );		   
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M)
       {         
		   coefs.push_back( 1.0 );
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableTatico::V_COMBINACAO_DIVISAO_CREDITO)
       {		   
		   int k = v.getK();
		   disciplina = v.getDisciplina();
		   turma = v.getTurma();
		   ITERA_GGROUP_N_PT( itDia, disciplina->diasLetivos, int )
		   {  
			   int numCreditos = ( disciplina->combinacao_divisao_creditos[ k ] )[ *itDia - 2 ].second;  // N{d,k,t}
			   coefs.push_back( -numCreditos );
			   dias.add( *itDia );
		   }
       }
	   else
	   {
		   vit++;
		   continue;
	   }

	   int at=0;	      
	   ITERA_GGROUP_N_PT( itDia, dias, int )
	   {		    
		    int dia = *itDia;

		    ConstraintTatico c;
		    c.reset();
		    c.setType( ConstraintTatico::C_DIVISAO_CREDITO );
		    c.setDisciplina( disciplina );
		    c.setTurma( turma );
		    c.setDia( dia );
		    sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 

			ConstraintTaticoHash::iterator cit;
			cit = cHashTatico.find( c );

			if ( cit != cHashTatico.end() )
			{
				auxCoef.first = cit->second;
				auxCoef.second = vit->second;

				coeffList.push_back( auxCoef );
				coeffListVal.push_back( coefs[at] );
			}
			else
			{
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
				nnz = 100;

				OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );

				row.insert( vit->second, coefs[at] );
				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}

			at++;
	   }

       vit++;
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

int SolverMIP::criaRestricaoTaticoDivisaoCredito( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );
	  
	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
	  {
		  continue;
	  }
	  #pragma endregion	

	  if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
		  continue;
		 
	  if ( disciplina->divisao_creditos.size() != 0 )
      {
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
            {
				int dia = *itDiasLetDisc;

                c.reset();
                c.setType( ConstraintTatico::C_DIVISAO_CREDITO );

                c.setDisciplina( disciplina );
                c.setTurma( turma );
                c.setDia( dia );

                sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 

                if ( cHashTatico.find( c ) != cHashTatico.end() )
                {
                   continue;
                }

                nnz = ( problemData->totalSalas + ( (int)( disciplina->combinacao_divisao_creditos.size() ) * 2 ) );
                OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

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
						   int salaId = itCjtSala->salas.begin()->first;

							GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
								problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );

							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
							{
								HorarioAula *hi = *itHorario;

								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hf = *itHorario;

									if ( ! disciplina->inicioTerminoValidos( hi, hf, dia, horariosEmComum ) )
										continue;				
									
									v.reset();
									v.setType( VariableTatico::V_CREDITOS );

									v.setTurma( turma );
									v.setDisciplina( disciplina );
									v.setUnidade( *itUnidade );
									v.setSubCjtSala( *itCjtSala );
									v.setHorarioAulaInicial( hi );
									v.setHorarioAulaFinal( hf );
									v.setDia( *itDiasLetDisc );

									int nCreds = hi->getCalendario()->retornaNroCreditosEntreHorarios(hi,hf);
									
									it_v = vHashTatico.find( v );
									if ( it_v != vHashTatico.end() )
									{
									   row.insert( it_v->second, nCreds );
									}
								}
							}
                        }
                    }
                }

				for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
				{	
					v.reset();
					v.setType( VariableTatico::V_COMBINACAO_DIVISAO_CREDITO );

					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setK( k );
					
					// N{d,k,t}
					int numCreditos = ( disciplina->combinacao_divisao_creditos[ k ] )[ dia - 2 ].second;

					it_v = vHashTatico.find( v );
					if ( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -numCreditos );
					}

					v.reset();
					v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );

					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setDia( dia );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1.0 );
					}

					v.reset();
					v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );

					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setDia( dia );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -1.0 );
					}
				}

                if ( row.getnnz() != 0 )
                {
                   cHashTatico[ c ] = lp->getNumRows();

                   lp->addRow( row );
                   restricoes++;
                }
            }
         }
      }
   }

   return restricoes;
}


// Restricao 1.2.15
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Somente uma combinação de regra de divisão de créditos pode ser escolhida
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{k \in K_{d}} m_{d,i,k} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::criaRestricaoTaticoCombinacaoDivisaoCredito( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
	  {
		  continue;
	  }
	  #pragma endregion	

	  if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
		  continue;
		 

      for ( int i = 1; i <= disciplina->getNumTurmas(); i++ )
      {
         c.reset();
         c.setType( ConstraintTatico::C_COMBINACAO_DIVISAO_CREDITO );

         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 

         if ( cHashTatico.find( c ) != cHashTatico.end() )
         {
            continue;
         }

         nnz = (int)( disciplina->combinacao_divisao_creditos.size() );
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
         {
            v.reset();
            v.setType( VariableTatico::V_COMBINACAO_DIVISAO_CREDITO );

            v.setTurma( i );
            v.setDisciplina( disciplina );
            v.setK( k );

            it_v = vHashTatico.find( v );

            if ( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() != 0 )
         {
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoTaticoSalaTurma( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator vit = vHashTatico.begin();
   ConstraintTaticoHash::iterator cit;

   while (vit != vHashTatico.end())
   {
      v = vit->first;

      if ( v.getType() == VariableTatico::V_CREDITOS )
      {
         c.reset();
         c.setType( ConstraintTatico::C_SALA_TURMA );

         c.setDisciplina( v.getDisciplina() );
         c.setTurma( v.getTurma() );
         c.setSubCjtSala(v.getSubCjtSala());

         sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 

         cit = cHashTatico.find( c );
         if ( cit != cHashTatico.end() )
         {
            lp->chgCoef(cit->second,vit->second,1.0);
         }
         else
         {
            nnz = 100;
            OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );
            row.insert( vit->second, 1.0 );
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
      else if ( v.getType() == VariableTatico::V_SALA_TURMA )
      {
         c.reset();
         c.setType( ConstraintTatico::C_SALA_TURMA );

         c.setDisciplina( v.getDisciplina() );
         c.setTurma( v.getTurma() );
         c.setSubCjtSala(v.getSubCjtSala());

         sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 

         cit = cHashTatico.find( c );
         if ( cit != cHashTatico.end() )
         {
            lp->chgCoef(cit->second,vit->second,-7.0);
         }
         else
         {
            nnz = 100;
            OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );
            row.insert( vit->second, -7.0 );
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }

      vit++;
   }

   return restricoes;
}

int SolverMIP::criaRestricaoTaticoSalaUnicaTurma( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator vit = vHashTatico.begin();
   ConstraintTaticoHash::iterator cit;

   while (vit != vHashTatico.end())
   {
      v = vit->first;

      if ( v.getType() == VariableTatico::V_SALA_TURMA )
      {
         c.reset();
         c.setType( ConstraintTatico::C_SALA_UNICA_TURMA );

         c.setDisciplina( v.getDisciplina() );
         c.setTurma( v.getTurma() );

         sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 

         cit = cHashTatico.find( c );
         if ( cit != cHashTatico.end() )
         {
            lp->chgCoef(cit->second,vit->second,1.0);
         }
         else
         {
            nnz = 100;
            OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
            row.insert( vit->second, 1.0 );
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }

      vit++;
   }

   return restricoes;
}


// Restricao 1.2.16
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativação da variável zc
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{i \in I} \sum\limits_{u \in U} \sum\limits_{s \in S_{u}} \sum\limits_{hi \in H} \sum\limits_{hf \in H}
  x_{i,d,u,s,hi,hf,t} \leq zc_{d,t} \cdot N \nonumber \qquad 
\forall cp \in CP \quad
\forall d \in D \quad
\forall t \in T
\end{eqnarray}


%DocEnd
/====================================================================*/
int SolverMIP::criaRestricaoTaticoAtivacaoVarZC( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

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
	  
		  // A disciplina deve ser ofertada no campus especificado
		  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
				problemData->cp_discs[campusId].end() )
 		  {
			  continue;
		  }

		  #pragma region Equivalencias
		  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		  {
			   continue;
		  }
		  #pragma endregion

		  if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			   continue;
		 
		  GGroup< int >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();

		  for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
		  {
             int dia = *itDiasLetDisc;

			 c.reset();
			 c.setType( ConstraintTatico::C_VAR_ZC );

			 c.setCampus( *itCampus );
			 c.setDisciplina( disciplina );
			 c.setDia( dia );

			 sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 
			 if ( cHashTatico.find( c ) != cHashTatico.end() )
			 {
				continue;
			 }

			 nnz = 100;

			 OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );
 
			 ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
			 {
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
				{
					int salaId = itCjtSala->salas.begin()->first;

					GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
						problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );

					ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
					{
						HorarioAula *hi = *itHorario;

						ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
						{
							HorarioAula *hf = *itHorario;

							if ( *hf < *hi )
							{
							 	continue;
							}

							for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
							{
								v.reset();
								v.setType( VariableTatico::V_CREDITOS );

								v.setTurma( turma );
								v.setDisciplina( disciplina );
								v.setUnidade( *itUnidade );
								v.setSubCjtSala( *itCjtSala );
								v.setHorarioAulaInicial( hi );
								v.setHorarioAulaFinal( hf );
                        std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
                        v.setDateTimeInicial(auxP.first);
                        auxP = problemData->horarioAulaDateTime[hf->getId()];
                        v.setDateTimeFinal(auxP.first);
								v.setDia( dia );

								it_v = vHashTatico.find( v );
								if ( it_v != vHashTatico.end() )
								{
									row.insert( it_v->second, 1.0 );
								}
							}
						}
					}
				}
			 }

			 v.reset();
			 v.setType( VariableTatico::V_ABERTURA_COMPATIVEL );

			 v.setDisciplina( disciplina );
			 v.setDia( *itDiasLetDisc );
			 v.setCampus( *itCampus );

			 it_v = vHashTatico.find( v );
			 if ( it_v != vHashTatico.end() )
			 {
				 row.insert( it_v->second, - disciplina->getNumTurmas() );
			 }

			 if ( row.getnnz() != 0 )
			 {
				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			 }
		  }
	   }
   }

   return restricoes;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Disciplinas incompatíveis
%Desc

%MatExp
\begin{eqnarray}
zc_{d_1,t} + zc_{d_2,t} \leq 1 \nonumber \qquad 
(d_1, d_2),
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::criaRestricaoTaticoDisciplinasIncompativeis( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   Campus *campus = problemData->refCampus[campusId];
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );
	  
	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			 problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
	  {
		   continue;
	  }
	  #pragma endregion

	  if ( ! problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
			 continue;		

      ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
      {
         ITERA_GGROUP_N_PT( it_inc, disciplina->ids_disciplinas_incompativeis, int )
         {
			std::map< int, Disciplina* >::iterator it_Ref_Disc = problemData->refDisciplinas.find( *it_inc );
			if ( it_Ref_Disc == problemData->refDisciplinas.end() )
			{ 
				continue;
			}
			
			Disciplina * nova_disc = it_Ref_Disc->second;

			// A disciplina deve ser ofertada no campus especificado
			if ( problemData->cp_discs[campusId].find( nova_disc->getId() ) ==
				problemData->cp_discs[campusId].end() )
 			{
				continue;
			}

			#pragma region Equivalencias
			if ( ( problemData->mapDiscSubstituidaPor.find( nova_disc ) !=
				   problemData->mapDiscSubstituidaPor.end() ) &&
				  !problemData->ehSubstituta( nova_disc ) )
			{
				continue;
			}
			#pragma endregion

            c.reset();
            c.setType( ConstraintTatico::C_DISC_INCOMPATIVEIS );
			c.setParDisciplinas( std::make_pair( nova_disc, disciplina ) );
			c.setCampus( campus );
			c.setDia( *itDiasLetDisc );

            sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 

            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = 2;
            OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

            v.reset();
            v.setType( VariableTatico::V_ABERTURA_COMPATIVEL );

            v.setDisciplina( disciplina );
            v.setDia( *itDiasLetDisc );
			v.setCampus( campus );

            it_v = vHashTatico.find( v );
            if ( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, 1.0 );
            }

            v.setDisciplina( nova_disc );

            it_v = vHashTatico.find( v );
			if ( it_v != vHashTatico.end() )
            {
               row.insert(it_v->second, 1.0);
            }

            if ( row.getnnz() != 0 )
            {
               cHashTatico[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }

         }
      }
   }

   return restricoes;
}

/*
	Restricao 1.2.18

	Impede a alocação de aulas de um aluno em horários com sobreposição.
	Para cada dia t, horário h, e aluno al:

	sum[u] sum[s] sum[hi] sum[hf] x_{i,d,u,s,hi,hf,t} <= 1

	sendo al alocado em (i,d) e	(hi,hf) sobrepõe o inicio de h
*/
int SolverMIP::criaRestricaoTaticoAlunoHorario( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintTatico c;
   VariableTaticoHash::iterator vit;
   ConstraintTaticoHash::iterator cit;
   HorarioAula *aula = NULL;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;
   std::vector<DateTime*> dateTimeOrd;
   std::map<HorarioAula*,int> mapHorarioAulaOrd;

   for( GGroup<DateTime*,LessPtr<DateTime> >::iterator itDt = problemData->horariosInicioValidos.begin(); 
      itDt != problemData->horariosInicioValidos.end(); 
      itDt++)
   {
      dateTimeOrd.push_back(*itDt);
   }

   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
      Calendario * calendario = ( *it_calendario );

      ITERA_GGROUP_LESSPTR( it_horario_aula, calendario->horarios_aula, HorarioAula )
      {
         HorarioAula * horario_aula = ( *it_horario_aula );
         int idx = -1;

         for (int i=0; i < (int)dateTimeOrd.size(); i++)
         {
            if ( *(dateTimeOrd[i]) == horario_aula->getInicio() )
            {
               idx = i;
               break;
            }
         }

         if ( idx < 0 )
         {
            printf("ERRO INESPERADO\n");
            exit(1);
         }

         mapHorarioAulaOrd[horario_aula] = idx;
      }
   }


   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_CREDITOS )
		{
			continue;
		}
					
		int campusId = v.getUnidade()->getIdCampus();
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();		

		Trio< int, int, Disciplina* > trio;
		trio.set( campusId, turma, disc );
		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda;

		std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
			itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
		if ( itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
		{
			alunosDemanda = (*itMapAtend).second;
		}
		else
		{
			std::cout<<"\nErro, existe variavel x mas nao existe a turma no map!"; continue;
		}

      int idxIni = mapHorarioAulaOrd[v.getHorarioAulaInicial()];
      int idxFim = mapHorarioAulaOrd[v.getHorarioAulaFinal()];
	  
		DateTime dtf = v.getHorarioAulaFinal()->getFinal();
		while ( ( (idxFim + 1) < dateTimeOrd.size() ) &&
				(  dtf > *(dateTimeOrd[idxFim+1])	) )
		{
			idxFim++;
		}

		Sala *sala = v.getSubCjtSala()->salas.begin()->second;

      for (int hh = idxIni; hh <= idxFim; hh++)
      {
         DateTime *dateTime = dateTimeOrd[hh];
         //HorarioAula* hAula = problemData->horarios_aula_ordenados[hh];
		
			// Para cada aluno alocado na aula
			ITERA_GGROUP_LESSPTR( itAlunoDem, alunosDemanda, AlunoDemanda )
			{
				int alunoId = ( *itAlunoDem )->getAlunoId();
				Aluno *aluno = problemData->retornaAluno( alunoId );
					
				c.reset();
				c.setType( ConstraintTatico::C_ALUNO_HORARIO );
				c.setAluno( aluno );
				c.setDia( v.getDia() );
				//c.setHorarioAula( hAula );
            c.setDateTime(dateTime);

				cit = cHashTatico.find( c );

				if ( cit != cHashTatico.end() )
				{
					auxCoef.first = cit->second;
					auxCoef.second = vit->second;

					coeffList.push_back( auxCoef );
					coeffListVal.push_back( 1.0 );
				}
				else
				{
					sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
					nnz = 100;

					OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

					row.insert( vit->second, 1.0 );
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}   
			}
      }
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;

   /*int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintTatico c;
   VariableTaticoHash::iterator vit;
   ConstraintTaticoHash::iterator cit;

   std::map<Trio<int,Disciplina*,int>,std::list<VariableTaticoHash::iterator> > varsDiaDiscTurma;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_CREDITOS )
		{
			continue;
		}
					
		int campusId = v.getUnidade()->getIdCampus();
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();		

		Trio< int, int, Disciplina* > trio;
		trio.set( campusId, turma, disc );
		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda;

		std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
			itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
		if ( itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
		{
			alunosDemanda = (*itMapAtend).second;
		}
		else
		{
			std::cout<<"\nErro, existe variavel x mas nao existe a turma no map!"; continue;
		}

		Sala *sala = v.getSubCjtSala()->salas.begin()->second;
		ITERA_GGROUP_LESSPTRPTR( it_horario_dia, sala->horariosDiaMap[v.getDia()], HorarioDia )
		{
			HorarioDia * horario_dia = ( *it_horario_dia );

			int dia = horario_dia->getDia();

			//if ( v.getDia() != dia )
			//	continue;

			HorarioAula * horario_aula = horario_dia->getHorarioAula();
			DateTime inicio = horario_aula->getInicio();
		
			DateTime vInicio = v.getHorarioAulaInicial()->getInicio();
			HorarioAula *horarioAulaFim = v.getHorarioAulaFinal();
			DateTime vFim = horarioAulaFim->getFinal();

			if ( !( ( vInicio <= inicio ) && ( vFim > inicio ) ) )
			{
				continue;
			}       
			
			// Para cada aluno alocado na aula
			ITERA_GGROUP_LESSPTR( itAlunoDem, alunosDemanda, AlunoDemanda )
			{
				int alunoId = ( *itAlunoDem )->getAlunoId();
				Aluno *aluno = problemData->retornaAluno( alunoId );
					
				c.reset();
				c.setType( ConstraintTatico::C_ALUNO_HORARIO );
				c.setAluno( aluno );
				c.setDia( dia );
				c.setHorarioAula( horario_aula );

				cit = cHashTatico.find( c );

				if ( cit != cHashTatico.end() )
				{
					auxCoef.first = cit->second;
					auxCoef.second = vit->second;

					coeffList.push_back( auxCoef );
					coeffListVal.push_back( 1.0 );
				}
				else
				{
					sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
					nnz = 100;

					OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

					row.insert( vit->second, 1.0 );
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}   
			}			
       }
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;*/
}


/*
	Impede a alocação de aulas de um aluno em horários com sobreposição.
	Para cada dia t, horários (hi,hf), aluno al e par (i,d) contendo o aluno al
	não atendido no tático anterior:

	x_{i,d,u,s,hi,hf,t} >= fad_{i,d,al,t,hi,hf}

*/
int SolverMIP::criaRestricaoTaticoDesalocaAlunoTurmaHorario( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintTatico c;
   VariableTaticoHash::iterator vit;
   ConstraintTaticoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_CREDITOS )
		{
			continue;
		}

		int dia = v.getDia();	
		int campusId = v.getUnidade()->getIdCampus();
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		HorarioAula *hi = v.getHorarioAulaInicial();
		HorarioAula *hf = v.getHorarioAulaFinal();

         DateTime *di = v.getDateTimeInicial();
         DateTime *df = v.getDateTimeFinal();

		Trio< int, int, Disciplina* > trio;
		trio.set( campusId, turma, disc );

		// FILTRANDO: SOMENTE AS TURMAS NÃO ATENDIDAS

		std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator 
			itMapAtend = problemData->mapSlackCampusTurmaDisc_AlunosDemanda.find( trio );
		if ( itMapAtend == problemData->mapSlackCampusTurmaDisc_AlunosDemanda.end() )
		{
			continue;
		}

      						
		itMapAtend = problemData->mapSlackCampusTurmaDisc_AlunosDemanda.find( trio );
		if ( itMapAtend != problemData->mapSlackCampusTurmaDisc_AlunosDemanda.end() )
		{
			GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda = (*itMapAtend).second;

			// Para cada aluno alocado na aula, não atendido
			ITERA_GGROUP_LESSPTR( itAlunoDem, alunosDemanda, AlunoDemanda )
			{
				int alunoId = ( *itAlunoDem )->getAlunoId();
				Aluno *aluno = problemData->retornaAluno( alunoId );
					
				c.reset();
				c.setType( ConstraintTatico::C_DESALOCA_ALUNO_TURMA );
				c.setAluno( aluno );
				c.setDia( dia );
				c.setHorarioAulaInicial( hi );
				c.setHorarioAulaFinal( hf );
				c.setTurma( turma );
				c.setDisciplina( disc );

				cit = cHashTatico.find( c );

				if ( cit != cHashTatico.end() )
				{
					auxCoef.first = cit->second;
					auxCoef.second = vit->second;

					coeffList.push_back( auxCoef );
					coeffListVal.push_back( 1.0 );
				}
				else
				{
					sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
					nnz = 100;

					OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );
					
					int turno = problemData->getFaseDoDia( hi->getInicio() );
					bool horarioViavelParaOAluno = ( *itAlunoDem )->demanda->getCalendario()->possuiTurno(turno, dia);

					if ( ! horarioViavelParaOAluno )
						row.setSense( OPT_ROW::EQUAL );
									
					// insere x
					row.insert( vit->second, 1.0 );

					// insere folga fad_{i,d,a,t,hi,hf} para permitir desalocar o aluno
					VariableTatico v_fa;
					v_fa.reset();
					v_fa.setType( VariableTatico::V_DESALOCA_ALUNO_DIA );
					v_fa.setAluno( aluno );
					v_fa.setTurma( turma );
					v_fa.setDisciplina( disc );
					v_fa.setDia( dia );
					v_fa.setHorarioAulaInicial( hi );
					v_fa.setHorarioAulaFinal( hf );
					v_fa.setDateTimeInicial(di);
					v_fa.setDateTimeFinal(df);

					VariableTaticoHash::iterator it_v;
					it_v = vHashTatico.find( v_fa );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -1.0 ); // insere fad_{i,d,a,t,hi,hf}
					}
					else
						std::cout<<"\nErro! Algo estranho aqui, acho que a variavel fad deveria existir.\n";

					// Insere restrição
					cHashTatico[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}   
			} 
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Impede a alocação de aulas de um aluno em horários com sobreposição.
	Para cada dia t, horário h, e aluno al:

	sum[i]sum[d]sum[u]sum[s]sum[hi]sum[hf] x_{i,d,u,s,hi,hf,t} 
	- sum[i']sum[d']sum[hi]sum[hf] fad_{i',d',al,t,hi,hf} <= 1

	sendo (hi,hf) sobrepõe o inicio de h; (i,d) contendo o aluno al; e 
	(i',d') contendo o aluno al não atendido no tático anterior.
*/
int SolverMIP::criaRestricaoTaticoDesalocaAlunoHorario( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintTatico c;
   VariableTaticoHash::iterator vit;
   ConstraintTaticoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;
      
   std::vector<DateTime*> dateTimeOrd;
   std::map<HorarioAula*,int> mapHorarioAulaOrd;

   for( GGroup<DateTime*,LessPtr<DateTime> >::iterator itDt = problemData->horariosInicioValidos.begin(); 
      itDt != problemData->horariosInicioValidos.end(); 
      itDt++)
   {
      dateTimeOrd.push_back(*itDt);
   }

   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
      Calendario * calendario = ( *it_calendario );

      ITERA_GGROUP_LESSPTR( it_horario_aula, calendario->horarios_aula, HorarioAula )
      {
         HorarioAula * horario_aula = ( *it_horario_aula );
         int idx = -1;

         for (int i=0; i < (int)dateTimeOrd.size(); i++)
         {
            if ( *(dateTimeOrd[i]) == horario_aula->getInicio() )
            {
               idx = i;
               break;
            }
         }

         if ( idx < 0 )
         {
            printf("ERRO INESPERADO 2\n");
            exit(1);
         }

         mapHorarioAulaOrd[horario_aula] = idx;
      }
   }

   vit = vHashTatico.begin();
   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_CREDITOS )
		{
			continue;
		}
			
		int campusId = v.getUnidade()->getIdCampus();
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		int dia = v.getDia();
        DateTime *di = v.getDateTimeInicial();
        DateTime *df = v.getDateTimeFinal();
		HorarioAula *hi = v.getHorarioAulaInicial();		
		HorarioAula *hf = v.getHorarioAulaFinal();

		Trio< int, int, Disciplina* > trio;
		trio.set( campusId, turma, disc );
		        
		int idxIni = mapHorarioAulaOrd[hi];
		int idxFim = mapHorarioAulaOrd[hf];
	  
		DateTime dtf = v.getHorarioAulaFinal()->getFinal();
		while ( ( (idxFim + 1) < dateTimeOrd.size() ) &&
				(  dtf > *(dateTimeOrd[idxFim+1])	) )
		{
			idxFim++;
		}

		for (int hh = idxIni; hh <= idxFim; hh++)
		{
			DateTime *dateTime = dateTimeOrd[hh];
						
			std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, 
					  GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator itMapAtend;
						
			itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
			if ( itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
			{
				GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda = (*itMapAtend).second;

				// Para cada aluno alocado na aula (atendida ou não)
				ITERA_GGROUP_LESSPTR( itAlunoDem, alunosDemanda, AlunoDemanda )
				{
					int alunoId = ( *itAlunoDem )->getAlunoId();
					Aluno *aluno = problemData->retornaAluno( alunoId );
					
					if ( !problemData->possuiNaoAtend(aluno) )
						continue;

					c.reset();
					c.setType( ConstraintTatico::C_DESALOCA_ALUNO_HORARIO );
					c.setAluno( aluno );
					c.setDia( dia );
					c.setDateTime(dateTime);

					// variavel de folga fad_{i,d,a,t,hi,hf} para permitir desalocar o aluno
					VariableTatico v_fa;
					v_fa.reset();
					v_fa.setType( VariableTatico::V_DESALOCA_ALUNO_DIA );
					v_fa.setAluno( aluno );
					v_fa.setTurma( turma );
					v_fa.setDisciplina( disc );
					v_fa.setDia( dia );
					v_fa.setHorarioAulaInicial( hi );
					v_fa.setHorarioAulaFinal( hf );
					v_fa.setDateTimeInicial(di);
					v_fa.setDateTimeFinal(df);

					cit = cHashTatico.find( c );
					if ( cit != cHashTatico.end() )
					{
						// insere x
						auxCoef.first = cit->second;
						auxCoef.second = vit->second;
						
						lp->chgCoef(cit->second, vit->second, 1.0);

						// insere fad_{i,d,a,t,hi,hf}
						VariableTaticoHash::iterator it_v;
						it_v = vHashTatico.find( v_fa );
						if( it_v != vHashTatico.end() )
						{
							auxCoef.first = cit->second;
							auxCoef.second = it_v->second;

							lp->chgCoef(cit->second, it_v->second, -1.0);	
						}
					}
					else
					{
						sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
						nnz = 100;
						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
						
						// insere x
						row.insert( vit->second, 1.0 );

						// insere fad_{i,d,a,t,hi,hf}
						VariableTaticoHash::iterator it_v;
						it_v = vHashTatico.find( v_fa );
						if( it_v != vHashTatico.end() )
						{
							row.insert( it_v->second, -1.0 );
						}

						// Insere restrição
						cHashTatico[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}   
				} 
			}
       }
   }



   /*
   vit = vHashTatico.begin();
   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_CREDITOS )
		{
			continue;
		}
			
		int campusId = v.getUnidade()->getIdCampus();
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();

		Trio< int, int, Disciplina* > trio;
		trio.set( campusId, turma, disc );

		HorarioAula *hi = v.getHorarioAulaInicial();
		DateTime vInicio = hi->getInicio();
		HorarioAula *hf = v.getHorarioAulaFinal();
		DateTime vFim = hf->getFinal();

         DateTime *di = v.getDateTimeInicial();
         DateTime *df = v.getDateTimeFinal();

		Sala *sala = v.getSubCjtSala()->salas.begin()->second;
		ITERA_GGROUP_LESSPTRPTR( it_horario_dia, sala->horariosDia, HorarioDia )
		{
			HorarioDia * horario_dia = ( *it_horario_dia );

			int dia = horario_dia->getDia();

			if ( v.getDia() != dia )
				continue;

			HorarioAula * horario_aula = horario_dia->getHorarioAula();
			
			std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[horario_aula->getId()];
            DateTime *inicio = auxP.first;

			if ( !( ( vInicio <= *inicio ) && ( vFim > *inicio ) ) )
			{
				continue;
			}
      
			std::map< Trio< int, int, Disciplina* >, 
					  GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator itMapAtend;
						
			itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
			if ( itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
			{
				GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda = (*itMapAtend).second;

				// Para cada aluno alocado na aula (atendida ou não)
				ITERA_GGROUP_LESSPTR( itAlunoDem, alunosDemanda, AlunoDemanda )
				{
					int alunoId = ( *itAlunoDem )->getAlunoId();
					Aluno *aluno = problemData->retornaAluno( alunoId );
					
					if ( !problemData->possuiNaoAtend(aluno) )
						continue;

					c.reset();
					c.setType( ConstraintTatico::C_DESALOCA_ALUNO_HORARIO );
					c.setAluno( aluno );
					c.setDia( dia );
					c.setDateTime( inicio );
					//c.setHorarioAula( horario_aula );

					// variavel de folga fad_{i,d,a,t,hi,hf} para permitir desalocar o aluno
					VariableTatico v_fa;
					v_fa.reset();
					v_fa.setType( VariableTatico::V_DESALOCA_ALUNO_DIA );
					v_fa.setAluno( aluno );
					v_fa.setTurma( turma );
					v_fa.setDisciplina( disc );
					v_fa.setDia( dia );
					v_fa.setHorarioAulaInicial( hi );
					v_fa.setHorarioAulaFinal( hf );
					v_fa.setDateTimeInicial(di);
					v_fa.setDateTimeFinal(df);

					cit = cHashTatico.find( c );
					if ( cit != cHashTatico.end() )
					{
						// insere x
						auxCoef.first = cit->second;
						auxCoef.second = vit->second;
						
						// não está funcionando inserir depois, não sei pq
						//coeffList.push_back( auxCoef );
						//coeffListVal.push_back( 1.0 );

						lp->chgCoef(cit->second, vit->second, 1.0);

						// insere fad_{i,d,a,t,hi,hf}
						VariableTaticoHash::iterator it_v;
						it_v = vHashTatico.find( v_fa );
						if( it_v != vHashTatico.end() )
						{
							auxCoef.first = cit->second;
							auxCoef.second = it_v->second;

							lp->chgCoef(cit->second, it_v->second, -1.0);

							//coeffList.push_back( auxCoef );
							//coeffListVal.push_back( -1.0 );						
						}
					}
					else
					{
						sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
						nnz = 100;
						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
						
						// insere x
						row.insert( vit->second, 1.0 );

						// insere fad_{i,d,a,t,hi,hf}
						VariableTaticoHash::iterator it_v;
						it_v = vHashTatico.find( v_fa );
						if( it_v != vHashTatico.end() )
						{
							row.insert( it_v->second, -1.0 );
						}

						// Insere restrição
						cHashTatico[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}   
				} 
			}
       }
   }*/

  // chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Para turma i, disciplina d, campus cp

	sum[a] fa_{i,d,a} <= ( nAlunos_{i,d,cp} - 1 ) * ( 1-fd_{i,d,cp} )

*/
int SolverMIP::criaRestricaoTaticoSumDesalocaAlunoFolgaDemanda( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintTatico c;
   VariableTaticoHash::iterator vit;
   ConstraintTaticoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_DESALOCA_ALUNO &&
			 v.getType() != VariableTatico::V_SLACK_DEMANDA )
		{
			continue;
		}
		
		Campus *campus = v.getCampus();
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
									
		c.reset();
		c.setType( ConstraintTatico::C_SUM_DESALOCA_ALUNOS_FOLGA_DEMANDA );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setCampus( campus );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			double coef;

			// insere folga fa_{i,d,a} para permitir desalocar o aluno
			if ( v.getType() == VariableTatico::V_SLACK_DEMANDA )
			{
				int nAlunos = problemData->existeTurmaDiscCampus( turma, disc->getId(), campus->getId() );
				coef = nAlunos-1;
			}

			// insere fd
			if ( v.getType() == VariableTatico::V_DESALOCA_ALUNO )
				coef = 1.0;

			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			int nAlunos = problemData->existeTurmaDiscCampus( turma, disc->getId(), campus->getId() );

			sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
			nnz = nAlunos+1;

			OPT_ROW row( nnz, OPT_ROW::LESS , nAlunos - 1, name );
						
			// insere fd
			if ( v.getType() == VariableTatico::V_SLACK_DEMANDA )
				row.insert( vit->second, nAlunos-1 );

			// insere folga fa_{i,d,a} para permitir desalocar o aluno
			if ( v.getType() == VariableTatico::V_DESALOCA_ALUNO )
				row.insert( vit->second, 1.0 );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Para turma i, disciplina d, aluno a

	sum[hi]sum[hf]sum[t] fad_{i,d,a,t,hi,hf} <= 7*fa_{i,d,a,cp}

*/
int SolverMIP::criaRestricaoTaticoSumDesalocaAluno( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintTatico c;
   VariableTaticoHash::iterator vit;
   ConstraintTaticoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_DESALOCA_ALUNO &&
			 v.getType() != VariableTatico::V_DESALOCA_ALUNO_DIA )
		{
			continue;
		}
				
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		Aluno *aluno = v.getAluno();
		Campus *campus = NULL;

		double coef;

		if ( v.getType() == VariableTatico::V_DESALOCA_ALUNO_DIA ) // fad
		{
			campus = v.getAluno()->getAlunoDemanda( disc->getId() )->getCampus();		
			coef = 1.0;
		}
		else if ( v.getType() == VariableTatico::V_DESALOCA_ALUNO ) // folga fa_{i,d,a,cp} para permitir desalocar o aluno
		{
			campus = v.getCampus();
			coef = -7.0;
		}

		c.reset();
		c.setType( ConstraintTatico::C_SUM_DESALOCA_ALUNO );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setAluno( aluno );
		c.setCampus( campus );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{			
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;
			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			int nAlunos = problemData->existeTurmaDiscCampus( turma, disc->getId(), campus->getId() );

			sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
			nnz = nAlunos+1;

			OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Caso p==1 e r==1 (Não é permitido turmas violando min de alunos): 
	
		nAlunos_{i,d,cp} - sum[a] fa_{i,d,a} >= MinAlunos * z_{i,d,cp}	Para toda turma i, disciplina d, campus cp

	Caso p==1 e r==2 (É permitido turmas com formando violando min de alunos) e
	caso p==2 e r==1 (É permitido turmas com formando violando min de alunos) :

		nAlunos_{i,d,cp} - sum[a] fa_{i,d,a} >= MinAlunos * (z_{i,d,cp} - f_{i,d,cp})	Para toda turma i, disciplina d, campus cp

*/
int SolverMIP::criaRestricaoTaticoGaranteMinAlunosTurma( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   
   if ( !problemData->parametros->min_alunos_abertura_turmas )
   {
	   return restricoes;
   }

   int MinAlunosTeor = problemData->parametros->min_alunos_abertura_turmas_value;
   int MinAlunosPrat = problemData->parametros->min_alunos_abertura_turmas_praticas_value;
   
   if ( MinAlunosPrat <= 0 ) MinAlunosPrat=1;
   if ( MinAlunosTeor <= 0 ) MinAlunosTeor=1;


   int nnz;
   char name[ 200 ];

   ConstraintTatico c;
   VariableTaticoHash::iterator vit;
   ConstraintTaticoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_DESALOCA_ALUNO &&
			 v.getType() != VariableTatico::V_FORMANDOS_NA_TURMA &&
			 v.getType() != VariableTatico::V_ABERTURA )
		{
			continue;
		}

		Campus *campus = v.getCampus();
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		
		//if ( r==2 && problemData->parametros->violar_min_alunos_turmas_formandos )
		//if ( problemData->possuiAlunoFormando(turma,disc,campus) && fixada )
		//	continue;
		
		int MinAlunos;
		if (disc->eLab()) 
			MinAlunos = MinAlunosPrat;
		else
			MinAlunos = MinAlunosTeor;

		double coef=0;
		if ( v.getType() == VariableTatico::V_DESALOCA_ALUNO )
			coef = -1;
		else if ( v.getType() == VariableTatico::V_FORMANDOS_NA_TURMA )
			coef = MinAlunos;
		else if ( v.getType() == VariableTatico::V_ABERTURA )
			coef = -MinAlunos;

		c.reset();
		c.setType( ConstraintTatico::C_MIN_ALUNOS_TURMA );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setCampus( campus );

		cit = cHashTatico.find( c );
		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			int nAlunosNaTurma = problemData->existeTurmaDiscCampus( turma, disc->getId(), campus->getId() );

			sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
			nnz = nAlunosNaTurma + 2;

			OPT_ROW row( nnz, OPT_ROW::GREATER , -nAlunosNaTurma, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

int SolverMIP::criaRestricaoTaticoDesalocaPT( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintTatico c;
   VariableTaticoHash::iterator vit;
   ConstraintTaticoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatico v = vit->first;

		if ( v.getType() != VariableTatico::V_DESALOCA_ALUNO )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();

		int id_oposto = - disc->getId();

		if ( problemData->refDisciplinas.find(id_oposto) == problemData->refDisciplinas.end() )
			continue;

		Disciplina *discTeor;
		if ( id_oposto < 0 )
			discTeor = v.getDisciplina();
		else
			discTeor = problemData->refDisciplinas[id_oposto];

		double coef;
		if ( disc->getId() < 0 )
			coef = 1.0;
		else
			coef = -1.0;

		c.reset();
		c.setType( ConstraintTatico::C_DESALOCA_PT );		
		c.setDisciplina( discTeor );
		c.setAluno( aluno );

		cit = cHashTatico.find( c );
		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{	
			sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
			nnz = 2;

			OPT_ROW row( nnz, OPT_ROW::EQUAL, 0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Seta a variavel f_{i,d,cp}, que indica se uma turma possui aluno formando. Só está sendo necessária em rodada 2
	(só a partir de r=2 pode ser permitida a violação do min de alunos por turma caso haja formando) do tatico 2 (aonde pode
	haver desalocações de alunos).

	Restrição 1: 
	
		M * f_{i,d,cp} >= nFormandos_{i,d,cp} * z_{i,d,cp} - sum[a] fa_{i,d,a}		Para cada turma i, disc d, campus cp

	Restrição 2: 
	
		f_{i,d,cp} <= nFormandos_{i,d,cp} * z_{i,d,cp} - sum[a] fa_{i,d,a}		Para cada turma i, disc d, campus cp
*/
int SolverMIP::criaRestricaoTaticoFormandos( int campusId, int prioridade, int r, int tatico )
{
    int restricoes = 0;

	if ( !problemData->parametros->min_alunos_abertura_turmas )
		return restricoes;	
	if ( !problemData->parametros->violar_min_alunos_turmas_formandos )
		return restricoes;		

	if ( tatico!=2 ) // só no tático 2 é que existe restrição de formandos
		return restricoes;
	if ( prioridade==1 && r==1 ) // só considera formandos a partir da segunda rodada
		return restricoes;
	
    char name[ 100 ];

    ConstraintTatico c;
	ConstraintTaticoHash::iterator cit;
    VariableTatico v;
    VariableTaticoHash::iterator vit;

	vit = vHashTatico.begin();

	for ( ; vit != vHashTatico.end(); vit++ )
	{
		// fa_{i,d,a}
		if( vit->first.getType() == VariableTatico::V_DESALOCA_ALUNO )
		{			
			VariableTatico v = vit->first;

			Aluno *aluno = v.getAluno();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus *campus = v.getCampus();

			if ( !aluno->ehFormando() )
				continue;

			// -----------------------------------------
			// Constraint 1
		
			c.reset();
			c.setType( ConstraintTatico::C_FORMANDOS1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);

			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=20;
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
			
			c.reset();
			c.setType( ConstraintTatico::C_FORMANDOS2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);

			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=20;
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// f_{i,d,cp}
		else if( vit->first.getType() == VariableTatico::V_FORMANDOS_NA_TURMA )
		{			
			VariableTatico v = vit->first;

			Campus *campus = v.getCampus();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();

			// -----------------------------------------
			// Constraint 1

			double M = problemData->getNroDemandaPorFormandos( disciplina, campus, prioridade );

			c.reset();
			c.setType( ConstraintTatico::C_FORMANDOS1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, M);
			}
			else
			{
				int nnz=20;
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, M);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
						
			c.reset();
			c.setType( ConstraintTatico::C_FORMANDOS2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);

			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, 1.0);
			}
			else
			{
				int nnz=20;
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, 1.0);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}		
		}
		// z_{i,d,cp}
		else if( vit->first.getType() == VariableTatico::V_ABERTURA )
		{			
			VariableTatico v = vit->first;

			Campus *campus = v.getCampus();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();

			// -----------------------------------------
			// Constraint 1

			double nFormandos = problemData->getNroFormandos(turma,disciplina,campus);

			c.reset();
			c.setType( ConstraintTatico::C_FORMANDOS1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, -nFormandos);
			}
			else
			{
				int nnz=20;
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, -nFormandos);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
						
			c.reset();
			c.setType( ConstraintTatico::C_FORMANDOS2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);

			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, -nFormandos);
			}
			else
			{
				int nnz=20;
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, -nFormandos);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}		
		}	
	}
	
	return restricoes;
}

int SolverMIP::criaRestricaoTaticoAtivaY( int campusId, int prioridade, int r, int tatico )
{
	int restricoes = 0;
	int nnz;
	char name[ 100 ];

	//VariableTatico v;
	ConstraintTatico c;
	//VariableTaticoHash::iterator it_v;
	ConstraintTaticoHash::iterator cit;

	//Disciplina * disciplina = NULL;

	Campus *campus = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

	map< int, map< Disciplina*, set< Aluno *, LessPtr< Aluno > >, LessPtr< Disciplina > > > mapAlunos;

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;
		GGroup< Trio< int, int, Disciplina* > >::iterator itT = problemData->mapAluno_CampusTurmaDisc[ aluno ].begin();
		for(; itT != problemData->mapAluno_CampusTurmaDisc[ aluno ].end(); itT++)
		{
			Trio< int, int, Disciplina* > trio = *itT;
			if(trio.first != campusId)
				continue;

			mapAlunos[trio.second][trio.third].insert(aluno);
		}
	}


	VariableTaticoHash::iterator vit;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatico::V_CREDITOS && vit->first.getType() != VariableTatico::V_ALUNO_UNID_DIA)
		{
			vit++;
			continue;
		}

		VariableTatico v = vit->first;

		if(vit->first.getType() == VariableTatico::V_CREDITOS)
		{
			if(campus->unidades.find(v.getUnidade()) == campus->unidades.end())
			{
				vit++;
				continue;
			}

			set< Aluno *, LessPtr< Aluno > >::iterator itA = mapAlunos[v.getTurma()][v.getDisciplina()].begin();
			for(; itA != mapAlunos[v.getTurma()][v.getDisciplina()].end(); itA++)
			{
				Aluno *aluno = *itA;

				c.reset();
				c.setType( ConstraintTatico::C_ATIVA_Y );
				c.setCampus( campus );
				c.setAluno( aluno );
				c.setDia( v.getDia() );
				c.setUnidade( v.getUnidade() );

				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
					OPT_ROW row( 100, OPT_ROW::GREATER , 0 , name );

					row.insert( vit->second, -1.0 );

					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second, -1.0);
				}

			}
		}
		else
		{
			if(campus->unidades.find(v.getUnidade()) == campus->unidades.end())
			{
				vit++;
				continue;
			}

			c.reset();
			c.setType( ConstraintTatico::C_ATIVA_Y );
			c.setCampus( campus );
			c.setAluno( v.getAluno() );
			c.setDia( v.getDia() );
			c.setUnidade( v.getUnidade() );

			cit = cHashTatico.find(c);
			if(cit == cHashTatico.end())
			{
				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
				OPT_ROW row( 100, OPT_ROW::GREATER , 0 , name );

				row.insert( vit->second, v.getAluno()->demandas.size() );

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, v.getAluno()->demandas.size());
			}

		}

		vit++;
	}

	return restricoes;
}

   
int SolverMIP::criaRestricaoTaticoAlunoUnidadesDifDia( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
	    {
			continue;
	    }

		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
			{
				int dia = *itDia;

				// CONSTRAINT --------------------------------------

				c.reset();
				c.setType( ConstraintTatico::C_ALUNO_VARIAS_UNIDADES_DIA );

				c.setCampus( *itCampus );
				c.setAluno( aluno );
				c.setDia( dia );

				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = ( itCampus->unidades.size() + 1 );

				OPT_ROW row( nnz, OPT_ROW::GREATER , -1.0 , name );

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					// Insere variavel y_{a,u,t} -------------------------------------------------------								
					v.reset();
					v.setType( VariableTatico::V_ALUNO_UNID_DIA );

					v.setUnidade( *itUnidade ); // u
					v.setAluno( aluno );  // a
					v.setDia( dia );	  // t

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -1.0 );
					}
				}

				// Insere variavel w_{a,t} -------------------------------------------------------								
				v.reset();
				v.setType( VariableTatico::V_ALUNO_VARIAS_UNID_DIA );

				v.setAluno( aluno );  // a
				v.setDia( dia );	  // t

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, itCampus->unidades.size() );
				}

				// Insere restrição no Hash --------------------------------------------------------
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}
		}
   }

   return restricoes;
}   

int SolverMIP::criaRestricaoTaticoMinCreds( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::INDIFERENTE )
   {
		return restricoes;
   }

   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
	    {
			continue;
	    }

		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
			{
				int dia = *itDia;

				// CONSTRAINT --------------------------------------

				c.reset();
				c.setType( ConstraintTatico::C_MIN_CREDS_ALUNO );

				c.setCampus( *itCampus );
				c.setAluno( aluno );
				c.setDia( dia );

				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = ( aluno->demandas.size() * itCampus->horarios.size() * 2 );

				OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0 , name );

				// Insere variaveis Credito (x) ------------------------------------------------------

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{				   
						Sala *sala = itCjtSala->salas.begin()->second;
						
						ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
						{
							disciplina = ( *it_disciplina );
							
							#pragma region Equivalencias
							if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
								   problemData->mapDiscSubstituidaPor.end() ) &&
								 !problemData->ehSubstituta( disciplina ) )
							{
								continue;
							}
							#pragma endregion

							for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
							{
								// Para cada trio <campus, turma, disciplina> no qual o aluno está alocado
								Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
								trio.set( campusId, turma, disciplina );

								if ( problemData->mapAluno_CampusTurmaDisc[ aluno ].find( trio ) ==
									 problemData->mapAluno_CampusTurmaDisc[ aluno ].end() )
								{
									continue;
								}

								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
									problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
					
								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hi = *itHorario;

									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
									{
											HorarioAula *hf = *itHorario;

											if ( hf < hi )
											{
							 					continue;
											}	

											int NCH = hi->getCalendario()->retornaNroCreditosEntreHorarios(hi,hf);

											v.reset();
											v.setType( VariableTatico::V_CREDITOS );									
											v.setTurma( turma );
											v.setDisciplina( disciplina );
											v.setUnidade( *itUnidade );
											v.setSubCjtSala( *itCjtSala );
											v.setDia( dia );                   
											v.setHorarioAulaInicial( hi );	 // hi
											v.setHorarioAulaFinal( hf );	 // hf

											it_v = vHashTatico.find( v );
											if( it_v != vHashTatico.end() )
											{
												row.insert( it_v->second, -NCH );
											}
									}
								}
							}
						}
					}
				}

				// Insere variavel h_{a} -------------------------------------------------------								
				v.reset();
				v.setType( VariableTatico::V_MIN_CRED_SEMANA );

				v.setAluno( aluno );  // a
					
				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}

				// Insere restrição no Hash --------------------------------------------------------
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}  
		}
   }

   return restricoes;
}

int SolverMIP::criaRestricaoTaticoMaxCreds( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::INDIFERENTE )
   {
		return restricoes;
   }

   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
	    {
			continue;
	    }

		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
			{
				int dia = *itDia;

				// CONSTRAINT --------------------------------------

				c.reset();
				c.setType( ConstraintTatico::C_MAX_CREDS_ALUNO );

				c.setCampus( *itCampus );
				c.setAluno( aluno );
				c.setDia( dia );

				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = ( aluno->demandas.size() * itCampus->horarios.size() * 2 );

				OPT_ROW row( nnz + 1, OPT_ROW::GREATER , 0 , name );

				// Insere variaveis Credito (x) ------------------------------------------------------

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{				   
						Sala *sala = itCjtSala->salas.begin()->second;
						
						ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
						{
							disciplina = ( *it_disciplina );
							
							#pragma region Equivalencias
							if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
								   problemData->mapDiscSubstituidaPor.end() ) &&
								 !problemData->ehSubstituta( disciplina ) )
							{
								continue;
							}
							#pragma endregion

							for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
							{
								// Para cada trio <campus, turma, disciplina> no qual o aluno está alocado
								Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
								trio.set( campusId, turma, disciplina );

								if ( problemData->mapAluno_CampusTurmaDisc[ aluno ].find( trio ) ==
									 problemData->mapAluno_CampusTurmaDisc[ aluno ].end() )
								{
									continue;
								}

								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
									problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
					
								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hi = *itHorario;

									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
									{
											HorarioAula *hf = *itHorario;

											if ( hf < hi )
											{
							 					continue;
											}	

											int NCH = hi->getCalendario()->retornaNroCreditosEntreHorarios(hi,hf);

											v.reset();
											v.setType( VariableTatico::V_CREDITOS );									
											v.setTurma( turma );
											v.setDisciplina( disciplina );
											v.setUnidade( *itUnidade );
											v.setSubCjtSala( *itCjtSala );
											v.setDia( dia );                   
											v.setHorarioAulaInicial( hi );	 // hi
											v.setHorarioAulaFinal( hf );	 // hf

											it_v = vHashTatico.find( v );
											if( it_v != vHashTatico.end() )
											{
												row.insert( it_v->second, -NCH );
											}
									}
								}
							}
						}
					}
				}

				// Insere variavel h_{a} -------------------------------------------------------								
				v.reset();
				v.setType( VariableTatico::V_MAX_CRED_SEMANA );

				v.setAluno( aluno );  // a
					
				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}

				// Insere restrição no Hash --------------------------------------------------------
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}  
		}
   }

   return restricoes;
}


// fu_{i1,d1,i2,d2,t,cp}
int SolverMIP::criaRestricaoTaticoAlunoUnidDifDia( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
   
   if ( ! problemData->parametros->minDeslocAlunoEntreUnidadesNoDia )
		return restricoes;

   Campus *campus = problemData->refCampus[campusId];
   if ( campus->unidades.size() <= 1 )
	   return restricoes;

   int nnz;
   char name[ 200 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
		   continue;
	   }

	   Campus* campus = *it_campus;
	     
	  GGroup< int > disciplinas = problemData->cp_discs[ campusId ];

	  // Disciplina 1
      ITERA_GGROUP_N_PT( it_disc1, disciplinas, int )
      {
		  Disciplina * disciplina1 = problemData->refDisciplinas[ *it_disc1 ];
		  
		 #pragma region Equivalencias
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina1 ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina1 ) )
		 {
			continue;
		 }
		 #pragma endregion
		 		 
		 if ( ! problemData->haDemandaDiscNoCampus( disciplina1->getId(), campusId ) )
			  continue;
		
		 // Turma 1
         for ( int turma1 = 1; turma1<= disciplina1->getNumTurmas(); turma1++ )
         {
			  Unidade *u1 = this->retornaUnidadeDeAtendimento( turma1, disciplina1, campus );
					   
			  if ( u1 == NULL )
			  {
				  continue;
			  }
			  
			  // Disciplina 2
			  ITERA_GGROUP_INIC_N_PT( it_disc2, it_disc1, disciplinas, int )
			  {
				  Disciplina * disciplina2 = problemData->refDisciplinas[ *it_disc2 ];
		  
				  #pragma region Equivalencias
				  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina2 ) !=
					     problemData->mapDiscSubstituidaPor.end() ) &&
						!problemData->ehSubstituta( disciplina2 ) )
				  {
					  continue;
				  }
				  #pragma endregion
		 		 
				  if ( ! problemData->haDemandaDiscNoCampus( disciplina2->getId(), campusId ) )
					  continue;

				  // Turma 2
				  for ( int turma2 = 1; turma2 <= disciplina2->getNumTurmas(); turma2++ )
				  {
					   Unidade *u2 = this->retornaUnidadeDeAtendimento( turma2, disciplina2, campus );

					   if ( u2 == NULL || u2 == u1 )
					   {
						   continue;
					   }

					    GGroup<Aluno*, LessPtr<Aluno>> alunosEmComum = 
							problemData->alunosEmComum( turma1, disciplina1, turma2, disciplina2, campus );

						if ( alunosEmComum.size() == 0 )
						{
							continue;
						}

						int nroAlunos = alunosEmComum.size();

						GGroup<int> dias = problemData->diasComunsEntreDisciplinas( disciplina1, disciplina2 );
						ITERA_GGROUP_N_PT( it_dias, dias, int )
						{
							int dia = *it_dias;

							// CONSTRAINT --------------------------------------

							c.reset();
							c.setType( ConstraintTatico::C_ALUNO_VARIAS_UNIDADES_DIA );

							c.setCampus( campus );
							c.setTurma1( turma1 );
							c.setTurma2( turma2 );
							c.setDisciplina1( disciplina1 );
							c.setDisciplina2( disciplina2 );
							c.setDia( dia );
							c.setCampus( campus );

							sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

							if ( cHashTatico.find( c ) != cHashTatico.end() )
							{
								continue;
							}

							nnz = 100;

							OPT_ROW row( nnz + 1, OPT_ROW::LESS , 1 , name );

							// Insere variavel fu_{i1,d1,i2,d2,t,cp} -------------------------------------------------------
							VariableTatico v;
							v.reset();
							v.setType( VariableTatico::V_SLACK_ALUNO_VARIAS_UNID_DIA );
							v.setTurma1( turma1 );            // i1
							v.setDisciplina1( disciplina1 );  // d1
							v.setTurma2( turma2 );            // i2
							v.setDisciplina2( disciplina2 );  // d2
							v.setCampus( campus );			  // cp
							v.setDia( dia );				  // t

							it_v = vHashTatico.find( v );
							if( it_v != vHashTatico.end() )
							{
								row.insert( it_v->second, -1.0 );
							}

							// Insere variaveis x_{i,d,u,s,hi,hf,t} -------------------------------------------------------
							it_v = vHashTatico.begin();

							for (; it_v != vHashTatico.end(); it_v++ )
							{
								VariableTatico v = it_v->first;

								if ( v.getType() != VariableTatico::V_CREDITOS )
								{
									continue;
								}

								// Insere variavel x_{i1,d1,u1,s1,hi,hf,t} ou x_{i2,d2,u2,s2,hi,hf,t}
								
								if ( ( v.getTurma() == turma1 &&
									   v.getDisciplina()->getId() == disciplina1->getId() && 
									   v.getDia() == dia && 
									   v.getUnidade() == u1 ) 
									   ||
									 ( v.getTurma() == turma2 &&
									   v.getDisciplina()->getId() == disciplina2->getId() && 
									   v.getDia() == dia && 
									   v.getUnidade() == u2 ) )
								{
									row.insert( it_v->second, 1.0 );
								}
							}

							// Insere restrição no Hash --------------------------------------------------------
							if ( row.getnnz() != 0 )
							{
								cHashTatico[ c ] = lp->getNumRows();

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

/*
	Evita os atendimentos parciais, ou seja, para um par de disciplinas
	(i1,-d) e (i2,d), aonde -d é a disciplina pratica e d a teorica correspondente,
	que possuam alunos em comum, tenta atender a as duas!

	fd_{i1,-d,cp} + fd_{i2,d,cp} - ffd_{i1,-d,i2,d,cp} = 0

	ffd é folga da folga de demanda, podendo assumir os valores {0,1,2}.
*/
int SolverMIP::criaRestricaoTaticoDiscPraticaTeorica( int campusId, int prioridade, int r, int tatico )
{
    int restricoes = 0;
    int nnz;
    char name[ 100 ];

    VariableTatico v;
    ConstraintTatico c;
    VariableTaticoHash::iterator it_v;
   
    Campus* campus = problemData->refCampus[ campusId ];
	     
	GGroup< int > disciplinas = problemData->cp_discs[ campusId ];

	ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
	{
		Disciplina * discPratica = problemData->refDisciplinas[ *it_disciplina ];

		#pragma region Equivalencias
		if ( ( problemData->mapDiscSubstituidaPor.find( discPratica ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( discPratica ) )
		{
			continue;
		}
		#pragma endregion

		if ( discPratica->getId() > 0 )
		{
			continue;
		}
		
		std::map< int, Disciplina * >::iterator itMapDisc =
			problemData->refDisciplinas.find( - discPratica->getId() );
				
		if ( itMapDisc == problemData->refDisciplinas.end() )
		{
			continue;
		}
		
		Disciplina *discTeorica = itMapDisc->second;

		if ( ! problemData->haDemandaDiscNoCampus( discPratica->getId(), campusId ) )
		{
			continue;
		}

		for ( int turma1 = 1; turma1 <= discPratica->getNumTurmas(); turma1++ )
		{
			for ( int turma2 = 1; turma2 <= discTeorica->getNumTurmas(); turma2++ )
			{		
				GGroup<Aluno*, LessPtr<Aluno>> alunos = problemData->alunosEmComum( turma1, discPratica, turma2, discTeorica, campus );
				
				if ( alunos.size() == 0 )
				{
					continue;
				}

				c.reset();
				c.setType( ConstraintTatico::C_DISC_PRATICA_TEORICA );
				c.setCampus( campus );
				c.setDisciplina1( discPratica );
				c.setTurma1( turma1 );
				c.setDisciplina2( discTeorica );
				c.setTurma2( turma2 );

				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}
			
				nnz = 3;
				OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

				// Variavel de folga fd_{dp,i1,cp}
				v.reset();
				v.setType( VariableTatico::V_SLACK_DEMANDA );
				v.setDisciplina( discPratica );  // dp
				v.setTurma( turma1 );			 // i1
				v.setCampus( campus );			 // cp

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}
			
				// Variavel de folga fd_{dt,i2,cp}
				v.reset();
				v.setType( VariableTatico::V_SLACK_DEMANDA );
				v.setDisciplina( discTeorica );  // dt
				v.setTurma( turma2 );			 // i2
				v.setCampus( campus );			 // cp

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}

				// Variavel de folga ffd_{i1,dp,i2,dt,cp}
				v.reset();
				v.setType( VariableTatico::V_SLACK_SLACKDEMANDA_PT );
				v.setTurma1( turma1 );			 // i1
				v.setDisciplina1( discPratica );  // dp
				v.setTurma2( turma2 );			 // i2
				v.setDisciplina2( discTeorica );  // dt				
				v.setCampus( campus );			 // cp

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, -2.0 );
				}

				// Insere restrição no Hash ---
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}
        }
   }

	return restricoes;
}


/*
	Evita atender somente a pratica e não atender a teórica. Mas não garante o contrário,
	ainda se faz necessária a restrição com a variavel fdd.
	
	Para um par de turmas (i1,-d,cp) e (i2,d,cp), aonde -d é a disciplina pratica e d a teorica correspondente,
	que possuam alunos em comum:

	fd_{i1,-d,cp} <= fd_{i2,d,cp}
*/
int SolverMIP::criaRestricaoTaticoDiscPraticaTeorica1xN( int campusId, int prioridade, int r, int tatico )
{
    int restricoes = 0;

	if ( ! problemData->parametros->discPratTeor1xN )
	{
		return restricoes;
	}

    int nnz;
    char name[ 100 ];

    VariableTatico v;
    ConstraintTatico c;
    VariableTaticoHash::iterator it_v;
   
    Campus* campus = problemData->refCampus[ campusId ];
	     
	GGroup< int > disciplinas = problemData->cp_discs[ campusId ];

	ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
	{
		int discId = *it_disciplina;
		if ( discId > 0 )
		{
			continue;
		}
		
		Disciplina * discPratica = problemData->refDisciplinas[ discId ];
		
		std::map< int, Disciplina * >::iterator itMapDisc =
			problemData->refDisciplinas.find( - discPratica->getId() );				
		if ( itMapDisc == problemData->refDisciplinas.end() )
		{
			std::cout << "\nErro em SolverMIP::criaRestricaoTaticoDiscPraticaTeorica1xN()!"
				<< " A disciplina " << discId << "  nao tem teorica.\n";
			continue;
		}
		
		Disciplina *discTeorica = itMapDisc->second;
		
		for ( int turmaP = 1; turmaP <= discPratica->getNumTurmas(); turmaP++ )
		{
			GGroup<int> turmasT = discPratica->getTurmasAssociadas( turmaP );

			if ( turmasT.size() != 1 )
			{
				std::cout<<"\nErro em SolverMIP::criaRestricaoTaticoDiscPraticaTeorica1xN()."
				<< " A disciplina " << discId << " tem " << turmasT.size() << " teoricas associadas.\n";
				continue;
			}

			int turmaT = *(turmasT.begin());
			
			c.reset();
			c.setType( ConstraintTatico::C_DISC_PRATICA_TEORICA_1xN );
			c.setCampus( campus );
			c.setDisciplina1( discPratica );
			c.setTurma1( turmaP );
			c.setDisciplina2( discTeorica );
			c.setTurma2( turmaT );

			sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

			if ( cHashTatico.find( c ) != cHashTatico.end() )
			{
				continue;
			}
			
			nnz = 2;
			OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

			// Variavel de folga fd_{dp,ip,cp}
			v.reset();
			v.setType( VariableTatico::V_SLACK_DEMANDA );
			v.setDisciplina( discPratica );  // dp
			v.setTurma( turmaP );			 // i1
			v.setCampus( campus );			 // cp

			it_v = vHashTatico.find( v );
			if( it_v != vHashTatico.end() )
			{
				row.insert( it_v->second, 1.0 );
			}
			
			// Variavel de folga fd_{dt,it,cp}
			v.reset();
			v.setType( VariableTatico::V_SLACK_DEMANDA );
			v.setDisciplina( discTeorica );  // dt
			v.setTurma( turmaT );			 // i2
			v.setCampus( campus );			 // cp

			it_v = vHashTatico.find( v );
			if( it_v != vHashTatico.end() )
			{
				row.insert( it_v->second, -1.0 );
			}
        }
   }

	return restricoes;
}


int SolverMIP::criaRestricaoTaticoMinDiasAluno( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno != ParametrosPlanejamento::MINIMIZAR_DIAS )
   {
		return restricoes;
   }

   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
	    {
			continue;
	    }

		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
			{
				int dia = *itDia;

				// CONSTRAINT --------------------------------------

				c.reset();
				c.setType( ConstraintTatico::C_MIN_DIAS_ALUNO );

				c.setCampus( *itCampus );
				c.setAluno( aluno );
				c.setDia( dia );

				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = ( aluno->demandas.size() * itCampus->horarios.size() * 2 );

				OPT_ROW row( nnz + 1, OPT_ROW::GREATER , 0 , name );

				// Insere variaveis Credito (x) ------------------------------------------------------

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{				   
						Sala *sala = itCjtSala->salas.begin()->second;
						
						ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
						{
							disciplina = ( *it_disciplina );
							
							#pragma region Equivalencias
							if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
								   problemData->mapDiscSubstituidaPor.end() ) &&
								 !problemData->ehSubstituta( disciplina ) )
							{
								continue;
							}
							#pragma endregion

							for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
							{
								// Para cada trio <campus, turma, disciplina> no qual o aluno está alocado
								Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
								trio.set( campusId, turma, disciplina );

								if ( problemData->mapAluno_CampusTurmaDisc[ aluno ].find( trio ) ==
									 problemData->mapAluno_CampusTurmaDisc[ aluno ].end() )
								{
									continue;
								}

								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
									problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
					
								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hi = *itHorario;

									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
									{
											HorarioAula *hf = *itHorario;

											if ( hf < hi )
											{
							 					continue;
											}	
											
											v.reset();
											v.setType( VariableTatico::V_CREDITOS );									
											v.setTurma( turma );
											v.setDisciplina( disciplina );
											v.setUnidade( *itUnidade );
											v.setSubCjtSala( *itCjtSala );
											v.setDia( dia );                   
											v.setHorarioAulaInicial( hi );	 // hi
											v.setHorarioAulaFinal( hf );	 // hf

											it_v = vHashTatico.find( v );
											if( it_v != vHashTatico.end() )
											{
												row.insert( it_v->second, -1.0 );
											}
									}
								}
							}
						}
					}
				}

				// Insere variavel du_{a,t} -------------------------------------------------------								
				v.reset();
				v.setType( VariableTatico::V_ALUNO_DIA );
				v.setAluno( aluno );
				v.setDia( dia );

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 15.0 );
				}

				// Insere restrição no Hash --------------------------------------------------------
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}  
		}
   }

   return restricoes;

}

int SolverMIP::criaRestricaoTaticoMaxDiasAluno( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno != ParametrosPlanejamento::EQUILIBRAR )
   {
		return restricoes;
   }

   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
	    {
			continue;
	    }

		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
			{
				int dia = *itDia;

				// CONSTRAINT --------------------------------------

				c.reset();
				c.setType( ConstraintTatico::C_MAX_DIAS_ALUNO );

				c.setCampus( *itCampus );
				c.setAluno( aluno );
				c.setDia( dia );

				sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = ( aluno->demandas.size() * itCampus->horarios.size() * 2 );

				OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0 , name );

				// Insere variaveis Credito (x) ------------------------------------------------------

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{				   
						Sala *sala = itCjtSala->salas.begin()->second;
						
						ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
						{
							disciplina = ( *it_disciplina );
							
							#pragma region Equivalencias
							if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
								   problemData->mapDiscSubstituidaPor.end() ) &&
								 !problemData->ehSubstituta( disciplina ) )
							{
								continue;
							}
							#pragma endregion

							for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
							{
								// Para cada trio <campus, turma, disciplina> no qual o aluno está alocado
								Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
								trio.set( campusId, turma, disciplina );

								if ( problemData->mapAluno_CampusTurmaDisc[ aluno ].find( trio ) ==
									 problemData->mapAluno_CampusTurmaDisc[ aluno ].end() )
								{
									continue;
								}

								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
									problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
					
								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hi = *itHorario;

									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
									{
											HorarioAula *hf = *itHorario;

											if ( hf < hi )
											{
							 					continue;
											}	
											
											v.reset();
											v.setType( VariableTatico::V_CREDITOS );									
											v.setTurma( turma );
											v.setDisciplina( disciplina );
											v.setUnidade( *itUnidade );
											v.setSubCjtSala( *itCjtSala );
											v.setDia( dia );                   
											v.setHorarioAulaInicial( hi );	 // hi
											v.setHorarioAulaFinal( hf );	 // hf

											it_v = vHashTatico.find( v );
											if( it_v != vHashTatico.end() )
											{
												row.insert( it_v->second, -1.0 );
											}
									}
								}
							}
						}
					}
				}

				// Insere variavel du_{a,t} -------------------------------------------------------								
				v.reset();
				v.setType( VariableTatico::V_ALUNO_DIA );
				v.setAluno( aluno );
				v.setDia( dia );

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}

				// Insere restrição no Hash --------------------------------------------------------
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}  
		}
   }

   return restricoes;

}

/*
	Para cada disciplina d, dia t, horarioAula h:

	sum[s]sum[i]sum[hi]sum[hf] x_{i,d,s,t,hi,hf} - fp_{d,t,h} <= nProfs_{d,t,h}

	sendo que (hi,hf) contém h

*/
int SolverMIP::criaRestricaoTaticoConsideraHorariosProfs( int campusId, int prioridade, int r, int tatico )
{
	int restricoes = 0;

	if ( ! problemData->parametros->considerar_disponibilidade_prof_em_tatico )
		return restricoes;

	int nnz;
	char name[ 100 ];

	VariableTatico v;
	ConstraintTatico c;

	VariableTaticoHash::iterator vit;
	ConstraintTaticoHash::iterator cit;
		
	std::map< Disciplina*, std::map<int /*dia*/, std::map< HorarioAula*, GGroup<Professor*, LessPtr<Professor>>, LessPtr<HorarioAula> >>, LessPtr<Disciplina> >
		mapDiscDiaHorProfs;
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
				mapDiscDiaHorProfs[disciplina][dia][horAula].add(prof);;
			}
		}
	}

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{		
		VariableTatico v = vit->first;
		
		if ( v.getType() == VariableTatico::V_CREDITOS )
		{
			Disciplina *disciplina = v.getDisciplina();
			int dia = v.getDia();

			HorarioAula *hf = v.getHorarioAulaFinal();
			HorarioAula *ha = v.getHorarioAulaInicial();
			
			bool end=false;
			while ( !end )
			{
				double coef = 1.0;

				c.reset();
				c.setType( ConstraintTatico::C_DISC_DIA_HOR_PROF );
				c.setDisciplina( disciplina );
				c.setDia( dia );
				c.setHorarioAula( ha );
				c.setDateTime(v.getDateTimeInicial());
				
				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					int nProfs = mapDiscDiaHorProfs[disciplina][dia][ha].size();

					sprintf( name, "%s", c.toString( prioridade,r,tatico ).c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS, nProfs, name );

					row.insert( vit->second, coef);

					cHashTatico[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
				else
				{
					bool jaExiste=false;
					for ( int i=0; i<(int)idxC.size(); i++ )
					{
						if ( idxC[i]==vit->second && idxR[i]==cit->second )
						{
							jaExiste=true;break;
						}
					}
					if (!jaExiste)
					{
						idxC.push_back(vit->second);
						idxR.push_back(cit->second);
						valC.push_back(coef);
					}
				}			

				if ( *ha == *hf ) end=true;
				else ha = ha->getCalendario()->getProximoHorario( ha );

				if ( ha==NULL )
				{
					std::cout<<"\n\nErro! horario eh null antes de chegar em hf";
					std::cout<<"\nDisc"<<disciplina->getId()<<" dia"<<dia;
					end=true;
				}
			}
		}
	}


	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{		
		VariableTatico v = vit->first;

		if ( v.getType() == VariableTatico::V_FOLGA_HOR_PROF )
		{
			Disciplina *disciplina = v.getDisciplina();
			int dia = v.getDia();

			HorarioAula *ha = v.getHorarioAulaInicial();

			double coef = -1.0;

			c.reset();
			c.setType( ConstraintTatico::C_DISC_DIA_HOR_PROF );			
			c.setDisciplina( disciplina );
			c.setDia( dia );
			c.setHorarioAula( ha );
			c.setDateTime(v.getDateTimeInicial());
			
			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
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


/*
Aulas de disciplinas pratica/teorica continuas

sum[s] x_{it,dt,s,t,hi,hf} <= sum[h]sum[s] x_{ip,dp,s,t,h,hi-1} + sum[h]sum[s] x_{ip,dp,s,t,hf+1,h}

Para toda disciplina d=(dt,dp) sendo d com obrigação de créditos contínuos
Para toda turma it \in I_{dt}, ip \in I_{dp} sendo que (it,dt) tem aluno comum com (ip,dp)
Para todo dia t
Para todo par de horários hi,hf
	
*/
int SolverMIP::criaRestricaoTaticoDiscPTAulasContinuas( int campusId, int prioridade, int r, int tatico )
{
	int restricoes = 0;
	int nnz;
	char name[ 100 ];

	VariableTatico v;
	ConstraintTatico c;
	VariableTaticoHash::iterator vit;
	ConstraintTaticoHash::iterator cit;

	Campus *campus = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

   std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableTatico > > >, LessPtr<Disciplina>> mapDiscDiaHiVar;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if( vit->first.getType() == VariableTatico::V_CREDITOS )
		{
			VariableTatico v = vit->first;
			if (v.getUnidade()->getIdCampus() == campusId)
			{
				if ( v.getDisciplina()->getId() > 0 )
				if ( problemData->getDisciplinaTeorPrat( v.getDisciplina() ) == NULL )
				{
					vit++; continue;
				}
				
				if ( v.getDisciplina()->aulasContinuas() )
				{	
					mapDiscDiaHiVar[ v.getDisciplina() ][ v.getTurma() ][ v.getDia() ][ v.getHorarioAulaInicial()->getInicio() ] = v;
				}
			}
		}
		vit++;
	}

	std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableTatico > > >, LessPtr<Disciplina>>::iterator
		itMapDisc = mapDiscDiaHiVar.begin();
	for ( ; itMapDisc != mapDiscDiaHiVar.end(); itMapDisc++ )
	{	
		if ( itMapDisc->first->getId() < 0 ) continue;

		Disciplina *disciplinaTeor = itMapDisc->first;

		std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableTatico > > >::iterator
			itTurma = itMapDisc->second.begin();
		for ( ; itTurma != itMapDisc->second.end(); itTurma++ )
		{
			int turmaTeor = itTurma->first;

			std::map< int /*dia*/, std::map< DateTime, VariableTatico > >::iterator
				itMapDia = itTurma->second.begin();

			for ( ; itMapDia != itTurma->second.end(); itMapDia++ )
			{	
				int dia = itMapDia->first;

				std::map< DateTime, VariableTatico >::iterator
					itMapDateTime = itMapDia->second.begin();
				for ( ; itMapDateTime != itMapDia->second.end(); itMapDateTime++ )
				{	
					DateTime dt = itMapDateTime->first;
					VariableTatico v_t = itMapDateTime->second;
				
					HorarioAula *hi = v_t.getHorarioAulaInicial();
					HorarioAula *hf = v_t.getHorarioAulaFinal();				
					
					DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
					DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;

					c.reset();
					c.setType( ConstraintTatico::C_AULA_PT_SEQUENCIAL );
					c.setCampus( campus );
					c.setTurma( turmaTeor );
					c.setDisciplina( disciplinaTeor );
					c.setDia( dia );
					c.setDateTimeInicial( dti );
					c.setDateTimeFinal( dtf );
					
					sprintf( name, "%s", c.toString( prioridade, r, tatico ).c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS , 0.0 , name );
				
					// --------------------
					// teórica
					row.insert( vHashTatico[v_t], 1.0 );
				
					Disciplina * disciplinaPrat = problemData->getDisciplinaTeorPrat( disciplinaTeor );
					Calendario * calendario = hf->getCalendario();
					int nCredsPrat = disciplinaPrat->getTotalCreditos();

					// --------------------
					// antes da teórica
					HorarioAula *hi_p1 = hi;
					for ( int i = 1; i <= nCredsPrat; i++ )
					{
						 hi_p1 = calendario->getHorarioAnterior( hi_p1 );
						 if ( hi_p1==NULL ) break;
					}
					if ( hi_p1 != NULL )
					{
						DateTime dti_p1 = hi_p1->getInicio();
					
						std::map<Disciplina*, std::map< int /*turma*/, 
							std::map< int /*dia*/, std::map< DateTime, VariableTatico > > >, LessPtr<Disciplina>>::iterator
						itDisc = mapDiscDiaHiVar.find( disciplinaPrat );
						if ( itDisc != mapDiscDiaHiVar.end() )
						{
							std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableTatico > > >::iterator
							itTurma = itDisc->second.find( turmaTeor );
							if ( itTurma != itDisc->second.end() )
							{
								std::map< int /*dia*/, std::map< DateTime, VariableTatico > >::iterator
								itDia = itTurma->second.find( dia );
								if ( itDia != itTurma->second.end() )
								{
									std::map< DateTime, VariableTatico >::iterator
									itDt = itDia->second.find( dti_p1 );
									if ( itDt != itDia->second.end() )
									{
										VariableTatico v_p1 = itDt->second;
										row.insert( vHashTatico[v_p1], -1.0 );
									}
								}
							}
						}										
					}

					// --------------------
					// após a teórica
					HorarioAula *hi_p2 = calendario->getProximoHorario( hf );
					if ( hi_p2 != NULL )
					{
						DateTime dti_p2 = hi_p2->getInicio();
									
						std::map<Disciplina*, std::map< int /*turma*/, 
							std::map< int /*dia*/, std::map< DateTime, VariableTatico > > >, LessPtr<Disciplina>>::iterator
						itDisc = mapDiscDiaHiVar.find( disciplinaPrat );
						if ( itDisc != mapDiscDiaHiVar.end() )
						{
							std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableTatico > > >::iterator
							itTurma = itDisc->second.find( turmaTeor );
							if ( itTurma != itDisc->second.end() )
							{
								std::map< int /*dia*/, std::map< DateTime, VariableTatico > >::iterator
								itDia = itTurma->second.find( dia );
								if ( itDia != itTurma->second.end() )
								{
									std::map< DateTime, VariableTatico >::iterator
									itDt = itDia->second.find( dti_p2 );
									if ( itDt != itDia->second.end() )
									{
										VariableTatico v_p2 = itDt->second;
										row.insert( vHashTatico[v_p2], -1.0 );
									}
								}
							}
						}
					}

					// --------------------
					if( row.getnnz() > 0 )
					{
						cHashTatico[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
			}
		}
	}

	if ( restricoes > 0 && !problemData->parametros->discPratTeor1x1 )
	{
		std::cout << "\nERRO: ha obrigatoriedade de aulas continuas para disciplinas praticas e teoricas"
			<< " mesmo permitindo-se a mistura de alunos entre turmas praticas e teoricas. Esse metodo nao"
			<< " esta preparado para isso. Assume-se aqui que um aluno está na turma 1 da disciplina teorica " 
			<< " se e somente se ele esta na turma 1 da pratica.\n";
	}

	return restricoes;
}


/*
	Alocação minima de demanda por aluno

	sum[d] nCreds_{d} * (1 - fd_{i,d,cp} - fa_{i,d,cp,a}) >= MinAtendPerc * TotalDemanda_{a} - fmd_{a}		, para cada aluno a
	
	sendo (i,d) deve ser uma turma na qual o aluno a está alocado pelo pre-modelo.

	min sum[a] fmd{a}
*/
int SolverMIP::criaRestricaoTaticoAlocMinPercAluno( int campusId, int prioridade, int r, int tatico )
{
   int restricoes = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return restricoes;
   }

   char name[ 200 ];
   int nnz=0;

   ConstraintTatico c;
   ConstraintTaticoHash::iterator cit;
   VariableTatico v;
   VariableTaticoHash::iterator vit;   

   std::map<int, GGroup<Disciplina*> > mapConstraintDiscs;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatico v = vit->first;
			
		double coef;
		Aluno *aluno;
		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDem;

		if( v.getType() == VariableTatico::V_SLACK_DEMANDA )			  // fd_{i,d,cp}
		{
			coef = - v.getDisciplina()->getTotalCreditos();
			Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio( v.getCampus()->getId(), v.getTurma(), v.getDisciplina() );
			alunosDem = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ];

			ITERA_GGROUP_LESSPTR( itAlDem, alunosDem, AlunoDemanda )
			{
				Aluno *aluno = problemData->retornaAluno( itAlDem->getAlunoId() );

				if ( !aluno->hasCampusId(campusId) )
				{
					continue;
				}
				
				c.reset();
				c.setType( ConstraintTatico::C_ALOC_MIN_ALUNO );
				c.setAluno( aluno );
		
				sprintf( name, "%s", c.toString(prioridade, r, tatico).c_str() ); 

				cit = cHashTatico.find( c ); 
				if ( cit != cHashTatico.end() )
				{
					lp->chgCoef( cit->second, vit->second, coef );
		
					mapConstraintDiscs[ cit->second ].add( v.getDisciplina() );
				}
				else
				{
					double init_rhs = problemData->parametros->minAtendPercPorAluno * aluno->getNroCredsOrigRequeridosP1();
		
					nnz += aluno->demandas.size() + 1;
					OPT_ROW row( nnz, OPT_ROW::GREATER, init_rhs , name );
		
					row.insert( vit->second, coef );
					cHashTatico[ c ] = lp->getNumRows();
			
					mapConstraintDiscs[ lp->getNumRows() ].add( v.getDisciplina() );
								
					lp->addRow( row );
					restricoes++;
				}
			}

		}
		else if( v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND1 ||	// fmd1_{a}
				 v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND2 ||	// fmd2_{a}
				 v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND3 ||	// fmd3_{a}
				 v.getType() == VariableTatico::V_DESALOCA_ALUNO )			// fa_{i,d,cp,a}
		{ 
			if( v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND1 ||	// fmd1_{a}
				v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND2 ||	// fmd2_{a}
				v.getType() == VariableTatico::V_FOLGA_ALUNO_MIN_ATEND3	)	// fmd3_{a})
			{
				coef = 1.0;
			}
			else
			{
				coef = - v.getDisciplina()->getTotalCreditos();
			}
			
			aluno = v.getAluno();						
		
			if ( !aluno->hasCampusId(campusId) )
			{
				continue;
			}
				
			c.reset();
			c.setType( ConstraintTatico::C_ALOC_MIN_ALUNO );
			c.setAluno( aluno );
		
			sprintf( name, "%s", c.toString(prioridade, r, tatico).c_str() ); 

			cit = cHashTatico.find( c ); 
			if ( cit != cHashTatico.end() )
			{
				lp->chgCoef( cit->second, vit->second, coef );
			
				if( v.getType() == VariableTatico::V_DESALOCA_ALUNO ) // fa_{i,d,a}
				{
					mapConstraintDiscs[ cit->second ].add( v.getDisciplina() );
				}
			}
			else
			{
				double init_rhs = problemData->parametros->minAtendPercPorAluno * aluno->getNroCredsOrigRequeridosP1();
		
				nnz += aluno->demandas.size() + 1;
				OPT_ROW row( nnz, OPT_ROW::GREATER, init_rhs , name );
		
				row.insert( vit->second, coef );
				cHashTatico[ c ] = lp->getNumRows();
			
				if( v.getType() == VariableTatico::V_DESALOCA_ALUNO ) // fa_{i,d,a}
				{			
					mapConstraintDiscs[ lp->getNumRows() ].add( v.getDisciplina() );
				}
			
				lp->addRow( row );
				restricoes++;
			}
			
		}
	}


	// Atualiza rhs

	std::map<int, GGroup<Disciplina*> >::iterator itMap = mapConstraintDiscs.begin();			
	for ( ; itMap != mapConstraintDiscs.end(); itMap++ )
	{		
		int rowId = itMap->first;
		int somaCredsFd = 0;
			
		GGroup<Disciplina*> discs = itMap->second;
		ITERA_GGROUP( itDisc, discs, Disciplina )
		{
			somaCredsFd += itDisc->getTotalCreditos();
		}

		double init_rhs = lp->getRHS( rowId );
		double rhs = init_rhs - somaCredsFd;
		lp->chgRHS( rowId, rhs );
	}

	return restricoes;
}




void SolverMIP::chgCoeffList(
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




void SolverMIP::imprimeAlocacaoFinalHeuristica( int campusId, int prioridade, int grupoAlunosAtualId )
{
	stringstream ssCp; 	stringstream ssP; 	stringstream ssCjt;
	ssCp << campusId; 	ssP << prioridade;	ssCjt << grupoAlunosAtualId;
	std::string heurFilename( "HeuristicaFinalAlunoDemanda_Cp" );    
	heurFilename += ssCp.str();
	heurFilename += "_P"; 
	heurFilename += ssP.str();
	heurFilename += "_Cjt"; 
	heurFilename += ssCjt.str();
    heurFilename += ".txt";

	ofstream heurisFile;
	heurisFile.open(heurFilename, ios::out);
	if (!heurisFile)
	{
		std::cout << "Error: Can't open output file " << heurFilename << endl;fflush(NULL);
		return;
	}
	
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunoDemandaAtendsHeuristica, AlunoDemanda )
	{
		heurisFile << "AlunoDemandaId" << itAlDem->getId() 
					<< " Aluno" << itAlDem->getAlunoId() 
					<< " Demanda" <<  itAlDem->getDemandaId() 
					<< " Disc" <<  itAlDem->demanda->getDisciplinaId()
					<< " Prior" <<  itAlDem->getPrioridade() << endl;
	}
	heurisFile.close();
}

void SolverMIP::escreveSolucaoBinHeuristica( int campusId, int prioridade, int grupoAlunosAtualId )
{
	// WRITES SOLUTION
	char solName[1024];
	strcpy( solName, getSolHeuristBinFileName( campusId, prioridade, grupoAlunosAtualId ).c_str() );

	FILE * fout = fopen( solName, "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em SolverMIP::gravaSolucaoBinHeuristica( int campusId, int prioridade, int cjtAlunosId ):"
				<< "\nArquivo nao pode ser aberto.\n";
	}
	else
	{		 
		 fwrite( &problemData->totalTurmas_AlDem, sizeof( int ), 1, fout );

		 ITERA_GGROUP_LESSPTR( itAlDem, problemData->alunosDemanda, AlunoDemanda )
		 {
			 Aluno* aluno = problemData->retornaAluno( (*itAlDem)->getAlunoId() );
			 Disciplina *disciplina = (*itAlDem)->demanda->disciplina;

			 int turmaAloc = problemData->retornaTurmaDiscAluno( aluno, disciplina );

			 for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			 {
				 if ( turma == turmaAloc )
				 {
					double value = 1.0;
					fwrite( &value, sizeof( double ), 1, fout );
				 }
				 else
				 {
					 double value = -1.0;
					 fwrite( &value, sizeof( double ), 1, fout );
				 }
			 }
		 }
		 fclose( fout ); 
	}

}

bool SolverMIP::leSolucaoHeuritica( int campusId, int prioridade, int grupoAlunosAtualId )
{
	char solHeuristName[1024];
	strcpy( solHeuristName, getSolHeuristBinFileName( campusId, prioridade, grupoAlunosAtualId ).c_str() );
	FILE* finh = fopen( solHeuristName,"rb");
	if ( finh == NULL )
	{
		std::cout << "\nA partir de " << solHeuristName << " , nao foram lidas mais solucoes.\n";
		return false;
	}
	else
	{
		cout<<"\n====================> carregando pre " <<solHeuristName <<endl;

		// lê Solucao Heuristica   	
		int nCols = 0;
		fread(&nCols,sizeof(int),1,finh);
		
		double * xSol = new double[ nCols ];

		for (int i=0; i < nCols; i++)
		{
			double auxDbl;
			fread(&auxDbl,sizeof(double),1,finh);
			xSol[i] = auxDbl;
		}	
		fclose(finh);
		
		if ( nCols != problemData->totalTurmas_AlDem )
		{
			std::cout<<"\nError em leSolucaoHeuritica: O TOTAL DE TURMAS NAO BATE!\n";
			std::cout<<"\nnCols="<<nCols<<"   total de turma="<<problemData->totalTurmas_AlDem<<endl;
			return false;
		}

		int at = 0;
		ITERA_GGROUP_LESSPTR( itAlDem, problemData->alunosDemanda, AlunoDemanda )
		{
			Aluno* aluno = problemData->retornaAluno( (*itAlDem)->getAlunoId() );
			Disciplina *disciplina = (*itAlDem)->demanda->disciplina;
			int campusId = aluno->getOferta( itAlDem->demanda )->getCampusId();

			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			{
				if ( xSol[at] == 1.0 )
				{					
					Trio< int, int, Disciplina* > trio;
					trio.set( campusId, turma, disciplina);
					std::map<int/*dia*/, double> diasNCreds;

					// enviando diasNCreds vazio. Nao dará problema,
					// pois por enquanto isso só é usado na execução da heuristica.
					problemData->insereAlunoEmTurma( aluno, trio, diasNCreds );
				}
				else if ( xSol[at] != -1.0 )
				{
					std::cout<<"\nError!!!!!!!!!\nSo deveria ter 1.0 e -1.0\n";
				}
				at++;
			}
		}

		cout<<"<==================== Solucao carregada com sucesso"<<endl<<endl;
		return true;
	}
}


/* ======================================================================================================
							NOVA HEURISTICA - MODELO TATICO COM HORARIOS
*/

void SolverMIP::heuristica2AlocaAlunos( int campusId, int prioridade, int grupoAlunosAtualId )
{
	std::cout<<"\n======================================================";
	std::cout<<"\nHeuristica\n";

	stringstream ssCp; 	stringstream ssP; 	stringstream ssCjt;
	ssCp << campusId; 	ssP << prioridade;	ssCjt << grupoAlunosAtualId;
	std::string heurFilename( "HeuristAlunoDemanda_Cp" );    
	heurFilename += ssCp.str();
	heurFilename += "_P"; 
	heurFilename += ssP.str();
	heurFilename += "_Cjt"; 
	heurFilename += ssCjt.str();
    heurFilename += ".txt";

	ofstream heurisFile;
	heurisFile.open(heurFilename, ios::out);
	if (!heurisFile)
	{
		cerr << "Can't open output file " << heurFilename << endl;
	}
	else heurisFile.close();

	Campus *cp = problemData->refCampus[campusId];

	map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = problemData->cjtAlunos.begin();
	
	// Para o conjunto de alunos com id igual ao atual
	for ( ; itMapCjtAlunos != problemData->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int grupoAlunosId = itMapCjtAlunos->first;
		if ( grupoAlunosId != grupoAlunosAtualId )
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

			std::cout<<"\n--------------------------\nAluno "<<aluno->getAlunoId();	fflush(NULL);

			// Calcula a carga horária máxima que deve ser atendida de P2
			double tempoNaoAtendidoP1 = problemData->cargaHorariaNaoAtendidaPorPrioridade( prioridade-1, aluno->getAlunoId() );		
			double tempoP2 = problemData->cargaHorariaAtualRequeridaPorPrioridade( prioridade, aluno );
			if ( tempoNaoAtendidoP1 == 0 || tempoP2 == 0 )
				continue;

			double maxTempoP2 = tempoNaoAtendidoP1;
			if (maxTempoP2 > tempoP2)
				maxTempoP2 = tempoP2;
			
			std::cout<<"\nItera alunoDemandas";	fflush(NULL);

			double tempoAddP2 = 0.0;		
			
			GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alDemList = aluno->demandas;
			
			std::list< std::pair< AlunoDemanda*, int > > 
				alunosDemandaP2Ordenados = ordenaAlunosDemandaP2ParaHeurist( alDemList, aluno, prioridade ); 

			while (1)
			{
				bool sucesso=false; 

				GGroup<AlunoDemanda*> alDemPTnaoConsiderar;

				std::list< std::pair< AlunoDemanda*, int > >::iterator 
					itList = alunosDemandaP2Ordenados.begin();
				for ( ; itList!=alunosDemandaP2Ordenados.end(); itList++ )
				{
					// Máximo de 2 créditos de excesso em P2
					if ( (*itList).second > 2 )
						continue;

					// ---------------------------------------------
					// Primeira disciplina
					AlunoDemanda *alDem1 = (*itList).first;

					Disciplina *disciplina1 = alDem1->demanda->disciplina;
				
					std::cout<<"\nAlDem1 disc"<<disciplina1->getId();	fflush(NULL);

					// Se existir a disciplina teorica/pratica correspondente
					bool TEOR_PRAT=false;
					Disciplina *disciplina2 = problemData->getDisciplinaTeorPrat( disciplina1 );
					if ( disciplina2 != NULL ) TEOR_PRAT = true;
								
					if ( TEOR_PRAT )
					{	
						if ( problemData->retornaTurmaDiscAluno( aluno, disciplina1 ) != -1 && 
							 problemData->retornaTurmaDiscAluno( aluno, disciplina2 ) != -1 )
							continue; // alocações já efetivadas

						AlunoDemanda *alDem2=NULL;				

						std::list< std::pair< AlunoDemanda*, int > >::iterator 
							itListAux = alunosDemandaP2Ordenados.begin();
						for ( ; itListAux!=alunosDemandaP2Ordenados.end(); itListAux++ )
						{
							if ( itListAux->first->demanda->getDisciplinaId() == disciplina2->getId() ){
								alDem2 = itListAux->first;
								break;
							}
						}					
						if ( alDem2 == NULL )
						{
							std::cout<<"\nError: nao existe o par da disciplina teorica/pratica "
							<<disciplina2->getId()<<" nas demandas do aluno "<<aluno->getAlunoId();
							continue;
						}
						if ( ( problemData->retornaTurmaDiscAluno( aluno, disciplina1 ) != -1 && 
							   problemData->retornaTurmaDiscAluno( aluno, disciplina2 ) == -1 ) ||
							 ( problemData->retornaTurmaDiscAluno( aluno, disciplina1 ) == -1 && 
							   problemData->retornaTurmaDiscAluno( aluno, disciplina2 ) != -1 ) )
						{
							std::cout<<"\nAtencao/Erro1: aluno alocado somente em 1 das disciplinas de par teorico/pratico "<<disciplina1->getId();
							fflush(NULL);
							continue;
						}
					
						for ( int turma1 = 1; turma1 <= disciplina1->getNumTurmas(); turma1++ )
						{
							std::cout<<"\nTurma"<<turma1;	fflush(NULL);

							if ( heuristica2TentaInsercaoNaTurma( alDem1, turma1, heurFilename ) )
							{		
								std::cout<<"... inseriu";	fflush(NULL);

								// ---------------------------------------------
								// Segunda disciplina
														
								bool SUCESSO2 = false;

								std::cout<<"\nAlDem2 disc"<<disciplina2->getId();	fflush(NULL);
						
								for ( int turma2 = 1; turma2 <= disciplina2->getNumTurmas(); turma2++ )
								{
									std::cout<<"\nTurma"<<turma2;	fflush(NULL);

									if ( heuristica2TentaInsercaoNaTurma( alDem2, turma2, heurFilename ) )
									{
										std::cout<<"... inseriu";	fflush(NULL);
										tempoAddP2 += disciplina1->getTempoCredSemanaLetiva() * disciplina1->getTotalCreditos();
										tempoAddP2 += disciplina2->getTempoCredSemanaLetiva() * disciplina2->getTotalCreditos();										
										SUCESSO2 = true;
										break;
									}
								}
								if ( !SUCESSO2 )
								{									
									std::cout<<"\nRemovendo disc 1";	fflush(NULL);

									// Retira a inserção feita da PRIMEIRA disciplina
									
									Campus *campus1 = alDem1->getOferta()->campus;
									int campus1Id = campus1->getId();
			
									Trio< int, int, Disciplina* > trio1;
									trio1.set( campus1Id, turma1, disciplina1 );
															
									// Recupera os HorarioDia da turma
									GGroup<HorarioDia*> horariosDias1;
									GGroup< VariableTatico*, LessPtr<VariableTatico> > varsTaticoX =
										this->retornaVariableTaticoCreditosAnterior( turma1, disciplina1, campus1 );
									ITERA_GGROUP_LESSPTR( itVarX, varsTaticoX, VariableTatico )
									{
										int dia = (*itVarX)->getDia();
										HorarioAula* hi = (*itVarX)->getHorarioAulaInicial();
										HorarioAula* hf = (*itVarX)->getHorarioAulaFinal();
		
										int nCreds = hi->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
										HorarioAula *ha = hi;
										for ( int i = 1; i <= nCreds; i++ )
										{
											HorarioDia *hd = problemData->getHorarioDiaCorrespondente( ha, dia );
											horariosDias1.add( hd );
											ha = hi->getCalendario()->getProximoHorario( ha );				
										}
									}

									std::cout<<"\nRemovendo aluno de turma";	fflush(NULL);							
									problemData->removeAlunoDeTurma( alDem1, trio1, horariosDias1 );

									alunoDemandaAtendsHeuristica.remove( alDem1 );
									ofstream heurisFile;
									heurisFile.open(heurFilename, ios::app);
									if ( heurisFile )
									{
										heurisFile << "\nRemovido: AlunoDemandaId" << alDem1->getId() 
												<< " Aluno" << alDem1->getAlunoId() 
												<< " Demanda" << alDem1->getDemandaId() 
												<< " Disc" << alDem1->demanda->getDisciplinaId()
												<< " Turma" << turma1
												<< " Prior" << alDem1->getPrioridade() << endl;
										heurisFile.close();
									}
									std::cout<<"\nFim da remocao";	fflush(NULL);
								}
								else
								{
									sucesso=true;
									break; // SUCESSO!
								}
								// ---------------------------------------------
							}
						}
						if (!sucesso)
						{
							// Não conseguiu inserir a disciplina 1,
							// não considerar a disciplina-par que está ADIANTE
							// na list alunosDemandaP2Ordenados (ou seja, alDem2).
							alDemPTnaoConsiderar.add( alDem2 );
						}
					}
					else
					{
						for ( int turma = 1; turma <= disciplina1->getNumTurmas(); turma++ )
						{
							std::cout<<"\nTurma"<<turma;	fflush(NULL);

							if ( heuristica2TentaInsercaoNaTurma( alDem1, turma, heurFilename ) )
							{
								std::cout<<"... inseriu";	fflush(NULL);

								tempoAddP2 += disciplina1->getTempoCredSemanaLetiva() * disciplina1->getTotalCreditos();
								sucesso=true;
								break;
							}
						}
					}

					if ( sucesso ) break;
				
				} // FIM DE INSERÇÃO BEM SUCEDIDA
								
				if ( tempoAddP2 - maxTempoP2 > -1e-5 )
					break;

				// Reconstroi a lista de AlunoDemanda contendo somente os alunoDemanda não testados ainda.
				alDemList.clear();
				if ( sucesso )
				{
					alunosDemandaP2Ordenados.reverse();
					std::list< std::pair< AlunoDemanda*, int > >::iterator 
						itListAux = alunosDemandaP2Ordenados.begin();
					for ( ; itListAux!=itList; itListAux++ )
					{
						if ( alDemPTnaoConsiderar.find( itListAux->first ) != alDemPTnaoConsiderar.end() ) 
							continue;	

						alDemList.add( itListAux->first );
					}
				}

				alunosDemandaP2Ordenados = ordenaAlunosDemandaP2ParaHeurist( alDemList, aluno, prioridade );
   
				if ( alunosDemandaP2Ordenados.size() == 0 )
					break;
			}

			std::cout<<"\n-------------------------Fim aluno\n";	fflush(NULL);			
		}
	}
	
	std::cout<<"\nFim da Heuristica!";
	std::cout<<"\n======================================================\n\n";
}


bool SolverMIP::heuristica2TentaInsercaoNaTurma( AlunoDemanda *alunoDemanda, int turma, std::string heurFilename )
{
	std::cout<<"\n====>heuristicaTentaInsercaoNaTurma";

	fflush(NULL);

	ofstream heurisFile;
	heurisFile.open(heurFilename, ios::app);
	if (!heurisFile)
	{
		std::cout << "\nError: Can't open output file " << heurFilename << endl;
	
		fflush(NULL);
		return false;
	}
	
	Disciplina* disciplina = alunoDemanda->demanda->disciplina;
	Aluno *aluno = problemData->retornaAluno( alunoDemanda->getAlunoId() );
	int campusId = alunoDemanda->getOferta()->getCampusId();
	Campus *cp = problemData->refCampus[campusId];
	Calendario *sl_aluno = alunoDemanda->getOferta()->curriculo->calendario;

	bool viola = false;

	if ( !problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId ) )
	{
		if (heurisFile)
			heurisFile.close();
		std::cout<<"\nNao existe a turma";	fflush(NULL);
			
		fflush(NULL);
		return false;
	}
		
	GGroup< VariableTatico*, LessPtr<VariableTatico> > varsTaticoX =
			this->retornaVariableTaticoCreditosAnterior( turma, disciplina, cp );			
	
	if ( varsTaticoX.size() == 0 ){
		std::cout<<"\n\nERROR: varsTaticoX null\n\n";	fflush(NULL);
		if (heurisFile)
			heurisFile.close();
		return false;
	}

	Trio< int, int, Disciplina* > trio;
	trio.set( campusId, turma, disciplina );
	int lotacao = problemData->mapCampusTurmaDisc_AlunosDemanda[trio].size();			

	// Calendario igual ao da oferta do aluno
	ITERA_GGROUP_LESSPTR( itVarX, varsTaticoX, VariableTatico )
	{
		if ( itVarX->getHorarioAulaInicial()->getCalendario()->getId() != sl_aluno->getId() )
			viola=true;
	}

	// ----------------------------------------------
	// Capacidade máxima da sala
	ITERA_GGROUP_LESSPTR( itVarX, varsTaticoX, VariableTatico )
	{
		Sala* s = (*itVarX)->getSala();
		int capSala = s->getCapacidade();
		
		if ( lotacao >= capSala )
		{
			viola = true;
			break;	
		}
	}
	
	std::cout<<"\nTestei capacidade da sala";	fflush(NULL);
	
	GGroup<HorarioDia*> horariosDias;

	
	// ----------------------------------------------
	// Horarios do aluno nos dias	
	if ( ! viola )
	{
		ITERA_GGROUP_LESSPTR( itVarX, varsTaticoX, VariableTatico )
		{
			int dia = (*itVarX)->getDia();
			HorarioAula* hi = (*itVarX)->getHorarioAulaInicial();
			HorarioAula* hf = (*itVarX)->getHorarioAulaFinal();
		
			Calendario* sl = hi->getCalendario();

			int nCreds = sl->retornaNroCreditosEntreHorarios( hi, hf );
			HorarioAula *ha = hi;
			for ( int i = 1; i <= nCreds; i++ )
			{
				HorarioDia *hd = problemData->getHorarioDiaCorrespondente( ha, dia );
				horariosDias.add( hd );
				if ( aluno->sobrepoeHorarioDiaOcupado( hd ) )
				{
					viola = true; break;
				}
				ha = sl->getProximoHorario( ha );				
			}
			if ( viola ) break;
		}
		std::cout<<"\nTestei horarios do aluno no dia";	fflush(NULL);
	}
	// ----------------------------------------------
	
	if ( ! viola )
	{					
		std::cout<<"\nSolucao valida!!!";	fflush(NULL);

		// Insere aluno na turma e atualiza o nro de creditos por dia alocados pro aluno
		problemData->insereAlunoEmTurma( alunoDemanda, trio, horariosDias );	
		
		alunoDemandaAtendsHeuristica.add( alunoDemanda );

		heurisFile << "AlunoDemandaId" << alunoDemanda->getId() 
					<< " Aluno" << alunoDemanda->getAlunoId() 
					<< " Demanda" <<  alunoDemanda->getDemandaId() 
					<< " Disc" <<  alunoDemanda->demanda->getDisciplinaId()
					<< " Turma" <<  turma
					<< " Prior" <<  alunoDemanda->getPrioridade() << endl;

		if (heurisFile)
			heurisFile.close();

		fflush(NULL);
		return true; // ok
	}
	else
	{
		std::cout<<"\nSolucao INvalida!!!";	fflush(NULL);		    
	}

	if (heurisFile)
		heurisFile.close();

	fflush(NULL);
	return false;
}

/*
	Retorna list de pares <AlunoDemanda,int> ordenada em ordem crescente do módulo segundo termo do par,
	que indica a diferença entre o número de créditos da disciplina em AlunoDemanda e o 
	número de créditos que falta para que o aluno complete a carga horária requerida em P1.
	O segundo termo do par NÃO é o módulo, somente a ordenação da lista leva o módulo em consideração.
*/
std::list< std::pair< AlunoDemanda*, int > > SolverMIP::ordenaAlunosDemandaP2ParaHeurist( 
	GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alDemList, Aluno *aluno, int prioridade )
{
	std::list< std::pair< AlunoDemanda*, int > > alunosDemandaP2Ordenados;

	double tempoRequeridoP1 = aluno->getCargaHorariaOrigRequeridaP1();	
	double totalJaAtendido = problemData->cargaHorariaJaAtendida( aluno );

	double tempoNaoAtendido = tempoRequeridoP1 - totalJaAtendido;
	if ( tempoNaoAtendido<=0 )
		return alunosDemandaP2Ordenados;

	// ----------------------------------------------------------------
	// Ordena as demandas de P2 para tentativas de inserção em ordem apropriada.
			
	ITERA_GGROUP_LESSPTR( itAlDemanda, alDemList, AlunoDemanda )
	{
		AlunoDemanda *alDem = *itAlDemanda;
		Disciplina *disciplina = alDem->demanda->disciplina;
		
		if ( alDem->getPrioridade() != prioridade )
			continue;
		if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) != -1 )
			continue;

		double tempoDisc = disciplina->getTempoCredSemanaLetiva() * disciplina->getTotalCreditos();
		int discId = disciplina->getId();

		// se for pratica/teorica, adiciona a tempoDisc a duração da disciplina-par
		std::map< int, Disciplina* >::iterator itMapDisc = problemData->refDisciplinas.find(-discId );
		if ( itMapDisc != problemData->refDisciplinas.end() )
		{  
			tempoDisc += (*itMapDisc).second->getTempoCredSemanaLetiva() *
							(*itMapDisc).second->getTotalCreditos();
		}

		int dif = tempoDisc-tempoNaoAtendido;
		std::pair< AlunoDemanda*, int > alDem_dif( alDem, dif );

		alunosDemandaP2Ordenados.push_back( alDem_dif );
	}

	alunosDemandaP2Ordenados.sort( compare_dif_func );

	// ----------------------------------------------------------------

	return alunosDemandaP2Ordenados;
}


#pragma region Módulo Tático com Horários - Heurístico

#ifdef TATICO_COM_HORARIOS_HEURISTICO

// x_{i,d,u,s,hi,hf,t} (utilização do método criaVariavelTaticoCreditos)
// fu_{i1,d1,i2,d2,t,cp} (utilização do método criaVariavelTaticoFolgaAlunoUnidDifDia)
SolverTaticoHeur* SolverMIP::criaSolverTaticoHeuristico(int campusId, int P, int r)
{
   Campus* campus = problemData->getCampus(campusId);
   if (campus == NULL)
      return NULL;

   SolverTaticoHeur* solver = new SolverTaticoHeur(problemData, campus);

   int numVars = 0;
   TrioInt trio;  // Trio<int campusId, int turma, int discId>
	std::map<int, HorarioDiaGroup*> turma_horariosDiaComunsAosAlunos;
   std::hash_map<int, bool> demandaDisciplinaCampus;

   ITERA_GGROUP_LESSPTR(itDisciplina, problemData->disciplinas, Disciplina)
   {
      Disciplina* disciplina = *itDisciplina;
			   
	   if (!problemData->haDemandaDiscNoCampus(disciplina->getId(), campusId))
      {
         demandaDisciplinaCampus[disciplina->getId()] = false;
         continue;
      }

      demandaDisciplinaCampus[disciplina->getId()] = true;

	   turma_horariosDiaComunsAosAlunos.clear();
	   for (int turma=1; turma <= disciplina->getNumTurmas(); turma++)
      {
		   trio.set(campusId, turma, disciplina->getId());
         std::map<TrioInt, HorarioDiaGroup>::iterator itTrio = problemData->mapTurma_HorComunsAosCalendarios.find(trio);
         if (itTrio != problemData->mapTurma_HorComunsAosCalendarios.end())
            turma_horariosDiaComunsAosAlunos[turma] = &itTrio->second;
	   }

      for (std::map<int, ConjuntoSala*>::iterator itCjtSala = disciplina->cjtSalasAssociados.begin(); itCjtSala != disciplina->cjtSalasAssociados.end(); ++itCjtSala)
      {
         ConjuntoSala* cjtSala = itCjtSala->second;

         if (cjtSala->salas.size() > 1)
   			std::cout << "\nATENCAO em criaInstanciaTaticoHeuristico: conjunto sala deve ter somente 1 sala! \n";

         Sala* sala = cjtSala->salas.begin()->second;
         Unidade* unidade = problemData->refUnidade[sala->getIdUnidade()];

         // std::map<std::pair<int idDisc, int idSala>, std::map<int Dia, HorarioAulaGroup>>
			std::map<std::pair<int, int>, std::map<int, HorarioAulaGroup>>::iterator it_Disc_Sala_Dias_HorariosAula = problemData->disc_Salas_Dias_HorariosAula.find(std::pair<int, int>(disciplina->getId(), sala->getId()));
			if (it_Disc_Sala_Dias_HorariosAula == problemData->disc_Salas_Dias_HorariosAula.end())
            continue;
			   			   
			for (int turma=1; turma <= disciplina->getNumTurmas(); turma++)
         {
            HorarioDiaGroup* horariosDiaComunsAosAlunos = turma_horariosDiaComunsAosAlunos[turma];

            std::map<int, HorarioAulaGroup> dia_horariosAula_comuns = it_Disc_Sala_Dias_HorariosAula->second;
            // -------------------------------------------------------------
            // Faz a interseção entre os dois conjuntos de horarios comuns
            HorarioDiaGroup remover;
            for (std::map<int, HorarioAulaGroup>::iterator it_Dia_HorarioAula = dia_horariosAula_comuns.begin(); it_Dia_HorarioAula != dia_horariosAula_comuns.end(); ++it_Dia_HorarioAula)
            {
               int dia = it_Dia_HorarioAula->first;
               ITERA_GGROUP_LESSPTR(itHorAula, it_Dia_HorarioAula->second, HorarioAula)
               {
                  HorarioDia *hd = problemData->getHorarioDiaCorrespondente(*itHorAula, dia);
                  if (horariosDiaComunsAosAlunos->find(hd) == horariosDiaComunsAosAlunos->end())
                     remover.add(hd);
               }
            }
            ITERA_GGROUP_LESSPTR(itRemover, remover, HorarioDia)
            {
               dia_horariosAula_comuns[itRemover->getDia()].remove(itRemover->getHorarioAula());
            }
				// -------------------------------------------------------------

				for (std::map<int, HorarioAulaGroup>::iterator it_Dia_HorarioAula = dia_horariosAula_comuns.begin(); it_Dia_HorarioAula != dia_horariosAula_comuns.end(); ++it_Dia_HorarioAula)
            {
               int dia = it_Dia_HorarioAula->first;
					  
               ITERA_GGROUP_LESSPTR( itHorarioI, it_Dia_HorarioAula->second, HorarioAula )
               {
                  HorarioAula *hi = *itHorarioI;

                  ITERA_GGROUP_INIC_LESSPTR(itHorarioF, itHorarioI, it_Dia_HorarioAula->second, HorarioAula)
                  {
                     HorarioAula *hf = *itHorarioF;

						   if (!disciplina->inicioTerminoValidos(hi, hf, dia, it_Dia_HorarioAula->second))
								continue;

                     std::pair<DateTime*, int>& auxPi = problemData->horarioAulaDateTime[hi->getId()];
                     std::pair<DateTime*, int>& auxPf = problemData->horarioAulaDateTime[hf->getId()];

                     VariableTatico v;
                     v.reset();
                     v.setType(VariableTatico::V_CREDITOS);

                     v.setTurma(turma);            // i
                     v.setDisciplina(disciplina);  // d
                     v.setUnidade(unidade);        // u
                     v.setSubCjtSala(cjtSala);     // tps  
                     v.setDia(dia);                // t
                     v.setHorarioAulaInicial(hi);  // hi
                     v.setHorarioAulaFinal(hf);    // hf
                     v.setDateTimeInicial(auxPi.first);
                     v.setDateTimeFinal(auxPf.first);

                     if (!solver->hasVariable(v))
                     {
							   if (!criaVariavelTatico(&v))
								   continue;

                        if (P == 1 && r > 1 && !criaVariavelTatico_Anterior(&v))
                           continue;

                        solver->addVariable(v);
						   }
                  }
               }
            }
         }
      }
   }
   
   solver->build();

   return solver;
}


#endif

#pragma endregion