
#include "AlunoHeur.h"
#include "../ProblemData.h"
#include "../Disciplina.h"
#include "../Demanda.h"
#include "../AlunoDemanda.h"
#include "OfertaDisciplina.h"
#include "TurmaHeur.h"
#include "UtilHeur.h"
#include "AulaHeur.h"
#include "HeuristicaNuno.h"
#include "../Oferta.h"
#include "DadosHeuristica.h"
#include "../TurnoIES.h"
#include "AbridorTurmas.h"

AlunoHeur::flagComp AlunoHeur::flagCompAlunos = AlunoHeur::flagComp::MAIS_CRED;

AlunoHeur::AlunoHeur(Aluno* const aluno)
	: EntidadeAlocavel(true, "aluno"), aluno_(aluno), nrDemAtendP1_(0)
{
}

AlunoHeur::~AlunoHeur(void)
{
}

// retorma demanda original associada ao aluno
AlunoDemanda* AlunoHeur::getDemandaOriginal(Campus* const campus, Disciplina* const disciplina, bool exc) const 
{
	AlunoDemanda* demanda = nullptr;
	pair<int, AlunoDemanda*> parDem;
	if(DadosHeuristica::getPrioridadeDemanda(campus, disciplina, aluno_->getAlunoId(), parDem))
		demanda = parDem.second;

	if(demanda == NULL || demanda == nullptr)
	{
		if(exc)
			HeuristicaNuno::excepcao("AlunoHeur::getDemandaOriginal", "Demanda original não encontrada!");
	}
	else if(demanda->getAlunoId() != getId())
		HeuristicaNuno::excepcao("AlunoHeur::getDemandaOriginal", "Demanda nao e deste aluno!!!");

	return demanda;
}
AlunoDemanda* AlunoHeur::getDemandaOriginal(OfertaDisciplina* const oferta, bool exc) const
{
	return getDemandaOriginal(oferta->getCampus(), oferta->getDisciplina(), exc);
}

// adiciona oferta às demandas alocadas do aluno
void AlunoHeur::addOferta(OfertaDisciplina* const oferta)
{
	AlunoDemanda* const demandaOriginal = getDemandaOriginal(oferta->getCampus(), oferta->getDisciplina());
	if(demandaOriginal == nullptr || demandaOriginal == NULL)
		HeuristicaNuno::excepcao("AlunoHeur::addOferta", "Demanda do aluno não encontrada!");
	
	// registar demanda atendida
	int demandaId = demandaOriginal->getId();
	if(demandasAtendidas_.find(demandaId) != demandasAtendidas_.end())
		HeuristicaNuno::excepcao("AlunoHeur::addOferta", "Demanda do aluno ja atendida!");
	demandasAtendidas_.insert(make_pair(demandaId, oferta));

	// verificar equivalencia forçada
	bool equivForc = demandaOriginal->getExigeEquivalenciaForcada();
	if(equivForc && (std::abs(demandaOriginal->demanda->getDisciplinaId()) == std::abs(oferta->getDisciplina()->getId()))
		&& (demandaOriginal->getCampus()->getId() == oferta->getCampus()->getId()))
	{
		HeuristicaNuno::excepcao("AlunoHeur::addOferta", "Equivalencia forcada nao respeitada!");
	}

	if(demandaOriginal->getPrioridade() == 1)
		nrDemAtendP1_++;
}
void AlunoHeur::removeOferta(OfertaDisciplina* const oferta)
{
	AlunoDemanda* const demandaOriginal = getDemandaOriginal(oferta->getCampus(), oferta->getDisciplina());
	if(demandaOriginal == nullptr)
		HeuristicaNuno::excepcao("AlunoHeur::removeOferta", "Demanda do aluno não encontrada!");

	// remover demanda atendida
	int demandaId = demandaOriginal->getId();
	auto itDem = demandasAtendidas_.find(demandaId);
	if(itDem == demandasAtendidas_.end())
		HeuristicaNuno::excepcao("AlunoHeur::removeOferta", "Demanda do aluno nao registada!");
	demandasAtendidas_.erase(itDem);

	if(demandaOriginal->getPrioridade() == 1)
		nrDemAtendP1_--;
}

bool AlunoHeur::temTurmaOferta(OfertaDisciplina* const oferta) const
{
	for(auto it = turmas_.cbegin(); it != turmas_.cend(); ++it)
	{
		if((*it)->ofertaDisc == oferta)
			return true;
	}
	return false;
}

// get ofertas em que está incluido
bool AlunoHeur::alocCompleto(void)  const
{
	unordered_map<OfertaDisciplina*, unordered_set<TurmaHeur*>> ofertas;
	unordered_set<TurmaHeur*> emptySet;
	for(auto it = turmas_.cbegin(); it != turmas_.cend(); ++it)
	{
		OfertaDisciplina* const oferta = (*it)->ofertaDisc;
		auto itOft = ofertas.find(oferta);
		if(itOft == ofertas.end())
			itOft = ofertas.insert(make_pair(oferta, emptySet)).first;

		// verificar se há uma turma do mesmo tipo
		for(auto itT = itOft->second.cbegin(); itT != itOft->second.cend(); ++itT)
		{
			if((*itT)->tipoAula == (*it)->tipoAula)
			{
				HeuristicaNuno::warning("AlunoHeur::alocCompleto", "Aluno alocado a duas turmas da mesma oferta e mesmo tipo de aula");
				return false;
			}
		}

		// inserir turma
		itOft->second.insert(*it);
	}

	// verificar se tá alocado ao numero certo de turmas!
	for(auto itOft = ofertas.cbegin(); itOft != ofertas.cend(); ++itOft)
	{
		if(itOft->second.size() != itOft->first->nrTiposAula())
			return false;
	}

	return true;
}

// já alocado?
bool AlunoHeur::jaAlocDemanda(AlunoDemanda* const &demanda) const
{
	if(getId() != demanda->getAlunoId())
	{
		HeuristicaNuno::warning("AlunoHeur::jaAlocDemanda", "Demanda nao e deste aluno!");
		return false;
	}

	return (demandasAtendidas_.find(demanda->getId()) != demandasAtendidas_.end());
}

// procurar uma turma de uma disciplina ou equivalente e mesmo tipo
void AlunoHeur::getTurmaTipoEquiv(OfertaDisciplina* const oferta, bool teorico, TurmaHeur* &turma) const
{
	Disciplina* const disciplina = oferta->getDisciplina();
	AlunoDemanda* const alunoDem = getDemandaOriginal(oferta->getCampus(), disciplina);
	unordered_set<int> discs;

	if(!alunoDem->getExigeEquivalenciaForcada())
		discs.insert(alunoDem->demanda->getDisciplinaId());

	// juntar as disciplinas que interessa procurar
	for(auto itDiscEquiv = alunoDem->demanda->discIdSubstitutasPossiveis.begin(); 
				itDiscEquiv != alunoDem->demanda->discIdSubstitutasPossiveis.end(); ++itDiscEquiv)
	{
		int id = itDiscEquiv->second->getId();
		if(id < 0)
			continue;

		int priorEquiv = DadosHeuristica::getPrioridade(oferta->getCampus(), itDiscEquiv->second, getId());
		if(!DadosHeuristica::checkEquivalencia(alunoDem, oferta->getCampus(), itDiscEquiv->second, alunoDem->getAlunoId()))
			continue;

		discs.insert(id);
	}

	// procurar turma
	turma = nullptr;
	for(auto it = turmas_.cbegin(); it != turmas_.cend(); ++it)
	{
		int idDisc = (*it)->ofertaDisc->getDisciplina()->getId();
		if(discs.find(idDisc) == discs.end())
			continue;

		// se esta oferta disciplina tem 2 tipos de aula, então esperar para ter o que procuramos. senão, devolver o unico
		if(((*it)->ofertaDisc->nrTiposAula() == 2) && ((*it)->ehTeoricaTag() != teorico))
			continue;

		turma = *it;
		return;
	}
}

// Disponibilidade
bool AlunoHeur::estaDisponivelHorarios(OfertaDisciplina* const oferta, int dia, AulaHeur* const aula) const 
{
	//HeuristicaNuno::logMsg("disponivel Horário");

	Disciplina* const disciplina = oferta->getDisciplina();
	if(disciplina->getId() < 0)
		HeuristicaNuno::warning("AlunoHeur::estaDisponivelHorarios", "Atencao, id negativo!");

	//HeuristicaNuno::logMsg("get aluno demanda...");
	AlunoDemanda const *alunoDem = getDemandaOriginal(oferta->getCampus(), disciplina);
	if(alunoDem == nullptr || alunoDem == NULL)
		return true;
	
	// Com Turno
	TurnoIES* const turno = alunoDem->demanda->getTurnoIES();
	
	// Com Calendario
	Calendario* const calendario = alunoDem->demanda->getCalendario();

	if(aula == nullptr)
		HeuristicaNuno::excepcao("AlunoHeur::estaDisponivelHorarios", "Aula e nula!");

	for(auto itHor = aula->horarios.begin(); itHor != aula->horarios.end(); ++itHor)
	{
		if(!UtilHeur::turnoAbrange(turno, dia, *itHor))
		{
			return false;
		}
		if(!UtilHeur::calendarioAbrangeNoTurno(calendario, turno, dia, *itHor))
		{
			//std::cout << "\nCaso impedido: hor " << (*itHor)->getInicio()
			//	<< " (id=" << (*itHor)->getId()
			//	<< "), dia " << dia << ", aluno " << this->getAluno()->getAlunoId()
			//	<< ", alunodemanda " << alunoDem->getId()
			//	<< ", disc " << oferta->getDisciplina()->getId();
			return false;
		}
	}
	
	return true;
}
bool AlunoHeur::estaDisponivel(OfertaDisciplina* const oferta, int dia, AulaHeur* const aula) const 
{
	// verificar se está disponível nestes horários
	if(!estaDisponivelHorarios(oferta, dia, aula))
		return false;

	// verificar se está disponível tendo em conta as turmas já alocadas
	if(!EntidadeAlocavel::estaDisponivel(dia, aula))
		return false;

	return true;
}
bool AlunoHeur::estaDisponivel(OfertaDisciplina* const oferta, unordered_map<int, AulaHeur*> const &aulas) const 
{
	if(aulas.size() == 0)
		HeuristicaNuno::excepcao("AlunoHeur::estaDisponivel", "Nenhuma aula!");

	for(auto itAulas = aulas.begin(); itAulas != aulas.end(); ++itAulas)
	{
		if(!estaDisponivel(oferta, itAulas->first, itAulas->second))
			return false;
	}
	return true;
}
bool AlunoHeur::estaDisponivel(TurmaHeur* const turma) const 
{
	// check se excede os creditos
	unordered_map<int, AulaHeur*> aulas;
	turma->getAulas(aulas);

	return estaDisponivel(turma->ofertaDisc, aulas);
}
bool AlunoHeur::estaDisponivelAlguma(unordered_set<TurmaHeur*> const &turmas) const
{
	if(turmas.size() == 0)
		return true;

	for(auto it = turmas.begin(); it != turmas.end(); ++it)
	{
		if(estaDisponivel(*it))
			return true;
	}
	return false;
}
bool AlunoHeur::estaDisponivelRealoc(TurmaHeur* const turma) const
{
	for(auto itTurmas = turmas_.begin(); itTurmas != turmas_.end(); ++itTurmas)
	{
		if(*itTurmas == turma)
			continue;

		if(turma->ehIncompativel(*itTurmas, deslocavel_))
			return false;
	}

	return true;
}
// verifica se o aluno esta disponivel para alguma das turmas e se elas têm capacidade

// nr max de horários sequenciais que está disponivel num dia para uma disciplina
bool AlunoHeur::temHorsSeqDisponivel(int nrHors, OfertaDisciplina* const oferta, int dia)
{
	TurnoIES* const turno = getDemandaOriginal(oferta->getCampus(), oferta->getDisciplina())->demanda->getTurnoIES();
	GGroup<HorarioAula*, LessPtr<HorarioAula>> horarios;
	turno->retornaHorariosDisponiveisNoDia(dia, horarios);

	return EntidadeAlocavel::temHorsSeqDisponivel(nrHors, dia, horarios);
}

// co-requisitos
void AlunoHeur::getCoRequisitos (vector<vector<AlunoDemanda*>> &coReq) const
{
	auto itAluno = DadosHeuristica::coRequisitosPorAluno.find(aluno_->getAlunoId());
	if(itAluno != DadosHeuristica::coRequisitosPorAluno.end())
		coReq = itAluno->second;
}
bool AlunoHeur::temCoRequisito (OfertaDisciplina* const &oferta) const
{
	AlunoDemanda* const demanda = getDemandaOriginal(oferta);
	
	return DadosHeuristica::temCoReq(this->getId(), demanda->getId());
}
bool AlunoHeur::temCoReqsIncompletos (void) const
{
	auto itAluno = DadosHeuristica::coRequisitosPorAluno.find(aluno_->getAlunoId());
	if(itAluno == DadosHeuristica::coRequisitosPorAluno.end())
		return false;

	for(auto it = itAluno->second.cbegin(); it != itAluno->second.cend(); ++it)
	{
		int nrDisc = (int)it->size();
		if(nrDisc <= 1)
		{
			HeuristicaNuno::warning("AlunoHeur::temCoReqsIncompletos", "Co-requisito com 1 ou menos demandas.");
			continue;

		}

		int nrAtend = 0;
		for(auto itDem = it->cbegin(); itDem != it->cend(); ++itDem)
		{
			if(jaAlocDemanda(*itDem))
				++nrAtend;
		}
		if((nrAtend > 0) && (nrAtend != nrDisc))
			return true;
	}
	return false;
}
void AlunoHeur::getOfertasCoReqsIncompletos (vector<OfertaDisciplina*> &ofertas) const
{
	auto itAluno = DadosHeuristica::coRequisitosPorAluno.find(aluno_->getAlunoId());
	if(itAluno == DadosHeuristica::coRequisitosPorAluno.end())
		return;

	vector<OfertaDisciplina*> ofertasCoReq;
	for(auto it = itAluno->second.cbegin(); it != itAluno->second.cend(); ++it)
	{
		int nrDisc = (int)it->size();
		if(nrDisc <= 1)
		{
			HeuristicaNuno::warning("AlunoHeur::getOfertasCoReqsIncompletos", "Co-requisito com 1 ou menos demandas.");
			continue;

		}

		ofertasCoReq.clear();
		for(auto itDem = it->cbegin(); itDem != it->cend(); ++itDem)
		{
			auto itAtend = demandasAtendidas_.find((*itDem)->getId());
			if(itAtend != demandasAtendidas_.end())
				ofertasCoReq.push_back(itAtend->second);
		}

		// verificar se co-requisito foi parcialmente atendido
		int nrAtend = (int)ofertasCoReq.size();
		if( (nrAtend > 0) && (nrAtend != nrDisc) )
			ofertas.insert(ofertas.end(), ofertasCoReq.begin(), ofertasCoReq.end());
	}
}

// imprimir demandas atendidas (disciplina id requerida + disciplina id da oferta)
void AlunoHeur::printDemsAtendidas(void) const
{
	HeuristicaNuno::logMsgInt(">> Aluno ", getId(), 1);
	HeuristicaNuno::logMsgInt("# demandas atendidas: ", (int)demandasAtendidas_.size(), 1);
	for(auto itDems = demandasAtendidas_.cbegin(); itDems != demandasAtendidas_.cend(); ++itDems)
	{
		std::stringstream ss;
		ss << "demanda: " << itDems->first;
		HeuristicaNuno::logMsg(ss.str(), 1);
	}
}

bool AlunoHeur::checkLinks(void) const
{
	auto itTurmas = turmas_.begin();
	for(; itTurmas != turmas_.end(); ++itTurmas)
	{
		if((*itTurmas)->temAluno(getId()) == false)
		{
			HeuristicaNuno::warning("AlunoHeur::checkLinks", "Aluno associado a uma turma a que nao pertence!");
			return false;
		}
	}
	return true;
}


// [COMPARADORES]

// da prioridade a calouros, depois formandos, depois alunos com MAIS creditos
bool AlunoHeur::compAlunosI(AlunoHeur* const first, AlunoHeur* const second)
{
	// check priorização por turnos
	bool veredito = true;
	if(checkTurnoDisc(first, second, veredito))
		return veredito;

	// check se há priorização de formando/calouro
	bool check = checkFormCal(first, second, veredito);
	if(check)
		return veredito;

	// prioridade a mais credito alocados
	/*int credsUm = first->getNrCreditosAlocados();
	int credsDois = second->getNrCreditosAlocados();
	if(credsUm != credsDois)
		return credsUm > credsDois;*/

	// mais creditos para alocar
	int toCredsUm = first->maxCredsAloc();
	int toCredsDois = second->maxCredsAloc();
	if(toCredsUm != toCredsDois)
		return toCredsUm > toCredsDois;

	// prioridade a demanda original sobre equivalente
	check = checkDemOrig(first, second, veredito);
	if(check)
		return veredito;

	return first->getId() < second->getId();
}
// da prioridade a calouros, depois formandos, depois alunos com MENOS creditos
bool AlunoHeur::compAlunosII(AlunoHeur* const first, AlunoHeur* const second)
{
	// check priorização por turnos
	bool veredito = true;
	if(checkTurnoDisc(first, second, veredito))
		return veredito;

	// check se há priorização de formando/calouro
	bool check = checkFormCal(first, second, veredito);
	if(check)
		return veredito;

	// prioridade a menos creditos alocados
	/*int credsUm = first->getNrCreditosAlocados();
	int credsDois = second->getNrCreditosAlocados();
	if(credsUm != credsDois)
		return credsUm < credsDois;*/

	// mais creditos para alocar
	int toCredsUm = first->maxCredsAloc();
	int toCredsDois = second->maxCredsAloc();
	if(toCredsUm != toCredsDois)
		return toCredsUm > toCredsDois;

	// prioridade a demanda original sobre equivalente
	check = checkDemOrig(first, second, veredito);
	if(check)
		return veredito;

	return first->getId() < second->getId();
}

// [Comparadores singulares]

// check formandos calouros (Retorna se encontrou)
bool AlunoHeur::checkFormCal(AlunoHeur* const first, AlunoHeur* const second, bool &veredito)
{
	// Priorizar calouros
	if(HeuristicaNuno::probData->parametros->priorizarCalouros)
	{
		bool cal = first->ehCalouro();
		bool calOutro = second->ehCalouro();
		if(cal != calOutro)
		{
			veredito = cal;
			return true;
		}
	}

	// $PForm Priorizar Formandos
	if(HeuristicaNuno::probData->parametros->priorizarFormandos)
	{
		bool form = first->ehFormando();
		bool formOutro = second->ehFormando();
		if(form != formOutro)
		{
			veredito = form;
			return true;
		}
	}
	return false;
}
// da prioridade a calouros, depois formandos, depois alunos com MENOS creditos
bool AlunoHeur::checkTurnoDisc(AlunoHeur* const first, AlunoHeur* const second, bool& veredito)
{
	OfertaDisciplina* const oft = AbridorTurmas::currOferta;
	if(oft == nullptr)
		return false;

	TurnoIES* const turnoFirst = first->getDemandaOriginal(oft->getCampus(), oft->getDisciplina())->demanda->getTurnoIES();
	TurnoIES* const turnoSecond = second->getDemandaOriginal(oft->getCampus(), oft->getDisciplina())->demanda->getTurnoIES();

	// comparar
	int diff = turnoFirst->nrHorariosSemana - turnoSecond->nrHorariosSemana;
	if(diff != 0)
	{
		veredito = (diff < 0);
		return true;
	}
	else
		return false;
}
// da prioridade a demanda original sobre equivalente
bool AlunoHeur::checkDemOrig(AlunoHeur* const first, AlunoHeur* const second, bool &veredito)
{
	OfertaDisciplina* const oft = AbridorTurmas::currOferta;
	if(oft == nullptr)
		return false;

	Demanda* demOrigFst = first->getDemandaOriginal(oft->getCampus(), oft->getDisciplina())->demanda;
	int discFst = demOrigFst->getDisciplinaId();
	Demanda* demOrigScd = second->getDemandaOriginal(oft->getCampus(), oft->getDisciplina())->demanda;
	int discScd = demOrigScd->getDisciplinaId();

	if(demOrigFst == demOrigScd)
		return false;
	
	bool fstOrig = (discFst == oft->getDisciplina()->getId());
	bool scdOrig = (discScd == oft->getDisciplina()->getId());
	// check se um é original e outro equivalente
	if(fstOrig != scdOrig)
	{
		veredito = fstOrig;
		return true;
	}

	// check oferta id
	/*int fstOft = first->getAluno()->getOfertaId(demOrigFst);
	int scdOft = second->getAluno()->getOfertaId(demOrigScd);
	if(fstOft != scdOft)
		return fstOft < scdOft;*/

	return false;
}