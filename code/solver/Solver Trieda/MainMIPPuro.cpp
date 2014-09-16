#include <cstdio>

#include "MainMIPPuro.h"
#include "SolverMIPUnico.h"
#include "SolverMIP.h"
#include "CmdLine.h"

// ----------------------------------------------------------------------------------------------
// MAIN HEURÍSTICA
int mainMIPPuro( CmdLine *cmd, ProblemData* data, ProblemDataLoader* dataLoader, ProblemSolution* &solution )
{
	int status = 0;
	try
	{
		initSolution( solution, data );

		Solver * solver = new SolverMIPUnico( data, solution, dataLoader );

		status = solver->solve();

		if ( solver )
		{
			delete solver;
			solver = nullptr;
		}
	}
	catch( std::exception & e )
	{
		if( ErrorHandler::getErrorMessages().size() == 0 )
		{
			std::string message = "Ocorreu um erro interno no resolvedor.";
			
			ErrorHandler::addErrorMessage(
				 UNEXPECTED_ERROR, message, "mainMIPPuro.cpp", true );
		}

		cout << "\n\nERROR: " << e.what() << endl;
		status = 0;
	}

	return status;
}

// ----------------------------------------------------------------------------------------------

void initSolution( ProblemSolution*& solution, ProblemData* data )
{
	if ( !solution )
	{
		solution = new ProblemSolution( data->parametros->modo_otimizacao == "TATICO" );
		solution->setCenarioId( data->getCenarioId() );
	}
	else
	{
		delete solution;
		solution = new ProblemSolution( data->parametros->modo_otimizacao == "TATICO" );
		solution->setCenarioId( data->getCenarioId() );
	}	
}