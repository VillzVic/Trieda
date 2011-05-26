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
   virtual ~Horario(void);

   virtual void le_arvore( ItemHorario & );

   Turno * turno;
   HorarioAula * horario_aula;

   GGroup< int > dias_semana;

   void setHorarioAulaId(int v) { horarioAulaId = v; }
   void setTurnoId(int v) { turnoId = v; }

   int getHorarioAulaId() const { return horarioAulaId; }
   int getTurnoId() const { return turnoId; }

   virtual bool operator < ( Horario const & right ) 
   {
      if(horario_aula)
         return ( (*horario_aula) <  (*right.horario_aula) );

      return horarioAulaId < right.horarioAulaId;
   }

   virtual bool operator == ( Horario const & right ) 
   {
      if(horario_aula)
         return ( (*horario_aula) ==  (*right.horario_aula) );

      return horarioAulaId == right.horarioAulaId;
   }

private:
   int horarioAulaId;
   int turnoId;
};

#endif