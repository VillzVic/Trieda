#ifndef _SAVE_SOLUCAO_H_
#define _SAVE_SOLUCAO_H_

#include<unordered_map>
#include<unordered_set>

class TurmaHeur;

using namespace std;

class SaveSolucao
{
public:
	SaveSolucao(void);
	~SaveSolucao(void);

	unordered_map<TurmaHeur*, pair<unordered_set<int>, int>> alocacoes;
	unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> turmasAssoc;

	void getTurmasAssoc(TurmaHeur* const turma, unordered_set<TurmaHeur*> &assoc) const;
};

#endif
