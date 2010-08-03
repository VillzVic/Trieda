#include "TipoDisciplina.h"

TipoDisciplina::TipoDisciplina(void)
{
}

TipoDisciplina::~TipoDisciplina(void)
{
}

void TipoDisciplina::le_arvore(ItemTipoDisciplina& elem)
{
   id = elem.id();
   nome = elem.nome();
}
