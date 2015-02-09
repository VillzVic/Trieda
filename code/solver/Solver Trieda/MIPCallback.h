#ifndef _MIP_CALLBACK_H_
#define _MIP_CALLBACK_H_

#ifdef SOLVER_GUROBI

#include "opt_gurobi.h"


// 
// CALLBACK
// 

/* Define structure to pass my data to the callback function */

struct CBData {
	double bestIntObj;
	double runtime;
	double timeLimit;
	double gapMax;
	
	void reset(){
	bestIntObj = GRB_INFINITY;
	runtime = 0.0;
	}
};

int __stdcall 
	MIPcallback(GRBmodel *model,
           void     *cbdata,
           int       where,
           void     *usrdata);


#endif

#endif
