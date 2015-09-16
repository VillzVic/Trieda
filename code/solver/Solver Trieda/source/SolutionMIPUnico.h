#ifndef _SOLUTION_MIPUNICO_H_
#define _SOLUTION_MIPUNICO_H_

#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

using std::unordered_set;
using std::unordered_map;
using std::set;

class GoalStatus;
class Disciplina;
class Campus;
class Aluno;
class AlunoDemanda;
class DateTime;
class HorarioAula;
class Professor; 
class VariableMIPUnico;

class SolutionMIPUnico
{
public:
	SolutionMIPUnico(void);
	~SolutionMIPUnico(void);

	// Data
	void setMapDiscAlunosDemanda(std::map<Disciplina*, std::set<AlunoDemanda*>> & mapDemandas);

	// Modifica a solução
	void deleteGoalStatus();
	void clearMapsSolution();
	void deleteVariablesSol();
	void addSolAlocProfTurma(Professor* const p, Campus * const cp, Disciplina * const d, int turma);
	void addSolAlocAlunoTurma(Aluno* const a, Disciplina * const d, int turma, int dia, DateTime dti);
	void addVarSol(VariableMIPUnico* const v);
	GoalStatus* getAddNewGoal(int fase);
	
	// Consultas
    bool haDemandaNaoAtendida(Disciplina* const disc);
    bool demandaTodaAtendidaPorReal(Disciplina* const disc);
    bool existeTurmaAtendida(Campus* const campus, Disciplina* const disc, int turma) const;
    bool haDemandaPossivelNoDiaHor(Disciplina* const disc, int dia, HorarioAula* const ha);
    bool permitirAlunoDiscNoHorDia(AlunoDemanda* const alDem, int dia, DateTime dti) const;
    bool permitirAlunoNaTurma(Aluno* const aluno, Disciplina* const disciplina, int turma) const;
    bool permitirTurma(Campus* const campus, Disciplina* const disciplina, int turma);
    bool alunoHorVazioNoDia(Aluno* const aluno, int dia, DateTime dti) const;
    bool alunoAlocNaTurma(Aluno* const aluno, Disciplina* const disciplina, int turma) const;
    int alunoAlocDisc(Aluno* const aluno, Disciplina* const disc) const;
    bool alunoAlocDiscNoHorDia(Aluno* const aluno, Disciplina* const disc, int dia, DateTime dti) const;
    bool alunoAlocIncompNaDisc(Aluno* const aluno, Disciplina* const disc) const;
    bool profAlocNaTurma(Professor* const prof, Campus* const campus, Disciplina* const disciplina, int turma) const;
    bool getProfAlocNaTurma(Professor* &professor, Campus* const campus, Disciplina* const disciplina, int turma) const;
	void copyFinalSolution(std::set<VariableMIPUnico*> &solMipUnico) const;

	// Imprime solução
	void imprimeTurmaProf( int campusId, int prioridade );
	void imprimeProfTurmas( int campusId, int prioridade );
	void imprimeGoals();

	void confereCorretude( int campusId, int prioridade );

private:
	
	std::map<Disciplina*, std::set<AlunoDemanda*>> *mapDiscAlunosDemanda_;
	

   /* 
		*******************************************************************************************
											SOLUTION
   */
	
	std::set<VariableMIPUnico*> solVarsTatInt_;
		
   
	unordered_map<Professor*, unordered_map< Campus*, unordered_map< Disciplina*, unordered_set<int>> >> solAlocProfTurma_;
	unordered_map<Campus*, unordered_map< Disciplina*, unordered_map<int, Professor*> >> solAlocTurmaProf_;	
	unordered_map<Aluno*, unordered_map<Disciplina*, std::pair<int, unordered_map<int, std::set<DateTime>> >>> solAlocAlunoDiscTurmaDiaDti_;
	unordered_map<Aluno*, unordered_map<int, std::set<DateTime>>> solAlocAlunoDiaDti_;
				
	std::vector<GoalStatus*> goalStats_;


};

#endif