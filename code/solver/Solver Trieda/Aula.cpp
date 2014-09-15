#include "Aula.h"

Aula::Aula(bool _aulaVirtual)
{
   turma = -1;
   disciplina = NULL;
   campus = NULL;
   sala = NULL;
   dia_semana = 0;
   quantidade.clear();
   creditos_teoricos = 0;
   creditos_praticos = 0;
   aula_fixada = false;
   aula_virtual = _aulaVirtual;
   mapOftDiscorigAlsDem.clear();
   hi = NULL;
   hf = NULL;


   _fixaSala = true;
   _fixaDia = true;
   _fixaCredsTeor = true;
   _fixaCredsPrat = true;
   _fixaHi = true;
   _fixaHf = true;
   _fixaAbertura = true;
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

void Aula::setCampus(Campus* c)
{
   this->campus = c;
}

void Aula::setUnidade(Unidade* u)
{
   this->unidade = u;
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

void Aula::setQuantPorOferta( int value, Oferta* oft )
{
   quantidade[oft] = value;
}

void Aula::addCursoAtendido( Curso* curso, int qtd )
{
	std::map<Curso*, int>::iterator itMap = cursosAtendidos.find(curso);
	if ( itMap != cursosAtendidos.end() )
	{
		itMap->second += qtd;
	}
	else itMap->second = qtd;

}

void Aula::setDisciplinaSubstituida( Disciplina * d, Oferta* oft )
{
	if ( d== NULL )
		std::cout<<"\nDisciplina substituida NULL, nao deveria! Em casos de nao haver substituida, enviar a original!\n";
	else
	{
		this->mapOftDiscorigAlsDem[oft][d];
	}
}

void Aula::addAlunoDemanda( Oferta* oft, Disciplina* orig, int alDemId )
{
	this->mapOftDiscorigAlsDem[oft][orig].add( alDemId );
}

void Aula::addAlunoDemanda( Oferta* oft, Disciplina* orig, GGroup<int> alsDemId )
{
	this->mapOftDiscorigAlsDem[oft][orig].add( alsDemId );
}

GGroup<int> Aula::getAlunosDemanda( Oferta* oft, Disciplina* orig )
{
	GGroup<int> alsDem;
	auto itOft = this->mapOftDiscorigAlsDem.find(oft);
	if ( itOft != this->mapOftDiscorigAlsDem.end() )
	{
		auto itDisc = itOft->second.find(orig);
		if ( itDisc != itOft->second.end() )
			alsDem = itDisc->second;
	}
	return alsDem;
}

void Aula::setHorarioAulaInicial( HorarioAula *h )
{
   hi = h;
}

void Aula::setHorarioAulaFinal( HorarioAula *h )
{
   hf = h;
}

void Aula::setDateTimeInicial( DateTime *dt )
{
   dti = dt;
}

void Aula::setDateTimeFinal( DateTime *dt )
{
   dtf = dt;
}

void Aula::addHorariosAulaId( int id )
{
	horariosAulaId.add(id);
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

ConjuntoSala * Aula::getCjtSala() const
{
	return this->sala->getCjtSalaAssociado();
}
 
Campus* Aula::getCampus() const
{
   return this->campus;
}  

Unidade* Aula::getUnidade() const
{
   return this->unidade;
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

std::map<Oferta*,int> Aula::getMapQuantPorOft() const
{
   return quantidade;
}

int Aula::getQuantidadePorOft( Oferta *oft )
{
   return quantidade[oft];
}

int Aula::getQuantidadeAlunos()
{
	int n=0;
	std::map<Oferta*, int>::iterator it = quantidade.begin();
	for ( ; it != quantidade.end(); it++ )
		n += it->second;
	return n;
}

std::map<Curso*, int>* Aula::getMapCursosAtendidos()
{
	std::map<Curso*, int> *pt = &cursosAtendidos;
	return pt;
}

GGroup<Disciplina*, LessPtr<Disciplina>> Aula::getDisciplinasSubstituidas( Oferta* oft )
{
	GGroup<Disciplina*, LessPtr<Disciplina>> disciplinas;

	auto itOft = mapOftDiscorigAlsDem.find(oft);
	if ( itOft != mapOftDiscorigAlsDem.end() )
	{
		auto itDisc = itOft->second.begin();
		for ( ; itDisc != itOft->second.end(); itDisc++ ) 
		{
			disciplinas.add( itDisc->first );
		}
	}
	return disciplinas;
}

HorarioAula* Aula::getHorarioAulaInicial() const
{
   return hi;
}

HorarioAula* Aula::getHorarioAulaFinal() const
{
   return hf;
}

DateTime* Aula::getDateTimeInicial() const
{
   return dti;
}

DateTime* Aula::getDateTimeFinal() const
{
   return dtf;
}

GGroup<int> Aula::getHorariosAulaId() const
{
	return horariosAulaId;
}

bool Aula::atendeAoCurso( int cursoId )
{
	ITERA_GGROUP_LESSPTR( it_oferta, this->ofertas, Oferta )
	{
		Oferta * oferta = ( *it_oferta );
		if ( oferta->getCursoId() == cursoId )
		{
			return true;
		}
	}
	return false;
}

void Aula::print()
{
   //-------------------------------------------------------------
   std::cout << "\n=================AULA================="
			    << "\nTurma: " << turma
			    << "\nDisciplina: " << disciplina->getCodigo();
   //-------------------------------------------------------------

   //-------------------------------------------------------------
   // Exibe a lista de ofertas atendidas por essa aula
   std::cout << "\nOfertas atendidas: ";

	auto itOft = mapOftDiscorigAlsDem.begin();
	for ( ; itOft != mapOftDiscorigAlsDem.end(); itOft++ )
	{
		std::cout << itOft->first->getId() << " ";

		auto itDisc = itOft->second.begin();
		for ( ; itDisc != itOft->second.end(); itDisc++ ) 
		{
			Disciplina* original = itDisc->first;
			if ( original->getId() != disciplina->getId() )
				std::cout << " (Disciplina Substituida: " << original->getCodigo() << ")";
		}
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

	if ( hi != NULL )
		std::cout << "\nHorario inicial: " << hi->getId();
	if ( hf != NULL )
		std::cout << "\nHorario final: " << hf->getId();
   //-------------------------------------------------------------
}

string Aula::toString()
{
	stringstream ss;
	
	ss << "{i" << turma;
	if ( disciplina != NULL )
		ss << "_d" << disciplina->getCodigo();
	if ( campus != NULL )
		ss << "_cp" << campus->getId();
	if ( sala != NULL )
		ss << "_s" << sala->getCodigo();	
	ss << "_t" << dia_semana;
	if ( hf != NULL )
		ss << "_" << hi->getInicio().getHour() << ":" << hi->getInicio().getMinute();
	if ( hf != NULL )
		ss << "-" << hf->getFinal().getHour() << ":" << hf->getFinal().getMinute();

	ss << "}";
	return ss.str();
}