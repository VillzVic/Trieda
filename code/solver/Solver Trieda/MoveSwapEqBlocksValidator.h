#ifndef _MOVE_SWAP_EQ_BLOCKS_VALIDATOR_HPP_
#define _MOVE_SWAP_EQ_BLOCKS_VALIDATOR_HPP_

#include "MoveValidator.h"

class MoveSwapEqBlocksValidator : public MoveValidator
{
public:

   MoveSwapEqBlocksValidator(ProblemData * pD);

   virtual ~MoveSwapEqBlocksValidator();

   bool isValid(Aula & aX, Aula & aY, SolucaoOperacional & solOp);
};

#endif /*_MOVE_SWAP_EQ_BLOCKS_VALIDATOR_HPP_*/
