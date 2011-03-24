#include "AlocacaoAula.h"

AlocacaoAula::AlocacaoAula(void)
{
   dia_semana = 0;
   professor_id = 0;
   horario_id = 0;
   professor = NULL;
   horario = NULL;
}

AlocacaoAula::~AlocacaoAula(void)
{
}

void AlocacaoAula::setDiaSemana(int d)
{
	dia_semana = d;
}

void AlocacaoAula::setProfessor(Professor* p)
{
	professor = p;
}

void AlocacaoAula::setProfessorId(int id)
{
	professor_id = id;
}

void AlocacaoAula::setHorario(Horario* h)
{
	horario = h;
}

void AlocacaoAula::setHorarioId(int id)
{
	horario_id = id;
}

int AlocacaoAula::getDiaSemana() const
{
	return dia_semana;
}

Professor* AlocacaoAula::getProfessor() const
{
	return professor;
}

int AlocacaoAula::getProfessorId() const
{
	return professor_id;
}

Horario* AlocacaoAula::getHorario() const
{
	return horario;
}

int AlocacaoAula::getHorarioId() const
{
	return horario_id;
}
