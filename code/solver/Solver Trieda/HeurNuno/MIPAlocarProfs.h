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

	// Carregar solu��o
	void carregarSolucao(void);

	#pragma endregion


	#pragma region [CRIAR VARI�VEIS]

	// turmas que t�m pelo menos um aluno de um curso
	unordered_map<Curso*, unordered_set<TurmaHeur*>> turmasPorCurso;

	// criar vari�veis do modelo
	void criarVariaveis_(void);

	// vars binarias que indicam quando um professor � usado
	unordered_map<int, int> varsProfUsed;

	// vars bin�rias que associa prof � turma (varsProfTurma e varsTurmaProf s�o estruturas inversas que guardam os idxs das mesmas vars)
	unordered_map<ProfessorHeur*, unordered_map<TurmaHeur*, int>> varsProfTurma; // prof -> (turma, col. nr)		(sem virtual)
	unordered_map<TurmaHeur*, unordered_map<ProfessorHeur*, int>> varsTurmaProf; // turma -> (prof, col. nr)

	// vars bin�rias que registam que dias um professor � alocado
	unordered_map<int, unordered_map<int, int>> varsProfDia;				// prof -> (dia, col. nr)
	// vars bin�rias que indicam quando a restri��o de m�nimo de cr�ditos semanal � violada
	unordered_map<int, int> varsProfMinChSem;								// prof -> col. nr
	// vars binarias que indicam quando o professor � alocado � mesma cara que o semestre anterior
	unordered_map<int, int> varsProfChAnterior;
	// vars inteiras que indicam o primeiro hip e o �ltimo hfp hor�rio da fase do dia (M/T/N) usados pelo professor
	unordered_map<ProfessorHeur*, unordered_map<int, unordered_map<int, pair<int,int>>>> varsProfDiaFaseHiHf;	// Prof -> Dia -> FaseDoDia -> (col. nr. hip/ col. nr. hfp)

	// stats
	int nrVarsProfTurmaVirtualIndiv_;

	// criar vari�veis que alocam um professor a uma turma
	void criarVariaveisProfTurma_(void);
	// criar vari�veis para turma
	void criarVarsProfTurma_(TurmaHeur* const turma, unordered_set<ProfessorHeur*> &profsAssoc);
	// criar vari�veis que identificam a viola��o do m�nimo de carga hor�ria di�ria e semanal e variaveis prof dia, e o uso do professor
	// obs : s� chamar criarVarsMinCh depois de criarVariaveisProfTurma_
	void criarVarsMinChProfDia_(void);
	// criar vari�veis que setam o primeiro e o �ltimo hor�rio da fase do dia (M/T/N) usados por professor
	void criarVarsHiHfProfFaseDoDia_(void);

	// get coeficiente var prof turma
	double getCoefObjVarProfTurma(ProfessorHeur* const professor);
	// get var prof dia
	int getVarProfDia(ProfessorHeur* const professor, int dia);

	#pragma endregion

	#pragma region [CRIAR RESTRI��ES]

	// criar restri��es do modelo
	void criarRestricoes_(void);

	// criar restri��es de ativa��o do professor virtual
	void criarRestAtivarProf_(ProfessorHeur* const professor);
	// criar restri��es de assignment de professor � turma
	void criarRestProfAssignment_(TurmaHeur* const turma, unordered_map<ProfessorHeur*, int> &varsProf);
	// criar restri��es incompatibilidade de turmas
	void criarRestProfIncompTurmas_(unordered_map<TurmaHeur*, int> &varsProf);
	// criar restri��es que activam as vari�veis prof dia
	void criarRestProfAtivProfDia_(ProfessorHeur* const professor, int dia, unordered_map<TurmaHeur*, int> &varsProf);
	// criar restri��o m�ximo de dias por semana
	void criarRestProfMaxDiasSem_(ProfessorHeur* const professor);
	// criar restri��es que detectam viola��o de minimo e o m�ximo de cr�ditos semanais [FORTE ou FRACA(min)]
	void criarRestProfLimitesChSem_(ProfessorHeur* const professor, unordered_map<TurmaHeur*, int> &varsProf);
	// criar restri��es m�nimo ch por dia [FORTE]
	void criarRestProfMinChDia_(ProfessorHeur* const professor, int dia, unordered_map<TurmaHeur*, int> &varsProf);

	// criar restri��es de uso sequencial de professores virtuais com mesmo perfil e do mesmo curso.
	void criarRestsSeqProfsVirtuais_(void);

	// criar restricoes que determinam o minimo de alunos por titulacao
	void criarRestMinMestres_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas);
	void criarRestMinDoutores_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas);
	void criarRestMinTitulacao_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas, double perc, int titulacao);
	// criar restricoes que determinam o minimo de alunos por tipo contrato
	void criarRestMinIntegral_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas);
	void criarRestMinParcial_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas);
	void criarRestMinTipoContrato_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas, double perc, int tipoContrato);

	// criar restri��es que impedem gaps nos hor�rios do professor em uma mesma fase de um dia
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

	// pega na solu��o herdada da heur�stica e carrega-a para o MIP
	void definirMIPStart_(void);
	// MIP start com todas as turmas com professor virtual �nico
	void definirMIPStartAllVirtual_(void);

	// verificar a % de profs reais que teve a chAnterior * perc% satisfeita
	void checkChAnterior_(void);

	// verificar se a proibi��o de gaps no hor�rios de mesma fase do dia do prof foi respeitada
	void checkGapsProf_(void);
};

#endif

