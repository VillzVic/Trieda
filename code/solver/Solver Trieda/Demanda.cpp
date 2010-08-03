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
   disciplina = new Disciplina;
   disciplina->le_arvore(elem.disciplina());
   turno = new Turno;
   turno->le_arvore(elem.turno());
}
