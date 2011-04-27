#include "MoveSwapEqBlocksValidator.h"

MoveSwapEqBlocksValidator::MoveSwapEqBlocksValidator(ProblemData * pD) : MoveValidator(pD)
{
}

MoveSwapEqBlocksValidator::~MoveSwapEqBlocksValidator()
{
}

bool MoveSwapEqBlocksValidator::isValid(Aula & aX, Aula & aY)
{
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