#ifndef NSITERATORSWAPEQBLOCKS_H_
#define NSITERATORSWAPEQBLOCKS_H_

#include "NSIterator.hpp"

#include "MoveSwap.h"
#include "MoveSwapEqBlocksValidator.h"

#include "GGroup.h"

class NSIteratorSwapEqBlocks : public NSIterator
{
public:
   NSIteratorSwapEqBlocks(ProblemData & pD, GGroup<Aula*> & aulas, SolucaoOperacional & solOp);
   virtual ~NSIteratorSwapEqBlocks();

	void first();

	void next();

	bool isDone();

	Move & current();

private:
   MoveSwap * move;

   MoveSwapEqBlocksValidator * moveValidator;

   GGroup<Aula*>::iterator itCurrent;
   GGroup<Aula*>::iterator itNext;

   GGroup<Aula*>::iterator itBegin;
   GGroup<Aula*>::iterator itBeforeEnd;
   GGroup<Aula*>::iterator itEnd;

   SolucaoOperacional & solOp;
};

#endif /*NSITERATORSWAPEQBLOCKS_H_*/