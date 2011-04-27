#ifndef ILSLPerturbation_HPP_
#define ILSLPerturbation_HPP_

#include <math.h>
#include <vector>

#include "NS.hpp"

#include "SolucaoOperacional.h"

using namespace std;

class ILSLPerturbation
{
public:
   virtual void perturb(SolucaoOperacional & s, double timelimit, double target_f, int level) = 0;
};

class ILSLPerturbationLPlus2: public ILSLPerturbation
{
private:
	vector<NS*> ns;
   Avaliador & evaluator;
	int pMax;

public:
	ILSLPerturbationLPlus2(Avaliador & e, int _pMax, NS& _ns);

   virtual ~ILSLPerturbationLPlus2();

	void add_ns(NS & _ns);

   void perturb(SolucaoOperacional & s, double timelimit, double target_f, int level);
};

#endif /*ILSLPerturbation_HPP_*/
