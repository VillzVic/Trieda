#include "RecursivePreProc.h"


RecursivePreProc::RecursivePreProc(vector<pair<int,int>>::const_iterator const &it, 
							unordered_set<AlunoHeur*> const &alunosDsp, unordered_set<SalaHeur*> const &salasDsp,
							unordered_map<int, AulaHeur*> const &aul)
			: itDia(it), alunosDisp(new unordered_set<AlunoHeur*>(alunosDsp)), salasDisp(new unordered_set<SalaHeur*>(salasDsp)), 
			  aulas(new unordered_map<int, AulaHeur*>(aul))
{
}


RecursivePreProc::~RecursivePreProc(void)
{
	delete alunosDisp;
	delete salasDisp;
	delete aulas;
}
