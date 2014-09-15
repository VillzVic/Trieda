#ifndef NSSWAPEQBLOCKS_H
#define NSSWAPEQBLOCKS_H

#include "NS.hpp"

#include "MoveSwapEqBlocks.h"

#include "ProblemData.h"

class NSSwapEqBlocks : public NS
{
private:
   ProblemData & problemData;

   std::pair<Aula*,Aula*> pickTwoClasses(const SolucaoOperacional& s);

public:
   NSSwapEqBlocks(ProblemData & problemData);

   virtual ~NSSwapEqBlocks();

   Move & move(const SolucaoOperacional& s);

   void print();

};

#endif /*NSSWAPEQBLOCKS_H*/