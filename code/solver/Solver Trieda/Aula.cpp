#include "Aula.h"

Aula::Aula(bool _aulaVirtual)
{
   turma = 0;
   disciplina = NULL;
   sala = NULL;
   dia_semana = 0;
   quantidade.clear();
   creditos_teoricos = 0;
   creditos_praticos = 0;
   aula_fixada = false;
   aula_virtual = _aulaVirtual;
}

Aula::~Aula(void)
{
}

void Aula::setTurma(int t)
{
   this->turma = t;
}

void Aula::setDisciplina(Disciplina * d)
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

void Aula::setAulaFixada( bool value )
{
	aula_fixada = value;
}

void Aula::setQuantidade( int value, Oferta* oft )
{
   quantidade[oft] = value;
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
   return ( this->getCreditosTeoricos() + this->getCreditosPraticos() );
}

bool Aula::eVirtual() const
{
   return aula_virtual;
}

bool Aula::eFixada() const
{
   return aula_fixada;
}

std::map<Oferta*,int> Aula::getQuantidade() const
{
   return quantidade;
}

int Aula::getQuantidadePorOft( Oferta *oft )
{
   return quantidade[oft];
}

int Aula::getQuantidadeTotal()
{
	int n=0;
	std::map<Oferta*, int>::iterator it = quantidade.begin();
	for ( ; it != quantidade.end(); it++ )
		n += it->second;
	return n;
}

void Aula::toString()
{
   //-------------------------------------------------------------
   std::cout << "\n=================AULA================="
			    << "\nTurma: " << turma
			    << "\nDisciplina: " << disciplina->getCodigo()
			    << std::endl;
   //-------------------------------------------------------------

   //-------------------------------------------------------------
   // Exibe a lista de ofertas atendidas por essa aula
   std::cout << "Ofertas atendidas: ";

   GGroup< Oferta *, LessPtr< Oferta > >::iterator 
      itOferta = ofertas.begin();

   for (; itOferta != ofertas.end(); ++itOferta )
   {
      std::cout << itOferta->getId() << " ";
   }
   //-------------------------------------------------------------

   //-------------------------------------------------------------
   // Exibe os dados da aula
   std::cout << "\nSala de Aula: " << sala->getCodigo()
			    << "\nDia da Semana: " << dia_semana
			    << "\nCreditos Praticos: " << creditos_praticos
			    << "\nCreditos Teoricos: " << creditos_teoricos
				<< "\nQuantidade: ";

   	std::map<Oferta*, int>::iterator it = quantidade.begin();
	for ( ; it != quantidade.end(); it++ )
		std::cout << it->second << "/";	
	std::cout << std::endl;
   //-------------------------------------------------------------
}