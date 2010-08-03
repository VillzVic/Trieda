#include "Curso.h"

Curso::Curso(void)
{
}

Curso::~Curso(void)
{
}

void Curso::le_arvore(ItemCurso& elem)
{
   id = elem.id();
   codigo = elem.codigo();
   //tipo = new TipoCurso();
   //tipo->le_arvore(elem.tipo());
   qtd_min_doutores = elem.qtdMinDoutores();
   qtd_min_mestres = elem.qtdMinMestres();
   qtd_max_prof_disc = elem.qtdMaxProfDisc();
   //area_titulacao = new AreaTitulacao();
   ITERA_SEQ(it_curriculo,elem.Curriculos(),Curriculo)
   {
      Curriculo* curriculo = new Curriculo();
      curriculo->le_arvore(*it_curriculo);
      curriculos.add(curriculo);
   }
}
