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

/* Verifica se o HorarioAula esta disponivel no dia especificado da semana letiva  */
bool HorarioAula::horarioDisponivel( int dia )
{
	GGroup< int >::GGroupIterator it = dias_semana.begin();

	it = this->dias_semana.find(dia);
	if (it != this->dias_semana.end())
		return true;
	else
		return false;
}