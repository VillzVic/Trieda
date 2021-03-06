#include "Aluno.h"

#include "CentroDados.h"
#include "ProblemData.h"

using namespace std;

ostream & operator<<(ostream & out, Aluno & aluno)
{
	out << "<Aluno>" << endl;

	out << "<alunoId>" << aluno.getAlunoId()
		<< "</alunoId>" << endl;

	out << "<nomeAluno>" << aluno.getNomeAluno()
		<< "</nomeAluno>" << endl;

	//out << "<ofertaId>" << aluno.getOfertaId()
	//    << "</ofertaId>" << endl;

	out << "<Demandas>" << endl;
	ITERA_GGROUP_LESS(itAlDemandas, aluno.demandas, AlunoDemanda)
	{
		out << "<demandaId>" << itAlDemandas->demanda->getId()
			<< "</demandaId>" << endl;
	}
	out << "</Demandas>" << endl;

	out << "</AlunoDemanda>" << endl;

	return out;
}

Aluno::Aluno(void)
	: nrCredsReqP1(0), nrCredsMedioDia_(-1), turnoPrincipal_(nullptr)
{
	this->setAlunoId(-1);
	this->oferta = NULL;
	this->ofertaId = -1;
	this->nCredsAlocados.clear();
	this->formando = false;
	this->calouro = false;
	this->cargaHorariaOrigRequeridaP1 = 0;
	this->nroCredsOrigRequeridosP1 = 0;
	this->nCredsJaAlocados = 0;
	this->nroDiscsOrigRequeridosP1 = 0;
	this->prioridadeDoAluno = 0;
}

Aluno::Aluno(int id, string nome, bool formando, Oferta* oft)
	: nrCredsReqP1(0), nrCredsMedioDia_(-1), turnoPrincipal_(nullptr)
{
	this->setAlunoId(id);
	this->setNomeAluno(nome);
	this->formando = formando;
	this->calouro = false;
	this->oferta = oft;
	this->ofertaId = oft->getId();
	this->nCredsAlocados.clear();
	this->cargaHorariaOrigRequeridaP1 = 0;
	this->nroCredsOrigRequeridosP1 = 0;
	this->nCredsJaAlocados = 0;
	this->nroDiscsOrigRequeridosP1 = 0;
	this->prioridadeDoAluno = 0;
}

Aluno::~Aluno(void)
{
}

void Aluno::le_arvore(ItemAluno & elem)
{
	this->setAlunoId(elem.alunoId());
	this->setNomeAluno(elem.nomeAluno());
	this->setFormando(elem.formando());
	this->setPrioridadeDoAluno(elem.prioridade());
}

void Aluno::setNrMedioCredsDia()
{
	double nrCredsPorDia = 0;
	for (auto itAlDem = demandas.begin(); itAlDem != demandas.end(); itAlDem++)
	{
		Demanda *demanda = itAlDem->demanda;

		int nrCreds = demanda->disciplina->getTotalCreditos();
		int nrDias = demanda->disciplina->diasLetivos.size();

		double mediaPorDia = (double)nrCreds / (double)nrDias;
		nrCredsPorDia += mediaPorDia;
	}

	nrCredsMedioDia_ = (int)nrCredsPorDia;
}

int Aluno::getNrMedioCredsDia()
{
	if (nrCredsMedioDia_ == -1)
		setNrMedioCredsDia();
	return nrCredsMedioDia_;
}

bool Aluno::possuiEquivForcada()
{
	auto it = this->demandas.begin();
	for (; it != this->demandas.end(); it++)
	{
		if (it->getExigeEquivalenciaForcada())
			return true;
	}
	return false;
}

double Aluno::getReceita(Disciplina *disciplina)
{
	AlunoDemanda *al = this->getAlunoDemandaEquiv(disciplina);
	if (al == NULL)
	{
		cout << "\nErro: AlunoDemanda nao encontrado para disciplina "
			<< disciplina->getId() << " e aluno " << this->getAlunoId();
		return 0;
	}
	return al->demanda->oferta->getReceita();
}

bool Aluno::demandaDisciplina(int idDisc)
{
	ITERA_GGROUP_LESS(itAlunoDemanda, this->demandas, AlunoDemanda)
	{
		if ((*itAlunoDemanda)->demanda->getDisciplinaId() == idDisc)
		{
			return true;
		}
	}
	return false;
}

AlunoDemanda* Aluno::getAlunoDemanda(int disciplinaId)
{
	ITERA_GGROUP_LESS(itAlunoDemanda, this->demandas, AlunoDemanda)
	{
		if ((*itAlunoDemanda)->demanda->getDisciplinaId() == disciplinaId)
		{
			return (*itAlunoDemanda);
		}
	}
	return NULL;
}

AlunoDemanda* Aluno::getAlunoDemandaEquiv(Disciplina *disciplina)
{
	ITERA_GGROUP_LESS(itAlunoDemanda, this->demandas, AlunoDemanda)
	{
		if ((*itAlunoDemanda)->demanda->getDisciplinaId() == disciplina->getId())
		{
			return (*itAlunoDemanda);
		}
	}
	ITERA_GGROUP_LESS(itAlunoDemanda, this->demandas, AlunoDemanda)
	{
		if (CentroDados::getProblemData()->alocacaoEquivViavel((*itAlunoDemanda)->demanda, disciplina))
			//if ( (*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas.find( disciplina )!=
			//	 (*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas.end() )
		{
			AlunoDemanda* alDemTeor = (*itAlunoDemanda);
			int idOrig = alDemTeor->demanda->disciplina->getId();

			if (disciplina->getId() > 0)
				return alDemTeor;
			else
			{
				AlunoDemanda* alDemPrat = getAlunoDemanda(-idOrig);
				if (alDemPrat != NULL)
					return alDemPrat;
				else
					return alDemTeor;
			}
		}
	}
	return NULL;
}

// olha para a estrutura demandasP2
AlunoDemanda* Aluno::getAlunoDemandaEquivP2(Disciplina *disciplina)
{
	for (auto itAlunoDemanda = demandasP2.cbegin(); itAlunoDemanda != demandasP2.cend(); ++itAlunoDemanda)
	{
		if ((*itAlunoDemanda)->demanda->getDisciplinaId() == disciplina->getId())
		{
			return (*itAlunoDemanda);
		}
	}
	for (auto itAlunoDemanda = demandasP2.cbegin(); itAlunoDemanda != demandasP2.cend(); ++itAlunoDemanda)
	{
		if ((*itAlunoDemanda)->demanda->discIdSubstitutasPossiveis.find(disciplina->getId()) !=
			(*itAlunoDemanda)->demanda->discIdSubstitutasPossiveis.end())
		{
			AlunoDemanda* alDemTeor = (*itAlunoDemanda);
			int idOrig = alDemTeor->demanda->disciplina->getId();

			if (disciplina->getId() > 0)
				return alDemTeor;
			else
			{
				AlunoDemanda* alDemPrat = getAlunoDemanda(-idOrig);
				if (alDemPrat != NULL)
					return alDemPrat;
				else
					return alDemTeor;
			}
		}
	}

	return nullptr;
}

/*
	Retorna o numero maximo de creditos possivel, dado um dia da semana (dia),
	um Calendario (sl) e tipo de combina��o de creditos das semanas letivas (id).
	*/
int Aluno::getNroMaxCredCombinaSL(int k, Calendario *c, int dia)
{
	int n = 0;

	if (dia < 0 || dia > 7)
	{
		cerr << "Erro em Aluno::getNroCredCombinaSL(): dia invalido.";
		return 0;
	}

	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.set(dia, k, c);

	map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator
		it = combinaCredSL.find(t);

	if (it != combinaCredSL.end())
	{
		n = it->second;
	}

	return n;
}


double Aluno::getNroCreditosJaAlocados(Calendario* c, int dia)
{
	double tempo = 0.0;

	map< Calendario*, map< int /*dia*/, double /*nCreds*/> >::iterator
		itMap1 = nCredsAlocados.find(c);
	if (itMap1 != nCredsAlocados.end())
	{
		map< int /*dia*/, double /*nCreds*/>::iterator
			itMap2 = itMap1->second.find(dia);
		if (itMap2 != itMap1->second.end())
			tempo += itMap2->second;
	}
	return tempo;
}

bool Aluno::totalmenteAtendido()
{
	if (this->getNroCreditosJaAlocados() >= this->getNroCredsOrigRequeridosP1())
		return true;
	return false;
}

int Aluno::getNroCreditosNaoAtendidos()
{
	return this->getNroCredsOrigRequeridosP1() - this->getNroCreditosJaAlocados();
}


/*
	Retorna todos os calendarios associados �s demandas do aluno.
	Aten��o: em geral, o aluno � alocado somente em disciplinas com
	o mesmo calend�rio de seu curriculo. Por�m, exce��es ocorrem em casos
	de equival�ncia. Nesses casos, um aluno pode ser associado a um
	calend�rio diferente desde que este tenha hor�rios em comum com o
	calend�rio de seu curriculo.
	*/
GGroup< Calendario*, Less<Calendario*> > Aluno::retornaSemanasLetivas()
{
	GGroup< Calendario*, Less<Calendario*> > calendarios;

	Calendario *slAluno = this->oferta->curriculo->calendario;
	calendarios.add(slAluno);

	// Insere possiveis demais calendarios necessarios
	ITERA_GGROUP_LESS(itAlunoDemanda, this->demandas, AlunoDemanda)
	{
		if ((*itAlunoDemanda)->demanda->disciplina->getCalendarios().find(slAluno) ==
			(*itAlunoDemanda)->demanda->disciplina->getCalendarios().end())
		{
			// seleciona somente os calendarios de maior interse��o de horarios com o calendario do curriculo do aluno
			GGroup< Calendario*, Less<Calendario*> > aux_calend;
			int max = 0;

			GGroup< Calendario*, Less<Calendario*> > calendsDisc = (*itAlunoDemanda)->demanda->disciplina->getCalendarios();
			ITERA_GGROUP_LESS(itCalend, calendsDisc, Calendario)
			{
				int n = 0;
				map< int, GGroup<HorarioAula*, Less<HorarioAula*>> > dia_hors_comum =
					itCalend->retornaDiaHorariosEmComum((*itAlunoDemanda)->demanda->disciplina->getCalendarios());

				map< int, GGroup<HorarioAula*, Less<HorarioAula*>> >::iterator itMapDiaHors = dia_hors_comum.begin();
				for (; itMapDiaHors != dia_hors_comum.end(); itMapDiaHors++)
					n += itMapDiaHors->second.size();

				if (n > max || (n == max && n != 0))
				{
					if (n > max)
					{
						max = n;
						aux_calend.clear();
					}
					aux_calend.add(*itCalend);
				}
			}

			ITERA_GGROUP_LESS(itCalend, aux_calend, Calendario)
				calendarios.add(*itCalend);
		}
	}

	return calendarios;
}

/*	-------------------------------------------------------------------------------------------
	FUN��ES PARA CRIAR A REGRA DE COMBINA��O DOS CREDITOS DO ALUNO
	*/


void Aluno::setCombinaCredSL(int dia, int k, Calendario* sl, int n)
{
	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.first = dia;
	t.second = k;
	t.third = sl;
	combinaCredSL[t] = n;
}

void Aluno::removeCombinaCredSL(Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t)
{
	combinaCredSL.erase(t);
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Aluno::combinaCredSL_eh_dominado(int i, Calendario *sl1, int j, Calendario *sl2, int dia)
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
bool Aluno::combinaCredSL_domina(int i, Calendario *sl1, int j, Calendario *sl2, int dia)
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
bool Aluno::combinaCredSL_eh_repetido(int i, Calendario *sl1, int j, Calendario *sl2, int dia)
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

map< Trio<int, int, Calendario*>, int > Aluno::retornaCombinaCredSL_Dominados(int dia)
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

//	-------------------------------------------------------------------------------------------

bool Aluno::sobrepoeHorarioDiaOcupado(HorarioDia *hd)
{
	map< int /*dia*/, map< DateTime, HorarioDia*> >::iterator
		itDia = horariosDiaOcupados.find(hd->getDia());
	if (itDia != horariosDiaOcupados.end())
	{
		map< DateTime, HorarioDia*> *horsDia = &itDia->second;
		map< DateTime, HorarioDia*>::iterator itDt = horsDia->begin();
		for (; itDt != horsDia->end(); itDt++)
		{
			HorarioAula *horAulaOcup = itDt->second->getHorarioAula();

			if (horAulaOcup->getInicio() > hd->getHorarioAula()->getFinal())
				return false;

			if (horAulaOcup->sobrepoe(*hd->getHorarioAula()))
				return true;
		}
	}
	return false;
}

bool Aluno::sobrepoeHorarioDiaOcupado(HorarioAula *ha, int dia)
{
	map< int /*dia*/, map< DateTime, HorarioDia*> >::iterator
		itDia = horariosDiaOcupados.find(dia);
	if (itDia != horariosDiaOcupados.end())
	{
		map< DateTime, HorarioDia*> *horsDia = &itDia->second;
		map< DateTime, HorarioDia*>::iterator itDt = horsDia->begin();
		for (; itDt != horsDia->end(); itDt++)
		{
			HorarioAula *horAulaOcup = itDt->second->getHorarioAula();

			if (horAulaOcup->getInicio() > ha->getFinal())
				return false;

			if (horAulaOcup->sobrepoe(*ha))
				return true;
		}
	}
	return false;
}

void Aluno::addHorarioDiaOcupado(HorarioDia* hd)
{
	horariosDiaOcupados[hd->getDia()];
	map< DateTime, HorarioDia*> *hors = &horariosDiaOcupados[hd->getDia()];
	if (hors->find(hd->getHorarioAula()->getInicio()) == hors->end())
	{
		(*hors)[hd->getHorarioAula()->getInicio()] = hd;
		nCredsJaAlocados++;
	}
}

void Aluno::addHorarioDiaOcupado(GGroup<HorarioDia*> hds)
{
	ITERA_GGROUP(it, hds, HorarioDia)
	{
		int dia = it->getDia();
		map< DateTime, HorarioDia*> *mapDt = &horariosDiaOcupados[dia];
		DateTime dt = it->getHorarioAula()->getInicio();

		if (mapDt->find(dt) == mapDt->end())
		{
			(*mapDt)[dt] = *it;
			nCredsJaAlocados++;
		}
	}
}

void Aluno::removeHorarioDiaOcupado(HorarioDia* hd)
{
	map< int /*dia*/, map< DateTime, HorarioDia*> >::iterator
		itDia = horariosDiaOcupados.find(hd->getDia());
	if (itDia != horariosDiaOcupados.end())
	{
		map< DateTime, HorarioDia*>::iterator
			itDt = itDia->second.find(hd->getHorarioAula()->getInicio());
		if (itDt != itDia->second.end())
		{
			itDia->second.erase(hd->getHorarioAula()->getInicio());
			nCredsJaAlocados--;
		}
	}
}

bool Aluno::sobrepoeAulaJaAlocada(HorarioAula *hi, HorarioAula *hf, int dia)
{
	Calendario *calendario = hi->getCalendario();
	int nCreds = calendario->retornaNroCreditosEntreHorarios(hi, hf);
	HorarioAula *ha = hi;

	bool fim = false;
	for (int i = 1; i <= nCreds; i++)
	{
		if (this->sobrepoeHorarioDiaOcupado(ha, dia))
			return true;

		if (ha == hf)
			fim = true;
		ha = calendario->getProximoHorario(ha);
	}

	if (!fim)
		cout << "\nErro em Aluno::sobrepoeAulaJaAlocada: nro de creditos nao corresponde ao intervalo hi-hf"
		<< "\nhi = " << hi->getId() << " hf = " << hf->getId() << " dia = " << dia << " nCreds = " << nCreds << endl;

	return false;
}

void Aluno::defineSeEhCalouro()
{
	bool calourinho = true;

	ITERA_GGROUP_LESS(itAlunoDemanda, demandas, AlunoDemanda)
	{
		Curriculo *curr = (*itAlunoDemanda)->demanda->oferta->curriculo;
		Disciplina *disciplina = (*itAlunoDemanda)->demanda->disciplina;

		int periodo = curr->getPeriodo(disciplina);
		if (periodo != 1)
		{
			calourinho = false;
			break;
		}
	}

	if (this->demandas.size() == 0)
		calourinho = false;

	this->setCalouro(calourinho);
}

string Aluno::getHorariosOcupadosStr(int dia)
{
	stringstream ss;
	ss.clear();
	map< int /*dia*/, map< DateTime, HorarioDia*> >::iterator
		itDia = horariosDiaOcupados.find(dia);
	if (itDia != horariosDiaOcupados.end())
	{
		map< DateTime, HorarioDia*>::iterator itMapDt = itDia->second.begin();
		for (; itMapDt != itDia->second.end(); itMapDt++)
		{
			DateTime dti = itMapDt->second->getHorarioAula()->getInicio();
			DateTime dtf = itMapDt->second->getHorarioAula()->getFinal();

			ss << dti.getHour() << ":" << dti.getMinute()
				<< " - "
				<< dtf.getHour() << ":" << dtf.getMinute();
			ss << " || ";
		}
	}
	return ss.str();
}

void Aluno::setTurnoPrincipal()
{
	unordered_map<TurnoIES*, int> turnoNrDem;
	ITERA_GGROUP_LESS(itAlDem, this->demandas, AlunoDemanda)
	{
		TurnoIES* t = (*itAlDem)->demanda->getTurnoIES();

		auto finder = turnoNrDem.find(t);
		if (finder == turnoNrDem.end())
		{
			finder = turnoNrDem.insert(pair<TurnoIES*, int>(t, 0)).first;
		}
		(finder->second)++;
	}

	int nrMaxDem = 0;
	TurnoIES* turnoMaxDem = nullptr;
	for (auto it = turnoNrDem.cbegin(); it != turnoNrDem.cend(); it++)
	{
		if (it->second > nrMaxDem)
		{
			nrMaxDem = it->second;
			turnoMaxDem = it->first;
		}
	}
	turnoPrincipal_ = turnoMaxDem;
}

TurnoIES* Aluno::getTurnoPrinc() const
{
	return turnoPrincipal_;
}

bool Aluno::estaEmContraTurno(Disciplina* const disciplina)
{
	AlunoDemanda* alDem = this->getAlunoDemanda(disciplina->getId());
	if (alDem)
	{
		if (turnoPrincipal_ != alDem->demanda->getTurnoIES())
			return true;
	}
	return false;
}

bool Aluno::ehContraTurno(TurnoIES* turno)
{
	return (turno == turnoPrincipal_);
}

bool Aluno::get31Tempo(DateTime &dt)
{
	if (!this->possuiEquivForcada()) return false; // nao eh caso de marreta
	if (this->ehFormando()) return false;			 // nao eh caso de marreta 1

	for (auto itAlDem = demandas.begin(); itAlDem != demandas.end(); itAlDem++)
	{
		if ((*itAlDem)->getExigeEquivalenciaForcada())
		{
			if (turnoPrincipal_->get31Tempo(itAlDem->demanda->getCalendario(), dt))
				return true;
		}
	}
	CentroDados::printError("Aluno::get31Tempo", "31o DateTime nao encontrado!!!");
	return false;
}

bool Aluno::chEhIgualDisponibSemanaLetiva(bool ignoraContraTurno)
{
	// M�todo usado no caso de escola.

	unordered_set<Calendario*> calends;
	int disponib = 0;
	int ch = 0;
	for (auto itAlDem = demandas.begin(); itAlDem != demandas.end(); itAlDem++)
	{
		if (!ehContraTurno(itAlDem->demanda->getTurnoIES()))
		{
			ch += itAlDem->demanda->disciplina->getTotalCreditos();
			calends.insert(itAlDem->demanda->getCalendario());
		}
	}

	if (calends.size() > 1)
		CentroDados::printWarning("Aluno::chEhIgualDisponibSemanaLetiva()",
		"Mais de um calendario para o aluno. Isso complica o calculo da disponibilidade do aluno.");

	if (calends.size() > 0)
	{
		Calendario* const c = *calends.begin();

		for (auto itHor = c->horarios_aula.begin(); itHor != c->horarios_aula.end(); itHor++)
		{
			if (itHor->getTurnoIESId() == turnoPrincipal_->getId())
			{
				disponib++;
			}
		}
	}

	bool ehIgual = (disponib == ch);

	return ehIgual;
}

void Aluno::fillCampusIds()
{
	ITERA_GGROUP_LESS(it, demandas, AlunoDemanda)
		campusIds.add(it->getOferta()->getCampusId());
}

Oferta* Aluno::getOferta(Demanda* demanda) const
{
	return demanda->oferta;
}

int Aluno::getOfertaId(Demanda* demanda) const
{
	return demanda->getOfertaId();
}

Calendario* Aluno::getCalendario(Demanda* demanda) const
{
	return demanda->oferta->curriculo->calendario;
}

Calendario* Aluno::getCalendario() const
{
	return (oferta != nullptr ? oferta->curriculo->calendario : nullptr);
}

Curso* Aluno::getCurso() const
{
	return (oferta != nullptr ? oferta->curso : nullptr);
}
