#ifndef _ATENDIMENTO_TATICO_SOLUCAO_H_
#define _ATENDIMENTO_TATICO_SOLUCAO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "AtendimentoOfertaSolucao.h"
#include "AtendimentoTatico.h"

class AtendimentoTaticoSolucao :
   public OFBase
{
public:
   AtendimentoTaticoSolucao(void);
   AtendimentoTaticoSolucao( AtendimentoTatico & );
   virtual ~AtendimentoTaticoSolucao(void);

   virtual void le_arvore( ItemAtendimentoTaticoSolucao & );

   AtendimentoOfertaSolucao * atendimento_oferta;

   virtual bool operator < ( AtendimentoTaticoSolucao & right ) 
   { 
	   return ( ( qtdeCreditosTeoricos < right.getQtdeCreditosTeoricos() ) &&
				( qtdeCreditosPraticos < right.getQtdeCreditosPraticos() ) );
   }

   virtual bool operator == ( AtendimentoTaticoSolucao & right )
   { 
	   return ( ( qtdeCreditosTeoricos == right.getQtdeCreditosTeoricos() ) &&
				( qtdeCreditosPraticos == right.getQtdeCreditosPraticos() ) );
   }

   void setQtdeCreditosTeoricos(int v) { qtdeCreditosTeoricos = v; }
   void setQtdeCreditosPraticos(int v) { qtdeCreditosPraticos = v; }

   int getQtdeCreditosTeoricos() { return qtdeCreditosTeoricos; }
   int getQtdeCreditosPraticos() { return qtdeCreditosPraticos; }

private:
   int qtdeCreditosTeoricos;
   int qtdeCreditosPraticos;
};

#endif