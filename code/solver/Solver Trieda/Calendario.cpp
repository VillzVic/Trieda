#include "Calendario.h"
#include <iostream>

Calendario::Calendario(void)
{
}

Calendario::~Calendario(void)
{
	delete turno;
}

void Calendario::le_arvore(ItemCalendario& elem) 
{
   id = elem.id();
   codigo = elem.codigo();
   ITERA_SEQ(it_turno,elem.turnos(),Turno) {
      turno = new Turno();
      turno->le_arvore(*it_turno);
      turnos.add(turno);
   }
}
