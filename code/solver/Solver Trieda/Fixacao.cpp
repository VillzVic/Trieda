#include "Fixacao.h"

Fixacao::Fixacao(void)
{
}

Fixacao::~Fixacao(void)
{
}
void Fixacao::le_arvore(ItemFixacao& elem) {
   if (elem.professorId()) 
      professor_id = elem.professorId().get();
   if (elem.disciplinaId()) 
      disciplina_id = elem.disciplinaId().get();
   if (elem.salaId())
      sala_id = elem.salaId().get();
   if (elem.diaSemana())
      dia_semana = elem.diaSemana().get();
   if(elem.turnoId())
      turno_id = elem.turnoId().get();
   if (elem.horarioAulaId())
      horario_id = elem.horarioAulaId().get();
}