#ifndef _SOLVER_H
#define _SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#endif


class ProblemData;
class ProblemSolution;


#if defined SOLVER_GUROBI

// 
// CALLBACK
// 

/* Define structure to pass my data to the callback function */

struct callback_data {
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
timeWithoutChangeCallback(GRBmodel *model,
			void     *cbdata,
			int       where,
			void     *usrdata);

#endif



/**
* Defines a generic solver.
*/
class Solver
{
public:
   /**
   * Default Constructor.
   * @param aProblemData The problem's input data.
   */
   Solver( ProblemData * );

   /** Destructor. */
   virtual ~Solver();

   /**
   * Solves the problem.
   * @return The solution status.
   */
   virtual int solve() = 0;

   /**
   * Processes the variable values and populates the output class.
   * @param ps A reference to the class to be populated.
   */
   virtual void getSolution( ProblemSolution * ) = 0;

   /**
   * Returns .
   * @return Current run time of the Solver.
   */
   double getRunTime()
   {
	   //solverTimer.stop();
	   //int runtime = solverTimer.getCronoCurrSecs();
	   //solverTimer.start();

	   int t = clock() - clockStart;
	   double runtime = ((float)t)/CLOCKS_PER_SEC;

	   return runtime;
   };


   int getTimeLimit( int v )
   {
		return OptimizTimeLimit[v];
   };

   int getMaxTimeNoImprov( int v )
   {
		return OptimizMaxTimeNoImprov[v];
   };


	#ifdef SOLVER_GUROBI
	 callback_data cb_data;
	#endif

protected:
   // A reference to the problem's input data.
   ProblemData * problemData;

   std::map< int, int > OptimizTimeLimit;
   
   std::map< int, int > OptimizMaxTimeNoImprov;

   //CPUTimer solverTimer;
   clock_t solverRunTime;
   int clockStart;

   enum OptimizStep
   {
	  SOL_INIT = -1,
	  EST = 0,
	  PRE_TAT = 1,
      PRE_TAT1 = 2,
      PRE_TAT2 = 3,
      PRE_TAT3 = 4,
      PRE_TAT4 = 5,
	  TAT_HOR = 6,					
	  TAT_HOR1 = 7,				
	  TAT_HOR2 = 8,		
	  TAT_HOR3 = 9,
	  OP = 10,					
	  OP1 = 11,				
	  OP2 = 12,		
	  OP3 = 13,
	  TAT_INT = 14,					
	  TAT_INT1 = 15,				
	  TAT_INT2 = 16,		
	  TAT_INT3 = 17,
	  TAT_INT_EQUIV = 18,			
	  TAT_INT_INS = 19,
	  TATICO1 = 20,		
	  TATICO2 = 21,
	  TAT_INT_BRANCH_SALA = 22,
	  PRE_TAT_INIT = 23,
	  TAT_HOR_CALOURO = 24,
	  TAT_INT_CALOURO = 25,
	  EST_FIXA_SAB_ZERO = 26,
	  PRE_TAT_SAB_ZERO = 27,
	  EST_M = 28,
	  EST_T = 29,
	  EST_N = 30,
	  PRE_M = 31,
	  PRE_T = 32,
	  PRE_N = 33,
	  TAT_M = 34,
	  TAT_T = 35,
	  TAT_N = 36,
	  TAT_INT_M = 37,
	  TAT_INT_T = 38,
	  TAT_INT_N = 39
   };
   
   void OptimizTimeLimitInit();
   void OptimizMaxTimeNoImprovInit();

   std::string toString( int step );

};



#endif
