#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoOferta.h"

class AtendimentoTatico:
   public OFBase
{
public:
   AtendimentoTatico(void);
   ~AtendimentoTatico(void);

   AtendimentoOferta *atendimentoOferta;
   int qtde_creditos_teoricos;
   int qtde_creditos_praticos;
   
   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};
