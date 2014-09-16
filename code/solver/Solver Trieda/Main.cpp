// OptFramework.cpp : Defines the entry point for the console application.

//#pragma warning(disable:4503)

#include <fstream>
#include <cstdio>
#include <exception>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <ctime>

#include "signal.h"
#include "ProblemSolution.h"
#include "ProblemData.h"
#include "ProblemDataLoader.h"
#include "CentroDados.h"
#include "Indicadores.h"
#include "CmdLine.h"
#include "MainHeuristicaIES.h"
#include "MainMIPPuro.h"

#include "CPUTimerWin.h"
#include "ErrorHandler.h"


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

void clearData( ProblemSolution * solution, ProblemData * data, ProblemDataLoader * dataLoader );
void seed();
void printIESDefine();
void _signals();
void _tprocesshandler( int );
bool writeOutput( ProblemSolution *&, char *, char * );

// MAIN
int main( int argc, char** argv )
{
    printIESDefine();
   	
	CPUTimer timer;
	double tempoSec = 0.0;
	timer.start();
	
	ofstream outTestFile;
	char outputTestFilename[] = "outTest.txt";
	outTestFile.open(outputTestFilename, ios::out);
	if (!outTestFile) {
		cerr << "Can't open output file " << outputTestFilename << endl;
		exit(1);
	}
	outTestFile << "Started..." <<endl;
	
	seed();

    _signals();
   
    // -----------------------------------------------------------
	// Lê linha de comando da execução
	CmdLine *cmd = new CmdLine(argc, argv);

	if ( ! cmd->checkNArgs() )
		return (EXIT_FAILURE);

	cmd->init();
	
   // -----------------------------------------------------------
   int inputId;
   char path[ 1024 ];
   char inputFile[ 1024 ];
   char tempOutput[ 1024 ];
   char outputFile[ 1024 ];
   
   inputId = cmd->getInputId();
   cmd->getPath(path);
   cmd->getInputName(inputFile);
   cmd->getTempOutputName(tempOutput);
   cmd->getOutputName(outputFile);   
   
   // -----------------------------------------------------------
   ProblemData* data = new ProblemData( argv[ 1 ], inputId );
   CentroDados::setProblemData(data);

   // -----------------------------------------------------------
   CentroDados::openFilesWarnError();
   
   // -----------------------------------------------------------
   std::stringstream indicaInputPlusId;
   indicaInputPlusId << "indicadores_" << argv[ 1 ];
   if ( inputId!=0 ) indicaInputPlusId << "_id" << inputId;
   Indicadores::setIndicadorFileName( indicaInputPlusId.str() );
   
   // -----------------------------------------------------------
   outTestFile << "dataLoader constructor..." <<endl;
   ProblemDataLoader * dataLoader;
   dataLoader = new ProblemDataLoader( inputFile, data );
   dataLoader->load();
  
   // -----------------------------------------------------------
   ProblemSolution* solution=nullptr;

   dtOutput.solution = solution;
   strcpy( dtOutput.outputFile, outputFile );
   strcpy( dtOutput.tempOutput, tempOutput );
   
   // -----------------------------------------------------------
   
   int statusOtimz = 0;
   int tipoSolver = cmd->getTipoSolver();

   if ( tipoSolver != CmdLine::MIP )
   {
	   // Heurística
	   outTestFile << "Heuristica..." <<endl; fflush(0);
	   statusOtimz = mainHeur( cmd, data, solution );
   }
   else
   {
	   // MIP Puro
	   outTestFile << "MIP Puro..." <<endl; fflush(0);
	   statusOtimz = mainMIPPuro( cmd, data, dataLoader, solution );
   }
   
   // -----------------------------------------------------------
   bool error;
   if ( statusOtimz )
   {	   
	   dtOutput.solution = solution;

	   if ( !solution )
		   CentroDados::printError("main"," Solucao null!");
	   else
		   error = writeOutput( solution, outputFile, tempOutput );
   }
   else error = true;

   // -----------------------------------------------------------

   clearData( solution, data, dataLoader );
   
   // -----------------------------------------------------------
   timer.stop();
   tempoSec = timer.getCronoCurrSecs();				// total em seg
   int hours = (int) (tempoSec / 3600);				// h
   int min = (int) ( (int) tempoSec % 3600 ) / 60;	// min
   int sec = tempoSec - (min*60 + hours*60*60);		// sec
   
   outTestFile << "\nTotal elapsed time: " << hours << "h" << min << "'" << sec << "''" << endl << endl;
   std::cout << "\nTotal elapsed time: " << hours << "h" << min << "'" << sec << "''" << endl << endl;
   
   	outTestFile << "Finished!" <<endl;
	outTestFile.close();

	CentroDados::closeFilesWarnError();
	
    // -----------------------------------------------------------
    
	if ( error )
	{
		std::cout << "\n\nError final!\n";
		return (EXIT_FAILURE);
	}

    std::cout << "\n\nTRIEDA executado com sucesso !!!\n";
   
    return (EXIT_SUCCESS);
}

void clearData( ProblemSolution * solution, ProblemData * data, ProblemDataLoader * dataLoader )
{
	std::cout << "\nClearData..."; fflush(0);

   // Delete solution
   if (solution)
   {
	   delete solution;
	   solution = nullptr;
   }

   // Clear dataLoader
   if (dataLoader)
   {
	   delete dataLoader;
	   dataLoader = nullptr;
   }

   // Clear data
   CentroDados::clearProblemData();
   data = nullptr;
}

void seed()
{
   //unsigned seed = time(NULL);
   unsigned seed = 1305057265;
   srand( seed );
   std::cout << "SEED: " << seed << std::endl;
   FILE * seedFile = fopen("./SEEDS.txt", "a");
   fprintf( seedFile, "%d\n", seed );
   if ( seedFile )
   {
      fclose( seedFile );
   }
}

void printIESDefine()
{
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
}

bool writeOutput( ProblemSolution *& solution,
   char * outputFile, char * tempOutput )
{
   bool error = false;
   try
   {
	   if ( !solution )
	   {
		   CentroDados::printError("main::writeOutput()"," Solucao null");
		   return false;
	   }

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
   catch ( int & status )
   {
        char mensagem[ 200 ];

        sprintf( mensagem, "Não foi possível escrever a solução. Error code: %d.", status );

        ErrorHandler::addErrorMessage(
			UNEXPECTED_ERROR, std::string( mensagem ), "Solver.Main::writeOutput()", false );

		error = true;
   }
   
   return error;
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