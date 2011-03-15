#pragma once

#include "ofbase.h"
#include "GGroup.h"
#include "AtendimentoOfertaSolucao.h"

class AtendimentoTaticoSolucao :
   public OFBase
{
public:
   AtendimentoTaticoSolucao(void);
   ~AtendimentoTaticoSolucao(void);
   virtual void le_arvore(ItemAtendimentoTaticoSolucao& elem);

   AtendimentoOfertaSolucao* atendimento_oferta;

   virtual bool operator < (AtendimentoTaticoSolucao & right) 
   { 
	   return ( ( qtdeCreditosTeoricos < right.getQtdeCreditosTeoricos()) &&
			( qtdeCreditosPraticos < right.getQtdeCreditosPraticos()) );
   }

   virtual bool operator == (AtendimentoTaticoSolucao & right)
   { 
	   return ( ( qtdeCreditosTeoricos == right.getQtdeCreditosTeoricos()) &&
			( qtdeCreditosPraticos == right.getQtdeCreditosPraticos()) );
   }

   void setQtdeCreditosTeoricos(int v) { qtdeCreditosTeoricos = v; }
   void setQtdeCreditosPraticos(int v) { qtdeCreditosPraticos = v; }

   int getQtdeCreditosTeoricos() { return qtdeCreditosTeoricos; }
   int getQtdeCreditosPraticos() { return qtdeCreditosPraticos; }

private:
   int qtdeCreditosTeoricos;
   int qtdeCreditosPraticos;
};
