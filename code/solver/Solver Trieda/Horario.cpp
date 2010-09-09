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
   diaSemana = elem.diaSemana();
   horarioAulaId = elem.horarioAulaId();
   turnoId = elem.turnoId();
}

int Horario::getId()
{
   return ((turnoId * 1000) + (horarioAulaId * 10) + diaSemana);
}