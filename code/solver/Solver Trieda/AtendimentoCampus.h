#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoUnidade.h"

class AtendimentoCampus:
   public OFBase
{
public:
   AtendimentoCampus(void);
   ~AtendimentoCampus(void);

   std::string campus_id;
   GGroup<AtendimentoUnidade*> atendimentos_unidades;

   //virtual void escreve_arvore(ItemAtendimentoCampus& elem);
   
   //virtual TriedaOutput& escreve_arvore(TriedaOutput& root);

};

std::ostream& operator << (std::ostream& out, AtendimentoCampus& campus);