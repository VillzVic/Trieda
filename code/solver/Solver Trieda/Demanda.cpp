#include "Demanda.h"

Demanda::Demanda(void)
{
	oferta = NULL;
	disciplina = NULL;
}

Demanda::Demanda(Demanda const & demanda)
{
   quantidade = demanda.quantidade;
   disciplina_id = demanda.disciplina_id;
   oferta_id = demanda.oferta_id;

   oferta = demanda.oferta;
   disciplina = demanda.disciplina;
}

Demanda::~Demanda(void)
{
}

void Demanda::le_arvore(ItemDemanda & elem)
{
	quantidade = elem.quantidade();
	disciplina_id = elem.disciplinaId();
	oferta_id = elem.ofertaCursoCampiId();
}

bool Demanda::operator== (Demanda const & right)
{
   return (
      (quantidade == right.quantidade) && 
      (disciplina == right.disciplina) && 
      (oferta == right.oferta));
}