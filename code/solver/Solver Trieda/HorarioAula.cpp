#include "HorarioAula.h"

HorarioAula::HorarioAula( void )
	: calendario(nullptr), tempo_aula(0.0), inicio()
{
   this->calendario = NULL;
   //this->setTurnoId(0);

   faseDoDia = -1;
}

HorarioAula::~HorarioAula( void )
{}

DateTime HorarioAula::getFinal() const
{
	DateTime t;
	t = inicio;
	t.addMinutes( tempo_aula );
	return t;
}
void HorarioAula::getFinal(DateTime &date) const
{
	date = inicio;
	date.addMinutes( tempo_aula);
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
	return  (dias_semana.find(dia) != dias_semana.end());
}

bool HorarioAula::sobrepoe( HorarioAula const &h )
{
	// se o final de um horario for menor ou igual que o inicio de outro não há intersecção
	if(h.getFinal().earlierTime(inicio, true) || getFinal().earlierTime(h.inicio, true))
		return false;

	return true;
}
bool HorarioAula::sobrepoe( HorarioAula* const &h )
{
	// se o final de um horario for menor ou igual que o inicio de outro não há intersecção
	if(h->getFinal().earlierTime(inicio, true) || this->getFinal().earlierTime(h->inicio, true))
		return false;

	return true;
}
bool HorarioAula::sobrepoe( DateTime const &dti, DateTime const &dtf ) const
{
	// se o final de um horario for menor ou igual que o inicio de outro não há intersecção
	if(dtf.earlierTime(inicio, true) || getFinal().earlierTime(dti, true))
		return false;

	return true;
}

bool HorarioAula::inicioFimIguais ( HorarioAula const & right ) const
{
	if ( !inicio.sameTime(right.inicio) )
		return false;

	return ( tempo_aula == right.tempo_aula );
}

bool HorarioAula::inicioFimIguais ( HorarioAula* const &outro ) const
{
	if ( !inicio.sameTime(outro->inicio) )
		return false;

	return ( tempo_aula == outro->tempo_aula );
}