#ifndef _ESTIMA_TURMAS_H_
#define _ESTIMA_TURMAS_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "Solver.h"
#include "opt_lp.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolutionLoader.h"
#include "ErrorHandler.h"
#include "ProblemDataLoader.h"
#include "VariableEstimaTurmas.h"
#include "ConstraintEstimaTurmas.h"

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#endif

class EstimaTurmas : public Solver
{
public:
	
	EstimaTurmas::EstimaTurmas( ProblemData * &aProblemData, bool *endCARREGA_SOLUCAO );

	virtual ~EstimaTurmas();


	int solve();
    void getSolution( ProblemSolution * );
	void solveEstimaTurmas( int campusId, int prioridade );

	void calculaResultados( int campusId, int prioridade );
	void carregaVariaveis( int campusAtualId, int prioridade );
	int solveModeloEstimaTurmas( int campusId, int prioridade );
	int atendMaximoSemSabado( int campusId, int P, bool &CARREGA_SOL_PARCIAL, double *xS );
	int solvePorFasesDoDia( int campusId, int prioridade, bool& CARREGA_SOL_PARCIAL, double *xS );
	int fixaAtendParcial( int campusId, int P, double *xS );

	void setNumTurmas( int campusAtualId, int prioridade );
	void imprimeNumTurmas( int campusAtualId, int prioridade, int fase );
	void imprimeTurmasPT_1xN( int campusAtualId, int prioridade, int fase );

private:

   enum OutPutFileType
   {
	  ESTIMA_TURMA_BIN = 0,
	  ESTIMA_TURMA_BIN1 = 1
   };	  

   /********************************************************************
   **             CRIAÇÃO DE VARIAVEIS DO EstimaTurmas                **
   *********************************************************************/

   
   int criaVariaveisEstimaTurmas( int campusId, int P );

   int criaVariavelNumTurmas( int campusId, int P );
   int criaVariavelLimiteSupCredsNasSalas( int campusId, int P );
   int criaVariavelFolgaTurmasMesmaDiscNaSala( int campusId, int P );
   int criaVariavelFolgaDemanda( int campusId, int P );
   int criaVariavelNumAtendPorTurno( int campusId, int P );
   
   int criaVariavelFolgaDemandaDiscAluno( int campusId, int P );
   int criaVariavelAlocaAlunoTurmaDiscEquiv( int campusId, int P );
   int criaVariavelSomaCredSala( int campusId ); // deletar
   int criaVariavelSomaCredSalaPorTurno( int campusId );
   int criaVariavelSalaAluno( int campusId );
   int criaVariavelNumDisciplinas( int campusId, int P );

   /********************************************************************
   **              CRIAÇÃO DE RESTRIÇÕES DO EstimaTurmas              **
   *********************************************************************/

   int criaRestricoesEstimaTurmas( int campusId, int prioridade );
   
   int criaRestricaoTempoMaxSala( int campusId );
   int criaRestricaoTempoMaxSalaSabado( int campusId );   
   int criaRestricaoTempoMaxAlunoSabado( int campusId );
   int criaRestricaoAtendeDemandas( int campusId );
   int criaRestricaoCapacidadeSala( int campusId );
   int criaRestricaoDistribuiEntreSalas( int campusId );
   int criaRestricaoTurmasDifMesmaDiscSalaDif( int campusId );
   int criaRestricaoDiscPraticaTeorica( int campusId );

   int criaRestricaoAtendeAlunoDemanda( int campusId, int prioridade );
   int criaRestricaoDiscPraticaTeoricaPorAluno( int campusId );
   int criaRestricaoAtendeDemandasComAluno( int campusId );
   int criaRestricaoSomaCredSalaPorTurno( int campusId );
   int criaRestricaoNroSalasPorAluno( int campusId );

   int criaRestricaoNroTurmasPratTeorIgual( int campusId );
   int criaRestricaoUsaDisciplina( int campusId );

   // The linear problem.	
   
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif
	   
    void chgCoeffList( std::vector< std::pair< int, int > > , std::vector< double > );

   // Hash which associates the column number with the VariableTatico object.
   VariableEstimaTurmasHash vHashTatico;

   // Hash which associates the row number with the ConstraintTatico object.
   ConstraintEstimaTurmasHash cHashTatico;
   
	GGroup< VariableEstimaTurmas *, LessPtr<VariableEstimaTurmas> > solVarsEstimaTurmas;
	
	
    bool *CARREGA_SOLUCAO;

	std::string getSolucaoEstimaTurmaFileName( int campusId, int prioridade, int fase );
	std::string getSolBinFileName( int campusId, int prioridade, int fase);
	std::string getEstimaTurmaLpFileName( int campusId, int prioridade, int fase );
	std::string getNumTurmasFileName( int campusId, int prioridade, int fase);
	
	void writeSolTxt( int campusId, int prioridade, int fase, int type, double *xSol );
	void writeSolBin( int campusId, int prioridade, int fase, int type, double *xSol );
	int readSolBin( int campusId, int prioridade, int fase, int type, double *xSol );
	int readSolTxt( int campusId, int prioridade, int fase, int type, double *xSol );

	int writeGapTxt( int campusId, int prioridade, int fase, int type, double gap );
};


#endif