#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoTatico.h"
#include "AtendimentoTurno.h"

using namespace std;

class AtendimentoDiaSemana:
   public OFBase
{
public:
   AtendimentoDiaSemana(void);
   ~AtendimentoDiaSemana(void);

   int dia_semana;
   GGroup<AtendimentoTatico*> atendimentos_tatico;
   GGroup<AtendimentoTurno*> atendimentos_turno;
   
   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};

std::ostream& operator << (std::ostream& out, AtendimentoDiaSemana& diaSem);