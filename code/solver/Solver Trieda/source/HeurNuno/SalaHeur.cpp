#include "SalaHeur.h"
#include "AulaHeur.h"
#include "../HorarioAula.h"
#include "HeuristicaNuno.h"
#include "../ProblemData.h"
#include "TurmaHeur.h"
#include "../ConjUnidades.h"
#include "UtilHeur.h"


SalaHeur::SalaHeur(void)
	: EntidadeAlocavel(false, "sala_virtual", true), sala_(nullptr), indicDem_(0.0), ehVirtual_(true)
{

}

SalaHeur::SalaHeur(Sala* const sala)
	: EntidadeAlocavel(false, "sala"), sala_(sala), indicDem_(0.0), ehVirtual_(false)
{
}

SalaHeur::~SalaHeur(void)
{
}

int SalaHeur::getCapacidade() const
{
	int cap = sala_->getCapacidade();
	if(cap > 0)
		return cap;
	else
	{
		stringstream ss;
		ss << "Sala " << sala_->getId() << " tem capacidade igual a zero ou negativa";
		HeuristicaNuno::warning("SalaHeur::getCapacidade", ss.str());
		return 1;
	}
}

const ConjUnidades* SalaHeur::getConjUnidades(void) const 
{ 
	return HeuristicaNuno::probData->refUnidade.at(sala_->getIdUnidade())->conjunto; 
}

bool SalaHeur::estaDisponivelHorarios(TurmaHeur* const turma) const
{
	unordered_map<int, AulaHeur*> aulas;
	turma->getAulas(aulas);
	for(auto it = aulas.cbegin(); it!= aulas.cend(); ++it)
	{
		if(!estaDisponivelHorarios(it->first, it->second))
			return false;
	}
	return true;
}
bool SalaHeur::estaDisponivelHorarios(int dia, AulaHeur* const aula) const
{
	if(ehVirtual_)
		return true;

	HeuristicaNuno::logMsg("SalaHeur::estaDisponivelHorarios(int dia, int aulaId)", 3);

	auto itHorsSalaDia = sala_->horariosDiaMap.find(dia);
	if(itHorsSalaDia == sala_->horariosDiaMap.end())
		return false;

	// verificar se está disponível neste horário
	auto it = itHorsSalaDia->second.begin();
	for(auto itHor = aula->horarios.begin(); itHor != aula->horarios.end(); ++itHor)
	{
		HorarioAula* const horario = *itHor;

		// verificar se está nas indisponibilidades extra
		if(indisponivelExtra(dia, horario))
			return false;

		bool temHor = false;
		DateTime ini;
		(*itHor)->getInicio(ini);

		for(; it != itHorsSalaDia->second.end(); ++it)
		//for(auto it = sala_->horariosDia.begin(); it != sala_->horariosDia.end(); ++it)
		{
			int diaS = it->getDia();
			if(diaS == dia && it->getHorarioAula()->inicioFimIguais(horario))
			{
				temHor = true;
				break;
			}
			// estrutura ordenada, por isso já passou
			if(diaS > dia || ((diaS == dia) && ini.earlierTime((*it)->getHorarioAula()->getInicio())))
				break;
		}
		if(!temHor)
			return false;
	}

	return true;
}
bool SalaHeur::estaDisponivel(int dia, AulaHeur* const aula) const
{
	if(ehVirtual_)
		return true;

	if(!estaDisponivelHorarios(dia, aula))
		return false;

	if(!EntidadeAlocavel::estaDisponivel(dia, aula))
		return false;

	return true;
}
bool SalaHeur::estaDisponivel(unordered_map<int, AulaHeur*> const &aulas) const
{
	if(ehVirtual_)
		return true;

	if(aulas.size() == 0)
		HeuristicaNuno::excepcao("SalaHeur::estaDisponivel", "Sem aulas!");

	for(auto it = aulas.cbegin(); it!= aulas.cend(); ++it)
	{
		if(!estaDisponivel(it->first, it->second))
			return false;
	}
	return true;
}
bool SalaHeur::estaDisponivel(TurmaHeur* const turma) const
{
	unordered_map<int, AulaHeur*> aulas;
	turma->getAulas(aulas);
	return estaDisponivel(aulas);
}

// verifica indisponibilidades extra carregadas
bool SalaHeur::indisponivelExtra(int dia, HorarioAula* const horario) const
{
	if(HeuristicaNuno::indispExtraSalas.size() == 0)
		return false;

	auto itSala = HeuristicaNuno::indispExtraSalas.find(sala_->getId());
	if(itSala == HeuristicaNuno::indispExtraSalas.end())
		return false;

	auto itDia = itSala->second.find(dia);
	if(itDia == itSala->second.end())
		return false;

	// verificar se o horario está nos turnos proibidos
	for(auto it = itDia->second.cbegin(); it != itDia->second.cend(); ++it)
	{
		if(UtilHeur::turnoAbrange(*it, dia, horario))
			return true;
	}

	return false;
}

// verifica se estao no mesmo conj unidades
bool SalaHeur::mesmoLocal(SalaHeur* const sala) const
{ 
	return this->getConjUnidades() == sala->getConjUnidades(); 
}

// verificar links com as turmas
bool SalaHeur::checkLinks(void) const
{
	for(auto itTurmas = turmas_.begin(); itTurmas != turmas_.end(); ++itTurmas)
	{
		if((*itTurmas)->getSala()->getId() != this->getId())
		{
			HeuristicaNuno::warning("SalaHeur::checkLinks", "Sala associada a uma turma que nao lhe esta alocada!");
			return false;
		}
	}
	return true;
}

// obter novo conjunto de aulas para nova sala
void SalaHeur::getNovasAulas(unordered_map<int, AulaHeur*> const &oldAulas, SalaHeur* const sala, unordered_map<int, AulaHeur*>& newAulas)
{
	newAulas.clear();
	int campusId = sala->campusId();
	int unidId = HeuristicaNuno::probData->refUnidade[sala->unidadeId()]->conjunto->getUnidadeId();
	for(auto itAulas = oldAulas.begin(); itAulas != oldAulas.end(); ++itAulas)
	{
		AulaHeur newAula (itAulas->second->calendarioId, campusId, unidId, itAulas->second->horarios);
		AulaHeur* const newAulaPtr = UtilHeur::saveAula(newAula);
		newAulas[itAulas->first] = newAulaPtr;
	}
}