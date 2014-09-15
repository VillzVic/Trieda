#ifndef _TATICO_INT_ALUNO_HOR_H_
#define _TATICO_INT_ALUNO_HOR_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "Solver.h"
#include "VariableTatico.h"
#include "VariableTatInt.h"
#include "ConstraintTatInt.h"
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

class TaticoIntAlunoHor : public Solver
{
public:
	
	TaticoIntAlunoHor( ProblemData * aProblemData, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *aSolVarsTatico, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *avars_xh,
				bool *endCARREGA_SOLUCAO, bool equiv, int permitirNovasTurmas );

	virtual ~TaticoIntAlunoHor();


	int solve();
    void getSolution( ProblemSolution * );
	void solveTaticoIntegrado( int campusId, int prioridade, int r );
	void solveTaticoIntegradoBranchSalas( int campusId, int prioridade, int r );
	
   enum Etapas
   {
	  Etapa1 = 1,
	  Etapa2 = 2,				
	  Etapa3 = 3,				
	  Etapa4 = 4,
	  Etapa5 = 5
   };

private:

   enum OutPutFileType
   {
	  TAT_INT_BIN = 0,
	  TAT_INT_BIN1 = 1,				
	  TAT_INT_BIN2 = 2,		
	  TAT_INT_BIN3 = 3,
	  TAT_INT_CALOURO_BIN = 4
   };	  

   /********************************************************************
   **             CRIAÇÃO DE VARIAVEIS DO TATICO-ALUNO                **
   *********************************************************************/

   
   int criaVariaveisTatico( int campusId, int P, int r );

   int criaVariavelTaticoAlunoCreditosAPartirDeX_MaisFiltroAluno( int campusId, int P );	// v_{a,i,d,u,s,hi,hf,t}    
   int criaVariavelTaticoAlunoCreditosAPartirDeX( int campusId, int P );					// v_{a,i,d,u,s,hi,hf,t}    
   int criaVariavelTaticoOferecimentosAPartirDeX( int campusId, int P );
   int criaVariavelTaticoConsecutivosAPartirDeX( int campusId, int P );
   int criaVariavelTaticoCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P );
   int criaVariavelTaticoFolgaCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P );   
   int criaVariavelTaticoAberturaCompativelAPartirDeX( int campusId, int P );
   int criaVariavelTaticoAberturaAPartirDeX( int campusId, int prior );
   int criaVariavelTaticoFolgaAbreTurmaSequencialAPartirDeX( int campusId, int P );
   int criaVariavelTaticoFolgaProfAPartirDeX( int campusId, int P );
   int criaVariavelTaticoAlocaAlunoTurmaDiscAPartirDeV( int campusId, int P );				// s_{i,d,a,cp}

   int criaVariavelTaticoCreditos__( int campusId, int P );
   void criaVariavelTaticoCreditosCopiadas( int campusId, int P, int &numVars );			// x_{i,d,u,s,hi,hf,t}      
   int criaVariavelTaticoCreditos( int campusId, int P );									// x_{i,d,u,s,hi,hf,t}      
   int criaVariavelTaticoCreditosComSolInicial( int campusId, int P );						// x_{i,d,u,s,hi,hf,t}
   int criaVariavelTaticoOferecimentos( int campusId, int P );								// o_{i,d,u,s}
   int criaVariavelTaticoAlocaAlunoTurmaDisc( int campusId, int P );						// s_{i,d,a,cp}
   int criaVariavelTaticoCursoAlunos( int campusId, int P );								// b_{i,d,c,c'}
   int criaVariavelTaticoConsecutivos( int campusId, int P );								// c_{i,d,t}
   int criaVariavelTaticoCombinacaoDivisaoCredito( int campusId, int P );					// m_{i,d,k}   
   int criaVariavelTaticoFolgaCombinacaoDivisaoCredito( int campusId, int P );				// fkp_{i,d,k} e fkm_{i,d,k}
   int criaVariavelTaticoFolgaDistCredDiaSuperior( int campusId, int P );					// fcp_{d,t}   
   int criaVariavelTaticoFolgaDistCredDiaInferior( int campusId, int P );					// fcm_{d,t}
   int criaVariavelTaticoAberturaCompativel( int campusId, int P );							// zc_{d,t}
   int criaVariavelTaticoFolgaDemandaDiscAluno( int campusId, int P  );						// fd_{d,a}
   int criaVariavelTaticoFolgaAlunoUnidDifDia( int campusId, int P );						// fu_{i1,d1,i2,d2,t,cp}
   int criaVariavelTaticoDiaUsadoPeloAluno( int campusId, int P );							// du_{a,t}
   int criaVariavelTaticoFolgaAbreTurmaSequencial( int campusId, int P );					// ft_{i,d,cp}
   int criaVariavelFolgaProibeCompartilhamento( int campusId, int P );						// fc_{i,d,c,c',cp}
   int criaVariavelFolgaPrioridadeInf( int campusId, int prior );							// fpi_{a,cp}
   int criaVariavelFolgaPrioridadeSup( int campusId, int prior );							// fps_{a,cp}
   int criaVariavelTaticoFormandosNaTurma( int campusId, int prior, int r );				// f_{i,d,cp}
   int criaVariavelTaticoAlocaAlunoTurmaDiscEquiv( int campusId, int P );					// s_{i,d,a,cp}
   int criaVariavelTaticoAlocaAlunoTurmaDiscEquivTotal( int campusId, int P );				// s_{i,d,a,cp}
   int criaVariavelTaticoAbertura( int campusId, int prior, int r );						// z_{i,d,cp}
   int criaVariavelTaticoAlunosMesmaTurmaPratica( int campusId, int P );					// ss_{a1,a2,dp}
   int criaVariavelTaticoFolgaMinimoDemandaPorAluno( int campusId, int P_ATUAL );			// fmd_{a}
   int criaVariavelFolgaOcupacaoSala( int campusId, int P_ATUAL );							// fos_{i,d,cp}
   int criaVariavelUsaPeriodoUnid( int campusId );											// uu_{u,oft,p}
   int criaVariavelUsaPeriodoSala( int campusId );											// us_{s,oft,p}
   int criaVariavelFolgaPeriodoUnid( int campusId, int prioridade );						// fuu_{oft,p}
   int criaVariavelFolgaPeriodoSala( int campusId, int prioridade );						// fus_{oft,p}


   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO TATICO-ALUNO              **
   *********************************************************************/

   int criaRestricoesTatico( int campusId, int prioridade, int r );

   int criaRestricaoTaticoAssociaVeX( int campusId );
   int criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId );		// Restricao 1.2.3   
   int criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId ); // Restricao 1.2.4
   int criaRestricaoTaticoTurmaDiscDiasConsec( int campusId );		    // Restricao 1.2.6
   int criaRestricaoTaticoCursosIncompat( int campusId );
   int criaRestricaoTaticoProibeCompartilhamento( int campusId );
   int criaRestricaoTaticoLimitaAberturaTurmas( int campusId, int prioridade ); 
   int criaRestricaoTaticoLimitaMaximoAlunosNasTurmas( int campusId, int prioridade );
   int criaRestricaoTaticoAlunoDiscPraticaTeoricaEquiv_MxN( int campusId, int prioridade );
   int criaRestricaoTaticoAlunoDiscPraticaTeorica_MxN( int campusId );
   int criaRestricaoTaticoAlunoDiscPraticaTeorica_1x1( int campusId );
   int criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN( int campusId );
   int criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN_antiga( int campusId );
   int criaRestricaoTaticoAlunosMesmaTurmaPratica( int campusId );
   int criaRestricaoTaticoDivisaoCredito_hash( int campusId );
   int criaRestricaoTaticoCombinacaoDivisaoCredito( int campusId );		// Restricao 1.2.9
   int criaRestricaoTaticoAtivacaoVarZC( int campusId );				// Restricao 1.2.10
   int criaRestricaoTaticoDisciplinasIncompativeis( int campusId );		// Restricao 1.2.11
   int criaRestricaoTaticoAlunoHorario( int campusId );					// Restricao 1.2.12
   int criaRestricaoTaticoAlunoUnidDifDia( int campusId );
   int criaRestricaoTaticoMinDiasAluno( int campusId );
   int criaRestricaoTaticoMaxDiasAluno( int campusId );
   int criaRestricaoTaticoAtendeAluno( int campusId, int prioridade );
   int criaRestricaoTaticoSalaUnica( int campusId );
   int criaRestricaoTaticoSalaPorTurma( int campusId );
   int criaRestricaoTaticoAssociaAlunoEGaranteNroCreds( int campusId );
   int criaRestricaoTaticoAbreTurmasEmSequencia( int campusId );
   int criaRestricaoTaticoAlunoCurso( int campusId );
   int criaRestricaoPrioridadesDemanda( int campusId, int prior );
   int criaRestricaoPrioridadesDemanda_v2( int campusId, int prior );
   int criaRestricaoPrioridadesDemandaEquiv( int campusId, int prior );
   int criaRestricaoTaticoFormandos( int campusId, int prioridade, int r );
   int criaRestricaoTaticoAtendeAlunoEquiv( int campusId, int prioridade );
   int criaRestricaoTaticoAtendeAlunoEquivTotal( int campusId, int prioridade );
   int criaRestricaoTaticoAtivaZ( int campusId ); // não precisa, pode deletar. A restrição criaRestricaoTaticoSalaUnica engloba esta
   int criaRestricaoTaticoTurmaComOsMesmosAlunosPorAula( int campusId );
   int criaRestricaoTaticoConsideraHorariosProfs( int campusId );
   int criaRestricaoTaticoDiscPTAulasContinuas( int campusId, int prioridade );
   int criaRestricaoTaticoAlocMinAluno( int campusId );
   int criaRestricaoFolgaOcupacaoSala( int campusId );
   int criaRestricaoLimitaUnidPeriodo( int campusId );
   int criaRestricaoUsaUnidPeriodo( int campusId );
   int criaRestricaoLimitaSalaPeriodo( int campusId );
   int criaRestricaoUsaSalaPeriodo( int campusId );
   int criaRestricaoProfDescansoMinimo( int campusId );
   
 /*******************************************************************************************************************
										MODELO PARA TROCA DE SALAS
  ******************************************************************************************************************/
   
   // Variaveis   
   int criaVariaveisBranchSala( int campusId, int P, int r );

   int criaVariavelCreditosCopiadasSalaLivre( int campusId, int P );
   int criaVariavelSalaUsada( int campusId, int P );
   int criaVariavelOferecimento( int campusId, int P );
   
   // Restrições
   int criaRestricoesBranchSala( int campusId );

   int criaRestricaoTotalAtend( int campusId );
   int criaRestricaoSalaUnica( int campusId );
   int criaRestricaoSalaHorario( int campusId );
   int criaRestricaoCapacidadeSala( int campusId );
   int criaRestricaoSalaUsada( int campusId );

   void atualizaVariaveisTatico();
   void sincronizaSolucaoBranchSala( int campusAtualId, int prioridade, int r );
   void carregaSolucaoBranchSala( int campusAtualId, int prioridade, int r );
   int solveBranchSala( int campusId, int prioridade, int r );
   void nroSalasUsadas();

 /*******************************************************************************************************************

  ******************************************************************************************************************/


    static int idCounter;
	
   // The linear problem.	
   
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif

   // Hash which associates the column number with the VariableTatico object.
   VariableTatIntHash vHashTatico;

   // Hash which associates the row number with the ConstraintTatico object.
   ConstraintTatIntHash cHashTatico;


   bool NAO_CRIAR_RESTRICOES_CJT_ANTERIORES;
   bool FIXAR_P1;
   bool FIXAR_TATICO_P1;
   bool *CARREGA_SOLUCAO;
   bool USAR_EQUIVALENCIA;
   bool PERMITIR_NOVAS_TURMAS;
   bool CRIAR_VARS_FIXADAS;
   bool CRIANDO_V_ATRAVES_DE_X;
   int ITERACAO;
   bool BRANCH_SALA;
   bool PERMITIR_REALOCAR_ALUNO;

   int etapa;

    void chgCoeffList( std::vector< std::pair< int, int > > , std::vector< double > );
	bool violaInsercao( Aluno* aluno, GGroup< VariableTatInt *, LessPtr<VariableTatInt> > aulasX );
	void imprimeGradeHorAlunos( int campusId, int prioridade );
	void imprimeGradeHorAlunosPorDemanda( int campusId, int prioridade );
	void confereCorretude( int campusId, int prioridade );
	void aumentaNroTurmas( int prioridade, int campusId );
	void corrigeNroTurmas( int prioridade, int campusId );
	void calculaNroFolgas( int prioridade, int campusId );
	std::string getCorrigeNrTurmasFileName( int campusId, int prioridade, int r);
	std::string getAumentaTurmasFileName( int campusId, int prioridade, int r);
	std::string getTaticoLpFileName( int campusId, int prioridade, int r );
	std::string getSolBinFileName( int campusId, int prioridade, int r );	
	int readSolTxt( int campusId, int prioridade, int r, int type, double *xSol, int fase );
	std::string getSolucaoTaticoFileName( int campusId, int prioridade, int r, int fase );	
	std::string getEquivFileName( int campusId, int prioridade );
	void writeSolBin( int campusId, int prioridade, int r, int type, double *xSol );
	void writeSolTxt( int campusId, int prioridade, int r, int type, double *xSol, int fase );
   void readSolTxtAux( char *fileName, double *xSol );
	int readSolBin( int campusId, int prioridade, int r, int type, double *xSol );
	int writeGapTxt( int campusId, int prioridade, int r, int type, double gap );

	void sincronizaSolucao( int campusAtualId, int prioridade, int r );
	void addVariaveisTatico();
	void initCredsSala();
	void carregaVariaveisSolucaoTaticoPorAlunoHor( int campusAtualId, int prioridade, int r );
	int solveTaticoIntAlunoHor( int campusId, int prioridade, int r );
	int solveTaticoIntAlunoHor_EtapaSimples( int campusId, int prioridade, int r );
	int solveGaranteSolucao( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveGaranteSolucaoInicial( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtendPorFasesDoDia( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtend( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtendCalourosFormandos( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	void polishTaticoHor(double *xSol, double maxTime, int percIni, int percMin, double maxTempoSemMelhora);
	bool SolVarsFound( VariableTatico v );
	bool criaVariavelTaticoInt( VariableTatInt *v, bool &fixar, int prioridade );
	Unidade* retornaUnidadeDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
	ConjuntoSala* retornaSalaDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
	GGroup< VariableTatico *, LessPtr<VariableTatico> > retornaAulasEmVarX( int turma, Disciplina* disciplina, int campusId );
	GGroup< VariableTatInt *, LessPtr<VariableTatInt> > retornaAulasEmVarXFinal( int turma, Disciplina* disciplina, int campusId );
	GGroup< VariableTatInt *, LessPtr<VariableTatInt> > retornaAulasEmVarV( Aluno* aluno, GGroup< VariableTatInt *, LessPtr<VariableTatInt> > aulasX );

	GGroup< VariableTatInt *, LessPtr<VariableTatInt> > vars_v;
	GGroup< VariableTatInt *, LessPtr<VariableTatInt> > solVarsTatInt;

	GGroup< VariableTatico *, LessPtr<VariableTatico> > *solVarsTatico;
	GGroup< VariableTatico *, LessPtr<VariableTatico> > *vars_xh;

	// map com o nro de folgas de demanda para o campus atual e prioridade no máximo P atual.
	std::map< Disciplina*, int, LessPtr< Disciplina > > mapDiscNroFolgasDemandas;
	inline int haFolgaDeAtendimento( Disciplina *disciplina ) { return this->mapDiscNroFolgasDemandas[disciplina]; }
	bool permitirAbertura( int turma, Disciplina *disciplina, int campusId );

	void atualizarDemandasEquiv( int campusId, int prioridade );

	std::map< Trio< int, Disciplina *, int >, bool > mapPermitirAbertura;

	std::map< int /*discId*/, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> > mapDiscAlunosDemanda; // para auxilio na criação das variaveis
	int getNroMaxAlunoDemanda( int discId );

};


#endif