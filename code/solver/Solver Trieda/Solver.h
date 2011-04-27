#ifndef _SOLVER_H
#define _SOLVER_H

class ProblemData;
class ProblemSolution;

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

protected:
   // A reference to the problem's input data.
   ProblemData * problemData;
};

#endif
