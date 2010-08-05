#ifndef SOLVERMIP_H
#define SOLVERMIP_H

#include "Solver.h"
#include "Variable.h"
#include "Constraint.h"
#include "opt_lp.h"
#include "input.h"

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

   double alpha, beta, gamma, delta, lambda, M;
public:
   int cria_variaveis(void);
   int cria_variavel_creditos(void);
   int cria_variavel_oferecimentos(void);
   int cria_variavel_abertura(void);
   int cria_variavel_alunos(void);
   int cria_variavel_consecutivos(void);
   int cria_restricao_carga(void);
   int cria_restricao_max_creditos_sd(void);
   int cria_restricao_min_creditos(void);
   int cria_restricao_ativacao(void);
   int cria_restricao_sobreposicao(void);
   int cria_restricao_mesma_unidade(void);
   int cria_restricao_max_creditos(void);
   int cria_variavel_turma_bloco(void);
   int cria_variavel_min_creds(void);
   int cria_variavel_max_creds(void);
   int cria_restricao_turmas_bloco(void);
   int cria_restricao_cap_demanda(void);
   int cria_restricao_cap_sala(void);
   int cria_restricao_cap_sala_unidade(void);
   int cria_restricao_dias_consecutivos(void);
};

#endif