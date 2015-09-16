#ifndef _ATENDIMENTO_UNIDADE_SOLUCAO_H_
#define _ATENDIMENTO_UNIDADE_SOLUCAO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "AtendimentoSalaSolucao.h"
#include "AtendimentoUnidade.h"

class AtendimentoUnidadeSolucao :
   public OFBase
{
public:
   AtendimentoUnidadeSolucao();
   AtendimentoUnidadeSolucao( AtendimentoUnidade & );
   virtual ~AtendimentoUnidadeSolucao();

   virtual void le_arvore( ItemAtendimentoUnidadeSolucao & );

   GGroup< AtendimentoSalaSolucao * > atendimentosSalas;

   virtual bool operator < ( AtendimentoUnidadeSolucao & right ) const
   { 
	   return ( this->unidadeId < right.getUnidadeId() );
   }

   virtual bool operator == ( AtendimentoUnidadeSolucao & right ) const
   { 
	   return ( this->unidadeId == right.getUnidadeId() );
   }

   void setUnidadeId( int v ) { this->unidadeId = v; }
   void setUnidadeCodigo( std::string s ) { this->unidadeCodigo = s; }

   int getUnidadeId() const { return this->unidadeId; }
   std::string getUnidadeCodigo() const { return this->unidadeCodigo; }

private:
   int unidadeId;
   std::string unidadeCodigo;
};

#endif