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
   AtendimentoTaticoSolucao( void );
   AtendimentoTaticoSolucao( AtendimentoTatico & );
   virtual ~AtendimentoTaticoSolucao( void );

   virtual void le_arvore( ItemAtendimentoTaticoSolucao & );

   AtendimentoOfertaSolucao * atendimento_oferta;

   virtual bool operator < ( AtendimentoTaticoSolucao & right ) const
   { 
	   return ( ( this->qtdeCreditosTeoricos < right.getQtdeCreditosTeoricos() )
         && ( this->qtdeCreditosPraticos < right.getQtdeCreditosPraticos() ) );
   }

   virtual bool operator == ( AtendimentoTaticoSolucao & right ) const
   { 
	   return ( ( this->qtdeCreditosTeoricos == right.getQtdeCreditosTeoricos() )
         && ( this->qtdeCreditosPraticos == right.getQtdeCreditosPraticos() ) );
   }

   void setQtdeCreditosTeoricos( int v ) { this->qtdeCreditosTeoricos = v; }
   void setQtdeCreditosPraticos( int v ) { this->qtdeCreditosPraticos = v; }

   int getQtdeCreditosTeoricos() const { return this->qtdeCreditosTeoricos; }
   int getQtdeCreditosPraticos() const { return this->qtdeCreditosPraticos; }

private:
   int qtdeCreditosTeoricos;
   int qtdeCreditosPraticos;
};

#endif