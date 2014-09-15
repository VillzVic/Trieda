#include "AlocarEquiv.h"
#include "TurmaHeur.h"
#include "OfertaDisciplina.h"
#include "AbridorTurmas.h"
#include "ParametrosHeuristica.h"
#include "SolucaoHeur.h"
#include "ImproveMethods.h"


AlocarEquiv::AlocarEquiv(SolucaoHeur* const solucao, bool mudarSalas, bool priorUm)
	: solucao_(solucao), mudarSalas_(mudarSalas), mudarSalasFlag_(false), priorUm_(priorUm)
{
}


AlocarEquiv::~AlocarEquiv(void)
{
}

// [NOVA VERSÃO] tentar alocar alunos com demanda equivalente a turmas já abertas
void AlocarEquiv::tryAlocEquiv()
{
	// olhar para todos e tentar alocar P1 mudando a sala
	if(priorUm_)
	{
		set<OfertaDisciplina*, compOftDisc> setOrdP1;
		solucao_->getOfertasDisciplinaOrd_<compOftDisc>(setOrdP1);
		for(auto it = setOrdP1.begin(); it != setOrdP1.end(); ++it)
		{
			ImproveMethods::tryAlocNaoAtendidos(solucao_, *it, mudarSalas_, 1);
		}
	}

	// agora tentar equivalentes mudando também a sala
	set<OfertaDisciplina*, compOftDiscEquiv> setOrdEquiv;
	solucao_->getOfertasDisciplinaOrd_<compOftDiscEquiv>(setOrdEquiv);
	for(auto it = setOrdEquiv.begin(); it != setOrdEquiv.end(); ++it)
	{
		ImproveMethods::tryAlocNaoAtendidos(solucao_, *it, mudarSalas_, -1);
	}
}