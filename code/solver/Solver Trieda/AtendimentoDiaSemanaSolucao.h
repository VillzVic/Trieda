#ifndef _ATENDIMENTO_DIA_SEMANA_SOLUCAO_H_
#define _ATENDIMENTO_DIA_SEMANA_SOLUCAO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "AtendimentoTaticoSolucao.h"
#include "AtendimentoDiaSemana.h"

class AtendimentoDiaSemanaSolucao :
   public OFBase
{
public:
   AtendimentoDiaSemanaSolucao(void);
   AtendimentoDiaSemanaSolucao( AtendimentoDiaSemana & );
   virtual ~AtendimentoDiaSemanaSolucao(void);

   virtual void le_arvore( ItemAtendimentoDiaSemanaSolucao & );

   GGroup< AtendimentoTaticoSolucao * > atendimentosTatico;

   virtual bool operator < ( AtendimentoDiaSemanaSolucao & right )
   { 
       return ( diaSemana < right.getDiaSemana() );
   }

   virtual bool operator == ( AtendimentoDiaSemanaSolucao & right )
   { 
	   return ( diaSemana == right.getDiaSemana() );
   }

   void setDiaSemana(int v) { diaSemana = v; }
   int getDiaSemana() { return diaSemana; }

private:
	int diaSemana;
};

#endif