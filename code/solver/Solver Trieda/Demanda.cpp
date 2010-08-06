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
   curso->id = elem.curso().id();
   unidade = new Unidade; 
   unidade->id = elem.unidade().id();
   turno = new Turno;
   turno->id = elem.turno().id();
}
