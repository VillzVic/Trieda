#include "Campus.h"
#include <iostream>

Campus::Campus(void)
{
}

Campus::~Campus(void)
{
}

void Campus::le_arvore(ItemCampus& elem) 
{
   id = elem.id();
   codigo = elem.codigo();
   nome = elem.nome();

   ITERA_SEQ(it_unidades, elem.unidades(), Unidade) {
      Unidade* unidade = new Unidade();
      unidade->le_arvore(*it_unidades);
      unidades.add(unidade);
   }

   ITERA_SEQ(it_professores, elem.professores(), Professor) {
      Professor* professor = new Professor();
      professor->le_arvore(*it_professores);
      professores.add(professor);
   }

   ITERA_SEQ(it_horarios, elem.horariosDisponiveis(), Horario) {
      Horario* horario = new Horario();
      horario->le_arvore(*it_horarios);
      horarios.add(horario);
   }

}