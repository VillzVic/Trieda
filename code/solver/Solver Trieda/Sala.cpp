#include "Sala.h"

Sala::Sala(void)
{
}

Sala::~Sala(void)
{
}

void Sala::le_arvore(ItemSala& elem)
{
   id = elem.id();
   codigo = elem.codigo();
   num_salas = elem.numSalas();
   capacidade = elem.capacidade();
   andar = elem.andar();
   TipoSala* tipo = new TipoSala;
   tipo->le_arvore(elem.tipoSala());
}
