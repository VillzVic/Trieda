#pragma once

#include "Move.h"

class SwapDivisionMove : Move
{
public:
   SwapDivisionMove(void) { type = SWAP_DIVISION; }
   ~SwapDivisionMove(void) {}

   inline int getDisciplina() { return disciplina; }
   inline int getTurma() { return turma; }
   inline int getDivisao1() { return divisao1; }
   inline int getDivisao2() { return divisao2; }
   std::vector<int>& getInicio1() { return inicio1; }
   std::vector<int>& getFim1() { return fim1; }
   std::vector<int>& getInicio2() { return inicio2; }
   std::vector<int>& getFim2() { return fim2; }

   inline void setDisciplina(int value) { disciplina = value; }
   inline void setTurma(int value) { turma = value; }
   inline void setDivisao1(int value) { divisao1 = value; }
   inline void setDivisao2(int value) { divisao2 = value; }

   std::string toString();

private:
   int disciplina;
   int turma;
   int divisao1;
   int divisao2;
   std::vector<int> inicio1;
   std::vector<int> fim1;
   std::vector<int> inicio2;
   std::vector<int> fim2;
};

