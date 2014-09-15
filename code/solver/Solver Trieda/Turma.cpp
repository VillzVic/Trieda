#include "Turma.h"

Turma::Turma(void)
{
}

Turma::~Turma(void)
{
}

void Turma::le_arvore(ItemTurma& elem)
{
   id = elem.id();
   nome = elem.nome();
   ITERA_SEQ(it_ofer,elem.oferecimentos(),Oferecimento) {
      Oferecimento* ofer = new Oferecimento();
      ofer->le_arvore(*it_ofer);
      oferecimentos.add(ofer);
   }
}
