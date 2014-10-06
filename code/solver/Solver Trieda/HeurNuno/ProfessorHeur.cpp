#include "ProfessorHeur.h"
#include "UtilHeur.h"
#include "AulaHeur.h"
#include "../HorarioAula.h"
#include "HeuristicaNuno.h"
#include "ParametrosHeuristica.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "TurmaHeur.h"

ProfessorHeur::ProfessorHeur(void)
	: EntidadeAlocavel(true, "professor", true), ehVirtualUnico_(true), professor_(ParametrosHeuristica::professorVirtual)
{
}
ProfessorHeur::ProfessorHeur(Professor* const professor)
	: EntidadeAlocavel(true, "professor"), professor_(professor), ehVirtualUnico_(false)
{
}

ProfessorHeur::~ProfessorHeur(void)
{
}

// todo 
bool ProfessorHeur::estaDisponivelHorarios(int dia, AulaHeur* const aula) const 
{
	if(ehVirtual() || ehVirtualUnico_)
		return true;

	// ver horários
	auto it = professor_->horariosDia.begin();
	for(auto itHor = aula->horarios.begin(); itHor != aula->horarios.end(); ++itHor)
	{
		bool temHor = false;
		DateTime ini;
		(*itHor)->getInicio(ini);
		// nao recomeça pois é ordenado
		for(; it != professor_->horariosDia.end(); it++)
		{
			int diaP = it->getDia();
			if(diaP == dia && it->getHorarioAula()->inicioFimIguais(*itHor))
			{
				temHor = true;
				break;
			}
			// estrutura ordenada, por isso já passou
			if(diaP > dia || (diaP == dia && ini.earlierTime((*it)->getHorarioAula()->getInicio())))
				return false;
		}
		if(!temHor)
			return false;
	}

	return true;
}
bool ProfessorHeur::estaDisponivelHorarios(unordered_map<int, AulaHeur*> const &aulas) const 
{
	if(ehVirtual() || ehVirtualUnico_)
		return true;

	for(auto it = aulas.begin(); it != aulas.end(); ++it)
	{
		if(!estaDisponivelHorarios(it->first, it->second))
			return false;
	}
	return true;
}
bool ProfessorHeur::estaDisponivel(int dia, AulaHeur* const aula) const 
{
	if(ehVirtualUnico_)
		return true;

	// verifica se está disponível de momento
	if(!EntidadeAlocavel::estaDisponivel(dia, aula))
		return false;

	// verifica se prof está disponível naqueles horários
	if(!estaDisponivelHorarios(dia, aula))
		return false;

	// verificar interjornada
	if(HeuristicaNuno::probData->parametros->considerarDescansoMinProf)
	{
		// verifica se viola interjornada com dia anterior
		int diaAnt = UtilHeur::diaAnterior(dia);
		AulaHeur* ultAulaAnt = ultAulaAlocDia(diaAnt);
		if(ultAulaAnt != nullptr && violaInterjornada(ultAulaAnt, diaAnt, aula, dia))
			return false;
	
		// verifica se viola interjornada com dia posterior
		int diaPost = UtilHeur::diaPosterior(dia);
		AulaHeur* primAulaPost = primAulaAlocDia(diaPost);
		if(primAulaPost != nullptr && violaInterjornada(aula, dia, primAulaPost, diaPost))
			return false;
	}

	return true;
}
bool ProfessorHeur::estaDisponivel(unordered_map<int, AulaHeur*> const &aulas) const 
{
	if(ehVirtualUnico_)
		return true;

	// pode ser melhorado
	unordered_set<int> diasSemanaAloc;
	getDiasSemanaAloc(diasSemanaAloc);

	int nrCredsTot = 0;
	for(auto itAulas = aulas.cbegin(); itAulas != aulas.cend(); ++itAulas)
	{
		int dia = itAulas->first;
		if(!estaDisponivel(dia, itAulas->second))
			return false;

		if(diasSemanaAloc.find(dia) == diasSemanaAloc.end())
			diasSemanaAloc.insert(dia);

		nrCredsTot += itAulas->second->nrCreditos();
	}

	// verificar se excede o máximo de créditos semanais
	if(credsAloc_ + nrCredsTot > professor_->getChMax())
		return false;

	// verificar se excede o máximo de dias por semana
	if(HeuristicaNuno::probData->parametros->considerarMaxDiasPorProf && 
		(diasSemanaAloc.size() > professor_->getMaxDiasSemana()))
	{
		return false;
	}

	return true;
}

int ProfessorHeur::nroCredsLivresEstimados() const
{
	int nr = professor_->getNroCredsCadastroDisc() - getNrCreditosAlocados();
	if (nr<0)
	{
		HeuristicaNuno::warning("ProfessorHeur::nroCredsLivresEstimados()","Nro de creditos livres negativo!");
		nr=0;
	}
	return nr;
}

// verifica se duas turmas violam a interjornada
bool ProfessorHeur::violaInterjornada(TurmaHeur* const turma1, TurmaHeur* const turma2)
{
	if(!HeuristicaNuno::probData->parametros->considerarDescansoMinProf
		|| (HeuristicaNuno::probData->parametros->descansoMinProfValue <= 0))
	{
		return false;
	}

	unordered_map<int, AulaHeur*> aulas1;
	unordered_map<int, AulaHeur*> aulas2;
	turma1->getAulas(aulas1);
	turma2->getAulas(aulas2);

	for(auto it1 = aulas1.cbegin(); it1 != aulas1.cend(); ++it1)
	{
		for(auto it2 = aulas2.cbegin(); it2 != aulas2.cend(); ++it2)
		{
			if(violaInterjornada(it1->second, it1->first, it2->second, it2->first))
				return true;
		}
	}
	return false;
}
// verifica se duas aulas violam a interjornada
bool ProfessorHeur::violaInterjornada(AulaHeur* const aula1, int dia1, AulaHeur* const aula2, int dia2)
{
	if(!HeuristicaNuno::probData->parametros->considerarDescansoMinProf
		|| (HeuristicaNuno::probData->parametros->descansoMinProfValue <= 0))
	{
		return false;
	}

	if(aula1 == nullptr || aula2 == nullptr)
		return false;

	// dia1 tem que ser anterior ao dia2. Se dia2 for anterior ao dia1, chamar inverso
	if(dia2 == UtilHeur::diaAnterior(dia1))
		return violaInterjornada(aula2, dia2, aula1, dia1);

	// se dia1 não for anterior ao dia2 return false.
	if(dia1 != UtilHeur::diaAnterior(dia2))
		return false;

	// último horário dia anterior
	DateTime ultHoraUm;
	aula1->getLastHor(ultHoraUm);
	// check
	//UtilHeur::printHoras(ultHoraUm, 1);

	// primeiro horário dia seguinta
	DateTime primHoraDois;
	aula2->getPrimeiroHor(primHoraDois);
	// check
	//UtilHeur::printHoras(primHoraDois, 1);

	double interjornMin = HeuristicaNuno::probData->parametros->descansoMinProfValue;

	// horas até ao final do dia anterior
	double interjorn = 24 - UtilHeur::horasDouble(ultHoraUm);
	interjorn += UtilHeur::horasDouble(primHoraDois);
	//HeuristicaNuno::logMsgDouble("interjorn: ", interjorn, 1);

	// debug
	//char* c = "";
	//cin >> c;

	return interjorn < interjornMin;
}
// verifica se dois horarios violam a interjornada
bool ProfessorHeur::violaInterjornada(HorarioAula* const hor1, int dia1, HorarioAula* const hor2, int dia2)
{
	if(!HeuristicaNuno::probData->parametros->considerarDescansoMinProf
		|| (HeuristicaNuno::probData->parametros->descansoMinProfValue <= 0))
	{
		return false;
	}

	if(hor1 == nullptr || hor2 == nullptr)
		return false;

	// dia1 tem que ser anterior ao dia2. Se dia2 for anterior ao dia1, chamar inverso
	if(dia2 == UtilHeur::diaAnterior(dia1))
		return violaInterjornada(hor2, dia2, hor1, dia1);

	// se dia1 não for anterior ao dia2 return false.
	if(dia1 != UtilHeur::diaAnterior(dia2))
		return false;

	// último horário dia anterior
	DateTime ultHoraUm = hor1->getFinal();
	// primeiro horário dia seguinta
	DateTime primHoraDois = hor2->getInicio();

	double interjornMin = HeuristicaNuno::probData->parametros->descansoMinProfValue;
	double interjorn = 24 - UtilHeur::horasDouble(ultHoraUm);
	interjorn += UtilHeur::horasDouble(primHoraDois);

	return interjorn < interjornMin;
}

// verificar links com as turmas
bool ProfessorHeur::checkLinks(void) const
{
	for(auto itTurmas = turmas_.begin(); itTurmas != turmas_.end(); ++itTurmas)
	{
		if((*itTurmas)->getProfessor()->getId() != this->getId())
		{
			HeuristicaNuno::warning("ProfessorHeur::checkLinks", "Professor associado a uma turma que nao lecciona!");
			return false;
		}
	}
	return true;
}

