#ifndef _ATENDIMENTO_OFERTA_SOLUCAO_H_
#define _ATENDIMENTO_OFERTA_SOLUCAO_H_

#include <iostream>

#include "ofbase.h"
#include "AtendimentoOferta.h"

class AtendimentoOfertaSolucao :
   public OFBase
{
public:
   AtendimentoOfertaSolucao( void );
   AtendimentoOfertaSolucao( AtendimentoOferta & );
   virtual ~AtendimentoOfertaSolucao( void );

   virtual void le_arvore( ItemAtendimentoOfertaSolucao & );

   virtual bool operator < ( AtendimentoOfertaSolucao & right ) const
   { 
	   return ( ( this->ofertaCursoCampiId < right.getOfertaCursoCampiId() )
         && ( this->disciplinaId < right.getDisciplinaId() )
         && ( this->turma < right.getTurma() ) );
   }

   virtual bool operator == ( AtendimentoOfertaSolucao & right ) const
   { 
	   return ( ( this->ofertaCursoCampiId == right.getOfertaCursoCampiId() )
         && ( this->disciplinaId == right.getDisciplinaId() )
         && ( this->turma == right.getTurma() ) );
   }

   void setOfertaCursoCampiId( int v ) { this->ofertaCursoCampiId = v; }
   void setDisciplinaId( int v ) { this->disciplinaId = v; }
   void setQuantidade( int v ) { this->quantidade = v; }
   void setTurma( std::string s ) { this->turma = s; }

   int getOfertaCursoCampiId() const { return this->ofertaCursoCampiId; }
   int getDisciplinaId() const { return this->disciplinaId; }
   int getQuantidade() const { return this->quantidade; }
   std::string getTurma() const { return this->turma; }

private:
   int ofertaCursoCampiId;
   int disciplinaId;
   int quantidade;
   std::string turma;
};

#endif