#ifndef _MIP_UNICO_H_
#define _MIP_UNICO_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "Solver.h"
#include "VariableMIPUnico.h"
#include "ConstraintMIPUnico.h"

class ProblemData;
class ProblemSolution;
class VariableTatico;


#define PRINT_cria_variaveis
#define PRINT_cria_restricoes

class MIPUnico : public Solver
{
public:
	
	MIPUnico( ProblemData * aProblemData, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *aSolVarsTatico, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *avars_xh,
				bool *endCARREGA_SOLUCAO, bool equiv, int permitirNovasTurmas );

	virtual ~MIPUnico();


	int solve();
    void getSolution( ProblemSolution * );
	void solveMainEscola( int campusId, int prioridade, int r );
	
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
	  MIP_GENERAL = 0,
	  MIP_GARANTE_SOL = 1,				
	  MIP_MAX_ATEND = 2,		
	  MIP_DISP_PROF = 3
   };	  


   void preencheMapDiscAlunosDemanda( int campusId, int P, int r );

   /********************************************************************
   **             CRIA��O DE VARIAVEIS DO TATICO-ALUNO                **
   *********************************************************************/

   
   int criaVariaveisTatico( int campusId, int P, int r );

   int criaVariavelTaticoAlunoCreditosAPartirDeX_MaisFiltroAluno( int campusId, int P );	// v_{a,i,d,u,s,hi,hf,t}    
   int criaVariavelTaticoAlunoCreditosAPartirDeX( int campusId, int P );					// v_{a,i,d,u,s,hi,hf,t}
   int criaVariavelTaticoOferecimentosAPartirDeX( int campusId, int P );
   int criaVariavelTaticoCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P );			// m_{i,d,k}
   int criaVariavelTaticoFolgaCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P );	// fkp_{i,d,k} e fkm_{i,d,k}
   int criaVariavelTaticoAberturaCompativelAPartirDeX( int campusId, int P );				// zc_{d,t}
   int criaVariavelTaticoAberturaAPartirDeX( int campusId, int prior );
   int criaVariavelTaticoAlocaAlunoTurmaDiscAPartirDeV( int campusId, int P );				// s_{i,d,a,cp}

   int criaVariavelTaticoCreditos( int campusId, int P );									// x_{i,d,u,s,hi,hf,t}
   void criaVariavelTaticoCreditosCopiadas( int campusId, int P, int &numVars );			// x_{i,d,u,s,hi,hf,t}
   int criaVariavelTaticoCreditosComSolInicial( int campusId, int P );						// x_{i,d,u,s,hi,hf,t}
   int criaVariavelTaticoOferecimentos( int campusId, int P );								// o_{i,d,u,s}
   int criaVariavelTaticoCursoAlunos( int campusId, int P );								// b_{i,d,c,c'}
   int criaVariavelTaticoFolgaDemandaDiscAluno( int campusId, int P  );						// fd_{d,a}
   int criaVariavelTaticoFolgaAlunoUnidDifDia( int campusId, int P );						// fu_{i1,d1,i2,d2,t,cp}
   int criaVariavelTaticoDiaUsadoPeloAluno( int campusId, int P );							// du_{a,t}
   int criaVariavelTaticoFolgaAbreTurmaSequencial( int campusId, int P );					// ft_{i,d,cp}
   int criaVariavelFolgaProibeCompartilhamento( int campusId, int P );						// fc_{i,d,c,c',cp}
   int criaVariavelFolgaPrioridadeInf( int campusId, int prior );							// fpi_{a,cp}
   int criaVariavelFolgaPrioridadeSup( int campusId, int prior );							// fps_{a,cp}
   int criaVariavelTaticoAbertura( int campusId, int prior, int r );						// z_{i,d,cp}
   int criaVariavelTaticoAlunosMesmaTurmaPratica( int campusId, int P );					// ss_{a1,a2,dp}
   int criaVariavelTaticoFolgaMinimoDemandaPorAluno( int campusId, int P_ATUAL );			// fmd_{a}
   int criaVariavelFolgaOcupacaoSala( int campusId, int P_ATUAL );							// fos_{i,d,cp}
   int criaVariavelProfTurmaAPartirDeZ();													// y_{p,i,d,cp}
   int criaVariavelProfAulaAPartirDeX();													// k_{p,i,d,cp,t,h}
   int criaVariavelProfessorDiaHorarioIF();
   int criaVariaveisHiHfProfFaseDoDiaAPartirDeK(void);										// hip_{p,t,f} e hfp_{p,t,f}
   int criaVariaveisHiHfAlunoDiaAPartirDeV(void);											// hia_{a,t} e hfa_{a,t}
   int criaVariavelFolgaGapProfAPartirDeK(void);											// fpgap_{p,t,f}
   int criaVariavelFolgaGapAlunoAPartirDeV(void);											// fagap_{a,t}
   
   int criarVariavelFolgaMinCredsDiaAluno();												// fcad_{a,t}

   /********************************************************************
   **              CRIA��O DE RESTRI��ES DO TATICO-ALUNO              **
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
   int criaRestricaoTaticoAtendeAlunoEquivTotal( int campusId, int prioridade );
   int criaRestricaoTaticoAtivaZ( int campusId ); // n�o precisa, pode deletar. A restri��o criaRestricaoTaticoSalaUnica engloba esta
   int criaRestricaoTaticoTurmaComOsMesmosAlunosPorAula( int campusId );
   int criaRestricaoTaticoDiscPTAulasContinuas( int campusId, int prioridade );
   int criaRestricaoTaticoAlocMinAluno( int campusId );
   int criaRestricaoFolgaOcupacaoSala( int campusId );
   int criaRestricaoProfDescansoMinimo( int campusId );
   int criaRestricaoGapsHorariosProfessores();
   int criaRestricaoGapsHorariosAlunos();
   
   int criaRestricaoSobreposHorariosProfs();
   int criaRestricaoProfAula();
   int criaRestricaoProfAulaSum();
   int criaRestricaoProfTurma();
   int criaRestricaoProfUnico();
   
	// criar restri��es que impedem gaps nos hor�rios do professor em uma mesma fase de um dia
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
	
	// criar restri��es que impedem gaps nos hor�rios do aluno em um mesmo dia
	int criarRestricaoAlunoHiHf_();
	int criarRestricaoAlunoHiUB_(Aluno* const aluno, const int dia,
		const int colHi, map<DateTime, set< pair<int,double> >> const & mapDtiVars);
	int criarRestricaoAlunoHiLB_(Aluno* const aluno, const int dia,
		const int colHi, map<DateTime, set< pair<int,double> >> const & mapDtiVars);
	int criarRestricaoAlunoHfLB_(Aluno* const aluno, const int dia,
		const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars);
	int criarRestricaoAlunoHfUB_(Aluno* const aluno, const int dia,
		const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars);
	int criarRestricaoAlunoGap_(Aluno* const aluno, const int dia,
		int rhs, const int colHi, const int colHf, set< pair<int,double> > const &varsColCoef);

	int criarRestricaoMinCredsDiaAluno();


   /* 
		****************************************************************************************************************
								MAPS para organizar variaveis existentes e agilizar as restri��es
   */
   
	map< Campus*, map< Disciplina*, map< int /*turma*/, map< Professor*, map< int /*dia*/, map<DateTime /*dti*/, 
		pair<int /*col*/, VariableMIPUnico> > >, LessPtr<Professor> > >, LessPtr<Disciplina> >, LessPtr<Campus> > vars_prof_aula1;	// k_{p,i,d,t,hi}
	map< Professor*, map< int /*dia*/, map<DateTime /*dti*/, map< Campus*, map< Disciplina*, map< int /*turma*/,
		pair<int /*col*/, VariableMIPUnico> >, LessPtr<Disciplina> >, LessPtr<Campus> > > >, LessPtr<Professor> > vars_prof_aula2;	// k_{p,i,d,t,hi}
	
	map< Aluno*, map< int /*dia*/, map<DateTime /*dti*/, map< Campus*, map< Disciplina*, map< int /*turma*/,
		set< pair<int /*col*/, VariableMIPUnico> > >, LessPtr<Disciplina> >, LessPtr<Campus> > > >, LessPtr<Aluno> > vars_aluno_aula;		// v_{a,i,d,s,t,hi,hf}

	map< Professor*, map< int /*dia*/, map< int /*fase*/, pair<DateTime /*dti*/, DateTime /*dtf*/> > >,
		LessPtr<Professor> > vars_prof_dia_fase_dt;												// min dti e max dtf para cada prof/dia/fase
	
	map< Aluno*, map< int /*dia*/, pair<DateTime /*dti*/, DateTime /*dtf*/> >,
		LessPtr<Aluno> > vars_aluno_dia_dt;														// min dti e max dtf para cada aluno/dia
	
	// vars inteiras que indicam o primeiro hip e o �ltimo hfp hor�rio da fase do dia (M/T/N) usados pelo professor
	// Prof -> Dia -> FaseDoDia -> (col. nr. hip/ col. nr. hfp)
	unordered_map<Professor*, unordered_map<int, unordered_map<int, pair<int,int>>>> varsProfDiaFaseHiHf;							// hip_{p,t,f} e hfp_{p,t,f}

	// vars inteiras que indicam o primeiro hia e o �ltimo hfa hor�rio do dia usados pelo aluno
	// Aluno -> Dia -> (col. nr. hia/ col. nr. hfa)
	unordered_map<Aluno*, unordered_map<int, pair<int,int>>> varsAlunoDiaHiHf;														// hia_{a,t} e hfa_{a,t}
	
	// Professor -> Dia -> Fase do dia -> (col. nr. fpgap)
	unordered_map<Professor*, unordered_map<int, unordered_map<int,int>>> varsProfFolgaGap;											// fpgap_{p,t,f}
	
	// Aluno -> Dia -> (col. nr. fagap)
	unordered_map<Aluno*, unordered_map<int, int>> varsAlunoFolgaGap;																// fagap_{a,t}
	
	map< Campus*, map< Disciplina*, map< int /*turma*/, map< Professor*, 
		pair<int /*col*/, VariableMIPUnico>, LessPtr<Professor> > >, LessPtr<Disciplina> >, LessPtr<Campus> > vars_prof_turma;		// y_{p,i,d,cp}
	
	unordered_map< Campus*, unordered_map< Disciplina*, unordered_map< int /*turma*/, unordered_map< int /*dia*/,
		map<DateTime /*dti*/, unordered_map<int /*col*/, VariableMIPUnico> > > > > > vars_turma_aula;								// x_{i,d,s,t,hi,hf}
	
	map< Campus*, map< Disciplina*, map< int /*turma*/,
		pair<int /*col*/, VariableMIPUnico> >, LessPtr<Disciplina> >, LessPtr<Campus> > vars_abertura_turma;						// z_{i,d,cp}

   /* 
		****************************************************************************************************************
   */

	static int idCounter;

   // The linear problem.	
   
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif
	   
	bool optimized;
    double *xSol_;
   
   // Hash which associates the column number with the VariableTatico object.
   VariableMIPUnicoHash vHashTatico;

   // Hash which associates the row number with the ConstraintTatico object.
   ConstraintMIPUnicoHash cHashTatico;


   bool *CARREGA_SOLUCAO;
   bool USAR_EQUIVALENCIA;
   bool PERMITIR_NOVAS_TURMAS;
   bool CRIAR_VARS_FIXADAS;
   int ITERACAO;
   bool PERMITIR_REALOCAR_ALUNO;

   int etapa;

	void updateOptLogFileName(int campusId, int prioridade, int r);
    void chgCoeffList( std::vector< std::pair< int, int > > , std::vector< double > );
	bool violaInsercao( Aluno* aluno, GGroup< VariableMIPUnico *, LessPtr<VariableMIPUnico> > aulasX );
	void imprimeGradeHorAlunos( int campusId, int prioridade );
	void imprimeGradeHorAlunosPorDemanda( int campusId, int prioridade );
	void imprimeTurmaProf( int campusId, int prioridade );
	void imprimeProfTurmas( int campusId, int prioridade );
	void imprimeTodasVars(int p);
	void confereCorretude( int campusId, int prioridade );
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
	void setOptLogFile(std::ofstream &file, string name, bool clear=true);
	void clearVariablesMaps();
	void carregaVariaveisSolucao( int campusAtualId, int prioridade, int r );

	int solveMIPUnico( int campusId, int prioridade, int r );
	int solveGaranteSolucao( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtendPorFasesDoDia( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtend( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtendCalourosFormandos( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveGeneral( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	
	bool SolVarsFound( VariableTatico v );
	bool criaVariavelTaticoInt( VariableMIPUnico *v, bool &fixar, int prioridade );
	Unidade* retornaUnidadeDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
	ConjuntoSala* retornaSalaDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
	GGroup< VariableTatico *, LessPtr<VariableTatico> > retornaAulasEmVarX( int turma, Disciplina* disciplina, int campusId );
	
	GGroup< VariableMIPUnico *, LessPtr<VariableMIPUnico> > vars_v;
	GGroup< VariableMIPUnico *, LessPtr<VariableMIPUnico> > solVarsTatInt;

	GGroup< VariableTatico *, LessPtr<VariableTatico> > *solVarsTatico;
	GGroup< VariableTatico *, LessPtr<VariableTatico> > *vars_xh;

	
	unordered_map< Professor*, unordered_map< Campus*, unordered_map< Disciplina*, unordered_set<int>> > > solAlocProfTurma;
	unordered_map< Campus*, unordered_map< Disciplina*, unordered_map< int, unordered_set<Professor*> > > > solAlocTurmaProf;
	

	// map com o nro de folgas de demanda para o campus atual e prioridade no m�ximo P atual.
	std::map< Disciplina*, int, LessPtr< Disciplina > > mapDiscNroFolgasDemandas;
	inline int haFolgaDeAtendimento( Disciplina *disciplina ) { return this->mapDiscNroFolgasDemandas[disciplina]; }
	bool permitirAbertura( int turma, Disciplina *disciplina, int campusId );

	void atualizarDemandasEquiv( int campusId, int prioridade );

	std::map< Trio< int, Disciplina *, int >, bool > mapPermitirAbertura;

	std::map< int /*discId*/, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> > mapDiscAlunosDemanda; // para auxilio na cria��o das variaveis
	int getNroMaxAlunoDemanda( int discId );
	
	
   /* 
		****************************************************************************************************************
   */
	
	// Constantes e par�metros
	    
	// Disciplinas
	static const int consideraDivCredDisc;

	// Professores
	static const bool minimizarCustoProf;
	static const bool permiteCriarPV;
	static const int pesoGapProf;

	// Alunos
	static const int pesoGapAluno;
	static const int pesoMinCredDiaAluno;
	static const int desvioMinCredDiaAluno;		// desvio m�ximo do nr m�dio de cr�ditos por dia do aluno, sem que haja penaliza��o 
	static const int considerarMinCredDiaAluno;

	// log file name
	string optLogFileName;
};


#endif