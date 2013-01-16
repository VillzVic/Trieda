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
   DateTime getFinal();
   Calendario * getCalendario() const { return this->calendario; }
   double getTempoAula() const { return this->tempo_aula; }

   void setInicio( DateTime dt ) { this->inicio = dt; }
   void setCalendario( Calendario * c ) { this->calendario = c; }
   void setTempoAula( double value ) { this->tempo_aula = value; }

   bool horarioDisponivel( int dia );
   bool sobrepoe( HorarioAula h );

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
		 int mesmoFim = ( this->getTempoAula() == right.getTempoAula() );
         if ( mesmoFim )
		 {	// inicio e fim identicos, compara o id
			// result = ( this->getId() < right.getId() );				 
		 }
		 else
		 {			
			result = ( this->getTempoAula() < right.getTempoAula() );
		 }
      }


      return result;
   }

   virtual bool operator == ( HorarioAula const & right ) const
   { 
		return ( !( *this < right ) && !( right < *this ) );

    //  // 0 --> FALSE
    //  // outro valor --> TRUE
    //  bool result = ( ( this->getInicio() == right.getInicio() ) != 0 );

    //  // Quando os horários_aula começam no mesmo
    //  // instante, comparamos o horário de final da aula
	   //if ( result )
	   //{
    //     result = ( this->getTempoAula() == right.getTempoAula() );
	   //}

	   //return result;
   }

private:
   DateTime inicio;
   Calendario * calendario;
   double tempo_aula;
};

#endif