// OptFramework.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <cstdio>
#include <exception>
#include "ProblemSolution.h"
#include "ProblemData.h"
#include "ProblemDataLoader.h"
#include "SolverMIP.h"
#include "ErrorHandler.h"

#ifndef PATH_SEPARATOR
#ifdef WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif
#endif

int main(int argc, char** argv)
{
   char path[1024];
   char inputFile[1024];
   char tempOutput[1024];
   char outputFile[1024];
   bool error;
   std::ofstream file;

   ProblemDataLoader* dataLoader;
   ProblemData *data = new ProblemData();
   ProblemSolution* solution = new ProblemSolution();
   Solver* solver;

   //Initializations
   path[0] = '\0';
   inputFile[0] = '\0';
   tempOutput[0] = '\0';
   outputFile[0] = '\0';
   error = false;

   //Check command line
   if( argc <= 2 )
   {
      if ( argc == 2 && strcmp(argv[1],"-version") == 0 )
      {
         printf("%s_%s\n",__DATE__,__TIME__);
         return 0; 
      }
      else
      {
         printf("Invalid parameters in command line.\n");
         printf("Usage: solver <instanceID> <path>\n");
         return 0;
      }
   }

   //Read path
   strcat(path,argv[2]);
   strcat(path,PATH_SEPARATOR);

   //Input file name
   strcat(inputFile,path);
   strcat(inputFile,"input");
   strcat(inputFile,argv[1]);

   //Temporary output file name
   strcat(tempOutput,path);
   strcat(tempOutput,"partialSolution.xml");
   std::string tempOutputFile = tempOutput;

   //Output file name
   strcat(outputFile,path);
   strcat(outputFile,"output");
   strcat(outputFile,argv[1]);
   strcat(outputFile,"F");

   if( argc > 3 )
   {
      //Read other parameters
   }

#ifndef DEBUG
   try
   {
#endif
      //Load data
      dataLoader = new ProblemDataLoader(inputFile, data);   
      dataLoader->load();
      delete dataLoader;

      // solve the problem
      solver = new SolverMIP(data);
      solver->solve();
      solver->getSolution(solution);
      delete solver;

#ifndef DEBUG
   }
   catch( std::exception& e )
   {
      if( ErrorHandler::getErrorMessages().size() == 0 )
         ErrorHandler::addErrorMessage( UNEXPECTED_ERROR, "Ocorreu um erro interno no resolvedor.", "main.cpp", true );
      printf("\n\nERROR: %s\n",e.what());
      error = true;
   }
#endif

   //Write output
   try
   {
      remove(tempOutput);

      file.open(tempOutput);
      file << *solution;
      file.close();

      remove(outputFile);
      rename(tempOutput,outputFile);
   }
   catch( std::exception& e )
   {
      printf("\n\nAn error occurred during creation of output file.\n",e.what());
      printf("ERROR: %s\n",e.what()); 
      error = true;
   }

   if( error )
      return 0;
   return 1;
}

