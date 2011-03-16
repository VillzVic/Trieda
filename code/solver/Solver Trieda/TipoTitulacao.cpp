#include "TipoTitulacao.h"

TipoTitulacao::TipoTitulacao(void)
{
}

TipoTitulacao::~TipoTitulacao(void)
{
}

void TipoTitulacao::le_arvore(ItemTipoTitulacao& elem)
{
   this->setId( elem.id() );
   nome = elem.nome();
}
