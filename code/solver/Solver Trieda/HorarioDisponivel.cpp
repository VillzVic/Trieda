#include "HorarioDisponivel.h"

HorarioDisponivel::HorarioDisponivel(void)
{
}

HorarioDisponivel::~HorarioDisponivel(void)
{
}

void HorarioDisponivel::le_arvore(ItemHorarioDisponivel& elem)
{
   id = elem.id();
   semana = elem.semana();
}
