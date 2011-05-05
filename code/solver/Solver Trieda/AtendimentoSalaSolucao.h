#ifndef _ATENDIMENTO_SALA_SOLUCAO_H_
#define _ATENDIMENTO_SALA_SOLUCAO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "AtendimentoDiaSemanaSolucao.h"
#include "AtendimentoSala.h"

class AtendimentoSalaSolucao :
   public OFBase
{
public:
   AtendimentoSalaSolucao(void);
   AtendimentoSalaSolucao( AtendimentoSala & );
   virtual ~AtendimentoSalaSolucao(void);

   virtual void le_arvore( ItemAtendimentoSalaSolucao & elem );

   GGroup< AtendimentoDiaSemanaSolucao * > atendimentosDiasSemana;

   virtual bool operator < ( AtendimentoSalaSolucao & right )
   { 
	   return ( salaId < right.getSalaId() );
   }

   virtual bool operator == ( AtendimentoSalaSolucao & right )
   { 
	   return ( salaId == right.getSalaId() );
   }

   void setSalaId(int v) { salaId = v; }
   void setSalaNome(std::string s) { salaNome = s; }

   int getSalaId() { return salaId; }
   std::string getSalaNome() { return salaNome; }

private:
   int salaId;
   std::string salaNome;
};

#endif