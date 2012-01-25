#include "HorarioAula.h"

HorarioAula::HorarioAula( void )
{
   this->calendario = NULL;
}

HorarioAula::~HorarioAula( void )
{
   this->calendario = NULL;
}

DateTime HorarioAula::getFinal()
{
	DateTime *t = new DateTime( this->inicio );
	t->addMinutes( this->tempo_aula );
	return *t;
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

bool HorarioAula::sobrepoe( HorarioAula h )
{
	if ( ( this->inicio < h.getFinal() && this->inicio >= h.getInicio() ) ||
		 ( this->getFinal() <= h.getFinal() && this->getFinal() > h.getInicio() ) )
	{
		return true;
	}

	return false;
}