#ifndef MAIN_HEURISTICA_IES
#define MAIN_HEURISTICA_IES


#include "CmdLine.h"
#include "ProblemSolution.h"
#include "ProblemData.h"


int mainHeur( CmdLine *cmd, ProblemData* data, ProblemSolution* &solution );
bool runHeuristica(ProblemSolution* &solucaoHeurExt);
bool loadSolucaoInicial( CmdLine *cmd, ProblemSolution* &solucao );

#endif