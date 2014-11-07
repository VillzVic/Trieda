#include "DadosHeuristica.h"
#include "../ProblemData.h"
#include "SalaHeur.h"
#include "AlunoHeur.h"
#include "ProfessorHeur.h"
#include "HeuristicaNuno.h"
#include "ParametrosHeuristica.h"
#include "ClusterUnidades.h"
#include "../ConjUnidades.h"
#include "OfertaDisciplina.h"
#include "../Unidade.h"
#include "../HorarioDia.h"
#include "../CentroDados.h"
#include "../AlunoDemanda.h"
#include "../Curriculo.h"

bool DadosHeuristica::inicializado_ = false;
unordered_set<ConjUnidades*> DadosHeuristica::clustersUnidades;
set<int> DadosHeuristica::prioridadesAlunos;
unordered_map<int, Disciplina *> DadosHeuristica::componentesPraticas_;
unordered_map<int, unordered_map<Campus *, unordered_map<Disciplina *, pair<int, AlunoDemanda*>>>> DadosHeuristica::prioridadeDemandasAlunos;
unordered_map<int, int> DadosHeuristica::nrDemandasPrioridade_;
unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<int, unordered_set<AlunoDemanda *>>>> DadosHeuristica::demandasAgregadas_;
unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<Curso*, unordered_map<int, unordered_set<AlunoDemanda *>>>>> DadosHeuristica::demandasAgregadasPorCurso_;
int DadosHeuristica::maxDeslocMin_ = 0;
unordered_map<int, vector<vector<AlunoDemanda*>>> DadosHeuristica::coRequisitosPorAluno;
unordered_map<int, unordered_set<int>> DadosHeuristica::demandasAlunoCoReq;

unordered_map<Campus*, unordered_map<Disciplina*, vector<set<Curso*>>>> DadosHeuristica::cursosCompatCampusDisc_;

DadosHeuristica::DadosHeuristica(void)
{
}

DadosHeuristica::~DadosHeuristica(void)
{
}

void DadosHeuristica::prepararDados(void)
{
	ProblemData* const probData = CentroDados::getProblemData();

	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg(">> Preparar dados heurística:", 1);
	// guardar demandas P2
	HeuristicaNuno::logMsg("Guardar demandas P2...", 1);
	guardarDemandasP2_();
	// Mapear componentes praticas
	HeuristicaNuno::logMsg("Mapear componentes praticas...", 1);
	mapearComponentesPraticas_();
	// criar clusters de unidades semelhantes
	HeuristicaNuno::logMsg("Criar clusters de unidades...", 1);
	criarClustersUnidades_();
	// reduzir calendarios das disciplinas, removendo os dominados
	HeuristicaNuno::logMsg("Set calendarios dominantes...", 1);
	setCalsReduzidos_();
	// pre-processar co-requisitos dos alunos [Antes de agregar demandas sempre!]
	if(probData->parametros->considerarCoRequisitos)
	{
		HeuristicaNuno::logMsg("Pre-processar co-requisitos...", 1);
		preProcCoRequisitos_();
	}
	// agregar demandas por prioridade
	HeuristicaNuno::logMsg("Agregar demandas...", 1);
	agregarDemandas_();
	// preencher prioridades alunos
	HeuristicaNuno::logMsg("Preencher prioridades alunos...", 1);
	fillPrioridadesAlunos_();

	// TESTAR!! 
	// gerar cliques cursos compativeis por <campus, disciplina>
	//HeuristicaNuno::logMsg("Gerar cliques de cursos compativeis por campus-disciplina...", 1);
	//setCursosCompatCampusDisc();

	// inicializar aulas possiveis por calendario
	HeuristicaNuno::logMsg("Init aulas calendario...", 1);
	UtilHeur::initAulasCalendario();

	// reordenar horariosdia salas
	//reordHorariosDiaSalas_();

	// check
	HeuristicaNuno::logMsg("Check demandas e equivalencias...", 1);
	checkDemandasEquiv_();

	inicializado_ = true;
}

int DadosHeuristica::getPrioridade(Campus* const campus, Disciplina* const disciplina, int alunoId)
{
	auto itAluno = prioridadeDemandasAlunos.find(alunoId);
	if(itAluno == prioridadeDemandasAlunos.end())
	{
		//HeuristicaNuno::excepcao("DadosHeuristica::prioridade", "Aluno não encontrado.");
		return 0;
	}

	auto itCampus = itAluno->second.find(campus);
	if(itCampus == itAluno->second.end())
	{
		//HeuristicaNuno::excepcao("DadosHeuristica::prioridade", "Campus não encontrado.");
		return 0;
	}

	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
	{
		//HeuristicaNuno::excepcao("DadosHeuristica::prioridade", "Disciplina não encontrada.");
		return 0;
	}
	
	return itDisc->second.first;
}

bool DadosHeuristica::getPrioridadeDemanda(Campus* const campus, Disciplina* const disciplina, int alunoId, 
										pair<int, AlunoDemanda*> &par)
{
	auto itAluno = prioridadeDemandasAlunos.find(alunoId);
	if(itAluno == prioridadeDemandasAlunos.end())
	{
		//HeuristicaNuno::excepcao("DadosHeuristica::getPrioridadeDemanda", "Aluno não encontrado.");
		return false;
	}

	auto itCampus = itAluno->second.find(campus);
	if(itCampus == itAluno->second.end())
	{
		//HeuristicaNuno::excepcao("DadosHeuristica::getPrioridadeDemanda", "Campus não encontrado.");
		return false;
	}

	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
	{
		//HeuristicaNuno::excepcao("DadosHeuristica::getPrioridadeDemanda", "Disciplina não encontrada.");
		return false;
	}

	par = itDisc->second;
	return true;
}

bool DadosHeuristica::temDemanda(Campus* const campus, Disciplina* const disciplina, int prioridade, int alunoId)
{
	HeuristicaNuno::logMsg("DadosHeuristica::temDemanda", 2);
	HeuristicaNuno::logMsg("campus", 2);
	auto itCampus = demandasAgregadas_.find(campus);
	if(itCampus == demandasAgregadas_.end())
		return false;

	HeuristicaNuno::logMsg("disciplina", 2);
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
		return false;

	HeuristicaNuno::logMsg("prioridade", 2);
	auto itPrior = itDisc->second.find(prioridade);
	if(itPrior == itDisc->second.end())
		return false;

	HeuristicaNuno::logMsg("search demandas", 2);
	auto itDem = itPrior->second.begin();
	for(; itDem != itPrior->second.end(); ++itDem)
	{
		if(*itDem == nullptr)
			continue;

		if((*itDem)->getAlunoId() == alunoId)
		{
			HeuristicaNuno::logMsg("true", 2);
			return true;
		}
	}

	HeuristicaNuno::logMsg("out", 2);

	return false;
}

// get mapEquivOrig para aluno
void DadosHeuristica::getMapEquivAluno(Aluno* const aluno, unordered_map<int, unordered_set<int>> &mapEquivOrig)
{
	unordered_set<AlunoDemanda*> demandas;
	// P1
	for(auto it = aluno->demandas.begin(); it != aluno->demandas.end(); ++it)
		demandas.insert(*it);
	// P2
	for(auto it = aluno->demandasP2.begin(); it != aluno->demandasP2.end(); ++it)
		demandas.insert(*it);

	for(auto it = demandas.begin(); it != demandas.end(); ++it)
	{
		Disciplina* const disciplina = (*it)->demanda->disciplina;
		int origId = disciplina->getId();
		// Se for uma componente prática ignorar
		if(origId < 0)
			continue;

		// verificar equivalencias
		auto itDiscEquiv = (*it)->demanda->discIdSubstitutasPossiveis.begin();
		for(; itDiscEquiv != (*it)->demanda->discIdSubstitutasPossiveis.end(); itDiscEquiv++)
		{
			int equivId = itDiscEquiv->first;
			if(equivId < 0)
				continue;

			// tbm é original
			if(getPrioridade((*it)->getCampus(), disciplina, aluno->getAlunoId()) >= 0)
				continue;

			auto itEquiv = mapEquivOrig.find(equivId);
			if(itEquiv == mapEquivOrig.end())
			{
				unordered_set<int> empty;
				auto par = mapEquivOrig.insert(make_pair<int, unordered_set<int>>(equivId, empty));
				if(!par.second)
					HeuristicaNuno::excepcao("DadosHeuristica::checkDemEquivAluno_", "equiv nao adicionada ao map");

				itEquiv = par.first;
			}

			if(itEquiv->second.find(origId) == itEquiv->second.end())
				itEquiv->second.insert(origId);
		}
	}
}

bool DadosHeuristica::getComponentePratica(int id, Disciplina* &pratica) 
{
	// componente prática tem id = -id da disciplina original
	int idPratica = -id;

	auto itComp = componentesPraticas_.find(idPratica);
	if(itComp == componentesPraticas_.end())
		return false;

	pratica = itComp->second;
	return true;
}

// verifica se esta disciplina pode ser considerada equivalente de outra
bool DadosHeuristica::checkEquivalencia(AlunoDemanda* const demandaOrig, Campus* const campus, Disciplina* const discEquiv, int alunoId)
{
	if(demandaOrig == nullptr)
	{
		HeuristicaNuno::warning("DadosHeuristica::checkEquivalencia", "Demanda original nula!");
		return false;
	}

	int prioridade = demandaOrig->getPrioridade();
	pair<int, AlunoDemanda*> parDemEquiv;
	bool got = getPrioridadeDemanda(campus, discEquiv, alunoId, parDemEquiv);
	if(!got || (parDemEquiv.first != -prioridade) || (parDemEquiv.second->getId() != demandaOrig->getId()))
		return false;

	// OBS: Se tem co-requisito não pode ser usada equivalência!
	if(HeuristicaNuno::probData->parametros->considerarCoRequisitos)
	{
		auto itCoReqsAluno = demandasAlunoCoReq.find(alunoId);
		if(itCoReqsAluno != demandasAlunoCoReq.end() && 
			itCoReqsAluno->second.find(demandaOrig->getId()) != itCoReqsAluno->second.end())
		{
			return false;
		}
	}

	return true;
}

// guardar demandas P2
void DadosHeuristica::guardarDemandasP2_(void)
{
	for(auto itDem = HeuristicaNuno::probData->alunosDemandaTotal.begin(); itDem != HeuristicaNuno::probData->alunosDemandaTotal.end(); itDem++)
	{
		AlunoDemanda* const alunoDemanda = (*itDem);
		if(std::abs(alunoDemanda->getPrioridade()) == 1)
			continue;

		Aluno* const aluno = alunoDemanda->getAluno();
		if(aluno == nullptr || aluno == NULL)
			HeuristicaNuno::excepcao("DadosHeuristica::guardarDemandasP2_", "Aluno nao encontrado!");

		aluno->demandasP2.insert(alunoDemanda);
	}
}

// agregar as demandas [Classic]
void DadosHeuristica::agregarDemandas_(void)
{
	demandasAgregadas_.clear();
	prioridadeDemandasAlunos.clear();
	nrDemandasPrioridade_.clear();

	ProblemData* const probData = CentroDados::getProblemData();

	// stats
	int nrEquivForc = 0;
	int nrAddEquivForc = 0;

	for(auto itDem = HeuristicaNuno::probData->alunosDemandaTotal.begin(); itDem != HeuristicaNuno::probData->alunosDemandaTotal.end(); itDem++)
	{
		AlunoDemanda* const alunoDemanda = (*itDem);
		Aluno* const aluno = alunoDemanda->getAluno();
		Curso* const curso = aluno->getCurso();
		Campus* const campus = alunoDemanda->demanda->oferta->campus;
		Disciplina* const disciplina = alunoDemanda->demanda->disciplina;
		const int p = alunoDemanda->getPrioridade();
		if(p <= 0)
		{
			HeuristicaNuno::logMsgInt("aluno dem id: ", alunoDemanda->getId(), 1);
			HeuristicaNuno::warning("DadosHeuristica::agregarDemandas_", "Prioridade da demanda menor ou igual a zero!");
			continue;
		}

		// verificar se deve ser usada demanda p2
		if(!probData->parametros->utilizarDemandasP2 && (p > 1) )
			continue;

		// Se for uma componente prática ignorar
		if(disciplina->getId() < 0)
			continue;

		int maxCreds = disciplina->getTotalCreditos();
		auto itPrat = HeuristicaNuno::probData->refDisciplinas.find(-disciplina->getId());
		if(itPrat != HeuristicaNuno::probData->refDisciplinas.end())
			maxCreds += itPrat->second->getTotalCreditos();

		bool equivForc = alunoDemanda->getExigeEquivalenciaForcada();
		// registar prioridade e demanda original
		if(!equivForc)
		{
			if(adicionarPrioridade_(alunoDemanda, campus, disciplina, curso, p))
			{
				adicionarDemanda_(alunoDemanda, campus, disciplina, p);
				adicionarDemandaPorCurso_(alunoDemanda, campus, disciplina, curso, p);
			}
			else if(p == 1)
			{
				stringstream ss;
				ss << "Demanda P1 da disciplina " << disciplina->getId() << "nao adicionada a prioridades!";
				HeuristicaNuno::warning("DadosHeuristica::agregarDemandas_", ss.str());
			}
		}
		else
		{
			nrEquivForc++;
		}

		// se tem co-requisito não registar demanda por equivalência!
		if(!DadosHeuristica::temCoReq(aluno->getAlunoId(), alunoDemanda->getId()))
		{
			// adicionar equivalências com prioridade -p
			for(auto itDiscEquiv = alunoDemanda->demanda->discIdSubstitutasPossiveis.begin(); 
					itDiscEquiv != alunoDemanda->demanda->discIdSubstitutasPossiveis.end(); ++itDiscEquiv)
			{
				Disciplina* const disciplinaEquiv = itDiscEquiv->second;
				if(disciplinaEquiv->getId() < 0)
					continue;

				int credsEquiv = disciplinaEquiv->getTotalCreditos();
				auto itPratEquiv = HeuristicaNuno::probData->refDisciplinas.find(-disciplinaEquiv->getId());
				if(itPratEquiv != HeuristicaNuno::probData->refDisciplinas.end())
					credsEquiv += itPratEquiv->second->getTotalCreditos();

				if(credsEquiv > maxCreds)
					maxCreds = credsEquiv;

				// registrar prioridade e demanda equivalencia
				if(adicionarPrioridade_(alunoDemanda, campus, disciplinaEquiv, curso, -p))
				{
					adicionarDemanda_(alunoDemanda, campus, disciplinaEquiv, -p);
					adicionarDemandaPorCurso_(alunoDemanda, campus, disciplinaEquiv, curso, -p);
					// stats
					if(equivForc)
						nrAddEquivForc++;

					// contabilizar nr demandas prioridade (equiv)
					auto itNrDemPrior = nrDemandasPrioridade_.find(-p);
					if(itNrDemPrior == nrDemandasPrioridade_.end())
						itNrDemPrior = nrDemandasPrioridade_.insert(make_pair<int, int>(-p, 0)).first;
					int nr = itNrDemPrior->second;
					itNrDemPrior->second = (nr+1);
				}
			}
		}

		if(p == 1)
		{
			aluno->addDemP1();
			aluno->addNrCredsReqP1(maxCreds);
		}

		// contabilizar nr demandas prioridade
		auto itNrDemPrior = nrDemandasPrioridade_.find(p);
		if(itNrDemPrior == nrDemandasPrioridade_.end())
			itNrDemPrior = nrDemandasPrioridade_.insert(make_pair<int, int>(p, 0)).first;
		int nr = itNrDemPrior->second;
		itNrDemPrior->second = (nr+1);
	}

	// stats demandas
	HeuristicaNuno::logMsgInt("nr demandas P1: ", nrDemandas(1), 1);
	HeuristicaNuno::logMsgInt("nr demandas P2: ", nrDemandas(2), 1);
	HeuristicaNuno::logMsgInt("nr demandas equiv forc: ", nrEquivForc, 1);
	HeuristicaNuno::logMsgInt("nr demandas add equiv forc: ", nrAddEquivForc, 1);
}
bool DadosHeuristica::adicionarPrioridade_(AlunoDemanda* const alunoDemanda, Campus* const campus, Disciplina* const disciplina, Curso* const &curso, 
										const int p)
{
	// CHECK
	bool print = false;
	/*bool print = (alunoDemanda->demanda->getDisciplinaId() == 20972) && (std::abs(p) == 2);
	if(print)
	{
		stringstream ss;
		ss << "[CHECK] Disciplina Original 20972! Disc " << disciplina->getId() << ", prior = " << p;
		HeuristicaNuno::logMsg(ss.str(), 1);
	}*/

	// campus
	int alunoId = alunoDemanda->getAlunoId();
	auto itAluno = prioridadeDemandasAlunos.find(alunoId);
	if(itAluno == prioridadeDemandasAlunos.end())
	{
		unordered_map<Campus*, unordered_map<Disciplina *, pair<int, AlunoDemanda*> > > mapAluno;
		std::pair<int, unordered_map<Campus *, unordered_map<Disciplina*, pair<int, AlunoDemanda*>>>> parAluno (alunoId, mapAluno);
		auto par = prioridadeDemandasAlunos.insert(parAluno);
		if(!par.second)
			HeuristicaNuno::excepcao("DadosHeuristica::adicionarPrioridade_", "Campus nao adicionado");
		itAluno = par.first;
	}

	// disciplina
	auto itCampus = itAluno->second.find(campus);
	if(itCampus == itAluno->second.end())
	{
		unordered_map<Disciplina *, pair<int, AlunoDemanda*>> mapDisc;
		std::pair<Campus*, unordered_map<Disciplina *, pair<int, AlunoDemanda*> > > parDisc (campus, mapDisc);
		auto par = itAluno->second.insert(parDisc);
		if(!par.second)
			HeuristicaNuno::excepcao("DadosHeuristica::adicionarPrioridade_", "Disciplina nao adicionada");
		itCampus = par.first;
	}

	// prioridade
	pair<int, AlunoDemanda*> parPrioDem (p, alunoDemanda);
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc != itCampus->second.end())
	{
		int currPrior = itDisc->second.first;
		// prioridade é menor, ou igual mas original
		int diff = std::abs(p) - std::abs(currPrior);
		if((diff < 0) || (diff == 0 && p > currPrior))
		{
			// remover a demanda da prioridade menor
			removeDemanda_(alunoDemanda->getAlunoId(), campus, disciplina, currPrior);
			removeDemandaPorCurso_(alunoDemanda->getAlunoId(), campus, disciplina, curso, currPrior);
			itDisc->second = parPrioDem;
			
			if(print)
			{
				stringstream ss;
				ss << "true. p = " << p << " ; curr = " << currPrior;
				HeuristicaNuno::logMsg(ss.str(), 1);
			}
			return true;
		}
		else
		{
			if(print)
			{
				stringstream ss;
				ss << "false. p = " << p << " ; curr = " << currPrior;
				HeuristicaNuno::logMsg(ss.str(), 1);
			}
			return false;
		}
	}

	if(parPrioDem.second <= 0)
	{
		stringstream ss;
		ss << "parPrior <= 0. Disc id: " << disciplina->getId() << " ; prior: " << parPrioDem.second;
		HeuristicaNuno::warning("DadosHeuristica::adicionarPrioridade_", ss.str());
	}
	
	// inserir <prior, demanda>
	if(!itCampus->second.insert(make_pair<Disciplina*, pair<int, AlunoDemanda*>>(disciplina, parPrioDem)).second)
		HeuristicaNuno::excepcao("DadosHeuristica::adicionarPrioridade_", "Prioridade nao adicionada");

	return true;
}
void DadosHeuristica::adicionarDemanda_(AlunoDemanda* const alunoDemanda, Campus* const campus, Disciplina* const disciplina, const int p)
{
	// campus
	auto itCampus = demandasAgregadas_.find(campus);
	if(itCampus == demandasAgregadas_.end())
	{
		unordered_map<Disciplina*, unordered_map<int, unordered_set<AlunoDemanda*>>> mapDisc;
		itCampus = demandasAgregadas_.insert(make_pair<Campus *, unordered_map<Disciplina*, unordered_map<int, unordered_set<AlunoDemanda*>>>>(campus, mapDisc)).first;
	}

	// disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
	{
		unordered_map<int, unordered_set<AlunoDemanda*>> mapPrior;
		itDisc = itCampus->second.insert(make_pair<Disciplina*, unordered_map<int, unordered_set<AlunoDemanda*>>>(disciplina, mapPrior)).first;
	}

	// prioridade
	auto itPrior = itDisc->second.find(p);
	if(itPrior == itDisc->second.end())
	{
		unordered_set<AlunoDemanda*> mapDem;
		itPrior = itDisc->second.insert(make_pair<int, unordered_set<AlunoDemanda*>>(p, mapDem)).first;
	}

	if(!itPrior->second.insert(alunoDemanda).second)
		HeuristicaNuno::warning("DadosHeuristica::adicionarDemanda_", "Demanda nao inserida!");
}
void DadosHeuristica::removeDemanda_(int alunoId, Campus* const campus, Disciplina* const disciplina, const int p)
{
	// campus
	auto itCamp = demandasAgregadas_.find(campus);
	if(itCamp == demandasAgregadas_.end())
	{
		HeuristicaNuno::warning("DadosHeuristica::removeDemanda_", "Demandas do campus não encontradas");
		return;
	}

	// disciplina
	auto itDisc = itCamp->second.find(disciplina);
	if(itDisc == itCamp->second.end())
	{
		HeuristicaNuno::warning("DadosHeuristica::removeDemanda_", "Disciplina não encontrada");
		return;
	}

	// prioridade
	auto itPrior = itDisc->second.find(p);
	if(itPrior == itDisc->second.end())
	{
		HeuristicaNuno::warning("DadosHeuristica::removeDemanda_", "Prioridade não encontrada");
		return;
	}	

	// remover
	auto it = itPrior->second.begin();
	for(; it != itPrior->second.end(); ++it)
	{
		if((*it)->getAlunoId() == alunoId)
			break;
	}
	if(it != itPrior->second.end())
		itPrior->second.erase(it);
	else
		HeuristicaNuno::warning("DadosHeuristica::removeDemanda_", "Demanda não encontrada");
}

// agregar as demandas não atendidas [POR CURSO!]
void DadosHeuristica::adicionarDemandaPorCurso_(AlunoDemanda* const &alunoDemanda, Campus* const &campus, Disciplina* const &disciplina, 
											Curso* const &curso, const int p)
{
	// campus
	auto itCampus = demandasAgregadasPorCurso_.find(campus);
	if(itCampus == demandasAgregadasPorCurso_.end())
	{
		unordered_map<Disciplina*, unordered_map<Curso*, unordered_map<int, unordered_set<AlunoDemanda*>>>> mapDisc;
		itCampus = demandasAgregadasPorCurso_.insert(make_pair(campus, mapDisc)).first;
	}

	// disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
	{
		unordered_map<Curso*, unordered_map<int, unordered_set<AlunoDemanda*>>> mapCurso;
		itDisc = itCampus->second.insert(make_pair(disciplina, mapCurso)).first;
	}

	// curso
	auto itCurso = itDisc->second.find(curso);
	if(itCurso == itDisc->second.end())
	{
		unordered_map<int, unordered_set<AlunoDemanda*>> mapPrior;
		itCurso = itDisc->second.insert(make_pair(curso, mapPrior)).first;
	}

	// prioridade
	auto itPrior = itCurso->second.find(p);
	if(itPrior == itCurso->second.end())
	{
		unordered_set<AlunoDemanda*> demandas;
		itPrior = itCurso->second.insert(make_pair(p, demandas)).first;
	}

	if(!itPrior->second.insert(alunoDemanda).second)
		HeuristicaNuno::warning("DadosHeuristica::adicionarDemandaPorCurso_", "Demanda nao inserida!");
}
void DadosHeuristica::removeDemandaPorCurso_(const int alunoId, Campus* const &campus, Disciplina* const &disciplina, Curso* const &curso, 
											const int p)
{
	// campus
	auto itCamp = demandasAgregadasPorCurso_.find(campus);
	if(itCamp == demandasAgregadasPorCurso_.end())
	{
		HeuristicaNuno::warning("DadosHeuristica::removeDemandaPorCurso_", "Demandas do campus não encontradas");
		return;
	}

	// disciplina
	auto itDisc = itCamp->second.find(disciplina);
	if(itDisc == itCamp->second.end())
	{
		HeuristicaNuno::warning("DadosHeuristica::removeDemandaPorCurso_", "Disciplina não encontrada");
		return;
	}

	// curso
	auto itCurso = itDisc->second.find(curso);
	if(itCurso == itDisc->second.end())
	{
		HeuristicaNuno::warning("DadosHeuristica::removeDemandaPorCurso_", "Curso não encontrado");
		return;
	}

	// prioridade
	auto itPrior = itCurso->second.find(p);
	if(itPrior == itCurso->second.end())
	{
		HeuristicaNuno::warning("DadosHeuristica::removeDemandaPorCurso_", "Prioridade não encontrada");
		return;
	}	

	// remover
	auto it = itPrior->second.begin();
	for(; it != itPrior->second.end(); ++it)
	{
		if((*it)->getAlunoId() == alunoId)
			break;
	}
	if(it != itPrior->second.end())
		itPrior->second.erase(it);
	else
		HeuristicaNuno::warning("DadosHeuristica::removeDemanda_", "Demanda não encontrada");
}

// juntar grupos de cursos compatíveis relevantes por <Campus, Disciplina> com base nos cursos que têm demanda
void DadosHeuristica::setCursosCompatCampusDisc(void)
{
	cursosCompatCampusDisc_.clear();

	vector<Curso*> cursos;
	unordered_map<Disciplina*, vector<set<Curso*>>> emptyMapDisc;
	vector<set<Curso*>> conjCursos;

	// campus
	for(auto itCampus = demandasAgregadasPorCurso_.cbegin(); itCampus != demandasAgregadasPorCurso_.cend(); ++itCampus)
	{
		Campus* const campus = itCampus->first;
		auto itCompCamp = cursosCompatCampusDisc_.insert(make_pair(campus, emptyMapDisc)).first;

		// disciplina
		for(auto itDisc = itCampus->second.cbegin(); itDisc != itCampus->second.cend(); ++itDisc)
		{
			Disciplina* const disciplina = itDisc->first;
			cursos.clear();
			conjCursos.clear();
			for(auto it = itDisc->second.cbegin(); it != itDisc->second.cend(); ++it)
				cursos.push_back(it->first);

			if(cursos.size() > 0)
			{
				getMaxCliquesCursos(cursos, conjCursos);
				itCompCamp->second.insert(make_pair(disciplina, conjCursos));
			}
		}
	}
}

// ----------------- [Calculador de Cliques para Compatibilidade de cursos (algoritmo de Bron–Kerbosch)] ----------------

// gerar cliques máximos de cursos
void DadosHeuristica::getMaxCliquesCursos(vector<Curso*> const &cursos, vector<set<Curso*>> &cliquesCursos)
{
	HeuristicaNuno::logMsg("criar nodes", 1);
	set<Node*, compNodes> nodesOrd;
	criarNodesCursos(cursos, nodesOrd);
	HeuristicaNuno::logMsg("out. criar nodes", 1);

	// cliques curso id
	vector<unordered_set<int>*> cliques;

	// preparar recursão
	unordered_set<Node*, Node::hashNode> R;
	unordered_set<Node*, Node::hashNode> P;
	unordered_set<Node*, Node::hashNode> X;
	for(auto it = nodesOrd.begin(); it != nodesOrd.end(); ++it)
		if(!P.insert(*it).second)
			HeuristicaNuno::excepcao("DadosHeuristica::getMaxCliquesCursos", "Node nao adicionado a P");

	// algoritmo de Bron–Kerbosch III
	unordered_set<Node*, Node::hashNode> newR;
	unordered_set<Node*, Node::hashNode> newP;
	unordered_set<Node*, Node::hashNode> newX;
	for(auto it = nodesOrd.begin(); it != nodesOrd.end(); ++it)
	{
		//HeuristicaNuno::logMsg("it node", 1);
		newR.clear();
		newR.insert(*it);

		// novo P
		UtilHeur::intersectSets<Node*, Node::hashNode>(P, (*it)->vizinhos, newP);
		// novo X
		UtilHeur::intersectSets<Node*, Node::hashNode>(X, (*it)->vizinhos, newX);

		// proximo passo da recursao
		Node::algBronKerboschRec(newR, newP, newX, cliques);
		// TESTAR COM PIVOTING
		//algBronKerboschRecPivoting(newR, newP, newX, cliques);

		auto itP = P.find(*it);
		if(itP != P.end())
			P.erase(itP);
		else
			HeuristicaNuno::excepcao("DadosHeuristica::getMaxCliquesCursos", "node nao deletado de P");
		X.insert(*it);
	}

	// libertar memoria dos nodes
	for(auto it = nodesOrd.begin(); it != nodesOrd.end(); )
	{
		Node* node = *it;
		it = nodesOrd.erase(it);
		delete node;
	}

	// substituir por sets de cursos
	for(auto it = cliques.begin(); it != cliques.end();)
	{
		set<Curso*> setCursos;
		unordered_set<int>* ids = *it;
		for(auto itId = ids->cbegin(); itId != ids->cend(); ++itId)
		{
			auto itData = HeuristicaNuno::probData->refCursos.find(*itId);
			if(itData == HeuristicaNuno::probData->refCursos.end())
			{
				stringstream ss;
				ss << "Curso com id " << *itId << " nao encontrado!";
				HeuristicaNuno::excepcao("DadosHeuristica::getMaxCliquesCursos", ss.str());
			}

			Curso* const curso = itData->second;
			setCursos.insert(curso);
		}

		cliquesCursos.push_back(setCursos);
		cliques.erase(it++);
		delete ids;
	}
}

void DadosHeuristica::criarNodesCursos(vector<Curso*> const &cursos, set<Node*, compNodes> &nodes)
{
	unordered_map<int, Node*> cursoIdNode;

	// criar nodes e registar vizinhos
	for(auto it = cursos.begin(); it != cursos.end(); ++it)
	{
		Curso* const curso = *it;
		if(curso == nullptr)
			HeuristicaNuno::excepcao("DadosHeuristica::criarNodesCursos", "curso e nulo");

		Node* node = nullptr;
		auto itNode = cursoIdNode.find(curso->getId());
		if(itNode != cursoIdNode.end())
		{
			node = itNode->second;
		}
		else
		{
			node = new Node(curso->getId());
			cursoIdNode.insert(make_pair(curso->getId(), node));
		}

		// verificar compatibilidade. começar depois para evitar repeticao
		for(auto itComp = std::next(it); itComp != cursos.end(); ++itComp)
		{
			Curso* const outro = *itComp;
			if(outro == nullptr)
				HeuristicaNuno::excepcao("DadosHeuristica::criarNodesCursos", "outro curso e nulo");

			if(curso->getId() == outro->getId())
				continue;

			// incompativeis
			if(curso->ehIncompativel(outro))
				continue;

			Node* othNode = nullptr;
			auto itNodeComp = cursoIdNode.find(outro->getId());
			if(itNodeComp != cursoIdNode.end())
				othNode = itNodeComp->second;
			else
			{
				othNode = new Node(outro->getId());
				cursoIdNode.insert(make_pair(outro->getId(), othNode));
			}

			// registar compatibilidade
			node->vizinhos.insert(othNode);
			othNode->vizinhos.insert(node);
		}
	}

	for(auto itNode = cursoIdNode.begin(); itNode != cursoIdNode.end(); ++itNode)
		nodes.insert(itNode->second);
}

// ----------------------------------------------------------------------------------------------------------------------

// mapear componentes teóricas e práticas
void DadosHeuristica::mapearComponentesPraticas_(void)
{
	componentesPraticas_.clear();
	GGroup< Disciplina *, LessPtr< Disciplina > >::iterator itDisc = HeuristicaNuno::probData->novasDisciplinas.begin();
	for(; itDisc != HeuristicaNuno::probData->novasDisciplinas.end(); itDisc++)
	{
		int id = (*itDisc)->getId();
		if(id >= 0)
		{
			HeuristicaNuno::warning("DadosHeuristica::mapearComponentesPraticas_", "Componente pratica com id positivo!");
			id = (-id);
		}
		componentesPraticas_[id] = *itDisc;
	}
}

// nr demandas de uma prioridade
int DadosHeuristica::nrDemandas(int prior, bool equiv)
{
	int nrDem = 0;
	auto itDem = nrDemandasPrioridade_.find(prior);
	if(itDem != nrDemandasPrioridade_.end())
		nrDem += itDem->second;

	if(equiv)
	{
		auto itDemEq = nrDemandasPrioridade_.find(-prior);
		if(itDemEq != nrDemandasPrioridade_.end())
			nrDem += itDemEq->second;
	}

	return nrDem;
}

// preencher prioridades alunos
void DadosHeuristica::fillPrioridadesAlunos_(void)
{
	ProblemData* const probData = CentroDados::getProblemData();
	for(auto it = probData->alunos.begin(); it != probData->alunos.end(); ++it)
	{
		int prior = (*it)->getPriorAluno();
		prioridadesAlunos.insert(prior);
	}
}

// criar clusters de unidades
void DadosHeuristica::criarClustersUnidades_(void)
{
	maxDeslocMin_ = 0;
	//HeuristicaNuno::logMsg("Criar clusters de unidades...", 1);
	for(auto it = HeuristicaNuno::probData->campi.begin(); it != HeuristicaNuno::probData->campi.end(); ++it)
	{
		ClusterUnidades clusterizer (*it);
		clusterizer.criarClusters();

		HeuristicaNuno::logMsgInt("# unidades: ", (*it)->unidades.size(), 1);
		HeuristicaNuno::logMsgInt("# clusters: ", (*it)->getClustersUnids().size(), 1);

		// associar unidade a cluster
		int nrUnCl = 0;
		auto clusters = (*it)->getClustersUnids();
		for(auto itUnid = (*it)->unidades.begin(); itUnid != (*it)->unidades.end(); ++itUnid)
		{
			bool clust = false;
			int unidId = (*itUnid)->getId();
			for(auto itClust = clusters.begin(); itClust != clusters.end(); ++itClust)
			{
				if((*itClust)->temUnidade(unidId))
				{
					clust = true;
					(*itUnid)->conjunto = *itClust;
					nrUnCl += (*itClust)->unidades.size();
					break;
				}
				
			}
			if(!clust)
			{
				HeuristicaNuno::logMsgInt("nr unids nos clusters: ", nrUnCl, 1);
				HeuristicaNuno::excepcao("DadosHeuristica::criarClustersUnidades_", "Unidade em nenhum cluster");
			}
		}
		// verificar max deslocamento, inserir clusters em 'clustersUnidades'
		for(auto itClust = clusters.begin(); itClust != clusters.end(); ++itClust)
		{
			clustersUnidades.insert(*itClust);
			Unidade* const unid = HeuristicaNuno::probData->refUnidade[(*itClust)->getUnidadeId()];
			for(auto itOther = next(itClust); itOther != clusters.end(); ++itOther)
			{
				Unidade* const outraUnid = HeuristicaNuno::probData->refUnidade[(*itOther)->getUnidadeId()];
				int desloc = HeuristicaNuno::probData->calculaTempoEntreCampusUnidades(*it, *it, unid, outraUnid);
				if(desloc > maxDeslocMin_)
					maxDeslocMin_ = desloc;
			}
		}
	}
}

// set calendarios reduzidos
void DadosHeuristica::setCalsReduzidos_(void)
{
	for(auto itDisc = HeuristicaNuno::probData->disciplinas.begin(); itDisc != HeuristicaNuno::probData->disciplinas.end(); ++itDisc)
	{
		if(ParametrosHeuristica::reduzirCalendarios)
			setCalsReduzidosDisc_(*itDisc);
		else
			(*itDisc)->calendariosReduzidos = (*itDisc)->getCalendarios(); // apenas copiar
	}
}
void DadosHeuristica::setCalsReduzidosDisc_(Disciplina* const disciplina)
{
	auto calendarios = disciplina->getCalendariosOriginais();
	if(calendarios.size() == 0)
	{
		HeuristicaNuno::logMsgInt("disc id: ", disciplina->getId(), 0);
		HeuristicaNuno::warning("DadosHeuristica::setCalsReduzidosDisc_", "Disciplina nao tem calendarios associados");
		return;
	}

	// calendarios dominados
	unordered_set<Calendario*> dominados;
	
	// encontrar dominancias
	int disciplinaId = disciplina->getId();
	for(auto it = calendarios.begin(); it != calendarios.end(); ++it)
	{
		//if(disciplinaId == 13104 || disciplinaId == 13104)
		//	HeuristicaNuno::logMsg((*it)->getCodigo(), 1);

		if(dominados.find(*it) != dominados.end())
			continue;

		for(auto itDom = calendarios.begin(); itDom != calendarios.end(); ++itDom)
		{
			// já está dominado
			if(dominados.find(*itDom) != dominados.end())
				continue;

			if(UtilHeur::calendarioDomina(*it, *itDom))
				dominados.insert(*itDom);
		}
	}

	// adicionar à disciplina
	disciplina->calendariosReduzidos.clear();
	for(auto it = calendarios.begin(); it != calendarios.end(); ++it)
	{
		// só adicionar não dominados
		if(dominados.find(*it) == dominados.end())
			disciplina->calendariosReduzidos.add(*it);
	}

	// verificar se ficou vazio
	if(disciplina->calendariosReduzidos.size() == 0)
		HeuristicaNuno::excepcao("DadosHeuristica::setCalsReduzidosDisc_", "Nenhum calendario depois da reducao!");

	// verificar a diferença
	int diff = disciplina->calendarios.size() - disciplina->calendariosReduzidos.size();
	if(diff > 0)
		HeuristicaNuno::logMsgInt("# cals removed: ", diff, 2);
	else if(diff < 0)
		HeuristicaNuno::excepcao("DadosHeuristica::setCalsReduzidosDisc_", "Nr calendarios reduzido maior que inicial");
}

// calcular numero de horarios por semana para cada turno
void DadosHeuristica::calcNrHorsSemanaTurnos_(void)
{
	for(auto it = HeuristicaNuno::probData->turnosIES.begin(); it != HeuristicaNuno::probData->turnosIES.end(); ++it)
	{
		int nr = 0;
		for(int dia = 2; dia <= 8; ++dia)
		{
			nr += (*it)->getNroDeHorariosAula(dia);
		}
		(*it)->nrHorariosSemana = nr;
	}
}

// pre-processar co-requisitos
void DadosHeuristica::preProcCoRequisitos_(void)
{
	ProblemData* const probData = CentroDados::getProblemData();

	unordered_map<int, vector<AlunoDemanda*>> coReqsAluno;		// id co-requisito -> demandas
	unordered_map<int, int> mapDiscCoReq;						// disc.id -> id co-requisito
	unordered_set<AlunoDemanda*> demandasAluno;
	set<int> coReqsDisc;
	vector<AlunoDemanda*> coReqDems;
	int idCoReq = 0;
	for(auto it = probData->alunos.begin(); it != probData->alunos.end(); ++it)
	{
		Aluno* const aluno = *it;
		coReqsAluno.clear();
		mapDiscCoReq.clear();
		demandasAluno.clear();
		idCoReq = 0;

		// juntar demandas
		for(auto itD = aluno->demandas.begin(); itD != aluno->demandas.end(); ++itD)
			demandasAluno.insert(*itD);
		for(auto itD = aluno->demandasP2.begin(); itD != aluno->demandasP2.end(); ++itD)
			demandasAluno.insert(*itD);

		// aglomerar as demandas por grupo co-requisito
		for(auto itDem = demandasAluno.cbegin(); itDem != demandasAluno.cend(); ++itDem)
		{
			AlunoDemanda* const alunoDem = *itDem;
			Curriculo* const curriculo = alunoDem->demanda->oferta->curriculo;
			int discId = alunoDem->demanda->getDisciplinaId();

			// se já foi identificado que tem co-requisitos inserir demanda no mapa
			auto itDiscCoReq = mapDiscCoReq.find(discId);
			if(itDiscCoReq != mapDiscCoReq.end())
			{
				auto itCoReq = coReqsAluno.find(itDiscCoReq->second);
				if(itCoReq != coReqsAluno.end())
					itCoReq->second.push_back(alunoDem);
				else
					HeuristicaNuno::excepcao("DadosHeuristica::preProcCoRequisitos_", "Disciplina associada a co-requisito nao encontrado!");

				continue;
			}

			// se não foi identificado verificar se tem co-requisito
			coReqsDisc.clear();
			curriculo->getCorrequisitos(discId, coReqsDisc);
			if(coReqsDisc.size() == 0)
				continue;

			// se tem co-requisito registar e adicionar demanda
			++idCoReq;
			coReqDems.clear();
			coReqDems.push_back(alunoDem);
			if(!coReqsAluno.insert(make_pair(idCoReq, coReqDems)).second)
				HeuristicaNuno::warning("DadosHeuristica::preProcCoRequisitos_", "Co-requisito nao registado!");
			if(!mapDiscCoReq.insert(make_pair(discId, idCoReq)).second)
				HeuristicaNuno::warning("DadosHeuristica::preProcCoRequisitos_", "Associacao disciplina co-requisito nao registada!");

			// associar as disciplinas do co-requisito a ele
			for(auto itDisc = coReqsDisc.cbegin(); itDisc != coReqsDisc.cend(); ++itDisc)
			{
				int coDiscId = *itDisc;
				if(coDiscId != discId && mapDiscCoReq.find(coDiscId) != mapDiscCoReq.end())
				{
					HeuristicaNuno::warning("DadosHeuristica::preProcCoRequisitos_", "Disciplina ja associada a outro co-requisito");
					continue;
				}
				
				if(!mapDiscCoReq.insert(make_pair(coDiscId, idCoReq)).second)
					HeuristicaNuno::warning("DadosHeuristica::preProcCoRequisitos_", "Associacao (outra) disciplina co-requisito nao registada!");
			}
		}

		// registar co-requisitos do aluno
		regCoRequisitosAluno_(aluno->getAlunoId(), coReqsAluno);
	}
}
void DadosHeuristica::regCoRequisitosAluno_(int alunoId, unordered_map<int, vector<AlunoDemanda*>> const &coReqs)
{
	// co-requisitos do aluno
	vector<vector<AlunoDemanda*>> coReqsAluno;
	unordered_set<int> demandas;
	// só registar co-requisitos para os quais o aluno demande mais de 1 disciplina
	for(auto it = coReqs.cbegin(); it != coReqs.cend(); ++it)
	{
		if(it->second.size() > 1)
		{
			coReqsAluno.push_back(it->second);

			for(auto itDem = it->second.cbegin(); itDem != it->second.cend(); ++itDem)
				demandas.insert((*itDem)->getId());
		}
	}

	if(coReqsAluno.size() == 0)
		return;

	if(!coRequisitosPorAluno.insert(make_pair(alunoId, coReqsAluno)).second)
		HeuristicaNuno::excepcao("DadosHeuristica::regCoRequisitosAluno_", "Vetor de co-requisitos do aluno não inserido");

	// registar demandas do aluno com co-requisito
	auto itDemsAlunoCoReq = demandasAlunoCoReq.find(alunoId);
	if(itDemsAlunoCoReq == demandasAlunoCoReq.end())
		demandasAlunoCoReq[alunoId] = demandas;
	else
		itDemsAlunoCoReq->second.insert(demandas.begin(), demandas.end());
}
bool DadosHeuristica::temCoReq(int alunoId, int demandaId)
{
	auto it = demandasAlunoCoReq.find(alunoId);
	if(it == demandasAlunoCoReq.end())
		return false;

	return (it->second.find(demandaId) != it->second.end());
}

// verificar as demandas e as equivalencias
void DadosHeuristica::checkDemandasEquiv_(void)
{
	for(auto it = HeuristicaNuno::probData->alunos.begin(); it != HeuristicaNuno::probData->alunos.end(); ++it)
	{
		checkDemEquivAluno_(*it);
	}
}

void DadosHeuristica::checkDemEquivAluno_(Aluno* const aluno)
{
	// associacao de disciplina equivalente com originais [ equiv.id -> [origs.id]]
	unordered_map<int, unordered_set<int>> mapEquivOrig;
	getMapEquivAluno(aluno, mapEquivOrig);
	int alunoId = aluno->getAlunoId();

	// imprimir no log disciplinas equivalentes associadas a mais de uma original
	for(auto itMap = mapEquivOrig.begin(); itMap != mapEquivOrig.end(); ++itMap)
	{
		if(itMap->second.size() > 1)
			HeuristicaNuno::logEquivMultiOrig(alunoId, itMap->first, itMap->second);
	}

	// check equivalencia directa entre demandas
	for(auto it = aluno->demandas.begin(); it != aluno->demandas.end(); ++it)
	{
		Disciplina* const disciplina = (*it)->demanda->disciplina;
		int origId = disciplina->getId();
		// Se for uma componente prática ignorar
		if(origId < 0)
			continue;

		// verificar demandas equivalentes
		for(auto itDois = aluno->demandas.begin(); itDois != aluno->demandas.end(); ++itDois)
		{
			Disciplina* const outra = (*itDois)->demanda->disciplina;
			if(outra->getId() < 0)
				continue;

			if(disciplina->discEquivSubstitutas.find(outra) != disciplina->discEquivSubstitutas.end())
				HeuristicaNuno::logDemandasEquiv(alunoId, origId, outra->getId());
			else if(outra->discEquivSubstitutas.find(disciplina) != outra->discEquivSubstitutas.end())
				HeuristicaNuno::logDemandasEquiv(alunoId, outra->getId(), origId);
		}
	}
}


// check print turno
void DadosHeuristica::checkPrintTurno_(AlunoDemanda* const demanda, int alunoId, int discId)
{
	if(demanda->getAlunoId() != alunoId)
		return;

	if(demanda->demanda->getDisciplinaId() != discId)
		return;

	stringstream ss;
	ss << "[CHECK] aluno: " << alunoId << " / disc: " << discId << " / turno: " << demanda->demanda->getTurnoIES()->getNome();
	HeuristicaNuno::logMsg(ss.str(), 1);
}
