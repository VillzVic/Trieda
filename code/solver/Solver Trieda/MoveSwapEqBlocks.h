#ifndef MOVESWAPEQBLOCKS_H
#define MOVESWAPEQBLOCKS_H

#include "Move.hpp"

#include "Aula.h"
#include "Professor.h"

class MoveSwapEqBlocks : public Move
{
protected:
   Aula & a1;
   Professor & profA1;

   Aula & a2;
   Professor & profA2;

public:
   MoveSwapEqBlocks(Aula & a1, Professor & profA1, Aula & a2, Professor & profA2);

   MoveSwapEqBlocks::MoveSwapEqBlocks(Aula & _a1, Aula & _a2);

   virtual ~MoveSwapEqBlocks();

   Move & apply(SolucaoOperacional & s);

   bool operator==(const Move & m) const;

   void print();
};

#endif /*MOVESWAPEQBLOCKS_H*/