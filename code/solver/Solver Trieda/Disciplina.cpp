#include "Disciplina.h"

Disciplina::Disciplina(void)
{
   max_demanda = 0;
   demanda_total = 0;
   max_alunos_t = -1;
   max_alunos_p = -1;
   min_creds = 0;

   divisao_creditos = NULL;
   tipo_disciplina = NULL;
   nivel_dificuldade = NULL;
   calendario = NULL;

   max_demanda = -1;
   num_turmas = -1;
   min_creds = -1;
   max_creds = -1;
   menorCapacSala = 10000;
   capacMediaSala = 0;
   nSalasAptas = 0;

}

Disciplina::~Disciplina(void)
{

}

void Disciplina::le_arvore( ItemDisciplina & elem )
{
   this->setId( elem.id() );
   codigo = elem.codigo();
   nome = elem.nome();
   cred_teoricos = elem.credTeoricos();
   cred_praticos = elem.credPraticos();
   max_creds = cred_teoricos + cred_praticos;
   e_lab = elem.laboratorio();

   if ( elem.maxAlunosTeorico().present() )
   {
      max_alunos_t = elem.maxAlunosTeorico().get();
   }

   if ( elem.maxAlunosPratico().present() )
   {
      max_alunos_p = elem.maxAlunosPratico().get();
   }

   tipo_disciplina_id = elem.tipoDisciplinaId();
   nivel_dificuldade_id = elem.nivelDificuldadeId();

   divisao_creditos = NULL;
   if ( elem.divisaoDeCreditos().present() )
   {
      divisao_creditos = new DivisaoCreditos();
      divisao_creditos->le_arvore( elem.divisaoDeCreditos().get() );
   }

   ITERA_NSEQ( it_contem, elem.disciplinasEquivalentes(), id, Identificador )
   {
      ids_disciplinas_equivalentes.add( *it_contem );
   }

   ITERA_NSEQ( it_inc, elem.disciplinasIncompativeis(), id, Identificador )
   {
      ids_disciplinas_incompativeis.add( *it_inc );
   }

   ITERA_SEQ( it_hora, elem.horariosDisponiveis(), Horario )
   {
      Horario * horario = new Horario();
      horario->le_arvore( *it_hora );
      horarios.add( horario );
   }
}

// Informa se uma dada disciplina é equivalente à esta disciplina
bool Disciplina::eh_equivalente( Disciplina * other )
{
	return (   ( this->getCredTeoricos() == other->getCredTeoricos() )
			&& ( this->getCredPraticos() == other->getCredPraticos() )
			&& ( this->eLab() == other->eLab() )
			&& ( this->getTipoDisciplinaId() == other->getTipoDisciplinaId() ) );
}