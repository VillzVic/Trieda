#ifndef _SOLVER_MIP_H_
#define _SOLVER_MIP_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "ValidateSolution.h"
#include "Solver.h"
#include "VariablePre.h"
#include "ConstraintPre.h"
#include "VariableTatico.h"
#include "ConstraintTatico.h"
#include "Variable.h"
#include "Constraint.h"
#include "VariableOp.h"
#include "ConstraintOp.h"
#include "opt_lp.h"
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


#pragma warning(disable:4503)


#define PRINT_cria_variaveis
#define PRINT_cria_restricoes

// -----------------------------------
//#define READ_SOLUTION_TATICO_BIN

#ifndef READ_SOLUTION_TATICO_BIN
#define WRITE_SOLUTION_TATICO_BIN
#endif
// -----------------------------------

// -----------------------------------
//#define READ_SOLUTION_PRETATICO_BIN

#ifndef READ_SOLUTION_PRETATICO_BIN
#define WRITE_SOLUTION_PRETATICO_BIN
#endif
// -----------------------------------

#ifndef PRE_TATICO
#define PRE_TATICO
#endif


//#define READ_SOLUTION // Se der certo o uso desse define, pode deletar os READ_SOLUTION_TATICO_BIN e READ_SOLUTION_PRETATICO_BIN

// ----------------------------------
// NOVA ABORDAGEM

#define TATICO_CJT_ALUNOS

#ifndef TATICO_CJT_ALUNOS
#define SEM_CJT_ALUNOS
#endif
// ----------------------------------


class SolverMIP : public Solver
{
public:

   SolverMIP( ProblemData *, ProblemSolution *, ProblemDataLoader * );
   virtual ~SolverMIP();

   int solve();
   void getSolution( ProblemSolution * );

   
#ifdef TATICO_CJT_ALUNOS


   /********************************************************************
   **                      Variaveis do pre-tatico                    **
   *********************************************************************/

   int cria_preVariaveis( int campusId, int prioridade, int grupoAlunosId );

   int cria_preVariavel_creditos( int campusId, int grupoAlunosId, int P_ATUAL  );						// x_{i,d,s}
   int cria_preVariavel_oferecimentos( int campusId, int grupoAlunosId, int P_ATUAL  );					// o_{i,d,s}
   int cria_preVariavel_abertura( int campusId, int grupoAlunosId, int P_ATUAL  );						// z_{i,d,cp}
   int cria_preVariavel_alunos( int campusId, int grupoAlunosId, int P_ATUAL  );							// a_{i,d,oft,s}
   int cria_preVariavel_aloc_alunos( int campusId, int grupoAlunosId, int P_ATUAL  );						// b_{i,d,c}   
   int cria_preVariavel_folga_compartilhamento_incomp( int campusId, int grupoAlunosId, int P_ATUAL  );	// bs_{i,d,c1,c2}
   int cria_preVariavel_folga_proibe_compartilhamento( int campusId, int grupoAlunosId, int P_ATUAL  );	// fc_{i,d,c1,c2}
   int cria_preVariavel_folga_turma_mesma_disc_sala_dif( int campusId, int grupoAlunosId, int P_ATUAL  ); // fs_{d,s,oft}
   int cria_preVariavel_limite_sup_creds_sala( int campusId );								// Hs_{cp}
   int cria_preVariavel_aloca_alunos_oferta( int campusId, int grupoAlunosId, int P_ATUAL  );				// c_{i,d,oft,s}
   

   // Usadas somente para o modelo Tatico-Aluno:
   int cria_preVariavel_folga_demanda_disciplina_aluno( int campusId, int grupoAlunosAtualId, int P_ATUAL );	// fd_{d,a}
   int cria_preVariavel_aloca_aluno_turma_disc( int campusId, int grupoAlunosAtualId, int P_ATUAL );			// s_{i,d,a,cp}
   int cria_preVariavel_folga_prioridade_inf( int campusId, int prior, int grupoAlunosAtualId );// fpi_{a}
   int cria_preVariavel_folga_prioridade_sup( int campusId, int prior, int grupoAlunosAtualId );	// fps_{a}

   /********************************************************************
   **                    Restrições do pre-Tatico                     **
   *********************************************************************/

   int cria_preRestricoes( int campusId, int prioridade, int cjtAlunosId );

   int cria_preRestricao_carga_horaria( int campusId, int cjtAlunosId  );				// Restrição 1.1
   int cria_preRestricao_max_cred_sala_sl( int campusId, int cjtAlunosId  );			// Restrição 1.2
   int cria_preRestricao_ativacao_var_o( int campusId, int cjtAlunosId  );			// Restrição 1.3
   int cria_preRestricao_evita_mudanca_de_sala( int campusId, int cjtAlunosId  );		// Restrição 1.4
   int cria_preRestricao_cap_aloc_dem_disc_aluno( int campusId, int cjtAlunosId  );	// Restrição 1.5
   int cria_preRestricao_aluno_curso_disc( int campusId, int cjtAlunosId  );			// Restrição 1.6
   int cria_preRestricao_cap_sala( int campusId, int cjtAlunosId  );					// Restrição 1.7
   int cria_preRestricao_compartilhamento_incompat( int campusId, int cjtAlunosId  );	// Restrição 1.8
   int cria_preRestricao_proibe_compartilhamento( int campusId, int cjtAlunosId  );	// Restrição 1.9
   int cria_preRestricao_ativacao_var_z( int campusId, int cjtAlunosId  );			// Restricao 1.10
   int cria_preRestricao_evita_turma_disc_camp_d( int campusId, int cjtAlunosId  );	// Restricao 1.11
   int cria_preRestricao_limita_abertura_turmas( int campusId, int cjtAlunosId  );    // Restricao 1.12
   int cria_preRestricao_abre_turmas_em_sequencia( int campusId, int cjtAlunosId, int prioridade );  // Restricao 1.13
   int cria_preRestricao_turma_mesma_disc_sala_dif( int campusId, int cjtAlunosId  ); // Restricao 1.14
   int cria_preRestricao_limite_sup_creds_sala( int campusId, int cjtAlunosId  );		// Restricao 1.15
   int cria_preRestricao_ativa_var_aloc_aluno_oft( int campusId, int cjtAlunosId  );	// Restricao 1.16

   // Usadas somente para o modelo Tatico-BlocoCurricular:
   int cria_preRestricao_cap_aloc_dem_disc_oft( int campusId );		// Restrição 1.5
   int cria_preRestricao_fixa_nao_compartilhamento( int campusId );	// Restricao 1.17
   
   // Usadas somente para o modelo Tatico-Aluno:
   int cria_preRestricao_atendimento_aluno( int campusId, int cjtAlunosId  );			 // Restricao 1.18
   int cria_preRestricao_aluno_unica_turma_disc( int campusId, int cjtAlunosId  );	 // Restricao 1.19
   int cria_preRestricao_aluno_discPraticaTeorica( int campusId, int cjtAlunosId  );	 // Restricao 1.20
   int cria_preRestricao_prioridadesDemanda( int campus, int prior, int cjtAlunosId  );// Restricao 1.21



   /********************************************************************
   **                    CRIAÇÃO DE VARIAVEIS DO TATICO               **
   *********************************************************************/
      
   int cria_variaveis_aluno_sh( int campusId, int cjtAlunosId, int prioridade );

   int cria_variavel_creditos( int campusId, int cjtAlunosId, int P );									// x_{i,d,u,tps,t}
   int cria_variavel_oferecimentos( int campusId, int cjtAlunosId, int P );							// o_{i,d,u,tps,t}
   int cria_variavel_abertura( int campusId, int cjtAlunosId, int P );									// z_{i,d,cp}
   int cria_variavel_consecutivos( int campusId, int cjtAlunosId, int P );								// c_{i,d,t}   
   int cria_variavel_de_folga_dist_cred_dia_superior( int campusId, int cjtAlunosId, int P );			// fcp_{d,t}
   int cria_variavel_de_folga_dist_cred_dia_inferior( int campusId, int cjtAlunosId, int P );			// fcm_{d,t}
   int cria_variavel_combinacao_divisao_credito( int campusId, int cjtAlunosId, int P );				// m_{i,d,k}
   int cria_variavel_de_folga_combinacao_divisao_credito( int campusId, int cjtAlunosId, int P );		// fkm{i,d,k} e fkp_{i,d,k}
   int cria_variavel_abertura_compativel( int campusId, int cjtAlunosId, int P );						// zc_{d,t}
   int cria_variavel_maxCreds_combina_sl_sala( int campusId, int cjtAlunosId );					// cs_{s,t,k} -> Usado somente quando tem 2 semanas letivas
   
   // Só para modelo com alunos
   int cria_variavel_maxCreds_combina_Sl_aluno( int campusId, int cjtAlunosId, int P );				// ca_{a,t,k}
   int cria_variavel_min_creds_aluno( int campusId, int cjtAlunosId, int P );							// h_{a}
   int cria_variavel_max_creds_aluno( int campusId, int cjtAlunosId, int P );							// H_{a}
   int cria_variavel_de_folga_demanda_disciplina_aluno( int campusId, int cjtAlunosId, int P );		// fd_{i,d,cp}
   int cria_variavel_folgafolga_demanda_p_t( int campusId, int cjtAlunosId, int P );					// ffd_{i1,-d,i2,d,cp}
   int cria_variavel_folga_aluno_unids_distintas_dia( int campusId, int cjtAlunosId, int P );			// fu_{i1,d1,i2,d2,t}

 
 
   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO TATICO                    **
   *********************************************************************/
      
   int cria_restricoes_aluno_sh( int campusId, int cjtAlunosId );

   int cria_restricao_carga_horaria( int campusId, int cjtAlunosId );				// Restricao 1.2.2 
   int cria_restricao_max_tempo_sd( int campusId, int cjtAlunosId );				// Restricao 1.2.3  -> Usada somente quando só tem 1 semana letiva
   int cria_restricao_ativacao_var_o( int campusId, int cjtAlunosId );				// Restricao 1.2.5 
   int cria_restricao_evita_sobreposicao( int campusId, int cjtAlunosId );			// Restricao 1.2.6 
   int cria_restricao_lim_cred_diar_disc( int campusId, int cjtAlunosId );			// Restricao 1.2.13   
   int cria_restricao_turma_disc_dias_consec( int campusId, int cjtAlunosId );		// Restricao 1.2.17
   int cria_restricao_de_folga_dist_cred_dia( int campusId, int cjtAlunosId );		// Restricao 1.2.22   
   int cria_restricao_abre_turmas_em_sequencia( int campusId, int cjtAlunosId, int prioridade );	// Restricao 1.2.25
   int cria_restricao_divisao_credito( int campusId, int cjtAlunosId );				// Restricao 1.2.26
   int cria_restricao_combinacao_divisao_credito( int campusId, int cjtAlunosId );	// Restricao 1.2.27   
   int cria_restricao_ativacao_var_zc( int campusId, int cjtAlunosId );				// Restricao 1.2.31
   int cria_restricao_disciplinas_incompativeis( int campusId, int cjtAlunosId );	// Restricao 1.2.32
   int cria_restricao_ativacao_var_cs( int campusId, int cjtAlunosId );				// Restricao 1.2.49 -> Usado somente quando há 2 semanas letivas 
   int cria_restricao_max_tempo_s_t_SL( int campusId, int cjtAlunosId );			// Restricao 1.2.3.b -> Usada somente quando tem 2 semanas letivas


   // Só para modelo tatico - aluno - sem horarios
   int cria_restricao_max_cred_disc_aluno( int campusId, int cjtAlunosId );
   int cria_restricao_max_creds_semana_aluno( int campusId, int cjtAlunosId ); 
   int cria_restricao_min_creds_semana_aluno( int campusId, int cjtAlunosId ); 
   int cria_restricao_evita_sobrepos_turmas_mesmos_alunos( int campusId, int cjtAlunosId );
   int cria_restricao_aloc_dem_disc( int campusId, int cjtAlunosId ); 
   int cria_restricao_limita_abertura_turmas_aluno( int campusId, int cjtAlunosId );
   int cria_restricao_disc_pratica_teorica( int campusId, int cjtAlunosId );
   int cria_restricao_ativacao_var_ca( int campusId, int cjtAlunosId );
   int cria_restricao_aluno_unid_dif_dia( int campusId, int cjtAlunosId );

   

#endif TATICO_CJT_ALUNOS




#ifdef SEM_CJT_ALUNOS
      
   void carregaVariaveisSolucaoTaticoPorAluno( int campusId, int prioridade  );

   /********************************************************************
   **                      Variaveis do pre-tatico                    **
   *********************************************************************/

   int cria_preVariaveis( int campusId, int prioridade );

   int cria_preVariavel_creditos( int campusId );						// x_{i,d,s}
   int cria_preVariavel_oferecimentos( int campusId );					// o_{i,d,s}
   int cria_preVariavel_abertura( int campusId );						// z_{i,d,cp}
   int cria_preVariavel_alunos( int campusId );							// a_{i,d,oft,s}
   int cria_preVariavel_aloc_alunos( int campusId );					// b_{i,d,c}
   int cria_preVariavel_folga_demanda_disciplina_aluno( int campusId );	// fd_{d,a}
   int cria_preVariavel_folga_demanda_disciplina_oft( int campusId );	// fd_{d,oft}
   int cria_preVariavel_folga_compartilhamento_incomp( int campusId );	// bs_{i,d,c1,c2}
   int cria_preVariavel_folga_proibe_compartilhamento( int campusId );	// fc_{i,d,c1,c2}
   int cria_preVariavel_folga_turma_mesma_disc_sala_dif( int campusId );// fs_{d,s,oft}
   int cria_preVariavel_limite_sup_creds_sala( int campusId );			// Hs_{cp}
   int cria_preVariavel_aloca_alunos_oferta( int campusId );			// c_{i,d,oft,s}
	
   // Usadas somente para o modelo Tatico-Aluno:
   int cria_preVariavel_aloca_aluno_turma_disc( int campusId );			// s_{i,d,a,cp}
   int cria_preVariavel_folga_prioridade_inf( int campusId, int prior );// fpi_{a}
   int cria_preVariavel_folga_prioridade_sup( int campusId, int prior );// fps_{a}

   /********************************************************************
   **                    Restrições do pre-Tatico                     **
   *********************************************************************/

   int cria_preRestricoes( int campusId, int prioridade );

   int cria_preRestricao_carga_horaria( int campusId );				// Restrição 1.1
   int cria_preRestricao_max_cred_sala_sl( int campusId );			// Restrição 1.2
   int cria_preRestricao_ativacao_var_o( int campusId );			// Restrição 1.3
   int cria_preRestricao_evita_mudanca_de_sala( int campusId );		// Restrição 1.4
   int cria_preRestricao_cap_aloc_dem_disc_oft( int campusId );		// Restrição 1.5
   int cria_preRestricao_cap_aloc_dem_disc_aluno( int campusId );	// Restrição 1.5
   int cria_preRestricao_aluno_curso_disc( int campusId );			// Restrição 1.6
   int cria_preRestricao_cap_sala( int campusId );					// Restrição 1.7
   int cria_preRestricao_compartilhamento_incompat( int campusId );	// Restrição 1.8
   int cria_preRestricao_proibe_compartilhamento( int campusId );	// Restrição 1.9
   int cria_preRestricao_ativacao_var_z( int campusId );			// Restricao 1.10
   int cria_preRestricao_evita_turma_disc_camp_d( int campusId );	// Restricao 1.11
   int cria_preRestricao_limita_abertura_turmas( int campusId );    // Restricao 1.12
   int cria_preRestricao_abre_turmas_em_sequencia( int campusId );  // Restricao 1.13
   int cria_preRestricao_turma_mesma_disc_sala_dif( int campusId ); // Restricao 1.14
   int cria_preRestricao_limite_sup_creds_sala( int campusId );		// Restricao 1.15
   int cria_preRestricao_ativa_var_aloc_aluno_oft( int campusId );	// Restricao 1.16

   // Usadas somente para o modelo Tatico-BlocoCurricular:
   int cria_preRestricao_fixa_nao_compartilhamento( int campusId );	// Restricao 1.17
   
   // Usadas somente para o modelo Tatico-Aluno:
   int cria_preRestricao_atendimento_aluno( int campusId );			 // Restricao 1.18
   int cria_preRestricao_aluno_unica_turma_disc( int campusId );	 // Restricao 1.19
   int cria_preRestricao_aluno_discPraticaTeorica( int campusId );	 // Restricao 1.20
   int cria_preRestricao_prioridadesDemanda( int campus, int prior );// Restricao 1.21
   

   /********************************************************************
   **                    CRIAÇÃO DE VARIAVEIS DO TATICO               **
   *********************************************************************/

   int cria_variaveis( int campusId );

   int cria_variaveis_aluno_sh( int campusId );

   int cria_variavel_creditos( int campusId );									// x_{i,d,u,tps,t}
   int cria_variavel_oferecimentos( int campusId );								// o_{i,d,u,tps,t}
   int cria_variavel_abertura( int campusId );									// z_{i,d,cp}
   int cria_variavel_alunos( int campusId );									// a_{i,d,oft}
   int cria_variavel_aloc_alunos( int campusId );								// b_{i,d,c,cp}
   int cria_variavel_consecutivos( int campusId );								// c_{i,d,t}   
   int cria_variavel_aloc_disciplina( int campusId );							// y_{i,d,tps,u}   
   int cria_variavel_de_folga_dist_cred_dia_superior( int campusId );			// fcp_{d,t}
   int cria_variavel_de_folga_dist_cred_dia_inferior( int campusId );			// fcm_{d,t}
   int cria_variavel_abertura_subbloco_de_blc_dia_campus( int campusId );		// r_{bc,t,cp}
   int cria_variavel_de_folga_aloc_alunos_curso_incompat( int campusId );		// bs_{i,d,c,cp}
   int cria_variavel_de_folga_demanda_disciplina( int campusId );				// f_{d,o}
   int cria_variavel_combinacao_divisao_credito( int campusId );				// m_{i,d,k}
   int cria_variavel_de_folga_combinacao_divisao_credito( int campusId );		// fk_{i,d,k}
   int cria_variavel_creditos_modificada( int campusId );						// xm_{d,t}
   int cria_variavel_abertura_compativel( int campusId );						// zc_{d,t}
   int cria_variavel_abertura_bloco_mesmoTPS( int campusId );					// n_{bc,tps}
   int cria_variavel_de_folga_abertura_bloco_mesmoTPS( int campusId );			// fn_{bc,tps}
   int cria_variavel_de_folga_compartilhamento( int campusId );					// fc_{i,d,c1,c2,cp}
   int cria_variavel_maxCreds_combina_sl_sala( int campusId );					// cs_{s,t,k} -> Usado somente quando tem 2 semanas letivas
   
   // Só para modelo com alunos
   int cria_variavel_maxCreds_combina_Sl_aluno( int campusId );					// ca_{a,t,k}
   int cria_variavel_min_creds_aluno( int campusId );							// h_{a}
   int cria_variavel_max_creds_aluno( int campusId );							// H_{a}
   int cria_variavel_de_folga_demanda_disciplina_aluno( int campusId );			// fd_{i,d,cp}
   int cria_variavel_folgafolga_demanda_p_t( int campusId );					// ffd_{i1,-d,i2,d,cp}
   int cria_variavel_folga_aluno_unids_distintas_dia( int campusId );			// fu_{i1,d1,i2,d2,t}

   // Só para modelo com blocos curriculares
   int cria_variavel_min_creds( int campusId );									// h_{bc,i}
   int cria_variavel_max_creds( int campusId );									// H_{bc,i}
   int cria_variavel_num_subblocos( int campusId );								// w_{bc,t,cp}
   int cria_variavel_num_abertura_turma_bloco( int campusId );					// v_{bc,t}
   int cria_variavel_aloc_alunos_oft( int campusId );							// e_{i,d,oft}
   int cria_variavel_creditos_oferta( int campusId );							// q_{i,d,oft,u,tps,t}
   int cria_variavel_aloc_alunos_parOft( int campusId );						// of_{i,d,oft1,oft2}
   int cria_variavel_creditos_parOferta( int campusId );						// p_{i,d,oft1,oft2,u,tps,t}
   int cria_variavel_min_hor_disc_oft_dia( int campusId );						// g_{d,oft,t}
   int cria_variavel_maxCreds_combina_Sl_bloco( int campusId );					// cbc_{bc,t,k} -> Usado somente quando tem 2 semanas letivas



   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO TATICO                    **
   *********************************************************************/

   int cria_restricoes( int campusId );
   
   int cria_restricoes_aluno_sh( int campusId );

   int cria_restricao_carga_horaria( int campusId );					// Restricao 1.2.2
   int cria_restricao_max_tempo_sd( int campusId );						// Restricao 1.2.3 -> Usada somente quando só tem 1 semana letiva
   int cria_restricao_min_cred_dd( int campusId );						// Restricao 1.2.4
   int cria_restricao_ativacao_var_o( int campusId );					// Restricao 1.2.5
   int cria_restricao_evita_sobreposicao( int campusId );			    // Restricao 1.2.6
   int cria_restricao_disciplina_sala( int campusId );					// Restricao 1.2.7
   int cria_restricao_turma_sala( int campusId );					    // Restricao 1.2.8
   int cria_restricao_evita_turma_disc_camp_d( );						// Restricao 1.2.9
   int cria_restricao_turmas_bloco( int campusId );						// Restricao 1.2.10
   int cria_restricao_num_tur_bloc_dia_difunid( int campusId );			// Restricao 1.2.12
   int cria_restricao_lim_cred_diar_disc( int campusId );			    // Restricao 1.2.13
   int cria_restricao_cap_aloc_dem_disc( int campusId );				// Restricao 1.2.14
   int cria_restricao_cap_sala_compativel_turma( int campusId );		// Restricao 1.2.15
   int cria_restricao_cap_sala_unidade( int campusId );					// Restricao 1.2.16
   int cria_restricao_turma_disc_dias_consec( int campusId );		    // Restricao 1.2.17
   int cria_restricao_aluno_curso_disc( int campusId );					// Restricao 1.2.20   
   int cria_restricao_alunos_cursos_incompat( int campusId );			// Restricao 1.2.21
   int cria_restricao_de_folga_dist_cred_dia( int campusId );		    // Restricao 1.2.22
   int cria_restricao_ativacao_var_r( int campusId );					// Restricao 1.2.23
   int cria_restricao_limita_abertura_turmas( int campusId );			// Restricao 1.2.24
   int cria_restricao_abre_turmas_em_sequencia( int campusId );	// Restricao 1.2.25
   int cria_restricao_divisao_credito( int campusId );					// Restricao 1.2.26
   int cria_restricao_combinacao_divisao_credito( int campusId );		// Restricao 1.2.27
   int cria_restricao_ativacao_var_y( int campusId );		            // Restricao 1.2.28
   int cria_restricao_max_creds_disc_dia( int campusId );				// Restricao 1.2.29
   int cria_restricao_ativacao_var_zc( int campusId );					// Restricao 1.2.31
   int cria_restricao_disciplinas_incompativeis( int campusId );		// Restricao 1.2.32
   int cria_restricao_proibe_compartilhamento( int campusId );			// Restricao 1.2.35   
   int cria_restricao_ativacao_var_cs( int campusId );					// Restricao 1.2.49-> Usado somente quando há 2 semanas letivas
   int cria_restricao_fixa_nao_compartilhamento( int campusId );		// Restricao 1.2.50-> Usado somente quando há 2 semanas letivas
   int cria_restricao_max_tempo_s_t_SL( int campusId );					// Restricao 1.2.3.b -> Usada somente quando tem 2 semanas letivas


   // Só para modelo tatico - aluno - sem horarios
   int cria_restricao_max_cred_disc_aluno( int campusId );
   int cria_restricao_max_creds_semana_aluno( int campusId );
   int cria_restricao_min_creds_semana_aluno( int campusId );
   int cria_restricao_evita_sobrepos_turmas_mesmos_alunos( int campusId );
   int cria_restricao_aloc_dem_disc( int campusId );
   int cria_restricao_limita_abertura_turmas_aluno( int campusId );
   int cria_restricao_disc_pratica_teorica( int campusId );
   int cria_restricao_ativacao_var_ca( int campusId );
   int cria_restricao_aluno_unid_dif_dia( int campusId );

   // Só para modelo tatico com bloco curricular
   int cria_restricao_max_cred_disc_bloco( int campusId );
   int cria_restricao_min_creds_turm_bloco( int campusId );				// Restricao 1.2.18
   int cria_restricao_max_creds_turm_bloco( int campusId );				// Restricao 1.2.19
   int cria_restricao_max_creds_bloco_dia( int campusId );				// Restricao 1.2.30
   int cria_restricao_abertura_bloco_mesmoTPS( int campusId );			// Restricao 1.2.33
   int cria_restricao_folga_abertura_bloco_mesmoTPS( int campusId );	// Restricao 1.2.34
   int cria_restricao_ativacao_var_e( int campusId );					//Restricao 1.2.36
   int cria_restricao_evita_sobrepos_sala_por_compartilhamento( int campusId );	// Restricao 1.2.37
   int cria_restricao_ativacao_var_of( int campusId );					//Restricao 1.2.38
   int cria_restricao_ativacao_var_p( int campusId );					//Restricao 1.2.39, 1.2.40, 1.2.41
   int cria_restricao_ativacao_var_g( int campusId );					//Restricao 1.2.42
   int cria_restricao_evita_sobrepos_sala_por_div_turmas( int campusId ); //Restricao 1.2.43
   int cria_restricao_ativacao_var_q( int campusId );					// Restricao 1.2.44, 1.2.45, 1.2.46
   int cria_restricao_ativacao_var_cbc( int campusId );					// Restricao 1.2.51
   
#endif SEM_CJT_ALUNOS


#pragma region TATICO ALUNO COM HORARIOS

   /********************************************************************
   **             CRIAÇÃO DE VARIAVEIS DO TATICO-ALUNO                **
   *********************************************************************/

   
   int criaVariaveisTatico( int campusId );

   int criaVariavelTaticoCreditos( int campusId );									// x_{i,d,u,s,hi,hf,t}      
   int criaVariavelTaticoAbertura( int campusId );									// z_{i,d,cp}
   int criaVariavelTaticoConsecutivos( int campusId );								// c_{i,d,t}
   int criaVariavelTaticoMinCreds( int campusId );									// h_{a}
   int criaVariavelTaticoMaxCreds( int campusId );									// H_{a}
   int criaVariavelTaticoCombinacaoDivisaoCredito( int campusId );					// m_{i,d,k}   
   int criaVariavelTaticoFolgaCombinacaoDivisaoCredito( int campusId );				// fkp_{i,d,k} e fkm_{i,d,k}
   int criaVariavelTaticoFolgaDistCredDiaSuperior( int campusId );					// fcp_{d,t}   
   int criaVariavelTaticoFolgaDistCredDiaInferior( int campusId );					// fcm_{d,t}
   int criaVariavelTaticoAberturaCompativel( int campusId );						// zc_{d,t}
//   int criaVariavelTaticoFolgaDemandaDiscAluno( int campusId );					// fd_{d,a}   
   int criaVariavelTaticoFolgaDemandaDisc( int campusId );							// fd_{i,d,cp}
   int criaVariavelTaticoAlunoUnidDia( int campusId );								// y_{a,u,t} 
   int criaVariavelTaticoAlunoUnidadesDifDia( int campusId );						// w_{a,t}

   int criaVariavelTaticoFolgaAlunoUnidDifDia( int campusId );						// fu_{i1,d1,i2,d2,t,cp}
   int criaVariavelTaticoFolgaFolgaDemandaPT( int campusId );						// ffd_{i1,-d,i2,d,cp}

   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO TATICO-ALUNO              **
   *********************************************************************/

   int criaRestricoesTatico( int campusId );

   int criaRestricaoTaticoCargaHoraria( int campusId );					// Restricao 1.2.2   
   int criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId );		// Restricao 1.2.3   
   int criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId ); // Restricao 1.2.4
//   int criaRestricaoTaticoAtendeDemandaAluno( int campusID );			// Restricao 1.2.5   
   int criaRestricaoTaticoAtendeDemanda( int campusID );				// Restricao 1.2.5   
   int criaRestricaoTaticoTurmaDiscDiasConsec( int campusId );		    // Restricao 1.2.6
   int criaRestricaoTaticoLimitaAberturaTurmas( int campusId );			// Restricao 1.2.7
   int criaRestricaoTaticoDivisaoCredito( int campusId );				// Restricao 1.2.8      
   int criaRestricaoTaticoCombinacaoDivisaoCredito( int campusId );		// Restricao 1.2.9
   int criaRestricaoTaticoAtivacaoVarZC( int campusId );				// Restricao 1.2.10
   int criaRestricaoTaticoDisciplinasIncompativeis( int campusId );		// Restricao 1.2.11
   int criaRestricaoTaticoEvitaSobreposicaoAulaAluno( int campusId );	// Restricao 1.2.12
   int criaRestricaoTaticoAtivaY( int campusId );						// Restricao 1.2.13
   int criaRestricaoTaticoAlunoUnidadesDifDia( int campusId );			// Restricao 1.2.14
   int criaRestricaoTaticoMinCreds( int campusId );						// Restricao 1.2.15
   int criaRestricaoTaticoMaxCreds( int campusId );						// Restricao 1.2.16
//   int criaRestricaoTaticoAlunoDiscPraticaTeorica( int campusId );	// Restricao 1.2.17
   int criaRestricaoTaticoDiscPraticaTeorica( int campusId );			// Restricao 1.2.17
   int criaRestricaoTaticoAlunoUnidDifDia( int campusId );

  // int criaRestricaoTaticoFixaDistribCredDia( int campusId );			 //TODO

// Restrições já garantidas pelo pre-modelo:
//   int criaRestricaoTaticoAbreTurmasEmSequencia( int campusId );	
//   int criaRestricaoTaticoAlunoCursoTurmaDisc( int campusId );	
//   int criaRestricaoTaticoCursosIncompat( int campusId );				
//   int criaRestricaoTaticoProibeCompartilhamento( int campusId );	
//   int criaRestricaoTaticoEvitaTurmaDiscCampiDif();					
//   int criaRestricaoTaticoUnicaSalaParaTurmaDisc( int campusId );	
   
   
#pragma endregion TATICO ALUNO COM HORARIOS




   /********************************************************************
   **              CRIAÇÃO DE VARIAVEIS DO OPERACIONAL                **
   *********************************************************************/

   int criaVariaveisOperacional( void );

   int criaVariavelProfessorAulaHorario( void );
   int criaVariavelProfessorDisciplina( void );
   int criaVariavelDisciplinaHorario( void );
   int criaVariavelFolgaFixProfDiscSalaDiaHor( void );
   int criaVariavelFolgaFixProfDiscDiaHor( void );
   int criaVariavelFolgaFixProfDisc( void );
   int criaVariavelFolgaFixProfDiscSala( void );
   int criaVariavelFolgaFixProfSala( void );
   int criaVariavelFolgaDisciplinaHorario( void );
   int criaVariavelProfessorCurso( void );
   int criaVariavelAvaliacaoCorpoDocente( void );
   int criaVariavelCustoCorpoDocente( void );
   int criaVariavelDiasProfessoresMinistramAulas( void );
   int criaVariavelFolgaUltimaPrimeiraAulas( void );
   int criaVariavelFolgaMinimoMestresCurso( void );
   int criaVariavelFolgaMinimoDoutoresCurso( void );
   int criaVariavelMaxDiscProfCurso( void );
   int criaVariavelFolgaMaxDiscProfCurso( void );
   int criaVariavelFolgaCargaHorariaMinimaProfessor( void );
   int criaVariavelFolgaCargaHorariaMinimaProfessorSemana( void );
   int criaVariavelFolgaCargaHorariaMaximaProfessorSemana( void );
   int criaVariavelGapsProfessores( void );
   int criaVariavelFolgaDemanda( void );
   int criaVariavelFolgaDisciplinaTurmaHorario( void );
   int criaVariavelProfessorDiaHorarioIF();
   int criaVariavelNroProfsAlocadosCurso();


   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO OPERACIONAL               **
   *********************************************************************/

   int criaRestricoesOperacional( void );

   int criaRestricaoSalaHorario( void );
   int criaRestricaoProfessorHorario( void );
   int criaRestricaoAlunoHorario( void );
   int criaRestricaoBlocoHorario( void );
   int criaRestricaoBlocoHorarioDisc( void );
   int criaRestricaoAlocAula( void );
   int criaRestricaoProfessorDisciplina( void );
   int criaRestricaoProfessorDisciplinaUnico( void );
   int criaRestricaoFixProfDiscSalaDiaHor( void );
   int criaRestricaoFixProfDiscDiaHor( void );
   int criaRestricaoFixProfDisc( void );
   int criaRestricaoFixProfDiscSala( void );
   int criaRestricaoFixProfSala( void );
   int criaRestricaoDisciplinaMesmoHorario( void );
   int criaRestricaoDisciplinaHorarioUnico( void );
   int criaRestricaoDeslocamentoViavel( void ); // x3
   int criaRestricaoDeslocamentoProfessor( void ); // x5
   int criaRestricaoAvaliacaoCorpoDocente( void ); // x6
   int criaRestricaoCustoCorpoDocente( void ); // x7
   int criaRestricaoRelacionaVariavelXDiaProf( void ); // x9
   int criaRestricaoCargaHorariaMinimaProfessor( void ); // x10
   int criaRestricaoUltimaPrimeiraAulas( void ); // x11
   int criaRestricaoAlocacaoProfessorCurso( void ); // x12
   int criaRestricaoMinimoMestresCurso( void ); // x12
   int criaRestricaoMinimoDoutoresCurso( void ); // x12
   int criaRestricaoMaxDiscProfCurso( void ); // x13
   int criaRestricaoCargaHorariaMinimaProfessorSemana( void ); // x14
   int criaRestricaoCargaHorariaMaximaProfessorSemana( void ); // x15
   int criaRestricaoGapsProfessores( void );
   int criaRestricaoProfHorarioMultiUnid( void );
   int criaRestricaoGapsHorariosProfessores();
   int criaRestricaoCalculaNroProfsAlocadosCurso();

   void cria_solucao_inicial( int , int * , double * );
   int localBranching( double *, double );
   void carregaVariaveisSolucaoTatico( int campusId );
   void relacionaProfessoresDisciplinas();
   void carregaVariaveisSolucaoPreTatico( int campusId, int prioridade );
   void preencheMapAtendimentoAluno( int campusId );

   void limpaMapAtendimentoAlunoPrioridadeAnterior( int campusId );
   
   void removeAtendimentosParciais( double *xSol, char solFilename[1024] );

   void imprimeAlocacaoAlunos( int campusId, int prioridade, int cjtAlunosId );


#ifdef TATICO_CJT_ALUNOS
   void carregaVariaveisSolucaoPreTatico_CjtAlunos( int campusId, int prioridade, int cjtAlunosId );
   void carregaVariaveisSolucaoTaticoPorAluno_CjtAlunos( int campusId, int prioridade, int cjtAlunosId );
   int solveTaticoPorCampusCjtAlunos();
   int solvePreTaticoCjtAlunos( int campusId, int prioridade, int cjtAlunosId );
   int solveTaticoBasicoCjtAlunos( int campusId, int prioridade, int cjtAlunosId );
   int fixaLimiteInferiorVariavelPre_CjtAlunos( VariablePre *v );
   int fixaLimiteSuperiorVariavelPre_CjtAlunos( VariablePre *v );

   int fixaLimitesVariavelTaticoPriorAnterior( Variable *v, bool &FOUND );
   int fixaLimitesVariavelTaticoCjtAlunosAnterior( Variable *v );

   bool NAO_CRIAR_RESTRICOES_CJT_ANTERIORES;
   bool FIXAR_P1;

#endif

#ifdef SEM_CJT_ALUNOS
   int solveTaticoPorCampus();
   int solvePreTatico( int campusId, int prioridade );
   int solveTaticoBasico( int campusId, int prioridade );
   int solveTatico( int campusId );
#endif

   void converteCjtSalaEmSala();
   void mudaCjtSalaParaSala();
   void separaDisciplinasEquivalentes();
   void getSolutionTatico();
   void getSolutionTaticoPorAluno();

   int solveOperacional();
   int solveOperacionalMIP();
   void getSolutionOperacional();
   void getSolutionOperacionalMIP();
   void geraProfessoresVirtuaisMIP();
   Professor * criaProfessorVirtual( HorarioDia *, int,
      std::set< std::pair< Professor *, HorarioDia * > > & );
   void preencheOutputOperacional( ProblemSolution * );
   void preencheOutputOperacionalMIP( ProblemSolution * );
   bool aulaAlocada( Aula *, Campus *, Unidade *, Sala *, int );
   std::vector< Variable * > variaveisAlunosAtendidos( Curso *, Disciplina * );
   std::vector< Variable * > variaveisCreditosAtendidos( Disciplina * );
   void criaVariaveisAlunosDisciplinasSubstituidas();
   void criaVariaveisCreditosDisciplinasSubstituidas();

   void relacionaAlunosDemandas();
   Unidade* retornaUnidadeDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
   ConjuntoSala* retornaSalaDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
   GGroup< std::pair< int,Disciplina* > > retornaAtendEmCjtSala( ConjuntoSala * cjtSala );

   Variable * criaVariavelAlunos(
      Campus *, Unidade *, ConjuntoSala *, Sala *,
      int, Oferta *, Curso *, Disciplina *, int );

   Variable * criaVariavelCreditos(
      Campus *, Unidade *, ConjuntoSala *, Sala *,
      int, Oferta *, Curso *, Disciplina *, int, BlocoCurricular * );

private:

	bool CARREGA_SOLUCAO;

	int TEMPO_PRETATICO, TEMPO_TATICO;

	std::string getPreLpFileName( int campusId, int prioridade, int cjtAlunosId );

	std::string getTaticoLpFileName( int campusId, int prioridade, int cjtAlunosId );

	std::string getSolBinFileName( int campusId, int prioridade, int cjtAlunosId );

	std::string getSolPreBinFileName( int campusId, int prioridade, int cjtAlunosId );

	std::string getSolucaoPreTaticoFileName( int campusId, int prioridade, int cjtAlunosId );	
	
	std::string getSolucaoTaticoFileName( int campusId, int prioridade, int cjtAlunosId );	

	// Filtro para a criação das variaveis do pre-modelo,
	// caso haja solução do tatico para iteração de prioridade de demanda anterior
   int fixaLimiteInferiorVariavelPre( VariablePre *v );

	// Filtro para a criação das variaveis do modelo tatico, caso haja solução do pre-modelo
   bool criaVariavelTatico( Variable *v );

   	// Filtro para a criação das variaveis do modelo tatico que considera horarios e alunos, caso haja solução do pre-modelo
   bool criaVariavelTatico( VariableTatico *v );

   // Dada uma disciplina 'A' que foi substituída por uma de suas disciplinas equivalentes 'B',
   // esse map informa o conjunto de variáveis que foram criadas para 'B' referentes à disciplina 'A'
   std::map< Disciplina *, std::vector< Variable > > mapVariaveisDisciplinasEquivalentes;

   std::vector< Variable > filtraVariaveisAlunos( std::vector< Variable > );
   std::vector< Variable > filtraVariaveisCreditos( std::vector< Variable > );

   void retornaHorariosPossiveis( Professor *, Aula *, std::list< HorarioDia * > & );

   // Vetor responsável por armazenar ponteiros para todas as
   // variáveis do tipo V_CREDITOS com credito(s) alocado(s).
   typedef GGroup< VariableTatico * > vars__X__i_d_u_s_hi_hf_t;

   // Vetor responsável por armazenar ponteiros para todas as
   // variáveis do  tipo V_CREDITOS com credito(s) alocado(s).
   typedef vector< Variable * > vars__X___i_d_u_tps_t;

   // Estrutura responsável por armazenar referências para todas variáveis
   // do tipo V_ALUNOS que possuirem algum valor de atendimento maior que 0.
   typedef std::map< std::pair< int /*turma*/, Disciplina * >,
					      std::vector< Variable * > > vars__A___i_d_o;

   void chgCoeffList( std::vector< std::pair< int, int > > , std::vector< double > );

   vars__X__i_d_u_s_hi_hf_t vars_xh;

   vars__X___i_d_u_tps_t vars_x;
   vars__A___i_d_o vars_a;

   ProblemSolution * problemSolution;
   ProblemDataLoader * problemDataLoader;

   // The linear problem.
   OPT_LP * lp;

   // Hash which associates the column number with the VariablePre object.
   VariablePreHash vHashPre;

   // Hash which associates the row number with the ConstraintPre object.
   ConstraintPreHash cHashPre;

   // Hash which associates the column number with the VariableTatico object.
   VariableTaticoHash vHashTatico;

   // Hash which associates the row number with the ConstraintTatico object.
   ConstraintTaticoHash cHashTatico;

   // Hash which associates the column number with the Variable object.
   VariableHash vHash;

   // Hash which associates the row number with the Constraint object.
   ConstraintHash cHash;

   // Hash which associates the column number with the VariableOp object.
   VariableOpHash vHashOp;

   // Hash which associates the row number with the ConstraintOp object.
   ConstraintOpHash cHashOp;

   // Stores the solution variables ( non - zero ).
   std::set< VariablePre > solVarsPre;
	
   GGroup< Variable * > solVars; // usado para armazenar a solução tatica da iteração cjtAluno anterior, a fim de fazer a fixação de valores

   std::vector< VariableOp * > solVarsOp;
   
   bool SolVarsPreFound( VariablePre v );
   
   double alpha, beta, gamma, delta, lambda, epsilon, rho, M, psi, tau, eta;

   struct Ordena
   {
      bool operator() ( std::vector< int > xI, std::vector< int > xJ )
      {
         return ( xI.front() > xJ.front() );
      }
   } ordenaPorCreditos;

   void buscaLocalTempoDeslocamentoSolucao();

   ValidateSolutionOp * validateSolution;

   int alteraHorarioAulaAtendimento( const int, const int );

   int calculaDeslocamentoUnidades( const int, const int );
};

#endif
