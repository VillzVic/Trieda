#include "ProblemData.h"
#include <iostream>

ProblemData::ProblemData()
{
   /*
   ToDo:
   */
}

ProblemData::~ProblemData()
{
   /*
   ToDo:
   */
}

void ProblemData::le_arvore(Trieda& raiz)
{
   calendario = new Calendario();
   calendario->le_arvore(raiz.calendario());
   ITERA_SEQ(it_unidade,raiz.unidades(),Unidade) {
      Unidade* u = new Unidade;
      u->le_arvore(*it_unidade);
      std::cout << "Li unidade " << u->getId() << std::endl;
      unidades.add(u);
   }
   ITERA_SEQ(it_curso,raiz.cursos(),Curso) {
      Curso* c = new Curso;
      c->le_arvore(*it_curso);
      std::cout << "Li curso " << c->getId() << std::endl;
      cursos.add(c);
   }
   ITERA_SEQ(it_professor,raiz.professores(),Professor) {
      Professor* p = new Professor;
      p->le_arvore(*it_professor);
      std::cout << "Li professor " << p->getId() << std::endl;
      professores.add(p);
   }
   ITERA_SEQ(it_disc,raiz.disciplinas(),Disciplina) {
      Disciplina* d = new Disciplina;
      d->le_arvore(*it_disc);
      std::cout << "Li disciplina " << d->getId() << std::endl;
      disciplinas.add(d);
   }
   ITERA_SEQ(it_regra,raiz.regrasCredito(),DivisaoCreditos) {
      DivisaoCreditos* dc = new DivisaoCreditos;
      dc->le_arvore(*it_regra);
      std::cout << "Li regra " << dc->getId() << std::endl;
      regras_credito.add(dc);
   }
}
