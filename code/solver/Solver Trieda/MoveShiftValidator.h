#ifndef _MOVE_SHIFT_VALIDATOR_HPP_
#define _MOVE_SHIFT_VALIDATOR_HPP_

#include "MoveValidator.h"

class MoveShiftValidator : public MoveValidator
{
public:
   MoveShiftValidator(ProblemData * pD);

   virtual ~MoveShiftValidator();

   bool isValid(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos);
};

#endif /*_MOVE_SHIFT_VALIDATOR_HPP_*/