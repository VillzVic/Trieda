#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoSala.h"

class AtendimentoUnidade:
   public OFBase
{
public:
   AtendimentoUnidade(void);
   ~AtendimentoUnidade(void);

   std::string unidade_id;
   GGroup<AtendimentoSala*> atendimentos_salas;

   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};
