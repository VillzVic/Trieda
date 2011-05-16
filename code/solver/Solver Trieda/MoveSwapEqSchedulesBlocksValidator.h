#ifndef _MOVE_SWAP_EQ_SCHEDULES_BLOCKS_VALIDATOR_HPP_
#define _MOVE_SWAP_EQ_SCHEDULES_BLOCKS_VALIDATOR_HPP_

#include "MoveValidator.h"

class MoveSwapEqSchedulesBlocksValidator : public MoveValidator
{
public:

   MoveSwapEqSchedulesBlocksValidator(ProblemData * pD);

   virtual ~MoveSwapEqSchedulesBlocksValidator();

   bool isValid(Aula & aX, Aula & aY, SolucaoOperacional & solOp);
};

#endif /*_MOVE_SWAP_EQ_SCHEDULES_BLOCKS_VALIDATOR_HPP_*/

