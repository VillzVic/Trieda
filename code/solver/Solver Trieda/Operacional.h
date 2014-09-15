#ifndef _OPERACIONAL_H_
#define _OPERACIONAL_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "ValidateSolution.h"
#include "Solver.h"
#include "VariableoP.h"
#include "ConstraintOp.h"
#include "opt_lp.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolutionLoader.h"
#include "ErrorHandler.h"
#include "ProblemDataLoader.h"

#ifdef HEURISTICA_MARCIO
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
#endif

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#endif


class Operacional : public Solver
{
public:
   Operacional( ProblemData * aProblemData, bool *endCARREGA_SOLUCAO, 
				GGroup< VariableOp *, LessPtr<VariableOp> > * asolVarsOp,
				ProblemSolution *problemSolution );

   virtual ~Operacional(void);
   
   int solve();
   void getSolution( ProblemSolution * ){};
      

    void relacionaProfessoresDisciplinas();
  	
   int solveOperacionalEtapas();
   void testaTrocarProfVirtualPorReal();
   void polishOperacional(double *xSol, double maxTime, int percIni, int percMin, double maxTempoSemMelhora);
   int solveOperacionalMIP();
   void clearModel();
   void carregaSolucaoOperacional();
   void getSolutionOperacionalMIP();
   void preencheOutputOperacionalMIP();
   void geraMinimoDeProfessoresVirtuaisMIP();
   void criaProfessoresVirtuaisPorCurso( int n, TipoTitulacao* titulacao, TipoContrato *contrato, Curso* curso, GGroup<Campus*,LessPtr<Campus>> campi );
   void separaProfsVirtuais();
   
   void preencheOutputOperacionalMIP_antigo(); // apagar se o novo corrigido funcionar

   int calculaDeslocamentoUnidades( const int, const int );
   
    #ifdef HEURISTICA_MARCIO
   int solveOperacional();
	#endif


private:

   /********************************************************************************************************************
   **						        CRIAÇÃO DE VARIAVEIS DO OPERACIONAL												  **
   /********************************************************************************************************************/

   int criaVariaveisOperacional(  );

   int criaVariavelProfessorAulaHorario_HorsFixoTat( void );
   int criaVariavelProfessorAulaHorario( void );
   int criaVariavelDisciplinaHorario__();

   int criaVariavelProfessorDisciplina( void );
   int criaVariavelDisciplinaHorario( void );
   int criaVariavelProfessorCurso( void );
   int criaVariavelDiasProfessoresMinistramAulas( void );
   int criaVariavelFolgaUltimaPrimeiraAulas( void );			 // NÃO USADA MAIS. RESTRIÇÃO SUBSTITUTA: INTERJORNADA
   int criaVariavelDiscProfCurso( void );
   int criaVariavelDiscProfOferta();
   int criaVariavelFolgaMaxDiscProfCurso( void );
   int criaVariavelFolgaCargaHorariaMinimaProfessor( void );
   int criaVariavelFolgaCargaHorariaMinimaProfessorSemana( void );
   int criaVariavelFolgaCargaHorariaMaximaProfessorSemana( void );
   int criaVariavelFolgaDemanda( void );
   int criaVariavelFolgaDisciplinaTurmaHorario( void );
   int criaVariavelProfessorDiaHorarioIF();
   int criaVariavelNroProfsReaisAlocadosCurso();
   int criaVariavelNroProfsVirtuaisAlocadosCurso();
   int criaVariavelNroProfsVirtuaisMestresAlocadosCurso();
   int criaVariavelNroProfsVirtuaisDoutoresAlocadosCurso();
   int criaVariavelNroProfsVirtuaisGeraisAlocadosCurso();
   int criaVariavelProfessorVirtual();
   int criaVariavelFolgaMinimoDemandaPorAluno();
   int criaVariaveisHiHfProfFaseDoDiaAPartirDeX(void);										// hip_{p,t,f} e hfp_{p,t,f}
   int criaVariavelFolgaGapProfAPartirDeK(void);											// fpgap_{p,t,f}


   /********************************************************************************************************************
   **						        CRIAÇÃO DE RESTRIÇÕES DO OPERACIONAL											  **
   /********************************************************************************************************************/

   int criaRestricoesOperacional(  );
   
   int criaRestricaoAlunoHorario__2( void );

   int criaRestricaoSalaParHorario(); // nao usado
   int criaRestricaoSalaHorario( void );
   int criaRestricaoProfessorParHorario(); // nao usado
   int criaRestricaoProfessorHorario(  );
   int criaRestricaoAlunoHorario( void );
   int criaRestricaoAlocAula( void );
   int criaRestricaoAtendimentoCompleto( void );
   int criaRestricaoProfessorDisciplina( void );
   int criaRestricaoProfessorDisciplinaUnico( void );
   int criaRestricaoProfessorDisciplinaUnicoPT( void );
   int criaRestricaoDisciplinaMesmoHorario( void );
   int criaRestricaoDisciplinaHorarioUnico( void );
   int criaRestricaoDeslocamentoViavel( void ); // x3
   int criaRestricaoDeslocamentoProfessor( void ); // x5
   int criaRestricaoCalculaDiasProfMinistra_Min(); // x9
   int criaRestricaoCalculaDiasProfMinistra_Max(); // x9
   int criaRestricaoMaxDiasSemanaProf(  );
   int criaRestricaoMinCredDiariosProf(  );
   int criaRestricaoCargaHorariaMinimaProfessor( void ); // x10
   int criaRestricaoUltimaPrimeiraAulas( void ); // x11			 // NÃO USADA MAIS. RESTRIÇÃO SUBSTITUTA: INTERJORNADA
   int criaRestricaoAlocacaoProfessorCurso( void ); // x12
   int criaRestricaoMinimoMestresCurso( void ); // x12
   int criaRestricaoMinimoDoutoresCurso( void ); // x12
   int criaRestricaoMaximoNaoMestresCurso();
   int criaRestricaoMaximoNaoDoutoresCurso();
   int criaRestricaoDiscProfCurso(); // x13
   int criaRestricaoMaxDiscProfCurso( void ); // x13
   int criaRestricaoDiscProfOferta();
   int criaRestricaoMaxDiscProfPeriodoOferta();
   int criaRestricaoGaranteMinProfsPorCurso();
   int criaRestricaoCargaHorariaMinimaProfessorSemana( void ); // x14
   int criaRestricaoCargaHorariaMaximaProfessorSemana( void );
   int criaRestricaoProfHorarioMultiUnid( void );
   int criaRestricaoGapsHorariosProfessores(  );
   int criaRestricaoCalculaNroProfsAlocadosCurso();
   int criaRestricaoEstimaNroProfsVirtuaisAlocadosCurso();
   int criaRestricaoEstimaNroProfsVirtuaisMestresAlocadosCurso();
   int criaRestricaoEstimaNroProfsVirtuaisDoutoresAlocadosCurso();
   int criaRestricaoEstimaNroProfsVirtuaisGeraisAlocadosCurso();
   int criaRestricaoEstimaNroProfsVirtuaisPorContratoCurso();
   int criaRestricaoMaximoProfSemContratoCurso();
   int criaRestricaoMinimoContratoCurso();
   int criaRestricaoCalculaNroProfsVirtPorContratoAlocadosCurso();
   int criaRestricaoSomaNroProfsVirtuaisAlocadosCurso();
   int criaRestricaoCalculaNroProfsVirtMestAlocadosCurso();
   int criaRestricaoCalculaNroProfsVirtDoutAlocadosCurso();
   int criaRestricaoCalculaNroProfsVirtAlocadosCurso();
   int criaRestricaoCalculaNroProfsVirtGeraisAlocadosCurso();
   int criaRestricaoUsaProfVirtual();
   int criaRestricaoDiscPTAulasContinuas();
   int criaRestricaoAlocMinAluno();
   int criaRestricaoProfDescansoMinimo(  );
         
	// criar restrições que impedem gaps nos horários do professor em uma mesma fase de um dia
	int criarRestricaoProfHiHf_();
	int criarRestricaoProfHiUB_(Professor* const prof, const int dia, const int fase, 
		const int colHi, map<DateTime, set< pair<int,double> >> const & mapDtiVars);
	int criarRestricaoProfHiLB_(Professor* const prof, const int dia, const int fase,
		const int colHi, map<DateTime, set< pair<int,double> >> const & mapDtiVars);
	int criarRestricaoProfHfLB_(Professor* const prof, const int dia, const int fase, 
		const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars);
	int criarRestricaoProfHfUB_(Professor* const prof, const int dia, const int fase, 
		const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars);
	int criarRestricaoProfGapMTN_(Professor* const prof, const int dia, const int fase, 
		int rhs, const int colHi, const int colHf, set< pair<int,double> > const &varsColCoef);
	

   /********************************************************************************************************************
   **						        ETAPAS DA RESOLUÇÃO																  **
   /********************************************************************************************************************/

   int solveGaranteTotalAtendHorInicial( bool& CARREGA_SOL_PARCIAL, double *xS );
   int solveMaxAtendPorFasesDoDia( bool& CARREGA_SOL_PARCIAL, double *xS );
   int solveMinPVPorFasesDoDia( bool& CARREGA_SOL_PARCIAL, double *xS );
   
   
   /********************************************************************************************************************
   **						        OUTROS																			  **
   /********************************************************************************************************************/

   void buscaLocalTempoDeslocamentoSolucao();
   
   int alteraHorarioAulaAtendimento( const int, const int );

   void chgCoeffList( std::vector< std::pair< int, int > > , std::vector< double > );

	std::string getOpLpFileName( int etapa );

	std::string getSolOpBinFileName( int etapa, int fase );

	std::string getSolucaoOpFileName( int etapa, int fase );

	void writeOpSolBin( int type, double *xSol, int fase );

	int readOpSolBin( int type, double *xSol, int fase );

	void writeOpSolTxt( int type, double *xSol, int fase );

	// Filtro para a criação das variaveis do modelo operacional, caso haja solução de etapa anterior
   bool criarVariavelOp( VariableOp v, double &lb );

   void retornaHorariosPossiveis( Professor *, Aula *, std::list< HorarioDia * > & );
         
   bool inSolution( VariableOp v );
   
   int getRodada() const { return rodadaOp; }
   void setRodada( int r ) { rodadaOp = r; }
   

   /********************************************************************************************************************
   **		        ESTRUTURAS PARA AGRUPAR VARIÁVEIS CRIADAS E AUXILIAR NA CRIAÇÃO DE RESTRIÇÕES					  **
   /********************************************************************************************************************/
   
    map< Professor*, map< int /*dia*/, map<DateTime /*dti*/, map< Campus*, map< Disciplina*, map< int /*turma*/,
		pair<int /*col*/, VariableOp> >, LessPtr<Disciplina> >, LessPtr<Campus> > > >, LessPtr<Professor> > vars_prof_aula2;	// x_{p,aula,hi}
	
	map< Professor*, map< int /*dia*/, map< int /*fase*/, pair<DateTime /*dti*/, DateTime /*dtf*/> > >,
		LessPtr<Professor> > vars_prof_dia_fase_dt;								// min dti e max dtf para cada prof/dia/fase
	
	// vars inteiras que indicam o primeiro hip e o último hfp horário da fase do dia (M/T/N) usados pelo professor
	// Prof -> Dia -> FaseDoDia -> (col. nr. hip/ col. nr. hfp)
	unordered_map<Professor*, unordered_map<int, unordered_map<int, pair<int,int>>>> varsProfDiaFaseHiHf;		// hip_{p,t,f} e hfp_{p,t,f}
	
	// Professor -> Dia -> Fase do dia -> (col. nr. fpgap)
	unordered_map<Professor*, unordered_map<int, unordered_map<int,int>>> varsProfFolgaGap;						// fpgap_{p,t,f}
	



   /********************************************************************************************************************
   **						        ESTRUTURAS E ATRIBUTOS															  **
   /********************************************************************************************************************/
   


   enum OutPutFileType
   {
	  OP_BIN = 9,					
	  OP_BIN0 = 13,
	  OP_BIN1 = 10,				
	  OP_BIN2 = 11,		
	  OP_BIN3 = 12
   };
   
	bool *CARREGA_SOLUCAO;

   // Hash which associates the column number with the VariableOp object.
   VariableOpHash vHashOp;

   // Hash which associates the row number with the ConstraintOp object.
   ConstraintOpHash cHashOp;
   
   GGroup< VariableOp *, LessPtr<VariableOp> > *solVarsOp;
   
   ProblemSolution * problemSolution;
   
   ProblemSolution * problemSolutionTemp;
   
   ValidateSolutionOp * validateSolution;

   int rodadaOp;

   enum Rodada
   {
	  OP_VIRTUAL_PERFIL = 1,			
	  OP_VIRTUAL_INDIVIDUAL = 2
   };

   // The linear problem.	
   //OPT_LP * lp;
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif

	bool FIXA_HOR_SOL_TATICO;

};

#endif