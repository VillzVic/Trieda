#include "Demanda.h"

Demanda::Demanda(void)
{
}

Demanda::~Demanda(void)
{
}

void Demanda::le_arvore(ItemDemanda& elem)
{
   quantidade = elem.quantidade();
   curso = new Curso;
   curso->le_arvore(elem.curso());
   unidade = new Unidade; /*não vai ser isso, ok? */
   unidade->le_arvore(elem.unidade());
   turno = new Turno;
   turno->le_arvore(elem.turno());
}
