#pragma once

#include "ofbase.h"
#include "GGroup.h"
#include "AtendimentoTaticoSolucao.h"

class AtendimentoDiaSemanaSolucao :
   public OFBase
{
public:
   AtendimentoDiaSemanaSolucao(void);
   ~AtendimentoDiaSemanaSolucao(void);
   virtual void le_arvore(ItemAtendimentoDiaSemanaSolucao& elem);

   GGroup<AtendimentoTaticoSolucao*> atendimentosTatico;

   virtual bool operator < (AtendimentoDiaSemanaSolucao & right) 
   { 
       return (diaSemana < right.getDiaSemana());
   }

   virtual bool operator == (AtendimentoDiaSemanaSolucao & right)
   { 
	   return (diaSemana == right.getDiaSemana());
   }

   void setDiaSemana(int v) { diaSemana = v; }
   int getDiaSemana() { return diaSemana; }

private:
	int diaSemana;
};
