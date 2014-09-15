#ifndef _MOVE_SWAP_H_
#define _MOVE_SWAP_H_

#include "Move.hpp"

#include "Aula.h"
#include "Professor.h"

class MoveSwap
   : public Move
{
public:

   using Move::apply; // prevents name hiding
   MoveSwap( Aula &, Aula & );
   virtual ~MoveSwap();

   Move & apply( SolucaoOperacional & );
   bool operator == ( const Move & ) const;
   void print();

protected:
   Aula & a1;
   Professor * profA1;
   Aula & a2;
   Professor * profA2;
};

#endif
