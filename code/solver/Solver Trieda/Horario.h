#ifndef _HORARIO_H_
#define _HORARIO_H_

#include "ofbase.h"
#include "TurnoIES.h"
#include "GGroup.h"
#include "HorarioAula.h"

class Horario :
   public OFBase
{
public:
   Horario( void );
   Horario( Horario *h );
   virtual ~Horario( void );

   virtual void le_arvore( ItemHorario & );

   TurnoIES * turnoIES;
   HorarioAula * horario_aula;

   GGroup< int > dias_semana;

   void setHorarioAulaId( int v ) { horarioAulaId = v; }
   void setTurnoIESId( int v ) { turnoIESId = v; }

   int getHorarioAulaId() const { return horarioAulaId; }
   int getTurnoIESId() const { return turnoIESId; }

   virtual bool operator < ( Horario const & right ) const
   {
      if ( horario_aula )
      {
         return ( ( *horario_aula ) <  ( *right.horario_aula ) );
      }

      return ( horarioAulaId < right.horarioAulaId );
   }

   virtual bool operator == ( Horario const & right ) const
   {
      if ( horario_aula )
      {
         return ( ( *horario_aula ) ==  ( *right.horario_aula ) );
      }

      return ( horarioAulaId == right.horarioAulaId );
   }

private:
   int horarioAulaId;
   int turnoIESId;
};

#endif
