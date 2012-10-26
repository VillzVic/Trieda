#ifndef _TATICO_INT_ALUNO_HOR_H_
#define _TATICO_INT_ALUNO_HOR_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "ValidateSolution.h"
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
	
	TaticoIntAlunoHor::TaticoIntAlunoHor( ProblemData * aProblemData, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *aSolVarsTatico, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *avars_xh, bool *endCARREGA_SOLUCAO, bool equiv );
	virtual ~TaticoIntAlunoHor();


	int solve();
    void getSolution( ProblemSolution * );
	void solveTaticoIntegrado( int campusId, int prioridade, int r );

private:
	  

   /********************************************************************
   **             CRIAÇÃO DE VARIAVEIS DO TATICO-ALUNO                **
   *********************************************************************/

   
   int criaVariaveisTatico( int campusId, int P, int r );

   int criaVariavelTaticoAlunoCreditos( int campusId, int P );								// v_{a,i,d,u,s,hi,hf,t}      
   int criaVariavelTaticoCreditos( int campusId, int P );									// x_{i,d,u,s,hi,hf,t}      
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
   int criaVariavelFolgaProibeCompartilhamento( int campusId, int P );
   int criaVariavelFolgaPrioridadeInf( int campusId, int prior );							// fpi_{a,cp}
   int criaVariavelFolgaPrioridadeSup( int campusId, int prior );							// fps_{a,cp}
   int criaVariavelTaticoFormandosNaTurma( int campusId, int prior, int r );				// f_{i,d,cp}
   int criaVariavelTaticoAlocaAlunoTurmaDiscEquiv( int campusId, int P );					// s_{i,d,a,cp}

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
   int criaRestricaoTaticoAlunoDiscPraticaTeorica( int campusId );
   int criaRestricaoTaticoDivisaoCredito( int campusId );				// Restricao 1.2.8      
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
   int criaRestricaoTaticoFormandos( int campusId, int prioridade, int r );
   int criaRestricaoTaticoAtendeAlunoEquiv( int campusId, int prioridade );
   int criaRestricaoTaticoAlunoDiscPraticaTeoricaEquiv( int campusId, int prioridade );

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
   bool PERMITIR_INSERCAO_ALUNODEMANDAP2_EM_TURMAP1;
   bool *CARREGA_SOLUCAO;
   bool USAR_EQUIVALENCIA;

    void chgCoeffList( std::vector< std::pair< int, int > > , std::vector< double > );
	void calculaNroFolgas( int prioridade, int campusId );
	std::string getTaticoLpFileName( int campusId, int prioridade, int r );
	std::string getSolBinFileName( int campusId, int prioridade, int r );	
	std::string getSolucaoTaticoFileName( int campusId, int prioridade, int r );	
	 	
	void sincronizaSolucao( int campusAtualId, int prioridade, int r );
	void addVariaveisTatico();
	void carregaVariaveisSolucaoTaticoPorAlunoHor( int campusAtualId, int prioridade, int r );
	int solveTaticoIntAlunoHor( int campusId, int prioridade, int r );
	bool SolVarsFound( VariableTatico v );
	bool criaVariavelTatico( VariableTatInt *v, bool &fixar, int prioridade );
	Unidade* retornaUnidadeDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );
	ConjuntoSala* retornaSalaDeAtendimento( int turma, Disciplina* disciplina, Campus* campus );

	
	GGroup< VariableTatInt *, LessPtr<VariableTatInt> > vars_v;
	GGroup< VariableTatInt *, LessPtr<VariableTatInt> > solVarsTatInt;

	GGroup< VariableTatico *, LessPtr<VariableTatico> > *solVarsTatico;
	GGroup< VariableTatico *, LessPtr<VariableTatico> > *vars_xh;

	// map com o nro de folgas de demanda para o campus atual e prioridade no máximo P atual.
	std::map< Disciplina*, int, LessPtr< Disciplina > > mapDiscNroFolgasDemandas;
	inline int haFolgaDeAtendimento( Disciplina *disciplina ) { return this->mapDiscNroFolgasDemandas[disciplina]; }
	bool permitirAbertura( int turma, Disciplina *disciplina, int campusId );

	void atualizarDemandasEquiv( int campusId, int prioridade );
};


#endif