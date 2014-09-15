#include "SwapDivisionMove.h"

#include "GUtil.h"

std::string SwapDivisionMove::toString()
{
   std::string out = "SwapDivisionMove { Type: SWAP_DIVISION; Disciplina: " + GUtil::intToString(disciplina) + "; Turma: " + GUtil::intToString(turma) + "; }\n";
   out += "                 { Origem[divisao = " + GUtil::intToString(divisao1) + "]: ";
   for (int i=0; i<(int)inicio1.size(); i++)
      out += "(" + GUtil::intToString(inicio1[i]) + ", " + GUtil::intToString(fim1[i]) + ") ";
   out += "}\n                 { Destino[divisao = " + GUtil::intToString(divisao2) + "]: ";
   for (int i=0; i<(int)inicio2.size(); i++)
      out += "(" + GUtil::intToString(inicio2[i]) + ", " + GUtil::intToString(fim2[i]) + ") ";
   out += "}\n";

   return out;
}