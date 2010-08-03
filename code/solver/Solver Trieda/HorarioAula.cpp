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
   ITERA_SEQ(it_disponivel,elem.horariosDisponiveis(),HorarioDisponivel)
   {
      HorarioDisponivel* disponivel = new HorarioDisponivel();
      disponivel->le_arvore(*it_disponivel);
      horarios_disponiveis.add(disponivel);
   }
}
