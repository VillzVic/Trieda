

#ifndef PRE_MODELO_H
#define PRE_MODELO_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "Solver.h"
#include "VariablePre.h"
#include "ConstraintPre.h"
#include "VariableTatico.h"
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

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#endif

#define PRINT_cria_variaveis
#define PRINT_cria_restricoes

class PreModelo : public Solver
{
public:
	
	PreModelo( ProblemData * aProblemData, 
				GGroup< VariablePre *, LessPtr<VariablePre> > *aSolVarsXPre, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *avars_xh,
				bool *endCARREGA_SOLUCAO );

	virtual ~PreModelo();
	
	int solve();
    void getSolution( ProblemSolution * );
	void solvePreTaticoMain( int campusId, int prioridade, int cjtAlunosId, int r );
   

private:

   //enum OutPutFileType
   //{
   //   PRE_TAT_BIN = 0,
   //   PRE_TAT_BIN1 = 1,
   //   PRE_TAT_BIN2 = 2,
   //   PRE_TAT_BIN3 = 3,
   //   PRE_TAT_BIN4 = 4,

	  //PRE_TAT_BIN_INIT = 5
   //};	  

   
 /*******************************************************************************************************************
								CRIAÇÃO DE VARIAVEIS DO PRE-MODELO   
  ******************************************************************************************************************/

   int cria_preVariaveis( int campusId, int prioridade, int grupoAlunosId, int r );

   int add_preVariaveis(  int campusId, int prioridade, int grupoAlunosId, int r );

   int cria_preVariavel_oferecimentos_turno( int campusId, int grupoAlunosId, int P_ATUAL, int r );				// q_{i,d,s,g}
   int cria_preVariavel_creditos( int campusId, int grupoAlunosId, int P_ATUAL, int r );						// x_{i,d,s}
   int cria_preVariavel_oferecimentos( int campusId, int grupoAlunosId, int P_ATUAL, int r );					// o_{i,d,s}
   int cria_preVariavel_abertura( int campusId, int grupoAlunosId, int P_ATUAL, int r );						// z_{i,d,cp}
   int cria_preVariavel_alunos( int campusId, int grupoAlunosId, int P_ATUAL, int r );							// a_{i,d,oft,s}
   int cria_preVariavel_aloc_alunos( int campusId, int grupoAlunosId, int P_ATUAL, int r );						// b_{i,d,c}   
   int cria_preVariavel_folga_compartilhamento_incomp( int campusId, int grupoAlunosId, int P_ATUAL  );	// bs_{i,d,c1,c2} // não esta sendo usada, é restrição forte
   int cria_preVariavel_folga_proibe_compartilhamento( int campusId, int grupoAlunosId, int P_ATUAL, int r );	// fc_{i,d,c1,c2}
   int cria_preVariavel_limite_sup_creds_sala( int campusId );												// Hs_{cp}
   int cria_preVariavel_soma_cred_sala_por_turno( int campusId );												// xcs_{s,g}
   int cria_preVariavel_soma_cred_sala( int campusId );															// xcs_{s}

   int cria_preVariavel_folga_demanda_disciplina_aluno( int campusId, int grupoAlunosAtualId, int P_ATUAL );	// fd_{d,a}
   int cria_preVariavel_folga_prioridade_inf( int campusId, int prior, int grupoAlunosAtualId );			// fpi_{a}
   int cria_preVariavel_folga_prioridade_sup( int campusId, int prior, int grupoAlunosAtualId );			// fps_{a}
   int cria_preVariavel_folga_abre_turma_sequencial( int campusId, int cjtAlunosId, int P_ATUAL, int r );	// ft_{i,d}
   int cria_preVariavel_turmas_compartilhadas( int campusId, int cjtAlunosId, int P_ATUAL );			// w_{i,d,i',d'}
   int cria_preVariavel_folga_distribuicao_aluno( int campusId, int cjtAlunosId, int P_ATUAL );			// fda
   int cria_preVariavel_folga_turma_mesma_disc_sala_dif( int campusId, int grupoAlunosId, int P_ATUAL, int r ); // fs_{d,s}
   int cria_preVariavel_aluno_sala( int campusId, int grupoAlunosId, int P_ATUAL, int r );						// as_{a,s}
   int cria_preVariavel_formandosNaTurma( int campusId, int grupoAlunosId, int P_ATUAL, int r );		// f_{i,d,cp}
   int cria_preVariavel_turmaCalendario( int campusId, int grupoAlunosId, int P_ATUAL, int r );			// v_{i,d,sl,s}
   int cria_preVariavel_turmaTurno( int campusId, int grupoAlunosId, int P_ATUAL, int r );				// v_{i,d,tt,s}
   int cria_preVariavel_nroAlunos_turma( int campusId, int grupoAlunosId, int P_ATUAL, int r );			// n_{i,d}
   int cria_preVariavel_nroAlunos_turma_oferta( int campusId, int grupoAlunosId, int P_ATUAL, int r );	// no_{i,d,oft}   
   int cria_preVariavel_folga_demanda_oferta( int campusId, int grupoAlunosId, int P_ATUAL, int r );		// fd_{d,oft}
   int cria_preVariavel_turma_so_sabado( int campusId, int grupoAlunosId, int P_ATUAL, int r );				// sab_{d,i,s}

   int cria_preVariavel_aloca_aluno_turma_disc( int campusId, int grupoAlunosAtualId, int P_ATUAL, int r );	// s_{i,d,a,cp}
   int cria_preVariavel_aloca_aluno_turma_disc_equiv( int campusId, int P );								// s_{i,d,a}
   int cria_preVariavel_aloca_aluno_turma_disc_turno( int campusId, int P );								// s_{i,d,a,g}
   int cria_preVariavel_aloca_aluno_turma_disc_turno_equiv( int campusId, int P );							// s_{i,d,a,g}
   
   int cria_preVariavel_alunos_mesma_turma_pratica( int campusId, int P );									// ss_{a1,a2,dp}
   
   int cria_preVariavel_folga_minimo_demanda_por_aluno( int campusId, int P_ATUAL );						// fmd_{a}
   int cria_preVariavel_folga_ocupacao_sala( int campusId, int P_ATUAL );									// fos_{i,d,cp}
   int cria_preVariavel_usa_disciplina( int campusId, int P );												// k_{d}
   int cria_preVariavel_usa_periodo_unid( int campusId );													// uu_{u,oft,p}
   int cria_preVariavel_usa_periodo_sala( int campusId );													// us_{s,oft,p}
   int cria_preVariavel_folga_periodo_sala( int campusId, int prioridade );									// fus_{oft,p}		
   int cria_preVariavel_folga_periodo_unid( int campusId, int prioridade );									// fuu_{oft,p}

   int cria_preVariavel_distrib_alunos( int campusId );														// da_{cp,d}
   int cria_preVariavel_folga_disponib_prof( int campusId );												// fp_{d,g}

   // Não estou usando, vamos tentar primeiro tratar co-requisito como pos processamento
   int cria_preVariavel_aluno_correquisito( int campusId, int grupoAlunosAtualId, int P_ATUAL );		// r_{a,k}

   
   
 /*******************************************************************************************************************
								CRIAÇÃO DE RESTRIÇÕES DO PRE-MODELO   
  ******************************************************************************************************************/

   int cria_preRestricoes( int campusId, int prioridade, int cjtAlunosId, int r );

   int add_preRestricoes( int campusId, int prioridade, int cjtAlunosId, int r );

   int cria_preRestricao_carga_horaria_salaUnicaPorTurma( int campusId, int cjtAlunosId );
   
   int cria_preRestricao_carga_horaria( int campusId, int cjtAlunosId  );				// Restrição 1.1
   int cria_preRestricao_max_cred_sala_turno( int campusId, int cjtAlunosId  );
   int cria_preRestricao_set_turno_por_turma( int campusId, int cjtAlunosId  );
   int cria_preRestricao_limita_turno_por_turma( int campusId, int cjtAlunosId  );
   int cria_preRestricao_turno_viavel_por_turma( int campusId, int cjtAlunosId  );
   int cria_preRestricao_turnoIES_viavel_por_turma( int campusId, int cjtAlunosId  );
   int cria_preRestricao_max_cred_sala_sl( int campusId, int cjtAlunosId  );			// Restrição 1.2
   int cria_preRestricao_ativacao_var_o( int campusId, int cjtAlunosId  );			// Restrição 1.3
   int cria_preRestricao_evita_mudanca_de_sala( int campusId, int cjtAlunosId  );		// Restrição 1.4
   int cria_preRestricao_cap_aloc_dem_disc_aluno( int campusId, int cjtAlunosId  );	// Restrição 1.5
   int cria_preRestricao_aluno_curso_disc_peloHash( int campusId, int cjtAlunosId  );
   int cria_preRestricao_cap_sala_peloHash( int campusId, int cjtAlunosId  );
   int cria_preRestricao_compartilhamento_incompat( int campusId, int cjtAlunosId  );	// Restrição 1.8
   int cria_preRestricao_proibe_compartilhamento( int campusId, int cjtAlunosId  );	// Restrição 1.9
   int cria_preRestricao_ativacao_var_z( int campusId, int cjtAlunosId  );			// Restricao 1.10
   int cria_preRestricao_evita_turma_disc_camp_d( int campusId, int cjtAlunosId  );	// Restricao 1.11
   int cria_preRestricao_limita_abertura_turmas_peloHash( int campusId, int cjtAlunosId  );
   int cria_preRestricao_abre_turmas_em_sequencia( int campusId, int cjtAlunosId, int prioridade, int r );  // Restricao 1.13
   int cria_preRestricao_limite_sup_creds_sala( int campusId, int cjtAlunosId  );		// Restricao 1.15
   int cria_preRestricao_soma_cred_sala( int campusId, int cjtAlunosId  );	// Restricao 1.26
   int cria_preRestricao_soma_cred_sala_por_turno( int campusId, int cjtAlunosId  );
   
   // Usadas somente para o modelo Tatico-Aluno:
   int cria_preRestricao_atendimento_aluno_peloHash( int campusId, int cjtAlunosId  );
   int cria_preRestricao_aluno_unica_turma_disc( int campusId, int cjtAlunosId  );	 // Restricao 1.19
   int cria_preRestricao_aluno_discPraticaTeorica_MxN( int campusId, int cjtAlunosId  );	 // Restricao 1.20  
   int cria_preRestricao_aluno_discPraticaTeorica_hash_MxN( int campusId );
   int cria_preRestricao_aluno_discPraticaTeorica_1x1( int campusId );
   int cria_preRestricao_aluno_discPraticaTeorica_1xN( int campusId );
   int cria_preRestricao_aluno_discPraticaTeorica_1xN_antiga( int campusId );
   int cria_preRestricao_alunos_mesma_turma_pratica( int campusId );
   int cria_preRestricao_limite_cred_aluno(int campusId, int cjtAlunosId, int prior); 
   int cria_preRestricao_turma_mesma_disc_sala_dif( int campusId, int cjtAlunosId  ); // Restricao 1.14
   int cria_preRestricao_aluno_sala( int campusId, int cjtAlunosId  );
   int cria_preRestricao_formandos( int campusId, int cjtAlunosId, int prioridade, int r );
   int cria_preRestricao_turma_calendarios_com_intersecao( int campusId, int cjtAlunosId, int prioridade, int r );
   int cria_preRestricao_turma_turnosIES_com_intersecao( int campusId, int cjtAlunosId, int prioridade, int r );
   int cria_preRestricao_setV( int campusId, int cjtAlunosId );
   int cria_preRestricao_distribuicao_aluno( int campusId, int cjtAlunosId  );
   int cria_preRestricao_aloca_aluno_turma_aberta( int campusId, int cjtAlunosId ); // necessaria se não for mais usar a variavel "a"
   int cria_preRestricao_nro_alunos_por_turma( int campusId, int cjtAlunosId );

   int cria_preRestricao_aloca_aluno_turma_aberta2( int campusId, int cjtAlunosId );
   int cria_preRestricao_nro_alunos_por_turma2( int campusId, int cjtAlunosId );
   int cria_preRestricao_cap_sala_peloHash2( int campusId, int cjtAlunosId  );
   int cria_preRestricao_aluno_curso_disc_peloHash2( int campusId, int cjtAlunosId  );
   int cria_preRestricao_limita_abertura_turmas_peloHash2( int campusId, int cjtAlunosId  );
   int cria_preRestricao_aluno_discPraticaTeorica_oferta( int campusId, int cjtAlunosId );
   int cria_preRestricao_cap_aloc_dem_disc_aluno2( int campusId, int cjtAlunosId  );
   int cria_preRestricao_setV2( int campusId, int cjtAlunosId );
   int cria_preRestricao_setV_TurnoIES( int campusId, int cjtAlunosId );
   int cria_preRestricao_aloca_turma_sab( int campusId, int cjtAlunosId );
   int cria_preRestricao_aloca_turma_sab_turnoIES( int campusId, int cjtAlunosId );
   int cria_preRestricao_max_cred_sab( int campusId, int cjtAlunosId );
   int cria_preRestricao_aloc_min_aluno( int campusId, int cjtAlunosId  );
   int cria_preRestricao_folga_ocupacao_sala( int campusId, int cjtAlunosId );
   int cria_preRestricao_tempo_max_aluno_sabado( int campusId );
   int cria_preRestricao_tempo_max_aluno_turno( int campusId );
   int cria_preRestricao_associa_aluno_turno( int campusId );
   int cria_preRestricao_usa_disciplina( int campusId );
   int cria_preRestricao_usa_unid_periodo( int campusId );
   int cria_preRestricao_limita_unid_periodo( int campusId );
   int cria_preRestricao_usa_sala_periodo( int campusId );
   int cria_preRestricao_limita_sala_periodo( int campusId );
   int cria_preRestricao_distrib_alunos( int campusId );	
   int cria_preRestricao_disponib_profs( int campusId );

   int cria_preRestricao_forca_min_atendimento( int campusId, int cjtAlunosId );

   // Só para p2 em diante
   int cria_preRestricao_prioridadesDemanda( int campus, int prior, int cjtAlunosId  );// Restricao 1.21
   int cria_preRestricao_evita_sobrepos_turmas_mesmos_alunos( int campusId, int cjtAlunosId, int prioridade ); //nao precisa, pq tem cliques agora
   int cria_preRestricao_ativa_var_compart_turma( int campusId, int cjtAlunosId, int prior  );
   int cria_preRestricao_maxCredsAlunoDia( int campusId, int cjtAlunosId, int prioridade );
   



 /*******************************************************************************************************************
										OTHER METHODS
  ******************************************************************************************************************/

   
	int localBranchingPre( double *, double );
	void solveAbreTurmasPorFaixasPreTatico(double *xSol, const double maxTime, const int percIni);
	int solvePreTaticoCjtAlunos_testeFases( int campusId, int prioridade, int cjtAlunosId, int r );
	void polishPreTatico(double *xSol, double maxTime, int percIni, int percMin);
    void imprimeSolVarsPre( int campusId, int prioridade, int cjtAlunosId );
	void testeCarregaPreSol( int campusId, int prioridade, int cjtAlunosId, int r );
    void carregaVariaveisSolucaoPreTatico_CjtAlunos( int campusId, int prioridade, int cjtAlunosId, int r );
    int solvePreTaticoCjtAlunos( int campusId, int prioridade, int cjtAlunosId, int r );
	int atendMaximoSemSabado( int campusId, int P, int cjtAlunos, int r, bool &CARREGA_SOL_PARCIAL, double *xSol );
    int solveMaxAtendCalourosFormandos( int campusId, int prioridade, int cjtAlunosId, int r, bool& CARREGA_SOL_PARCIAL, double *xSol );
    int solveMaxAtend( int campusId, int prioridade, int cjtAlunosId, int r, bool& CARREGA_SOL_PARCIAL, double *xSol );
	int solveMaxAtendPorFasesDoDia( int campusId, int prioridade, int cjtAlunosId, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMinTurmasConflitos( int campusId, int prioridade, int cjtAlunosId, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int fixaLimiteSuperiorVariavelPre_CjtAlunos( VariablePre *v );
    void preencheMapAtendimentoAluno( VariablePre *&var );
    void sincronizaSolucao();

	int retornaTempoDeExecucaoPreModelo( int campusId, int cjtAlunosId, int prioridade );

	
	std::string getEquivPreFileName( int campusId, int prioridade, int r );
	std::string getPreLpFileName( int campusId, int prioridade, int cjtAlunosId, int r );
	std::string getSolPreBinFileName( int campusId, int prioridade, int cjtAlunosId, int r, int particao );
	std::string getSolucaoPreTaticoFileName( int campusId, int prioridade, int cjtAlunosId, int r, int particao );	
    std::string getSolVarsPreFileName( int campusId, int prioridade, int cjtAlunosId, int particao );
	
	int readSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type, double *xSol );
	void writeSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type, double *xSol );
	void writeSolTxt( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int particao, int type, double *xSol );
	void writeSolTxt( string solName, double *xSol, bool printZeros );
	void writeSolBin( string solName, double *xSol );

	// Filtro para a criação das variaveis do pre-modelo,
	// caso haja solução do tatico COM horarios para iteração de prioridade de demanda anterior
   int fixaLimitesVariavelPre( VariablePre *v );
   	
   void atualizarDemandasEquiv( int campusId, int prioridade, int r );

   void chgCoeffList( std::vector< std::pair< int, int > > cL, std::vector< double > cLV );


 /*******************************************************************************************************************
										ATTRIBUTES
  ******************************************************************************************************************/


	int nroPreSolAvaliadas;
	int TEMPO_PRETATICO;

   
   bool NAO_CRIAR_RESTRICOES_CJT_ANTERIORES;
   bool FIXAR_P1;
   bool *CARREGA_SOLUCAO;
   bool USAR_EQUIVALENCIA;
   bool PERMITIR_INSERCAO_ALUNODEMANDAP2_EM_TURMAP1;
   bool MODELO_ESTIMA_TURMAS;
   

   // The linear problem.	
   
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif


	// Hash which associates the column number with the VariablePre object.
   VariablePreHash vHashPre;

   // Hash which associates the row number with the ConstraintPre object.
   ConstraintPreHash cHashPre;


   
   // Armazena as pre-variaveis 's'
   GGroup< VariablePre*, LessPtr<VariablePre> > solVarsPreS;

   // Armazena as pre-variaveis 'x'
   GGroup< VariablePre *, LessPtr<VariablePre> > solVarsXPre;  
         
   // Armazena referencia para a solução as variaveis 'x' armazenada em SolverMIP
   GGroup< VariablePre *, LessPtr<VariablePre> > *pSolVarsXPre;



	// Vetor responsável por armazenar ponteiros para todas as
    // variáveis do tipo V_CREDITOS com credito(s) alocado(s).
    typedef GGroup< VariableTatico *, LessPtr<VariableTatico> > vars__X__i_d_u_s_hi_hf_t;

	// Cópia da solução tática, caso haja rodadas passadas
    vars__X__i_d_u_s_hi_hf_t vars_xh;
   
	
};


#endif