#ifndef _HORARIO_AULA_H_
#define _HORARIO_AULA_H_

#include "ofbase.h"
#include "GGroup.h"
#include "DateTime.h"

class Calendario;

class HorarioAula :
   public OFBase
{
public:
   HorarioAula( void );
   virtual ~HorarioAula( void );

   virtual void le_arvore( ItemHorarioAula & );

   GGroup< int > dias_semana;

   DateTime getInicio() const { return this->inicio; }
   Calendario * getCalendario() const { return this->calendario; }

   void setInicio( DateTime dt ) { this->inicio = dt; }
   void setCalendario( Calendario * c ) { this->calendario = c; }

   virtual bool operator < ( HorarioAula const & right ) const
   { 
      return ( this->getInicio() < right.getInicio() );
   }

   virtual bool operator == ( HorarioAula const & right ) const
   { 
	   if ( this->getInicio() == right.getInicio() )
	   {
		   return true;
	   }

	   return false;
   }

private:
   DateTime inicio;
   Calendario * calendario;
};

#endif