#ifndef _MOVE_SWAP_EQ_TEACHERS_BLOCKS_VALIDATOR_HPP_
#define _MOVE_SWAP_EQ_TEACHERS_BLOCKS_VALIDATOR_HPP_

#include "MoveValidator.h"

class MoveSwapEqTeachersBlocksValidator : public MoveValidator
{
public:

   MoveSwapEqTeachersBlocksValidator(ProblemData * pD);

   virtual ~MoveSwapEqTeachersBlocksValidator();

   bool isValid(Aula & aX, Aula & aY);
};

#endif /*_MOVE_SWAP_EQ_TEACHERS_BLOCKS_VALIDATOR_HPP_*/
