#ifndef _NS_SWAP_EQ_TEACHERS_BLOCKS_H_
#define _NS_SWAP_EQ_TEACHERS_BLOCKS_H_

#include "NS.hpp"

#include "MoveSwap.h"

#include "MoveSwapEqTeachersBlocksValidator.h"

#include "ProblemData.h"


class NSSwapEqTeachersBlocks : public NS
{
private:
   ProblemData & problemData;

   std::pair<Aula*,Aula*> pickTwoClasses(const SolucaoOperacional& s);

   MoveSwapEqTeachersBlocksValidator * moveValidator;

public:
   NSSwapEqTeachersBlocks(ProblemData & problemData);

   virtual ~NSSwapEqTeachersBlocks();

   Move & move(const SolucaoOperacional & s);

   void print();
};

#endif /*_NS_SWAP_EQ_TEACHERS_BLOCKS_H_*/