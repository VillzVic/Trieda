#include <fstream>
#include <cstdio>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>

#include "MainHeuristicaIES.h"

#include "HeurNuno\HeuristicaNuno.h"
#include "HeurNuno\\ParametrosHeuristica.h"

#include "CPUTimerWin.h"
#include "ErrorHandler.h"
#include "CentroDados.h"

// ----------------------------------------------------------------------------------------------
// MAIN HEUR�STICA
int mainHeur( CmdLine *cmd, ProblemData* data, ProblemSolution* &solution )
{
	CPUTimer timer;
	double dif = 0.0;
	timer.start();

   // Executa Heur�stica?
   bool execHeur = ( cmd->getTipoSolver() == CmdLine::EXEC_HEUR ||
					 cmd->getTipoSolver() == CmdLine::LOAD_AND_EXEC_HEUR );
   
   // Carrega solu��o inicial?
   bool load = ( cmd->getTipoSolver() == CmdLine::LOAD_HEUR ||
				 cmd->getTipoSolver() == CmdLine::LOAD_AND_EXEC_HEUR );

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

   bool novaSolucaoHeur = false;
   
   // -----------------------------------------------------------------------------------------
   // Carrega solu��o
   if ( load )
   {
	   cout << "<< Loading solution..." << endl; fflush(0);

	   loadSolucaoInicial(cmd,solution);
	   if(solution)
	   {
		   cout << ">>> Solu��o inicial carregada com sucesso!" << endl; fflush(0);
	   }
   }
   
   // -----------------------------------------------------------------------------------------
   // Executa heur�stica. Se uma solu��o foi carregada, us�-la como solu��o inicial
   if (execHeur)
   {
	    cout << "<< Executando heuristica..." << endl; fflush(0);

	    // setup heur�stica
		HeuristicaNuno::setInputInfo(path, inputFile, inputId);
		HeuristicaNuno::setup(CentroDados::getProblemData());
		
	    cout << "<< 1" << endl; fflush(0);

	    // carregar solu��o inicial
		if(solution)
			HeuristicaNuno::loadSolucao(solution);
		
	    cout << "<< 2" << endl; fflush(0);

	    // load argumentos opcionais heuristica
	    cmd->checkArgsHeuristica();
		
	    cout << "<< 3" << endl; fflush(0);

		// Executa a heur�stica
		ProblemSolution* solucaoHeuristica = nullptr;
		runHeuristica(solucaoHeuristica);
		
	    cout << "<< 4" << endl; fflush(0);

		// Print solu��o
		if(solucaoHeuristica)
		{
			novaSolucaoHeur = true;
			solution = solucaoHeuristica;
		}
		// else: se nao tiver encontrado solu��o, passa a inicial (se houver).
   }
   
   // -----------------------------------------------------------------------------------------
   // Motivos de n�o-atendimento e de uso de professores virtuais

   if (solution)
   {
	   if (novaSolucaoHeur)
	   {
			// motivoUsoPV=true: professores virtuais gerados pela heur�stica
			solution->computaMotivos(true,true);   
	   }
	   else
	   {
			// motivoUsoPV=false: professores virtuais ainda n�o lidos pelo load da solu��o
			solution->computaMotivos(true,false);   
	   }
   }

   // -----------------------------------------------------------------------------------------
   
   timer.stop();
   dif = timer.getCronoCurrSecs();				// total em seg
   int hours = (int) (dif / 3600);				// h
   int min = (int) ( (int) dif % 3600 ) / 60;	// min
   
   std::cout << "\nTotal elapsed time: " << hours << "h" << min << endl << endl;
   
   int status = 0;
   if ( solution )
	   status = 1;

   return status;
}

// ----------------------------------------------------------------------------------------------

// tenta carregar solu��o inicial caso tenha sido inserida
bool loadSolucaoInicial( CmdLine *cmd, ProblemSolution* &solucao)
{	
	int idx;
	bool load = cmd->loadSolucaoInicial(idx);

	// ler solu��o (modoTatico = false!)
	if ( load )
	{
		solucao = nullptr;
		HeuristicaNuno::logMsg("tentar ler solucao", 0);

		char path[ 1024 ];
		cmd->getPath(path);
		char fullPath [1024];
		fullPath[0] = '\0';
		char **argv = cmd->getArgv();

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

	return false;
}

// ----------------------------------------------------------------------------------------------

// run heuristica
bool runHeuristica(ProblemSolution* &solucaoHeurExt)
{
	// run
	HeuristicaNuno::run();

	// imprimir output!
	HeuristicaNuno::logMsg("check se houve solucao", 1);
	if(HeuristicaNuno::solucaoHeur == nullptr)
	{
		HeuristicaNuno::logMsg("%%%%% Solu��o heur�stica n�o encontrada!!! %%%%", 1);
		return false;
	}

	// guardar a solu��o
	solucaoHeurExt = HeuristicaNuno::getSolucao();

	// limpar mem�ria
	cout << "limpar heurnuno" << endl;
	HeuristicaNuno::clean();

	return true;
}