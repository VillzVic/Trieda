#pragma once

#include <iostream>

#include "Solution.h"

enum MoveType
{
   SHIFT = 0,
   SWAP,
   DOUBLE_SHIFT,
   DOUBLE_SWAP,
   SWAP_DIVISION,
   UNKNOWN,
};

class Move
{
public:
   inline MoveType getType() { return type; }
   inline ObjectiveFunction getObjectiveFunction() { return objectiveFunction; }
   inline void setObjectiveFunction(ObjectiveFunction value) { objectiveFunction = value; }
   inline int getValue() { return value; }
   inline void setValue(int value) { this->value = value; }

   virtual std::string toString() = 0;

private:
   ObjectiveFunction objectiveFunction;
   int value;

protected:
   MoveType type;
};
