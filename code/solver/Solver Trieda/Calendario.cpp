#include "Calendario.h"

#include <iostream>

Calendario::Calendario( void )
{

}

Calendario::~Calendario( void )
{

}

void Calendario::le_arvore( ItemCalendario & elem ) 
{
   this->setId( elem.id() );
   this->setCodigo( elem.codigo() );
   this->setTempoAula( elem.tempoAula() );

   ITERA_SEQ( it_turno, elem.turnos(), Turno )
   {
      Turno * turno = new Turno();
      turno->le_arvore( *it_turno );
      turnos.add( turno );
   }
}

/*
  Dado um dia especifico, retorna o numero de horarios de aula disponiveis no dia da semana letiva em questao
  (ou numero de creditos).
*/
int Calendario::getNroDeHorariosAula(int dia)
{
	int nHorariosNoDia = 0;
	ITERA_GGROUP_LESSPTR( it, turnos, Turno )
	{
		nHorariosNoDia += it->getNroDeHorariosAula(dia);
	}
	return nHorariosNoDia;
}