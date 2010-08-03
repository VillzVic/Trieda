#include "AreaTitulacao.h"

AreaTitulacao::AreaTitulacao(void)
{
}

AreaTitulacao::~AreaTitulacao(void)
{
}
void AreaTitulacao::le_arvore(ItemAreaTitulacao& elem)
{
   id = elem.id();
   nome = elem.nome();
}
