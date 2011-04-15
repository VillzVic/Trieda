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
}

Aula::~Aula(void)
{
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

int Aula::getTotalCreditos() const
{
   return (creditos_praticos+creditos_teoricos);
}

bool Aula::eVirtual() const
{
   return aula_virtual;
}

void Aula::toString()
{
   std::cout << "\n====================AULA====================\n";
   std::cout << "Turma: " << turma << std::endl;
   std::cout << "Disciplina: " << disciplina->getCodigo() << std::endl;
   
   std::cout << "Ofertas (ids): ";
   ITERA_GGROUP(itOferta,ofertas,Oferta)
   {
      std::cout << itOferta->getId() << " ";
   }

   std::cout << std::endl;
   std::cout << "Sala: " << sala->getCodigo() << std::endl;
   std::cout << "Dia: " << dia_semana << std::endl;
   std::cout << "Cred. Prat.: " << creditos_praticos << std::endl;
   std::cout << "Cred. Teo: " << creditos_teoricos << std::endl;


   std::cout << "Horario(s):\n\t";

   std::vector< std::pair< Professor *, Horario * > >::iterator
      itBloco_aula = bloco_aula.begin();

   for(; itBloco_aula != bloco_aula.end(); ++itBloco_aula)
      std::cout << itBloco_aula->second->horario_aula->getInicio() << "\n\t";

   std::cout << std::endl;
}