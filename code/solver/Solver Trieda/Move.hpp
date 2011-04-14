#ifndef MOVE_HPP_
#define MOVE_HPP_

#include "SolucaoOperacional.h"
#include "Avaliador.h"

using namespace std;

class Move
{
public:

   Move()
   {
   }

	virtual ~Move()
	{
	}

	virtual Move & apply(SolucaoOperacional & s) = 0;

	virtual bool operator==(const Move & m) const = 0;

	bool operator!=(const Move & m) const
	{
		return !(*this == m);
	}

	virtual void print() = 0;
};

#endif /*MOVE_HPP_*/
