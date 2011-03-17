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

   Turno* turno;
   HorarioAula* horario_aula;
   GGroup<int> dias_semana;

   void setHorarioAulaId(int v) { horarioAulaId = v; }
   void setTurnoId(int v) { turnoId = v; }

   int getHorarioAulaId() { return horarioAulaId; }
   int getTurnoId() { return turnoId; }

private:
   int horarioAulaId;
   int turnoId;
};
