#pragma once

#include "SwapMove.h"

class DoubleSwapMove : Move
{
public:
   DoubleSwapMove(void) { type = DOUBLE_SWAP; move1 = NULL; move2 = NULL; }
   ~DoubleSwapMove(void) { if (move1 != NULL) delete move1; if (move2 != NULL) delete move2; }

   inline SwapMove* getMove1() { return move1; }
   inline SwapMove* getMove2() { return move2; }

   inline void setMove1(SwapMove* value) { this->move1 = value; }
   inline void setMove2(SwapMove* value) { this->move2 = value; }
   
   std::string toString();

private:
   SwapMove* move1;
   SwapMove* move2;
};

