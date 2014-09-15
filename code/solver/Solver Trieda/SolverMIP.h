#ifndef _SOLVER_MIP_H_
#define _SOLVER_MIP_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>


#include "EstimaTurmas.h"
#include "PreModelo.h"
#include "TaticoIntAlunoHor.h"
#include "Operacional.h"
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
#include "SolutionPool.h"

class SolverTaticoHeur;

#ifdef SOLVER_CPLEX

#include "opt_cplex.h"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#endif

#define PRINT_cria_variaveis
#define PRINT_cria_restricoes

class SolverMIP : public Solver
{
public:
	
   SolverMIP( ProblemData *, ProblemSolution *, ProblemDataLoader *, bool );
   virtual ~SolverMIP();

   int solve();
   void getSolution( ProblemSolution * ){};

   
#ifndef HEURISTICA
   bool readSolHeurInic();
   void carregaSolucaoHeuristicaInicial();
   void reenumeraTurmasSolInicProblemSolution();
   void loadProblemSolution();
   void setNumTurmasPratPorTeor( std::map< Aluno*, std::map< Disciplina*, int, LessPtr<Disciplina> >, LessPtr<Aluno> > &mapAlunoDiscTurma );
#endif



   /********************************************************************
   **             CRIAÇÃO DE VARIAVEIS DO TATICO-ALUNO                **
   *********************************************************************/

   
   int criaVariaveisTatico( int campusId, int P, int r, int tatico );
   
   int criaVariavelTaticoCreditosAtravesDePreSol( int campusId, int P, int r, int tatico );				// x_{i,d,u,s,hi,hf,t}  
   int criaVariavelTaticoCreditos( int campusId, int P, int r, int tatico );				// x_{i,d,u,s,hi,hf,t}  
   int criaVariavelTaticoAbertura( int campusId, int P, int r );							// z_{i,d,cp}
   int criaVariavelTaticoConsecutivos( int campusId, int P );								// c_{i,d,t}
   int criaVariavelTaticoConsecutivosAPartirDeX( int campusId, int P );
   int criaVariavelTaticoCombinacaoDivisaoCredito( int campusId, int P );					// m_{i,d,k}   
   int criaVariavelTaticoFolgaCombinacaoDivisaoCredito( int campusId, int P );				// fkp_{i,d,k} e fkm_{i,d,k}
   int criaVariavelTaticoFolgaDistCredDiaSuperior( int campusId, int P );					// fcp_{d,t}   
   int criaVariavelTaticoFolgaDistCredDiaInferior( int campusId, int P );					// fcm_{d,t}
   int criaVariavelTaticoAberturaCompativel( int campusId, int P );							// zc_{d,t}
//   int criaVariavelTaticoFolgaDemandaDiscAluno( int campusId );					// fd_{d,a}   
   int criaVariavelTaticoFolgaDemandaDisc( int campusId, int P, int r );					// fd_{i,d,cp}
   int criaVariavelTaticoAlunoUnidDia( int campusId, int P );								// y_{a,u,t}  // Não usada
   int criaVariavelTaticoAlunoUnidadesDifDia( int campusId, int P );						// w_{a,t}	  // Não usada

   int criaVariavelTaticoFolgaAlunoUnidDifDia( int campusId, int P );						// fu_{i1,d1,i2,d2,t,cp}
   int criaVariavelTaticoFolgaFolgaDemandaPT( int campusId, int P );						// ffd_{i1,-d,i2,d,cp}
   int criaVariavelTaticoDiaUsadoPeloAluno( int campusId, int P );							// du_{a,t}
   int criaVariavelTaticoDesalocaAlunoDiaHor( int campusId, int P, int tatico );			// fad_{i,d,a,t,hi,hf}
   int criaVariavelTaticoDesalocaAluno( int campusId, int P, int tatico );					// fa_{i,d,a}
   int criaVariavelTaticoFormandosNaTurma( int campusId, int P_ATUAL, int r, int tatico );	// f_{i,d,cp}
   int criaVariavelTaticoFolgaProfAPartirDeX( int campusId, int P );						// fp_{d,t,h}

   int criaVariavelTaticoSalaTurma( int campusId, int P, int r, int tatico );				// ys{d,i,s}
   int criaVariavelTaticoFolgaMinimoDemandaAtendPorAluno( int campusId, int P );			// fmd_{a}

#ifdef TATICO_COM_HORARIOS_HEURISTICO
   SolverTaticoHeur* solverTaticoHeur;

   SolverTaticoHeur* SolverMIP::criaSolverTaticoHeuristico(int campusId, int P, int r);
#endif


   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO TATICO-ALUNO              **
   *********************************************************************/

   int criaRestricoesTatico( int campusId, int prioridade, int r, int tatico );

   int criaRestricaoTaticoCargaHoraria( int campusId, int prioridade, int r, int tatico );					// Restricao 1.2.2   
   int criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId, int prioridade, int r, int tatico );		// Restricao 1.2.3   
   int criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId, int prioridade, int r, int tatico ); // Restricao 1.2.4
//   int criaRestricaoTaticoAtendeDemandaAluno( int campusID );			// Restricao 1.2.5   
   int criaRestricaoTaticoAtendeDemanda( int campusId, int prioridade, int r, int tatico );				// Restricao 1.2.5   
   int criaRestricaoTaticoTurmaDiscDiasConsec( int campusId, int prioridade, int r, int tatico );		    // Restricao 1.2.6
   int criaRestricaoTaticoLimitaAberturaTurmas( int campusId, int prioridade, int r, int tatico );		// Restricao 1.2.7 // Não está sendo mais usada. Inutil.
   int criaRestricaoTaticoDivisaoCredito_hash( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoDivisaoCredito( int campusId, int prioridade, int r, int tatico );				// Restricao 1.2.8      
   int criaRestricaoTaticoCombinacaoDivisaoCredito( int campusId, int prioridade, int r, int tatico );		// Restricao 1.2.9
   int criaRestricaoTaticoAtivacaoVarZC( int campusId, int prioridade, int r, int tatico );				// Restricao 1.2.10
   int criaRestricaoTaticoDisciplinasIncompativeis( int campusId, int prioridade, int r, int tatico );		// Restricao 1.2.11
   int criaRestricaoTaticoAlunoHorario( int campusId, int prioridade, int r, int tatico );					// Restricao 1.2.12
   int criaRestricaoTaticoAtivaY( int campusId, int prioridade, int r, int tatico );						// Restricao 1.2.13
   int criaRestricaoTaticoAlunoUnidadesDifDia( int campusId, int prioridade, int r, int tatico );			// Restricao 1.2.14
   int criaRestricaoTaticoMinCreds( int campusId, int prioridade, int r, int tatico );						// Restricao 1.2.15
   int criaRestricaoTaticoMaxCreds( int campusId, int prioridade, int r, int tatico );						// Restricao 1.2.16
//   int criaRestricaoTaticoAlunoDiscPraticaTeorica( int campusId );	// Restricao 1.2.17
   int criaRestricaoTaticoDiscPraticaTeorica( int campusId, int prioridade, int r, int tatico );			// Restricao 1.2.17
   int criaRestricaoTaticoDiscPraticaTeorica1xN( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoAlunoUnidDifDia( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoMinDiasAluno( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoMaxDiasAluno( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoConsideraHorariosProfs( int campusId, int prioridade, int r, int tatico );

   int criaRestricaoTaticoSalaTurma( int campusId, int prioridade, int r, int tatico );					// Restricao 1.2.30   
   int criaRestricaoTaticoSalaUnicaTurma( int campusId, int prioridade, int r, int tatico );					// Restricao 1.2.31

   int criaRestricaoTaticoDesalocaAlunoTurmaHorario( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoDesalocaAlunoHorario( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoSumDesalocaAlunoFolgaDemanda( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoSumDesalocaAluno( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoGaranteMinAlunosTurma( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoDesalocaPT( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoFormandos( int campusId, int prioridade, int r, int tatico );
   
   int criaRestricaoTaticoDiscPTAulasContinuas( int campusId, int prioridade, int r, int tatico );
   int criaRestricaoTaticoAlocMinPercAluno( int campusId, int prioridade, int r, int tatico );


   








   int localBranchingHor( double *, double );
   int localBranchingOperacional( double * xSol, double maxTime );
   void polishTaticoHor(double*, double, int percIni, int percMin, double maxTempoSemMelhora);
   void polishMutacaoCombTaticoHor(double*, double, int percIni, int percMin);
   void fixaVarsMutacaoTaticoHor(double*, int perc, int strateg);
   void fixaVarsCombTaticoHor(double*, double*);
   void desfixaVarsTaticoHor(double *lbs, double *ubs);
   bool mutacaoTaticoHor(double* &xSol, double &objSol, int perc, double tempoLim, bool paradaMelhora, SolutionPool* &pool);
   bool combTaticoHor(double* &xSol, double &objSol, double tempoLim, bool paradaMelhora, SolutionPool* &pool);
   void polishOperacional(double*, double, int percIni, int percMin);
   void carregaVariaveisSolucaoTatico( int campusId );

   void limpaMapAtendimentoAlunoPrioridadeAnterior( int campusId );
   
   int removeAtendimentosParciais( double *xSol, char solFilename[1024], int prioridade );


   // funções de teste para quando quiser fixar toda uma solução,
   // e dps alterar um valor especifico para se observar o comportamento
   void testeCarregaSol( int campusId, int prioridade, int cjtAlunosId, int r, int tatico );
   
   void deletaPreSol();
   int carregaVariaveisSolucaoTaticoPorAluno_CjtAlunos( int campusId, int prioridade, int cjtAlunosId, int r, int tatico );
   int solveTaticoPorCampusCjtAlunos();
   int solveTaticoPorCampusComSolHeur();
   int solveTaticoMain( int campusId, int prioridade, int cjtAlunosId, int r, int tatico );
   int solveTaticoBasicoCjtAlunos( int campusId, int prioridade, int cjtAlunosId, int r, int tatico );
   int solveMaxAtendPorFasesDoDia( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, bool& CARREGA_SOL_PARCIAL, double *xS );
   int solveMaxAtendCalourosFormandos( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, bool& CARREGA_SOL_PARCIAL, double *xS );
   int solveMaxAtend( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, bool& CARREGA_SOL_PARCIAL, double *xS );
   void atualizaFolgasAlunoDemanda( int campusId );
   void verificaNaoAtendimentos( int campusIdAtual, int P );

   map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >
	   procuraCombinacaoLivreEmSalas( Disciplina *disciplina, TurnoIES* turno, int campusId );
   vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >
	   procuraCombinacaoLivreNaSala( Disciplina *disciplina, TurnoIES* turno, Sala* sala );
   map< int, GGroup< Trio<int,int,Disciplina*> > > 
	   procuraChoqueDeHorariosAluno( Aluno *aluno, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > &opcoes );

   void fixaAtendimentosVariaveisCreditosAnterior();
   void liberaBoundsVariaveis_FFD_FD_FP();
   void voltaComAlunosNaoAlocados();
   void validaCorrequisitos();
   void criaVariaveisSolInicial( int campusId );

   double fixaLimitesVariavelTaticoPriorAnterior( Variable *v, bool &FOUND );
   double fixaLimitesVariavelTaticoCjtAlunosAnterior( Variable *v );
   double fixaLimitesVariavelTaticoComHorAnterior( VariableTatico *v, bool &found );

   bool NAO_CRIAR_RESTRICOES_CJT_ANTERIORES;
   bool FIXAR_P1;
   bool FIXAR_TATICO_P1;
   bool MODELO_ESTIMA_TURMAS;
   
   bool turmaAtendida( int turma, Disciplina *disciplina, Campus* campus );
   bool SolVarsPreFound( VariablePre v );
   void mudaCjtSalaParaSala();
   void getSolutionTaticoPorAlunoComHorario();
   
   bool aulaAlocada( Aula *, Campus *, Unidade *, Sala *, int );
   
   void relacionaAlunosDemandas();
   Unidade* retornaUnidadeDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
   ConjuntoSala* retornaSalaDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
   GGroup< std::pair< int,Disciplina* > > retornaAtendEmCjtSala( ConjuntoSala * cjtSala );

   GGroup< Trio< int, Disciplina*, int > > retornaAtendEmCjtSalaDia( ConjuntoSala * cjtSala, int dia );

   Unidade* retornaUnidadeDeAtendimentoTaticoAnterior( int turma, Disciplina* disciplina, Campus* campus );
   ConjuntoSala* retornaSalaDeAtendimentoTaticoAnterior( int turma, Disciplina* disciplina, Campus* campus );
   std::pair<ConjuntoSala*, GGroup<int> > retornaSalaEDiasDeAtendimentoTaticoAnterior( int turma, Disciplina* disciplina, Campus* campus );
   GGroup< std::pair< int,Disciplina* > > retornaAtendTaticoEmCjtSala( ConjuntoSala * cjtSala );
      
private:

   enum OutPutFileType
   {
	  TAT_HOR_BIN = 5,					
	  TAT_HOR_BIN1 = 6,				
	  TAT_HOR_BIN2 = 7,		
	  TAT_HOR_BIN3 = 8,
	  TAT_HOR_CALOURO_BIN = 9

   };


   // The linear problem.	
   //OPT_LP * lp;
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif



	bool CARREGA_SOLUCAO;

	int TEMPO_TATICO;

	int retornaTempoDeExecucaoTatico( int campusId, int cjtAlunosId, int prioridade );

	std::string getTaticoLpFileName( int campusId, int prioridade, int cjtAlunosId, int r, int tatico );

    std::string getSolHeuristBinFileName( int campusId, int prioridade, int cjtAlunosId );

	std::string getSolBinFileName( int campusId, int prioridade, int cjtAlunosId, int r, int tatico );
	
	std::string getSolucaoTaticoFileName( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int fase );	
	
	std::string getFixaNaoAtendFileName( int campusId, int prioridade, int cjtAlunosId, int r );

	std::string getAulasSolInitFileName( int campusId );
	std::string getSolXInitFileName( int campusId );
	std::string getSolSInitFileName( int campusId );
	std::string getSolNaoAtendInitFileName( int campusId );

	void writeSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type, double *xSol );

   void writeSolBinAux(char *fileName,double *xSol);

	int readSolBin( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type, double *xSol );

   int readSolBinAux(char *fileName,double *xSol);

   void writeSolTxt( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type, double *xSol, int fase );
	
   int readSolTxtTat( int campusId, int prioridade, int cjtAlunosId, int r, int tatico, int type, double *xSol, int fase );

   	// Filtro para a criação das variaveis do modelo tatico que considera horarios e alunos, caso haja solução do pre-modelo
   bool criaVariavelTatico( VariableTatico *v );

   // Filtro para a criação das variaveis do modelo tatico que considera horarios e alunos, caso haja solução de tatico anterior
   bool criaVariavelTatico_Anterior( VariableTatico *v );
   
   GGroup< VariableTatico*, LessPtr<VariableTatico> > retornaVariableTaticoCreditosAnterior( int turma, Disciplina* disciplina, Campus* campus );

   // Dada uma disciplina 'A' que foi substituída por uma de suas disciplinas equivalentes 'B',
   // esse map informa o conjunto de variáveis que foram criadas para 'B' referentes à disciplina 'A'
   std::map< Disciplina *, std::vector< Variable > > mapVariaveisDisciplinasEquivalentes;
      
   // Vetor responsável por armazenar ponteiros para todas as
   // variáveis do tipo V_CREDITOS com credito(s) alocado(s).
   typedef GGroup< VariableTatico *, LessPtr<VariableTatico> > vars__X__i_d_u_s_hi_hf_t;
   
   void chgCoeffList( std::vector< std::pair< int, int > > , std::vector< double > );

   vars__X__i_d_u_s_hi_hf_t vars_xh;
   
   ProblemSolution * problemSolution;
   ProblemDataLoader * problemDataLoader;


   // Hash which associates the column number with the VariableTatico object.
   VariableTaticoHash vHashTatico;

   // Hash which associates the row number with the ConstraintTatico object.
   ConstraintTaticoHash cHashTatico;
   
   GGroup< VariablePre *, LessPtr<VariablePre> > solVarsXPre;  

   // usado para armazenar a solução tatica da iteração cjtAluno anterior, a fim de fazer a fixação de valores
   GGroup< VariableTatico *, LessPtr<VariableTatico> > solVarsTatico; 
   
   GGroup< VariableOp *, LessPtr<VariableOp> > solVarsOp;
   
   double alpha, beta, gamma, delta, lambda, epsilon, rho, psi, tau, eta;

   struct Ordena
   {
      bool operator() ( std::vector< int > xI, std::vector< int > xJ )
      {
         return ( xI.front() > xJ.front() );
      }
   } ordenaPorCreditos;

   bool usaSolInicialHeur;

   int campusAtualId;
   
   std::map<int, std::vector<string> > alDemNaoAtend_output;

   GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alunoDemandaAtendsHeuristica;

   void imprimeAlocacaoFinalHeuristica( int campusId, int prioridade, int grupoAlunosAtualId );
   void escreveSolucaoBinHeuristica( int campusId, int prioridade, int grupoAlunosAtualId );
   bool leSolucaoHeuritica( int campusId, int prioridade, int grupoAlunosAtualId );


   void heuristica2AlocaAlunos( int campusId, int prioridade, int grupoAlunosAtualId );
   bool heuristica2TentaInsercaoNaTurma( AlunoDemanda *alunoDemanda, int turma, std::string heurFilename );
   void heuristicaP1AlocaAlunos( int campusId, int prioridade, int grupoAlunosAtualId );

   std::list< std::pair< AlunoDemanda*, int > > SolverMIP::ordenaAlunosDemandaP2ParaHeurist( GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alDemList, Aluno* aluno, int prioridade );
  

   void teste(double *& xSol, int n );
   void teste( int n );

};

#endif
