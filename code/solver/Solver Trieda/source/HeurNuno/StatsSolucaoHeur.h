#ifndef _STATS_SOLUCAO_HEUR_H_
#define _STATS_SOLUCAO_HEUR_H_

#include "UtilHeur.h"

class SolucaoHeur;
class DadosHeuristica;
class AbridorTurmas;

class StatsSolucaoHeur
{
	friend class SolucaoHeur;
	friend class AbridorTurmas;
	friend class MIPAlocarAlunos;
	friend class MIPAlocarProfs;
	friend class OfertaDisciplina;
	friend class TurmaHeur;
public:
	StatsSolucaoHeur(void);
	~StatsSolucaoHeur(void);

	// nr demandas prioridade
	int nrDemsPrior (int prior, bool equiv = true) const;
	// nr demandas atendidas prioridade
	int nrDemsAtend (int prior, bool equiv = true) const;

	// creditos aluno / creditos prof
	double getRatioCreds(void) const;
	// creditos prof / creditos aluno
	double getCustoCreds(void) const;

private:

	// [DADOS]
	// Nr de ofertas disciplina
	int nrDisc_;
	// Nr ofertas disciplinas com componente prática e teórica
	int nrDiscDuasComp_;
	// Nr ofertas não pode abrir por n ter salas associadas.
	int nrNaoPodeAbrir_;
	// Total demandas
	const unordered_map<int, int> nrDemandasPrioridade_;

	// [SOLUÇÃO]
	// número de turmas abertas
	int nrTurmasAbertas_;
	// componente secundária, i.e. turmas prática se a disciplina tbm tiver parte teórica
	int nrTurmasAbertasCompSec_;
	// número de turmas com prof real
	int nrTurmasProfReal_;
	// número de demandas atendidas por prioridade
	unordered_map<int, int> numDemandasAtend_;
	// créditos alocados professores
	int nrCreditosProfessores_;
	// créditos alocados a professores virtuais
	int nrCreditosProfsVirtuais_;
	// créditos alunos
	int nrCreditosAlunos_;

	// [POLISH]
	int nrTurmasInvalidas_;
	// nr de alunos incompletos removidos de ofertas disciplina
	int nrAlunosIncompletos_;

	// reset estrutura
	void reset();

	// add demanda de prioridade
	void addDemandaAtendPrior(int prioridade);
	// remove demanda de prioridade
	void removeDemandaAtendPrior(int prioridade);
};

#endif