#ifndef _HEURISTIC_HPP_
#define _HEURISTIC_HPP_

#include "SolucaoOperacional.h"
#include "Avaliador.h"

class Heuristic
{
public:

   Heuristic() { }

   virtual ~Heuristic() { }

   virtual void exec( SolucaoOperacional &, double, double ) = 0;

};

#endif // _HEURISTIC_HPP_
