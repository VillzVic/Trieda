#include "Disciplina.h"

Disciplina::Disciplina(void)
{
   max_demanda = 0;
   demanda_total = 0;
   max_alunos_t = -1;
   max_alunos_p = -1;
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
   if(elem.maxAlunosTeorico().present())
      max_alunos_t = elem.maxAlunosTeorico().get();
   if(elem.maxAlunosPratico().present())
      max_alunos_p = elem.maxAlunosPratico().get();
   tipo_disciplina_id = elem.tipoDisciplinaId();
   nivel_dificuldade_id = elem.nivelDificuldadeId();
   
   divisao_creditos = NULL;
   if (elem.divisaoDeCreditos().present()) {
      divisao_creditos = new DivisaoCreditos();
      divisao_creditos->le_arvore(elem.divisaoDeCreditos().get());
   }

   ITERA_NSEQ(it_contem,elem.disciplinasEquivalentes(),id,Identificador) {
      equivalentes.add(*it_contem);
   }

   ITERA_NSEQ(it_inc,elem.disciplinasIncompativeis(),id,Identificador) {
      incompativeis.add(*it_inc);
   }

   ITERA_SEQ(it_hora,elem.horariosDisponiveis(),Horario) {
      Horario* horario = new Horario();
      horario->le_arvore(*it_hora);
      horarios.add(horario);
   }
}
