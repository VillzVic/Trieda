#include "TipoTitulacao.h"

TipoTitulacao::TipoTitulacao(void)
{
}

TipoTitulacao::~TipoTitulacao(void)
{
}

void TipoTitulacao::le_arvore(ItemTipoTitulacao& elem)
{
   id = elem.id();
   nome = elem.nome();
}
