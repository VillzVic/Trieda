#include "AreaTitulacao.h"

AreaTitulacao::AreaTitulacao(void)
{
}

AreaTitulacao::~AreaTitulacao(void)
{
}
void AreaTitulacao::le_arvore(ItemAreaTitulacao& elem)
{
   this->setId( elem.id() );
   nome = elem.nome();
}
