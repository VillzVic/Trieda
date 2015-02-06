#ifndef _MIP_UNICO_H_
#define _MIP_UNICO_H_

#include <vector>
#include <fstream>
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
				GGroup<VariableTatico *, LessPtr<VariableTatico>> *aSolVarsTatico, 
				GGroup<VariableTatico *, LessPtr<VariableTatico>> *avars_xh,
				bool *endCARREGA_SOLUCAO, bool equiv, int permitirNovasTurmas );

	virtual ~MIPUnico();


	int solve();
    void getSolution( ProblemSolution * );
	void solveMainEscola( int campusId, int prioridade, int r, 
		std::set<VariableMIPUnico *, LessPtr<VariableMIPUnico>> &solMipUnico );
	void copyFinalSolution(std::set<VariableMIPUnico*, LessPtr<VariableMIPUnico>> &solMipUnico);

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
	  MIP_GENERAL,
	  MIP_GARANTE_SOL,
	  MIP_MAX_ATEND,
	  MIP_MIN_VIRT,
	  MIP_MIN_TURMAS_COMPART,
	  MIP_MIN_FASE_DIA_PROF,
	  MIP_MIN_DESLOC_PROF,
	  MIP_MIN_GAP_PROF,
	  MIP_MARRETA
   };	  

   static std::string getOutPutFileTypeToString(OutPutFileType type);

   void preencheMapDiscAlunosDemanda( int campusId, int P, int r );
   bool haDemanda(Disciplina* const disc) const;
   bool haDemandaNaoAtendida(Disciplina* const disc);
   bool demandaTodaAtendidaPorReal(Disciplina* const disc);
   bool existeTurmaAtendida(Campus* const campus, Disciplina* const disc, int turma) const;
   bool haDemandaPossivelNoDiaHor(Disciplina* const disc, int dia, HorarioAula* const ha);
   bool permitirAlunoDiscNoHorDia(AlunoDemanda* const alDem, int dia, DateTime dti) const;
   bool permitirAlunoNaTurma(Aluno* const aluno, Disciplina* const disciplina, int turma) const;
   bool permitirTurma(Campus* const campus, Disciplina* const disciplina, int turma);
   bool haProfHabilitNoDiaHor(Disciplina* const disc, int dia, HorarioAula* const ha);
   bool alunoHorVazioNoDia(Aluno* const aluno, int dia, DateTime dti) const;
   bool alunoAlocNaTurma(Aluno* const aluno, Disciplina* const disciplina, int turma) const;
   int alunoAlocDisc(Aluno* const aluno, Disciplina* const disc) const;
   bool alunoAlocDiscNoHorDia(Aluno* const aluno, Disciplina* const disc, int dia, DateTime dti) const;
   bool profAlocNaTurma(Professor* const prof, Campus* const campus, Disciplina* const disciplina, int turma) const;

   /********************************************************************
   **             CRIAÇÃO DE VARIAVEIS DO TATICO-ALUNO                **
   *********************************************************************/

   
   int criaVariaveisTatico( int campusId, int P, int r );

   int criaVariavelTaticoAlunoCreditosAPartirDeX( int campusId, int P );					// v_{a,i,d,u,s,hi,hf,t}
   int criaVariavelTaticoOferecimentosAPartirDeX( int campusId, int P );
   int criaVariavelTaticoCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P );			// m_{i,d,k}
   int criaVariavelTaticoFolgaCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P );	// fkp_{i,d,k} e fkm_{i,d,k}
   int criaVariavelTaticoAberturaCompativelAPartirDeX( int campusId, int P );				// zc_{d,t}
   int criaVariavelTaticoAberturaAPartirDeX( int campusId, int prior );
   int criaVariavelTaticoAlocaAlunoTurmaDiscAPartirDeV( int campusId, int P );				// s_{i,d,a,cp}

   int criaVariavelTaticoCreditos( int campusId, int P );									// x_{i,d,u,s,hi,hf,t}
   int criaVariavelTaticoOferecimentos( int campusId, int P );								// o_{i,d,u,s}
   int criaVariavelTaticoCursoAlunos( int campusId, int P );								// b_{i,d,c,c'}
   int criaVariavelTaticoFolgaDemandaDiscAluno( int campusId, int P  );						// fd_{d,a}
   int criaVariavelTaticoDiaUsadoPeloAlunoAPartirDeV();										// du_{a,t}
   int criaVariavelTaticoFolgaAbreTurmaSequencial( int campusId, int P );					// ft_{i,d,cp}
   int criaVariavelFolgaProibeCompartilhamento( int campusId, int P );						// fc_{i,d,c,c',cp}
   int criaVariavelFolgaPrioridadeInf( int campusId, int prior );							// fpi_{a,cp}
   int criaVariavelFolgaPrioridadeSup( int campusId, int prior );							// fps_{a,cp}
   int criaVariavelTaticoAbertura( int campusId, int prior, int r );						// z_{i,d,cp}
   int criaVariavelTaticoAlunosMesmaTurmaPratica( int campusId, int P );					// ss_{a1,a2,dp}
   int criaVariavelTaticoFolgaMinimoDemandaPorAluno( int campusId, int P_ATUAL );			// fmd_{a}
   int criaVariavelProfTurmaAPartirDeZ();													// y_{p,i,d,cp}
   int criaVariavelProfTurmaAPartirDeK();
   int criaVariavelProfAulaAPartirDeX();													// k_{p,i,d,cp,t,h}
   int criaVariaveisHiHfProfFaseDoDiaAPartirDeK(void);										// hip_{p,t,f} e hfp_{p,t,f}
   int criaVariaveisHiHfAlunoDiaAPartirDeV(void);											// hia_{a,t} e hfa_{a,t}
   int criaVariaveisInicioFimAlunoDiaAPartirDeV(void);										// inicio_{a,t,h} e fim_{a,t,h}
   int criaVariavelFolgaGapProfAPartirDeK(void);											// fpgap_{p,t,f}
   int criaVariavelUnidUsadaProfAPartirDeK(void);											// uu_{p,t,u}
   int criaVariavelDeslocProfAPartirDeK(void);												// desloc_{p,t,u1,2}
   int criaVariavelFolgaGapAlunoAPartirDeV(void);											// fagap_{a,t}
   int criaVariavelWAPartirDeX();															// w_{p,u,t,h}

   int criarVariavelFolgaMinCredsDiaAluno();												// fcad_{a,t}
   int criaVariavelFolgaCargaHorariaAnteriorProfessor();									// fch_{p}
   int criaVariaveisProfUsaFaseDiaAPartirDeK(void);											// ptf_{p,t,f}
   int criaVariaveisProfDiaUsadoAPartirDeK(void);											// pt_{p,t}

   bool checkValidCol(int col) const;

   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO TATICO-ALUNO              **
   *********************************************************************/

   int criaRestricoesTatico( int campusId, int prioridade, int r );

   int criaRestricaoTaticoAssociaVeX( int campusId );
   int criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId );		// Restricao 1.2.3   
   int criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId ); // Restricao 1.2.4
   int criaRestricaoTaticoCursosIncompat( int campusId );
   int criaRestricaoTaticoProibeCompartilhamento( int campusId );
   int criaRestricaoTaticoLimitaAberturaTurmas( int campusId, int prioridade ); 
   int criaRestricaoTaticoLimitaMaximoAlunosNasTurmas( int campusId, int prioridade );
   int criaRestricaoTaticoAlunoDiscPraticaTeoricaEquiv_MxN( int campusId, int prioridade );
   int criaRestricaoTaticoAlunoDiscPraticaTeorica_MxN( int campusId );
   int criaRestricaoTaticoAlunoDiscPraticaTeorica_1x1( int campusId );
   int criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN( int campusId );
   int criaRestricaoTaticoDivisaoCredito_hash( int campusId );
   int criaRestricaoTaticoCombinacaoDivisaoCredito( int campusId );		// Restricao 1.2.9
   int criaRestricaoTaticoAlunoHorario( int campusId );					// Restricao 1.2.12
   int criaRestricaoTaticoAlunoUnidDifDia( int campusId );
   int criaRestricaoTaticoDiaUsadoPeloAluno();
   int criaRestricaoTaticoAtendeAluno( int campusId, int prioridade );
   int criaRestricaoTaticoSalaUnica( int campusId );
   int criaRestricaoTaticoSalaPorTurma( int campusId );
   int criaRestricaoTaticoAssociaAlunoEGaranteNroCreds( int campusId );
   int criaRestricaoTaticoAbreTurmasEmSequencia( int campusId );
   int criaRestricaoTaticoAlunoCurso( int campusId );
   int criaRestricaoPrioridadesDemanda( int campusId, int prior );
   int criaRestricaoTaticoAtendeAlunoEquivTotal( int campusId, int prioridade );
   int criaRestricaoTaticoAtivaZ( int campusId ); // não precisa, pode deletar. A restrição criaRestricaoTaticoSalaUnica engloba esta
   int criaRestricaoTaticoTurmaComOsMesmosAlunosPorAula( int campusId );
   int criaRestricaoTaticoDiscPTAulasContinuas( int campusId, int prioridade );
   int criaRestricaoTaticoAlocMinAluno( int campusId );
   int criaRestricaoProfDescansoMinimo( int campusId );
   
   int criaRestricaoSobreposHorariosProfs();
   int criaRestricaoProfAula();
   int criaRestricaoProfAulaSum();
   int criaRestricaoProfTurma();
   int criaRestricaoProfUnico();
   
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
	
	// criar restrições que impedem gaps nos horários do aluno em um mesmo dia
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

	// criar restrições que impedem gaps nos horários do aluno em um mesmo dia (nova formulação)
	int criarRestricaoGapDiaAluno();
	int criarRestricaoAlunoDia_( Aluno* const aluno, const int dia,
			map<DateTime, pair<int,int>> const &mapDiaVarsInicioFim, 
			map<DateTime /*dti*/, map<Campus*, map<Disciplina*, map<int/*turma*/,	
				set<pair<int/*col*/, VariableMIPUnico>>>, LessPtr<Disciplina>>, LessPtr<Campus>>> const &mapDiaVarsV);
	int criarRestricaoAlunoDiaImpedeGap_( Aluno* const aluno, const int dia,
			map<DateTime, pair<int,int>> const &mapDiaVarsInicioFim, 
			map<DateTime /*dti*/, map<Campus*, map<Disciplina*, map<int/*turma*/,	
				set<pair<int/*col*/, VariableMIPUnico>>>, LessPtr<Disciplina>>, LessPtr<Campus>>> const &mapDiaVarsV);
	int criarRestricaoAlunoDiaInicio_( Aluno* const aluno, const int dia,
			map<DateTime, pair<int,int>> const &mapDiaVarsInicioFim, 
			map<DateTime /*dti*/, map<Campus*, map<Disciplina*, map<int/*turma*/,	
				set<pair<int/*col*/, VariableMIPUnico>>>, LessPtr<Disciplina>>, LessPtr<Campus>>> const &mapDiaVarsV);
	int criarRestricaoAlunoDiaFim_( Aluno* const aluno, const int dia,
			map<DateTime, pair<int,int>> const &mapDiaVarsInicioFim, 
			map<DateTime /*dti*/, map<Campus*, map<Disciplina*, map<int/*turma*/,	
				set<pair<int/*col*/, VariableMIPUnico>>>, LessPtr<Disciplina>>, LessPtr<Campus>>> const &mapDiaVarsV);
	int criarRestricaoAlunoDiaInicioUnico_( Aluno* const aluno, const int dia,
			map<DateTime, pair<int,int>> const &mapDiaVarsInicioFim);
	int criarRestricaoAlunoDiaFimUnico_( Aluno* const aluno, const int dia,
			map<DateTime, pair<int,int>> const &mapDiaVarsInicioFim);

	int criarRestricaoMinCredsDiaAluno();
	int criaRestricaoTempoDeslocProfessor();
	int criaRestricaoMax1DeslocProfessor();
	int criaRestricaoMinDeslocProfessor();
	int criaRestricaoMaxDeslocLongoProfessor();
	int criaRestricaoMaxDeslocLongoSemanaProfessor();
	int criaRestricaoUnidUsadaProf();
	int criaRestricaoNrMaxUnidDiaProf();
	int criaRestricaoProfDiaFaseUsada();
	int criaRestricaoProfDiaUsado();
	int criaRestricaoProfBuracoEntreFases();
	int criaRestricaoProfMinCredsDiaUsado();
	int criaRestricaoRedCargaHorAnteriorProfessor();

	int criarRestricaoMinCredsDiaAluno_Marreta();
	int criarVariavelFolgaMinCredsDiaAluno_MarretaCaso1e2();
	int criarRestricaoHorInicialDiaAluno_MarretaCaso1e2();
	int criarRestricaoHorFinalDiaAluno_MarretaCaso1();
	int criarRestricaoMinCredsDiaAluno_MarretaCaso1e2();
	int criarRestricaoMinCredsDiaAluno_MarretaCaso2();
	int criarVariavelDiaLongoAluno_MarretaCaso2();
	int criarRestricaoMinCredsDiaAluno_MarretaCaso2_1();
	int criarRestricaoMinCredsDiaAluno_MarretaCaso2_TardeUsada();
	int criarRestricaoMinCredsDiaAluno_MarretaCaso2_UnicaTarde();
	
   /* 
		****************************************************************************************************************
								MAPS para organizar variaveis existentes e agilizar as restrições
   */
   
	map< Unidade*, map< Disciplina*, map< int /*turma*/, map< Professor*, map< int /*dia*/, map<DateTime /*dti*/, 
		pair<int /*col*/, VariableMIPUnico> > >, LessPtr<Professor> > >, LessPtr<Disciplina> >, LessPtr<Unidade> > vars_prof_aula1;	// k_{p,i,d,u,t,hi}
	map< Professor*, map< int /*dia*/, map<DateTime /*dti*/, map< Unidade*, map< Disciplina*, map< int /*turma*/,
		pair<int /*col*/, VariableMIPUnico> >, LessPtr<Disciplina> >, LessPtr<Unidade> > > >, LessPtr<Professor> > vars_prof_aula2;	// k_{p,i,d,u,t,hi}
	map< Professor*, map< int /*dia*/, map<Unidade*, map< HorarioAula*,
		set< pair<int /*col*/,VariableMIPUnico> >, LessPtr<HorarioAula> >, LessPtr<Unidade> > >, LessPtr<Professor> > vars_prof_aula3;	// k_{p,i,d,u,t,hi}
	
	map<Professor*, map<int /*dia*/, map<Unidade*, int /*col*/>>> vars_prof_dia_unid;
	map<Professor*, map<int /*dia*/, map<Unidade*, map<Unidade*,int /*col*/>>>> vars_prof_desloc;										// desloc_{p,t,u1,u2}

	map< Aluno*, map< int /*dia*/, map<DateTime /*dti*/, map< Campus*, map< Disciplina*, map< int /*turma*/,
		set< pair<int /*col*/, VariableMIPUnico> > >, LessPtr<Disciplina> >, LessPtr<Campus> > > >, LessPtr<Aluno> > vars_aluno_aula;		// v_{a,i,d,s,t,hi,hf}

	map< Professor*, map< int /*dia*/, map< int /*fase*/, pair<DateTime /*dti*/, DateTime /*dtf*/> > >,
		LessPtr<Professor> > vars_prof_dia_fase_dt;												// min dti e max dtf para cada prof/dia/fase
	
	map< Aluno*, map< int /*dia*/, pair<DateTime /*dti*/, DateTime /*dtf*/> >,
		LessPtr<Aluno> > vars_aluno_dia_dt;														// min dti e max dtf para cada aluno/dia
	
	// vars inteiras que indicam o primeiro hip e o último hfp horário da fase do dia (M/T/N) usados pelo professor
	// Prof -> Dia -> FaseDoDia -> (col. nr. hip/ col. nr. hfp)
	unordered_map<Professor*, map<int, map<int, pair<int,int>>>> varsProfDiaFaseHiHf;							// hip_{p,t,f} e hfp_{p,t,f}

	// variaveis binarias que indicam se o professor usou a fase do dia
	// Prof -> Dia -> FaseDoDia -> (col. nr)
	unordered_map<Professor*, map<int, map<int, int>>> varsProfDiaFaseUsada;									// ptf_{p,t,f}
	// Prof -> Dia -> (col. nr)
	unordered_map<Professor*, map<int, int>> varsProfDiaUsado;													// pt_{p,t}

	// vars inteiras que indicam o primeiro hia e o último hfa horário do dia usados pelo aluno
	// Aluno -> Dia -> (col. nr. hia/ col. nr. hfa)
	map<Aluno*, unordered_map<int, pair<int,int>>> varsAlunoDiaHiHf;									// hia_{a,t} e hfa_{a,t}
	map<Aluno*, unordered_map<int, map<DateTime, pair<int,int>>>, LessPtr<Aluno>> varsAlunoDiaInicioFim;	// inicio_{a,t,h} e fim_{a,t,h}
	
	// Professor -> Dia -> Fase do dia -> (col. nr. fpgap)
	unordered_map<Professor*, unordered_map<int, unordered_map<int,int>>> varsProfFolgaGap;											// fpgap_{p,t,f}
	
	// Aluno -> Dia -> (col. nr. fagap)
	unordered_map<Aluno*, unordered_map<int, int>> varsAlunoFolgaGap;																// fagap_{a,t}
	
	map< Campus*, map< Disciplina*, map< int /*turma*/, map< Professor*, 
		pair<int /*col*/, VariableMIPUnico>, LessPtr<Professor> > >, LessPtr<Disciplina> >, LessPtr<Campus> > vars_prof_turma;		// y_{p,i,d,cp}
	
	unordered_map< Campus*, unordered_map< Disciplina*, unordered_map< int /*turma*/, unordered_map< int /*dia*/,
		map<DateTime /*dti*/, unordered_map<int /*col*/, VariableMIPUnico> > > > > > vars_turma_aula;								// x_{i,d,s,t,hi,hf}
	
	unordered_map< Unidade*, unordered_map< Disciplina*, unordered_map< int /*turma*/, unordered_map< int /*dia*/,
		map<DateTime /*dti*/, unordered_map<int /*col*/, VariableMIPUnico> > > > > > vars_turma_aula2;								// x_{i,d,s,t,hi,hf}
	
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
	   
	bool optimized_;
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
	std::string getCorrigeNrTurmasFileName( int campusId, int prioridade, int r);
	std::string getTaticoLpFileName( int campusId, int prioridade, int r );
	int readSolTxt( int campusId, int prioridade, int r, int type, double *xSol, int fase );
	std::string getSolucaoTaticoFileName( int campusId, int prioridade, int r );	
	std::string getEtapaName(int campusId, int prioridade, int r);
	void getPrefixFileName(int type, std::string & prefix);
	void getSolEtapaFileName(int campusId, int prioridade, int r, int type, std::string & fileName, bool byCode=false);
	void writeSolTxt( int campusId, int prioridade, int r, int type, double *xSol, int fase );
	void writeSolTxtById( int campusId, int prioridade, int r, int type, double *xSol, int fase );
	void writeSolTxtByCode( int campusId, int prioridade, int r, int type, double *xSol, int fase );
	bool getSolFilePt( int campusId, int prioridade, int r, int type, ifstream & fin, bool byCode=false );

	void sincronizaSolucao( int campusAtualId, int prioridade, int r );
	void addVariaveisTatico();
	void initCredsSala();
	void setOptLogFile(std::ofstream &file, string name, bool clear=true);
	void deleteVariablesSol();
	void clearVariablesMaps();
	void clearMapsSolution();
	void clearStrutures();
	void resetXSol();
	void carregaVariaveisSolucao( int campusAtualId, int prioridade, int r );
	bool carregaVariaveisSolucaoFromFile(int campusId, int prioridade, int r, MIPUnico::OutPutFileType type);
	void openSolucaoFile(FILE* &fout, int campusId, int prioridade, int r);
	void verificaCarregaSolucao( int campusId, int prioridade, int r );
	void criaNewLp( int campusId, int prioridade, int r );
	void printLog( string msg );

	void setGurobiVarsPrior();
	void makeLazyConstr();
	void getIdxN(int* idx);

	int solveMIPUnico( int campusId, int prioridade, int r );
	int solveMIPUnico_unico(int campusId, int prioridade, int r);
	int solveMIPUnico_pduplo(int campusId, int prioridade, int r);
	int solveMIPUnico_ptriplo(int campusId, int prioridade, int r);

	int solveMIPUnico_v2(int campusId, int prioridade, int r);
	int solveMIPUnicoEtapas(int campusId, int prioridade, int r, bool CARREGA_SOL_PARCIAL);	
	int solveMIPUnicoEtapaReal_Off(int campusId, int prioridade, int r, bool &CARREGA_SOL_PARCIAL);
	int solveMIPUnicoEtapaReal_1(int campusId, int prioridade, int r, bool &CARREGA_SOL_PARCIAL);
	int solveMIPUnicoEtapaReal_2(int campusId, int prioridade, int r, bool &CARREGA_SOL_PARCIAL);
	int solveMIPUnicoEtapaReal_3(int campusId, int prioridade, int r, bool &CARREGA_SOL_PARCIAL);
	int solveMIPUnicoEtapaReal_ProfPrior(int campusId, int prioridade, int r, bool &CARREGA_SOL_PARCIAL);
	int solveMIPUnicoEtapaVirtual(int campusId, int prioridade, int r, bool &CARREGA_SOL_PARCIAL);
	
	void fixaSolucaoReal();
	void fixaVariaveisPVZero(double * const xS);
	void liberaVariaveisPV(double * const xS);
	void fixaVariaveisProfNaoImportanteZero(double * const xS);
	void liberaVariaveisProfNaoImportanteZero(double * const xS);
	void fixaVariaveisProfImportanteAtend(double * const xS);

	int solveGaranteSolucao( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	void zeraObjSolucao(int &nBdsObj, int* idxN);
	void zeraAtendGaranteSolucao(int &nBds, double* valsOrig, int* idxs, BOUNDTYPE* bds);
	int solveMaxAtendPorFasesDoDia( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtendMarreta( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtend( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMaxAtendCalourosFormandos( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMinProfVirt( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMinTurmas( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMinDeslocProf( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMinFasesDoDiaProf( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveMinGapProf( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int solveGeneral( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	
	int optimizeMaxAtendMarreta( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int optimizeMaxAtend( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int optimizeMinProfVirt( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int optimizeMinTurmas(int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS);
	int optimizeMinDeslocProf(int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS);
	int optimizeMinFasesDoDiaProf( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );
	int optimizeMinGapProf( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS );

	bool fixaSolMaxAtendMarreta(double* const xS);
	bool fixaSolMaxAtendMarretaConstr(double* const xS);
	bool fixaSolVariaveisAtend(double* const xS);
	bool fixaSolMaxAtend(double* const xS);
	bool fixaSolMaxAtendReal(double* const xS);
	bool fixaSolMinProfVirt(double* const xS);
	bool fixaSolMinProfReal(double* const xS);
	bool fixaSolMinTurmas(double* const xS);
	bool fixaSolMinTurmasAbordPvFinal(double* const xS);
	bool fixaSolMinDeslocProf(double* const xS);
	void mapSolMinDeslocProf(double* const xS,
		map<Professor*, map<int, map<VariableMIPUnico, pair<int,double>>>> &solProfDiaDeslocUsado,
		map<Professor*, std::set< pair<int,double> >> &solProfDiaUnidUsada);
	bool fixaSolMinSumDeslocProf(
		map<Professor*, map<int, map<VariableMIPUnico, pair<int,double>>>> const &solProfDiaDeslocUsado);
	bool fixaSolMinDiaDeslocProf(
		map<Professor*, map<int, map<VariableMIPUnico, pair<int,double>>>> const &solProfDiaDeslocUsado);
	bool fixaSolMinUnidDiaProf(
		map<Professor*, std::set< pair<int,double> >> const &solProfDiaUnidUsada);
	bool fixaSolMaxFasesDoDiaProf(double* const xS);
	bool fixaSolMaxDiasProf(double* const xS);
	bool fixaSolMinGapProf(double* const xS);
	bool fixaSolAtendida(double* const xS);

	double getCoefObjMaxAtend(VariableMIPUnico v);

	bool chgObjMaxAtendMarreta();
	bool chgObjMaxAtend();
	bool chgObjMinTurmas();
	bool chgObjMinDeslocProf();
	bool chgObjMinFasesDoDiaProf();
	bool chgObjMinGapProf();

	void printNaoAtendimentos(double* const xS);
	void printAtendsVirtuais(double* const xS);

	bool priorProfLivre();
	bool considerarPriorProf();
	bool priorProf(Professor* const professor, bool ouMenor=false);
	bool priorDisc(Disciplina* const disciplina, bool ouMenor);
	bool ehMarreta(Disciplina *disciplina);

	void getXSol(double *xS);
	bool optimize();
	bool isOptimized(OPTSTAT status);
	bool infeasible(OPTSTAT status);
	bool checkFeasibility(OPTSTAT status);
	
	int addConstrGapProf();
	int copyInitialSolutionGapProf();
	int addConstrFasesDoDiaProf();
	int copyInitialSolutionProfDiaFaseUsada();
	int addConstrDivCred(int campusId);
	int copyInitialSolutionDivCred();
		
	GGroup< VariableMIPUnico *, LessPtr<VariableMIPUnico> > vars_v;
	GGroup< VariableMIPUnico *, LessPtr<VariableMIPUnico> > solVarsTatInt;

	GGroup< VariableTatico *, LessPtr<VariableTatico> > *solVarsTatico;
	GGroup< VariableTatico *, LessPtr<VariableTatico> > *vars_xh;

	
	unordered_map< Professor*, unordered_map< Campus*, unordered_map< Disciplina*, unordered_set<int>> > > solAlocProfTurma;
	unordered_map< Campus*, unordered_map< Disciplina*, unordered_map< int, unordered_set<Professor*> > > > solAlocTurmaProf;	
	unordered_map<Aluno*, unordered_map<Disciplina*, std::pair<int, unordered_map<int, set<DateTime>> >>> solAlocAlunoDiscTurmaDiaDti_;
	unordered_map<Aluno*, unordered_map<int, set<DateTime>>> solAlocAlunoDiaDti_;
		
	std::map<Disciplina*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<Disciplina> > mapDiscAlunosDemanda; // para auxilio na criação das variaveis
		
   /* 
		****************************************************************************************************************
   */
	
	// Constantes e parâmetros

	enum PRIOR_PROF_TYPE
	{
		PriorTypeOff,	// Sem prior de profs	= um único modelo contendo indiferentemente todos os professores reais
		PriorType1,	// Prior 1 > Prior 2	= etapa de maximizar demanda é única, com um único modelo contendo ambas prioridades
		PriorType2,	// Prior 1 >> Prior 2	= todas as etapas são duplas (exceto marreta), com um único modelo contendo ambas prioridades
		PriorType3	// Prior 1 >> Prior 2	= todas as etapas são duplas (exceto marreta), com um modelo para cada prioridade
	};

	    
	// Gurobi
	static const int timeLimitMaxAtend;
	static const int timeLimitMaxAtendSemMelhora;
	static const int timeLimitMinProfVirt;
	static const int timeLimitMinProfVirtSemMelhora;
	static const int timeLimitMinTurmas;
	static const int timeLimitMinTurmasSemMelhora;
	static const int timeLimitMinGapProf;
	static const int timeLimitMinGapProfSemMelhora;
	static const int timeLimitGeneral;
	static const int timeLimitGeneralSemMelhora;
	static const int timeLimitMinDeslocProf_;
	static const int timeLimitMinDeslocProfSemMelhora_;
	static const int timeLimitMinFaseDiaProf_;
	static const int timeLimitMinFaseDiaProfSemMelhora_;

	// Disciplinas
	static const int consideraDivCredDisc;
	static const double pesoDivCred;

	// Professores	
	static bool permiteCriarPV;
	static const bool limitarNrUnidsProfDia_;
	static const int MaxUnidProfDia_;
	static const bool filtroPVHorCompl_;
	static const bool minimizarCustoProf;
	static const double pesoGapProf;
	static const double pesoCredPV;
	static const double pesoDeslocProf;
	static const double pesoCHAntProf;
	static const bool limitar1DeslocSoUnidLonge_;
	static const bool limitarDeslocUnidLongeSemana_;
	static const int maxDeslocUnidLongeSemana_;	
	static const int maxTempoDeslocCurto_;
	static const bool minimizarProfFaseDoDiaUsada_;
	static const bool minimizarProfDiaUsado_;
	static int priorProfLevel_;
	static const PRIOR_PROF_TYPE priorProfImportante_;
	static const bool minimizarGapProfEntreFases_;
	static const int MaxGapEntreFase_;
	static const int minCredDispFaseMinGapProf_;

	static const bool fixarSolucaoProfPrior1_;

	// Alunos
	static const double pesoFD;
	static const int pesoGapAluno;
	static const int pesoMinCredDiaAluno;
	static const int desvioMinCredDiaAluno;		// desvio máximo do nr médio de créditos por dia do aluno, sem que haja penalização 
	static const int considerarMinCredDiaAluno;
	static const bool ignorarGapAlunoContraTurno_;

	// log file name
	string optLogFileName;
    std::ofstream mipFile;

};


#endif