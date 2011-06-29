#ifndef _ILSL_HPP_
#define _ILSL_HPP_

#include <iostream>

#include "Heuristic.hpp"
#include "Avaliador.h"

#include "ILSLPerturbation.h"

#ifdef WIN32
#include "CPUTimerWin.h"
#else
#include "CPUTimerUnix.h"
#endif

typedef std::pair< std::pair< int, int > , std::pair< int, int > > levelHistory;

// TRIEDA-923
class IteratedLocalSearchLevels
   : public Heuristic
{
public:
   IteratedLocalSearchLevels( Avaliador &, Heuristic &, ILSLPerturbation &, int, int );
   virtual ~IteratedLocalSearchLevels();

   virtual levelHistory & initializeHistory();
   virtual void localSearch( SolucaoOperacional &, double, double );
   virtual void perturbation( SolucaoOperacional &, double, double, levelHistory & );
   virtual SolucaoOperacional & acceptanceCriterion( SolucaoOperacional &, SolucaoOperacional &, levelHistory & );
   virtual bool terminationCondition( levelHistory & );

   void exec( SolucaoOperacional &, double, double );

protected:
   Avaliador & e;
   Heuristic & h;
   ILSLPerturbation & p;
   int iterMax;
   int levelMax;
};

#endif
