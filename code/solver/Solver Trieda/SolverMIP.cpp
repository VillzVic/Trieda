#include "opt_cplex.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolverMIP.h"

SolverMIP::SolverMIP(ProblemData *aProblemData)
:Solver(aProblemData)
{
   try
   {
	   lp = new OPT_CPLEX;
   }
   catch(...)
   {
   }
   
   solVars.clear();
}

SolverMIP::~SolverMIP()
{
   int i;

   if (lp != NULL) delete lp;

   for (i=0; i < (int)solVars.size(); i++)
   {
      if ( solVars[i] != NULL )
         delete solVars[i];
   }
   solVars.clear();
}

int SolverMIP::solve()
{
   int varNum = 0;
   int constNum = 0;

   lp->createLP("Solver Trieda", OPTSENSE_MAXIMIZE, PROB_LP);

#ifdef DEBUG
   printf("Creating LP...\n");
#endif

   /* Variable creation */

#ifdef DEBUG
   printf("Total of Variables: %i\n\n",varNum);
#endif

   /* Constraint creation */

#ifdef DEBUG
   printf("Total of Constraints: %i\n",constNum);
#endif

#ifdef DEBUG
   lp->writeProbLP("Solver Trieda");
#endif

   int status = lp->optimize(METHOD_PRIMAL);

   return status;
}


void SolverMIP::getSolution(ProblemSolution *problemSolution)
{
   double *xSol = NULL;
   VariableHash::iterator vit;

   xSol = new double[lp->getNumCols()];
   lp->getX(xSol);

   vit = vHash.begin();

#ifdef DEBUG
   FILE *fout = fopen("solucao.txt","wt");
#endif

   while (vit != vHash.end())
   {
      Variable *v = new Variable(vit->first);
      int col = vit->second;

      v->setValue( xSol[col] );

      if ( v->getValue() > 0.00001 )
      {
#ifdef DEBUG
         char auxName[100];
         lp->getColName(auxName,col,100);
         fprintf(fout,"%s = %f\n",auxName,v->getValue());
#endif
         /**
         ToDo:
         */
      }

      vit++;
   }

   // Fill the solution

#ifdef DEBUG
   if ( fout )
      fclose(fout);
#endif

   if ( xSol )
      delete[] xSol;
}

