#ifndef TABUSEARCH_HPP_
#define TABUSEARCH_HPP_

#include "Heuristic.hpp"

class TabuSearch
{
public:
   TabuSearch();

   virtual ~TabuSearch();

   void exec(SolucaoOperacional & s, double timelimit, double target_f);
};

#endif /*TABUSEARCH_HPP_*/