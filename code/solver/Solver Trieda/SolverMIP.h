#ifndef SOLVERMIP_H
#define SOLVERMIP_H

#include "Solver.h"
#include "Variable.h"
#include "Constraint.h"
#include "opt_lp.h"
#include "input.h"

#include "InitialSolution.h"

#define PRINT_cria_variaveis
#define PRINT_cria_restricoes
#define DEBUG

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

   double alpha, beta, gamma, delta, lambda, epsilon, rho, M, psi;

   struct Ordena
   {
      bool operator() (std::vector<int> xI, std::vector<int> xJ)
      { return (xI.front() > xJ.front()); }
   } ordenaPorCreditos;

public:

   int cria_variaveis(void);

   int cria_variavel_creditos(void);   // x_{i,d,u,tps,t}
   int cria_variavel_oferecimentos(void); // o_{i,d,u,tps,t}
   int cria_variavel_abertura(void);   // z_{i,d,cp}
   int cria_variavel_alunos(void);  // a_{i,d,oft}
   int cria_variavel_aloc_alunos(void);   // b_{i,d,c,cp}
   int cria_variavel_consecutivos(void);  // c_{i,d,t}
   int cria_variavel_min_creds(void);  // h_{bc,i}
   int cria_variavel_max_creds(void);  // H_{bc,i}
   int cria_variavel_aloc_disciplina(void);  // y_{i,d,tps,u}
   int cria_variavel_num_subblocos(void); // w_{bc,t,cp}
   int cria_variavel_num_abertura_turma_bloco(void);  // v_{bc,t}
   int cria_variavel_de_folga_dist_cred_dia_superior(void); // fcp_{d,t}
   int cria_variavel_de_folga_dist_cred_dia_inferior(void); // fcm_{d,t}
   int cria_variavel_abertura_subbloco_de_blc_dia_campus(void);   // r_{bc,t,cp}

   int cria_variavel_de_folga_aloc_alunos_curso_incompat(void); // bs_{i,d,c,cp}

   int cria_variavel_de_folga_demanda_disciplina(); // f_{d,o}

   int cria_variavel_combinacao_divisao_credito(void);//m_{i,d,k}
   int cria_variavel_de_folga_combinacao_divisao_credito(void); // fk_{i,d,k}

   /********************************************************************
   **                    CONSTRAINT CREATION                          **
   *********************************************************************/

   /*
   ToDo:
   All methods of constraint creation should be defined here
   */

   int cria_restricoes(void);

   int cria_restricao_carga_horaria(void);					// Restricao 1.2.3
   int cria_restricao_max_cred_sd(void);					// Restricao 1.2.4
   int cria_restricao_min_cred_dd(void);					// Restricao 1.2.5
   int cria_restricao_ativacao_var_o(void);						// Restricao 1.2.6
   int cria_restricao_evita_sobreposicao(void);			// Restricao 1.2.7
   int cria_restricao_disciplina_sala(void);				// Restricao 1.2.8
   int cria_restricao_turma_sala(void);					// Restricao 1.2.9
   int cria_restricao_evita_turma_disc_camp_d(void);		// Restricao 1.2.10
   int cria_restricao_turmas_bloco(void);					// Restricao 1.2.11
   int cria_restricao_max_cred_disc_bloco(void);			// Restricao 1.2.12
   int cria_restricao_num_tur_bloc_dia_difunid(void);		// Restricao 1.2.13
   int cria_restricao_lim_cred_diar_disc(void);			// Restricao 1.2.14
   int cria_restricao_cap_aloc_dem_disc(void);				// Restricao 1.2.15
   int cria_restricao_cap_sala_compativel_turma(void);		// Restricao 1.2.16
   int cria_restricao_cap_sala_unidade(void);				// Restricao 1.2.17
   int cria_restricao_turma_disc_dias_consec(void);		// Restricao 1.2.18
   int cria_restricao_min_creds_turm_bloco(void);			// Restricao 1.2.19
   int cria_restricao_max_creds_turm_bloco(void);			// Restricao 1.2.20
   int cria_restricao_aluno_curso_disc(void);				// Restricao 1.2.21
   int cria_restricao_alunos_cursos_dif(void);				// Restricao 1.2.22
   int cria_restricao_de_folga_dist_cred_dia(void);		// Restricao 1.2.23
   int cria_restricao_ativacao_var_r(void);						// Restricao 1.2.24

   int cria_restricao_limita_abertura_turmas(void);      // Restricao 1.2.25
   int cria_restricao_abre_turmas_em_sequencia(void);      // Restricao 1.2.26
   int cria_restricao_divisao_credito(void);			    //Restricao 1.2.27
   int cria_restricao_combinacao_divisao_credito(void);		//Restricao 1.2.28


   /********************************************************************
   **                        OTHER METHODS                            **
   *********************************************************************/

   void cria_solucao_inicial(int cnt, int *indices, double *valores);


};

#endif