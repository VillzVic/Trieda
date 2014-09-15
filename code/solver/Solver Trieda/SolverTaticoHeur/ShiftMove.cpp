#include "ShiftMove.h"

#include "GUtil.h"

std::string ShiftMove::toString()
{
   std::string out = "Move { Type: SHIFT; Disciplina: " + GUtil::intToString(disciplina) + "; Origem: (" + GUtil::intToString(inicio1) + "," + GUtil::intToString(fim1) + "); Destino: (" + GUtil::intToString(inicio2) + "," + GUtil::intToString(fim2) + "); Turma: " + GUtil::intToString(turma) + "; }\n";
   return out;
}