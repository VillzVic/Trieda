#ifndef _SOLVER_MIP_UNICO_H_
#define _SOLVER_MIP_UNICO_H_

#include "Solver.h"
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "VariableMIPUnico.h"

class ProblemDataLoader;
class ProblemSolution;
class ProblemData;
class Aluno;
class AlunoDemanda;
class Campus;
class Disciplina;
class Professor;
class HorarioAula;
class Demanda;
class Sala;
class Unidade;
class AtendimentoCampus;
class AtendimentoSala;
class AtendimentoDiaSemana;
class DateTime;

class SolverMIPUnico : public Solver
{
public:
	
   SolverMIPUnico( ProblemData *, ProblemSolution *, ProblemDataLoader * );
   virtual ~SolverMIPUnico();
   
   void getSolution( ProblemSolution * ){};

   int solve();
   int solveOpSemTatico();
   int solveEscolaTat();
   void solveCampusP1Escola();
   void solveCampusP2Escola();

   void preencheAtendTaticoProbSol();
   bool preencheAtendTaticoProbData();
   bool clearAtendTaticoProbSol();
   void relacionaAlunosDemandas();
   void mudaCjtSalaParaSala();
   void getSolutionTaticoPorAlunoComHorario();
   void getSolutionFinal();

   void writeOutputTatico();
   void writeOutputOp_();

   void getAlDemsAlocados(Campus* const cp, Disciplina* const d, 
			int const turma, unordered_set<AlunoDemanda*> &alDems) const;

private:
   
   void extractSolution_();
   void extractSolutionToMaps_();
   void extractSolutionFromMapY_();
   void extractSolutionFromMapX_();
   void extractSolutionFromMapS_();

   void individualizaProfsVirtuais_();   
   void agrupaTurmaProfVirtPorAluno_(
	   unordered_map<Aluno*, unordered_map<Campus*, unordered_map<Disciplina*, int>>> &alunoTurmasParaIndiv);      
   void individualProfVirtPorAluno_(
	   unordered_map<Aluno*, unordered_map<Campus*, unordered_map<Disciplina*, int>>> const &alunoTurmasParaIndiv);

   Professor* criaProfessorVirtual();
   bool cadastraTurmaMapPV(Campus* const cp, Disciplina* const d, int const turma);
   bool associaProfATurma(Campus* const cp, Disciplina* const d, int const turma, Professor* const p);
   bool associaProfATurmaMapPV(Campus* const cp, Disciplina* const d, int const turma, Professor* const p);
   bool associaProfATurmaMapAulas(Campus* const cp, Disciplina* const d, int const turma, Professor* const p);
   bool existeProfVirt(Campus* const cp, Disciplina* const d, int const turma) const;

   void contabilizaGapProfReal_();
   void mapSolutionProfReal_();
   void countGapProfReal_();

   void getAulas(Campus* const cp, Disciplina* const d, int const turma,
		unordered_map<Sala*, unordered_map<int, unordered_set<HorarioAula*> >> * &ptMapSala);

   void criarOutputFinal_(ProblemSolution* const solution) const;
   // criar output para a turma
   void criarTurmaOutput_(AtendimentoCampus &atendCampus, Disciplina* const disciplina, int turmaId,
	   unordered_map<Professor*, unordered_map<Sala*, unordered_map<int,
	   unordered_set<HorarioAula*>>> > const &mapTurma,
	   unordered_map<Demanda*, unordered_set<AlunoDemanda*>> const &mapDemAlDems) const;
   // criar output para a aula   
   void criarAulasOutput_(AtendimentoSala &atendSala, Disciplina* const disciplina, int turmaId, Professor* const professor, 
		int dia, unordered_set<HorarioAula*> const &mapHorAlDems,
		unordered_map<Demanda*, unordered_set<AlunoDemanda*>> const &mapDemAlDems) const;
   // criar output para a aula   
   void criarAulaPorOfertaOutput_(AtendimentoDiaSemana &atendDia, Disciplina* const disciplina, int turmaId,
	   Professor* const professor, HorarioAula* const h, int dia, 
	   unordered_map<Demanda*, unordered_set<AlunoDemanda*>> const &mapDemAlDems) const;
   // criar professores virtuais para problem solution
   void criarOutProfsVirtuais_(ProblemSolution* const solution) const;

	void clear();
	
    // The linear problem   
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp;
	#endif
	   
	bool CARREGA_SOLUCAO;
   
   ProblemSolution * problemSolution;
   ProblemDataLoader * problemDataLoader;
   
   // usado para armazenar a solução tatica da iteração cjtAluno anterior, a fim de fazer a fixação de valores      
   
   std::set<VariableMIPUnico*, LessPtr<VariableMIPUnico>> solMipUnico_;
   std::set<VariableMIPUnico*, LessPtr<VariableMIPUnico>> solMipUnicoX_;
   std::set<VariableMIPUnico*, LessPtr<VariableMIPUnico>> solMipUnicoY_;
   std::set<VariableMIPUnico*, LessPtr<VariableMIPUnico>> solMipUnicoS_;

   unordered_map<Campus*, unordered_map<Disciplina*, unordered_map<int, unordered_map<Professor*,
	   unordered_map<Sala*, unordered_map<int, unordered_set<HorarioAula*>>> >>>> solAulas_;

   unordered_map<Campus*, unordered_map<Disciplina*, unordered_map<int, Professor*>>> solTurmasComPV_;

   unordered_map<Campus*, unordered_map<Disciplina*, unordered_map<int, unordered_set<AlunoDemanda*> >>> solTurmaAlunosAloc_;
   
   // ------------
   // Typedefs
   typedef unordered_map<Disciplina*, unordered_map<int, unordered_map<Professor*,
		unordered_map<Sala*, unordered_map<int, unordered_set<HorarioAula*>>> >>> MapDiscTurmProfSalaDiaHors;
   
   typedef unordered_map<int, unordered_map<Professor*,
		unordered_map<Sala*, unordered_map<int, unordered_set<HorarioAula*>>> >> MapTurmProfSalaDiaHors;
   
   typedef unordered_map<Professor*,
		unordered_map<Sala*, unordered_map<int, unordered_set<HorarioAula*>>> > MapProfSalaDiaHors;
   
   typedef unordered_map<Sala*, unordered_map<int, unordered_set<HorarioAula*>>> MapSalaDiaHors;
   
   typedef unordered_map<int, unordered_set<HorarioAula*>> MapDiaHors;

   // ------------ 
   typedef unordered_map<Professor*, map<int, map<DateTime, pair<DateTime,Unidade*> >>> MapProfDiaDtiDtfUnid;

   MapProfDiaDtiDtfUnid solProfRealAloc_;

   // ------------ 

   struct Ordena
   {
      bool operator() ( std::vector< int > xI, std::vector< int > xJ )
      {
         return ( xI.front() > xJ.front() );
      }
   } ordenaPorCreditos;


   int campusAtualId;
   
   std::map<int, std::vector<string> > alDemNaoAtend_output;  
   
};

#endif
