#ifndef _HORARIO_AULA_H_
#define _HORARIO_AULA_H_

#include "ofbase.h"
#include "GGroup.h"
#include "DateTime.h"

class HorarioAula :
   public OFBase
{
public:
   HorarioAula(void);
   virtual ~HorarioAula(void);

   virtual void le_arvore( ItemHorarioAula & );

   GGroup< int > dias_semana;

   DateTime getInicio() const { return inicio; }
   void setInicio( DateTime dt ) { inicio = dt; }

   virtual bool operator < ( HorarioAula const & right ) 
   { 
      return ( this->getInicio() < right.getInicio() );
   }

   virtual bool operator == ( HorarioAula const & right ) 
   { 
	   if ( this->getInicio() == right.getInicio() )
	   {
		   return true;
	   }

	   return false;
   }

private:
   DateTime inicio;
};

#endif