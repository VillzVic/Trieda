#ifndef ILSLPerturbation_HPP_
#define ILSLPerturbation_HPP_

#include <math.h>
#include <vector>

#include "NS.hpp"
#include "SolucaoOperacional.h"

class ILSLPerturbation
{
public:
   virtual void perturb( SolucaoOperacional &, double, double, int ) = 0;
};

class ILSLPerturbationLPlus2
   : public ILSLPerturbation
{
public:
	ILSLPerturbationLPlus2( Avaliador &, int, NS & );
   virtual ~ILSLPerturbationLPlus2();

	void add_ns( NS & );
   void perturb( SolucaoOperacional &, double, double, int );

private:
	std::vector< NS * > ns;
   Avaliador & evaluator;
	int pMax;
};

#endif
