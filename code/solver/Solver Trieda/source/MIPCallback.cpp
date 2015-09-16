#include "MIPCallback.h"

#include <iostream>


#ifdef SOLVER_GUROBI

int __stdcall 
	MIPcallback(GRBmodel *model,
           void     *cbdata,
           int       where,
           void     *usrdata)
{
  struct CBData *mydata = (struct CBData *) usrdata;
  
  if (where == GRB_CB_MIP)
  {
	//    std::cout << "\n===>> Begin callback\n"; fflush(NULL);
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
	//		std::cout << "Improvement achieved at node " << nodecnt << ", runtime " << runtime << ", objbst " << objbst << ". "; fflush(NULL);
			mydata->bestIntObj = objbst;
			mydata->runtime = runtime;
	//		std::cout << "Updated.\n"; fflush(NULL);
		}
		else
		{		
	//		std::cout << "No Improvement achieved at node " << nodecnt << endl;
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
				std::cout << "Abort by MIPcallback. TimeLimit " << mydata->timeLimit 
					<< ", Runtime " << runtime << ", Objbst " << objbst << ", Bound " << bestBound
					<< ", gapMax " << mydata->gapMax << std::endl; fflush(NULL);
			
				mydata->reset();
			
				GRBterminate(model);
			}
		}
	//    std::cout << "\n<<=== End callback\n"; fflush(NULL);
  } 
  return 0;
}

#endif
