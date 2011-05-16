#include "NSIteratorSwapEqBlocks.h"

NSIteratorSwapEqBlocks::NSIteratorSwapEqBlocks(ProblemData & pD, GGroup<Aula*> & aulas, SolucaoOperacional & _solOp) : 
   move(NULL), itBegin(aulas.begin()), itCurrent(aulas.begin()), itNext(aulas.begin()), itEnd(aulas.end()), itBeforeEnd(aulas.end()), solOp(_solOp)
{
   moveValidator = new MoveSwapEqBlocksValidator(&pD);

   ++itNext;
   --itBeforeEnd;
   first();
}

NSIteratorSwapEqBlocks::~NSIteratorSwapEqBlocks()
{
   delete moveValidator;
}

void NSIteratorSwapEqBlocks::first()
{
   while(!isDone())
   {
      if(!moveValidator->isValid((**itCurrent),(**itNext),solOp))
         next();
      else
         break;
   }

   //current();
}

void NSIteratorSwapEqBlocks::next()
{
   while(!moveValidator->isValid((**itCurrent),(**itNext),solOp) && !isDone())
   {
      if(itNext != itBeforeEnd)
      {
         ++itNext;
      }
      else
      {
         GGroup<Aula*>::iterator it2BeforeEnd = itBeforeEnd;
         --it2BeforeEnd;

         if(itCurrent != it2BeforeEnd)
         {
            ++itCurrent;
            itNext = itCurrent;
            ++itNext;
         }
         else // Gerando o prox movimento depois do ultimo (este sera inviavel)
         {
            ++itCurrent;
            ++itNext;
         }
      }
   }

   //if(itNext != itBeforeEnd)
   //{
   //   ++itNext;
   //}
   //else
   //{
   //   GGroup<Aula*>::iterator it2BeforeEnd = itBeforeEnd;
   //   --it2BeforeEnd;

   //   if(itCurrent != it2BeforeEnd)
   //   {
   //      ++itCurrent;
   //      itNext = itCurrent;
   //      ++itNext;
   //   }
   //   else // Gerando o prox movimento depois do ultimo (este sera inviavel)
   //   {
   //      ++itCurrent;
   //      ++itNext;
   //   }
   //}
}

bool NSIteratorSwapEqBlocks::isDone()
{
   return((itNext == itEnd) && (itCurrent == itBeforeEnd));
}

Move & NSIteratorSwapEqBlocks::current()
{
   if (isDone())
   {
      std::cout << "There isnt any current element!" << std::endl;
      std::cout << "Aborting." << std::endl;
      exit(1);
   }

   std::cout << "\n\nCURRENT MOVE\n\n";

   itCurrent->toString();
   itNext->toString();

   return *new MoveSwap( (**itCurrent), (**itNext) );
}
