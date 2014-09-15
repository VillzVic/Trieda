#include "ConjUnidades.h"
#include "Campus.h"

ConjUnidades::ConjUnidades(Campus* const camp, unordered_set<int> const &unids)
	: campus(camp), unidades(unids)
{
}

ConjUnidades::~ConjUnidades(void)
{
}

bool ConjUnidades::temUnidade(int id) const
{
	for(auto it = unidades.begin(); it != unidades.end(); ++it)
	{
		if(*it == id)
			return true;
	}
	return false;
}
