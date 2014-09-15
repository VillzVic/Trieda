#include "DoubleShiftMove.h"

#include "GUtil.h"

std::string DoubleShiftMove::toString()
{
   std::string out = "Move { Type: DOUBLE_SHIFT; Move1: " + move1->toString() + "; Move2: " + move2->toString() + "; }\n";
   return out;
}