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
   cred_teoricos = elem.cred_teoricos();
   cred_praticos = elem.cred_praticos();
   e_lab = elem.laboratorio();
   tipo = new TipoDisciplina();
   tipo->le_arvore(elem.TipoDisciplina());
   /* Todo: ter certa divisão de créditos */
   //divisao_creditos = new DivisaoCreditos();
   //divisao_creditos->le_arvore(elem.divisaoDeCreditos());
   ITERA_SEQ(it_contem,elem.equivalencia(),Disciplina) {
      Disciplina* contida = new Disciplina();
      contida->le_arvore(*it_contem);
      continencias.add(contida);
   }
   ITERA_SEQ(it_hora,elem.horarios(),HorarioDisponivel) {
      HorarioDisponivel* horario = new HorarioDisponivel();
      horario->le_arvore(*it_hora);
      horarios.add(horario);
   }
   ITERA_SEQ(it_sala,elem.salas(),Sala) {
      Sala* sala = new Sala();
      sala->le_arvore(*it_sala);
      salas.add(sala);
   }
   ITERA_SEQ(it_turma,elem.turmas(),Turma) {
      Turma* turma = new Turma();
      turma->le_arvore(*it_turma);
      turmas.add(turma);
   }
   ITERA_SEQ(it_demanda,elem.demandas(),Demanda) {
      Demanda* demanda = new Demanda();
      demanda->le_arvore(*it_demanda);
      demandas.add(demanda);
   }
}
