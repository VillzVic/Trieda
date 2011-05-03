#include "MoveSwapEqSchedulesBlocksValidator.h"

MoveSwapEqSchedulesBlocksValidator::MoveSwapEqSchedulesBlocksValidator(ProblemData * pD) : MoveValidator(pD)
{
}

MoveSwapEqSchedulesBlocksValidator::~MoveSwapEqSchedulesBlocksValidator()
{
}

bool MoveSwapEqSchedulesBlocksValidator::isValid(Aula & aX, Aula & aY)
{
   std::vector< std::pair< Professor *, Horario * > >::iterator 
      itBlocoAulaX = aX.bloco_aula.begin();

   for(; itBlocoAulaX != aX.bloco_aula.end(); ++itBlocoAulaX)
   {
      std::vector< std::pair< Professor *, Horario * > >::iterator 
         itBlocoAulaY = aY.bloco_aula.begin();

      bool found = false;

      for(; itBlocoAulaY != aY.bloco_aula.end(); ++itBlocoAulaY)
      {
         if(itBlocoAulaX->second->getHorarioAulaId() == itBlocoAulaY->second->getHorarioAulaId())
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
   
   return(
      (canSwapSchedule(aX,aY)) &&

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