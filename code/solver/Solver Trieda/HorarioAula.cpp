#include "HorarioAula.h"

HorarioAula::HorarioAula(void)
{
}

HorarioAula::~HorarioAula(void)
{
}

void HorarioAula::le_arvore(ItemHorarioAula& elem)
{
   id = elem.id();
   inicio.setHour(elem.inicio().hours());
   inicio.setMinute(elem.inicio().minutes());
   ITERA_NSEQ(it_dia,elem.diasSemana(),diaSemana,DiaSemana) {
      diasSemana.add(*it_dia);
   }
}
