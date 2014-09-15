#pragma once

#include "Move.h"

class SwapMove : Move
{
public:
   SwapMove(void) { type = SWAP; }
   ~SwapMove(void) {}

   inline int getDisciplina() { return disciplina; }
   inline int getInicio1() { return inicio1; }
   inline int getFim1() { return fim1; }
   inline int getInicio2() { return inicio2; }
   inline int getFim2() { return fim2; }
   inline int getTurma1() { return turma1; }
   inline int getInicio1D() { return inicio1D; }
   inline int getFim1D() { return fim1D; }
   inline int getInicio2D() { return inicio2D; }
   inline int getFim2D() { return fim2D; }
   inline int getTurma2() { return turma2; }

   inline void setDisciplina(int value) { disciplina = value; }
   inline void setInicio1(int value) { inicio1 = value; }
   inline void setFim1(int value) { fim1 = value; }
   inline void setInicio2(int value) { inicio2 = value; }
   inline void setFim2(int value) { fim2 = value; }
   inline void setTurma1(int value) { turma1 = value; }
   inline void setInicio1D(int value) { inicio1D = value; }
   inline void setFim1D(int value) { fim1D = value; }
   inline void setInicio2D(int value) { inicio2D = value; }
   inline void setFim2D(int value) { fim2D = value; }
   inline void setTurma2(int value) { turma2 = value; }

   std::string toString();

private:
   int disciplina;
   int inicio1;
   int fim1;
   int inicio2;
   int fim2;
   int inicio1D;
   int fim1D;
   int inicio2D;
   int fim2D;
   int turma1;
   int turma2;
};

