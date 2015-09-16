#ifndef _RECURSIVE_PRE_PROC_
#define _RECURSIVE_PRE_PROC_

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "AlunoHeur.h"
#include "SalaHeur.h"
#include "AulaHeur.h"

using namespace std;

class RecursivePreProc
{
public:
	RecursivePreProc(vector<pair<int,int>>::const_iterator const &it, 
							unordered_set<AlunoHeur*> const &alunosDsp, unordered_set<SalaHeur*> const &salasDsp,
							unordered_map<int, AulaHeur*> const &aul);

	~RecursivePreProc(void);

	vector<pair<int,int>>::const_iterator const itDia;
	const unordered_set<AlunoHeur*>* const alunosDisp;
	const unordered_set<SalaHeur*>* const salasDisp;
	const unordered_map<int, AulaHeur*>* const aulas;
};

#endif

