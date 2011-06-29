#ifndef ILSL_HPP_
#define ILSL_HPP_

#include <iostream>

#include "Heuristic.hpp"
#include "Avaliador.h"

#include "ILSLPerturbation.h"

#ifdef WIN32
#include "CPUTimerWin.h"
#else
#include "CPUTimerUnix.h"
#endif

using namespace std;

typedef pair<pair<int, int> , pair<int, int> > levelHistory;

// TRIEDA-923
class IteratedLocalSearchLevels: public Heuristic
{
protected:
   Avaliador & e;
   Heuristic & h;
   ILSLPerturbation & p;
   int iterMax, levelMax;

public:

   IteratedLocalSearchLevels(Avaliador & e, Heuristic & h, ILSLPerturbation & p, int iterMax, int levelMax);
   
   virtual ~IteratedLocalSearchLevels();

   virtual levelHistory & initializeHistory();

   virtual void localSearch(SolucaoOperacional & s, double timelimit, double target_f);

   virtual void perturbation(SolucaoOperacional & s, double timelimit, double target_f, levelHistory& history);

   virtual SolucaoOperacional & acceptanceCriterion(SolucaoOperacional & s1, SolucaoOperacional & s2, levelHistory& history);

   virtual bool terminationCondition(levelHistory& history);

   void exec(SolucaoOperacional & s, double timelimit, double target_f);
};

#endif /*ILSL_HPP_*/
