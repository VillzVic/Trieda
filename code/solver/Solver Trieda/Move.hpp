#ifndef _MOVE_HPP_
#define _MOVE_HPP_

#include "SolucaoOperacional.h"
#include "Avaliador.h"

class Move
{
public:

   Move() { }

   virtual ~Move() { }

   //bool canBeApplied(const SolucaoOperacional & s) = 0;

   virtual Move & apply( SolucaoOperacional & ) = 0;

   virtual bool operator == ( const Move & ) const = 0;

   bool operator != ( const Move & m ) const { return !( *this == m ); }

   virtual void print() = 0;
};

#endif // _MOVE_HPP_
