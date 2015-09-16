#ifndef _CONJ_UNID_H_
#define _CONJ_UNID_H_

#include "HeurNuno/UtilHeur.h"


class Campus;

class ConjUnidades
{
public:
	ConjUnidades(Campus* const camp, unordered_set<int> const &unids);
	~ConjUnidades(void);

	Campus* const campus;
	const unordered_set<int> unidades;

	// pode ser id de qualquer unidade pois são equivalentes
	int getUnidadeId(void) const { return *(unidades.begin()); };

	bool temUnidade(int id) const;
};

#endif

