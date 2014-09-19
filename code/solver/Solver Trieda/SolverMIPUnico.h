#ifndef _SOLVER_MIP_UNICO_H_
#define _SOLVER_MIP_UNICO_H_

#include "Solver.h"

#include "VariableOp.h"
#include "VariableTatico.h"

class ProblemDataLoader;
class ProblemSolution;
class ProblemData;


class SolverMIPUnico : public Solver
{
public:
	
   SolverMIPUnico( ProblemData *, ProblemSolution *, ProblemDataLoader * );
   virtual ~SolverMIPUnico();
   
   void getSolution( ProblemSolution * ){};

   int solve();
   int solveEscola();
   
   void relacionaAlunosDemandas();
   void mudaCjtSalaParaSala();
   void getSolutionTaticoPorAlunoComHorario();

      
private:

   // The linear problem.	
   //OPT_LP * lp;
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif


	bool CARREGA_SOLUCAO;

   
   ProblemSolution * problemSolution;
   ProblemDataLoader * problemDataLoader;

   
   // usado para armazenar a solu��o tatica da itera��o cjtAluno anterior, a fim de fazer a fixa��o de valores
   GGroup< VariableTatico *, LessPtr<VariableTatico> > solVarsTatico; 
   
   GGroup< VariableTatico *, LessPtr<VariableTatico> > vars_xh;

   GGroup< VariableOp *, LessPtr<VariableOp> > solVarsOp;
   

   struct Ordena
   {
      bool operator() ( std::vector< int > xI, std::vector< int > xJ )
      {
         return ( xI.front() > xJ.front() );
      }
   } ordenaPorCreditos;


   int campusAtualId;
   
   std::map<int, std::vector<string> > alDemNaoAtend_output;

   
};

#endif
