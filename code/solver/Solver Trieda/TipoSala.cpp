#include "TipoSala.h"

TipoSala::TipoSala(void)
{
}

TipoSala::~TipoSala(void)
{
}

void TipoSala::le_arvore(ItemTipoSala& elem)
{
   id = elem.id();
   nome = elem.nome();
}
