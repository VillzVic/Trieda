#ifndef _SOLVER_MIP_H_
#define _SOLVER_MIP_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "Solver.h"
#include "Variable.h"
#include "Constraint.h"
#include "VariableOp.h"
#include "ConstraintOp.h"
#include "opt_lp.h"
#include "input.h"
#include "opt_gurobi.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolutionLoader.h"
#include "ErrorHandler.h"
#include "ProblemDataLoader.h"
#include "SolucaoOperacional.h"
#include "SolucaoInicialOperacional.h"
#include "Avaliador.h"

#include "NSSeqSwapEqBlocks.h"
#include "NSSwapEqSchedulesBlocks.h"
#include "NSSwapEqTeachersBlocks.h"

#include "NSShift.h"

#include "IteratedLocalSearchLevels.h"
#include "RandomDescentMethod.h"
#include "RVND.hpp"

#define PRINT_cria_variaveis
#define PRINT_cria_restricoes

// -----------------------------------
//#define READ_SOLUTION_TATICO_BIN

#ifndef READ_SOLUTION_TATICO_BIN
#define WRITE_SOLUTION_TATICO_BIN
#endif
// -----------------------------------

/*
 * Implements a MIP Solver.
 */
class SolverMIP : public Solver
{
public:
   /**
   * Default Constructor.
   * @param aProblemData The problem's input data.
   */
   SolverMIP( ProblemData *, ProblemSolution *, ProblemDataLoader * );

   /** Destructor. */
   virtual ~SolverMIP();

   /**
   * Solves the MIP previously created.
   * @return The status returned by GUROBI.
   */
   int solve();

   /**
   * Processes the variable values and populates the output class.
   * @param ps A reference to the class to be populated.
   */
   void getSolution( ProblemSolution * );

   /********************************************************************
   **                      VARIABLE CREATION                          **
   *********************************************************************/

   int cria_variaveis(void);

   int cria_variavel_creditos(void);   // x_{i,d,u,tps,t}
   int cria_variavel_creditos_permitir_alunos_varios_campi(void);   // x_{i,d,u,tps,t}
   int cria_variavel_oferecimentos(void); // o_{i,d,u,tps,t}
   //int cria_variavel_oferecimentos_permitir_alunos_varios_campi(void); // o_{i,d,u,tps,t}
   int cria_variavel_abertura(void);   // z_{i,d,cp}
   //int cria_variavel_abertura_permitir_alunos_varios_campi(void);   // z_{i,d,cp}
   int cria_variavel_alunos(void);  // a_{i,d,oft}
   int cria_variavel_aloc_alunos(void);   // b_{i,d,c,cp}
   //int cria_variavel_aloc_alunos_permitir_alunos_varios_campi(void);   // b_{i,d,c,cp}
   int cria_variavel_consecutivos(void);  // c_{i,d,t}
   int cria_variavel_min_creds(void);  // h_{bc,i}
   int cria_variavel_max_creds(void);  // H_{bc,i}
   int cria_variavel_aloc_disciplina(void);  // y_{i,d,tps,u}
   //int cria_variavel_aloc_disciplina_permitir_alunos_varios_campi(void);  // y_{i,d,tps,u}
   int cria_variavel_num_subblocos(void); // w_{bc,t,cp}
   int cria_variavel_num_abertura_turma_bloco(void);  // v_{bc,t}
   int cria_variavel_de_folga_dist_cred_dia_superior(void); // fcp_{d,t}
   //int cria_variavel_de_folga_dist_cred_dia_superior_permitir_alunos_varios_campi(void); // fcp_{d,t}
   int cria_variavel_de_folga_dist_cred_dia_inferior(void); // fcm_{d,t}
   //int cria_variavel_de_folga_dist_cred_dia_inferior_permitir_alunos_varios_campi(void); // fcm_{d,t}
   int cria_variavel_abertura_subbloco_de_blc_dia_campus(void);   // r_{bc,t,cp}
   int cria_variavel_de_folga_aloc_alunos_curso_incompat(void); // bs_{i,d,c,cp}
   //int cria_variavel_de_folga_aloc_alunos_curso_incompat_permitir_alunos_varios_campi(void); // bs_{i,d,c,cp}
   int cria_variavel_de_folga_demanda_disciplina(); // f_{d,o}
   int cria_variavel_combinacao_divisao_credito(void);//m_{i,d,k}
   int cria_variavel_de_folga_combinacao_divisao_credito(void); // fk_{i,d,k}
   int cria_variavel_creditos_modificada(void); // xm_{d,t}
   //int cria_variavel_creditos_modificada_permitir_alunos_varios_campi(void); // xm_{d,t}
   int cria_variavel_abertura_compativel(void); // zc_{d,t}
   int cria_variavel_abertura_bloco_mesmoTPS(void); // n_{bc,tps}
   int cria_variavel_de_folga_abertura_bloco_mesmoTPS(void); //fn_{bc,tps}

   /********************************************************************
   **                    CONSTRAINT CREATION                          **
   *********************************************************************/

   int cria_restricoes(void);

   int cria_restricao_carga_horaria(void);					// Restricao 1.2.2
   int cria_restricao_max_cred_sd(void);					// Restricao 1.2.3
   int cria_restricao_min_cred_dd(void);					// Restricao 1.2.4
   int cria_restricao_ativacao_var_o(void);					// Restricao 1.2.5
   int cria_restricao_evita_sobreposicao(void);			    // Restricao 1.2.6
   int cria_restricao_disciplina_sala(void);				// Restricao 1.2.7
   int cria_restricao_turma_sala(void);					    // Restricao 1.2.8
   int cria_restricao_evita_turma_disc_camp_d(void);		// Restricao 1.2.9
   int cria_restricao_turmas_bloco(void);					// Restricao 1.2.10
   int cria_restricao_max_cred_disc_bloco(void);			// Restricao 1.2.11
   int cria_restricao_num_tur_bloc_dia_difunid(void);		// Restricao 1.2.12
   int cria_restricao_lim_cred_diar_disc(void);			    // Restricao 1.2.13
   int cria_restricao_cap_aloc_dem_disc(void);				// Restricao 1.2.14
   int cria_restricao_cap_sala_compativel_turma(void);		// Restricao 1.2.15
   int cria_restricao_cap_sala_unidade(void);				// Restricao 1.2.16
   int cria_restricao_turma_disc_dias_consec(void);		    // Restricao 1.2.17
   int cria_restricao_min_creds_turm_bloco(void);			// Restricao 1.2.18
   int cria_restricao_max_creds_turm_bloco(void);			// Restricao 1.2.19
   int cria_restricao_aluno_curso_disc(void);				// Restricao 1.2.20
   int cria_restricao_alunos_cursos_dif(void);				// Restricao 1.2.21
   int cria_restricao_de_folga_dist_cred_dia(void);		    // Restricao 1.2.22
   int cria_restricao_ativacao_var_r(void);					// Restricao 1.2.23
   int cria_restricao_limita_abertura_turmas(void);         // Restricao 1.2.24
   int cria_restricao_abre_turmas_em_sequencia(void);       // Restricao 1.2.25
   int cria_restricao_divisao_credito(void);			    // Restricao 1.2.26
   int cria_restricao_combinacao_divisao_credito(void);		// Restricao 1.2.27
   int cria_restricao_ativacao_var_y(void);		            // Restricao 1.2.28
   int cria_restricao_max_creds_disc_dia(void);				// Restricao 1.2.29
   int cria_restricao_max_creds_bloco_dia(void);			// Restricao 1.2.30
   int cria_restricao_ativacao_var_zc(void);				// Restricao 1.2.31
   int cria_restricao_disciplinas_incompativeis(void);		// Restricao 1.2.32
   int cria_restricao_abertura_bloco_mesmoTPS(void);		// Restricao 1.2.33
   int cria_restricao_folga_abertura_bloco_mesmoTPS(void);  // Restricao 1.2.34

   // Criacao de variaveis operacional
   int criaVariaveisOperacional();
   int criaVariavelProfessorAulaHorario();
   int criaVariavelProfessorDisciplina();
   int criaVariavelFolgaFixProfDiscSalaDiaHor();
   int criaVariavelFolgaFixProfDiscDiaHor();
   int criaVariavelFolgaFixProfDisc();
   int criaVariavelFolgaFixProfDiscSala();
   int criaVariavelFolgaFixProfSala();
   int criaRestricoesOperacional();
   int criaRestricaoSalaHorario();
   int criaRestricaoProfessorHorario();
   int criaRestricaoBlocoHorario();
   int criaRestricaoAlocAula();
   int criaRestricaoProfessorDisciplina();
   int criaRestricaoProfessorDisciplinaUnico();
   int criaRestricaoFixProfDiscSalaDiaHor();
   int criaRestricaoFixProfDiscDiaHor();
   int criaRestricaoFixProfDisc();
   int criaRestricaoFixProfDiscSala();
   int criaRestricaoFixProfSala();
      
   void cria_solucao_inicial( int , int * , double * );
   int localBranching( double *, double );
   void carregaVariaveisSolucaoTatico();

   int solveTatico();
   int solveTaticoBasico();
   void converteCjtSalaEmSala();
   void separaDisciplinasEquivalentes();
   void getSolutionTatico();

   int solveOperacional();
   int solveOperacionalMIP();
   void getSolutionOperacional();
   void getSolutionOperacionalMIP();
   void geraProfessoresVirtuaisMIP();
   Professor* criaProfessorVirtual(HorarioDia *horario,int cred,std::set<std::pair<Professor*,HorarioDia*> > &profVirtualList);
   void preencheOutputOperacional( ProblemSolution * );
   void preencheOutputOperacionalMIP( ProblemSolution * );
   bool aulaAlocada( Aula *, Campus *, Unidade *, Sala *, int );
   vector< Variable * > variaveisAlunosAtendidos( Curso *, Disciplina * );
   vector< Variable * > variaveisCreditosAtendidos( Disciplina * );
   void criaVariaveisAlunosDisciplinasSubstituidas();
   void criaVariaveisCreditosDisciplinasSubstituidas();

   Variable * criaVariavelAlunos(
      Campus *, Unidade *, ConjuntoSala *, Sala *,
      int, Oferta *, Curso *, Disciplina *, int );

   Variable * criaVariavelCreditos(
      Campus *, Unidade *, ConjuntoSala *, Sala *,
      int, Oferta *, Curso *, Disciplina *, int, BlocoCurricular * );

private:
   // Dada uma disciplina 'A' que foi substituída por uma de suas disciplinas equivalentes 'B',
   // esse map informa o conjunto de variáveis que foram criadas para 'B' referentes à disciplina 'A'
   std::map< Disciplina *, std::vector< Variable > > mapVariaveisDisciplinasEquivalentes;

   std::vector< Variable > filtraVariaveisAlunos( std::vector< Variable > );
   std::vector< Variable > filtraVariaveisCreditos( std::vector< Variable > );

   void retornaHorariosPossiveis(Professor *prof, Aula *aula, std::list<HorarioDia*> &listaHor);

   // Vetor responsável por armazenar ponteiros para todas as
   // variáveis do  tipo V_CREDITOS com credito(s) alocado(s).
   typedef vector< Variable * > vars__X___i_d_u_tps_t;

   // Estrutura responsável por armazenar referências para todas variáveis
   // do tipo V_ALUNOS que possuirem algum valor de atendimento maior que 0.
   typedef std::map< std::pair< int /*turma*/, Disciplina * >,
					      std::vector< Variable * > > vars__A___i_d_o;

   void chgCoeffList(std::vector<std::pair<int,int> > cL, std::vector<double> cLV);

   vars__X___i_d_u_tps_t vars_x;
   vars__A___i_d_o vars_a;

   ProblemSolution * problemSolution;
   ProblemDataLoader * problemDataLoader;

   // The linear problem.
   OPT_LP * lp;

   // Hash which associates the column number with the Variable object.
   VariableHash vHash;

   // Hash which associates the row number with the Constraint object.
   ConstraintHash cHash;

   // Hash which associates the column number with the VariableOp object.
   VariableOpHash vHashOp;

   // Hash which associates the row number with the ConstraintOp object.
   ConstraintOpHash cHashOp;

   // Stores the solution variables (non-zero).
   std::vector< Variable * > solVars;

   std::vector< VariableOp * > solVarsOp;

   double alpha, beta, gamma, delta, lambda, epsilon, rho, M, psi, tau, eta;

   struct Ordena
   {
      bool operator() ( std::vector< int > xI, std::vector< int > xJ )
      {
		  return (xI.front() > xJ.front());
	  }
   } ordenaPorCreditos;
};

#endif
