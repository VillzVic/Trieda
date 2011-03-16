#include "TipoDisciplina.h"

TipoDisciplina::TipoDisciplina(void)
{
}

TipoDisciplina::~TipoDisciplina(void)
{
}

void TipoDisciplina::le_arvore(ItemTipoDisciplina& elem)
{
   this->setId( elem.id() );
   nome = elem.nome();
}
