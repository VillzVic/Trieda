#ifndef _MOVE_SHIFT_VALIDATOR_HPP_
#define _MOVE_SHIFT_VALIDATOR_HPP_

#include "MoveValidator.h"

#include "SolucaoOperacional.h"

class MoveShiftValidator : public MoveValidator
{
public:
   //MoveShiftValidator(ProblemData * pD);

   MoveShiftValidator(ProblemData * pD, SolucaoOperacional & solOp);

   virtual ~MoveShiftValidator();

   bool canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos) const;

   bool isValid(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos);

private:
   SolucaoOperacional & solOp;
};

#endif /*_MOVE_SHIFT_VALIDATOR_HPP_*/