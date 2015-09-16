#ifndef _MIP_ALOCAR_PROFS_H_
#define _MIP_ALOCAR_PROFS_H_

#include <unordered_map>
#include <unordered_set>
#include "MIPAloc.h"
#include "ParametrosHeuristica.h"

class TurmaHeur;
class Curso;
class Disciplina;
class TipoContrato;
class TipoTitulacao;

class MIPAlocarProfs : MIPAloc
{
	friend class SolucaoHeur;
public:
	MIPAlocarProfs(std::string nome, SolucaoHeur* const solucao, bool profsIndiv);
	~MIPAlocarProfs(void);

	// alocar professores
	void alocar(void);

private:

	// professores virtuais individualizados
	bool profsVirtuaisIndiv_;
	unordered_map<TurmaHeur*, ProfessorHeur*> turmasProfs_;

	#pragma region [MAIN METHODS]

	// Preparar dados
	void prepararDados(void);
	void getTurmasCurso(void);

	// Carregar solução
	void carregarSolucao(void);

	#pragma endregion


	#pragma region [CRIAR VARIÁVEIS]

	// turmas que têm pelo menos um aluno de um curso
	unordered_map<Curso*, unordered_set<TurmaHeur*>> turmasPorCurso;

	// criar variáveis do modelo
	void criarVariaveis_(void);

	// vars binarias que indicam quando um professor é usado
	unordered_map<int, int> varsProfUsed;

	// vars binárias que associa prof à turma (varsProfTurma e varsTurmaProf são estruturas inversas que guardam os idxs das mesmas vars)
	unordered_map<ProfessorHeur*, unordered_map<TurmaHeur*, int>> varsProfTurma; // prof -> (turma, col. nr)		(sem virtual)
	unordered_map<TurmaHeur*, unordered_map<ProfessorHeur*, int>> varsTurmaProf; // turma -> (prof, col. nr)

	// vars binárias que registam que dias um professor é alocado
	unordered_map<int, unordered_map<int, int>> varsProfDia;				// prof -> (dia, col. nr)
	// vars binárias que indicam quando a restrição de mínimo de créditos semanal é violada
	unordered_map<int, int> varsProfMinChSem;								// prof -> col. nr
	// vars binarias que indicam quando o professor é alocado à mesma cara que o semestre anterior
	unordered_map<int, int> varsProfChAnterior;
	// vars inteiras que indicam o primeiro hip e o último hfp horário da fase do dia (M/T/N) usados pelo professor
	unordered_map<ProfessorHeur*, unordered_map<int, unordered_map<int, pair<int,int>>>> varsProfDiaFaseHiHf;	// Prof -> Dia -> FaseDoDia -> (col. nr. hip/ col. nr. hfp)

	// stats
	int nrVarsProfTurmaVirtualIndiv_;

	// criar variáveis que alocam um professor a uma turma
	void criarVariaveisProfTurma_(void);
	// criar variáveis para turma
	void criarVarsProfTurma_(TurmaHeur* const turma, unordered_set<ProfessorHeur*> &profsAssoc);
	// criar variáveis que identificam a violação do mínimo de carga horária diária e semanal e variaveis prof dia, e o uso do professor
	// obs : só chamar criarVarsMinCh depois de criarVariaveisProfTurma_
	void criarVarsMinChProfDia_(void);
	// criar variáveis que setam o primeiro e o último horário da fase do dia (M/T/N) usados por professor
	void criarVarsHiHfProfFaseDoDia_(void);

	// get coeficiente var prof turma
	double getCoefObjVarProfTurma(ProfessorHeur* const professor);
	// get var prof dia
	int getVarProfDia(ProfessorHeur* const professor, int dia);

	#pragma endregion

	#pragma region [CRIAR RESTRIÇÕES]

	// criar restrições do modelo
	void criarRestricoes_(void);

	// criar restrições de ativação do professor virtual
	void criarRestAtivarProf_(ProfessorHeur* const professor);
	// criar restrições de assignment de professor à turma
	void criarRestProfAssignment_(TurmaHeur* const turma, unordered_map<ProfessorHeur*, int> &varsProf);
	// criar restrições incompatibilidade de turmas
	void criarRestProfIncompTurmas_(unordered_map<TurmaHeur*, int> &varsProf);
	// criar restrições que activam as variáveis prof dia
	void criarRestProfAtivProfDia_(ProfessorHeur* const professor, int dia, unordered_map<TurmaHeur*, int> &varsProf);
	// criar restrição máximo de dias por semana
	void criarRestProfMaxDiasSem_(ProfessorHeur* const professor);
	// criar restrições que detectam violação de minimo e o máximo de créditos semanais [FORTE ou FRACA(min)]
	void criarRestProfLimitesChSem_(ProfessorHeur* const professor, unordered_map<TurmaHeur*, int> &varsProf);
	// criar restrições mínimo ch por dia [FORTE]
	void criarRestProfMinChDia_(ProfessorHeur* const professor, int dia, unordered_map<TurmaHeur*, int> &varsProf);

	// criar restrições de uso sequencial de professores virtuais com mesmo perfil e do mesmo curso.
	void criarRestsSeqProfsVirtuais_(void);

	// criar restricoes que determinam o minimo de alunos por titulacao
	void criarRestMinMestres_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas);
	void criarRestMinDoutores_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas);
	void criarRestMinTitulacao_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas, double perc, int titulacao);
	// criar restricoes que determinam o minimo de alunos por tipo contrato
	void criarRestMinIntegral_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas);
	void criarRestMinParcial_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas);
	void criarRestMinTipoContrato_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas, double perc, int tipoContrato);

	// criar restrições que impedem gaps nos horários do professor em uma mesma fase de um dia
	void criarRestProfHiHf_();
	void criarRestProfHiUB_(ProfessorHeur* const prof, const int dia, const int fase, 
		const int colHi, map<DateTime, set< pair<int,double> >> const & mapDtiVars);
	void criarRestProfHiLB_(ProfessorHeur* const prof, const int dia, const int fase,
		const int colHi, map<DateTime, set< pair<int,double> >> const & mapDtiVars);
	void criarRestProfHfLB_(ProfessorHeur* const prof, const int dia, const int fase, 
		const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars);
	void criarRestProfHfUB_(ProfessorHeur* const prof, const int dia, const int fase, 
		const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars);
	void criarRestProfGapMTN_(ProfessorHeur* const prof, const int dia, const int fase, 
		int rhs, const int colHi, const int colHf, set< pair<int,double> > const &varsColCoef);

	#pragma endregion

	// pega na solução herdada da heurística e carrega-a para o MIP
	void definirMIPStart_(void);
	// MIP start com todas as turmas com professor virtual único
	void definirMIPStartAllVirtual_(void);

	// verificar a % de profs reais que teve a chAnterior * perc% satisfeita
	void checkChAnterior_(void);

	// verificar se a proibição de gaps no horários de mesma fase do dia do prof foi respeitada
	void checkGapsProf_(void);
};

#endif

