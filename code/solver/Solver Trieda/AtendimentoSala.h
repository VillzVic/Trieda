#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoDiaSemana.h"

using namespace std;

class AtendimentoSala:
   public OFBase
{
public:
   AtendimentoSala(void);
   ~AtendimentoSala(void);

   std::string sala_id;
   GGroup<AtendimentoDiaSemana*> atendimentos_dias_semana;

   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};

std::ostream& operator << (std::ostream& out, AtendimentoSala& sala);