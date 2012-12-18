// OptFramework.cpp : Defines the entry point for the console application.

#include <fstream>
#include <cstdio>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "signal.h"
#include "ProblemSolution.h"
#include "ProblemData.h"
#include "ProblemDataLoader.h"
#include "SolverMIP.h"
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

int main( int argc, char** argv )
{
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

   std::cout << "SEED: " << seed << std::endl;

   FILE * seedFile = fopen("./SEEDS.txt", "a");
   fprintf( seedFile, "%d\n", seed );
   if ( seedFile )
   {
      fclose( seedFile );
   }

   _signals();

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

   outTestFile << "Reading path..." <<endl;

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
   ProblemData * data = new ProblemData( argv[ 1 ] );
   ProblemSolution * solution;
   Solver * solver;

   outTestFile << "dataLoader constructor..." <<endl;

   dataLoader = new ProblemDataLoader( inputFile, data );
	
   outTestFile << "Loading data..." <<endl;

   dataLoader->load();

   outTestFile << "Solution constructor..." <<endl;

   solution = new ProblemSolution( data->parametros->modo_otimizacao == "TATICO" );

   outTestFile << "Creating temporary output file name..." << endl;

   // Temporary output file name
   strcat( tempOutput, path );
   strcat( tempOutput, "partialSolution.xml" );
   std::string tempOutputFile = tempOutput;

   outTestFile << "Creating output file name..." <<endl;

   // Output file name
   strcat( outputFile, path );
   strcat( outputFile, "output" );
   strcat( outputFile, argv[ 1 ] );
   strcat( outputFile, "F" );

   dtOutput.solution = solution;
   strcpy( dtOutput.outputFile, outputFile );
   strcpy( dtOutput.tempOutput, tempOutput );

   if( argc > 3 )
   {
      // Read other parameters
   }

#ifndef DEBUG
   try
   {
#endif
      try
      {
		 outTestFile << "Creating SolverMIP..." <<endl;
       
		 // Solve the Problem
         solver = new SolverMIP( data, solution, dataLoader );
		 
		 outTestFile << "Solving..." <<endl;
		 
		 solver->solve();
		 		 
#ifdef LEAK_MEMORY
#ifdef DEBUG
		 _CrtDumpMemoryLeaks();
#endif
#endif
		 outTestFile << "Deleting data, solver and dataLoader..." <<endl;

		 delete data;
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

   if ( error )
   {
      return 0;
   }

   	outTestFile << "Finished!" <<endl;
	outTestFile.close();

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
