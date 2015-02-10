#include "Solver.h"
#include "ProblemData.h"
#include <cstdio>

Solver::Solver(ProblemData *aProblemData)
{
   problemData = aProblemData;
   
   clockStart = clock();
}

Solver::~Solver()
{
   problemData = NULL;

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