#include "MoveSwapEqTeachersBlocksValidator.h"

MoveSwapEqTeachersBlocksValidator::MoveSwapEqTeachersBlocksValidator(ProblemData * pD) : MoveValidator(pD)
{
}

MoveSwapEqTeachersBlocksValidator::~MoveSwapEqTeachersBlocksValidator()
{
}

bool MoveSwapEqTeachersBlocksValidator::isValid(Aula & aX, Aula & aY)
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
      !(aY.eVirtual()) &&

      (*aX.bloco_aula.begin()->first) != (*aY.bloco_aula.begin()->first) // Comparing the teachers.
      );
}