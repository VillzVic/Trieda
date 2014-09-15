// OptFramework.cpp : Defines the entry point for the console application.

//#pragma warning(disable:4503)

#include <fstream>
#include <cstdio>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>

#include "signal.h"
#include "ProblemSolution.h"
#include "ProblemData.h"
#include "ProblemDataLoader.h"
#include "CentroDados.h"
#include "Indicadores.h"

#ifndef HEURISTICA
#ifndef MIP_UNICO
#include "SolverMIP.h"
#include "SolverTaticoHeur\Instance.h"
#include "SolverTaticoHeur\SolverTaticoHeur.h"
#endif
#endif

#ifdef MIP_UNICO
#include "SolverMIPUnico.h"
#endif


#include "HeurNuno\HeuristicaNuno.h"
#include "HeurNuno\\ParametrosHeuristica.h"

#include "CPUTimerWin.h"

#include "ErrorHandler.h"

//#define LEAK_MEMORY

#ifdef LEAK_MEMORY
#ifdef DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif


#ifndef PATH_SEPARATOR
#ifdef WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif
#endif

struct _dtOutput
{ 
   ProblemSolution * solution;
   char tempOutput[ 1024 ];
   char outputFile[ 1024 ];
} dtOutput;

void _signals();
void _tprocesshandler( int );
void writeOutput( ProblemSolution *, char *, char * );
bool getInputId( int argc, char** argv, int &inputId);
int findArg( int argc, char** argv, char* argFind);

bool checkExecHeuristica( int argc, char** argv);
void checkArgsHeuristica( int argc, char** argv, char* const path);
bool checkMinReceitaCredito( int argc, char** argv );
bool checkTempoLimMIPs( int argc, char** argv );
bool checkMudarSalasMIP( int argc, char** argv );
bool checkLigarMinAlunosInterno( int argc, char** argv );
bool checkRelaxMinAlunos( int argc, char** argv );
// so realocar salas de solucao carregada
bool checkSoMudarSalas( int argc, char** argv, char* const path );

bool runHeuristica(char* path, char** argv, char* outputFile, char* tempOutput, ofstream &outTestFile, 
					ProblemSolution* &solucaoHeurExt);
bool loadSolucaoInicial( int argc, char** argv, char* const path, ProblemSolution* &solucao );
void printSolucao(char* path, char* outputFile, char* tempOutput, ofstream &outTestFile, ProblemSolution* const solucao);

// MAIN
int main( int argc, char** argv )
{
	CPUTimer timer;
	double dif = 0.0;
	timer.start();
	
	ofstream outTestFile;
	char outputTestFilename[] = "outTest.txt";
	outTestFile.open(outputTestFilename, ios::out);
	if (!outTestFile) {
		cerr << "Can't open output file " << outputTestFilename << endl;
		exit(1);
	}
	outTestFile << "Started..." <<endl;
	
   //unsigned seed = time(NULL);
   unsigned seed = 1305057265;
   srand( seed );

   outTestFile << "Opening the seedFile..." <<endl;

#ifdef TESTE
   std::cout << "\nTESTE\n";   
#elif KROTON
   std::cout << "\nKROTON\n";
#elif UNIT
   std::cout << "\nUNIT\n";
#elif UNIRITTER
   std::cout << "\nUNIRITTER\n";
#else
   std::cout << "\nESTACIO\n";
#endif

   std::cout << "SEED: " << seed << std::endl;

   FILE * seedFile = fopen("./SEEDS.txt", "a");
   fprintf( seedFile, "%d\n", seed );
   if ( seedFile )
   {
      fclose( seedFile );
   }

   _signals();

   int inputId;
   char path[ 1024 ];
   char inputFile[ 1024 ];
   char tempOutput[ 1024 ];
   char outputFile[ 1024 ];
   bool error;

   // Initializations
   path[0] = '\0';
   inputFile[0] = '\0';
   tempOutput[0] = '\0';
   outputFile[0] = '\0';
   error = false;

   outTestFile << "Checking arguments..." <<endl;
   std::cout << "Arguments[" << argc << "]: ";
   for (int i=0; i<argc; i++)
   {
      std::cout << argv[i] << " ";
   }
   std::cout << "\n";

   // Check command line
   if( argc <= 2 )
   {
      if ( argc == 2 && strcmp( argv[1], "-version" ) == 0 )
      {
         printf( "%s_%s\n", __DATE__, __TIME__ );

         return 0;
      }
      else
      {
         printf( "Invalid parameters in command line.\n" );
         printf( "Usage: solver <instanceID> <path>\n" );

         return 0;
      }
   }
#ifndef HEURISTICA
   #ifndef MIP_UNICO
   else if ( strcmp( argv[1], "-heur" ) == 0 )
   {
      Instance* instance = new Instance();
      std::string filename = argv[2];
      std::cout << "Carregando a instancia do modulo tatico com horarios (heuristica)...\n";
      instance->readInstance(filename);

      std::cout << "Criando o solver heuristico...\n";
      SolverTaticoHeur* solverHeur = new SolverTaticoHeur(NULL, NULL);
      solverHeur->setInstance(instance);
      solverHeur->enableTestingPhase();

      double timeLimit = 100000.0;

      std::cout << "Otimizando...\n";
      solverHeur->useRandomSeed();
      solverHeur->solve(timeLimit);

      std::cout << "Concluido.\n";
      return 0;
   }
   #endif
#endif

   outTestFile << "Reading path..." <<endl;

   // Input Id
   getInputId(argc, argv, inputId);

   // Read path
   strcat( path, argv[2] );
   strcat( path, PATH_SEPARATOR );

   outTestFile << "Reading inputFile..." <<endl;

   // Input file name
   strcat( inputFile, path );
   strcat( inputFile, "input" );
   strcat( inputFile, argv[ 1 ] );

   std::cout << inputFile << std::endl;

   ProblemDataLoader * dataLoader;
   ProblemSolution* solution;

   ProblemData* data = new ProblemData( argv[ 1 ], inputId );
   CentroDados::setProblemData(data);
   CentroDados::openFilesWarnError();

   std::stringstream indicaInputPlusId;
   indicaInputPlusId << "indicadores_" << argv[ 1 ];
   if ( inputId!=0 ) indicaInputPlusId << "_id" << inputId;
   Indicadores::setIndicadorFileName( indicaInputPlusId.str() );

#ifndef HEURISTICA
   Solver * solver;
#endif

   outTestFile << "dataLoader constructor..." <<endl;
   dataLoader = new ProblemDataLoader( inputFile, data );
	
   outTestFile << "Loading data..." <<endl;
   dataLoader->load();

   outTestFile << "Creating temporary output file name..." << endl;
   strcat( tempOutput, path );
   strcat( tempOutput, "partialSolution.xml" );
   std::string tempOutputFile = tempOutput;

   outTestFile << "Creating output file name..." <<endl;
   strcat( outputFile, path );
   strcat( outputFile, "output" );
   strcat( outputFile, argv[ 1 ] );
   strcat( outputFile, "F" );
   if ( inputId!=0 )
   {
	   stringstream ss;
	   ss << inputId;
	   strcat( outputFile, "_id" );
	   strcat( outputFile, ss.str().c_str() ); 
   }
  
   // Solução da heuristica
   ProblemSolution* solucaoHeurExt = nullptr;
   
   // Teste Heurística Nuno
   bool execHeur = false;
   execHeur = checkExecHeuristica(argc, argv);

   // Tentar carregar solução inicial
   bool load = false;
   load = loadSolucaoInicial(argc, argv, path, solucaoHeurExt);
   if(load && solucaoHeurExt != nullptr)
   {
		cout << ">>> Solução inicial carregada com sucesso!" << endl; fflush(0);
		
		if (!execHeur)
		{
			solucaoHeurExt->computaMotivos(true,false);	// motivoUsoPV=false: professores virtuais ainda não lidos pelo load da solução
			printSolucao(path, outputFile, tempOutput, outTestFile, solucaoHeurExt);

			// limpar estruturas e retornar
			cout << "limpar solucao inicial" << endl; fflush(0);
			delete solucaoHeurExt;
			solucaoHeurExt = nullptr;
			cout << "limpar data" << endl; fflush(0);
			data = nullptr;
			CentroDados::clearProblemData();
			cout << "limpar data loader" << endl; fflush(0);
			delete dataLoader;
		
			std::cout << "\n\nTRIEDA executado com sucesso !!!\n";
			return 1;
		}
   }
   // executar heurística. se a solução foi carregada, usá-la como solução inicial
   if (execHeur)
   {
	   // setup heurística
		HeuristicaNuno::setInputInfo(path, inputFile, inputId);
		HeuristicaNuno::setup(CentroDados::getProblemData());

	   // carregar solução inicial
		if(solucaoHeurExt != nullptr)
			HeuristicaNuno::loadSolucao(solucaoHeurExt);

	    // load argumentos opcionais heuristica
	    checkArgsHeuristica( argc, argv, path );

		// correr a heurística
		ProblemSolution* solucaoHeuristica = nullptr;
		runHeuristica(path, argv, outputFile, tempOutput, outTestFile, solucaoHeuristica);

		// Print solução
		if(solucaoHeuristica != nullptr)
		{
			solucaoHeurExt = solucaoHeuristica;
			solucaoHeurExt->computaMotivos(true,true);
			printSolucao(path, outputFile, tempOutput, outTestFile, solucaoHeuristica);
		}
		// else: se nao tiver encontrado solução passar a inicial (se houver).

	//#ifdef HEURISTICA

		// limpar estruturas e retornar
		cout << "limpar solucao inicial" << endl;
		delete solucaoHeurExt;
		solucaoHeurExt = nullptr;
		cout << "limpar data" << endl;
		data = nullptr;
		CentroDados::clearProblemData();
		cout << "limpar data loader" << endl;
		delete dataLoader;
		
		timer.stop();
		dif = timer.getCronoCurrSecs();				// total em seg
		int hours = (int) (dif / 3600);				// h
		int min = (int) ( (int) dif % 3600 ) / 60;	// min
   
		stringstream runtime;
		runtime << "\nTotal elapsed time: " << hours << "h" << min;
		Indicadores::printSeparator(2);
		Indicadores::printIndicador( runtime.str() );

   		outTestFile << "Finished!" <<endl;
		outTestFile.close();
		CentroDados::closeFilesWarnError();

		std::cout << "\n\nTRIEDA executado com sucesso !!!\n";

		return 1;

	//#endif
   }

   bool solucaoHeuristica = false;
   if ( solucaoHeurExt == nullptr )
   {
	   outTestFile << "Solution constructor..." <<endl;
	   solution = new ProblemSolution( data->parametros->modo_otimizacao == "TATICO" );
	   solution->setCenarioId( data->getCenarioId() );
   }
   else
   {
	   solution = solucaoHeurExt;
	   solucaoHeuristica=true;
   }

   dtOutput.solution = solution;

   // -----------------------------------------------------------------------------------------
   // Redefine o nome dos outputs, pós possivelmente alterado em heuristica
   outTestFile << "Creating final output file name..." << endl;

   tempOutput[0] = '\0';
   //strcat( tempOutput, path ); // comentado para o output ficar na mesma pasta que os logs
   strcat( tempOutput, "partialSolution.xml" );
   tempOutputFile = tempOutput;
   
   outputFile[0] = '\0';
   //strcat( outputFile, path ); // comentado para o output ficar na mesma pasta que os logs
   strcat( outputFile, "output" );
   strcat( outputFile, argv[ 1 ] );
   strcat( outputFile, "F" );
   if ( inputId!=0 )
   {
	   stringstream ss;
	   ss << inputId;
	   strcat( outputFile, "_id" );
	   strcat( outputFile, ss.str().c_str() ); 
   }

   strcpy( dtOutput.outputFile, outputFile );
   strcpy( dtOutput.tempOutput, tempOutput );
   // -----------------------------------------------------------------------------------------
   

   if( argc > 3 )
   {
      // Read other parameters
   }

#ifndef DEBUG
   try
   {
#endif
#ifndef HEURISTICA
      try
      {	 
		#ifndef MIP_UNICO
		 outTestFile << "Creating SolverMIP..." <<endl;
		 solver = new SolverMIP( data, solution, dataLoader, solucaoHeuristica );
		 
		 outTestFile << "Solving..." <<endl;		 
		 solver->solve();		 		 
		#else
		 outTestFile << "Creating SolverMIPUnico..." <<endl;
		 solver = new SolverMIPUnico( data, solution, dataLoader );
		 
		 outTestFile << "Solving..." <<endl;		 
		 solver->solve();
		#endif

#ifdef LEAK_MEMORY
#ifdef DEBUG
		 _CrtDumpMemoryLeaks();
#endif
#endif
		 outTestFile << "Deleting data, solver and dataLoader..." <<endl;

		 //delete data;
		 data = nullptr;
		 CentroDados::clearProblemData();
         delete solver;
         delete dataLoader;
      }
      catch( int & status )
      {
		 outTestFile << "Catching first inner exception..." <<endl;

         char mensagem[ 200 ];
         sprintf( mensagem, "Não foi possível processar o modelo matemático ( erro %d )", status );

         ErrorHandler::addErrorMessage(
            UNEXPECTED_ERROR, std::string( mensagem ), "Solver::main()", false );

         error = true;
      }

#endif

	   // Write output
      try
      {
		 outTestFile << "Writing Output..." <<endl;

         writeOutput( solution, outputFile, tempOutput );
      }
      catch ( int & status )
      {
		  outTestFile << "Catching second inner exception..." <<endl;

         char mensagem[ 200 ];

         sprintf( mensagem, "Não foi possível escrever a solução. Error code: %d.", status );

         ErrorHandler::addErrorMessage(
			   UNEXPECTED_ERROR, std::string( mensagem ), "Solver::main()", false );

         error = true;
      }

#ifndef DEBUG
   }
   catch( std::exception & e )
   {
	  outTestFile << "Catching external exception..." <<endl;
      
	  if( ErrorHandler::getErrorMessages().size() == 0 )
	   {
		    std::string message = "Ocorreu um erro interno no resolvedor.";

          ErrorHandler::addErrorMessage(
             UNEXPECTED_ERROR, message, "main.cpp", true );
	   }

      printf( "\n\nERROR: %s\n", e.what() );
      error = true;
   }
#endif

   delete solution;
  
   if ( error )
   {
      return 0;
   }    

   timer.stop();
   dif = timer.getCronoCurrSecs();				// total em seg
   int hours = (int) (dif / 3600);				// h
   int min = (int) ( (int) dif % 3600 ) / 60;	// min
   
   outTestFile << "\nTotal elapsed time: " << hours << "h" << min << endl << endl;
   std::cout << "\nTotal elapsed time: " << hours << "h" << min << endl << endl;
   
   	outTestFile << "Finished!" <<endl;
	outTestFile.close();

	CentroDados::closeFilesWarnError();

   std::cout << "\n\nTRIEDA executado com sucesso !!!\n";
   
   return 1;
}

void writeOutput( ProblemSolution * solution,
   char * outputFile, char * tempOutput )
{
   // Write output
   remove( tempOutput );
   std::ofstream file;
   file.open( tempOutput );
   file << ( *solution );
   file.flush();
   file.close();

   remove( outputFile );
   rename( tempOutput, outputFile );
}

void _signals()
{
   signal( SIGINT, _tprocesshandler );            //  (2) interrupt
   signal( SIGILL, _tprocesshandler );            //  (4) illegal instruction - invalid function image
   // signal( SIGABRT_COMPAT, _tprocesshandler ); //  (6) abnormal termination triggered by abort call
   signal( SIGFPE, _tprocesshandler );            //  (8) floating point exception
   signal( SIGSEGV, _tprocesshandler );           // (11) segment violation
   signal( SIGTERM, _tprocesshandler );           // (15) software termination signal from kill
   // signal( SIGBREAK, _tprocesshandler );       // (21) Ctrl-Break sequence
   signal( SIGABRT, _tprocesshandler );           // (22) abnormal termination triggered by abort call
}

void _tprocesshandler( int _code )
{
   printf( "O programa foi finalizado com o código (%d).\n", _code );
   string mensagem = ( "O programa foi finalizado com o código" );

   ErrorHandler::addErrorMessage(
	   UNEXPECTED_ERROR, mensagem, "Solver::main()", false );

   writeOutput( dtOutput.solution,
      dtOutput.outputFile, dtOutput.tempOutput );

   exit( _code );
}


// [CARREGAMENTO DE ARGUMENTOS OPCIONAIS]

// procurar argumento na lista e retornar indice
int findArg( int argc, char** argv, char* argFind)
{
	int idx = -1;
	bool found = false;
	string argF (argFind);
	string argStr;
	for(int i = 0; i < argc; i++)
	{
		argStr = argv[i];
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
bool getInputId( int argc, char** argv, int &inputId)
{
	cout << "tentar carregar input id" << endl;

	inputId = 0;
	int idx = findArg( argc, argv, "-id" );

	if((idx < 0) || (idx >= argc))
	{
		cout << "argumento '-id' nao encontrado" << endl;
		return false;
	}

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc )
	{
		cout << "id nao inserido depois de -id" << endl;
		return false;
	}

	// converter para inteiro
	string arg = argv[idx];
	inputId = std::atoi(arg.c_str());

	return true;
}

// tenta carregar solução inicial caso tenha sido inserida
bool loadSolucaoInicial( int argc, char** argv, char* const path, ProblemSolution* &solucao)
{
	HeuristicaNuno::logMsg("", 0);
	HeuristicaNuno::logMsg("Tentar carregar solucao inicial", 0);

	int idx = findArg(argc, argv, "-load");
	if((idx < 0) || (idx >= argc))
	{
		HeuristicaNuno::logMsg("argumento '-load' nao encontrado", 0);
		return false;
	}

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc )
	{
		HeuristicaNuno::warning("main::loadSolucaoInicial", "Path nao inserido depois de load");
		return false;
	}
	

	// ler solução (modoTatico = false!)
	solucao = nullptr;
	HeuristicaNuno::logMsg("tentar ler solucao", 0);
	char fullPath [1024];
	fullPath[0] = '\0';
	strcat(fullPath, path);
	strcat(fullPath, argv[idx]);
	solucao = ProblemSolution::lerSolucao(fullPath);
	if(solucao == nullptr)
	{
		HeuristicaNuno::warning("main::loadSolucaoInicial", "Problema na leitura da solucao.");
		return false;
	}

	return true;
}

// [HEURÍSTICA]

// --------- check args opcionais ---------------

// verifica se a heuristica deve ser executada de raíz e se foi fornecido algum parametro para o limite minimo de receita credito
bool checkExecHeuristica( int argc, char** argv )
{
	int idx = findArg( argc, argv, "-notHeurn" );

	#ifdef MIP_UNICO
		return false;
	#endif

	return (idx < 0) || (idx >= argc);	// não encontrada 'notHeurn' -> executa heurística
}

// verifica os argumentos da heuristica
void checkArgsHeuristica( int argc, char** argv, char* const path)
{
	// check minimo receita crédito
	checkMinReceitaCredito( argc, argv );
	// check tempo lim MIP
	checkTempoLimMIPs( argc, argv );
	// check mudar salas MIP
	checkMudarSalasMIP( argc, argv );
	// check desligar mínimo interno
	checkLigarMinAlunosInterno( argc, argv );

	// verificar se so se deve tentar mudar as salas (só improve)
	checkSoMudarSalas( argc, argv, path );	
}

// verificar se foi passado um minimo de receita crédito
bool checkMinReceitaCredito( int argc, char** argv )
{
	int idx = findArg( argc, argv, "-lrc" );
	if(idx < 0 || idx >= argc)
		return false;

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc )
	{
		HeuristicaNuno::logMsg("[ARG-HEUR] valor de receita credito nao inserido depois de -lrc", 0);
		return false;
	}

	std::string arg = argv[idx];
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
bool checkTempoLimMIPs( int argc, char** argv )
{
	int idx = findArg( argc, argv, "-t" );
	if(idx < 0 || idx >= argc)
		return false;

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc )
	{
		HeuristicaNuno::logMsg("[ARG-HEUR] nao foi introduzido limite de tempo depois de -t", 0);
		return false;
	}

	std::string arg = argv[idx];
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
bool checkMudarSalasMIP( int argc, char** argv )
{
	int idx = findArg( argc, argv, "-semSalaMip" );
	if(idx < 0 || idx >= argc)
		return false;

	HeuristicaNuno::logMsg("[ARG-HEUR] MIP realocar alunos setado para nao realocar salas.", 0);
	ParametrosHeuristica::mipRealocSalas = false;
	return true;
}

// verificar se foi passado parâmetro para desligar minimo de alunos interno
bool checkLigarMinAlunosInterno( int argc, char** argv )
{
	int idx = findArg( argc, argv, "-onMin" );
	if(idx < 0 || idx >= argc)
		return false;

	HeuristicaNuno::logMsg("[ARG-HEUR] Ligar mínimo de alunos interno.", 0);
	ParametrosHeuristica::desligarMinAlunosInterno = false;
	return true;
}

// verificar se está em modo realocar salas de solução fixada
bool checkSoMudarSalas( int argc, char** argv, char* const path )
{
	int idx = findArg( argc, argv, "-rsalas" );
	if(idx < 0 || idx >= argc)
		return false;

	HeuristicaNuno::soRealocSalas = true;

	// O Path para as indisponibilidades extra da sala
	++idx;
	if(idx >= argc )
	{
		HeuristicaNuno::logMsg("[ARG-HEUR] nao foi introduzido o caminho para as indisponibilidade extra das salas", 0);
		return true;
	}

	char fullPath [1024];
	fullPath[0] = '\0';
	strcat(fullPath, path);
	strcat(fullPath, argv[idx]);
	HeuristicaNuno::loadIndispExtraSalas(fullPath);
	
	return true;
}

// verifica se foi setado um valor para o relaxamento do mínimo de alunos na fase ilegal
bool checkRelaxMinAlunos( int argc, char** argv )
{
	int idx = findArg( argc, argv, "-rlx" );
	if(idx < 0 || idx >= argc)
		return false;

	// o path da solução tem que estar depois
	++idx;
	if(idx >= argc )
	{
		HeuristicaNuno::logMsg("[ARG-HEUR] Nao foi introduzido valor de relaxamento depois de -rlx", 0);
		return false;
	}

	std::string arg = argv[idx];
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

// -----------------------------------------------

// run heuristica
bool runHeuristica(char* path, char** argv, char* outputFile, char* tempOutput, ofstream &outTestFile, 
				ProblemSolution* &solucaoHeurExt)
{
	// run
	HeuristicaNuno::run();

	// imprimir output!
	HeuristicaNuno::logMsg("check se houve solucao", 1);
	if(HeuristicaNuno::solucaoHeur == nullptr)
	{
		HeuristicaNuno::logMsg("%%%%% Solução heurística não encontrada!!! %%%%", 1);
		return false;
	}

	// guardar a solução
	solucaoHeurExt = HeuristicaNuno::getSolucao();

	// limpar memória
	cout << "limpar heurnuno" << endl;
	HeuristicaNuno::clean();

	return true;
}

// print solucao
void printSolucao(char* path, char* outputFile, char* tempOutput, ofstream &outTestFile, ProblemSolution* const solucao)
{
	cout << "print solucao final!" << endl;
	//char sufixLimCred[1024];
	//sufixLimCred[0] = '\0';
	//std::sprintf(sufixLimCred, "%2.1f", ParametrosHeuristica::minProdutividadeCred);
	//strcat( outputFile, "_lim_" );
	//strcat( outputFile, sufixLimCred);

	// sufixo destruição!! (só usado em testes)
	if(ParametrosHeuristica::finalDestroy)
		strcat( outputFile, "_destroy_");
	
	outTestFile << "Writing Output..." << endl;
	writeOutput( solucao, outputFile, tempOutput );
}

