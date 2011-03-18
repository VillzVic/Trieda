#pragma once

#include "ofbase.h"
#include "GGroup.h"
#include "AtendimentoSalaSolucao.h"
#include "AtendimentoUnidade.h"

class AtendimentoUnidadeSolucao :
   public OFBase
{
public:
   AtendimentoUnidadeSolucao(void);

   AtendimentoUnidadeSolucao(AtendimentoUnidade & at_Und);

   ~AtendimentoUnidadeSolucao(void);
   virtual void le_arvore(ItemAtendimentoUnidadeSolucao& elem);

   GGroup<AtendimentoSalaSolucao *> atendimentosSalas;

   virtual bool operator < (AtendimentoUnidadeSolucao & right) 
   { 
	   return (unidadeId < right.getUnidadeId());
   }

   virtual bool operator == (AtendimentoUnidadeSolucao & right)
   { 
	   return (unidadeId == right.getUnidadeId());
   }

   void setUnidadeId(int v) { unidadeId = v; }
   void setUnidadeCodigo(std::string s) { unidadeCodigo = s; }

   int getUnidadeId() { return unidadeId; }
   std::string getUnidadeCodigo() { return unidadeCodigo; }

private:
   int unidadeId;
   std::string unidadeCodigo;   
};
