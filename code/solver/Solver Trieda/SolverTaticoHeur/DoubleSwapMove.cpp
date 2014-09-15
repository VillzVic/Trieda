#include "DoubleSwapMove.h"

#include "GUtil.h"

std::string DoubleSwapMove::toString()
{
   std::string out = "Move { Type: DOUBLE_SWAP; Move1: " + move1->toString() + "; Move2: " + move2->toString() + "; }\n";
   return out;
}