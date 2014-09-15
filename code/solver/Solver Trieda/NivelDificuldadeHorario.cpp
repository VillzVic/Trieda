#include "NivelDificuldadeHorario.h"

NivelDificuldadeHorario::NivelDificuldadeHorario(void)
{
}

NivelDificuldadeHorario::~NivelDificuldadeHorario(void)
{
}

void NivelDificuldadeHorario::le_arvore(ItemNivelDificuldadeHorario& elem)
{
   nivel_dificuldade_id = elem.nivelDificuldadeId();

   ITERA_NSEQ(it_horarios_aula, elem.horariosAula(), id, Identificador)
   {
      horarios_aula.add(*it_horarios_aula);
   }
}
