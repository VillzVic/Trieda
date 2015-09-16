#include "SaveSolucao.h"
#include "TurmaHeur.h"


SaveSolucao::SaveSolucao(void)
{
}


SaveSolucao::~SaveSolucao(void)
{
}

void SaveSolucao::getTurmasAssoc(TurmaHeur* const turma, unordered_set<TurmaHeur*> &assoc) const
{
	assoc.clear();
	auto it = turmasAssoc.find(turma);
	if(it != turmasAssoc.end())
		assoc = it->second;
}
