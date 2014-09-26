#include "CmdLine.h"
#include <iostream>
#include <string>
#include <sstream>

#include "HeurNuno\HeuristicaNuno.h"
#include "HeurNuno\\ParametrosHeuristica.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

#ifndef PATH_SEPARATOR
#ifdef WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif
#endif


#define DEFAULT_MIP_UNICO


CmdLine::CmdLine( int argc, char**& argv) : argc_ (argc), argv_ (argv)
{}

CmdLine::~CmdLine()
{

}

bool CmdLine::checkNArgs()
{
   cout << "Checking arguments..." <<endl;
   cout << "Arguments[" << argc_ << "]: ";
   for (int i=0; i<argc_; i++)
   {
      cout << argv_[i] << " ";
   }
   cout << "\n";

   // Check command line
   if( argc_ <= 2 )
   {
	   cout << "Invalid parameters in command line.\n";
       cout << "Usage: solver <instanceID> <path>\n";

	   return false;
   }

   return true;
}

void CmdLine::init()
{
   // Initializations
   path_[0] = '\0';
   inputFile_[0] = '\0';
   
   cout << "Reading id..." <<endl;

   // Input Id
   bool possuiId = findInputId(inputId_);

   cout << "Reading path..." <<endl;

   // Read path
   strcat( path_, argv_[2] );
   strcat( path_, PATH_SEPARATOR );

   cout << "Reading inputFile..." <<endl;

   // Input file name
   strcat( inputFile_, path_ );
   strcat( inputFile_, "input" );
   strcat( inputFile_, argv_[ 1 ] );

   // Output names
   setOutputNames();

   // Tipo do solver a ser executado
   setTipoSolver();
   
   std::cout<<"\nid " << inputId_; fflush(0);
   std::cout<<"\npath " << path_; fflush(0);
   std::cout<<"\ninputFile" << inputFile_; fflush(0);
   std::cout<<"\ntempOutput " << tempOutput_; fflush(0);
   std::cout<<"\noutputFile " << outputFile_; fflush(0);
}

void CmdLine::setOutputNames()
{
   tempOutput_[0] = '\0';
   outputFile_[0] = '\0';

//   strcat( tempOutput_, path_ );
   strcat( tempOutput_, "partialSolution.xml" );
   std::string tempOutputFile = tempOutput_;

//   strcat( outputFile_, path_ );
   strcat( outputFile_, "output" );
   strcat( outputFile_, argv_[ 1 ] );
   strcat( outputFile_, "F" );
   if ( inputId_!=0 )
   {
	   stringstream ss;
	   ss << inputId_;
	   strcat( outputFile_, "_id" );
	   strcat( outputFile_, ss.str().c_str() ); 
   }
}

void CmdLine::setTipoSolver()
{
   bool load = false;
   bool execHeur = checkExecHeuristica();

   int idx;
   load = loadSolucaoInicial(idx);
   
   if ( load && execHeur )
	   tipoSolver_ = TipoSolver::LOAD_AND_EXEC_HEUR;
   else if ( load )
	   tipoSolver_ = TipoSolver::LOAD_HEUR;
   else if ( execHeur )
	   tipoSolver_ = TipoSolver::EXEC_HEUR;   
   else
	   tipoSolver_ = TipoSolver::MIP;
}

char* CmdLine::getPath( char* path ) const
{
	strcpy(path,path_);
	return path;
}

char* CmdLine::getInputName( char* inputFile ) const
{ 
	strcpy(inputFile,inputFile_); 
	return inputFile;
}

char* CmdLine::getTempOutputName( char* tempOutput ) const
{
	strcpy(tempOutput,tempOutput_); 
	return tempOutput;
}

char* CmdLine::getOutputName( char* outputFile ) const
{
	strcpy(outputFile,outputFile_); 
	return outputFile;
}

// [CARREGAMENTO DE ARGUMENTOS OPCIONAIS]

// procurar argumento na lista e retornar indice
int CmdLine::findArg(char* argFind)
{
	int idx = -1;
	bool found = false;
	string argF (argFind);
	string argStr;
	for(int i = 0; i < argc_; i++)
	{
		argStr = argv_[i];
		if(argF.compare(argStr) == 0)
		{
			idx = i;
			found = true;
			break;
		}
	}

	return idx;
}

// tenta carregar o input id caso tenha sido inserido
bool CmdLine::findInputId(int &inputId)
{
	inputId = 0;
	int idx = findArg( "-id" );

	if((idx < 0) || (idx >= argc_))
	{
		cout << "argumento '-id' nao encontrado" << endl;
		return false;
	}

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc_ )
	{
		cout << "id nao inserido depois de -id" << endl;
		return false;
	}

	// converter para inteiro
	string arg = argv_[idx];
	inputId = std::atoi(arg.c_str());

	return true;
}


// [HEURÍSTICA]

// --------- check args opcionais ---------------

// verifica se a heuristica deve ser executada de raíz e se foi fornecido algum parametro para o limite minimo de receita credito
bool CmdLine::checkExecHeuristica()
{
#ifdef DEFAULT_MIP_UNICO				// default mip único
	int idx = findArg( "-heurn" );
	bool found = (idx >= 0) && (idx < argc_);
	bool execHeur = found;				// encontrada '-heurn' -> executa heurística
#else									// default heuristica
	int idx = findArg( "-mip" );
	bool found = (idx >= 0) && (idx < argc_);
	bool execHeur = !found;				// não encontrada '-notHeurn' -> executa heurística
#endif	
	
	return (execHeur);
}

// verifica os argumentos da heuristica
void CmdLine::checkArgsHeuristica()
{
	// check minimo receita crédito
	checkMinReceitaCredito();
	// check tempo lim MIP
	checkTempoLimMIPs();
	// check mudar salas MIP
	checkMudarSalasMIP();
	// check desligar mínimo interno
	checkLigarMinAlunosInterno();

	// verificar se so se deve tentar mudar as salas (só improve)
	checkSoMudarSalas();	
}

// verificar se foi passado um minimo de receita crédito
bool CmdLine::checkMinReceitaCredito()
{
	int idx = findArg( "-lrc" );
	if(idx < 0 || idx >= argc_)
		return false;

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc_ )
	{
		HeuristicaNuno::logMsg("[ARG-HEUR] valor de receita credito nao inserido depois de -lrc", 0);
		return false;
	}

	std::string arg = argv_[idx];
	double limRecCred = std::atof(arg.c_str());
	if(limRecCred >= 0.0)
	{
		ParametrosHeuristica::minProdutividadeCred = limRecCred;
		HeuristicaNuno::logMsgInt("[ARG-HEUR] min receita credito MIP inserido: ", (int) limRecCred, 0);
		return true;
	}

	return false;
}

// verificar se foi passado um tempo limite para os MIPs
bool CmdLine::checkTempoLimMIPs()
{
	int idx = findArg( "-t" );
	if(idx < 0 || idx >= argc_)
		return false;

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc_ )
	{
		HeuristicaNuno::logMsg("[ARG-HEUR] nao foi introduzido limite de tempo depois de -t", 0);
		return false;
	}

	std::string arg = argv_[idx];
	int lim = std::atoi(arg.c_str());
	if(lim >= 0)
	{
		ParametrosHeuristica::timeLimitMIP = lim;
		HeuristicaNuno::logMsgInt("[ARG-HEUR] tempo limit MIPs inserido (seg): ", lim, 0);
		return true;
	}
	
	return false;
}

// verificar se foi passado parametro sobre mudança de salas
bool CmdLine::checkMudarSalasMIP()
{
	int idx = findArg( "-semSalaMip" );
	if(idx < 0 || idx >= argc_)
		return false;

	HeuristicaNuno::logMsg("[ARG-HEUR] MIP realocar alunos setado para nao realocar salas.", 0);
	ParametrosHeuristica::mipRealocSalas = false;
	return true;
}

// verificar se foi passado parâmetro para desligar minimo de alunos interno
bool CmdLine::checkLigarMinAlunosInterno()
{
	int idx = findArg( "-onMin" );
	if(idx < 0 || idx >= argc_)
		return false;

	HeuristicaNuno::logMsg("[ARG-HEUR] Ligar mínimo de alunos interno.", 0);
	ParametrosHeuristica::desligarMinAlunosInterno = false;
	return true;
}

// verificar se está em modo realocar salas de solução fixada
bool CmdLine::checkSoMudarSalas()
{
	int idx = findArg( "-rsalas" );
	if(idx < 0 || idx >= argc_)
		return false;

	HeuristicaNuno::soRealocSalas = true;

	// O Path para as indisponibilidades extra da sala
	++idx;
	if(idx >= argc_ )
	{
		HeuristicaNuno::logMsg("[ARG-HEUR] nao foi introduzido o caminho para as indisponibilidade extra das salas", 0);
		return true;
	}

	char fullPath [1024];
	fullPath[0] = '\0';
	strcat(fullPath, path_);
	strcat(fullPath, argv_[idx]);
	HeuristicaNuno::loadIndispExtraSalas(fullPath);
	
	return true;
}

// verifica se foi setado um valor para o relaxamento do mínimo de alunos na fase ilegal
bool CmdLine::checkRelaxMinAlunos()
{
	int idx = findArg( "-rlx" );
	if(idx < 0 || idx >= argc_)
		return false;

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc_ )
	{
		HeuristicaNuno::logMsg("[ARG-HEUR] Nao foi introduzido valor de relaxamento depois de -rlx", 0);
		return false;
	}

	std::string arg = argv_[idx];
	double lim = std::atof(arg.c_str());
	if(lim > 0 && lim < 1)
	{
		ParametrosHeuristica::relaxMinAlunosTurma = lim;
		HeuristicaNuno::logMsgInt("[ARG-HEUR] relaxamento minimo de alunos inserido: ", (int) lim, 0);
		return true;
	}
	else
	{
		HeuristicaNuno::logMsgInt("[ARG-HEUR] relaxamento minimo de alunos tem que ter um valor entre 0 e 1 (exclusivo). Valor inserido: ", (int)lim,  1);
	}
	
	return false;
}

// tenta carregar solução inicial caso tenha sido inserida
bool CmdLine::loadSolucaoInicial( int &idx )
{
	HeuristicaNuno::logMsg("", 0);
	HeuristicaNuno::logMsg("Tentar carregar solucao inicial", 0);

	idx = findArg("-load");
	if((idx < 0) || (idx >= argc_))
	{
		HeuristicaNuno::logMsg("argumento '-load' nao encontrado", 0);
		return false;
	}

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc_ )
	{
		HeuristicaNuno::warning("main::loadSolucaoInicial", "Path nao inserido depois de load");
		return false;
	}

	return true;
}