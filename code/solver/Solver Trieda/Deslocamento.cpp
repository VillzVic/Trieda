#include "Deslocamento.h"

Deslocamento::Deslocamento(void)
{
   origem = NULL;
   destino = NULL;
}

Deslocamento::~Deslocamento(void)
{
}
void Deslocamento::le_arvore(ItemDeslocamento& elem) {
   origem_id = elem.origemId();
   destino_id = elem.destinoId();
   tempo = elem.tempo();
   custo = elem.custo();
}