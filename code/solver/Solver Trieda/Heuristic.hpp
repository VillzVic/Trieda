#ifndef HEURISTIC_HPP_
#define HEURISTIC_HPP_

#include "SolucaoOperacional.h"
#include "Avaliador.h"

using namespace std;

class Heuristic
{
public:

   Heuristic()
   {}

   virtual ~Heuristic()
   {}

   virtual void exec(SolucaoOperacional & s, double timelimit, double target_f) = 0;

};

#endif /* HEURISTIC_HPP_ */
