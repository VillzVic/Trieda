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
#ifdef TESTE
	OptimizTimeLimit[OP] = (int) one_hour * 0.02;
	OptimizTimeLimit[OP1] = (int) one_hour * 0.02;
	OptimizTimeLimit[OP2] = (int) one_hour * 0.02;
	OptimizTimeLimit[OP3] = (int) one_hour * 0.02;
	OptimizTimeLimit[TAT_INT] = (int) one_hour * 0.02;
	OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.02;
	OptimizTimeLimit[TAT_INT2] = (int) one_hour * 0.02;
	OptimizTimeLimit[TAT_INT3] = (int) one_hour * 0.02;
	OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 0.02;
	OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 0.02;
	OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 0.02;
#else	   
	OptimizTimeLimit[OP] = (int) one_hour * 2;
	OptimizTimeLimit[OP1] = (int) one_hour * 1;
	OptimizTimeLimit[OP2] = (int) one_hour * 2;
	OptimizTimeLimit[OP3] = (int) one_hour * 2;
	OptimizTimeLimit[TAT_INT] = (int) one_hour * 2;
	OptimizTimeLimit[TAT_INT1] = (int) one_hour * 0.1;
	OptimizTimeLimit[TAT_INT2] = (int) one_hour * 3;
	OptimizTimeLimit[TAT_INT3] = (int) one_hour * 2;
	OptimizTimeLimit[TAT_INT_M] = (int) one_hour * 2.5;
	OptimizTimeLimit[TAT_INT_T] = (int) one_hour * 1.0;
	OptimizTimeLimit[TAT_INT_N] = (int) one_hour * 4.0;
#endif
}




void Solver::OptimizMaxTimeNoImprovInit()
{
	int one_hour=3600;
	   
#ifdef SOLVER_GUROBI

#ifdef TESTE
	OptimizMaxTimeNoImprov[OP] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[OP1] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[OP2] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[OP3] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT1] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT2] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT3] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT_M] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT_T] = (int) one_hour * 0.02;
	OptimizMaxTimeNoImprov[TAT_INT_N] = (int) one_hour * 0.02;
#else
	OptimizMaxTimeNoImprov[OP] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[OP2] = (int) one_hour * 0.6;
	OptimizMaxTimeNoImprov[OP3] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT1] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT2] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT3] = (int) one_hour * 0.5;	
	OptimizMaxTimeNoImprov[TAT_INT_M] = (int) one_hour * 1.0;
	OptimizMaxTimeNoImprov[TAT_INT_T] = (int) one_hour * 0.5;
	OptimizMaxTimeNoImprov[TAT_INT_N] = (int) one_hour * 1.0;
#endif

#endif

}


std::string Solver::toString( int model )
{
	switch( model )
	{
	case SOL_INIT:
		return "SOL_INIT";
	case TAT_INT:
		return "TAT_INT";	
	case OP:
		return "OP";
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