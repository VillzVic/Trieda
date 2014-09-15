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
   AtendimentoSalaSolucao( void );
   AtendimentoSalaSolucao( AtendimentoSala & );
   virtual ~AtendimentoSalaSolucao( void );

   virtual void le_arvore( ItemAtendimentoSalaSolucao & elem );

   GGroup< AtendimentoDiaSemanaSolucao * > atendimentosDiasSemana;

   virtual bool operator < ( AtendimentoSalaSolucao & right ) const
   { 
	   return ( this->salaId < right.getSalaId() );
   }

   virtual bool operator == ( AtendimentoSalaSolucao & right ) const
   { 
	   return ( this->salaId == right.getSalaId() );
   }

   void setSalaId( int v ) { this->salaId = v; }
   void setSalaNome( std::string s ) { this->salaNome = s; }

   int getSalaId() { return this->salaId; }
   std::string getSalaNome() const { return this->salaNome; }

private:
   int salaId;
   std::string salaNome;
};

#endif