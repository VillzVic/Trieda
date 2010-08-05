#include "Oferecimento.h"

Oferecimento::Oferecimento(void)
{
}

Oferecimento::~Oferecimento(void)
{
}

void Oferecimento::escreve_arvore(ItemOferecimento& elem)
{
   elem.id(id);
   elem.creditos(creditos);
   ItemSala item_sala = elem.sala();
   item_sala.id(sala->getId());
   elem.sala(item_sala);
   elem.semana(dia_da_semana);
   ItemTurma item_turma = elem.turma();
   item_turma.id(turma->getId());
   elem.turma(item_turma);
}
