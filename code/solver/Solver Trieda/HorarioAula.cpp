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
   ITERA_SEQ(it_dia,elem.diasSemana(),DiaSemana) {
      diasSemana.add(*it_dia);
   }
}
