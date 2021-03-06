#include "Sala.h"

#include "CentroDados.h"

#include "TipoSala.h"
#include "Horario.h"
#include "Calendario.h"
#include "HorarioDia.h"
#include "Disciplina.h"

using namespace std;

Sala::~Sala()
{
	horarios_disponiveis.deleteElements();
}

void Sala::le_arvore(ItemSala& elem)
{
	this->setId(elem.id());
	codigo = elem.codigo();
	andar = elem.andar();
	numero = elem.numero();
	tipo_sala_id = elem.tipoSalaId();
	capacidade = elem.capacidade();

	ITERA_NSEQ(it_disc, elem.disciplinasAssociadas(), id, Identificador)
	{
		disciplinas_associadas.add(*it_disc);
	}
}

// 'possuiOutputTatico'
// TRUE  -> XML de entrada possui a sa�da do t�tico
// FALSE -> XML de entrada N�O possui a sa�da do t�tico
// 'modo_operacao' = TATICO ou OPERACIONAL
void Sala::construirCreditosHorarios(ItemSala& elem, const string& modo_operacao, bool possuiOutputTatico)
{
	// L� horarios disponiveis
	if (elem.horariosDisponiveis().present())
	{
		ITERA_SEQ(it_hora, elem.horariosDisponiveis().get(), Horario)
		{
			Horario * horario = new Horario();
			horario->le_arvore(*it_hora);
			horarios_disponiveis.add(horario);
		}
	}

}

int Sala::max_creds(int dia)
{
	int tempoDisp = 0;
	int creds = 0;

	ITERA_GGROUP(itSl, this->temposSL, Calendario)
	{
		int duracaoSl = itSl->getTempoAula();
		int tempo = tempoDispPorDiaSL[make_pair(dia, *itSl)];
		if (tempoDisp < tempo)
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

	GGroup< Calendario* >::iterator itSl = temposSL.find(sl);

	if (itSl == this->temposSL.end())
		return creds;

	tempoDisp = tempoDispPorDiaSL[make_pair(dia, sl)];
	creds = tempoDisp / sl->getTempoAula();

	return creds;
}

int Sala::getTempoDispPorDiaSL(int dia, Calendario *sl)
{
	map< pair<int, Calendario*>, int >::iterator it = tempoDispPorDiaSL.find(make_pair(dia, sl));
	if (it != tempoDispPorDiaSL.end())
		return it->second;
	else
		return 0;
}

int Sala::getTempoDispPorDia(int dia)
{
	map<int, int>::iterator it = tempoDispPorDia.find(dia);
	if (it != tempoDispPorDia.end())
		return it->second;
	else
		return 0;
}

void Sala::calculaTemposSL()
{
	ITERA_GGROUP(it_horarios, this->horarios_disponiveis, Horario)
	{
		Calendario* novoCalendario = it_horarios->horario_aula->getCalendario();

		if (temposSL.find(novoCalendario) == temposSL.end())
			temposSL.add(novoCalendario);
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

			GGroup<HorarioAula*, Less<HorarioAula*>> horariosAula;
			ITERA_GGROUP(it_horarios, this->horarios_disponiveis, Horario)
			{
				if (it_horarios->dias_semana.find(dia) != it_horarios->dias_semana.end())
				{
					if (it_horarios->horario_aula->getCalendario()->getId() == sl->getId())
						horariosAula.add(it_horarios->horario_aula);
				}
			}

			int T = somaTempo(horariosAula);
			setTempoDispPorDiaSL(dia, sl, T);
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

		GGroup<HorarioAula*, Less<HorarioAula*>> horariosAula;
		ITERA_GGROUP(it_horarios, this->horarios_disponiveis, Horario)
		{
			if (it_horarios->dias_semana.find(dia) != it_horarios->dias_semana.end())
			{
				horariosAula.add(it_horarios->horario_aula);
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
int Sala::somaTempo(GGroup<HorarioAula*, Less<HorarioAula*>> horariosAula)
{
	int T = 0; // tempo total contido em horariosAula
	if (horariosAula.size() != 0){

		// variavel controle, que indica o instante de tempo ate o qual T se refere
		DateTime *atual = new DateTime(horariosAula.begin()->getInicio());

		// (i, f) = (inicio, fim) do proximo horarioAula a ser considerado
		// * = datetime atual
		ITERA_GGROUP_LESS(it, horariosAula, HorarioAula)
		{
			int incremento;

			if (*atual <= it->getInicio()){ //  *  i   f
				incremento = it->getTempoAula();
				*atual = it->getFinal();
			}
			else if ((*atual > it->getInicio()) &&
				(*atual < it->getFinal())){  //  i  *  f

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
	Calcula o tempo total disponivel dada uma lista de horarios de aula
	*/
int Sala::somaTempo(GGroup<HorarioDia*, Less<HorarioDia*>> horariosDia)
{
	int T = 0; // tempo total contido em horariosAula
	if (horariosDia.size() != 0){

		ITERA_GGROUP_LESS(it, horariosDia, HorarioDia)
		{
			int incremento;
			HorarioAula* ha = it->getHorarioAula();
			incremento = ha->getTempoAula();

			T += incremento;
		}
	}
	return T;
}

/*
	Retorna o numero maximo de creditos possivel, dado um dia da semana (dia),
	um Calendario (sl) e tipo de combina��o de creditos das semanas letivas (id).
	*/
int Sala::getNroCredCombinaSL(int k, Calendario *c, int dia)
{
	if (dia < 0 || dia > 7)
	{
		cerr << "Erro em Sala::getNroCredCombinaSL(): dia invalido.";
		return 0;
	}

	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.set(dia, k, c);

	return combinaCredSL[t];
}


GGroup<HorarioAula*> Sala::retornaHorariosDisponiveisNoDiaPorSL(int dia, Calendario* sl)
{
	GGroup<HorarioAula*> horarios;

	ITERA_GGROUP(it_horarios, this->horarios_disponiveis, Horario)
	{
		if ((it_horarios->dias_semana.find(dia) != it_horarios->dias_semana.end()) &&
			(it_horarios->horario_aula->getCalendario()->getId() == sl->getId()))
		{
			horarios.add(it_horarios->horario_aula);
		}
	}

	return horarios;
}

void Sala::setCombinaCredSL(int dia, int k, Calendario* sl, int n)
{
	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.first = dia;
	t.second = k;
	t.third = sl;
	combinaCredSL[t] = n;
}

void Sala::removeCombinaCredSL(Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t)
{
	combinaCredSL.erase(t);
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Sala::combinaCredSL_eh_dominado(int i, Calendario *sl1, int j, Calendario *sl2, int dia)
{
	map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for (; it_map != combinaCredSL.end(); it_map++)
	{
		if (it_map->first.first == dia &&
			it_map->first.third == sl1 &&
			it_map->second > i)
		{
			int k = it_map->first.second;

			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if (combinaCredSL[t] >= j)
			{
				return true;
			}
		}
		else if (it_map->first.first == dia &&
			it_map->first.third == sl1 &&
			it_map->second == i)
		{
			int k = it_map->first.second;

			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if (combinaCredSL[t] > j)
			{
				return true;
			}
		}
	}

	return false;
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Sala::combinaCredSL_domina(int i, Calendario *sl1, int j, Calendario *sl2, int dia)
{
	map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for (; it_map != combinaCredSL.end(); it_map++)
	{
		if (it_map->first.first == dia &&
			it_map->first.third == sl1 &&
			it_map->second < i)
		{
			int k = it_map->first.second;
			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if (combinaCredSL[t] <= j)
			{
				return true;
			}
		}
		else
		{
			if (it_map->first.first == dia &&
				it_map->first.third == sl1 &&
				it_map->second == i)
			{
				int k = it_map->first.second;
				Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
				t.set(dia, k, sl2);

				if (combinaCredSL[t] < j)
				{
					return true;
				}
			}
		}
	}

	return false;
}


// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Sala::combinaCredSL_eh_repetido(int i, Calendario *sl1, int j, Calendario *sl2, int dia)
{
	map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for (; it_map != combinaCredSL.end(); it_map++)
	{
		if (it_map->first.first == dia &&
			it_map->first.third == sl1 &&
			it_map->second == i)
		{
			int k = it_map->first.second;
			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if (combinaCredSL[t] == j)
			{
				return true;
			}
		}
	}

	return false;
}

map< Trio<int, int, Calendario*>, int > Sala::retornaCombinaCredSL_Dominados(int dia)
{
	map< Trio<int, int, Calendario*>, int > dominados;

	map< Trio< int, int, Calendario* >, int >::iterator it1_map = combinaCredSL.begin();
	for (; it1_map != combinaCredSL.end(); it1_map++)
	{
		if (it1_map->first.first == dia)
		{
			Calendario *sl1 = it1_map->first.third;
			int n1 = it1_map->second;
			int k = it1_map->first.second;

			map< Trio< int, int, Calendario* >, int >::iterator it2_map = combinaCredSL.begin();
			for (; it2_map != combinaCredSL.end(); it2_map++)
			{
				if (it2_map != it1_map &&
					it2_map->first.first == dia &&
					it2_map->first.second == k)
				{
					Calendario *sl2 = it2_map->first.third;
					int n2 = it2_map->second;

					if (combinaCredSL_eh_dominado(n1, sl1, n2, sl2, dia))
					{
						dominados.insert(*it1_map);
						dominados.insert(*it2_map);
					}

				}
			}
		}
	}

	return dominados;
}

void Sala::addHorarioDiaOcupado(GGroup<HorarioDia*> hds)
{
	ITERA_GGROUP(it, hds, HorarioDia)
	{
		horariosDiaOcupados.add(*it);
	}
}

void Sala::addHorarioDiaOcupado(HorarioDia *hd)
{
	horariosDiaOcupados.add(hd);
}

void Sala::deleteHorarioDiaOcupados()
{
	horariosDiaOcupados.clear();
}

bool Sala::sobrepoeHorarioDiaOcupado(HorarioAula *ha, int dia)
{
	ITERA_GGROUP_LESS(itHorDia, horariosDiaOcupados, HorarioDia)
	{
		int diaOcupado = (*itHorDia)->getDia();
		HorarioAula *horAulaOcup = (*itHorDia)->getHorarioAula();

		if (diaOcupado == dia)
		{
			if (horAulaOcup->sobrepoe(*ha))
				return true;
		}
	}
	return false;
}

bool Sala::sobrepoeAulaJaAlocada(HorarioAula *hi, HorarioAula *hf, int dia)
{
	Calendario *calendario = hi->getCalendario();
	int nCreds = calendario->retornaNroCreditosEntreHorarios(hi, hf);
	HorarioAula *ha = hi;

	bool fim = false;
	for (int i = 1; i <= nCreds; i++)
	{
		if (this->sobrepoeHorarioDiaOcupado(ha, dia))
			return true;

		if (ha->getInicio() == hf->getInicio())
			fim = true;
		ha = calendario->getProximoHorario(ha);
	}

	if (!fim)
		cout << "\nErro em Sala::sobrepoeAulaJaAlocada: nro de creditos nao corresponde ao intervalo hi-hf"
		<< "\nhi = " << hi->getId() << " hf = " << hf->getId() << " dia = " << dia << " nCreds = " << nCreds << "\n";

	return false;
}

bool Sala::ehViavelNaSala(Disciplina* disciplina, HorarioAula *hi, HorarioAula *hf, int dia)
{
	if (this->sobrepoeAulaJaAlocada(hi, hf, dia))
		return false;

	int nTotalCreds = disciplina->getTotalCreditos();
	Calendario *calendario = hi->getCalendario();
	int nCreds = calendario->retornaNroCreditosEntreHorarios(hi, hf);

	int creds_restantes = nTotalCreds - nCreds;
	int livre = 0;
	map< int, GGroup<DateTime> > dtLivres;

	// Verifica se os creditos restantes s�o vi�veis nos dias restantes
	for (auto itDia = horariosDiaMap.begin(); itDia != horariosDiaMap.end(); itDia++)
	{
		int diaSala = itDia->first;
		if (diaSala == dia) continue;

		for (auto itHor = itDia->second.begin(); itHor != itDia->second.end(); ++itHor)
		{
			if (disciplina->possuiHorarioDiaOuAnalogo(dia, itHor->getHorarioAula()))
			{
				if (!sobrepoeHorarioDiaOcupado(itHor->getHorarioAula(), diaSala))
				{
					DateTime dt = itHor->getHorarioAula()->getInicio();
					if (dtLivres.find(diaSala) == dtLivres.end())
					{
						dtLivres[diaSala].add(dt);
						livre++;
					}
					else if (dtLivres[diaSala].find(dt) == dtLivres[diaSala].end())
					{
						dtLivres[diaSala].add(dt);
						livre++;
					}
				}
			}
		}
	}

	if (livre >= creds_restantes) return true;

	return false;
}


/*
   Calcula o tempo de aula de cada semana letiva disponivel para cada turno
   */
void Sala::calculaTempoDispPorTurno()
{
	// S� considera calendarios de disciplinas associadas � sala
	GGroup< Calendario*, LessPtr<Calendario> > calendarios;
	ITERA_GGROUP(itDisc, this->disciplinasAssociadas, Disciplina)
	{
		if (itDisc->getCalendarios().size() == 0)
			continue;
		GGroup< Calendario*, Less<Calendario*> > calendsDisc = itDisc->getCalendarios();
		ITERA_GGROUP_LESS(itCalend, calendsDisc, Calendario)
		{
			calendarios.add(*itCalend);
		}
	}

	// Separa os horarios disponiveis por turno e por calendario
	map<int, map<Calendario*, GGroup<HorarioDia*, Less<HorarioDia*>>>> mapTurnoCalendTimes;
	ITERA_GGROUP_LESSPTR(it_sl, calendarios, Calendario) // Para cada semana letiva
	{
		Calendario * sl = *it_sl;
		for (auto it_horarios_dia = horariosDia.begin(); it_horarios_dia != horariosDia.end(); ++it_horarios_dia)
		{
			if (it_horarios_dia->getHorarioAula()->getCalendario() != sl) continue;

			int turno = CentroDados::getFaseDoDia(it_horarios_dia->getHorarioAula()->getInicio());
			mapTurnoCalendTimes[turno][sl].add(*it_horarios_dia);
		}
	}


	// Calcula o menor e maior tempo semanal por turno
	for (auto itTurno = mapTurnoCalendTimes.begin(); itTurno != mapTurnoCalendTimes.end(); itTurno++)
	{
		int turno = itTurno->first;
		pair<int, int> min_max(99999, 0);

		ITERA_GGROUP_LESSPTR(it_sl, calendarios, Calendario) // Para cada semana letiva
		{
			int T = somaTempo(mapTurnoCalendTimes[turno][*it_sl]);
			if (min_max.first > T)
				min_max.first = T;
			if (min_max.second < T)
				min_max.second = T;
		}
		if (min_max != make_pair(99999, 0))
			tempoMinMaxDispPorTurno[turno] = min_max;
		else
			tempoMinMaxDispPorTurno[turno] = make_pair(0, 0);
	}
}

int Sala::getTempoMinSemanal(int turno)
{
	if (tempoMinMaxDispPorTurno.find(turno) != tempoMinMaxDispPorTurno.end())
		return tempoMinMaxDispPorTurno[turno].first;
	else
		return 0;
}

int Sala::getTempoMaxSemanal(int turno)
{
	if (tempoMinMaxDispPorTurno.find(turno) != tempoMinMaxDispPorTurno.end())
		return tempoMinMaxDispPorTurno[turno].second;
	else
		return 0;
}



int Sala::getMaxCredsPossiveis(int dia, Disciplina* disciplina)
{
	map<Calendario*, GGroup<HorarioDia*, LessPtr<HorarioDia>>, Less<Calendario*>> mapHorsNoDiaPorCalend;

	auto itDia = horariosDiaMap.find(dia);
	if (itDia != horariosDiaMap.end())
		for (auto itHor = itDia->second.begin(); itHor != itDia->second.end(); ++itHor)
			if (disciplina->possuiHorarioDia(dia, itHor->getHorarioAula()))
				mapHorsNoDiaPorCalend[itHor->getHorarioAula()->getCalendario()].add(*itHor);

	int max = 0;
	for (auto it = mapHorsNoDiaPorCalend.begin(); it != mapHorsNoDiaPorCalend.end(); it++)
		if (max < (int)it->second.size())
			max = (int)it->second.size();

	return max;
}


map<int, map<DateTime /*dti*/, map<DateTime /*dtf*/, GGroup<HorarioAula*, Less<HorarioAula*>>>>> Sala::retornaHorariosAulaVagos()
{
	map<int, map<DateTime /*dti*/, map<DateTime /*dtf*/, GGroup<HorarioAula*, Less<HorarioAula*>>>>> horariosVagos;

	for (auto it_dia_horarios = horariosDiaMap.begin(); it_dia_horarios != horariosDiaMap.end(); it_dia_horarios++)
	{
		for (auto diaLivre = it_dia_horarios->second.begin(); diaLivre != it_dia_horarios->second.end(); ++diaLivre)
		{
			DateTime inicioLivre = diaLivre->getHorarioAula()->getInicio();
			DateTime finalLivre = diaLivre->getHorarioAula()->getFinal();
			bool livre = true;
			ITERA_GGROUP_LESS(diaOcupado, horariosDiaOcupados, HorarioDia)
			{
				if (diaLivre->getDia() == diaOcupado->getDia())
				{
					DateTime inicioOcupado = diaOcupado->getHorarioAula()->getInicio();
					DateTime finalOcupado = diaOcupado->getHorarioAula()->getFinal();
					if ((inicioLivre >= inicioOcupado && inicioLivre < finalOcupado)
						|| (inicioLivre <= inicioOcupado && finalLivre > inicioOcupado))
					{
						livre = false;
						break;
					}
				}
			}
			if (livre)
			{
				horariosVagos[diaLivre->getDia()][inicioLivre][finalLivre].add(diaLivre->getHorarioAula());
			}
		}
	}
	return horariosVagos;
}


bool Sala::possuiHorariosNoDia(HorarioAula *hi, HorarioAula *hf, int dia)
{
	auto itDia = mapDiaDtiDtf.find(dia);

	if (itDia != mapDiaDtiDtf.end())
	{
		HorarioAula *h = hi;

		Calendario *calendario = h->getCalendario();

		while (h != NULL)
		{
			DateTime dti = h->getInicio();
			DateTime dtf = h->getFinal();

			auto *mapDti = &itDia->second;
			auto itHi = mapDti->find(dti);

			if (itHi != mapDti->end())
			{
				auto itHf = itHi->second.find(dtf);

				if (itHf != itHi->second.end())
				{
					if (h->inicioFimIguais(*hf))
						return true;

					h = calendario->getProximoHorario(h);
				}
				else return false;
			}
			else return false;
		}
	}

	return false;
}

bool Sala::ehLab() const
{
	return tipo_sala->getAceitaAulaPratica();
}
