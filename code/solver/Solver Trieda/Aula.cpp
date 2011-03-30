#include "Aula.h"

Aula::Aula(bool _aulaVirtual) : aula_virtual(_aulaVirtual)
{
   turma = 0;
   disciplina = NULL;
   sala = NULL;
   dia_semana = 0;
   quantidade = 0;
   creditos_teoricos = 0;
   creditos_praticos = 0;
   alocacao_aula.clear();
}

Aula::~Aula(void)
{
   alocacao_aula.clear();
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
   this->dia_semana = d;
}

void Aula::setCreditosTeoricos(int ct)
{
   this->creditos_teoricos = ct;
}

void Aula::setCreditosPraticos(int cp)
{
   this->creditos_praticos = cp;
}

void Aula::setOfertaCursoCampusId(int v)
{
	this->oferta_curso_campus_id = v;
}

void Aula::setAulaVirtual(bool value)
{
	aula_virtual = value;
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
   return this->dia_semana;
}

int Aula::getCreditosTeoricos() const
{
   return this->creditos_teoricos;
}

int Aula::getCreditosPraticos() const
{
   return this->creditos_praticos;
}

int Aula::getOfertacursoCampusId() const
{
	return this->oferta_curso_campus_id;
}

int Aula::getTotalCreditos() const
{
   return (creditos_praticos+creditos_teoricos);
}

bool Aula::eVirtual() const
{
   return aula_virtual;
}
