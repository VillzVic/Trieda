#include "EntidadeAlocavel.h"
#include "TurmaHeur.h"
#include "AulaHeur.h"
#include "../HorarioAula.h"
#include "DadosHeuristica.h"
#include "../ProblemData.h"
#include "HeuristicaNuno.h"
#include "ParametrosHeuristica.h"


EntidadeAlocavel::EntidadeAlocavel(bool deslocavel, std::string const &tipo, bool sempreDisp)
	: turmas_(), deslocavel_(deslocavel), tipo_(tipo), sempreDisp_(sempreDisp), credsAloc_(0)
{
}

EntidadeAlocavel::~EntidadeAlocavel(void)
{
}

// add/remove turmas a que está alocado
void EntidadeAlocavel::addTurma(TurmaHeur* const turma)
{
	if(turmas_.find(turma) != turmas_.end())
		HeuristicaNuno::excepcao("EntidadeAlocavel::addTurma", "Entidade já alocada a essa turma");

	/*if(temConflitos())
	{
		HeuristicaNuno::logMsg(tipo_, 1);
		HeuristicaNuno::excepcao("EntidadeAlocavel::addTurma", "Conflito!");
	}*/

	if(!sempreDisp_)
		addAulasTurma(turma);

	turmas_.insert(turma);
	credsAloc_ += turma->getNrCreditos();
}
void EntidadeAlocavel::removeTurma(TurmaHeur* const turma)
{
	auto it = turmas_.find(turma);
	if(it == turmas_.end())
	{
		HeuristicaNuno::logMsg(tipo_, 0);
		HeuristicaNuno::excepcao("EntidadeAlocavel::removeTurma", "Entidade não esta alocada a essa turma");
	}

	if(!sempreDisp_)
		removeAulasTurma(turma);

	credsAloc_ -= turma->getNrCreditos();
	turmas_.erase(it);
}

// get turmas a que tá alocado
void EntidadeAlocavel::getTurmas(unordered_set<TurmaHeur*>& turmas) 
{
	for(auto itTurmas = turmas_.begin(); itTurmas != turmas_.end(); ++itTurmas)
		turmas.insert(*itTurmas);
}
bool EntidadeAlocavel::temTurmas(void) const
{ 
	return turmas_.size() > 0; 
}
bool EntidadeAlocavel::temTurma(TurmaHeur* turma) const
{ 
	return turmas_.find(turma) != turmas_.end(); 
}
void EntidadeAlocavel::getOfertasDisciplina(unordered_set<OfertaDisciplina*> &ofertas) const
{
	for(auto itTurmas = turmas_.cbegin(); itTurmas != turmas_.cend(); ++itTurmas)
	{
		ofertas.insert((*itTurmas)->ofertaDisc);
	}
}
void EntidadeAlocavel::getOfertasDisciplina(unordered_set<OfertaDisciplina*>* const ofertas) const
{
	for(auto itTurmas = turmas_.cbegin(); itTurmas != turmas_.cend(); ++itTurmas)
	{
		ofertas->insert((*itTurmas)->ofertaDisc);
	}
}

// add/remove aulas dias da turma
void EntidadeAlocavel::addAulasTurma(TurmaHeur* const turma)
{
	unordered_map<int, AulaHeur*> aulas;
	turma->getAulas(aulas);
	for(auto itAula = aulas.begin(); itAula != aulas.end(); ++itAula)
	{
		int dia = itAula->first;
		AulaHeur* const aula = itAula->second;
		if(itAula->second == nullptr)
			HeuristicaNuno::excepcao("EntidadeAlocavel::addAulasTurma", "Aula nula");

		auto itDia = diasAulas_.find(dia);
		if(itDia == diasAulas_.end())
		{
			unordered_set<AulaHeur*> aulasDia;
			auto par = diasAulas_.insert(make_pair<int, unordered_set<AulaHeur*> >(dia, aulasDia));
			if(!par.second)
				HeuristicaNuno::excepcao("EntidadeAlocavel::addAulasTurma", "aulas do dia nao adicionadas!");

			itDia = par.first;
		}
		// check
		/*auto itAulaDia = itDia->second.begin();
		for(; itAulaDia != itDia->second.end(); ++itAulaDia)
		{
			if( (*itAulaDia)->getId() == itAula->second->getId() )
				break;
		}*/
		auto itAulaDia = itDia->second.find(aula);
		if(itAulaDia != itDia->second.end())
		{
			HeuristicaNuno::logMsg(tipo_, 1);
			HeuristicaNuno::excepcao("EntidadeAlocavel::addAulasTurma", "aula ja inserida nesse dia!!");
		}
		else if(!itDia->second.insert(aula).second)
		{
			HeuristicaNuno::logMsg(tipo_, 1);
			HeuristicaNuno::excepcao("EntidadeAlocavel::addAulasTurma", "aula da turma nao adicionada!");
		}
	}
}
void EntidadeAlocavel::removeAulasTurma(TurmaHeur* const turma)
{
	unordered_map<int, AulaHeur*> aulas;
	turma->getAulas(aulas);
	for(auto itAula = aulas.begin(); itAula != aulas.end(); ++itAula)
	{
		AulaHeur* const aula = itAula->second;
		auto itDia = diasAulas_.find(itAula->first);
		if(itDia == diasAulas_.end())
		{
			HeuristicaNuno::logMsg(tipo_, 1);
			HeuristicaNuno::excepcao("EntidadeAlocavel::removeAulasTurma", "Aulas desse dia nao encontradas");
		}
		/*auto itAulaDia = itDia->second.begin();
		for(; itAulaDia != itDia->second.end(); ++itAulaDia)
		{
			if( (*itAulaDia)->getId() == itAula->second->getId())
				break;
		}*/
		auto itAulaDia = itDia->second.find(aula);
		if(itAulaDia == itDia->second.end())
		{
			HeuristicaNuno::logMsg(tipo_, 1);
			HeuristicaNuno::excepcao("EntidadeAlocavel::removeAulasTurma", "Aula nao encontrada!");
		}
		else
		{
			itDia->second.erase(itAulaDia);
			if(itDia->second.size() == 0)
				diasAulas_.erase(itDia);
		}
	}
}


// disponibilidade só olhando a conflitos de horário (e deslocamento se for o caso)
bool EntidadeAlocavel::estaDisponivel(int dia, AulaHeur* const aula) const
{
	// verificar se está disponivel tendo em conta as aulas já alocadas
	auto itDia = diasAulas_.find(dia);
	if(itDia == diasAulas_.end())
		return true;
	
	// Testar: começar por cima ou baixo?
	for(auto itAula = itDia->second.cbegin(); itAula != itDia->second.cend(); ++itAula)
	{
		if(!aulasCompativeis(*itAula, aula, deslocavel_))
			return false;
	}

	return true;
}
bool EntidadeAlocavel::estaDisponivel(int dia, HorarioAula* const horario) const
{
	// verificar se está disponivel tendo em conta as aulas já alocadas
	auto itDia = diasAulas_.find(dia);
	if(itDia == diasAulas_.end())
		return true;

	// Testar: começar por cima ou baixo?
	for(auto itAula = itDia->second.cbegin(); itAula != itDia->second.cend(); ++itAula)
	{
		if((*itAula)->intersectaHorario(horario))
			return false;
	}

	return true;
}
bool EntidadeAlocavel::estaDisponivel(TurmaHeur* const turma, int dia, AulaHeur* const aula) const
{
	HeuristicaNuno::logMsg("EntidadeAlocavel::estaDisponivel(const TurmaHeur* const turma, int dia, int aula)", 3);
	AulaHeur* aulaDia = nullptr;
	if(turma->getAulaDia(dia, aulaDia) && !aulasCompativeis(aulaDia, aula, deslocavel_))
	{
		return false;
	}

	return true;
}
bool EntidadeAlocavel::estaDisponivel(const unordered_map<int, AulaHeur*> &aulas) const
{
	if(aulas.size() == 0)
		HeuristicaNuno::excepcao("EntidadeAlocavel::estaDisponivel", "Nenhuma aula!");

	for(auto it = aulas.cbegin(); it != aulas.cend(); ++it)
	{
		if(!estaDisponivel(it->first, it->second))
			return false;
	}
	return true;
}

// diz se tem nr max de horários sequenciais disponiveis num dia para um conjunto de horarios. 
// OBS: não considerar sequencial intervalo grande! (PH: maxIntervalo!)
bool EntidadeAlocavel::temHorsSeqDisponivel(int nrHors, int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>> const &horarios) const
{
	int nr = 0;
	const int maxInter = ParametrosHeuristica::maxIntervAulas;
	HorarioAula* anterior = nullptr;
	for(auto it = horarios.begin(); it != horarios.end(); ++it)
	{
		// não considerar sequencial intervalo grande!
		if(nr > 0 && UtilHeur::intervaloHorarios(anterior, *it) > maxInter)
			nr = 0;

		if(estaDisponivel(dia, *it))
		{
			nr++;
			if(nr >= nrHors)
				return true;
		}
		else
			nr = 0;

		anterior = (*it);
	}
	return false;
}

// verifica a quantos créditos está alocado nesse dia
int EntidadeAlocavel::credsAlocDia(int dia) const
{
	// verificar se está disponivel tendo em conta as aulas já alocadas
	auto itDia = diasAulas_.find(dia);
	if(itDia == diasAulas_.end())
		return 0;

	int nrCreds = 0;
	for(auto itAula = itDia->second.cbegin(); itAula != itDia->second.cend(); ++itAula)
		nrCreds  += (*itAula)->nrCreditos();

	return nrCreds;
}

// verifica se estes dois horários são compatíveis tendo em conta a deslocação
bool EntidadeAlocavel::violaDeslocacao(HorarioAula* const horario1, int campusId1, int unidadeId1, HorarioAula* const horario2, int campusId2, int unidadeId2)
{
	if(campusId1 < 0 || campusId2 < 0 || unidadeId1 < 0 || unidadeId2 < 0)
		return false;

	int intervalo = UtilHeur::intervaloHorarios(horario1, horario2);

	// intersectam-se, logo violam deslocação
	if(intervalo < 0)
		return true;

	// mesmo campus e unidade
	if((campusId1 == campusId2) && (unidadeId1 == unidadeId2))
		return false;

	// find campi e unidades
	auto itC1 = HeuristicaNuno::probData->refCampus.find(campusId1);
	if(itC1 == HeuristicaNuno::probData->refCampus.end())
		HeuristicaNuno::excepcao("EntidadeAlocavel::violaDeslocacao", "Campus 1 nao encontrado!");
	auto itC2 = HeuristicaNuno::probData->refCampus.find(campusId2);
	if(itC2 == HeuristicaNuno::probData->refCampus.end())
		HeuristicaNuno::excepcao("EntidadeAlocavel::violaDeslocacao", "Campus 2 nao encontrado!");

	auto itU1 = HeuristicaNuno::probData->refUnidade.find(unidadeId1);
	if(itU1 == HeuristicaNuno::probData->refUnidade.end())
		HeuristicaNuno::excepcao("EntidadeAlocavel::violaDeslocacao", "Unidade 1 nao encontrada!");
	auto itU2 = HeuristicaNuno::probData->refUnidade.find(unidadeId2);
	if(itU2 == HeuristicaNuno::probData->refUnidade.end())
		HeuristicaNuno::excepcao("EntidadeAlocavel::violaDeslocacao", "Unidade 2 nao encontrada!");

	Campus* campus1 = itC1->second;
	Campus* campus2 = itC2->second;
	Unidade* unid1 = itU1->second;
	Unidade* unid2 = itU2->second;

	int minimoNecessario = HeuristicaNuno::probData->calculaTempoEntreCampusUnidades(campus1, campus2, unid1, unid2);

	return intervalo < minimoNecessario;
}
bool EntidadeAlocavel::violaDeslocacao(AulaHeur* const aula1, AulaHeur* const aula2)
{
	// sem unidade ainda definida
	if(!aula1->temCampusUnid() || !aula2->temCampusUnid())
		return false;

	HorarioAula* const primHor1 = *(aula1->horarios.begin());
	HorarioAula* const ultHor1 = *(aula1->horarios.rbegin());

	HorarioAula* const primHor2 = *(aula2->horarios.begin());
	HorarioAula* const ultHor2 = *(aula2->horarios.rbegin());

	if(violaDeslocacao(ultHor1, aula1->campusId, aula1->unidadeId, primHor2, aula2->campusId, aula2->unidadeId))
		return true;
	if(violaDeslocacao(ultHor2, aula2->campusId, aula2->unidadeId, primHor1, aula1->campusId, aula1->unidadeId))
		return true;

	return false;
}
// verifica se este conjunto de aulas viola alguma deslocacao para o aluno
bool EntidadeAlocavel::violaAlgumaDeslocacao(unordered_map<int, AulaHeur*> const &aulas)
{
	if(!deslocavel_ || turmas_.size() == 0)
		return false;

	for(auto it = aulas.cbegin(); it != aulas.cend(); ++it)
	{
		int dia = it->first;
		// ver se a entidade ta alocada a aulas nesse dia
		auto itDia = diasAulas_.find(dia);
		if(itDia == diasAulas_.end())
			continue;

		// sem unidade ainda definida
		if(!it->second->temCampusUnid())
			continue;

		for(auto itAulasDia = itDia->second.cbegin(); itAulasDia != itDia->second.cend(); ++itAulasDia)
		{
			if(EntidadeAlocavel::violaDeslocacao(it->second, *itAulasDia))
				return true;
		}
	}
	return false;
}


// verifica se duas aulas são compatíveis tendo em conta a deslocação
bool EntidadeAlocavel::aulasCompativeis(AulaHeur* const aulaUm, AulaHeur* const aulaDois, bool deslocavel)
{
	HeuristicaNuno::logMsg("EntidadeAlocavel::aulasCompativeis(const AulaHeur* const aulaUm, const AulaHeur* const aulaDois, bool deslocavel)", 3);
	
	if(aulaUm->getId() == aulaDois->getId())
		return true;

	// por precaução verifica-se a intersecção de horários	(pois podem haver calendários bizarros com buracos)
	for(auto itHor = aulaUm->horarios.begin(); itHor != aulaUm->horarios.end(); ++itHor)
	{
		if(aulaDois->intersectaHorario(*itHor))
			return false;
	}

	// esta aula antes da outra
	if(deslocavel && aulaUm->temCampusUnid() && aulaDois->temCampusUnid())
	{
		// check
		//if(EntidadeAlocavel::violaDeslocacao(aulaUm, aulaDois) != EntidadeAlocavel::violaDeslocacao(aulaDois, aulaUm))
		//	HeuristicaNuno::excepcao("EntidadeAlocavel::aulasCompativeis", "violaDeslocacao devia dar o mesmo resultado independente da ordem");

		return !EntidadeAlocavel::violaDeslocacao(aulaUm, aulaDois);
	}
	
	return true;
}

// horarios incompativeis
bool EntidadeAlocavel::horariosCompativeis(int campusIdUm, int unidIdUm, HorarioAula* const horUm, 
											int campusIdDois, int unidIdDois, HorarioAula* const horDois, bool deslocavel)
{
	// verificar se se sobrepoem
	if(horUm->sobrepoe(horDois))
		return false;
	if(!deslocavel)
		return true;

	return !EntidadeAlocavel::violaDeslocacao(horUm, campusIdUm, unidIdUm, horDois, campusIdDois, unidIdDois);
}


// última/primeira aula alocado no dia
AulaHeur* EntidadeAlocavel::ultAulaAlocDia(int dia) const
{
	// verificar se está disponivel tendo em conta as aulas já alocadas
	auto itDia = diasAulas_.find(dia);
	if(itDia == diasAulas_.end())
		return nullptr;

	return (*itDia->second.rbegin());
}
AulaHeur* EntidadeAlocavel::primAulaAlocDia(int dia) const
{
	// verificar se está disponivel tendo em conta as aulas já alocadas
	auto itDia = diasAulas_.find(dia);
	if(itDia == diasAulas_.end())
		return nullptr;

	return (*itDia->second.begin());
}

// retorna os dias da semana nos quais a entidade tem alocação
void EntidadeAlocavel::getDiasSemanaAloc(unordered_set<int> &diasSemana) const
{
	for(auto itDia = diasAulas_.cbegin(); itDia != diasAulas_.cend(); ++itDia)
	{
		if(itDia->second.size() > 0)
			diasSemana.insert(itDia->first);
	}
}

bool EntidadeAlocavel::temConflitos(void) const
{
	
	for(auto itTurma1 = turmas_.cbegin(); itTurma1 != turmas_.cend(); ++itTurma1)
	{
		auto itTurma2 = itTurma1;
		for(; itTurma2 != turmas_.cend(); ++itTurma2)
		{
			if((*itTurma1) == (*itTurma2))
				continue;

			if((*itTurma1)->ehIncompativel(*itTurma2, deslocavel_))
				return true;
		}
	}

	return false;
}