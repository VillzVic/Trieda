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

   GGroup< int > dias_semana; // Dias referentes à disponibilidade do Calendario! Para outras estruturas, os dias devem estar em Horario...

   DateTime getInicio() const { return this->inicio; }
   void getInicio(DateTime &date) const { date = inicio; }
   DateTime getFinal() const;
   void getFinal(DateTime &date) const;
   Calendario * getCalendario() const { return this->calendario; }
   double getTempoAula() const { return this->tempo_aula; }
   int getTurnoIESId() const { return turnoIES; }
   int getFaseDoDia() const {
	   if ( faseDoDia == -1) 
		   std::cout<<"\nErro, fase do dia nao setada para horAula id" << this->getId();
	   return faseDoDia; }

   void setInicio( DateTime dt ) { inicio = dt; }
   void setCalendario( Calendario * c ) { calendario = c; }
   void setTempoAula( double value ) { tempo_aula = value; }
   void setTurnoIESId( int v ) { turnoIES = v; }
   void setFaseDoDia( int fase ) { faseDoDia = fase; }

   bool horarioDisponivel( int dia );
   bool sobrepoe( HorarioAula const &h );
   bool sobrepoe( HorarioAula* const &h );

   // retorna nr minutos do inicio e fim
   int getInicioTimeMin(void) const { return inicio.timeMin(); }
   int getFinalTimeMin(void) const { return inicio.timeMin() + int(tempo_aula); }

   bool comparaMenor(HorarioAula const & right) const
   {
	   bool mesmoInicio = ( inicio.sameTime(right.inicio) );

		// Horários de início de aula diferentes
		if ( !mesmoInicio )
			return ( inicio.earlierTime(right.inicio) );

		// Quando os horários_aula começam no mesmo
		// instante, comparamos o horário de final da aula
		int diff = (int) ( tempo_aula - right.tempo_aula );
		
		return ( diff < 0 );
   }

   bool operator < ( HorarioAula const & right ) const
   {
	   bool mesmoInicio = ( inicio.sameTime(right.inicio) );

		// Horários de início de aula diferentes
		if ( !mesmoInicio )
			return ( inicio.earlierTime(right.inicio) );

		// Quando os horários_aula começam no mesmo
		// instante, comparamos o horário de final da aula
		int diff = (int) ( tempo_aula - right.tempo_aula );
		if ( diff == 0 )
			return ( getId() < right.getId() );
		else
			return ( diff < 0 );
   }

   bool operator > ( HorarioAula const & right ) const
   { 
      return ( right < *this );
   }

   bool operator == ( HorarioAula const & right ) const
   { 
		return ( !( *this < right ) && !( right < *this ) );
   }

   bool inicioFimIguais ( HorarioAula const & right ) const;
   bool inicioFimIguais ( HorarioAula* const &outro ) const;


private:
   DateTime inicio;
   Calendario * calendario;
   double tempo_aula;
   
   int turnoIES;
   
   int faseDoDia; // fase do dia
};

typedef GGroup< HorarioAula *, LessPtr< HorarioAula > > HorarioAulaGroup;

namespace std
{
	template<>
	struct less<HorarioAula*>
	{
		bool operator() (HorarioAula* const first, HorarioAula* const second) const
		{
			DateTime fstIni = first->getInicio();
			DateTime scdIni = second->getInicio();
			bool mesmoInicio = ( fstIni.sameTime(scdIni) );

			// Horários de início de aula diferentes
			if ( !mesmoInicio )
				return ( fstIni.earlierTime(scdIni) );

			// Quando os horários_aula começam no mesmo
			// instante, comparamos o horário de final da aula
			int diff = (int) ( first->getTempoAula() - second->getTempoAula() );
			if ( diff != 0 )
				return ( diff < 0 );
				
			return ( first->getId() < second->getId() );	
		}
	};
}

#endif