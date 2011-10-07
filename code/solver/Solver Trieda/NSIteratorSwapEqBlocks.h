#ifndef _NS_ITERATOR_SWAP_EQ_BLOCKS_H_
#define _NS_ITERATOR_SWAP_EQ_BLOCKS_H_

#include "NSIterator.hpp"
#include "MoveSwap.h"
#include "MoveSwapEqBlocksValidator.h"
#include "GGroup.h"

class NSIteratorSwapEqBlocks
   : public NSIterator
{
public:
   NSIteratorSwapEqBlocks( ProblemData &,
      GGroup< Aula *, LessPtr< Aula > > &, SolucaoOperacional & );
   virtual ~NSIteratorSwapEqBlocks();

	void first();
	void next();
	bool isDone();
	Move & current();

private:
   MoveSwap * move;
   MoveSwapEqBlocksValidator * moveValidator;

   GGroup< Aula *, LessPtr< Aula > >::iterator itCurrent;
   GGroup< Aula *, LessPtr< Aula > >::iterator itNext;

   GGroup< Aula *, LessPtr< Aula > >::iterator itBegin;
   GGroup< Aula *, LessPtr< Aula > >::iterator itBeforeEnd;
   GGroup< Aula *, LessPtr< Aula > >::iterator itEnd;

   SolucaoOperacional & solOp;
};

#endif
