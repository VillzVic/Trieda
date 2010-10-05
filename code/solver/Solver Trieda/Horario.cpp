#include "Horario.h"
#include <iostream>

Horario::Horario(void)
{
	turno = NULL;
	horario_aula = NULL;
}

Horario::~Horario(void)
{
}

void Horario::le_arvore(ItemHorario& elem) 
{
   ITERA_NSEQ(it_dia,elem.diasSemana(),diaSemana,DiaSemana) {
      dias_semana.add(*it_dia);
   }
   horarioAulaId = elem.horarioAulaId();
   turnoId = elem.turnoId();
}

int Horario::getId()
{
   return ((turnoId * 1000) + (horarioAulaId * 10)/* + diaSemana*/);
}