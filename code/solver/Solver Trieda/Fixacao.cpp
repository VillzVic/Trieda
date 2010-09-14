#include "Fixacao.h"

Fixacao::Fixacao(void)
{
}

Fixacao::~Fixacao(void)
{
}
void Fixacao::le_arvore(ItemFixacao& elem) {
   if (elem.professorId().present()) 
      professor_id = elem.professorId().get();
   if (elem.disciplinaId().present()) 
      disciplina_id = elem.disciplinaId().get();
   if (elem.salaId().present())
      sala_id = elem.salaId().get();
   if (elem.diaSemana().present())
      dia_semana = elem.diaSemana().get();
   if(elem.turnoId().present())
      turno_id = elem.turnoId().get();
   if (elem.horarioAulaId().present())
      horario_id = elem.horarioAulaId().get();
}