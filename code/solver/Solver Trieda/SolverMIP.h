#ifndef SOLVERMIP_H
#define SOLVERMIP_H

#include "Solver.h"
#include "Variable.h"
#include "Constraint.h"
#include "opt_lp.h"

/**
* Implements a MIP Solver.
*/
class SolverMIP : public Solver
{
public:
   /**
   * Default Constructor.
   * @param aProblemData The problem's input data.
   */
   SolverMIP(ProblemData *aProblemData);

   /** Destructor. */
   ~SolverMIP();

   /**
   * Solves the MIP previously created.
   * @return The status returned by CPLEX.
   */
   int solve();

   /**
   * Processes the variable values and populates the output class.
   * @param ps A reference to the class to be populated.
   */
   void getSolution(ProblemSolution *ps);

private:
   /** The linear problem. */
   OPT_LP *lp;

   /** Hash which associates the column number with the Variable object. */
   VariableHash vHash;

   /** Hash which associates the row number with the Constraint object. */
   ConstraintHash cHash;

   /** Stores the solution variables (non-zero). */
   std::vector<Variable*> solVars;


   /********************************************************************
   **                     VARIABLE CREATION                           **
   *********************************************************************/

   /*
   ToDo:
   All methods of variable creation should be defined here
   */

   /********************************************************************
   **                    CONSTRAINT CREATION                          **
   *********************************************************************/

   /*
   ToDo:
   All methods of constraint creation should be defined here
   */

};

#endif