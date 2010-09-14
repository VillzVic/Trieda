#include "Disciplina.h"

Disciplina::Disciplina(void)
{
}

Disciplina::~Disciplina(void)
{
}

void Disciplina::le_arvore(ItemDisciplina& elem)
{
   id = elem.id();
   codigo = elem.codigo();
   cred_teoricos = elem.credTeoricos();
   cred_praticos = elem.credPraticos();
   e_lab = elem.laboratorio();
   max_alunos_t = elem.maxAlunosTeorico();
   max_alunos_p = elem.maxAlunosPratico();
   tipo_disciplina_id = elem.tipoDisciplinaId();
   nivel_dificuldade_id = elem.nivelDificuldadeId();
   
   divisao_creditos = NULL;
   if (elem.divisaoDeCreditos().present()) {
      divisao_creditos = new DivisaoCreditos();
      divisao_creditos->le_arvore(elem.divisaoDeCreditos().get());
   }

   ITERA_SEQ(it_contem,elem.disciplinasEquivalentes(),Identificador) {
      equivalentes.add(*it_contem);
   }

   ITERA_SEQ(it_inc,elem.disciplinasIncompativeis(),Identificador) {
      incompativeis.add(*it_inc);
   }

   ITERA_SEQ(it_hora,elem.horariosDisponiveis(),Horario) {
      Horario* horario = new Horario();
      horario->le_arvore(*it_hora);
      horarios.add(horario);
   }
}
