#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoHorarioAula.h"

using namespace std;

class AtendimentoTurno:
   public OFBase
{
public:
   AtendimentoTurno(void);
   virtual ~AtendimentoTurno(void);

   int turno_id;
   GGroup<AtendimentoHorarioAula*> * atendimentos_horarios_aula;

   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};

std::ostream& operator << (std::ostream& out, AtendimentoTurno& turno);