#ifndef NS_HPP_
#define NS_HPP_

#include "Move.hpp"
#include "SolucaoOperacional.h"

class NS
{
public:

   NS() : MAX_ATTEMPTS(250)
   {
   }

   virtual ~NS()
   {
   }

   virtual Move & move(SolucaoOperacional& s) = 0;

   virtual void print() = 0;
   
protected:

   int MAX_ATTEMPTS;
};

#endif /*NS_HPP_*/
