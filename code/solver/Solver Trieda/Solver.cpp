#include "Solver.h"
#include "ProblemData.h"
#include <cstdio>

Solver::Solver(ProblemData *aProblemData)
{
   problemData = aProblemData;
   
   OptimizTimeLimitInit();
   OptimizMaxTimeNoImprovInit();

    clockStart = clock();
}

Solver::~Solver()
{
   problemData = NULL;

}


void Solver::OptimizTimeLimitInit()
{
	int one_hour=3600;
	   
#ifdef SOLVER_CPLEX
#ifdef TESTE		
		OptimizTimeLimit[EST] = (int) one_hour * 0.05;
		OptimizTimeLimit[EST_M] = (int) one_hour * 1.0;
		OptimizTimeLimit[EST_T] = (int) one_hour * 1.0;
		OptimizTimeLimit[EST_N] = (int) one_hour * 2.0;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 1.00;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_M] = (int) one_hour * 1.0;
		OptimizTimeLimit[PRE_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[PRE_N] = (int) one_hour * 4.0;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_N] = (int) one_hour * 3.0;
		OptimizTimeLimit[OP] = (int) one_hour * 0.05;
		OptimizTimeLimit[OP1] = (int) one_hour * 0.05;
		OptimizTimeLimit[OP2] = (int) one_hour * 0.05;
		OptimizTimeLimit[OP3] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 0.05;		  
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 3.0;
#elif KROTON
		OptimizTimeLimit[EST] = (int) one_hour * 3.5;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 3;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour * 4;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour * 1.2;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour;
		OptimizTimeLimit[OP] = (int) one_hour * 2;
		OptimizTimeLimit[OP1] = (int) one_hour * 2;
		OptimizTimeLimit[OP2] = (int) one_hour * 3;
		OptimizTimeLimit[OP3] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 5;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 4;
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 3.0;
#elif UNIT
		OptimizTimeLimit[EST] = (int) one_hour * 3.5;
		OptimizTimeLimit[EST_M] = (int) one_hour * 1.0;
		OptimizTimeLimit[EST_T] = (int) one_hour * 1.0;
		OptimizTimeLimit[EST_N] = (int) one_hour * 2.0;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 3;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_M] = (int) one_hour * 1.0;
		OptimizTimeLimit[PRE_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[PRE_N] = (int) one_hour * 4.0;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour * 5;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour * 1.0;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour * 5;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour * 1.5;
		OptimizTimeLimit[TAT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_N] = (int) one_hour * 3.0;
		OptimizTimeLimit[OP] = (int) one_hour * 2;
		OptimizTimeLimit[OP1] = (int) one_hour * 2;
		OptimizTimeLimit[OP2] = (int) one_hour * 3;
		OptimizTimeLimit[OP3] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 4;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 5;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 4;
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 3.0;
#elif UNIRITTER
		OptimizTimeLimit[EST] = (int) one_hour * 2;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 3;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour * 3;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour * 4;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour;
		OptimizTimeLimit[OP] = (int) one_hour;
		OptimizTimeLimit[OP1] = (int) one_hour*2;
		OptimizTimeLimit[OP2] = (int) one_hour*2;
		OptimizTimeLimit[OP3] = (int) one_hour*2;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 3.0;
#else
		OptimizTimeLimit[EST] = (int) one_hour * 2;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour*3;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour*3;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour*3;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour;
		OptimizTimeLimit[OP] = (int) one_hour;
		OptimizTimeLimit[OP1] = (int) one_hour*2;
		OptimizTimeLimit[OP2] = (int) one_hour*2;
		OptimizTimeLimit[OP3] = (int) one_hour*2;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 5;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 3.0;
#endif
#else 
// SOLVER_GUROBI
#ifdef TESTE		
		OptimizTimeLimit[EST] = (int) one_hour * 0.05;
		OptimizTimeLimit[EST_M] = (int) one_hour * 1.0;
		OptimizTimeLimit[EST_T] = (int) one_hour * 1.0;
		OptimizTimeLimit[EST_N] = (int) one_hour * 2.0;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 1.00;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour * 0.05;
		OptimizTimeLimit[PRE_M] = (int) one_hour * 1.0;
		OptimizTimeLimit[PRE_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[PRE_N] = (int) one_hour * 4.0;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_N] = (int) one_hour * 3.0;
		OptimizTimeLimit[OP] = (int) one_hour * 0.05;
		OptimizTimeLimit[OP1] = (int) one_hour * 0.05;
		OptimizTimeLimit[OP2] = (int) one_hour * 0.05;
		OptimizTimeLimit[OP3] = (int) one_hour * 0.05;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 0.3;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.3;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 0.3;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 0.3;		  
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 0.2;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 0.3;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.3;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 0.3;
#elif KROTON
		OptimizTimeLimit[EST] = (int) one_hour * 3.5;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 3;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour * 4;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour * 1.2;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour;
		OptimizTimeLimit[OP] = (int) one_hour * 2;
		OptimizTimeLimit[OP1] = (int) one_hour * 2;
		OptimizTimeLimit[OP2] = (int) one_hour * 3;
		OptimizTimeLimit[OP3] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 5;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 4;
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 3.0;
#elif UNIT
		OptimizTimeLimit[EST] = (int) one_hour * 3.5;
		OptimizTimeLimit[EST_M] = (int) one_hour * 1.0;
		OptimizTimeLimit[EST_T] = (int) one_hour * 0.5;
		OptimizTimeLimit[EST_N] = (int) one_hour * 2.0;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 3;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_M] = (int) one_hour * 4.0;
		OptimizTimeLimit[PRE_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[PRE_N] = (int) one_hour * 4.0;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour * 5;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour * 1.0;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour * 4;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour * 1.5;
		OptimizTimeLimit[TAT_M] = (int) one_hour * 3.0;
		OptimizTimeLimit[TAT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_N] = (int) one_hour * 3.5;
		OptimizTimeLimit[OP] = (int) one_hour * 2;
		OptimizTimeLimit[OP1] = (int) one_hour * 1;
		OptimizTimeLimit[OP2] = (int) one_hour * 2;
		OptimizTimeLimit[OP3] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 1.0;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 4.0;
#elif UNIRITTER
		OptimizTimeLimit[EST] = (int) one_hour * 2;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 3;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour * 3;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour * 4;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour;
		OptimizTimeLimit[OP] = (int) one_hour;
		OptimizTimeLimit[OP1] = (int) one_hour*2;
		OptimizTimeLimit[OP2] = (int) one_hour*2;
		OptimizTimeLimit[OP3] = (int) one_hour*2;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 3;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 3.0;
#else
		OptimizTimeLimit[EST] = (int) one_hour * 2;
		OptimizTimeLimit[EST_FIXA_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT_SAB_ZERO] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT1] = (int) one_hour * 1;
		OptimizTimeLimit[PRE_TAT2] = (int) one_hour * 2;
		OptimizTimeLimit[PRE_TAT3] = (int) one_hour*3;
		OptimizTimeLimit[PRE_TAT4] = (int) one_hour*3;
		OptimizTimeLimit[TAT_HOR] = (int) one_hour;
		OptimizTimeLimit[TAT_HOR1] = 1e10;
		OptimizTimeLimit[TAT_HOR_CALOURO] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_HOR2] = (int) one_hour*3;
		OptimizTimeLimit[TAT_HOR3] = (int) one_hour;
		OptimizTimeLimit[OP] = (int) one_hour;
		OptimizTimeLimit[OP1] = (int) one_hour*2;
		OptimizTimeLimit[OP2] = (int) one_hour*2;
		OptimizTimeLimit[OP3] = (int) one_hour*2;
		OptimizTimeLimit[TAT_INT] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
		OptimizTimeLimit[TAT_INT2] = (int) one_hour * 5;
		OptimizTimeLimit[TAT_INT3] = (int) one_hour * 2;
		OptimizTimeLimit[TAT_INT_CALOURO] = (int) one_hour * 1;
		OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
		OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.4;
		OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 3.0;
#endif

#endif

}




void Solver::OptimizMaxTimeNoImprovInit()
{
	int one_hour=3600;
	   
#ifdef SOLVER_GUROBI

#ifdef TESTE		
	OptimizMaxTimeNoImprov[EST] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[EST_M] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[EST_T] = (int) one_hour * 0.01;
	OptimizMaxTimeNoImprov[EST_N] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[EST_FIXA_SAB_ZERO] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[PRE_TAT] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[PRE_TAT_SAB_ZERO] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[PRE_TAT1] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[PRE_TAT2] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[PRE_TAT3] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[PRE_TAT4] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[PRE_M] = (int) one_hour * 0.1;
	OptimizMaxTimeNoImprov[PRE_T] = (int) one_hour * 0.05;
	OptimizMaxTimeNoImprov[PRE_N] = (int) one_hour * 0.2;
	OptimizMaxTimeNoImprov[TAT_HOR] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_HOR1] = 1e10;
	OptimizMaxTimeNoImprov[TAT_HOR_CALOURO] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_HOR2] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_HOR3] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_M] = (int) one_hour * 0.1;
	OptimizMaxTimeNoImprov[TAT_T] = (int) one_hour * 0.05;
	OptimizMaxTimeNoImprov[TAT_N] = (int) one_hour * 0.2;
	OptimizMaxTimeNoImprov[OP] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[OP1] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[OP2] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[OP3] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT1] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT2] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT3] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT_CALOURO] = (int) one_hour * 0.02; 
	OptimizMaxTimeNoImprov[TAT_INT_M] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT_T] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT_N] = (int) one_hour * 0.02;
#elif defined KROTON
	OptimizMaxTimeNoImprov[EST] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[EST_FIXA_SAB_ZERO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[PRE_TAT] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT_SAB_ZERO] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_TAT1] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_TAT2] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT3] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT4] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[TAT_HOR] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_HOR1] = 1e10;
	OptimizMaxTimeNoImprov[TAT_HOR_CALOURO] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_HOR2] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_HOR3] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[OP] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[OP1] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[OP2] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[OP3] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT2] = (int) one_hour * 1.5;
	OptimizMaxTimeNoImprov[TAT_INT3] = (int) one_hour * 1.0;	
	OptimizMaxTimeNoImprov[TAT_INT_CALOURO] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_M] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_T] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_N] = (int) one_hour * 1.5;
#elif defined UNIT
	OptimizMaxTimeNoImprov[EST] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[EST_M] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[EST_T] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[EST_N] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[EST_FIXA_SAB_ZERO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[PRE_TAT] = (int) one_hour * 3.0;
	OptimizMaxTimeNoImprov[PRE_TAT_SAB_ZERO] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_TAT1] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_TAT2] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT3] = (int) one_hour * 3.0;
	OptimizMaxTimeNoImprov[PRE_TAT4] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_M] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_T] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_N] = (int) one_hour * 2.5;
	OptimizMaxTimeNoImprov[TAT_HOR] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_HOR1] = 1e10;
	OptimizMaxTimeNoImprov[TAT_HOR_CALOURO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_HOR2] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_HOR3] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_M] = (int) one_hour * 2.5;
	OptimizMaxTimeNoImprov[TAT_T] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_N] = (int) one_hour * 3.0;
	OptimizMaxTimeNoImprov[OP] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP2] = (int) one_hour * 0.6;
	OptimizMaxTimeNoImprov[OP3] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT2] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT3] = (int) one_hour * 0.5;	
	OptimizMaxTimeNoImprov[TAT_INT_CALOURO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT_M] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_T] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT_N] = (int) one_hour * 1.0;
#elif defined UNIRITTER
	OptimizMaxTimeNoImprov[EST] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[EST_FIXA_SAB_ZERO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[PRE_TAT] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT_SAB_ZERO] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_TAT1] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_TAT2] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT3] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT4] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[TAT_HOR] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_HOR1] = 1e10;
	OptimizMaxTimeNoImprov[TAT_HOR_CALOURO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_HOR2] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_HOR3] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP2] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP3] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT2] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT3] = (int) one_hour * 0.5;	
	OptimizMaxTimeNoImprov[TAT_INT_CALOURO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT_M] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_T] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_N] = (int) one_hour * 1.5;
#else
	OptimizMaxTimeNoImprov[EST] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[EST_FIXA_SAB_ZERO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[PRE_TAT] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT_SAB_ZERO] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_TAT1] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[PRE_TAT2] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT3] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[PRE_TAT4] = (int) one_hour * 2.0;
	OptimizMaxTimeNoImprov[TAT_HOR] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_HOR1] = 1e10;
	OptimizMaxTimeNoImprov[TAT_HOR_CALOURO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_HOR2] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_HOR3] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP2] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP3] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT2] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT3] = (int) one_hour * 0.5;	
	OptimizMaxTimeNoImprov[TAT_INT_CALOURO] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT_M] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_T] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_N] = (int) one_hour * 1.5;
#endif

#endif

}


std::string Solver::toString( int model )
{
	switch( model )
	{
	case SOL_INIT:
		return "SOL_INIT";
	case EST:
		return "EST";
	case EST_FIXA_SAB_ZERO:
		return "EST_FIXA_SAB_ZERO";	
	case PRE_TAT:
		return "PRE_TAT";
	case PRE_TAT_SAB_ZERO:
		return "PRE_TAT_SAB_ZERO";	
	case TAT_HOR1:
		return "TAT1";	
	case TAT_HOR2:
		return "TAT2";	
	case TAT_INT:
		return "TAT_INT";	
	case OP:
		return "OP";
	case TAT_INT_EQUIV:
		return "TAT_INT_EQUIV";
	case TAT_INT_INS:
		return "TAT_INT_INS";
	case TATICO1:
		return "TATICO1";
	case TATICO2:
		return "TATICO2";
	case TAT_INT_BRANCH_SALA:
		return "TAT_INT_BRANCH_SALA";
	}

	return "error-type-opt";
}



#if defined SOLVER_GUROBI

// CALLBACK function

int __stdcall 
	timeWithoutChangeCallback(GRBmodel *model,
           void     *cbdata,
           int       where,
           void     *usrdata)
{
  struct callback_data *mydata = (struct callback_data *) usrdata;
  
  if (where == GRB_CB_MIP)
  {
	//    cout << "\n===>> Begin callback\n"; fflush(NULL);
		double objbst, nodecnt, runtime, bestBound;

		/* Best valid solution */
		GRBcbget(cbdata, where, GRB_CB_MIP_OBJBST, &objbst);
		/* Current node */
		GRBcbget(cbdata, where, GRB_CB_MIP_NODCNT, &nodecnt);	
		/* run time */	
		GRBcbget(cbdata, where, GRB_CB_RUNTIME, &runtime);				
		/* Best bound */
		GRBcbget(cbdata, where, GRB_CB_MIP_OBJBND, &bestBound);
		
		if ( fabs(objbst - mydata->bestIntObj) >= 10e-4 )
		{
			/* improvement */
	//		cout << "Improvement achieved at node " << nodecnt << ", runtime " << runtime << ", objbst " << objbst << ". "; fflush(NULL);
			mydata->bestIntObj = objbst;
			mydata->runtime = runtime;
	//		cout << "Updated.\n"; fflush(NULL);
		}
		else
		{		
	//		cout << "No Improvement achieved at node " << nodecnt << endl;
			bool hugeGap = false;
			if ( fabs(mydata->bestIntObj) > 10e-3 )
			{
				double gap = 100 * fabs( bestBound - mydata->bestIntObj ) / mydata->bestIntObj;
				if ( gap > mydata->gapMax )
					hugeGap = true;
			}

			if ( !hugeGap )
			if ( runtime - mydata->runtime >= mydata->timeLimit )
			{
				/* if there is too much time without any improvement, then quit */
				cout << "Abort by timeWithoutChangeCallback. TimeLimit " << mydata->timeLimit 
					<< ", Runtime " << runtime << ", Objbst " << objbst << ", Bound " << bestBound
					<< ", gapMax " << mydata->gapMax << endl; fflush(NULL);
			
				mydata->reset();
			
				GRBterminate(model);
			}
		}
	//    cout << "\n<<=== End callback\n"; fflush(NULL);
  } 
  return 0;
}

#endif