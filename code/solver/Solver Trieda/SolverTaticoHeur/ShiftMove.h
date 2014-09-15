#pragma once

#include "Move.h"

class ShiftMove : Move
{
public:
   ShiftMove(void) { type = SHIFT; }
   ~ShiftMove(void) {}

   inline int getDisciplina() { return disciplina; }
   inline int getInicio1() { return inicio1; }
   inline int getFim1() { return fim1; }
   inline int getInicio2() { return inicio2; }
   inline int getFim2() { return fim2; }
   inline int getTurma() { return turma; }

   inline void setDisciplina(int value) { disciplina = value; }
   inline void setInicio1(int value) { inicio1 = value; }
   inline void setFim1(int value) { fim1 = value; }
   inline void setInicio2(int value) { inicio2 = value; }
   inline void setFim2(int value) { fim2 = value; }
   inline void setTurma(int value) { turma = value; }

   std::string toString();

private:
   int disciplina;
   int inicio1;
   int fim1;
   int inicio2;
   int fim2;
   int turma;
};

