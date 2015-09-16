#include "Fixacao.h"

Fixacao::Fixacao(void)
{
	professor = NULL;
	disciplina = NULL;
	sala = NULL;
	turno = NULL;
	horario_aula = NULL;

	professor_id = -1;
	disciplina_id = -1;
	sala_id = -1;
	dia_semana = -1;
	turno_id = -1;
	horario_aula_id = -1;
}

Fixacao::Fixacao( Fixacao const & fixacao )
{
   this->setId(-fixacao.getId());

   this->professor_id = fixacao.getProfessorId();
   this->disciplina_id = fixacao.getDisciplinaId();
   this->sala_id = fixacao.getSalaId();
   this->dia_semana = fixacao.getDiaSemana();
   this->turno_id = fixacao.getTurnoId();
   this->horario_aula_id = fixacao.getHorarioAulaId();

   this->professor = fixacao.professor;
   this->disciplina = fixacao.disciplina;
   this->sala = fixacao.sala;
   this->turno = fixacao.turno;
   this->horario_aula = fixacao.horario_aula;
}

Fixacao::~Fixacao( void )
{

}

void Fixacao::le_arvore( ItemFixacao & elem )
{
	this->setId( elem.id() );

	if ( elem.professorId().present() )
	{
		professor_id = elem.professorId().get();
	}

	if ( elem.disciplinaId().present() )
	{
		disciplina_id = elem.disciplinaId().get();
	}

	if ( elem.salaId().present() )
	{
		sala_id = elem.salaId().get();
	}

	if ( elem.diaSemana().present() )
	{
		dia_semana = elem.diaSemana().get();
	}

	if ( elem.turnoId().present() )
	{
		turno_id = elem.turnoId().get();
	}

	if ( elem.horarioAulaId().present() )
	{
		horario_aula_id = elem.horarioAulaId().get();
	}
}