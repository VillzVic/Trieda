#include "Curriculo.h"

Curriculo::Curriculo(Curso* curso)
{
   this->curso = curso;
}

Curriculo::~Curriculo(void)
{
}

void Curriculo::le_arvore(ItemCurriculo& elem)
{
   id = elem.id();
   codigo = elem.codigo();
   descricao = elem.descricao();
   ITERA_SEQ(it_dp,elem.disciplinasPeriodo(),DisciplinaPeriodo) {
      Disciplina* d = new Disciplina;
      d->setId(it_dp->disciplina().id());
      disciplinas_periodo.add(std::make_pair(it_dp->periodo(),d));
   }
}
