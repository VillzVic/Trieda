#pragma once

#include "ShiftMove.h"

class DoubleShiftMove : Move
{
public:
   DoubleShiftMove(void) { type = DOUBLE_SHIFT; move1 = NULL; move2 = NULL; }
   ~DoubleShiftMove(void) { if (move1 != NULL) delete move1; if (move2 != NULL) delete move2; }

   inline ShiftMove* getMove1() { return move1; }
   inline ShiftMove* getMove2() { return move2; }

   inline void setMove1(ShiftMove* value) { this->move1 = value; }
   inline void setMove2(ShiftMove* value) { this->move2 = value; }
   
   std::string toString();

private:
   ShiftMove* move1;
   ShiftMove* move2;
};

