#ifndef MOVEGENERIC_H_
#define MOVEGENERIC_H_

#include "Move.hpp"

#include "Aula.h"
#include "Professor.h"

class MoveGeneric : public Move
{
protected:
   Aula & a1;
   Professor & profA1;

   Aula & a2;
   Professor & profA2;

public:
   MoveGeneric(Aula & a1, Professor & profA1, Aula & a2, Professor & profA2);

   MoveGeneric::MoveGeneric(Aula & _a1, Aula & _a2);

   virtual ~MoveGeneric();

   //bool canBeApplied(const SolucaoOperacional & s);

   Move & apply(SolucaoOperacional & s);

   bool operator==(const Move & m) const;

   void print();
};

#endif /*MOVEGENERIC_H_*/