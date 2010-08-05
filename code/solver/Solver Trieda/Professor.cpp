#include "Professor.h"

Professor::Professor(void)
{
}

Professor::~Professor(void)
{
}

void Professor::le_arvore(ItemProfessor& elem)
{
   id = elem.id();
   area_titulacao = new AreaTitulacao;
   area_titulacao->le_arvore(elem.areaTitulacao());
   cpf = elem.cpf();
   nome = elem.nome();
   ch_min = elem.chMin();
   ch_max = elem.chMax();
   ch_anterior = elem.credAnterior();
   valor_credito = elem.valorCred();
   ITERA_SEQ(it_unidade,elem.unidades(),Unidade) {
      Unidade* u = new Unidade;
      /* Le apenas o id, porque árvore já foi lida alhures */
      u->id = it_unidade->id();
      unidades.add(u);
   }
   ITERA_SEQ(it_horario,elem.horarios(),HorarioDisponivel) {

      HorarioDisponivel* h = new HorarioDisponivel;
      h->le_arvore(*it_horario);
      horarios.add(h);
   }
   // TODO: ler magistério do professor
   //ITERA_SEQ(it_mag,elem.disciplinas(),ProfessorDisciplina) {
   //   Magisterio* m = new Magisterio(this);
   //   m->le_arvore(*it_mag);
   //   magisterio.add(m);
   //}
}
