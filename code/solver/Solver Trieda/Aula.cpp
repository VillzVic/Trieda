#include "Aula.h"

Aula::Aula(void)
{
	this->turma = 0;
	this->disciplina = NULL;
	this->sala = NULL;
}

Aula::~Aula(void)
{
	//delete this->turma;
	delete this->disciplina;
	delete this->sala;
}

void Aula::setTurma(int t)
{
	this->turma = t;
}

void Aula::setDisciplina(Disciplina* d)
{
	this->disciplina = d;
}

void Aula::setSala(Sala* s)
{
	this->sala = s;
}

int Aula::getTurma() const
{
	return this->turma;
}

Disciplina* Aula::getDisciplina() const
{
	return this->disciplina;
}

Sala* Aula::getSala() const
{
	return this->sala;
}
















