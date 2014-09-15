#ifndef _MOVE_SHIFT_H_
#define _MOVE_SHIFT_H_

#include <vector>

#include "Move.hpp"
#include "Aula.h"
#include "Professor.h"

class MoveShift
   : public Move
{
public:
   using Move::apply; // prevents name hiding   
   MoveShift( Aula &, Professor &, std::vector< HorarioAula * > );
   virtual ~MoveShift();

   Move & apply( SolucaoOperacional & );
   bool operator == ( const Move & ) const;
   void print();

protected:
   Aula & aula;
   Professor * profAula;
   Professor & novoProfAula;
   std::vector< HorarioAula * > blocoHorariosVagos;
};

#endif
