#include "Magisterio.h"

Magisterio::Magisterio()
{
}

Magisterio::~Magisterio(void)
{
}

void Magisterio::le_arvore(ItemProfessorDisciplina& elem)
{
   nota = elem.nota();
   preferencia = elem.preferencia();
   disciplina_id = elem.disciplinaId();
}
