#ifndef _MOVE_SWAP_H_
#define _MOVE_SWAP_H_

#include "Move.hpp"

#include "Aula.h"
#include "Professor.h"

class MoveSwap : public Move
{
protected:
   Aula & a1;
   Professor & profA1;

   Aula & a2;
   Professor & profA2;

public:
   MoveSwap(Aula & a1, Professor & profA1, Aula & a2, Professor & profA2);

   MoveSwap(Aula & _a1, Aula & _a2);

   virtual ~MoveSwap();

   //bool canBeApplied(const SolucaoOperacional & s);

   Move & apply(SolucaoOperacional & s);

   bool operator==(const Move & m) const;

   void print();
};

#endif /*_MOVE_SWAP_H_*/