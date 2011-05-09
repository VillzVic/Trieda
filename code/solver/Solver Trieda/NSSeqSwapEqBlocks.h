#ifndef _NS_SEQ_SWAP_EQ_BLOCKS_H_
#define _NS_SEQ_SWAP_EQ_BLOCKS_H_

#include "NSSeq.hpp"

#include "MoveSwap.h"

#include "MoveSwapEqBlocksValidator.h"

#include "NSIteratorSwapEqBlocks.h"

#include "ProblemData.h"

class NSSeqSwapEqBlocks : public NSSeq
{
private:
   ProblemData & problemData;

   std::pair<Aula*,Aula*> pickTwoClasses(const SolucaoOperacional& s);

   MoveSwapEqBlocksValidator * moveValidator;

public:
   NSSeqSwapEqBlocks(ProblemData & problemData);

   virtual ~NSSeqSwapEqBlocks();

   Move & move(const SolucaoOperacional & s);

   NSIterator & getIterator(const SolucaoOperacional & s);

   void print();

};

#endif /*_NS_SEQ_SWAP_EQ_BLOCKS_H_*/