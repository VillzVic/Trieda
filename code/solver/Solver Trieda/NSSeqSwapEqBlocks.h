#ifndef NSSEQSWAPEQBLOCKS_H
#define NSSEQSWAPEQBLOCKS_H

#include "NSSeq.hpp"

#include "MoveSwapEqBlocks.h"
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

#endif /*NSSEQSWAPEQBLOCKS_H*/