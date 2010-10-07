#include "Demanda.h"

Demanda::Demanda(void)
{
	oferta = NULL;
	disciplina = NULL;
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
