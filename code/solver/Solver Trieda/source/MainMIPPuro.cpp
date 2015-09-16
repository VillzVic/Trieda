#include <cstdio>
#include <iostream>

#include "MainMIPPuro.h"
#include "SolverMIPUnico.h"
#include "CmdLine.h"
#include "ProblemSolution.h"
#include "ProblemData.h"
#include "ProbDataAnalyzer.h"

#include "ErrorHandler.h"

// ----------------------------------------------------------------------------------------------
// MAIN HEURÍSTICA
int mainMIPPuro( CmdLine *cmd, ProblemData* data, ProblemDataLoader* dataLoader, ProblemSolution* &solution )
{
	int status = 0;
	try
	{
		ProbDataAnalyzer::estatisticasDemandasEscola();

		initSolution( solution, data );
		
		ProblemSolution * probSolInic;
		cmd->loadInputSolucaoInicial(probSolInic);

		Solver * solver = new SolverMIPUnico(data, solution, dataLoader, probSolInic);

		status = solver->solve();

		if (solver)
		{
			delete solver;
			solver = nullptr;
		}
		if (probSolInic)
		{
			delete probSolInic;
			probSolInic = nullptr;
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
	if (solution){
		delete solution;
		solution = nullptr;
	}

	solution = new ProblemSolution( data->parametros->modo_otimizacao == "TATICO" );
	solution->setCenarioId( data->getCenarioId() );
}
