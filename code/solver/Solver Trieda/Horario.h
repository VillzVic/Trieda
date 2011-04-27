#ifndef _HORARIO_H_
#define _HORARIO_H_

#include "ofbase.h"
#include "Turno.h"
#include "GGroup.h"
#include "HorarioAula.h"

class Horario :
   public OFBase
{
public:
   Horario(void);
   ~Horario(void);

   virtual void le_arvore(ItemHorario& raiz);

   Turno * turno;
   HorarioAula * horario_aula;

   GGroup<int> dias_semana;

   void setHorarioAulaId(int v) { horarioAulaId = v; }
   void setTurnoId(int v) { turnoId = v; }

   int getHorarioAulaId() { return horarioAulaId; }
   int getTurnoId() { return turnoId; }

   virtual bool operator < (Horario const & right) 
   { 
      return ( *horario_aula <  *right.horario_aula );
   }

   virtual bool operator == (Horario const & right) 
   { 
      return ( *horario_aula ==  *right.horario_aula );
   }

private:
   int horarioAulaId;
   int turnoId;
};

#endif