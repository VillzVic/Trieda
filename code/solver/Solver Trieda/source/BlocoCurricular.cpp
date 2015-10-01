#include "BlocoCurricular.h"

BlocoCurricular::BlocoCurricular( void )
{
   periodo = -1;
   curso = NULL;
   campus = NULL;
   total_turmas = -1;
   curriculo = NULL;
   oferta = NULL;
}

BlocoCurricular::~BlocoCurricular( void )
{

}

int BlocoCurricular::getMaxCredsNoDia(int dia)
{	
	std::map< int, int >::iterator it = max_creds_dia.begin();
	it = this->max_creds_dia.find( dia );

	if (it != this->max_creds_dia.end())
	{
		return( it->second );
	}

	return (0);
}

void BlocoCurricular::preencheMaxCredsPorDia()
{
	ITERA_GGROUP_N_PT( it_Dia_Letivo, this->diasLetivos, int )
    { 
		max_creds_dia[*it_Dia_Letivo] = curriculo->getMaxCreds(*it_Dia_Letivo);
    }
}


/*
	Retorna o numero maximo de creditos possivel, dado um dia da semana (dia), 	
	um Calendario (sl) e tipo de combinação de creditos das semanas letivas (id).
*/
int BlocoCurricular::getNroMaxCredCombinaSL( int k, Calendario *c, int dia )
{
	int n = 0;

	if ( dia < 0 || dia > 7 )
	{
		std::cerr<<"Erro em BlocoCurricular::getNroCredCombinaSL(): dia invalido.";
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


GGroup<HorarioAula*> BlocoCurricular::retornaHorariosDisponiveisNoDiaPorSL( int dia, Calendario* sl )
{
	GGroup<HorarioAula*> horariosNoDiaSL;

	map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPeriodoDisc = this->curriculo->disciplinas_periodo.begin();

	for (; itPeriodoDisc != this->curriculo->disciplinas_periodo.end(); itPeriodoDisc++ )
	{
		Disciplina *d = itPeriodoDisc->first;

		GGroup< Calendario*, Less<Calendario*> > calendsDisc = d->getCalendarios();
		if ( calendsDisc.find( sl ) != calendsDisc.end() )
		{
			ITERA_GGROUP_LESSPTR( it_horarios, d->horarios, Horario )
			{
				if ( it_horarios->horario_aula->getCalendario()->getId() == sl->getId() )
				{
					if ( it_horarios->horario_aula->horarioDisponivel( dia ) )
					{
						horariosNoDiaSL.add( it_horarios->horario_aula );
					}
				}
			}		
		}
    }
	return horariosNoDiaSL;
}

void BlocoCurricular::setCombinaCredSL( int dia, int k, Calendario* sl , int n )
{
	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.first = dia;
	t.second = k;
	t.third = sl;
	combinaCredSL[t] = n;
}

void BlocoCurricular::removeCombinaCredSL( Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t )
{
	combinaCredSL.erase(t);
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool BlocoCurricular::combinaCredSL_eh_dominado( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
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
bool BlocoCurricular::combinaCredSL_domina( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
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
bool BlocoCurricular::combinaCredSL_eh_repetido( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
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

std::map< Trio<int, int, Calendario*>, int > BlocoCurricular::retornaCombinaCredSL_Dominados( int dia )
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