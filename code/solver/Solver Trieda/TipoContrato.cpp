#include "TipoContrato.h"

TipoContrato::TipoContrato(void)
{
}

TipoContrato::~TipoContrato(void)
{
}

void TipoContrato::le_arvore(ItemTipoContrato& elem)
{
   this->setId( elem.id() );
   nome = elem.nome();
}
