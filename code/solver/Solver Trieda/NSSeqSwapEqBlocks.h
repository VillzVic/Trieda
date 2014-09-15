//TRIEDA-890
#ifndef _NS_SEQ_SWAP_EQ_BLOCKS_H_
#define _NS_SEQ_SWAP_EQ_BLOCKS_H_

#include "NSSeq.hpp"
#include "MoveSwap.h"
#include "MoveSwapEqBlocksValidator.h"
#include "NSIteratorSwapEqBlocks.h"
#include "ProblemData.h"

class NSSeqSwapEqBlocks
   : public NSSeq
{
public:
   NSSeqSwapEqBlocks( ProblemData & );
   virtual ~NSSeqSwapEqBlocks();

   Move & move( SolucaoOperacional & );
   NSIterator & getIterator( SolucaoOperacional & );
   void print();

private:
   ProblemData & problemData;
   std::pair< Aula *, Aula * > pickTwoClasses( SolucaoOperacional & );
   MoveSwapEqBlocksValidator * moveValidator;
};

#endif /*_NS_SEQ_SWAP_EQ_BLOCKS_H_*/