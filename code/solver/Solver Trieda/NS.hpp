#ifndef NS_HPP_
#define NS_HPP_

#include "Move.hpp"
#include "SolucaoOperacional.h"

class NS
{
public:

   NS()
   {
   }

   virtual ~NS()
   {
   }

   virtual Move & move(const SolucaoOperacional& s) = 0;

   virtual void print() = 0;
};

#endif /*NS_HPP_*/
