#include "ProblemSolution.h"

#include "ErrorHandler.h"
#include "NaoAtendimento.h"
#include "GGroup.h"
#include "InputMethods.h"

#include "Indicadores.h"
#include "CentroDados.h"
#include "ProblemData.h"
#include "AtendimentoHorarioAula.h"
#include "AlocacaoProfVirtual.h"
#include "AlunoSolution.h"
#include "RestricaoViolada.h"
#include "ProfessorVirtualOutput.h"

using namespace std;

ProblemSolution::ProblemSolution(bool _modoOtmTatico) : modoOtmTatico(_modoOtmTatico)
{
	folgas = new RestricaoVioladaGroup();
	atendimento_campus = new GGroup<AtendimentoCampus*>();
	professores_virtuais = new GGroup<ProfessorVirtualOutput*>();
	alunosDemanda = new GGroup<AlunoDemanda*, Less<AlunoDemanda*>>();
	nao_atendimentos = new GGroup<NaoAtendimento*, Less<NaoAtendimento*>>();
	idsAtendimentos = 1;
	cenarioId = 0;

	nroTotalAlunoDemandaP1 = 0;
	nroAlunoDemAtendP1 = 0;
	nroAlunoDemNaoAtendP1 = 0;
	nroAlunoDemAtendP2 = 0;
	nroAlunoDemAtendP1P2 = 0;
	nrMaxDiscSimult_ = 0;
}

ProblemSolution::~ProblemSolution()
{
	clearMapsDaSolucao();

	if (folgas)
	{
		folgas->deleteElements();
		delete folgas;
	}

	if (atendimento_campus)
	{
		atendimento_campus->deleteElements();
		delete atendimento_campus;
	}

	if (professores_virtuais)
	{
		professores_virtuais->deleteElements();
		delete professores_virtuais;
	}

	// alunosDemanda: dados de ProblemData, n�o deletar.

	if (nao_atendimentos)
	{
		nao_atendimentos->deleteElements();
		delete nao_atendimentos;
	}
}

void ProblemSolution::resetProblemSolution()
{
	if (folgas != NULL)
	{
		folgas->deleteElements();
		delete folgas;
	}
	if (atendimento_campus != NULL)
	{
		atendimento_campus->deleteElements();
		delete atendimento_campus;
	}
	if (professores_virtuais != NULL)
	{
		professores_virtuais->deleteElements();
		delete professores_virtuais;
	}
	if (alunosDemanda != NULL)
	{
		// alunosDemanda: dados de ProblemData, n�o deletar.
		delete alunosDemanda;
	}
	if (nao_atendimentos != NULL)
	{
		nao_atendimentos->deleteElements();
		delete nao_atendimentos;
	}

	folgas = new RestricaoVioladaGroup();
	atendimento_campus = new GGroup<AtendimentoCampus*>();
	professores_virtuais = new GGroup<ProfessorVirtualOutput*>();
	alunosDemanda = new GGroup<AlunoDemanda*, Less<AlunoDemanda*>>();
	nao_atendimentos = new GGroup<NaoAtendimento*, Less<NaoAtendimento*>>();
	idsAtendimentos = 1;
}

// procura o atendimento turno com um determinado id. se n�o encontra cria um novo e retorna-o
AtendimentoCampus* ProblemSolution::getAddAtendCampus(int id_cp)
{
	auto itAtCp = atendimento_campus->begin();
	for (; itAtCp != atendimento_campus->end(); ++itAtCp)
	{
		if (itAtCp->campus->getId() == id_cp)
			return *itAtCp;
	}

	AtendimentoCampus* atendCp = new AtendimentoCampus(id_cp);
	atendimento_campus->add(atendCp);

	return atendCp;
}

ProfessorVirtualOutput* ProblemSolution::getProfVirtualOutput(int id)
{
	ProfessorVirtualOutput *pvo = nullptr;

	auto itFinder = this->professores_virtuais->begin();
	for (; itFinder != this->professores_virtuais->end(); itFinder++)
	{
		if (itFinder->getId() == id)
		{
			pvo = *itFinder;
			break;
		}
	}

	return pvo;
}

map<Sala*, vector<map<int, GGroup<HorarioAula*, Less<HorarioAula*>>>>, Less<Sala*>>
ProblemSolution::procuraCombinacaoLivreEmSalas(Disciplina *disciplina, TurnoIES* turno, int campusId)
{
	map<Sala*, vector<map<int, GGroup<HorarioAula*, Less<HorarioAula*>>>>, Less<Sala*>> mapSalaCombinacoesLivres;
	for (auto it = disciplina->cjtSalasAssociados.begin(); it != disciplina->cjtSalasAssociados.end(); it++)
	{
		Sala *sala = (*it).second->getSala();
		if (sala->getIdCampus() != campusId) continue;

		vector<map<int, GGroup<HorarioAula*, Less<HorarioAula*>>>> opcoesLivresNaSala = procuraCombinacaoLivreNaSala(disciplina, turno, sala);
		if (opcoesLivresNaSala.size() != 0)
			mapSalaCombinacoesLivres[sala] = opcoesLivresNaSala;
	}

	return mapSalaCombinacoesLivres;
}

vector<map<int, GGroup<HorarioAula*, Less<HorarioAula*>>>>
ProblemSolution::procuraCombinacaoLivreNaSala(Disciplina *disciplina, TurnoIES* turno, Sala *sala)
{
	bool haSalaDisponivel = false;
	vector<map<int, GGroup<HorarioAula*, Less<HorarioAula*>>>> mapCombinacoesLivres;

	map<int, map<DateTime, map<DateTime, GGroup<HorarioAula*, Less<HorarioAula*>>>>>* map_dia_horarios_vagos = &mapSalaDiaHorariosVagos_[sala];

	auto it_combinacao_creditos = disciplina->combinacao_divisao_creditos.begin();
	for (; it_combinacao_creditos != disciplina->combinacao_divisao_creditos.end(); it_combinacao_creditos++)
	{
		bool combinacaoOK = true;
		map<int /*dia*/, GGroup<HorarioAula*, Less<HorarioAula*>>> opcaoValida;

		auto it_creditos = (*it_combinacao_creditos).begin();
		for (; it_creditos != (*it_combinacao_creditos).end(); it_creditos++)
		{
			int dia_credito = (*it_creditos).first;
			int n_credito = (*it_creditos).second;
			bool blocoCred = false;

			if (n_credito > 0)
			{
				// PERCORRE HORARIOS LIVRES DA SALA NO DIA

				auto itMap1 = map_dia_horarios_vagos->find(dia_credito);
				if (itMap1 != map_dia_horarios_vagos->end())
				{
					auto itMapDti = itMap1->second.begin();
					for (; itMapDti != itMap1->second.end(); itMapDti++)
					{
						DateTime dti = itMapDti->first;
						auto itMapDtf = itMapDti->second.begin();
						for (; itMapDtf != itMapDti->second.end(); itMapDtf++)
						{
							DateTime dtf = itMapDtf->first;
							HorarioAula *horarioAula = *(itMapDtf->second.begin());

							GGroup<HorarioAula*, Less<HorarioAula*>> horsValidosAPartirDeHi;

							if (turno->possuiHorarioDiaOuCorrespondente(horarioAula, dia_credito))
								horsValidosAPartirDeHi.add(horarioAula);
							else
								continue;

							bool hInicialComNCreditosOK = true;

#pragma region CONFERE SE OS N HOR�RIOS DO BLOCO S�O V�LIDOS NO DIA NOS HOR�RIOS LIVRES DA SALA E NO TURNO
							HorarioAula* h = horarioAula;
							for (int i = 2; i <= n_credito && hInicialComNCreditosOK; i++)
							{
								h = h->getCalendario()->getProximoHorario(h);

								if (h == NULL)
								{
									hInicialComNCreditosOK = false;
									break;
								}

								DateTime dti_next = h->getInicio();
								DateTime dtf_next = h->getFinal();

								// CONFERE SE O HORARIO-DIA EST� DISPONIVEL NA SALA
								bool achouNaSala = false;
								for (auto itMapDtiProx = itMapDti; itMapDtiProx != itMap1->second.end(); itMapDtiProx++)
								{
									if (itMapDtiProx->first > dti_next) break;
									if (itMapDtiProx->first == dti_next)
									{
										for (auto itMapDtfProx = itMapDtiProx->second.begin(); itMapDtfProx != itMapDtiProx->second.end(); itMapDtfProx++)
										{
											if (itMapDtfProx->first > dtf_next) break;
											if (itMapDtfProx->first == dtf_next)
											{
												achouNaSala = true;
												break;
											}
										}
									}
								}

								if (!achouNaSala) hInicialComNCreditosOK = false;

								// CONFERE SE O HORARIO-DIA EST� DISPONIVEL NO TURNO
								if (hInicialComNCreditosOK)
									if (turno->possuiHorarioDiaOuCorrespondente(h, dia_credito))
										horsValidosAPartirDeHi.add(horarioAula);
									else
										hInicialComNCreditosOK = false;
							}
#pragma endregion

							// SE OS HORARIOS-DIAS EST�O DISPONIVEIS NA SALA E NO TURNO, CONFERE SE � UMA AULA V�LIDA NA DISCIPLINA
							if (hInicialComNCreditosOK)
								if (disciplina->inicioTerminoValidos(horarioAula, h, dia_credito))
								{
									blocoCred = true;
									opcaoValida[dia_credito].add(horsValidosAPartirDeHi);
								}
						}
					}
				}
			}
			else blocoCred = true;

			if (!blocoCred)
			{
				combinacaoOK = false;
				break;
			}
		}
		if (combinacaoOK)
		{
			mapCombinacoesLivres.push_back(opcaoValida);
			haSalaDisponivel = true;
		}
	}

	return mapCombinacoesLivres;
}

void ProblemSolution::procuraOpcoesSemChoque(
	const map<int /*opcao*/, map<int /*dia*/, GGroup<HorarioAula*, Less<HorarioAula*>>>> &semChoquesNaSala,
	const map<TurnoIES*, GGroup<AlunoDemanda*, Less<AlunoDemanda*>>, Less<TurnoIES*>> &mapAlsDemNaoAtend,
	map<int /*op��o*/, GGroup<AlunoDemanda*, Less<AlunoDemanda*>>> &mapOpcaoAlunosSemChoque)
{
	// Percorre as op��es de conjuntos de hor�rios
	for (auto itOpcao = semChoquesNaSala.begin(); itOpcao != semChoquesNaSala.end(); itOpcao++)
	{
		int opcaoIdx = itOpcao->first;

		// Percorre Alunos n�o atendidos na disciplina
		auto itTurnoNaoAtend = mapAlsDemNaoAtend.begin();
		for (; itTurnoNaoAtend != mapAlsDemNaoAtend.end(); itTurnoNaoAtend++)
		{
			ITERA_GGROUP_LESS(itAlDemNaoAtend, itTurnoNaoAtend->second, AlunoDemanda)
			{
				AlunoDemanda *alDem = *itAlDemNaoAtend;
				Aluno *aluno = alDem->getAluno();

				bool semChoque = true;

				AlunoSolution *alunoSol = this->getAlunoSolution(aluno);
				if (alunoSol != nullptr)
				{
					// Percorre os horarios da op��o corrente para verificar se o aluno tem choque em suas aloca��es 

					for (auto itDiaOpcao = itOpcao->second.begin();
						itDiaOpcao != itOpcao->second.end() && semChoque; itDiaOpcao++)
					{
						int dia = itDiaOpcao->first;

						map < DateTime /*dti*/, map< DateTime /*dtf*/, map< Disciplina*, Aula*,
							LessPtr<Disciplina> > > > *mapOcupacaoNoDia = alunoSol->getMapAulasNoDia(dia);

						if (mapOcupacaoNoDia == nullptr)
							continue;

						ITERA_GGROUP_LESS(itHorOpcao, itDiaOpcao->second, HorarioAula)
						{
							HorarioAula *h_op = *itHorOpcao;
							DateTime dti_op = h_op->getInicio();
							DateTime dtf_op = h_op->getFinal();

							auto itDtiOcup = mapOcupacaoNoDia->begin();
							for (; itDtiOcup != mapOcupacaoNoDia->end() && semChoque; itDtiOcup++)
							{
								DateTime dti_ocup = itDtiOcup->first;

								if (dtf_op <= dti_ocup)
									break;

								if (dti_ocup >= dti_op && dti_ocup < dtf_op)
									semChoque = false;

								auto itDtfOcup = itDtiOcup->second.begin();
								for (; itDtfOcup != itDtiOcup->second.end() && semChoque; itDtfOcup++)
								{
									DateTime dtf_ocup = itDtfOcup->first;

									if ((dti_ocup >= dti_op && dti_ocup < dtf_op) ||
										(dti_op >= dti_ocup && dti_op < dtf_ocup))
									{
										semChoque = false;	// Achou choque
									}
								}
							}

							if (!semChoque) break;
						}
					}

				}

				if (semChoque)
				{
					(mapOpcaoAlunosSemChoque[opcaoIdx]).add(alDem);
				}
			}
		}
	}

}

int ProblemSolution::retornaTurmaDiscAluno(AlunoDemanda* alunoDemanda, bool teorica)
{
	int turma = -1;
	auto itAluno = this->mapAlunoSolution_.find(alunoDemanda->getAluno());
	if (itAluno != this->mapAlunoSolution_.end())
	{
		AlunoSolution *alSol = itAluno->second;
		turma = alSol->getTurma(alunoDemanda, teorica);
	}
	return turma;
}

bool ProblemSolution::getAlunoSolution(Aluno* aluno, AlunoSolution *& alunoSolution)
{
	bool ehNovo = false;

	auto itFinderAluno = mapAlunoSolution_.find(aluno);
	if (itFinderAluno != mapAlunoSolution_.end())
	{
		alunoSolution = itFinderAluno->second;
	}
	else
	{
		alunoSolution = new AlunoSolution(aluno);
		ehNovo = true;
	}

	return ehNovo;
}

AlunoSolution* ProblemSolution::getAlunoSolution(Aluno* aluno)
{
	AlunoSolution* alunoSolution = nullptr;

	auto itFinderAluno = mapAlunoSolution_.find(aluno);
	if (itFinderAluno != mapAlunoSolution_.end())
	{
		alunoSolution = itFinderAluno->second;
	}

	return alunoSolution;
}

void ProblemSolution::getMapsDaSolucao(
	unordered_map < Sala*, unordered_map < Disciplina*, unordered_map<int,
	pair<Professor*, unordered_set<Aluno*>> >> > & solTurmaProfAlunos,
	unordered_map < Campus*, unordered_map < Disciplina*, unordered_map<int,
	unordered_map<int, set<DateTime>> >> > & solCpDiscTurmaDiaDti,
	bool somenteFixado)
{
	ProblemData *problemData = CentroDados::getProblemData();

	if (!this->atendimento_campus)
		return;

	ITERA_GGROUP(it_At_Campus, (*this->atendimento_campus), AtendimentoCampus)
	{
		// Campus do atendimento	   
		Campus *campus = it_At_Campus->campus;
		int campusId = campus->getId();

		if (!it_At_Campus->atendimentos_unidades)
			continue;

		auto ptCpSol = &solCpDiscTurmaDiaDti[campus];

		ITERA_GGROUP_LESSPTR(it_At_Unidade,
			(*it_At_Campus->atendimentos_unidades), AtendimentoUnidade)
		{
			// Unidade do atendimento
			Unidade *unidade = problemData->refUnidade[it_At_Unidade->getId()];

			if (!it_At_Unidade->atendimentos_salas)
				continue;

			ITERA_GGROUP_LESSPTR(it_At_Sala,
				(*it_At_Unidade->atendimentos_salas), AtendimentoSala)
			{
				// Sala do atendimento
				Sala *sala = problemData->refSala[it_At_Sala->getId()];

				if (!it_At_Sala->atendimentos_dias_semana)
					continue;

				auto ptSalaSolTurma = &solTurmaProfAlunos[sala];

				ITERA_GGROUP_LESSPTR(it_At_DiaSemana,
					(*it_At_Sala->atendimentos_dias_semana), AtendimentoDiaSemana)
				{
					// Dia da semana do atendimento
					int dia = it_At_DiaSemana->getDiaSemana();

					if (!it_At_DiaSemana->atendimentos_turno)
						continue;

					ITERA_GGROUP_LESSPTR(it_At_Turno,
						(*it_At_DiaSemana->atendimentos_turno), AtendimentoTurno)
					{
						// Turno do atendimento
						TurnoIES* turno = it_At_Turno->turno;

						if (!it_At_Turno->atendimentos_horarios_aula)
							continue;

						ITERA_GGROUP_LESSPTR(it_At_Hor_Aula,
							(*it_At_Turno->atendimentos_horarios_aula), AtendimentoHorarioAula)
						{
							// Pula aloca��es n�o fixadas
							if (!it_At_Hor_Aula->fixar() && somenteFixado)
								continue;

							// Horario do atendimento
							HorarioAula* horAula = it_At_Hor_Aula->horario_aula;

							// Valida horario-dia alocado na sala						   		    
							if (!sala->possuiHorariosNoDia(horAula, horAula, dia))
							{
								stringstream msg;
								msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
									<< " no dia " << dia << " na sala " << sala->getCodigo();
								CentroDados::printWarning("void ProblemSolution::getMapsDaSolucao()", msg.str());
								continue;
							}

							// Tipo de Credito
							bool ehTeorica = it_At_Hor_Aula->getCreditoTeorico();

							// Professor usado
							int profId = it_At_Hor_Aula->getProfessorId();
							bool profVirtual = it_At_Hor_Aula->profVirtual();

#pragma region Procura o objeto Professor
							Professor *profReal = nullptr;
							if (!profVirtual)
							{
								profReal = problemData->findProfessor(profId);
								if (!profReal)
								{
									stringstream msg;
									msg << "Professor real id " << profId << " nao encontrado.";
									CentroDados::printWarning("void ProblemSolution::getMapsDaSolucao()", msg.str());
									continue;
								}

								// Valida horario-dia alocado no professor
								if (!profReal->possuiHorariosNoDia(horAula, horAula, dia))
								{
									stringstream msg;
									msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
										<< " no dia " << dia << " no professor " << profReal->getNome();
									CentroDados::printWarning("void ProblemSolution::getMapsDaSolucao()", msg.str());
									continue;
								}
							}
#pragma endregion

							if (!it_At_Hor_Aula->atendimentos_ofertas)
								continue;

							// Atendimentos por Oferta
							ITERA_GGROUP_LESSPTR(it_At_Oft,
								(*it_At_Hor_Aula->atendimentos_ofertas), AtendimentoOferta)
							{
								// Pula aloca��es n�o fixadas
								if (!it_At_Oft->fixar() && somenteFixado)
									continue;

								int turma = it_At_Oft->getTurma();
								GGroup<int /*alDem*/> *alDemAtend = &it_At_Oft->alunosDemandasAtendidas;
								string strOfertaId = it_At_Oft->getOfertaCursoCampiId();
								int ofertaId = atoi(strOfertaId.c_str());
								Oferta *oferta = problemData->refOfertas[ofertaId];

#pragma region Procura disciplina real (caso de equival�ncia e/ou pr�tica-te�rica)

								int discOrigId = it_At_Oft->getDisciplinaId();
								int discRealId = it_At_Oft->getDisciplinaSubstitutaId();

								// Se n�o houve substitui��o por equival�ncia, a original � igual � real
								if (discRealId == NULL)
									discRealId = discOrigId;

								Disciplina* discOrig = problemData->refDisciplinas[discOrigId];
								Disciplina* discReal = problemData->refDisciplinas[discRealId];

								if (!ehTeorica)
								{
									Disciplina* discTemp;
									int discTempId = -discRealId;
									discTemp = problemData->getDisciplinaTeorPrat(discReal);
									if (discTemp != NULL)
									{
										discReal = discTemp;
										discRealId = discTempId;
									}
								}

								// Valida horario-dia alocado na disciplina						   		    
								if (!discReal->possuiHorariosNoDia(horAula, horAula, dia))
								{
									stringstream msg;
									msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
										<< " no dia " << dia << " para a turma " << turma
										<< " da disciplina " << discReal->getCodigo();
									CentroDados::printWarning("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
									continue;
								}

								// Valida equival�ncia
								bool usouEquiv = discReal != discOrig;
								if (usouEquiv)
									if (!problemData->ehSubstituivel(discOrig->getId(), discReal->getId(), oferta->curso))
									{
										stringstream msg;
										msg << "Solucao usa equivalencia invalida " << discReal->getCodigo()
											<< " -> " << discOrig->getCodigo()
											<< " para a turma " << turma << " com alunos do curso " << oferta->curso->getCodigo();
										CentroDados::printWarning("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
										continue;
									}
#pragma endregion


								(*ptCpSol)[discReal][turma][dia].insert(horAula->getInicio());

								(*ptSalaSolTurma)[discReal][turma].first = profReal;	// se for virtual, prof � null!															
								auto ptTurmaSol = &(*ptSalaSolTurma)[discReal][turma];

								ITERA_GGROUP_N_PT(itAlDem, *alDemAtend, int)
								{
									AlunoDemanda *alunoDemandaTeor = problemData->retornaAlunoDemanda(*itAlDem);
									if (alunoDemandaTeor)
									{
										Aluno *aluno = alunoDemandaTeor->getAluno();
										(*ptTurmaSol).second.insert(aluno);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}



void ProblemSolution::constroiMapsDaSolucao()
{
	cout << "\nconstroiMapsDaSolucao..."; fflush(0);

	ProblemData *problemData = CentroDados::getProblemData();

	// ------------------------------------------------------------------------------------------
	// Inicializa zerados os creditos j� alocados aos alunos

	ITERA_GGROUP_LESSPTR(itAluno, problemData->alunos, Aluno)
	{
		itAluno->setNroCreditosJaAlocados(0);
	}


	// ------------------------------------------------------------------------------------------
	// Conjunto de aulas para cada turma criada

	mapSolProfRealDiaHorarios_.clear();
	mapSolDiscTurmaDiaAula_.clear();
	mapSolTurmaProfVirtualDiaAula_.clear();
	mapCpDiscTurmaPVId_.clear();

	cout << " lendo atendimentos..."; fflush(0);

	if (this->atendimento_campus != nullptr)
	{
		ITERA_GGROUP(it_At_Campus, (*this->atendimento_campus), AtendimentoCampus)
		{
			// Campus do atendimento	   
			Campus *campus = it_At_Campus->campus;
			int campusId = campus->getId();

			if (!it_At_Campus->atendimentos_unidades)
				continue; // error!

			ITERA_GGROUP_LESSPTR(it_At_Unidade,
				(*it_At_Campus->atendimentos_unidades), AtendimentoUnidade)
			{
				// Unidade do atendimento
				Unidade *unidade = problemData->refUnidade[it_At_Unidade->getId()];

				if (!it_At_Unidade->atendimentos_salas)
					continue; // error!

				ITERA_GGROUP_LESSPTR(it_At_Sala,
					(*it_At_Unidade->atendimentos_salas), AtendimentoSala)
				{
					// Sala do atendimento
					Sala *sala = problemData->refSala[it_At_Sala->getId()];

					if (!it_At_Sala->atendimentos_dias_semana)
						continue; // error!

					ITERA_GGROUP_LESSPTR(it_At_DiaSemana,
						(*it_At_Sala->atendimentos_dias_semana), AtendimentoDiaSemana)
					{
						// Dia da semana do atendimento
						int dia = it_At_DiaSemana->getDiaSemana();

						if (!it_At_DiaSemana->atendimentos_turno)
							continue; // error!

						ITERA_GGROUP_LESSPTR(it_At_Turno,
							(*it_At_DiaSemana->atendimentos_turno), AtendimentoTurno)
						{
							// Turno do atendimento
							TurnoIES* turno = it_At_Turno->turno;

							if (!it_At_Turno->atendimentos_horarios_aula)
								continue; // error!

							ITERA_GGROUP_LESSPTR(it_At_Hor_Aula,
								(*it_At_Turno->atendimentos_horarios_aula), AtendimentoHorarioAula)
							{
								// Horario do atendimento
								int horAulaId = it_At_Hor_Aula->getHorarioAulaId();
								HorarioAula* horAula = it_At_Hor_Aula->horario_aula;

								// Valida horario-dia alocado na sala						   		    
								if (!sala->possuiHorariosNoDia(horAula, horAula, dia))
								{
									stringstream msg;
									msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
										<< " no dia " << dia << " na sala " << sala->getCodigo();
									CentroDados::printError("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
								}

								// Tipo de Credito
								bool ehTeorica = it_At_Hor_Aula->getCreditoTeorico();

								// Atualiza horario ocupado na sala
								HorarioDia *hd = problemData->getHorarioDiaCorrespondente(horAula, dia);
								sala->addHorarioDiaOcupado(hd);

								// Professor usado
								int profId = it_At_Hor_Aula->getProfessorId();
								bool profVirtual = it_At_Hor_Aula->profVirtual();

#pragma region Procura o objeto Professor
								Professor *profReal = nullptr;
								ProfessorVirtualOutput *profVirtualOut = nullptr;
								if (profVirtual)
								{
									profVirtualOut = this->getProfVirtualOutput(profId);
									if (profVirtualOut == nullptr)
									{
										stringstream msg;
										msg << "Professor virtual id " << profId << " nao encontrado.";
										CentroDados::printError("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
									}
								}
								else
								{
									profReal = problemData->findProfessor(profId);
									if (profReal == nullptr)
									{
										stringstream msg;
										msg << "Professor real id " << profId << " nao encontrado.";
										CentroDados::printError("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
									}
									else
									{
										// Valida horario-dia alocado no professor						   		    
										if (!profReal->possuiHorariosNoDia(horAula, horAula, dia))
										{
											stringstream msg;
											msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
												<< " no dia " << dia << " no professor " << profReal->getNome();
											CentroDados::printError("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
										}
									}
								}

#pragma endregion

								int tempoDoCredito = 0;

								if (!it_At_Hor_Aula->atendimentos_ofertas)
									continue;

								// Atendimentos por Oferta
								ITERA_GGROUP_LESSPTR(it_At_Oft,
									(*it_At_Hor_Aula->atendimentos_ofertas), AtendimentoOferta)
								{
									int turma = it_At_Oft->getTurma();
									int qtd = it_At_Oft->getQuantidade();
									GGroup<int /*alDem*/> *alDemAtend = &it_At_Oft->alunosDemandasAtendidas;
									string strOfertaId = it_At_Oft->getOfertaCursoCampiId();
									int ofertaId = atoi(strOfertaId.c_str());
									Oferta *oferta = problemData->refOfertas[ofertaId];

#pragma region Procura disciplina real (caso de equival�ncia e/ou pr�tica-te�rica)

									int discOrigId = it_At_Oft->getDisciplinaId();
									int discRealId = it_At_Oft->getDisciplinaSubstitutaId();

									// Se n�o houve substitui��o por equival�ncia, a original � igual � real
									if (discRealId == NULL)
										discRealId = discOrigId;

									Disciplina* discOrig = problemData->refDisciplinas[discOrigId];
									Disciplina* discReal = problemData->refDisciplinas[discRealId];

									if (!ehTeorica)
									{
										Disciplina* discTemp;
										int discTempId = -discRealId;
										discTemp = problemData->getDisciplinaTeorPrat(discReal);
										if (discTemp != NULL)
										{
											discReal = discTemp;
											discRealId = discTempId;
										}
									}

									// Valida horario-dia alocado na disciplina						   		    
									if (!discReal->possuiHorariosNoDia(horAula, horAula, dia))
									{
										stringstream msg;
										msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
											<< " no dia " << dia << " para a turma " << turma
											<< " da disciplina " << discReal->getCodigo();
										CentroDados::printError("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
									}

									// Valida equival�ncia
									bool usouEquiv = discReal != discOrig;
									if (usouEquiv)
									{
										if (!problemData->ehSubstituivel(discOrig->getId(), discReal->getId(), oferta->curso))
										{
											stringstream msg;
											msg << "Solucao usa equivalencia invalida " << discReal->getCodigo()
												<< " -> " << discOrig->getCodigo()
												<< " para a turma " << turma << " com alunos do curso " << oferta->curso->getCodigo();
											CentroDados::printError("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
										}
									}
#pragma endregion

									mapSolTurmaCursos_[campusId][discReal][turma].add(oferta->curso);

									tempoDoCredito = discReal->getTempoCredSemanaLetiva();

#pragma region Procura ou cria nova aula, e insere

									bool novaAula = true;
									Aula *aula = nullptr;

									auto itFinderCp = mapSolDiscTurmaDiaAula_.find(campusId);
									if (itFinderCp != mapSolDiscTurmaDiaAula_.end())
									{
										auto itFinderDisc = itFinderCp->second.find(discReal);
										if (itFinderDisc != itFinderCp->second.end())
										{
											auto itFinderTurma = itFinderDisc->second.find(turma);
											if (itFinderTurma != itFinderDisc->second.end())
											{
												auto itFinderDia = itFinderTurma->second.first.find(dia);
												if (itFinderDia != itFinderTurma->second.first.end())
												{
													aula = itFinderDia->second;
													novaAula = false;
												}
											}
										}
									}

									if (novaAula)
									{
										// Monta o objeto 'aula'
										aula = new Aula();

										int nCredTeor = (ehTeorica ? 1 : 0);
										int nCredPrat = (ehTeorica ? 0 : 1);

										aula->setTurma(turma);
										aula->setDisciplina(discReal);
										aula->setCampus(campus);
										aula->setSala(sala);
										aula->setUnidade(unidade);
										aula->setDiaSemana(dia);
										aula->setCreditosTeoricos(nCredTeor);
										aula->setCreditosPraticos(nCredPrat);

										HorarioAula *hi = horAula;
										HorarioAula *hf = horAula;
										aula->setHorarioAulaInicial(hi);
										aula->setHorarioAulaFinal(hf);

										DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
										DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;
										aula->setDateTimeInicial(dti);
										aula->setDateTimeFinal(dtf);

										mapSolDiscTurmaDiaAula_[campusId][discReal][turma].first[dia] = aula;
										mapSolDiscTurmaDiaAula_[campusId][discReal][turma].second.add(*alDemAtend);
									}
									else
									{
										HorarioAula *hi = aula->getHorarioAulaInicial();
										HorarioAula *hf = aula->getHorarioAulaFinal();

										bool temHorario = true;
										bool horAulaMenorQueHi = horAula->comparaMenor(*hi);
										bool horAulaMaiorQueHf = hf->comparaMenor(*horAula);

										// Se horAula n�o est� entre hi e hf, ent�o horAula ainda n�o est� sendo englobado na aula
										if (horAulaMenorQueHi || horAulaMaiorQueHf)
											temHorario = false;

										if (!temHorario)
										{
											if (horAulaMenorQueHi)
											{
												hi = horAula;
												aula->setHorarioAulaInicial(hi);
												DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
												aula->setDateTimeInicial(dti);
											}
											if (horAulaMaiorQueHf)
											{
												hf = horAula;
												aula->setHorarioAulaFinal(hf);
												DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;
												aula->setDateTimeFinal(dtf);
											}

											Calendario *calendario = hi->getCalendario();
											int nCreds = calendario->retornaNroCreditosEntreHorarios(hi, hf);

											if (ehTeorica)
												aula->setCreditosTeoricos(nCreds);
											else
												aula->setCreditosPraticos(nCreds);
										}

										mapSolDiscTurmaDiaAula_[campusId][discReal][turma].second.add(*alDemAtend);
									}
#pragma endregion

#pragma region Atualiza situa��o dos alunos da turma
									// Adiciona a aula aos maps dos alunos da turma
									ITERA_GGROUP_N_PT(itAlDem, *alDemAtend, int)
									{
										AlunoDemanda *alunoDemandaTeor = problemData->retornaAlunoDemanda(*itAlDem);
										if (alunoDemandaTeor != nullptr)
										{
											Aluno *aluno = alunoDemandaTeor->getAluno();

											aluno->addNroCreditosJaAlocados(1);

											AlunoSolution *alunoSolution = nullptr;
											bool novo = getAlunoSolution(aluno, alunoSolution);
											alunoSolution->addNroCreditosJaAlocados(1);
											alunoSolution->addChAtendida(tempoDoCredito);
											alunoSolution->addDiaDiscAula(dia, discReal, aula);
											alunoSolution->addTurma(alunoDemandaTeor, discReal, turma, campus);
											if (novo) mapAlunoSolution_[aluno] = alunoSolution;

#pragma region Valida equival�ncia for�ada
											if (alunoDemandaTeor->getExigeEquivalenciaForcada() && !usouEquiv)
											{
												// Equivalencia N�O usada							
												if (alunoDemandaTeor->demanda->disciplina->discEquivSubstitutas.size() > 0)
												{
													stringstream msg;
													msg << "Equivalencia forcada nao respeitada. Aluno "
														<< alunoDemandaTeor->getAlunoId() << " e Disciplina " << discReal->getId();
													CentroDados::printError("void ProblemData::confereCorretudeAlocacoes()", msg.str());
												}
												else
												{
													stringstream msg;
													msg << "Disciplina sem equivalentes encontradas, mas exige equivalencia forcada. Aluno "
														<< alunoDemandaTeor->getAlunoId() << " e Disciplina " << discReal->getId();
													CentroDados::printError("void ProblemData::confereCorretudeAlocacoes()", msg.str());
												}
											}
#pragma endregion
										}
										else
										{
											stringstream msg;
											msg << "AlunoDemanda " << *itAlDem << " nao encontrado";
											CentroDados::printError("void ProblemSolution::constroiMapsDaSolucao()", msg.str());
										}
									}
#pragma endregion

									// Adiciona a aula ao map do professor
									if (profVirtual && profVirtualOut)
									{
										mapSolTurmaProfVirtualDiaAula_[campusId][discReal][turma][profVirtualOut][dia] = aula;
									}

									// temporario
									if (profVirtual)
										mapCpDiscTurmaPVId_[campusId][discReal][turma] = profId;
								}

								// Adiciona horario-dia ocupado do professor
								if (!profVirtual && profReal)
									mapSolProfRealDiaHorarios_[profReal][dia].push_back(hd);
							}
						}
					}
				}
			}
		}
	}

	// ------------------------------------------------------------------------------------------
	// Preenche map com hor�rios livres das salas

	cout << " preenchendo map com horarios livres das salas..."; fflush(0);

	ITERA_GGROUP_LESSPTR(itCp, problemData->campi, Campus)
	{
		ITERA_GGROUP_LESS(itUnidade, itCp->unidades, Unidade)
		{
			ITERA_GGROUP_LESSPTR(itSala, itUnidade->salas, Sala)
			{
				mapSalaDiaHorariosVagos_[*itSala] = (*itSala)->retornaHorariosAulaVagos();
			}
		}
	}

	preencheQuantChProfs();

	gatherIndicadores();

	cout << " fim!"; fflush(0);
}

void ProblemSolution::preencheQuantChProfs()
{
	for (auto itProf = mapSolProfRealDiaHorarios_.cbegin();
		itProf != mapSolProfRealDiaHorarios_.cend(); itProf++)
	{
		auto ptMapCh = &quantChProfs_[itProf->first->getId()];
		(*ptMapCh) = pair<int, int>(0, 0);

		for (auto itDia = itProf->second.cbegin();
			itDia != itProf->second.cend(); itDia++)
		{
			map<DateTime, DateTime> inicioFim;
			for (auto itHorDia = itDia->second.cbegin();
				itHorDia != itDia->second.cend(); itHorDia++)
			{
				inicioFim[(*itHorDia)->getHorarioAula()->getInicio()]
					= (*itHorDia)->getHorarioAula()->getFinal();
			}

			for (auto itDtiDtf = inicioFim.cbegin();
				itDtiDtf != inicioFim.cend(); itDtiDtf++)
			{
				DateTime dti = itDtiDtf->first;
				DateTime dtf = itDtiDtf->second;
				DateTime diff = (dtf - dti);
				(*ptMapCh).first += diff.getDateMinutes();
			}
			(*ptMapCh).second = (int)inicioFim.size();
		}
	}
}

void ProblemSolution::clearMapsDaSolucao()
{
	cout << "\nclearMapsDaSolucao..."; fflush(0);

	// ----------------------------------------------------------
	// DELETA AULAS
	for (auto itMapCp = mapSolDiscTurmaDiaAula_.begin(); itMapCp != mapSolDiscTurmaDiaAula_.end(); itMapCp++)
	{
		for (auto itMapDisc = itMapCp->second.begin(); itMapDisc != itMapCp->second.end(); itMapDisc++)
		{
			for (auto itMapTurma = itMapDisc->second.begin(); itMapTurma != itMapDisc->second.end(); itMapTurma++)
			{
				for (auto itMapDia = itMapTurma->second.first.begin(); itMapDia != itMapTurma->second.first.end(); itMapDia++)
				{
					if (itMapDia->second != nullptr)
						delete itMapDia->second;
				}
				itMapTurma->second.first.clear();
				itMapTurma->second.second.clear();
			}
			itMapDisc->second.clear();
		}
		itMapCp->second.clear();
	}
	mapSolDiscTurmaDiaAula_.clear();
	mapSalaDiaHorariosVagos_.clear();
	mapSolProfRealDiaHorarios_.clear();
	mapSolTurmaProfVirtualDiaAula_.clear();
	quantChProfs_.clear();
	mapSolTurmaCursos_.clear();

	cout << " limpando alunoSolution..."; fflush(0);

	// ----------------------------------------------------------	
	// DELETA ALUNO_SOLUTION
	for (auto itMap = mapAlunoSolution_.begin(); itMap != mapAlunoSolution_.end(); itMap++)
	{
		if (itMap->second != nullptr)
			delete itMap->second;
	}
	mapAlunoSolution_.clear();
	// ----------------------------------------------------------

	cout << " limpo!"; fflush(0);
}

void ProblemSolution::gatherIndicadores()
{
	cout << " indicadores op..."; fflush(0);

	// Calcula nro total de turmas
	int nTurmasT = 0;
	int nTurmasP = 0;
	for (auto itMapCp = mapSolDiscTurmaDiaAula_.cbegin(); itMapCp != mapSolDiscTurmaDiaAula_.cend(); itMapCp++)
		for (auto itMapDisc = itMapCp->second.cbegin(); itMapDisc != itMapCp->second.cend(); itMapDisc++)
			if (itMapDisc->first->getId() < 0)
				nTurmasP += (int)itMapDisc->second.size();
			else
				nTurmasT += (int)itMapDisc->second.size();

	// Calcula nro de turmas ministradas por profs virtuais
	int nroTotalProfsVirtuaisUsados = 0;
	int nTurmasVirtT = 0;
	int nTurmasVirtP = 0;
	int nTurmasPV_semHabReal = 0;
	int nCredsPV_semHabReal = 0;
	int nPV_semHabReal = 0;
	GGroup<ProfessorVirtualOutput*> pvo_discSemProfHab;
	GGroup<ProfessorVirtualOutput*> pvo_totais;
	int nroCredsProfsVirtuais = 0;
	double chProfsVirt = 0;

	for (auto itMapCp = mapSolTurmaProfVirtualDiaAula_.cbegin(); itMapCp != mapSolTurmaProfVirtualDiaAula_.cend(); itMapCp++)
	{
		for (auto itMapDisc = itMapCp->second.cbegin(); itMapDisc != itMapCp->second.cend(); itMapDisc++)
		{
			Disciplina *disc = itMapDisc->first;
			if (disc->getId() < 0)
				nTurmasVirtP += (int)itMapDisc->second.size();
			else
				nTurmasVirtT += (int)itMapDisc->second.size();

			nroCredsProfsVirtuais += disc->getTotalCreditos() * (int)itMapDisc->second.size();
			chProfsVirt += (disc->getTotalCreditos() * itMapDisc->second.size()) * disc->getTempoCredSemanaLetiva();

			bool semProfRealHab = (disc->getNroProfRealHabilit() == 0);

			for (auto itMapTurma = itMapDisc->second.begin(); itMapTurma != itMapDisc->second.end(); itMapTurma++)
			{
				// Calcula nro de profs virtuais usados para turmas de disciplinas sem prof real habilitado
				if (semProfRealHab)
				{
					nCredsPV_semHabReal += disc->getTotalCreditos();

					if (disc->getId() > 0) nTurmasPV_semHabReal++;
				}

				for (auto itMapPVO = itMapTurma->second.begin(); itMapPVO != itMapTurma->second.end(); itMapPVO++)
				{
					if (semProfRealHab)
						pvo_discSemProfHab.add(itMapPVO->first);

					pvo_totais.add(itMapPVO->first);
				}
			}
		}
	}

	nroTotalProfsVirtuaisUsados = (int)pvo_totais.size();
	nPV_semHabReal = (int)pvo_discSemProfHab.size();

	GGroup<int> pvoId_totais;
	GGroup<int> pvoId_discSemProfHab;

	// --------------------------------------------------------
	// Temporario: s� para leitura de solu��o, quando mapSolTurmaProfVirtualDiaAula 
	// n�o ser� preenchido por falta do prof virtual individualizado
	if (mapSolTurmaProfVirtualDiaAula_.size() == 0)
	{
		auto itCp = mapCpDiscTurmaPVId_.cbegin();
		for (; itCp != mapCpDiscTurmaPVId_.cend(); itCp++)
		{
			auto itDisc = itCp->second.cbegin();
			for (; itDisc != itCp->second.cend(); itDisc++)
			{
				Disciplina *disc = itDisc->first;

				nroCredsProfsVirtuais += disc->getTotalCreditos() * (int)itDisc->second.size();
				chProfsVirt += (disc->getTotalCreditos() * itDisc->second.size()) * disc->getTempoCredSemanaLetiva();

				if (disc->getNroProfRealHabilit() == 0)
				{
					auto itTurma = itDisc->second.cbegin();
					for (; itTurma != itDisc->second.cend(); itTurma++)
					{
						if (disc->getId() < 0)
							nTurmasVirtP++;
						else
							nTurmasVirtT++;

						pvoId_totais.add(itTurma->second);

						// Calcula nro de profs virtuais usados para turmas de disciplinas sem prof real habilitado
						if (disc->getNroProfRealHabilit() == 0)
						{
							if (disc->getId() > 0) nTurmasPV_semHabReal++;

							nCredsPV_semHabReal += disc->getTotalCreditos();

							pvoId_discSemProfHab.add(itTurma->second);
						}
					}
				}
			}
		}

		nPV_semHabReal = (int)pvoId_discSemProfHab.size();
		nroTotalProfsVirtuaisUsados = pvoId_totais.size();
	}

	// --------------------------------------------------------
	// Reais
	int nroCredsProfsReais = 0;
	double chProfsReal = 0;

	auto itProf = quantChProfs_.cbegin();
	for (; itProf != quantChProfs_.cend(); itProf++)
	{
		chProfsReal += itProf->second.first;
		nroCredsProfsReais += itProf->second.second;
	}
	// --------------------------------------------------------

	int nTurmasRealT = nTurmasT - nTurmasVirtT;
	int nTurmasRealP = nTurmasP - nTurmasVirtP;

	int nroTotalProfsReaisUsados = (int)mapSolProfRealDiaHorarios_.size();
	int nroTotalProfsUsados = nroTotalProfsReaisUsados + nroTotalProfsVirtuaisUsados;

	int nroCredsProfsUsados = nroCredsProfsReais + nroCredsProfsVirtuais;

	Indicadores::printSeparator(1);
	Indicadores::printIndicador("\nNumero total de professores usados: ", nroTotalProfsUsados);
	Indicadores::printIndicador("\nNumero de professores virtuais usados: ", nroTotalProfsVirtuaisUsados);
	Indicadores::printIndicador("\nNumero de professores reais usados: ", nroTotalProfsReaisUsados);
	Indicadores::printSeparator(1);
	Indicadores::printIndicador("\nNumero total de creditos alocados a professores: ", nroCredsProfsUsados);
	Indicadores::printIndicador("\nNumero de creditos alocados a professores reais: ", nroCredsProfsReais);
	Indicadores::printIndicador("\nNumero de creditos alocados a professores virtuais: ", nroCredsProfsVirtuais);
	Indicadores::printSeparator(1);

	Indicadores::printIndicador("\nCarga horaria alocada a professores (h): ", (chProfsReal + chProfsVirt) / 60, 2);
	Indicadores::printIndicador("\nCarga horaria alocada a professores reais (h): ", chProfsReal / 60, 2);
	Indicadores::printIndicador("\nCarga horaria alocada a professores virtuais (h): ", chProfsVirt / 60, 2);
	Indicadores::printSeparator(1);
	Indicadores::printIndicador("\nNumero de turmas atendidas: ", nTurmasT);
	Indicadores::printIndicador("\nNumero de turmas atendidas por professores reais: ", nTurmasRealT);
	Indicadores::printIndicador("\nNumero de turmas atendidas por professores virtuais: ", nTurmasVirtT);
	Indicadores::printIndicador("\nNumero de turmas praticas (com divisao PT) atendidas: ", nTurmasP);
	Indicadores::printIndicador("\nNumero de turmas praticas (com divisao PT) atendidas por professores reais: ", nTurmasRealP);
	Indicadores::printIndicador("\nNumero de turmas praticas (com divisao PT) atendidas por professores virtuais: ", nTurmasVirtP);
	Indicadores::printSeparator(1);
	Indicadores::printIndicador("\nPara disciplinas sem professor real habilitado:");
	Indicadores::printIndicador("\n\tNumero de professores virtuais usados: ", nPV_semHabReal);
	Indicadores::printIndicador("\n\tNumero de turmas te�ricas atendidas por professores virtuais: ", nTurmasPV_semHabReal);
	Indicadores::printIndicador("\n\tNumero de cr�ditos atendidos por professores virtuais: ", nCredsPV_semHabReal);
}

void ProblemSolution::verificaNrDiscSimultVirtual()
{
	cout << "\nverificaNrDiscSimultVirtual..."; fflush(0);

	// PERCORRE TODAS AS TURMAS COM PROFESSORES VIRTUAIS
	nrMaxDiscSimult_ = 0;
	auto itFinder1 = mapSolTurmaProfVirtualDiaAula_.begin();
	for (; itFinder1 != mapSolTurmaProfVirtualDiaAula_.end(); itFinder1++)
	{
		int cpId = itFinder1->first;

		auto itFinder2 = itFinder1->second.begin();
		for (; itFinder2 != itFinder1->second.end(); itFinder2++)
		{
			Disciplina* disciplina = itFinder2->first;

			unordered_map<int, map<DateTime, unordered_set<Aula*>>> mapDiaDtiAulas;

			// agrupa as aulas da disciplina de profs virtuais por dia/dti
			auto itFinder3 = itFinder2->second.begin();
			for (; itFinder3 != itFinder2->second.end(); itFinder3++)
			{
				auto itFinder4 = itFinder3->second.begin();
				for (; itFinder4 != itFinder3->second.end(); itFinder4++)
				{
					auto itFinder5 = itFinder4->second.begin();
					for (; itFinder5 != itFinder4->second.end(); itFinder5++)
					{
						int dia = itFinder5->first;
						Aula *aula = itFinder5->second;

						mapDiaDtiAulas[dia][*aula->getDateTimeInicial()].insert(aula);
					}
				}
			}

			// histograma
			for (auto itDia = mapDiaDtiAulas.cbegin(); itDia != mapDiaDtiAulas.cend(); itDia++)
			{
				for (auto itDti = itDia->second.cbegin(); itDti != itDia->second.cend(); itDti++)
				{
					int qtd = (int)itDti->second.size();
					if (qtd > 1)
						mapNrDiscSimult[qtd].insert(disciplina);
					if (qtd > nrMaxDiscSimult_)
						nrMaxDiscSimult_ = qtd;
				}
			}
		}
	}

	stringstream msg;
	msg << "\nNr maximo de aulas de mesma disciplina simultaneas " << nrMaxDiscSimult_;
	Indicadores::printIndicador(msg.str());
}

void ProblemSolution::verificaNaoAtendimentosTaticos()
{
	/*
		Algoritmo para, a partir de uma solu��o do modo T�tico do Trieda, determinar
		o motivo de n�o atendimento de um par [aluno, disciplina].
		*/

	CentroDados::startTimer();

	cout << "\nVerificando nao atendimentos tatico..."; fflush(NULL);

	/*
	Possiveis restricoes:
	* Salas lotadas
	* Choque de horarios
	* Disciplina criada um outro turno
	* Demanda minima nao atingida
	* Nao ha sala disponivel para criar a disciplina
	*/

	ProblemData* const problemData = CentroDados::getProblemData();

	// ------------------------------------------------------------------------------------------
	// Maps para Alunos-Demanda n�o atendidos
	map<Disciplina*, map<TurnoIES*, GGroup<AlunoDemanda*,
		Less<AlunoDemanda*>>, Less<TurnoIES*>>, Less<Disciplina*>> mapDiscTurnoAlDemNaoAtend;

	map<Disciplina*, map<TurnoIES*, GGroup<AlunoDemanda*,
		Less<AlunoDemanda*>>, Less<TurnoIES*>>, Less<Disciplina*>> mapDiscTotalNaoAtend;

	vector<AlunoDemanda*> alunoChCompletaMasP1NaoAtend;

	int nroAlunoDemAtendP1 = 0;
	int nroAlunoDemAtendP2 = 0;
	int nroAlunoDemNaoAtendP1 = 0;
	int nroTotalAlunoDemFormandoP1 = 0;
	int nroTotalAlunoDemCalouroP1 = 0;
	int nroAlunoDemFormandoAtendP1 = 0;
	int nroAlunoDemCalouroAtendP1 = 0;

	// ------------------------------------------------------------------------------------------
	// Percore todos os Alunos-Demanda do problema, agrupando os n�o atendidos

	ITERA_GGROUP_LESSPTR(it_alunos_demanda, problemData->alunosDemandaTotal, AlunoDemanda)
	{
		AlunoDemanda* ad = (*it_alunos_demanda);
		Demanda *demanda = (ad->demandaOriginal == nullptr ? ad->demanda : ad->demandaOriginal);

		// N�mero de disciplinas requeridas x N�mero de disciplinas atendidas
		int nroDiscsReqP1 = ad->getAluno()->getNroDiscsOrigRequeridosP1();
		int nroDiscsAtend = 0;
		AlunoSolution *alunoSol = this->getAlunoSolution(ad->getAluno());
		if (alunoSol != nullptr)
			nroDiscsAtend = alunoSol->getNroDiscAtendidas();

		// Completude do atendimento do aluno
		bool nroDiscsCompleto = (nroDiscsAtend >= nroDiscsReqP1);
		bool chCompleta = ad->getAluno()->totalmenteAtendido();

		bool P1 = (ad->getPrioridade() == 1);
		bool teorica = (demanda->disciplina->getId() > 0);

		// ------------------------------------------------- procura AlunoDemanda original.
		AlunoDemanda* adOrig = problemData->procuraAlunoDemanda(abs(demanda->getDisciplinaId()), ad->getAlunoId(), ad->getPrioridade());
		if (adOrig == nullptr)
		{
			CentroDados::printError("", "Erro, adOrig null");
			continue;
		}
		// Procura sempre por aloca��o te�rica, pq a equivalencia pode ter eliminado a exist�ncia de pr�tica
		int nTurma = this->retornaTurmaDiscAluno(adOrig, true);
		bool alocado = (nTurma == -1 ? false : true);
		// -------------------------------------------------------------------------------

		if (alocado && P1 && teorica) nroAlunoDemAtendP1++;
		else if (!alocado && P1 && teorica) nroAlunoDemNaoAtendP1++;
		else if (alocado && !P1 && teorica) nroAlunoDemAtendP2++;

		if (P1 && teorica)
		{
			if (ad->getAluno()->ehFormando())
			{
				nroTotalAlunoDemFormandoP1++;
				if (alocado) nroAlunoDemFormandoAtendP1++;
			}
			if (ad->getAluno()->ehCalouro())
			{
				nroTotalAlunoDemCalouroP1++;
				if (alocado) nroAlunoDemCalouroAtendP1++;
			}
		}

		if (!alocado)
		{
			//if ( ad->getAluno()->ehFormando() )
			//	formandosNaoAtendidos[ad->getAluno()][ad->getPrioridade()].add( ad );
			//if ( ad->getAluno()->ehCalouro() )
			//	calourosNaoAtendidos[ad->getAluno()][ad->getPrioridade()].add( ad );

			if (!(chCompleta || nroDiscsCompleto))
			{
				mapDiscTurnoAlDemNaoAtend[demanda->disciplina][ad->getOferta()->turno].add(ad);

				// ------------------------------------------------
				// Agrupamento de todas as folgas, incluindo as disciplinas equivalentes
				mapDiscTotalNaoAtend[demanda->disciplina][ad->getOferta()->turno].add(ad);
				if (problemData->parametros->considerar_equivalencia_por_aluno && ad->demandaOriginal == NULL)
				{
					ITERA_GGROUP_LESSPTR(itDiscEquiv, ad->demanda->disciplina->discEquivSubstitutas, Disciplina)
					{
						if (problemData->alocacaoEquivViavel(ad->demanda, *itDiscEquiv))
							mapDiscTotalNaoAtend[*itDiscEquiv][ad->getOferta()->turno].add(ad);
					}
				}
				// ------------------------------------------------
			}
			else if ((chCompleta || nroDiscsCompleto) && P1 && teorica)
				alunoChCompletaMasP1NaoAtend.push_back(ad);
		}
	}

	// ------------------------------------------------------------------------------------------
	setNroTotalAlunoDemandaP1(nroAlunoDemAtendP1 + nroAlunoDemNaoAtendP1);
	setNroAlunoDemAtendP1(nroAlunoDemAtendP1);
	setNroAlunoDemNaoAtendP1(nroAlunoDemNaoAtendP1);
	setNroAlunoDemAtendP2(nroAlunoDemAtendP2);
	setNroAlunoDemAtendP1P2(nroAlunoDemAtendP1 + nroAlunoDemAtendP2);
	setNroTotalAlunoDemFormandosP1(nroTotalAlunoDemFormandoP1);
	setNroTotalAlunoDemCalourosP1(nroTotalAlunoDemCalouroP1);
	setNroTotalAlunoDemFormandosAtendP1(nroAlunoDemFormandoAtendP1);
	setNroTotalAlunoDemCalourosAtendP1(nroAlunoDemCalouroAtendP1);

	imprimeIndicadores();

	// ------------------------------------------------------------------------------------------
	// Alunos com carga hor�ria completa ou n�mero de disciplinas pedido completo,
	// mas com folga de disciplina original de P1

	for (int at = 0; at < alunoChCompletaMasP1NaoAtend.size(); at++)
	{
		AlunoDemanda* ad = alunoChCompletaMasP1NaoAtend[at];

		NaoAtendimento* naoAtendimento = new NaoAtendimento(ad->getId());
		auto itNaoAtend = this->nao_atendimentos->find(naoAtendimento);
		if (itNaoAtend == this->nao_atendimentos->end())
		{
			stringstream ss;
			ss << "O aluno teve o total de carga hor�ria e/ou o total de disciplinas requisitado de prioridade 1 atendido, "
				<< "por�m com uso de prioridade 2 e/ou equival�ncias.";
			naoAtendimento->addMotivo(ss.str());

			this->nao_atendimentos->add(naoAtendimento);
		}
		else delete naoAtendimento;
	}

	// ------------------------------------------------------------------------------------------
	// Investiga AlunosDemanda n�o atendidos

	map<int, vector<string> > aluno_saida_em_txt;

	auto itDisc = mapDiscTurnoAlDemNaoAtend.begin();
	for (; itDisc != mapDiscTurnoAlDemNaoAtend.end(); itDisc++)
	{
		map<TurnoIES*, GGroup<AlunoDemanda*, Less<AlunoDemanda*>>, Less<TurnoIES*>>* mapTurnoAlDemNaoAtend = &itDisc->second;
		auto itTurno = (*mapTurnoAlDemNaoAtend).begin();
		for (; itTurno != (*mapTurnoAlDemNaoAtend).end(); itTurno++)
		{
			GGroup<AlunoDemanda*, Less<AlunoDemanda*>> *alDemNaoAtend = &itTurno->second;
			ITERA_GGROUP_LESS(itAlDem, *alDemNaoAtend, AlunoDemanda)
			{
				AlunoDemanda* ad = (*itAlDem);
				Demanda *demanda = (ad->demandaOriginal == nullptr ? ad->demanda : ad->demandaOriginal);

				//	cout << "\nVerificando nao atendimento do AlunoDemanda id " 
				//		<< ad->getId() << ", aluno " << ad->getAlunoId() << ", disc " << ad->demanda->disciplina->getId();

				bool bd = false;
				if (ad->getAlunoId() == 111180 && demanda->getDisciplinaId() == 24690)
				{
					//bd = true;
				}


				if (bd) cout << "\nALuno " << ad->getAlunoId() << " Disc " << demanda->disciplina->getId()
					<< " Prior " << ad->getPrioridade();

				//if ( ad->getAlunoId() == 53254 && ad->demanda->disciplina->getId() == -16004 ){
				//bd = true;
				//}

				int campusAlunoId = ad->getCampus()->getId();
				Aluno *aluno = ad->getAluno();
				int alunoId = ad->getAluno()->getAlunoId();
				int alDemId = ad->getId();
				int turnoIESid = demanda->getTurnoIES()->getId();
				TurnoIES* turnoIES = demanda->getTurnoIES();
				string turnoIESname = demanda->getTurnoIES()->getNome();

				if (bd) cout << "\n1"; fflush(NULL);

				int alDemIdOrig = ad->getId();
				if (demanda->getDisciplinaId() < 0)
				{
					AlunoDemanda *adOrig = problemData->procuraAlunoDemanda(-demanda->getDisciplinaId(), ad->getAlunoId(), ad->getPrioridade());
					if (adOrig == nullptr)
					{
						stringstream msg;
						msg << "2. AlunoDemanda teorico associado ao pratico nao encontrado para a disc "
							<< -demanda->getDisciplinaId() << " e aluno " << ad->getAlunoId();
						CentroDados::printError("void ProblemSolution::verificaNaoAtendimentosTaticos()", msg.str());
						continue;
					}
					else alDemIdOrig = adOrig->getId();
				}

				NaoAtendimento* naoAtendimento = new NaoAtendimento(alDemIdOrig);
				auto itNaoAtend = this->nao_atendimentos->find(naoAtendimento);
				if (itNaoAtend != this->nao_atendimentos->end())
				{
					delete naoAtendimento;
					naoAtendimento = *itNaoAtend;
					this->nao_atendimentos->remove(itNaoAtend);
				}

				if (bd) cout << "\n2"; fflush(NULL);

				// Verifica se alguma turma para o campus foi criada
				auto itMapSol = mapSolDiscTurmaDiaAula_.find(campusAlunoId);
				if (itMapSol == mapSolDiscTurmaDiaAula_.end())
				{
					stringstream ss2;
					ss2 << "Nenhuma turma de nenhuma disciplina criada no campus " << ad->getCampus()->getCodigo() << ".";
					naoAtendimento->addMotivo(ss2.str());

					this->nao_atendimentos->add(naoAtendimento);

					continue;
				}

				if (bd) cout << "\n3"; fflush(NULL);

				// Acha as aulas do aluno
				AlunoSolution* alSol = nullptr;
				auto itAulasAluno = this->mapAlunoSolution_.find(aluno);
				if (itAulasAluno != this->mapAlunoSolution_.end())
					alSol = itAulasAluno->second;

				// Disciplinas que serviriam para atender esse aluno-demanda
				GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas;
				disciplinas.add(demanda->disciplina);

				if (problemData->parametros->considerar_equivalencia_por_aluno)
				{
					if (ad->demandaOriginal == NULL)
					{
						ITERA_GGROUP_LESSPTR(itDiscEquiv, ad->demanda->disciplina->discEquivSubstitutas, Disciplina)
						{
							if (problemData->alocacaoEquivViavel(ad->demanda, *itDiscEquiv))
								disciplinas.add(*itDiscEquiv);
						}
					}
					else
						disciplinas.add(ad->demandaOriginal->disciplina);
				}

				if (bd) cout << "\n4"; fflush(NULL);

				ITERA_GGROUP_LESSPTR(it_disciplina, disciplinas, Disciplina)
				{
					Disciplina *discEquiv = *it_disciplina;

					bool discEhEquiv;
					if (ad->demandaOriginal == nullptr)
						discEhEquiv = discEquiv->getId() != ad->demanda->getDisciplinaId();
					else
						discEhEquiv = discEquiv->getId() != ad->demandaOriginal->getDisciplinaId();

					if (bd) cout << "\n\t4.1"; fflush(NULL);

					// Equival�ncia for�ada
					if (ad->getExigeEquivalenciaForcada() && !discEhEquiv)
					{
						stringstream ss2;
						ss2 << "Equival�ncia for�ada para a disciplina " << discEquiv->getCodigo() << ".";
						naoAtendimento->addMotivo(ss2.str());
						continue;
					}

					// Disciplina pratica resultante de divis�o de cr�ditos pr�ticos+te�ricos
					bool discPratica = false;
					if (discEquiv->getId() < 0)
						discPratica = true;

					string strTurmaTipoCred("");
					string strTurmasTipoCred("");
					if (discPratica)
					{
						strTurmaTipoCred = "pr�tica ";
						strTurmasTipoCred = "pr�ticas ";
					}
					else if (discEquiv->temParPratTeor())
					{
						strTurmaTipoCred = "te�rica ";
						strTurmasTipoCred = "te�ricas ";
					}


					// Calcula minimo de alunos na turma
					int minAlunosNaTurma = 0;
					if (problemData->parametros->min_alunos_abertura_turmas && !discEquiv->eLab())
						minAlunosNaTurma = problemData->parametros->min_alunos_abertura_turmas_value;
					else if (problemData->parametros->min_alunos_abertura_turmas_praticas && discEquiv->eLab())
						minAlunosNaTurma = problemData->parametros->min_alunos_abertura_turmas_praticas_value;


					// Procura todos os n�o atendimentos na disciplina discEquiv
					map<TurnoIES*, GGroup<AlunoDemanda*, Less<AlunoDemanda*>>, Less<TurnoIES*>>
						*mapAlsDemNaoAtend = nullptr;
					auto itDiscNaoAtend = mapDiscTotalNaoAtend.find(discEquiv);
					if (itDiscNaoAtend != mapDiscTotalNaoAtend.end())
						mapAlsDemNaoAtend = &itDiscNaoAtend->second;


					// Verifica se alguma turma no campus para a disciplina foi criada
					map<Disciplina*, map<int /*turma*/, pair<map<int /*dia*/, Aula*>,
						GGroup<int /*alDem*/>>>, Less<Disciplina*>> *mapDiscsSol = &itMapSol->second;
					auto itDiscSol = (*mapDiscsSol).find(discEquiv);
					if (itDiscSol != (*mapDiscsSol).end())
					{
						if (bd) cout << "\n\t4.2"; fflush(NULL);

						/*
						Disciplina Foi Criada
						- Capacidade da sala
						- Se houve choque de horario
						- Verificar se foi em outro turno
						*/
#pragma region Disciplina_Criada

						// Indica se alguma turma foi aberta no turno da oferta
						bool haTurmaMesmoTurno = false;

						// Conjunto de turmas que estao com salas lotadas
						GGroup< pair<int /*turma*/, Sala*> > salas_lotadas;

						// Conjunto de turmas em que houve choque de horario
						map< int /*turma existente*/, map< Trio< int, int, Disciplina* >, GGroup<Aula*, LessPtr<Aula>> > > turmas_choque_horarios;

						GGroup< int /*turmas existentes*/ > turmasForaDoTurno;
						GGroup< int /*turmas existentes*/ > turmasMaxPedagAlunosAtingido;

						int maxAlunosPedagogico = discEquiv->getMaxAlunosP() + discEquiv->getMaxAlunosT();

						int totalTurmasCriadas = (int)itDiscSol->second.size();

						GGroup<int> turmasSemMotivo;

#pragma region Pesquisa os motivos de n�o inser��o do aluno em cada turma aberta da disciplina

						map< int /*turma*/, pair< map<int /*dia*/, Aula*>, GGroup<int /*alDem*/> > >
							*turmasCriadas = &itDiscSol->second;
						auto itTurmasSol = (*turmasCriadas).begin();
						for (; itTurmasSol != (*turmasCriadas).end(); itTurmasSol++)
						{
							int turma = itTurmasSol->first;

							int turmaSize = itTurmasSol->second.second.size();

							if (turmaSize >= maxAlunosPedagogico)
							{
								turmasMaxPedagAlunosAtingido.add(turma);
								break;
							}

							map< int /*dia*/, Aula* >::iterator itDiaSol = itTurmasSol->second.first.begin();
							for (; itDiaSol != itTurmasSol->second.first.end(); itDiaSol++)
							{
								Aula *aula = itDiaSol->second;
								HorarioAula *hi = aula->getHorarioAulaInicial();
								HorarioAula *hf = aula->getHorarioAulaFinal();
								DateTime inicio = hi->getInicio();
								DateTime fim = hf->getFinal();
								int dia = aula->getDiaSemana();
								Sala* sala = aula->getSala();

								if (ad->getOferta()->turno->possuiHorarioDiaOuCorrespondente(hi, hf, dia))
								{
									if (sala->getCapacidade() > turmaSize)
									{
										/*  Ha vaga na sala;
											Checar se houve choque de horario
											*/

										turmasSemMotivo.add(turma);

										// Turmas em que o aluno foi alocado
										if (alSol != nullptr)
										{
											GGroup< Aula* > aulasComChoque;
											alSol->procuraChoqueDeHorariosAluno(dia, inicio, fim, aulasComChoque);
											ITERA_GGROUP(itAula, aulasComChoque, Aula)
											{
												Aula *aula = *itAula;
												Trio< int, int, Disciplina* > trio_aluno
													(aula->getCampus()->getId(), aula->getTurma(), aula->getDisciplina());

												turmas_choque_horarios[turma][trio_aluno].add(aula);
											}
										}
									}
									else
									{
										/* Sala lotada */
										pair<int /*turma*/, Sala*> lotacao(turma, sala);
										salas_lotadas.add(lotacao);
									}
								}
								else
								{
									turmasForaDoTurno.add(turma);
								}
							}
						}
#pragma endregion

						if (turmasForaDoTurno.size() == totalTurmasCriadas)
						{
							// TODAS AS TURMAS CRIADAS EST�O EM TURNOS DIFERENTES
							stringstream ss2;
							ss2 << "Todas as turmas " << strTurmasTipoCred
								<< "da disciplina " << discEquiv->getCodigo()
								<< (discEhEquiv ? "(equivalente) " : " ")
								<< "criadas est�o em turnos diferentes do turno do aluno.";
							naoAtendimento->addMotivo(ss2.str());
						}
						else
						{
#pragma region Turmas com limite pedag�gico de alunos atingido
							int nroTurmasMaxPedag = turmasMaxPedagAlunosAtingido.size();
							if (nroTurmasMaxPedag == 1)
							{
								stringstream ss;
								ss << "A turma " << strTurmaTipoCred
									<< *(turmasMaxPedagAlunosAtingido.begin())
									<< " da disciplina " << discEquiv->getCodigo()
									<< (discEhEquiv ? "(equivalente)" : " ")
									<< " atingiu o m�ximo pedag�gico de " << maxAlunosPedagogico
									<< " alunos na turma.";
								naoAtendimento->addMotivo(ss.str());
							}
							else if (nroTurmasMaxPedag > 1)
							{
								stringstream ss;
								ss << "As turmas " << strTurmasTipoCred;
								int counter = 0;
								ITERA_GGROUP_N_PT(itTurma, turmasMaxPedagAlunosAtingido, int)
								{
									counter++;
									ss << *itTurma;
									if (counter + 1 < nroTurmasMaxPedag) ss << ", ";			// antes do penultimo
									else if (counter + 1 == nroTurmasMaxPedag) ss << " e ";	// penultimo
								}

								ss << "da disciplina " << discEquiv->getCodigo()
									<< (discEhEquiv ? "(equivalente)" : " ")
									<< " atingiram o m�ximo pedag�gico de "
									<< maxAlunosPedagogico << " alunos.";

								naoAtendimento->addMotivo(ss.str());
							}
#pragma endregion

#pragma region Salas lotadas
							int nroSalasLotadas = salas_lotadas.size();
							if (nroSalasLotadas == 1)
							{
								stringstream ss;
								ss << "A turma " << strTurmaTipoCred
									<< (*salas_lotadas.begin()).first
									<< " da disciplina " << discEquiv->getCodigo()
									<< (discEhEquiv ? "(equivalente)" : " ")
									<< " est� lotada na sala " << (*salas_lotadas.begin()).second->getCodigo() << ".";
								naoAtendimento->addMotivo(ss.str());
							}
							else if (nroSalasLotadas > 1)
							{
								stringstream ss;
								ss << "As turmas " << strTurmasTipoCred;
								int counter = 0;
								GGroup< pair<int /*turma*/, Sala*> >::iterator it_salas_lotadas = salas_lotadas.begin();
								for (; it_salas_lotadas != salas_lotadas.end(); it_salas_lotadas++)
								{
									counter++;
									ss << (*it_salas_lotadas).first;
									if (counter + 1 < nroSalasLotadas) ss << ", ";			// antes do penultimo
									else if (counter + 1 == nroSalasLotadas) ss << " e ";		// penultimo
								}

								ss << " da disciplina " << discEquiv->getCodigo()
									<< (discEhEquiv ? "(equivalente)" : " ")
									<< " est�o lotadas.";

								naoAtendimento->addMotivo(ss.str());
							}
#pragma endregion

#pragma region Turmas fora do turno do aluno
							int nroTurmasForaDoTurno = turmasForaDoTurno.size();
							if (nroTurmasForaDoTurno == 1)
							{
								stringstream ss;
								ss << "A turma " << strTurmaTipoCred
									<< *(turmasForaDoTurno.begin())
									<< " da disciplina " << discEquiv->getCodigo()
									<< (discEhEquiv ? "(equivalente)" : " ")
									<< " est� em turno diferente do turno do aluno.";
								naoAtendimento->addMotivo(ss.str());
							}
							else if (nroTurmasForaDoTurno > 1)
							{
								stringstream ss;
								ss << "As turmas " << strTurmasTipoCred;
								int counter = 0;
								ITERA_GGROUP_N_PT(itTurma, turmasForaDoTurno, int)
								{
									counter++;
									ss << *itTurma;
									if (counter + 1 < nroTurmasForaDoTurno) ss << ", ";			// antes do penultimo
									else if (counter + 1 == nroTurmasForaDoTurno) ss << " e ";	// penultimo
								}

								ss << "da disciplina " << discEquiv->getCodigo()
									<< (discEhEquiv ? "(equivalente)" : " ")
									<< " est�o em turno diferente do turno do aluno.";

								naoAtendimento->addMotivo(ss.str());
							}
#pragma endregion

#pragma region Turmas com choques de hor�rios com o aluno
							map< int, map< Trio< int, int, Disciplina* >, GGroup<Aula*, LessPtr<Aula>> > >::iterator
								it_turma_criada = turmas_choque_horarios.begin();
							for (; it_turma_criada != turmas_choque_horarios.end(); it_turma_criada++)
							{
								int turmaCriada = it_turma_criada->first;

								turmasSemMotivo.remove(turmaCriada);

								map< Trio< int, int, Disciplina* >, GGroup<Aula*, LessPtr<Aula>> > ::iterator
									it_turmas_choque_horarios = it_turma_criada->second.begin();
								for (; it_turmas_choque_horarios != it_turma_criada->second.end(); it_turmas_choque_horarios++)
								{
									Disciplina *discChoque = (*it_turmas_choque_horarios).first.third;
									string strChoqueTipoCred("");
									if (discChoque->temParPratTeor() && discChoque->getId() < 0)
										strChoqueTipoCred = "pratica ";
									else if (discChoque->temParPratTeor())
										strChoqueTipoCred = "teorica ";

									// HOUVE CHOQUE DE HORARIO COM OUTRAS AULAS DO ALUNO
									stringstream ss2;
									ss2 << "A turma " << strTurmaTipoCred
										<< turmaCriada << " da disciplina " << discEquiv->getCodigo()
										<< (discEhEquiv ? " (equivalente)" : "")
										<< " tem choque nos hor�rios do aluno com a turma " << strChoqueTipoCred
										<< (*it_turmas_choque_horarios).first.second
										<< " da disciplina " << discChoque->getCodigo()
										<< " no campus " << (*it_turmas_choque_horarios).first.first << ".";
									naoAtendimento->addMotivo(ss2.str());
								}
							}
#pragma endregion

#pragma region Turmas sem motivo encontrado para n�o inser��o do aluno
							for (auto itTurmaSemMot = turmasSemMotivo.begin();
								itTurmaSemMot != turmasSemMotivo.end(); itTurmaSemMot++)
							{
								stringstream ss;
								ss << "N�o foi encontrado motivo para n�o inser��o do aluno na turma "
									<< strTurmaTipoCred << *itTurmaSemMot
									<< " da disciplina " << discEquiv->getCodigo()
									<< (discEhEquiv ? " (equivalente)" : "") << ".";
								if (discPratica)
									ss << " Verifique se o aluno pode ser atendido em alguma turma te�rica associada.";
								naoAtendimento->addMotivo(ss.str());
							}
#pragma endregion
						}

						if (totalTurmasCriadas == discEquiv->getNumTurmas())
						{
							// FOI CRIADO O MAXIMO DE TURMAS		

							stringstream ss2;
							ss2 << "N�o � poss�vel abrir mais turmas " << strTurmasTipoCred
								<< "para a disciplina " << discEquiv->getCodigo()
								<< (discEhEquiv ? " (equivalente)" : "") << ".";
							naoAtendimento->addMotivo(ss2.str());
						}
						else
						{
							this->verificaPossivelNovaTurma(naoAtendimento, discEquiv, ad, campusAlunoId, *mapAlsDemNaoAtend);
						}
#pragma endregion
					}
					else
					{
						if (bd) cout << "\n\t4.3"; fflush(NULL);

						/*
						Disciplina Nao Foi Criada
						- verificar se a disciplina poderia ser criada no turno ofertado
						- Verificar se houve demanda suficiente
						- Verificar se havia sala disponivel para criar a disciplina
						*/
#pragma region Disciplina_Nao_Criada

						if (!discEquiv->possuiTurnoIES(turnoIES))
						{
							// DISCIPLINA COM HORARIO DISPONIVEL EM TURNO DIFERENTE DA OFERTA						
							stringstream ss2;
							ss2 << "A disciplina "
								<< discEquiv->getCodigo()
								<< (discEhEquiv ? " (EQUIVALENTE)" : " ")
								<< ", n�o tem hor�rios suficientes dispon�veis no turno "
								<< demanda->getTurnoIES()->getNome() << " associado ao aluno.";
							naoAtendimento->addMotivo(ss2.str());

							continue;
						}

						if (bd) cout << "\n\t\t4.31"; fflush(NULL);


#pragma region Verifica demanda m�nima necess�ria
						int demandaSizeNoTurnoIES = 0;
						if (minAlunosNaTurma > 0)
						{
							bool formando = false;

							if (bd) cout << "\n\t\t4.32"; fflush(NULL);

							if (mapAlsDemNaoAtend != nullptr)
							{
								auto itTurnoNaoAtend = mapAlsDemNaoAtend->begin();
								for (; itTurnoNaoAtend != mapAlsDemNaoAtend->end(); itTurnoNaoAtend++)
								{
									if (itTurnoNaoAtend->first->getId() != turnoIESid)
										continue;

									ITERA_GGROUP_LESS(itAlDem, itTurnoNaoAtend->second, AlunoDemanda)
									{
										demandaSizeNoTurnoIES++;

										if ((*itAlDem)->getAluno()->ehFormando())
										{
											formando = true;
										}
									}
								}
							}

							if (bd) cout << "\n\t\t4.33"; fflush(NULL);

							if (demandaSizeNoTurnoIES < minAlunosNaTurma)
								if ((problemData->parametros->violar_min_alunos_turmas_formandos && !formando) ||
									(!problemData->parametros->violar_min_alunos_turmas_formandos))
								{
									// DEMANDA MINIMA NAO ATINGIDA
									stringstream ss2;
									ss2 << "Demanda m�nima n�o atingida pela disciplina "
										<< discEquiv->getCodigo()
										<< (discEhEquiv ? " (equivalente)" : " ")
										<< ", no turno " << turnoIESname << ".";
									naoAtendimento->addMotivo(ss2.str());

									this->nao_atendimentos->add(naoAtendimento);

									continue;
								}
						}
#pragma endregion

						if (bd) cout << "\n\t\t4.34"; fflush(NULL);

						this->verificaPossivelNovaTurma(naoAtendimento, discEquiv, ad, campusAlunoId, *mapAlsDemNaoAtend);

						if (bd) cout << "\n\t\t4.35"; fflush(NULL);

#pragma endregion
					}
				}

				if (bd) cout << "\n5"; fflush(NULL);

				this->nao_atendimentos->add(naoAtendimento);

				if (bd) cout << "\n6"; fflush(NULL);
			}
		}
	}


	double dif = CentroDados::getLastRunTime();
	CentroDados::stopTimer();
	cout << " " << dif << "sec"; fflush(NULL);

}

void ProblemSolution::verificaPossivelNovaTurma(NaoAtendimento *naoAtendimento, Disciplina *discEquiv, AlunoDemanda *ad, int campusAlunoId,
	const map<TurnoIES*, GGroup<AlunoDemanda*, Less<AlunoDemanda*>>, Less<TurnoIES*>>& mapAlsDemNaoAtend)
{
	// ---------------------------------
	// Sa�da com detalhes para an�lise interna
	bool printMotivosInternos = CentroDados::getPrintLogs() && false;
	ofstream outMotivosInternos;
	if (printMotivosInternos)
	{
		outMotivosInternos.open("infoExtraPossivelNovaTurm.txt", ios::app);
	}

	// ---------------------------------
	// Referencia para ProblemData
	ProblemData* const problemData = CentroDados::getProblemData();

	// ---------------------------------
	// Debug
	bool bd = false;

	// ---------------------------------
	// Equiv
	bool discEhEquiv = (ad->demanda->disciplina != discEquiv);

	// ---------------------------------
	// Disciplina pratica resultante de divis�o de cr�ditos pr�ticos+te�ricos
	bool discPratica = false;
	if (discEquiv->getId() < 0)
		discPratica = true;

	string strTurmaTipoCred("");
	string strTurmasTipoCred("");
	if (discPratica)
	{
		strTurmaTipoCred = "pr�tica ";
		strTurmasTipoCred = "pr�ticas ";
	}
	else if (discEquiv->temParPratTeor())
	{
		strTurmaTipoCred = "te�rica ";
		strTurmasTipoCred = "te�ricas ";
	}

	// ---------------------------------
	// Calcula minimo de alunos na turma
	int minAlunosNaTurma = 0;
	if (problemData->parametros->min_alunos_abertura_turmas && !discEquiv->eLab())
		minAlunosNaTurma = problemData->parametros->min_alunos_abertura_turmas_value;
	else if (problemData->parametros->min_alunos_abertura_turmas_praticas && discEquiv->eLab())
		minAlunosNaTurma = problemData->parametros->min_alunos_abertura_turmas_praticas_value;

	// ---------------------------------
	// Acha as aulas do aluno
	Aluno *aluno = ad->getAluno();
	AlunoSolution* alSol = nullptr;
	auto itAulasAluno = this->mapAlunoSolution_.find(aluno);
	if (itAulasAluno != this->mapAlunoSolution_.end())
		alSol = itAulasAluno->second;


	// ---------------------------------------------------------------------------------------------------
	// VERIFICA SALAS

	map<Sala*, vector<map<int /*dia*/, GGroup<HorarioAula*, Less<HorarioAula*>>>>, Less<Sala*>>
		mapSalaDiaHorsLivres = procuraCombinacaoLivreEmSalas(discEquiv, ad->demanda->getTurnoIES(), campusAlunoId);

	if (mapSalaDiaHorsLivres.size() == 0)		// N�o h� salas com hor�rios livres suficientes
	{
		stringstream ss;
		stringstream ss2;
		ss2 << "N�o existe sala com hor�rios livres suficientes para abrir turmas "
			<< strTurmasTipoCred << "da disciplina "
			<< discEquiv->getCodigo() << (discEhEquiv ? " (equivalente)" : "") << ".";
		naoAtendimento->addMotivo(ss2.str());
	}
	else										// H� salas com hor�rios livres suficientes
	{
		// Verifica para cada sala com hor�rio livre se h� choque de hor�rios com o aluno
		stringstream ss1ComChoqueComAlunos;
		stringstream ss2ComChoqueComAlunos;
		ss1ComChoqueComAlunos << "N�o h� alunos com hor�rios livres em comum "
			<< "no turno " << ad->demanda->getTurnoIES()->getNome()
			<< " suficientes "
			<< "para abrir nova turma " << strTurmaTipoCred
			<< "da disciplina " << discEquiv->getCodigo()
			<< (discEhEquiv ? " (equivalente)" : "");
		int opcaoComChoqueComAlunos = 0;

		stringstream ss1SemChoque;
		stringstream ss2SemChoque;
		ss1SemChoque << "Possibilidade para abrir nova turma " << strTurmaTipoCred
			<< "da disciplina " << discEquiv->getCodigo()
			<< (discEhEquiv ? " (equivalente)" : "")
			<< " encontrada";
		int opcaoSemChoque = 0;

		stringstream ss1ComChoqueEmSala;
		stringstream ss2ComChoqueEmSala;
		ss1ComChoqueEmSala << "N�o h� hor�rios livres "
			<< "no turno " << ad->demanda->getTurnoIES()->getNome()
			<< " em comum com o aluno suficientes "
			<< "para abrir mais turmas " << strTurmasTipoCred
			<< "da disciplina " << discEquiv->getCodigo()
			<< (discEhEquiv ? " (equivalente)" : "");
		int opcaoComChoqueEmSala = 0;

		auto itSala = mapSalaDiaHorsLivres.begin();
		for (; itSala != mapSalaDiaHorsLivres.end(); itSala++)
		{
			Sala *sala = itSala->first;
			vector<map<int /*dia*/, GGroup<HorarioAula*, Less<HorarioAula*>>>>
				*mapOpcoesSalaLivre = &itSala->second;

			if (bd) cout << "\n\t\t\t4.34.1  sala " << sala->getId(); fflush(NULL);

			map< int, GGroup< Trio<int, int, Disciplina*> > > choques;

			if (alSol != nullptr)
				alSol->procuraChoqueDeHorariosAluno(*mapOpcoesSalaLivre, choques);

			if (bd) cout << "\n\t\t\t4.34.2"; fflush(NULL);

			if (choques.size() == itSala->second.size())	// Todas as opcoes da sala t�m choques
			{
				opcaoComChoqueEmSala++;
				ss2ComChoqueEmSala << sala->getCodigo() << "; ";
			}
			else											// H� op��o sem choque
			{
				map<int /*opcao*/, map<int /*dia*/, GGroup<HorarioAula*, Less<HorarioAula*>>>> semChoquesNaSala;

				for (int at = 0; at < mapOpcoesSalaLivre->size(); at++)
					if (choques.find(at) == choques.end())
						semChoquesNaSala[at] = (*mapOpcoesSalaLivre)[at];

				map<int /*op��o*/, GGroup<AlunoDemanda*, Less<AlunoDemanda*>>> mapOpcaoAlunosSemChoque;

				// Verifica cjt m�nimo de alunos com disponibilidade em uma das op��es sem choque
				procuraOpcoesSemChoque(semChoquesNaSala, mapAlsDemNaoAtend, mapOpcaoAlunosSemChoque);

				if (bd) cout << "\n\t\t\t4.34.3"; fflush(NULL);

				bool opcaoEncontrada = false;
				auto itAlunosSemChoque = mapOpcaoAlunosSemChoque.begin();
				for (; itAlunosSemChoque != mapOpcaoAlunosSemChoque.end(); itAlunosSemChoque++)
				{
					int opcaoIdx = itAlunosSemChoque->first;
					int nroAlunos = itAlunosSemChoque->second.size();
					bool haFormando = false;

					if (problemData->parametros->violar_min_alunos_turmas_formandos)
						ITERA_GGROUP_LESS(itAl, itAlunosSemChoque->second, AlunoDemanda)
						{
							if (itAl->getAluno()->ehFormando())
								haFormando = true;
						}

					if (nroAlunos > minAlunosNaTurma || haFormando)
					{
						// op��o encontrada para nova turma!
						opcaoEncontrada = true;

						map<int /*dia*/, GGroup<HorarioAula*, Less<HorarioAula*>>>
							*opcao = &semChoquesNaSala[opcaoIdx];

						if (printMotivosInternos)
						{
							outMotivosInternos << "\n\n----------------------------------------------";
							outMotivosInternos << "\nAnalise do nao atendimento do AlunoDemanda Id " << ad->getId();
							outMotivosInternos << "\nPossivel nova turma " << strTurmaTipoCred
								<< "no turno " << ad->demanda->getTurnoIES()->getNome() << "com seguinte alocacao:";
							outMotivosInternos << "\nDisc " << discEquiv->getId() << "  " << discEquiv->getCodigo();
							if (discEquiv->temParPratTeor())
								outMotivosInternos << " (possui par teorico/prat)";
							outMotivosInternos << " - " << discEquiv->getTotalCreditos() << " creditos.";
							outMotivosInternos << "\nSala " << sala->getId() << "  " << sala->getCodigo();
							auto itDiaOpcao = opcao->begin();
							for (; itDiaOpcao != opcao->end(); itDiaOpcao++)
							{
								outMotivosInternos << "\nDia " << itDiaOpcao->first << "\nHorarios: ";
								ITERA_GGROUP_LESS(itHor, itDiaOpcao->second, HorarioAula)
								{
									outMotivosInternos << " " << itHor->getInicio().hourMinToStr();
								}
							}

							outMotivosInternos << "\nAlsDem (" << itAlunosSemChoque->second.size() << " alunos): ";
							ITERA_GGROUP_LESS(itAl, itAlunosSemChoque->second, AlunoDemanda)
							{
								outMotivosInternos << " " << itAl->getId();
								if (itAl->getAluno()->ehFormando())
									outMotivosInternos << " (formando)";
							}
						}
					}
				}

				if (!opcaoEncontrada)
				{
					opcaoComChoqueComAlunos++;
					ss2ComChoqueComAlunos << sala->getCodigo() << "; ";
					// nenhuma das opcoes na sala atinge o m�nimo de alunos com horarios em comum
				}
				else
				{
					opcaoSemChoque++;
					ss2SemChoque << sala->getCodigo() << "; ";
				}
			}
		}

		if (bd) cout << "\n\t\t\t4.35"; fflush(NULL);

		if (opcaoSemChoque)
		{
			stringstream ss;
			ss << ss1SemChoque.str();

			if (opcaoSemChoque == 1) ss << " na sala ";
			else ss << " nas salas ";

			ss << ss2SemChoque.str();

			if (discEquiv->temParPratTeor())
				ss << " Verificar se a parte "
				<< (discEquiv->getId() < 0 ? "te�rica " : "pr�tica ")
				<< "tamb�m poderia ser alocada.";

			naoAtendimento->addMotivo(ss.str());
		}
		if (opcaoComChoqueEmSala)
		{
			stringstream ss;
			ss << ss1ComChoqueEmSala.str();

			if (opcaoComChoqueEmSala == 1) ss << " na sala ";
			else ss << " nas salas ";

			ss << ss2ComChoqueEmSala.str();

			naoAtendimento->addMotivo(ss.str());
		}
		if (opcaoComChoqueComAlunos)
		{
			stringstream ss;
			ss << ss1ComChoqueComAlunos.str();

			if (opcaoComChoqueComAlunos == 1) ss << " na sala ";
			else ss << " nas salas ";

			ss << ss2ComChoqueComAlunos.str();

			naoAtendimento->addMotivo(ss.str());
		}
	}

	if (printMotivosInternos)
	{
		outMotivosInternos.close();
	}
}

void ProblemSolution::verificaUsoDeProfsVirtuais()
{
	CentroDados::startTimer();

	cout << "\nVerifica Uso De Profs Virtuais..."; fflush(NULL);

	/*
		1) Algoritmo para, a partir de uma solu��o do modo Operacional do Trieda, determinar que professor
		real poderia substituir um professor virtual, supondo total disponibilidade de dias e hor�rios do
		professor na semana. Isto �, somente a disponibilidade do professor � alterada (considerada total),
		o professor ainda tem que ser habilitado na disciplina e sua aloca��o na aula n�o pode violar
		restri��es fortes.

		Se um professor real pode, sem qualquer altera��o em sua disponibilidade de dias e hor�rios na semana,
		substituir um professor virtual em uma aula sem violar nenhuma restri��o forte, ent�o isso indica
		poss�vel falha na solu��o.
		*/

	ProblemData *problemData = CentroDados::getProblemData();

	// Flags para a escrita dos arquivos de saida
	bool escreveAlocacoesAlteradas = false;

	ofstream saidaAlteracoesFile;
	string saidaAlteracoesFilename("saidaTestaTrocarProfVirtualPorReal_Alteracoes.txt");
	saidaAlteracoesFile.open(saidaAlteracoesFilename, ios::out);

	bool DETALHAR_TODOS_OS_MOTIVOS = true;	// N�o p�ra no primeiro motivo encontrado. Percorre todos.
	bool DETALHAR_TODOS_PROFS = false;		// Detalha at� mesmo os profs sem magisterio na disciplina ou nao cadastrados no campus
	bool IMPRIMIR_SOMENTE_RESUMO = true;

	// PERCORRE TODAS AS TURMAS COM PROFESSORES VIRTUAIS

	auto itFinder1 = mapSolTurmaProfVirtualDiaAula_.begin();
	for (; itFinder1 != mapSolTurmaProfVirtualDiaAula_.end(); itFinder1++)
	{
		int cpId = itFinder1->first;
		Campus* cp = problemData->refCampus[cpId];

		auto itFinder2 = itFinder1->second.begin();
		for (; itFinder2 != itFinder1->second.end(); itFinder2++)
		{
			Disciplina* disciplina = itFinder2->first;
			int discId = disciplina->getId();

			auto itFinder3 = itFinder2->second.begin();
			for (; itFinder3 != itFinder2->second.end(); itFinder3++)
			{
				int turma = itFinder3->first;

				vector<Aula*> listaAulasDoPar;
				ProfessorVirtualOutput* profVirtualOut = nullptr;

#pragma region AGRUPA AULAS DA TURMA E SETA PROFESSOR VIRTUAL USADO NA TURMA
				auto itFinder4 = itFinder3->second.begin();
				for (; itFinder4 != itFinder3->second.end(); itFinder4++)
				{
					profVirtualOut = itFinder4->first;

					auto itFinder5 = itFinder4->second.begin();
					for (; itFinder5 != itFinder4->second.end(); itFinder5++)
					{
						Aula *aula = itFinder5->second;

						listaAulasDoPar.push_back(aula);
					}
				}
#pragma endregion

				if (itFinder3->second.size() > 1)
				{
					stringstream msg;
					msg << "Mais de um prof ministrando aulas para a turma " << turma << " da disc " << discId;
					CentroDados::printError("void ProblemSolution::verificaUsoDeProfsVirtuais()", msg.str());
				}

				vector<pair<Aula*, Professor*> >possiveisTrocasParcial;
				vector<pair<Aula*, Professor*> > alocacoesAlteradasParcial;

				map<pair<int, int>, pair<Aula, int> > variableOpsNaoDisponiveisHorarioIdsHorAulas;

#pragma region FLAGS
				// Flags para motivos comuns a todos os professores reais
				int flagFaltaCadastroNoCampus = -1;
				int flagFaltaMagistrado = -1;
				int flagFaltaMestreDoutor = -1;
				int flagFaltaCargaHoraria = -1;
				int flagFaltaHorarioConflito = -1;
				int flagFaltaHorarioDisponivel = -1;
				int flagFaltaMaxDiasSemana = -1;
				int flagFaltaMinCredsDiarios = -1;
				int flagFaltaInterjornada = -1;

				// Flags para resumo
				int flagNroFaltaCadastroNoCampus = 0;
				int flagNroFaltaMagistrado = 0;
				int flagNroFaltaMestreDoutor = 0;
				int flagNroFaltaCargaHoraria = 0;
				int flagNroFaltaHorarioConflito = 0;
				int flagNroFaltaHorarioDisponivel = 0;
				int flagNroFaltaMaxDiasSemana = 0;
				int flagNroFaltaMinCredsDiarios = 0;
				int flagNroFaltaInterjornada = 0;
#pragma endregion

				Aula* aula0 = listaAulasDoPar[0];

				bool ehPrat = (aula0->getCreditosPraticos() > 0);

#pragma region PROCURA OBJETOS AlocacaoProfVirtual E TipoTitulacao
				AlocacaoProfVirtual *alocacao = profVirtualOut->getAlocacao(abs(discId), turma, cpId, ehPrat);
				if (alocacao == nullptr)
				{
					stringstream msg;
					msg << "Alocacao nao encontrada. Disc" << abs(discId)
						<< ", turma " << turma << ", cp " << cpId << ", pratica " << ehPrat;
					CentroDados::printError("void ProblemSolution::verificaUsoDeProfsVirtuais()", msg.str());
					continue;
				}

				TipoTitulacao *titulacaoProfVirt = problemData->retornaTitulacaoPorId(profVirtualOut->getTitulacaoId());
				if (titulacaoProfVirt == nullptr)
				{
					stringstream msg;
					msg << "Titulacao id " << profVirtualOut->getTitulacaoId() << " nao encontrada";
					CentroDados::printError("void ProblemSolution::verificaUsoDeProfsVirtuais()", msg.str());
					continue;
				}
#pragma endregion

#pragma region CONFERE TODOS OS PROFESSORES REAIS POSS�VEIS PARA A TURMA

				//Para cada aula confere todos os professores
				auto itProf = problemData->refProfessores.begin();
				for (; itProf != problemData->refProfessores.end(); itProf++)
				{
					Professor* p = itProf->second;
					int idProfReal = p->getId();

					// Flags por professor real
					int flagProfSemCadastroNoCampus = -1;
					int flagProfSemMagisterio = -1;
					int flagProfSemTituloMinimo = -1;
					int flagProfCHMaxima = -1;
					int flagProfHorarioConflito = 0;
					int flagProfFaltaHorarioDisponivel = 0;
					int flagProfViolaInterjornada = 0;
					int flagProfViolaMaxDias = 0;
					int flagProfViolaMinCredsDiarios = 0;

					//Flag para indicar se o professor atual p pode assumir a turma do professor virtual considerando
					//disposi��o total de horarios daquele e sem violar restric�es fortes.
					int flagPossivelTroca = 1;

					//Flag para indicar que possivel o professor p assumir a turma de um professor virtual nao violando restri��es fortes
					//e n�o violando a disposi��o de horarios daquele. (Indica possivel falha no modelo matematico)
					int flagAltera = 1;

					//// 1. No algoritmo primeiro � verificado se � possivel trocar o professor virtual responsavel pelo trio<Disciplina,turma,campus> pelo professor real
					//para isso ele confere se o professor real � magistrado para aquela disciplina, depois confere se ao atribuir essa disciplina/turma
					//n�o ir� ultrapassar a carga horaria maxima semanal do professor. Feito isso o algoritmo checa os horarios. Primeiramente o dia, depois o turno
					//e em segida se n�o h� conflito nos horarios ja alocados para o professor real. Se todas essa restri��es forem atendidas a flag de que pode-se
					//trocar o professor � setada parcialmente em 1.

					//// 2. Na segunda etapa se � possivel fazer uma troca entre os professores ent�o o algoritmo checa se � possivel alterar dentro da disponibilidade
					//do professor real. Para isso � checado se h� disponibilidade do professor no dia, turno e enfim no horario seguindo essa ordem. Se existir a
					//flag de altera��o � ativada.

					flagProfSemCadastroNoCampus = !(cp->professores.find(p) != cp->professores.end());

					flagFaltaCadastroNoCampus = flagFaltaCadastroNoCampus && flagProfSemCadastroNoCampus;

					flagNroFaltaCadastroNoCampus += (flagProfSemCadastroNoCampus ? 1 : 0);

					if (!flagProfSemCadastroNoCampus || DETALHAR_TODOS_PROFS)
					{
						flagProfSemMagisterio = !(p->possuiMagisterioEm(aula0->getDisciplina()));

						flagFaltaMagistrado = flagFaltaMagistrado && flagProfSemMagisterio;

						flagNroFaltaMagistrado += (flagProfSemMagisterio ? 1 : 0);

						if (!flagProfSemMagisterio || DETALHAR_TODOS_PROFS) // Se possui magisterio
						{
							flagProfSemTituloMinimo = !(p->getTitulacao() >= titulacaoProfVirt->getTitulacao());

							flagFaltaMestreDoutor = flagFaltaMestreDoutor && flagProfSemTituloMinimo;

							flagNroFaltaMestreDoutor += (flagProfSemTituloMinimo ? 1 : 0);

							if (!flagProfSemTituloMinimo || DETALHAR_TODOS_OS_MOTIVOS) // Se a titula��o for maior ou igual
							{
								int acrescimo = aula0->getDisciplina()->getTotalCreditos() *
									aula0->getDisciplina()->getTempoCredSemanaLetiva();

								flagProfCHMaxima = !(quantChProfs_[p->getId()].first + acrescimo <= p->getChMax() * 60);

								flagFaltaCargaHoraria = flagFaltaCargaHoraria && flagProfCHMaxima;

								flagNroFaltaCargaHoraria += (flagProfCHMaxima ? 1 : 0);

								if (!flagProfCHMaxima || DETALHAR_TODOS_OS_MOTIVOS) // Se a carga horaria maxima desse professor nao � violada
								{
									auto itDiaHorsAlocados = mapSolProfRealDiaHorarios_.find(p);

									// -------------------------------------------------
									// Percorre todas as aulas do trio e confere
									for (int i = 0; i < listaAulasDoPar.size(); i++)
									{
#pragma region CONFERE A I-�SIMA AULA DA TURMA

										if (!flagPossivelTroca && !DETALHAR_TODOS_OS_MOTIVOS){
											break;
										}

										Aula* aula = listaAulasDoPar[i];

										HorarioAula* horarioAulaV = aula->getHorarioAulaInicial();
										int nCredDia = aula->getTotalCreditos();

										// Horarios ocupados no dia do professor
										vector<HorarioDia*> horariosAlocadosNoDia;
										if (itDiaHorsAlocados != mapSolProfRealDiaHorarios_.end())
										{
											int nroDiasUsados = (int)itDiaHorsAlocados->second.size();

											auto itHorsAlocados = itDiaHorsAlocados->second.find(aula->getDiaSemana());
											if (itHorsAlocados != itDiaHorsAlocados->second.end())
											{
												horariosAlocadosNoDia = itHorsAlocados->second;
											}
											else
											{
												if (nroDiasUsados + 1 > p->getMaxDiasSemana())
												{
													flagProfViolaMaxDias++;
													flagAltera = 0;
												}

												flagFaltaMaxDiasSemana = flagFaltaMaxDiasSemana && flagProfViolaMaxDias;

												if (nCredDia < p->getMinCredsDiarios())
												{
													int gapViolado = p->getMinCredsDiarios() - nCredDia;
													if (flagProfViolaMinCredsDiarios < gapViolado)
														flagProfViolaMinCredsDiarios = gapViolado;
													flagAltera = 0;
												}

												flagFaltaMinCredsDiarios = flagFaltaMinCredsDiarios && flagProfViolaMinCredsDiarios;
											}
										}

										// Confere para todos os horarios da turma no dia da VariableOp v
										for (int k = 1; k <= nCredDia; k++)
										{
#pragma region CONFERE CONFLITOS DO HORARIO COM OUTRAS ALOCACOES DO PROFESSOR
											// Confere se existe conflito entre os horarios da VariavelOp e os horarios alocados ao professor
											if (horariosAlocadosNoDia.size() > 0)
											{
												for (int l = 0; l < horariosAlocadosNoDia.size(); l++)
												{
													HorarioDia *h = horariosAlocadosNoDia[l];
													HorarioAula* hAula = h->getHorarioAula();

													if ((hAula->getInicio() >= horarioAulaV->getInicio()) && (hAula->getInicio() < horarioAulaV->getFinal()) ||
														(hAula->getFinal() > horarioAulaV->getInicio()) && (hAula->getFinal() <= horarioAulaV->getFinal()))
													{
														// H� conflito
														// Seta as flags para zero
														flagAltera = 0;
														flagPossivelTroca = 0;
														flagProfHorarioConflito = 1;
													}
												}
											}
											else{ // Se o professor n�o tem nenhum horario alocado ent�o confere se existe algum horario diponivel que de certo
												flagPossivelTroca = 1;
											}

#pragma endregion

#pragma region CONFERE SE O HORARIO � HABILITADO NO CADASTRO DO PROFESSOR
											int flagEncontrouHorario = 0;

											// Procura se o HorarioDia est� nos hor�rios de disponibilidade do professor
											GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
												itrHorDispProf = p->horariosDia.begin();
											for (; itrHorDispProf != p->horariosDia.end(); itrHorDispProf++)
											{
												HorarioAula* horarioDisp = itrHorDispProf->getHorarioAula();

												if (aula->getDiaSemana() == itrHorDispProf->getDia()){		// Se for no mesmo dia
													if (horarioAulaV->inicioFimIguais(*horarioDisp)){	// Se for no mesmo horario
														flagEncontrouHorario = 1;
														break;
													}
												}
											}
											flagAltera = flagAltera && flagEncontrouHorario;

											if (!flagEncontrouHorario)
											{
												flagProfFaltaHorarioDisponivel = 1;

												pair<int, int> chaveMap(aula->getDiaSemana(), horarioAulaV->getId());

												if (variableOpsNaoDisponiveisHorarioIdsHorAulas.find(chaveMap) == variableOpsNaoDisponiveisHorarioIdsHorAulas.end()){
													pair<Aula, int> pairVHorario(*aula, horarioAulaV->getId());
													variableOpsNaoDisponiveisHorarioIdsHorAulas[chaveMap] = pairVHorario;
												}
											}
#pragma endregion

											if (k < nCredDia)
												horarioAulaV = horarioAulaV->getCalendario()->getProximoHorario(horarioAulaV);
										}

#pragma region CONFERE INTERJORNADA MINIMA DO PROFESSOR
										// Confere interjornada
										if (problemData->parametros->considerarDescansoMinProf)
										{
											int tempoMinimoDescanso = (int)problemData->parametros->descansoMinProfValue;	// em horas
											tempoMinimoDescanso *= 60;													// em minutos

											// Dia seguinte
											DateTime dtf_maisDescanso = horarioAulaV->getFinal();
											dtf_maisDescanso.addMinutes(tempoMinimoDescanso);
											if (horarioAulaV->getFinal().getDay() != dtf_maisDescanso.getDay())
											{
												// Verifica se o professor esta alocado em algum horario h no dia seguinte
												// tal que h < dtf_maisDescanso
												if (itDiaHorsAlocados != mapSolProfRealDiaHorarios_.end())
												{
													auto itHorsDiaSeg = itDiaHorsAlocados->second.find(aula->getDiaSemana() + 1);
													if (itHorsDiaSeg != itDiaHorsAlocados->second.end())
													{
														for (int at = 0; at < itHorsDiaSeg->second.size(); at++)
														{
															HorarioDia* hd = itHorsDiaSeg->second[at];
															if (less_than(hd->getHorarioAula()->getInicio(), dtf_maisDescanso))
															{
																// Viola interjornada m�nima
																flagProfViolaInterjornada = 1;
																flagAltera = 0;
																flagPossivelTroca = 0;
																break;
															}
														}
													}
												}
											}

											// Dia anterior
											DateTime dti_menosDescanso = horarioAulaV->getInicio();
											dti_menosDescanso.subMinutes(tempoMinimoDescanso);
											if (horarioAulaV->getFinal().getDay() != dti_menosDescanso.getDay())
											{
												// Verifica se o professor esta alocado em algum horario h no dia anterior
												// tal que h > dti_menosDescanso									
												if (itDiaHorsAlocados != mapSolProfRealDiaHorarios_.end())
												{
													auto itHorsDiaAnt = itDiaHorsAlocados->second.find(aula->getDiaSemana() - 1);
													if (itHorsDiaAnt != itDiaHorsAlocados->second.end())
													{
														for (int at = 0; at < itHorsDiaAnt->second.size(); at++)
														{
															HorarioDia *hd = itHorsDiaAnt->second[at];
															if (less_than(dti_menosDescanso, hd->getHorarioAula()->getFinal()))
															{
																// Viola interjornada m�nima
																flagProfViolaInterjornada = 1;
																flagAltera = 0;
																flagPossivelTroca = 0;
																break;
															}
														}
													}
												}
											}
										}
#pragma endregion

										if (flagPossivelTroca && flagProfFaltaHorarioDisponivel){
											possiveisTrocasParcial.push_back(pair<Aula*, Professor*>(aula, p));
										}
										if (flagAltera){
											alocacoesAlteradasParcial.push_back(pair<Aula*, Professor*>(aula, p));
										}
#pragma endregion
										// -------------------------------------------------
									}

									flagFaltaHorarioConflito = flagFaltaHorarioConflito && flagProfHorarioConflito;
									flagFaltaHorarioDisponivel = flagFaltaHorarioDisponivel && flagProfFaltaHorarioDisponivel;
									flagFaltaInterjornada = flagFaltaInterjornada && flagProfViolaInterjornada;

									flagNroFaltaHorarioConflito += (flagProfHorarioConflito ? 1 : 0);
									flagNroFaltaHorarioDisponivel += (flagProfFaltaHorarioDisponivel ? 1 : 0);
									flagNroFaltaInterjornada += (flagProfViolaInterjornada ? 1 : 0);
								}
							}
						}
					}

					if (!IMPRIMIR_SOMENTE_RESUMO)
					{
						if (flagProfSemCadastroNoCampus == 1)
						{
							alocacao->addDescricaoDoMotivo(idProfReal, "N�o possui cadastro no campus da turma.");
							flagAltera = 0;
							flagPossivelTroca = 0;
						}
						if (flagProfSemMagisterio == 1)
						{
							alocacao->addDescricaoDoMotivo(idProfReal, "N�o possui habilita��o nesta disciplina.");
							flagAltera = 0;
							flagPossivelTroca = 0;
						}
						if (flagProfSemTituloMinimo == 1)
						{
							stringstream ss;
							ss << "Titula��o do professor n�o suficiente. M�nimo t�tulo necess�rio: "
								<< titulacaoProfVirt->getNome() << ".";
							alocacao->addDescricaoDoMotivo(idProfReal, ss.str());
							flagAltera = 0;
							flagPossivelTroca = 0;
						}
						if (flagProfCHMaxima == 1)
						{
							alocacao->addDescricaoDoMotivo(idProfReal, "Ultrapassaria a carga hor�ria m�xima.");
							flagAltera = 0;
							flagPossivelTroca = 0;
						}
						if (flagProfHorarioConflito == 1)
						{
							alocacao->addDescricaoDoMotivo(idProfReal, "Haveria conflito nos hor�rios da turma com outras aloca��es do professor.");
						}
						if (flagProfFaltaHorarioDisponivel == 1)
						{
							alocacao->addDescricaoDoMotivo(idProfReal, "Hor�rios da turma n�o presentes na disponibilidade do professor.");
						}
						if (flagProfViolaInterjornada == 1)
						{
							alocacao->addDescricaoDoMotivo(idProfReal, "Violaria interjornada m�nima do professor.");
						}
						if (flagProfViolaMaxDias > 0)
						{
							alocacao->addDescricaoDoMotivo(idProfReal, "Violaria o m�ximo de dias na semana para o professor.");
						}
						if (flagProfViolaMinCredsDiarios > 0)
						{
							alocacao->addDescricaoDoMotivo(idProfReal, "Violaria o m�nimo de cr�ditos di�rios do professor.");
						}
					}

					if (flagPossivelTroca)
					{
						if (possiveisTrocasParcial.size() > 0)
						{
							stringstream ss;
							ss << "Incluir na disponibilidade do professor os hor�rios:";
							for (int i = 0; i < possiveisTrocasParcial.size(); i++)
							{
								Aula* aula = possiveisTrocasParcial[i].first;
								int hour1 = aula->getHorarioAulaInicial()->getInicio().getHour();
								int min1 = aula->getHorarioAulaInicial()->getInicio().getMinute();
								int ncreds = aula->getTotalCreditos();
								int tempoAula = aula->getDisciplina()->getTempoCredSemanaLetiva();

								DateTime final = aula->getHorarioAulaInicial()->getInicio();
								final.addMinutes(ncreds*tempoAula);
								int hour2 = final.getHour();
								int min2 = final.getMinute();
								ss << " de " << hour1 << ":" << (min1 < 10 ? "0" : "") << min1;
								ss << " a " << hour2 << ":" << (min2 < 10 ? "0" : "") << min2;
								ss << " n" << problemData->getDiaEmString(aula->getDiaSemana(), true) << ";";
							}
							alocacao->addDicaEliminacao(possiveisTrocasParcial[0].second->getId(), ss.str());
						}
						if (flagProfViolaMaxDias)
						{
							stringstream ss;
							ss << "Aumentar o m�ximo de dias na semana do professor para "
								<< p->getMaxDiasSemana() + flagProfViolaMaxDias << ".";
							alocacao->addDicaEliminacao(p->getId(), ss.str());
						}
						if (flagProfViolaMinCredsDiarios)
						{
							stringstream ss;
							ss << "Alterar o m�nimo de cr�ditos no dia do professor para "
								<< p->getMinCredsDiarios() - flagProfViolaMinCredsDiarios << ".";
							alocacao->addDicaEliminacao(p->getId(), ss.str());
						}
					}
					possiveisTrocasParcial.clear();

					if (flagAltera)
					{
						flagFaltaHorarioDisponivel = 0;
						variableOpsNaoDisponiveisHorarioIdsHorAulas.clear();

						if (!escreveAlocacoesAlteradas)
						{
							escreveAlocacoesAlteradas = true;
							if (!saidaAlteracoesFile){
								cout << "Can't open output file " << saidaAlteracoesFilename << endl;
							}
							else{
								saidaAlteracoesFile << "-------------------------------------------------------------------------------------------------" << endl;
								saidaAlteracoesFile << "------------------------ALTERACOES ENTRE DE PROFESSORES VIRTUAIS POR REAIS-----------------------" << endl;
								saidaAlteracoesFile << "---------Considerando todas as restri��es fortes, ou seja, indica possivel erro no modelo--------" << endl;
								saidaAlteracoesFile << "-------------------------------------------------------------------------------------------------\n" << endl;
								saidaAlteracoesFile << "=================================================================================================\n" << endl;
							}
						}

						if (saidaAlteracoesFile)
						{
							for (int i = 0; i < alocacoesAlteradasParcial.size(); i++)
							{
								pair<Aula*, Professor*> alteracao = alocacoesAlteradasParcial[i];

								saidaAlteracoesFile << "Aula IDENTIFICADA:" << endl;
								saidaAlteracoesFile << alteracao.first->toString() << endl << endl;
								saidaAlteracoesFile << "PROFESSOR REAL POSSIVEL PARA ALTERACAO:" << endl;
								saidaAlteracoesFile << "ID: " << alteracao.second->getId() << endl;
								saidaAlteracoesFile << "NOME: " << alteracao.second->getNome() << endl;
								saidaAlteracoesFile << "CPF: " << alteracao.second->getCpf() << endl;
								saidaAlteracoesFile << "TITULO: " << alteracao.second->titulacao->getNome() << endl;
								saidaAlteracoesFile << "VALOR CREDITO: " << alteracao.second->getValorCredito() << endl << endl;
								saidaAlteracoesFile << "=================================================================================================\n" << endl;
							}
						}
					}

					alocacoesAlteradasParcial.clear();
				}

#pragma endregion

#pragma region IMPRIME CAUSAS E O NRO DE PROFESSORES ASSOCIADOS � CAUSA
				if (IMPRIMIR_SOMENTE_RESUMO)
				{
					//Gera o relatorio de motivos da cria��o de professores virtuais

					if (flagNroFaltaCadastroNoCampus){
						stringstream ss;
						if (flagNroFaltaCadastroNoCampus == 1)
							ss << flagNroFaltaCadastroNoCampus << " professor n�o cadastrado no campus dessa turma.";
						else
							ss << flagNroFaltaCadastroNoCampus << " professores n�o cadastrados no campus dessa turma.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
					if (flagNroFaltaMagistrado){
						stringstream ss;
						if (flagNroFaltaMagistrado == 1)
							ss << flagNroFaltaMagistrado << " professor n�o possui habilita��o nesta disciplina.";
						else
							ss << flagNroFaltaMagistrado << " professores n�o possuem habilita��o nesta disciplina.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
					if (flagNroFaltaMestreDoutor){
						stringstream ss;
						if (flagNroFaltaMestreDoutor == 1)
							ss << flagNroFaltaMestreDoutor << " professor n�o possui titula��o necess�ria para manter o m�nimo de mestres e doutores no curso atendido pela turma.";
						else
							ss << flagNroFaltaMestreDoutor << " professores n�o possuem titula��o necess�ria para manter o m�nimo de mestres e doutores no curso atendido pela turma.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
					if (flagNroFaltaCargaHoraria){
						stringstream ss;
						if (flagNroFaltaCargaHoraria == 1)
							ss << flagNroFaltaCargaHoraria << " professor excederia a carga hor�ria m�xima semanal.";
						else
							ss << flagNroFaltaCargaHoraria << " professores excederiam a carga hor�ria m�xima semanal.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
					if (flagNroFaltaHorarioConflito){
						stringstream ss;
						ss << "Para ";
						if (flagNroFaltaHorarioConflito == 1)
							ss << flagNroFaltaHorarioConflito << " professor h� conflito entre os hor�rios da turma com os hor�rios de outras turmas alocadas.";
						else
							ss << flagNroFaltaHorarioConflito << " professores h� conflito entre os hor�rios da turma com os hor�rios de outras turmas alocadas.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
					if (flagNroFaltaHorarioDisponivel && (!variableOpsNaoDisponiveisHorarioIdsHorAulas.empty())){
						stringstream ss;
						if (flagNroFaltaHorarioDisponivel == 1)
							ss << flagNroFaltaHorarioDisponivel << " professor n�o possui hor�rios dispon�veis para esta turma.";
						else
							ss << flagNroFaltaHorarioDisponivel << " professores n�o possuem hor�rios dispon�veis para esta turma.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
					if (flagNroFaltaMaxDiasSemana){
						stringstream ss;
						if (flagNroFaltaMaxDiasSemana == 1)
							ss << flagNroFaltaMaxDiasSemana << " professor j� atingiu o m�ximo de dias da semana.";
						else
							ss << flagNroFaltaMaxDiasSemana << " professores j� atingiram o m�ximo de dias da semana.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
					if (flagNroFaltaMinCredsDiarios){
						stringstream ss;
						ss << "O m�nimo de cr�ditos nos dias das aulas da turma n�o foi atingido para ";
						if (flagNroFaltaMinCredsDiarios == 1)
							ss << flagNroFaltaMinCredsDiarios << " professor.";
						else
							ss << flagNroFaltaMinCredsDiarios << " professores.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
					if (flagNroFaltaInterjornada){
						stringstream ss;
						if (flagNroFaltaInterjornada == 1)
							ss << flagNroFaltaInterjornada << " professor j� atingiu a interjornada m�nima.";
						else
							ss << flagNroFaltaInterjornada << " professores j� atingiram a interjornada m�nima.";
						alocacao->addDescricaoDoMotivo(-1, ss.str());
					}
				}
#pragma endregion

#pragma region IMPRIME CAUSAS COMUNS A TODOS OS PROFESSORES
				if (!IMPRIMIR_SOMENTE_RESUMO)
				{
					//Gera o relatorio de motivos da cria��o de professores virtuais

					if (flagFaltaCadastroNoCampus == 1){
						alocacao->addDescricaoDoMotivo(-1, "N�o existe professor cadastrado no campus dessa turma.");
					}
					if (flagFaltaMagistrado == 1){
						alocacao->addDescricaoDoMotivo(-1, "N�o existe professor com habilita��o nesta disciplina.");
					}
					if (flagFaltaMestreDoutor == 1){
						alocacao->addDescricaoDoMotivo(-1, "N�o existe professor com titula��o necess�ria para manter o m�nimo de mestres e doutores no curso atendido pela turma.");
					}
					if (flagFaltaCargaHoraria == 1){
						alocacao->addDescricaoDoMotivo(-1, "N�o existe professor capaz de ministrar esta turma sem exceder a sua carga horaria maxima semanal.");
					}
					if (flagFaltaHorarioConflito == 1){
						alocacao->addDescricaoDoMotivo(-1, "H� conflito entre os hor�rios da turma com os hor�rios de outras turmas alocadas aos professores.");
					}
					if ((flagFaltaHorarioDisponivel == 1) && (!variableOpsNaoDisponiveisHorarioIdsHorAulas.empty())){
						alocacao->addDescricaoDoMotivo(-1, "N�o h� horarios disponiveis para esta turma.");
					}
					if (flagFaltaMaxDiasSemana > 0){
						alocacao->addDescricaoDoMotivo(-1, "Todos os professores j� atingiram o m�ximo de dias da semana.");
					}
					if (flagFaltaMinCredsDiarios > 0){
						alocacao->addDescricaoDoMotivo(-1, "O m�nimo de cr�ditos nos dias das aulas da turma n�o foi atingido para nenhum professor.");
					}
					if (flagFaltaInterjornada == 1){
						alocacao->addDescricaoDoMotivo(-1, "Todos os professores j� atingiram a interjornada m�nima.");
					}
				}
#pragma endregion
			}
		}
	}

	if (saidaAlteracoesFile)
		saidaAlteracoesFile.close();

	double dif = CentroDados::getLastRunTime();
	CentroDados::stopTimer();
	cout << " " << dif << "sec";

	verificaNrDiscSimultVirtual();
}

void ProblemSolution::computaMotivos(bool motivoNaoAtend, bool motivoUsoPV)
{
	cout << "Computando motivos..." << endl;

	clearMapsDaSolucao();
	constroiMapsDaSolucao();
	imprimeMapsDaSolucao();
	if (motivoNaoAtend) verificaNaoAtendimentosTaticos();
	if (motivoUsoPV) verificaUsoDeProfsVirtuais();
	clearMapsDaSolucao();
}

void ProblemSolution::imprimeIndicadores()
{
	cout << "\timprimindo Indicadores..."; fflush(0);

	// -----------------------------------------------------------------------------------------------
#pragma region TURMAS COM COMPARTILHAMENTO DE CURSOS
	Indicadores::printSeparator(1);

	auto itCpId = mapSolTurmaCursos_.begin();
	for (; itCpId != mapSolTurmaCursos_.end(); itCpId++)
	{
		int nroTurmasMaisDeUmCurso = 0;
		auto itDisc = itCpId->second.begin();
		for (; itDisc != itCpId->second.end(); itDisc++)
		{
			// considera turmas te�ricas apenas
			if (itDisc->first->getId() < 0)
				continue;

			auto itTurma = itDisc->second.begin();
			for (; itTurma != itDisc->second.end(); itTurma++)
			{
				int nCursos = itTurma->second.size();
				if (nCursos > 1) nroTurmasMaisDeUmCurso++;
			}
		}
		stringstream msg;
		msg << "\nNumero de turmas com compartilhamento de curso no campus " << itCpId->first << ": " << nroTurmasMaisDeUmCurso;
		Indicadores::printIndicador(msg.str());
	}
#pragma endregion

#pragma region GERAL
	Indicadores::printSeparator(2);
	Indicadores::printIndicador("\nNumero total de alunos-demanda de P1: ", getNroTotalAlunoDemandaP1());
	Indicadores::printIndicador("\nNumero total de alunos-demanda de P1 atendidos: ", getNroAlunoDemAtendP1());
	Indicadores::printIndicador("\nNumero total de alunos-demanda de P1 n�o atendidos: ", getNroAlunoDemNaoAtendP1());
	Indicadores::printIndicador("\nNumero total de alunos-demanda de P2 atendidos: ", getNroAlunoDemAtendP2());
	Indicadores::printIndicador("\nNumero total de alunos-demanda de P1+P2 atendidos: ", getNroAlunoDemAtendP1P2());
#pragma endregion

#pragma region FORMANDOS
	Indicadores::printSeparator(1);
	double nroAlDemFormP1 = getNroTotalAlunoDemFormandosP1();
	double nroAlDemFormAtendP1 = getNroTotalAlunoDemFormandosAtendP1();
	double percForm = (nroAlDemFormP1 > 0 ? (100 * nroAlDemFormAtendP1 / nroAlDemFormP1) : 100);
	Indicadores::printIndicador("\nNumero total de alunos-demanda de formandos P1: ", (int)nroAlDemFormP1);
	stringstream msgFormAtend;
	msgFormAtend << "\nNumero total de alunos-demanda de formandos P1 atendidos: "
		<< (int)nroAlDemFormAtendP1 << " (" << percForm << "%)";
	Indicadores::printIndicador(msgFormAtend.str());
#pragma endregion

#pragma region CALOUROS
	Indicadores::printSeparator(1);
	double nroAlDemCalouroP1 = getNroTotalAlunoDemCalourosP1();
	double nroAlDemCalouroAtendP1 = getNroTotalAlunoDemCalourosAtendP1();
	double percCalouro = (nroAlDemFormP1 > 0 ? (100 * nroAlDemCalouroAtendP1 / nroAlDemCalouroP1) : 100);
	Indicadores::printIndicador("\nNumero total de alunos-demanda de calouros P1: ", (int)nroAlDemCalouroP1);
	stringstream msgCalouAtend;
	msgCalouAtend << "\nNumero total de alunos-demanda de calouros P1 atendidos: "
		<< (int)nroAlDemCalouroAtendP1 << " (" << percCalouro << "%)";
	Indicadores::printIndicador(msgCalouAtend.str());
#pragma endregion
}

void ProblemSolution::imprimeMapsDaSolucao()
{
	cout << "Imprime Maps Da Solucao..." << endl;

	// Map de aulas da solu��o por turma
	imprimeMapSolDiscTurmaDiaAula();

	// Maps de aulas da solu��o por aluno
	imprimeMapSolAlunoDiaDiscAulas();
	imprimeMapAlunoDiscTurmaCp();

	// Map de aulas da solu��o por sala
	imprimeMapSalaDiaHorariosVagos();

	// Maps de aulas da solu��o com professor
	imprimeMapSolTurmaProfVirtualDiaAula();
	imprimeMapSolProfRealDiaHorarios();
	imprimeQuantChProfs();

	// Histograma para disciplinas com profs virtuais e aulas simultaneas
	imprimeNrDiscSimultVirtual();
}

void ProblemSolution::imprimeMapSolDiscTurmaDiaAula()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solDiscTurmaDiaAula.txt");
	outFile.open(fileName, ofstream::out);

	if (!outFile)
	{
		cout << "\nErro em void ProblemSolution::imprimeMapSolDiscTurmaDiaAula():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	auto itMap1 = mapSolDiscTurmaDiaAula_.begin();
	for (; itMap1 != mapSolDiscTurmaDiaAula_.end(); itMap1++)
	{
		int campusId = itMap1->first;

		map<Disciplina*, map<int /*turma*/, pair<map<int /*dia*/, Aula*>,
			GGroup<int /*alDem*/>>>, Less<Disciplina*>> *map2 = &itMap1->second;

		auto itMap2 = (*map2).begin();
		for (; itMap2 != (*map2).end(); itMap2++)
		{
			Disciplina *disc = itMap2->first;

			map < int /*turma*/, pair< map<int /*dia*/, Aula*>,
				GGroup<int /*alDem*/> > > *map3 = &itMap2->second;

			auto itMap3 = (*map3).begin();
			for (; itMap3 != (*map3).end(); itMap3++)
			{
				int turma = itMap3->first;

				outFile << "\n\n========================================================================="
					<< "\nCampus " << campusId << ", Turma " << turma << ", Disciplina " << disc->getId()
					<< "  -  " << disc->getTotalCreditos() << " creditos";

				pair< map<int /*dia*/, Aula*>, GGroup<int /*alDem*/> >
					*pairMapDiaAulasAlsDem = &itMap3->second;

				GGroup<int /*alDem*/> *alunosDemandaId = &pairMapDiaAulasAlsDem->second;

				outFile << "\n--------";
				outFile << "\nAlunosDemanda (" << alunosDemandaId->size() << " alunos): ";
				ITERA_GGROUP_N_PT(itAlDemId, (*alunosDemandaId), int)
				{
					outFile << *itAlDemId << "; ";
				}
				outFile << "\n--------";

				map<int /*dia*/, Aula*> *mapDiaAula = &pairMapDiaAulasAlsDem->first;

				for (auto itDiaAula = mapDiaAula->begin(); itDiaAula != mapDiaAula->end(); itDiaAula++)
				{
					Aula* aula = itDiaAula->second;

					outFile << "\nDia " << aula->getDiaSemana();
					outFile << ", Sala " << aula->getSala()->getId();

					if (aula->getHorarioAulaInicial() != NULL)
						outFile << ", Hi " << aula->getHorarioAulaInicial()->getInicio().getHour() << ":"
						<< aula->getHorarioAulaInicial()->getInicio().getMinute()
						<< " (id" << aula->getHorarioAulaInicial()->getId() << ")";
					if (aula->getHorarioAulaFinal() != NULL)
						outFile << ", Hf " << aula->getHorarioAulaFinal()->getInicio().getHour() << ":"
						<< aula->getHorarioAulaFinal()->getInicio().getMinute() << " (id"
						<< aula->getHorarioAulaFinal()->getId() << ")";

					outFile << "; " << aula->getTotalCreditos() << " creditos";
				}
			}
		}
	}
	outFile.close();
}

void ProblemSolution::imprimeMapSolAlunoDiaDiscAulas()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solAlunoDiaDiscAulas.txt");
	outFile.open(fileName, ofstream::out);

	if (!outFile)
	{
		cout << "\nErro em void ProblemSolution::imprimeMapSolAlunoDiaDiscAulas():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for (auto itAluno = mapAlunoSolution_.begin(); itAluno != mapAlunoSolution_.end(); itAluno++)
	{
		Aluno* aluno = itAluno->first;
		AlunoSolution *alSol = itAluno->second;

		alSol->mapDiaDiscAulasToStream(outFile);
	}

	outFile.close();
}

void ProblemSolution::imprimeMapAlunoDiscTurmaCp()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solAlunoDiscTurmaCp.txt");
	outFile.open(fileName, ofstream::out);

	if (!outFile)
	{
		cout << "\nErro em void ProblemSolution::imprimeMapAlunoDiscTurmaCp():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for (auto itAluno = mapAlunoSolution_.begin(); itAluno != mapAlunoSolution_.end(); itAluno++)
	{
		Aluno* aluno = itAluno->first;
		AlunoSolution *alSol = itAluno->second;

		alSol->mapAlDemDiscTurmaCpToStream(outFile);
	}

	outFile.close();
}

void ProblemSolution::imprimeMapSalaDiaHorariosVagos()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solSalaDiaHorariosVagos.txt");
	outFile.open(fileName, ofstream::out);

	if (!outFile)
	{
		cout << "\nErro em void ProblemSolution::imprimeMapSalaDiaHorariosVagos():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for (auto itMapSala = mapSalaDiaHorariosVagos_.begin(); itMapSala != mapSalaDiaHorariosVagos_.end(); itMapSala++)
	{
		Sala* sala = itMapSala->first;

		outFile << "\n\n-----------------------------------------------------------------------";
		outFile << "\nSala " << sala->getId() << "\t" << sala->getCodigo();
		for (auto itMapDia = itMapSala->second.begin(); itMapDia != itMapSala->second.end(); itMapDia++)
		{
			int dia = itMapDia->first;

			outFile << "\n\tDia " << dia << ": ";
			for (auto itDti = itMapDia->second.begin(); itDti != itMapDia->second.end(); itDti++)
			{
				DateTime dti = itDti->first;

				for (auto itDtf = itDti->second.begin(); itDtf != itDti->second.end(); itDtf++)
				{
					DateTime dtf = itDtf->first;

					outFile << "  " << dti.getHour() << ":" << dti.getMinute()
						<< " - " << dtf.getHour() << ":" << dtf.getMinute() << " || ";
				}
			}
		}
	}

	outFile.close();
}

void ProblemSolution::imprimeMapSolTurmaProfVirtualDiaAula()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solTurmaProfVirtualDiaAula.txt");
	outFile.open(fileName, ofstream::out);

	if (!outFile)
	{
		cout << "\nErro em void ProblemSolution::imprimeMapSolTurmaProfVirtualDiaAula():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for (auto itMapCpId = mapSolTurmaProfVirtualDiaAula_.begin(); itMapCpId != mapSolTurmaProfVirtualDiaAula_.end(); itMapCpId++)
	{
		int cpId = itMapCpId->first;

		outFile << "\n\n-----------------------------------------------------------------------";
		outFile << "\nCampus " << cpId;
		for (auto itDisc = itMapCpId->second.begin(); itDisc != itMapCpId->second.end(); itDisc++)
		{
			Disciplina *disciplina = itDisc->first;

			outFile << "\nDisc " << disciplina->getId();

			for (auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++)
			{
				int turma = itTurma->first;

				outFile << "\n\tTurma " << turma;

				for (auto itPV = itTurma->second.begin(); itPV != itTurma->second.end(); itPV++)
				{
					outFile << "\n\t\tProf " << itPV->first->getId();

					for (auto itDiaAula = itPV->second.begin(); itDiaAula != itPV->second.end(); itDiaAula++)
					{
						outFile << "\n\t\t\tDia " << itDiaAula->first;
						outFile << ", Aula " << itDiaAula->second->toString();
					}
				}
			}
		}
	}

	outFile.close();
}

void ProblemSolution::imprimeMapSolProfRealDiaHorarios()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solProfRealDiaHorariosAlocados.txt");
	outFile.open(fileName, ofstream::out);

	if (!outFile)
	{
		cout << "\nErro em void ProblemSolution::imprimeMapSolProfRealDiaHorarios():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for (auto itMapProfReal = mapSolProfRealDiaHorarios_.begin(); itMapProfReal != mapSolProfRealDiaHorarios_.end(); itMapProfReal++)
	{
		Professor* professor = itMapProfReal->first;

		outFile << "\n\n-----------------------------------------------------------------------";
		outFile << "\nProfessor " << professor->getId() << "\t" << professor->getNome();
		for (auto itMapDia = itMapProfReal->second.begin(); itMapDia != itMapProfReal->second.end(); itMapDia++)
		{
			int dia = itMapDia->first;
			vector<HorarioDia*> *horsDia = &itMapDia->second;
			int size = (int)horsDia->size();

			outFile << "\n\tDia " << dia << ": ";
			for (int idx = 0; idx != size; idx++)
			{
				HorarioDia* hd = (*horsDia)[idx];
				DateTime dti = hd->getHorarioAula()->getInicio();
				DateTime dtf = hd->getHorarioAula()->getFinal();

				outFile << "  " << dti.getHour() << ":" << dti.getMinute()
					<< " - " << dtf.getHour() << ":" << dtf.getMinute() << " || ";
			}
		}
	}

	outFile.close();
}

void ProblemSolution::imprimeQuantChProfs()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solQuantChProfs.txt");
	outFile.open(fileName, ofstream::out);

	if (!outFile)
	{
		cout << "\nErro em void ProblemSolution::imprimeQuantChProfs():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	outFile << "\n-----------------------------------------------------------------\n";
	outFile << "Prof \t Qtd total de creditos alocados na semana\n";
	for (auto itProf = quantChProfs_.begin(); itProf != quantChProfs_.end(); itProf++)
	{
		outFile << "\n" << itProf->first << "\t\t" << itProf->second.first
			<< " minutos \t" << itProf->second.second << " creditos";
	}

	outFile.close();
}

void ProblemSolution::imprimeNrDiscSimultVirtual()
{
	if (!CentroDados::getPrintLogs())
		return;

	ofstream outFile;
	string fileName("nrDiscSimultVirtual.txt");
	outFile.open(fileName, ofstream::out);
	if (!outFile)
	{
		cout << "\nErro em void ProblemSolution::imprimeNrDiscSimultVirtual():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for (auto itNrSimult = mapNrDiscSimult.cbegin(); itNrSimult != mapNrDiscSimult.cend(); itNrSimult++)
	{
		int qtd = itNrSimult->first;
		outFile << "\nQtd de aulas simultaneas: " << qtd << "\n\tDisciplinas: ";
		for (auto itDisc = itNrSimult->second.cbegin(); itDisc != itNrSimult->second.cend(); itDisc++)
		{
			outFile << " " << (*itDisc)->getId();
		}
	}
	outFile.close();
}

// ler output de um arquivo
ProblemSolution* ProblemSolution::lerSolucao(char* const filePath, bool modoTatico)
{
	cout << ">>>> Ler solucao de arquivo:" << endl;

	// abrir arquivo
	ifstream file;
	file.open(filePath);
	if (!file.is_open())
	{
		cout << filePath << endl;
		cout << "[WARN] Arquivo nao foi aberto!" << endl;
		return nullptr;
	}

	// extrair
	istream& fileStream = file;
	ProblemSolution* const probSolution = new ProblemSolution(modoTatico);
	fileStream >> probSolution;
	file.close();

	ProblemSolution* ps = nullptr;

	// libera memoria se nao foi encontrada solucao inicial
	if (probSolution->atendimento_campus->size() == 0)
	{
		delete probSolution;
	}
	else
	{
		ps = probSolution;
	}

	cout << "solucao carregada!" << endl;

	return ps;
}

ostream & operator << (
	ostream & out, ProblemSolution & solution)
{
	// TATICO
	if (solution.modoOtmTatico)
	{
		out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>"
			<< endl;
		// out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
		//   << endl;

		out << "<TriedaOutput>" << endl;

		// out << "<cenarioId>" << solution.getCenarioId() << "</cenarioId>";

		//-----------------------------------------------------------------------
		out << "<atendimentos>" << endl;
		auto it_campus = solution.atendimento_campus->begin();
		for (; it_campus != solution.atendimento_campus->end(); it_campus++)
			out << (**it_campus);

		out << "</atendimentos>" << endl;
		//-----------------------------------------------------------------------

		if (solution.nao_atendimentos != NULL)
		{
			out << "<NaoAtendimentosTatico>" << endl;
			auto it_aluno_demanda = solution.nao_atendimentos->begin();
			for (; it_aluno_demanda != solution.nao_atendimentos->end(); it_aluno_demanda++)
				out << (**it_aluno_demanda);
			out << "</NaoAtendimentosTatico>" << endl;
		}
		else out << "<NaoAtendimentosTatico/>" << endl;



		//-----------------------------------------------------------------------
		// Erros e warnings:
		out << (*ErrorHandler::getInstance());
		//-----------------------------------------------------------------------

		//-----------------------------------------------------------------------
		// Folgas:
		out << "<restricoesVioladas>\n";
		auto it = solution.getFolgas()->begin();
		for (; it != solution.getFolgas()->end(); ++it)
			out << (**it);

		out << "</restricoesVioladas>" << endl;
		//-----------------------------------------------------------------------

		out << "<professoresVirtuais/>" << endl;

		//-----------------------------------------------------------------------
		/*
		if ( solution.alunosDemanda != NULL )
		{
		out << "<alunosDemanda>" << endl;

		GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > >::GGroupIterator it_aluno_demanda
		= solution.alunosDemanda->begin();

		for (; it_aluno_demanda != solution.alunosDemanda->end();
		it_aluno_demanda++ )
		{
		out << ( **it_aluno_demanda );
		}

		out << "</alunosDemanda>" << endl;
		}
		else
		{
		out << "<alunosDemanda/>" << endl;
		}
		*/
		//-----------------------------------------------------------------------

		out << "</TriedaOutput>" << endl;
	}
	// OPERACIONAL
	else
	{
		out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>"
			<< endl;

		//out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
		//    << endl;

		out << "<TriedaOutput>" << endl;

		//   out << "<cenarioId>" << solution.getCenarioId() << "</cenarioId>";

		//-----------------------------------------------------------------------
		out << "<atendimentos>" << endl;
		if (solution.atendimento_campus != NULL)
		{
			GGroup< AtendimentoCampus * >::GGroupIterator it_campus
				= solution.atendimento_campus->begin();

			for (; it_campus != solution.atendimento_campus->end();
				it_campus++)
			{
				out << (**it_campus);
			}

			out << "</atendimentos>" << endl;
		}
		//-----------------------------------------------------------------------

		if (solution.nao_atendimentos != NULL)
		{
			out << "<NaoAtendimentosTatico>" << endl;

			auto it_aluno_demanda = solution.nao_atendimentos->begin();
			for (; it_aluno_demanda != solution.nao_atendimentos->end(); it_aluno_demanda++)
			{
				out << (**it_aluno_demanda);
			}

			out << "</NaoAtendimentosTatico>" << endl;
		}
		else out << "<NaoAtendimentosTatico/>" << endl;


		//-----------------------------------------------------------------------
		// Erros e warnings:
		out << (*ErrorHandler::getInstance());
		//-----------------------------------------------------------------------

		//-----------------------------------------------------------------------
		// Folgas:
		out << "<restricoesVioladas>\n";
		RestricaoVioladaGroup::iterator it
			= solution.getFolgas()->begin();

		for (; it != solution.getFolgas()->end(); ++it)
		{
			out << (**it);
		}

		out << "</restricoesVioladas>" << endl;
		//-----------------------------------------------------------------------

		//-----------------------------------------------------------------------
		// C�digo relacionado � issue TRIEDA-833 e TRIEDA-883
		out << "<professoresVirtuais>" << endl;
		if (solution.professores_virtuais != NULL)
		{
			GGroup< ProfessorVirtualOutput * >::GGroupIterator it_professor_virtual
				= solution.professores_virtuais->begin();

			for (; it_professor_virtual != solution.professores_virtuais->end();
				it_professor_virtual++)
			{
				out << (**it_professor_virtual);
			}

			out << "</professoresVirtuais>" << endl;
		}
		//-----------------------------------------------------------------------

		//-----------------------------------------------------------------------
		if (solution.alunosDemanda != NULL)
		{
			out << "<alunosDemanda>" << endl;

			auto it_aluno_demanda = solution.alunosDemanda->begin();
			for (; it_aluno_demanda != solution.alunosDemanda->end(); it_aluno_demanda++)
				out << (**it_aluno_demanda);

			out << "</alunosDemanda>" << endl;
		}
		else
			out << "<alunosDemanda/>" << endl;
		//-----------------------------------------------------------------------

		out << "</TriedaOutput>" << endl;
	}

	return out;
}

istream& operator >> (istream &file, ProblemSolution* const &ptrProbSolution)
{
	string line;
	while (!getline(file, line).eof())
	{
		// ATENDIMENTOS CAMPUS
		// ---------------------------------------------------------- 
		if (line.find("<AtendimentoCampus>") != string::npos)
		{
			AtendimentoCampus* const atendCampus = new AtendimentoCampus();
			file >> atendCampus;
			if (atendCampus->getId() == InputMethods::fakeId)
				throw "[EXC: ProblemSolution::operator>>] AtendimentoCampus* e nullptr!";
			ptrProbSolution->atendimento_campus->add(atendCampus);
		}

		// PROFESSORES VIRTUAIS
		// ---------------------------------------------------------- 

		// N�O IMPLEMENTADO ENQUANTO N�O CONSEGUIR LIGAR PROF A HABILITA��ES (por exemplo por curso ID)
	}
	return file;
}