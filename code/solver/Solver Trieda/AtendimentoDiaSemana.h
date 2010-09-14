#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoTatico.h"
#include "AtendimentoTurno.h" // PAREI AQUI - mario

class AtendimentoDiaSemana:
   public OFBase
{
public:
   AtendimentoDiaSemana(void);
   ~AtendimentoDiaSemana(void);

   int dia_semana;
   GGroup<AtendimentoTatico*> atendimentos_tatico;

   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};
