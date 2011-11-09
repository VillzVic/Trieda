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
   int getTempoAula() const { return this->tempo_aula; }

   void setInicio( DateTime dt ) { this->inicio = dt; }
   void setCalendario( Calendario * c ) { this->calendario = c; }
   void setTempoAula( int value ) { this->tempo_aula = value; }

   virtual bool operator < ( HorarioAula const & right ) const
   {
      bool result = false;

      // 0 --> FALSE
      // outro valor --> TRUE
      int mesmoInicio = ( this->getInicio() == right.getInicio() );

      // Horários de início de aula diferentes
      if ( mesmoInicio == 0 )
      {
         result = ( this->getInicio() < right.getInicio() );
      }
      else
      {
         // Quando os horários_aula começam no mesmo
         // instante, comparamos o horário de final da aula
         result = ( this->getTempoAula() < right.getTempoAula() );
      }

      return result;
   }

   virtual bool operator == ( HorarioAula const & right ) const
   { 
      // 0 --> FALSE
      // outro valor --> TRUE
      bool result = ( ( this->getInicio() == right.getInicio() ) != 0 );

      // Quando os horários_aula começam no mesmo
      // instante, comparamos o horário de final da aula
	   if ( result )
	   {
         result = ( this->getTempoAula() == right.getTempoAula() );
	   }

	   return result;
   }

private:
   DateTime inicio;
   Calendario * calendario;
   int tempo_aula;
};

#endif