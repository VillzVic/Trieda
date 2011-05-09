#ifndef _NS_SWAP_EQ_SCHEDULES_BLOCKS_H_
#define _NS_SWAP_EQ_SCHEDULES_BLOCKS_H_

#include "NS.hpp"

#include "MoveSwap.h"

#include "MoveSwapEqSchedulesBlocksValidator.h"

#include "ProblemData.h"

class NSSwapEqSchedulesBlocks : public NS
{
private:
   ProblemData & problemData;

   std::pair<Aula*,Aula*> pickTwoClasses(const SolucaoOperacional& s);

   MoveSwapEqSchedulesBlocksValidator * moveValidator;

public:
   NSSwapEqSchedulesBlocks(ProblemData & problemData);

   virtual ~NSSwapEqSchedulesBlocks();

   Move & move(const SolucaoOperacional & s);

   void print();
};

#endif /*_NS_SWAP_EQ_SCHEDULES_BLOCKS_H_*/
