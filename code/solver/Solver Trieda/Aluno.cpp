#include "Aluno.h"

Aluno::Aluno( void )
{
   this->setAlunoId( -1 );
   this->oferta = NULL;
   this->ofertaId = -1;
   this->nCredsAlocados.clear();
   this->formando = false;
}

Aluno::Aluno( int id, std::string nome, Oferta* oft )
{
   this->setAlunoId( id );
   this->setNomeAluno( nome );
   this->oferta = oft;
   this->ofertaId = oft->getId();
   this->nCredsAlocados.clear();
}

Aluno::~Aluno( void )
{
}

bool Aluno::demandaDisciplina( int idDisc )
{
	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->getDisciplinaId() == idDisc )
		{
			return true;
		}
	}
	return false;
}

AlunoDemanda* Aluno::getAlunoDemanda( int disciplinaId )
{
	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->getDisciplinaId() == disciplinaId )
		{
			return (*itAlunoDemanda);
		}
	}
	return NULL;
}


std::ostream & operator << (
   std::ostream & out, Aluno & aluno )
{
   out << "<Aluno>" << std::endl;
      
   out << "<alunoId>" << aluno.getAlunoId()
       << "</alunoId>" << std::endl;

   out << "<nomeAluno>" << aluno.getNomeAluno()
	    << "</nomeAluno>" << std::endl;
   
   out << "<ofertaId>" << aluno.getOfertaId()
       << "</ofertaId>" << std::endl;

   out << "<Demandas>" << std::endl;
   ITERA_GGROUP_LESSPTR( itAlDemandas, aluno.demandas, AlunoDemanda )
   {
	   out << "<demandaId>" << itAlDemandas->demanda->getId()
			<< "</demandaId>" << std::endl;
   }  
   out << "</Demandas>" << std::endl;
      
   out << "</AlunoDemanda>" << std::endl;

   return out;
}


/*
	Retorna o numero maximo de creditos possivel, dado um dia da semana (dia), 	
	um Calendario (sl) e tipo de combinação de creditos das semanas letivas (id).
*/
int Aluno::getNroMaxCredCombinaSL( int k, Calendario *c, int dia )
{
	int n = 0;

	if ( dia < 0 || dia > 7 )
	{
		std::cerr<<"Erro em Aluno::getNroCredCombinaSL(): dia invalido.";
		return 0;
	}

	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.set( dia, k, c );

	std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator 
		it = combinaCredSL.find( t );

	if ( it != combinaCredSL.end() )
	{
		n = it->second;
	}
	
	return n;
}


double Aluno::getNroCreditosJaAlocados( Calendario* c, int dia )
{
	double tempo = 0.0;

	std::map< Calendario*, std::map< int /*dia*/, double /*nCreds*/> >::iterator
		itMap1 = nCredsAlocados.find( c );
	if ( itMap1 != nCredsAlocados.end() )
	{
		std::map< int /*dia*/, double /*nCreds*/>::iterator
			itMap2 = itMap1->second.find( dia );
		if ( itMap2 != itMap1->second.end() )
			tempo += itMap2->second;
	}
	return tempo;
}

double Aluno::getTempoJaAlocado( int dia )
{
	double tempo = 0.0;

	std::map< Calendario*, std::map< int /*dia*/, double /*nCreds*/> >::iterator
		itMap1 = nCredsAlocados.begin();
	for ( ; itMap1 != nCredsAlocados.end(); itMap1++ )
	{
		std::map< int /*dia*/, double /*nCreds*/>::iterator
			itMap2 = itMap1->second.find( dia );
		if ( itMap2 != itMap1->second.end() )
			tempo += itMap2->second;
	}
	return tempo;
}


/*
	Retorna todos os calendarios associados às demandas do aluno.
*/
GGroup< Calendario*, LessPtr<Calendario> > Aluno::retornaSemanasLetivas()
{
	GGroup< Calendario*, LessPtr<Calendario> > calendarios;

	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		Calendario *sl = (*itAlunoDemanda)->demanda->disciplina->getCalendario();

		calendarios.add( sl );
		
	}

	return calendarios;

}

/*
	FUNÇÕES PARA CRIAR A REGRA DE COMBINAÇÃO DOS CREDITOS DO ALUNO
*/


void Aluno::setCombinaCredSL( int dia, int k, Calendario* sl , int n )
{
	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.first = dia;
	t.second = k;
	t.third = sl;
	combinaCredSL[t] = n;
}

void Aluno::removeCombinaCredSL( Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t )
{
	combinaCredSL.erase(t);
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Aluno::combinaCredSL_eh_dominado( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
{
	std::map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for ( ; it_map != combinaCredSL.end(); it_map++  )
	{
		if ( it_map->first.first == dia &&
			 it_map->first.third == sl1 &&
			 it_map->second > i )
		{
			int k = it_map->first.second;

			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if ( combinaCredSL[t] >= j )
			{
					return true;
			}
		}
		else if ( it_map->first.first == dia &&
			 it_map->first.third == sl1 &&
			 it_map->second == i )
		{
			int k = it_map->first.second;

			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if ( combinaCredSL[t] > j )
			{
					return true;
			}
		}
	}

	return false;
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Aluno::combinaCredSL_domina( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
{
	std::map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for ( ; it_map != combinaCredSL.end(); it_map++  )
	{
		if ( it_map->first.first == dia &&
			 it_map->first.third == sl1 &&
			 it_map->second < i )
		{
			int k = it_map->first.second;
			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if ( combinaCredSL[t] <= j )
			{
				return true;
			}
		}
		else
		{
			if ( it_map->first.first == dia &&
				 it_map->first.third == sl1 &&
				 it_map->second == i )
			{
				int k = it_map->first.second;
				Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
				t.set(dia, k, sl2);

				if ( combinaCredSL[t] < j )
				{
					return true;
				}
			}
		}
	}

	return false;
}


// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Aluno::combinaCredSL_eh_repetido( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
{
	std::map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for ( ; it_map != combinaCredSL.end(); it_map++  )
	{
		if ( it_map->first.first == dia &&
			 it_map->first.third == sl1 &&
			 it_map->second == i )
		{
			int k = it_map->first.second;
			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if ( combinaCredSL[t] == j )
			{
				return true;
			}
		}
	}

	return false;
}

std::map< Trio<int, int, Calendario*>, int > Aluno::retornaCombinaCredSL_Dominados( int dia )
{
	std::map< Trio<int, int, Calendario*>, int > dominados;

	std::map< Trio< int, int, Calendario* >, int >::iterator it1_map = combinaCredSL.begin();
	for ( ; it1_map != combinaCredSL.end(); it1_map++  )
	{
		if ( it1_map->first.first == dia )
		{
			Calendario *sl1 = it1_map->first.third;
			int n1 = it1_map->second;
			int k = it1_map->first.second;

			std::map< Trio< int, int, Calendario* >, int >::iterator it2_map = combinaCredSL.begin();
			for ( ; it2_map != combinaCredSL.end(); it2_map++  )
			{
				if ( it2_map != it1_map &&
					 it2_map->first.first == dia &&
					 it2_map->first.second == k )
				{
					Calendario *sl2 = it2_map->first.third;
					int n2 = it2_map->second;
					
					if ( combinaCredSL_eh_dominado( n1, sl1, n2, sl2, dia ) )
					{
						dominados.insert( *it1_map );
						dominados.insert( *it2_map );
					}

				}
			}
		}
	}

	return dominados;
}


void Aluno::addNCredsAlocados( Calendario* sl, int dia, double value ) 
{ 
	std::map< Calendario*, std::map< int /*dia*/, double /*nCreds*/> >::iterator 
		it1 = nCredsAlocados.find(sl);
	
	if ( it1 == nCredsAlocados.end() )
	{
		std::map< int /*dia*/, double /*nCreds*/> diasCreds;
		diasCreds[dia] = value;

		nCredsAlocados[ sl ] = diasCreds;
	}
	else
	{
		std::map< int , double >::iterator 
			it2 = nCredsAlocados[sl].find( dia );
		if ( it2 == nCredsAlocados[sl].end() )
		{
			std::map< int /*dia*/, double /*nCreds*/> diasCreds;
			diasCreds[dia] = value;

			nCredsAlocados[sl] = diasCreds;
		}
		else
		{
			nCredsAlocados[sl][dia] += value;
		}
	}	
}

 bool Aluno::sobrepoeHorarioDiaOcupado( HorarioDia *hd )
 {
	 ITERA_GGROUP_LESSPTR( itHorDia, horariosDiaOcupados, HorarioDia )
	 {
		 int dia = (*itHorDia)->getDia();
		 HorarioAula *horAulaOcup = (*itHorDia)->getHorarioAula();

		 if( dia == hd->getDia() )
		 {
			 if ( horAulaOcup->sobrepoe( *hd->getHorarioAula() ) )
				 return true;
		 }
	 }
	 return false;
 }

 void Aluno::addHorarioDiaOcupado( GGroup<HorarioDia*> hds )
 { 
	 ITERA_GGROUP( it, hds, HorarioDia )
	 {
		 horariosDiaOcupados.add(*it); 
	 }
 }