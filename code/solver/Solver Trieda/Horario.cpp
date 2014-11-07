#include "Horario.h"

#include <iostream>

Horario::Horario(void)
{
	horario_aula = nullptr;
	this->dias_semana.clear();
}

/*
	Cria novo Horario com ALGUNS dos atributos de h.
*/
Horario::Horario( Horario *h )
{
	this->horario_aula = new HorarioAula();
	this->horario_aula->setInicio( h->horario_aula->getInicio() );
	this->horario_aula->setTempoAula( h->horario_aula->getTempoAula() );
	this->horario_aula->dias_semana = h->horario_aula->dias_semana;
	this->horario_aula->setFaseDoDia( h->horario_aula->getFaseDoDia() );
	this->dias_semana = h->dias_semana;
	this->setHorarioAulaId( h->getHorarioAulaId() );
	this->setTurnoIESId( h->getTurnoIESId() );
}

Horario::~Horario(void)
{
	// Não deleto HorarioAula aqui, porque a principio ele é criado somente no seu calendario
	// e deverá ser deletado pelo mesmo.
}

void Horario::le_arvore( ItemHorario & elem ) 
{
   ITERA_NSEQ( it_dia, elem.diasSemana(), diaSemana, DiaSemana )
   {
      dias_semana.add( *it_dia );
   }

   horarioAulaId = elem.horarioAulaId();
   turnoIESId = elem.turnoId();
}
