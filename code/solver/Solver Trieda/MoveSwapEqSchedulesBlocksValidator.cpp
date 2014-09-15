#include "MoveSwapEqSchedulesBlocksValidator.h"

MoveSwapEqSchedulesBlocksValidator::MoveSwapEqSchedulesBlocksValidator(ProblemData * pD) : MoveValidator(pD)
{
}

MoveSwapEqSchedulesBlocksValidator::~MoveSwapEqSchedulesBlocksValidator()
{
}

bool MoveSwapEqSchedulesBlocksValidator::isValid(Aula & aX, Aula & aY, SolucaoOperacional & solOp)
{
   // --------------------------------------------------------------------------
   // Checking the schedules.

   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::iterator 
      itBlocoAula_X = solOp.blocoAulas.find(&aX);

   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::iterator 
      itBlocoAula_Y = solOp.blocoAulas.find(&aY);

   if((itBlocoAula_X == solOp.blocoAulas.end()) || (itBlocoAula_Y == solOp.blocoAulas.end()))
   {
      std::cout << "Nao encontrei a aula desejada na estrutura de blocoAula de uma dada solucao. ERRO.\n";
      exit(1);
   }

   std::vector<HorarioAula*>::iterator 
      itScheduleAX = itBlocoAula_X->second.second.begin();

   for(; itScheduleAX != itBlocoAula_X->second.second.end(); ++itScheduleAX)
   {

      std::vector<HorarioAula*>::iterator 
         itScheduleAY = itBlocoAula_Y->second.second.begin();

      bool found = false;

      for(; itScheduleAY != itBlocoAula_Y->second.second.end(); ++itScheduleAY)
      {
         if(**itScheduleAX == **itScheduleAY)
         {
            found = true;
            break;
         }
      }

      if(!found)
      {
         return false;
      }
   }

   // --------------------------------------------------------------------------
  
   return(
      (canSwapSchedule(aX,aY,solOp)) &&

      (aX.getTotalCreditos() == aY.getTotalCreditos()) &&

      (aX.getDiaSemana() == aY.getDiaSemana()) &&

      (aX.getSala() == aY.getSala()) &&

      (aX != aY) &&

      !(aX.getDisciplina()->eLab()) &&
      !(aY.getDisciplina()->eLab()) &&

      !(aX.eVirtual()) &&
      !(aY.eVirtual())
      );
}