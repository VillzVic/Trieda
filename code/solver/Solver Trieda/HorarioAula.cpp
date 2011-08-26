#include "HorarioAula.h"

HorarioAula::HorarioAula( void )
{
   this->calendario = NULL;
}

HorarioAula::~HorarioAula( void )
{
   this->calendario = NULL;
}

void HorarioAula::le_arvore( ItemHorarioAula & elem )
{
   this->setId( elem.id() );
   this->inicio.setHour( elem.inicio().hours() );
   this->inicio.setMinute( elem.inicio().minutes() );

   ITERA_NSEQ( it_dia, elem.diasSemana(), diaSemana, DiaSemana )
   {
      this->dias_semana.add( *it_dia );
   }
}
