#include "Calendario.h"
#include <iostream>

Calendario::Calendario(void)
{
}

Calendario::~Calendario(void)
{
}

void Calendario::le_arvore(ItemCalendario& elem) 
{
   id = elem.id();
   codigo = elem.codigo();
   descricao = elem.descricao();
   tempo_aula = elem.tempoAula();
   ITERA_SEQ(it_turno,elem.turnos(),Turno) {
      Turno* turno = new Turno();
      turno->le_arvore(*it_turno);
      turnos.add(turno);
   }
}
