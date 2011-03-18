// OptFramework.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <cstdio>
#include <exception>
#include "signal.h"
#include "ProblemSolution.h"
#include "ProblemData.h"
#include "ProblemDataLoader.h"
#include "SolverMIP.h"
#include "ErrorHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// >>>
//GGroup<int/*ids de campus existentes*/> * AtendimentoCampus::campi_id = new GGroup<int>;

//GGroup<int/*ids de unidades existentes*/> * AtendimentoUnidade::__ids_cadastrados = new GGroup<int>;

//GGroup<int/*ids de salas existentes*/> * AtendimentoSala::__ids_cadastrados = new GGroup<int>;

//std::vector<GGroup<int/*ids das salas que possuem o dia (dado pela posicao do vector)*/> > * AtendimentoDiaSemana::__ids_cadastrados = new std::vector<GGroup<int> >(7);
// <<<

#ifndef PATH_SEPARATOR
#ifdef WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif
#endif

struct _dtOutput
{ 
   ProblemSolution* solution;
   char tempOutput[1024];
   char outputFile[1024];
} dtOutput;

void _signals();
void _tprocesshandler(int _code);
void writeOutput(ProblemSolution* solution, char* outputFile, char* tempOutput);

int main(int argc, char** argv)
{
   srand((unsigned int)time(NULL));

   _signals();

	char path[1024];
	char inputFile[1024];
	char tempOutput[1024];
	char outputFile[1024];
	bool error;

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

	dtOutput.solution = solution;
    strcpy(dtOutput.outputFile, outputFile);
    strcpy(dtOutput.tempOutput, tempOutput);

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
		//delete dataLoader;

		
		try
        {
			// solve the problem
			//solver = new SolverMIP(data);
         solver = new SolverMIP(data,solution,dataLoader);
			solver->solve();
			solver->getSolution(solution);
			delete solver;
         delete dataLoader;
		}
		catch(int& status)
		{
			char mensagem[200];
			sprintf(mensagem, "Não foi possível processar o modelo matemático (erro %d)", status);
            ErrorHandler::addErrorMessage(UNEXPECTED_ERROR, ";;;;;;"+std::string(mensagem), "Solver::main()", false);
            error = true;
       }

		 try
         {
            writeOutput(solution, outputFile, tempOutput);
         }
         catch (int& status)
         {
            char mensagem[200];
            sprintf(mensagem, "Não foi possível escrever a solução. Error code: %d.", status);
            ErrorHandler::addErrorMessage(UNEXPECTED_ERROR, std::string(mensagem), "Solver::main()", false);
            error = true;
         }
		
		
		/*
		// solve the problem
		solver = new SolverMIP(data);
		solver->solve();
		solver->getSolution(solution);
		delete solver;
		*/

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
/*
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
*/
	if( error )
		return 0;
	return 1;
}

void writeOutput(ProblemSolution* solution, char* outputFile, char* tempOutput)
{
	//Write output
	remove(tempOutput);
	std::ofstream file;
	file.open(tempOutput);
	file << *solution;
	file.close();

	remove(outputFile);
	rename(tempOutput,outputFile);
}

void _signals()
{
   signal(SIGINT, _tprocesshandler);         //  (2) interrupt
   signal(SIGILL, _tprocesshandler);         //  (4) illegal instruction - invalid function image
   signal(SIGABRT_COMPAT, _tprocesshandler); //  (6) abnormal termination triggered by abort call
   signal(SIGFPE, _tprocesshandler);         //  (8) floating point exception
   signal(SIGSEGV, _tprocesshandler);        // (11) segment violation
   signal(SIGTERM, _tprocesshandler);        // (15) software termination signal from kill
   signal(SIGBREAK, _tprocesshandler);       // (21) Ctrl-Break sequence
   signal(SIGABRT, _tprocesshandler);        // (22) abnormal termination triggered by abort call
}

void _tprocesshandler(int _code)
{
   printf("O programa foi finalizado com o código (%d).\n", _code);
   string mensagem = "O programa foi finalizado com o código";
   ErrorHandler::addErrorMessage(UNEXPECTED_ERROR, ";;;;;;"+mensagem, "Solver::main()", false);
   writeOutput(dtOutput.solution, dtOutput.outputFile, dtOutput.tempOutput);
   exit(_code);
}
