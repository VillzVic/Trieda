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
   disciplina_id = elem.disciplinaId();
   oferta_id = elem.ofertaCursoCampiId();
}
