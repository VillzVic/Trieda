#ifndef _MOVE_SHIFT_H_
#define _MOVE_SHIFT_H_

#include <vector>

#include "Move.hpp"

#include "Aula.h"
#include "Professor.h"

using namespace std;

class MoveShift : public Move
{
protected:
   
   Aula & aula;
   Professor * profAula;

   Professor & novoProfAula;
   std::vector<HorarioAula*> blocoHorariosVagos;

public:

   using Move::apply; // prevents name hiding
   
   MoveShift(Aula & aula, Professor & novoProfAula, std::vector<HorarioAula*> blocoHorariosVagos);

   virtual ~MoveShift();

   Move & apply(SolucaoOperacional & s);

   bool operator==(const Move & m) const;

   void print();
};

#endif /*_MOVE_SHIFT_H_*/