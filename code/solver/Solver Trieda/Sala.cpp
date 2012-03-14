#include "Sala.h"
#include "ConverteHorariosCreditos.h"

Sala::Sala(void)
{
}

Sala::~Sala(void)
{
}

void Sala::le_arvore(ItemSala& elem)
{
	this->setId( elem.id() );
	codigo = elem.codigo();
	andar = elem.andar();
	numero = elem.numero();
	tipo_sala_id = elem.tipoSalaId();
	capacidade = elem.capacidade();

	ITERA_NSEQ(it_disc,elem.disciplinasAssociadas(), id, Identificador)
	{
		disciplinas_associadas.add(*it_disc);
	}
}

// 'possuiOutputTatico'
// TRUE  -> XML de entrada possui a saída do tático
// FALSE -> XML de entrada NÃO possui a saída do tático
// 'modo_operacao' = TATICO ou OPERACIONAL
void Sala::construirCreditosHorarios(ItemSala& elem, std::string modo_operacao, bool possuiOutputTatico)
{
	// Primeiro caso : executar o solver apenas com a entrada do tático
	bool primeiroCaso = ( modo_operacao == "TATICO" );

	// Segundo caso  : executar o solver com a saída do tático e a entrada do operacional
	bool segundoCaso  = ( modo_operacao == "OPERACIONAL" && possuiOutputTatico == true );

	// Terceiro caso : executar o solver apenas com a entrada do operacional (sem saída do tático)
	bool terceiroCaso = ( modo_operacao == "OPERACIONAL" && possuiOutputTatico == false );

	// TODO: depois que creditos_disponiveis for retirado, e horarios_disponiveis usado para os dois modos de otimiz
	// deletar essa divisao de casos comentada abaixo
	/*
	if( primeiroCaso )
	{
		// Lê creditos disponiveis
		if ( elem.creditosDisponiveis().present() )
		{
			ITERA_SEQ(it_cred, elem.creditosDisponiveis().get(), CreditoDisponivel)
			{
				CreditoDisponivel * credito_disp = new CreditoDisponivel();
				credito_disp->le_arvore( *it_cred );
				creditos_disponiveis.add( credito_disp );
			}
		}
	}
	else if( segundoCaso )
	{
		// Lê horarios disponiveis
		if ( elem.horariosDisponiveis().present() )
		{
			ITERA_SEQ(it_hora, elem.horariosDisponiveis().get(), Horario)
			{
				Horario * horario = new Horario();
				horario->le_arvore( *it_hora );
				horarios_disponiveis.add( horario );
			}
		}
	}
	else if( terceiroCaso )
	{
		if ( elem.horariosDisponiveis().present() )
		{
			// Lê horarios disponiveis
			ITERA_SEQ(it_hora, elem.horariosDisponiveis().get(), Horario)
			{
				Horario * horario = new Horario();
				horario->le_arvore( *it_hora );
				horarios_disponiveis.add( horario );
			}

			// A partir dos 'horários disponíveis' (input operacional),
			// devemos construir a estrutura de 'créditos disponíveis'
			GGroup< CreditoDisponivel * > creditosDisponiveis
				= converteHorariosParaCreditos();

			ITERA_GGROUP(it_cred, creditosDisponiveis, CreditoDisponivel)
			{
				CreditoDisponivel * credito_disp = new CreditoDisponivel();

				credito_disp->setTurnoId( it_cred->getTurnoId() );
				credito_disp->setDiaSemana( it_cred->getDiaSemana() );
				credito_disp->setMaxCreditos( it_cred->getMaxCreditos() );

				creditos_disponiveis.add( credito_disp );
			}
		}
	}*/
	
	// Lê horarios disponiveis
	if ( elem.horariosDisponiveis().present() )
	{
		ITERA_SEQ(it_hora, elem.horariosDisponiveis().get(), Horario)
		{
			Horario * horario = new Horario();
			horario->le_arvore( *it_hora );
			horarios_disponiveis.add( horario );
		}
	}

}


// TODO: essa funcao comentada abaixo pode ser deletada depois que creditos_disponiveis for deletado em Sala.h
/*
GGroup< CreditoDisponivel * > Sala::converteHorariosParaCreditos()
{
	// Objeto de retorno, contendo os créditos criados
	// a partir dos horários contidos no XML de entrada
	GGroup< CreditoDisponivel * > creditosDisponiveis;

	GGroup< ConverteHorariosCreditos * > groupHorariosCreditos;
	ITERA_GGROUP(it_horario, horarios_disponiveis, Horario)
	{
		GGroup< int >::iterator it_dia_semana = it_horario->dias_semana.begin();
		for (; it_dia_semana != it_horario->dias_semana.end();
			   it_dia_semana++)
		{
            int diaaaa = *(it_dia_semana);

			ConverteHorariosCreditos * item = new ConverteHorariosCreditos();
			item->setTurno( it_horario->getTurnoId() );
			item->setDiaSemana( *it_dia_semana );

			bool found = false;
			ITERA_GGROUP(it, groupHorariosCreditos, ConverteHorariosCreditos)
			{
			   if((it->getDiaSemana() == *it_dia_semana)
					&& (it->getTurno() == it_horario->getTurnoId()))
			   {
			      // Inserir mais um horário em 'groupHorariosCreditos'
			      it->horarios.add( it_horario->getHorarioAulaId() );

			      found = true;
			      break;
			   }
			}

			if( !found )
			{
			   // Adicionar novo item na lista
			   // 'groupHorariosCreditos' e inserir o horário				
			   groupHorariosCreditos.add( item );
			   item->horarios.add( it_horario->getHorarioAulaId() );
			}
		}
	}

	// Com a lista de objetos 'ConverteHorariosCreditos'
	// preenchida, construimos agora os créditos correspondentes
	ITERA_GGROUP(it_converte, groupHorariosCreditos, ConverteHorariosCreditos)
	{
		CreditoDisponivel * credito = new CreditoDisponivel();

		credito->setTurnoId( it_converte->getTurno() );
		credito->setDiaSemana( it_converte->getDiaSemana() );
		credito->setMaxCreditos( it_converte->horarios.size() );

		creditosDisponiveis.add( credito );
	}

	return creditosDisponiveis;
}*/


int Sala::max_creds(int dia)
{
	int tempoDisp = 0;
	int creds = 0;

    ITERA_GGROUP( itSl, this->temposSL, Calendario )
    {
	   int duracaoSl = itSl->getTempoAula();
	   int tempo = tempoDispPorDiaSL[ std::make_pair( dia, *itSl) ];
       if ( tempoDisp < tempo ) 
	   {
          tempoDisp = tempo;
		  creds = tempo / duracaoSl;
       }
    }

    return creds;
}

int Sala::max_credsPorSL(int dia, Calendario* sl)
{
	int tempoDisp = 0;
	int creds = 0;

	GGroup< Calendario* >::iterator itSl = temposSL.find( sl );
	
	if ( itSl == this->temposSL.end() )
		return creds;

    tempoDisp = tempoDispPorDiaSL[ std::make_pair( dia, sl) ];
	creds = tempoDisp / sl->getTempoAula();

    return creds;
}

int Sala::getTempoDispPorDiaSL( int dia, Calendario *sl )
{
	std::map< std::pair<int,Calendario*>, int >::iterator it = tempoDispPorDiaSL.find( std::make_pair(dia,sl) );
	if ( it != tempoDispPorDiaSL.end() )
		return it->second;
	else
		return 0;
}

int Sala::getTempoDispPorDia( int dia )
{
	std::map<int, int>::iterator it = tempoDispPorDia.find(dia);
	if ( it != tempoDispPorDia.end() )
		return it->second;
	else
		return 0;
}

void Sala::calculaTemposSL()
{
	ITERA_GGROUP(it_horarios, this->horarios_disponiveis, Horario)
	{
		Calendario* novoCalendario = it_horarios->horario_aula->getCalendario();

		if ( temposSL.find (novoCalendario ) == temposSL.end() )
			temposSL.add( novoCalendario );
	}

}

/*
	Calcula o tempo de aula de cada semana letiva disponivel para cada dia letivo da sala.
*/
void Sala::calculaTempoDispPorDiaSL()
{
	calculaTemposSL();

	ITERA_GGROUP_N_PT(it_dia, diasLetivos, int) // Para cada dia
	{
		int dia = *(it_dia);
		
		ITERA_GGROUP(it_sl, temposSL, Calendario) // Para cada semana letiva
		{
			Calendario * sl = *it_sl;
			
			GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosAula;
			ITERA_GGROUP(it_horarios, this->horarios_disponiveis, Horario)
			{
				if ( it_horarios->dias_semana.find(dia) != it_horarios->dias_semana.end() )
				{
					if ( it_horarios->horario_aula->getCalendario() == sl )
						horariosAula.add( it_horarios->horario_aula );
				}
			}

			int T = somaTempo(horariosAula);
			setTempoDispPorDiaSL( dia, sl, T );
		}
	}

}


/*
	Calcula o tempo total disponivel para cada dia letivo da sala.
	Considera a intersecao das semanas letivas, tratando o caso de sobreposicao de horarios.
*/
void Sala::calculaTempoDispPorDia()
{
	ITERA_GGROUP_N_PT(it_dia, diasLetivos, int)
	{
		int dia = *(it_dia);
		
		GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosAula;
		
		ITERA_GGROUP(it_horarios, this->horarios_disponiveis, Horario)
		{
			if ( it_horarios->dias_semana.find(dia) != it_horarios->dias_semana.end() )
			{
					horariosAula.add( it_horarios->horario_aula );
			}
		}
		int T = somaTempo(horariosAula);
		setTempoDispPorDia(dia, T);
	}
}

/*
	Calcula o tempo total disponivel dada uma lista de horarios de aula, tratando o caso de sobreposicao de horarios.
	A lista de horarios de aula deve estar em ordem crescente de horario de inicio de aula.
*/
int Sala::somaTempo( GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosAula)
{
	int T = 0; // tempo total contido em horariosAula
	if ( horariosAula.size() != 0 ){
				
		// variavel controle, que indica o instante de tempo ate o qual T se refere
		DateTime *atual = new DateTime( horariosAula.begin()->getInicio() );
		
		// (i, f) = (inicio, fim) do proximo horarioAula a ser considerado
		// * = datetime atual
		ITERA_GGROUP_LESSPTR( it, horariosAula, HorarioAula )
		{
			int incremento;

			if ( *atual <= it->getInicio() ){ //  *  i   f
				incremento = it->getTempoAula();
				*atual = it->getFinal();
			}
			else if ( ( *atual > it->getInicio() ) &&
					  ( *atual < it->getFinal() )){  //  i  *  f

					int diferenca = (*atual - it->getInicio()).getDateMinutes();
					incremento = it->getTempoAula() - diferenca;
					*atual = it->getFinal();
			
			}
			else{  //  i  f  *	
					incremento = 0;
			}
			
			T += incremento;
		}
	}
	return T;
}

/*
	Retorna o numero maximo de creditos possivel, dado um dia da semana (dia), 	
	um Calendario (sl) e tipo de combinação de creditos das semanas letivas (id).
*/
int Sala::getNroCredCombinaSL( int k, Calendario *c, int dia )
{
	if ( dia < 0 || dia > 7 )
	{
		std::cerr<<"Erro em Sala::getNroCredCombinaSL(): dia invalido.";
		return 0;
	}

	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.set( dia, k, c );

	return combinaCredSL[ t ];
}


GGroup<HorarioAula*> Sala::retornaHorariosDisponiveisNoDiaPorSL( int dia, Calendario* sl )
{
	GGroup<HorarioAula*> horarios;

	ITERA_GGROUP(it_horarios, this->horarios_disponiveis, Horario)
	{
		if ( ( it_horarios->dias_semana.find(dia) != it_horarios->dias_semana.end() ) &&
			 ( it_horarios->horario_aula->getCalendario() == sl ) )
		{
				horarios.add( it_horarios->horario_aula );
		}
	}

	return horarios;
}

void Sala::setCombinaCredSL( int dia, int k, Calendario* sl , int n )
{
	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.first = dia;
	t.second = k;
	t.third = sl;
	combinaCredSL[t] = n;
}

void Sala::removeCombinaCredSL( Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t )
{
	combinaCredSL.erase(t);
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Sala::combinaCredSL_eh_dominado( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
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
bool Sala::combinaCredSL_domina( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
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
bool Sala::combinaCredSL_eh_repetido( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
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

std::map< Trio<int, int, Calendario*>, int > Sala::retornaCombinaCredSL_Dominados( int dia )
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