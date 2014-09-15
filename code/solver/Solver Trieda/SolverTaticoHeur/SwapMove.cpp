#include "SwapMove.h"

#include "GUtil.h"

std::string SwapMove::toString()
{
   std::string out = "Move { Type: SWAP; Disciplina: " + GUtil::intToString(disciplina) + "; Origem1: (" + GUtil::intToString(inicio1) + "," + GUtil::intToString(fim1) + "); Destino1: (" + GUtil::intToString(inicio1D) + "," + GUtil::intToString(fim1D) + "); Turma1: " + GUtil::intToString(turma1) + "; Origem2: (" + GUtil::intToString(inicio2) + "," + GUtil::intToString(fim2) + "); Destino2: (" + GUtil::intToString(inicio2D) + "," + GUtil::intToString(fim2D) + "); Turma2: " + GUtil::intToString(turma2) + "; }\n";
   return out;
}