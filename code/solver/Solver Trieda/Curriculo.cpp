#include "Curriculo.h"

Curriculo::Curriculo(void)
{
}

Curriculo::~Curriculo(void)
{
}

void Curriculo::le_arvore(ItemCurriculo& elem)
{
   id = elem.id();
   codigo = elem.codigo();
   descricao = elem.descricao();
   /* todo: ler conjunto de disciplinas do curriculo */
}
