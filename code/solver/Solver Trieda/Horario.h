#pragma once
#include "ofbase.h"
#include "Turno.h"
#include "HorarioAula.h"

class Horario :
   public OFBase
{
public:
   Horario(void);
   ~Horario(void);

   virtual void le_arvore(ItemHorario& raiz);

   GGroup<int> dias_semana;
   int horarioAulaId;
   int turnoId;
   Turno* turno;
   HorarioAula* horario_aula;
};
