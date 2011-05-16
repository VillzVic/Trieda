#ifndef _MOVE_SHIFT_VALIDATOR_HPP_
#define _MOVE_SHIFT_VALIDATOR_HPP_

#include "MoveValidator.h"

class MoveShiftValidator : public MoveValidator
{
public:

   MoveShiftValidator(ProblemData * pD);

   virtual ~MoveShiftValidator();

   bool canShiftSchedule(Aula & aula, Professor & prof, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp) const;

   bool isValid(Aula & aula, Professor & prof, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp);

private:
   
};

#endif /*_MOVE_SHIFT_VALIDATOR_HPP_*/