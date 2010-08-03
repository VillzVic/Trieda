#include "TipoCurso.h"

TipoCurso::TipoCurso(void)
{
}

TipoCurso::~TipoCurso(void)
{
}

void TipoCurso::le_arvore(ItemTipoCurso& elem)
{
   id = elem.id();
   nome = elem.nome();
}
