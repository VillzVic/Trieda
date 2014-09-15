#ifndef _RDM_HPP_
#define _RDM_HPP_

#include "Heuristic.hpp"

#ifdef WIN32
#include "CPUTimerWin.h"
#else
#include "CPUTimerUnix.h"
#endif

#include "NS.hpp"

class RandomDescentMethod: public Heuristic
{
private:
   NS & ns;
   Avaliador & evaluator;
   unsigned int iterMax;

public:

   RandomDescentMethod(Avaliador & _eval, NS & _ns, unsigned int _iterMax);

   virtual ~RandomDescentMethod();

   virtual void exec(SolucaoOperacional & s, double timelimit, double target_f);
};

#endif /*_RDM_HPP_*/
