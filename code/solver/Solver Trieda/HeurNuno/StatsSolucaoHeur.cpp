#include "StatsSolucaoHeur.h"
#include "DadosHeuristica.h"
#include "HeuristicaNuno.h"


StatsSolucaoHeur::StatsSolucaoHeur(void)
	: nrDemandasPrioridade_(DadosHeuristica::nrDemandasPrioridade())
{
	reset();
}

StatsSolucaoHeur::~StatsSolucaoHeur(void)
{
}

void StatsSolucaoHeur::reset(void)
{
	nrDisc_ = 0;
	nrDiscDuasComp_ = 0;
	nrNaoPodeAbrir_ = 0;
	nrTurmasAbertas_ = 0;
	nrTurmasAbertasCompSec_ = 0;
	nrTurmasProfReal_ = 0;
	numDemandasAtend_.clear();
	nrCreditosProfessores_ = 0;
	nrCreditosProfsVirtuais_ = 0;
	nrCreditosAlunos_ = 0;
	nrTurmasInvalidas_ = 0;
	nrAlunosIncompletos_ = 0;
}

// nr demandas prioridade
int StatsSolucaoHeur::nrDemsPrior (int prior, bool equiv) const
{
	int nr = 0;
	auto it = nrDemandasPrioridade_.find(prior);
	if(it != nrDemandasPrioridade_.end())
		nr += it->second;

	if(equiv)
	{
		auto itEq = nrDemandasPrioridade_.find(-prior);
		if(itEq != nrDemandasPrioridade_.end())
			nr += itEq->second;
	}

	return nr;
}
// nr demandas atendidas prioridade
int StatsSolucaoHeur::nrDemsAtend (int prior, bool equiv) const
{
	int nr = 0;
	auto it = numDemandasAtend_.find(prior);
	if(it != numDemandasAtend_.end())
		nr += it->second;

	if(equiv)
	{
		auto itEq = numDemandasAtend_.find(-prior);
		if(itEq != numDemandasAtend_.end())
			nr += itEq->second;
	}

	return nr;
}

// creditos aluno / creditos prof
double StatsSolucaoHeur::getRatioCreds(void) const
{
	if(nrCreditosProfessores_ == 0)
		return 0;

	return (double(nrCreditosAlunos_) / nrCreditosProfessores_);
}

// creditos prof / creditos aluno
double StatsSolucaoHeur::getCustoCreds(void) const
{
	if(nrCreditosAlunos_ == 0)
		return 0;

	return (double(nrCreditosProfessores_) / nrCreditosAlunos_);
}

// add demanda de prioridade
void StatsSolucaoHeur::addDemandaAtendPrior(int prioridade)
{
	auto itPrior = numDemandasAtend_.find(prioridade);
	if(itPrior == numDemandasAtend_.end())
		itPrior = numDemandasAtend_.insert(make_pair<int, int>(prioridade, 0)).first;

	itPrior->second++;
}
// remove demanda de prioridade
void StatsSolucaoHeur::removeDemandaAtendPrior(int prioridade)
{
	auto itPrior = numDemandasAtend_.find(prioridade);
	if(itPrior == numDemandasAtend_.end())
		HeuristicaNuno::excepcao("StatsSolucaoHeur::removeDemandaAtendPrior", "prioridade nao encontrada");

	int nr = itPrior->second;
	itPrior->second = (nr - 1);
	if(itPrior->second < 0)
		HeuristicaNuno::excepcao("StatsSolucaoHeur::removeDemandaAtendPrior", "demandas atendidas menor que zero");
}