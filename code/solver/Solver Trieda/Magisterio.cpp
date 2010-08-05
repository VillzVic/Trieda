#include "Magisterio.h"

Magisterio::Magisterio(Professor* professor)
{
   this->professor = professor;
}

Magisterio::~Magisterio(void)
{
}

void Magisterio::le_arvore(ItemProfessorDisciplina& elem)
{
   disciplina = new Disciplina;
   /* Le apenas o id, porque árvore já foi lida alhures */
   disciplina->id = elem.disciplina().id();

}
