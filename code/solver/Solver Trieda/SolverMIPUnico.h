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
   int solveOpComTatico();
   int solveOpSemTatico();
   int solveEscolaTat();
   void solveCampusP1Escola();
   void solveCampusP2Escola();
   int solveEscolaOp();

   void preencheAtendTaticoProbSol();
   bool preencheAtendTaticoProbData();
   bool clearAtendTaticoProbSol();
   void relacionaAlunosDemandas();
   void mudaCjtSalaParaSala();
   void getSolutionTaticoPorAlunoComHorario();

   void writeOutputTatico();
      
private:

	void clearSolutionTat();
	void clearSolutionOp();

    // The linear problem   
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif
	   
	bool CARREGA_SOLUCAO;
   
   ProblemSolution * problemSolution;
   ProblemDataLoader * problemDataLoader;
   
   // usado para armazenar a solução tatica da iteração cjtAluno anterior, a fim de fazer a fixação de valores
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
