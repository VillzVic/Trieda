#include "Turno.h"

Turno::Turno( void )
{

}

Turno::~Turno( void )
{

}

void Turno::le_arvore( ItemTurno & elem )
{
	this->setId( elem.id() );
	this->setNome( elem.nome() );

	ITERA_SEQ( it_horarios, elem.HorariosAula(), HorarioAula )
	{
		HorarioAula * horario = new HorarioAula();
		horario->le_arvore( *it_horarios );
		this->horarios_aula.add( horario );
	}
}

/*
  Dado um dia especifico, retorna o numero de horarios de aula disponiveis no dia da semana letiva e turno em questao
  (ou numero de creditos).
*/
int Turno::getNroDeHorariosAula(int dia)
{
	int nHorariosNoTurnoDoDia = 0;
	ITERA_GGROUP_LESSPTR( it, horarios_aula, HorarioAula )
	{
		if ( it->horarioDisponivel(dia) ) nHorariosNoTurnoDoDia++;
	}
	return nHorariosNoTurnoDoDia;
}