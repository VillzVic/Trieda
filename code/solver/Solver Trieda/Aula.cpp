#include "Aula.h"

Aula::Aula(void)
{
	this->turma = 0;
	this->disciplina = NULL;
	this->sala = NULL;
}

Aula::~Aula(void)
{
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

void Aula::setDiaSemana(int d)
{
	this->diaSemana = d;
}

void Aula::setCreditosTeoricos(int ct)
{
	this->creditos_teoricos = ct;
}

void Aula::setCreditosPraticos(int cp)
{
	this->creditos_praticos = cp;
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

int Aula::getDiaSemana() const
{
	return this->diaSemana;
}

int Aula::getCreditosTeoricos() const
{
	return this->creditos_teoricos;
}

int Aula::getCreditosPraticos() const
{
	return this->creditos_praticos;
}















