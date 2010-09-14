#include "Horario.h"
#include <iostream>

Horario::Horario(void)
{
}

Horario::~Horario(void)
{
}

void Horario::le_arvore(ItemHorario& elem) 
{
   ITERA_SEQ(it_dia,elem.diasSemana(),DiaSemana) {
      dias_semana.add(*it_dia);
   }
   horarioAulaId = elem.horarioAulaId();
   turnoId = elem.turnoId();
}

int Horario::getId()
{
   return ((turnoId * 1000) + (horarioAulaId * 10)/* + diaSemana*/);
}