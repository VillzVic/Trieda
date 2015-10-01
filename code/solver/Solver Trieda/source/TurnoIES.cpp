#include "TurnoIES.h"

#include "HorarioAula.h"
#include "Calendario.h"
#include "CentroDados.h"

TurnoIES::TurnoIES()
{
	nrHorariosSemana = 0;
}

void TurnoIES::addHorarioAula(HorarioAula* h)
{
	horarios_aula.add(h);
	for (auto itDia = h->dias_semana.begin(); itDia != h->dias_semana.end(); ++itDia)
		mapDiaDateTime[*itDia][h->getInicio()].insert(h);
}

int TurnoIES::getNroDeHorariosAula(int dia) const
{
	auto finder = mapDiaDateTime.find(dia);
	if (finder != mapDiaDateTime.end())
		return (int)finder->second.size();
	return 0;
}

bool TurnoIES::possuiHorarioDiaOuCorrespondente(HorarioAula *h, int dia) const
{
	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
	{
		auto finderDti = finderDia->second.find(h->getInicio());
		if (finderDti != finderDia->second.end())
			for (auto itHor = finderDti->second.cbegin(); itHor != finderDti->second.cend(); ++itHor)
				if ((*itHor)->getTempoAula() == h->getTempoAula())
					return true;
	}
	return false;
}

bool TurnoIES::possuiHorarioDia(int dia, DateTime dti, DateTime dtf) const
{
	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
	{
		auto finderDti = finderDia->second.find(dti);
		if (finderDti != finderDia->second.end())
			for (auto itHor = finderDti->second.cbegin(); itHor != finderDti->second.cend(); ++itHor)
				if ((*itHor)->getFinal() == dtf)
					return true;
	}
	return false;
}

GGroup<HorarioAula*, LessPtr<HorarioAula>> TurnoIES::retornaHorarioDiaOuCorrespondente(HorarioAula *h, int dia) const
{
	GGroup<HorarioAula*, LessPtr<HorarioAula>> ggroup;

	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
	{
		auto finderDti = finderDia->second.find(h->getInicio());
		if (finderDti != finderDia->second.end())
			for (auto itHor = finderDti->second.cbegin(); itHor != finderDti->second.cend(); ++itHor)
				if ((*itHor)->getTempoAula() == h->getTempoAula())
					ggroup.add((*itHor));
	}

	return ggroup;
}

bool TurnoIES::possuiHorarioDiaOuCorrespondente(HorarioAula *hi, HorarioAula *hf, int dia)
{
	Calendario *calend = hi->getCalendario();
	int nCreds = calend->retornaNroCreditosEntreHorarios(hi, hf);

	bool valid = true;
	HorarioAula *h = hi;
	for (int j = 1; j <= nCreds; j++)
	{
		bool possuiHor = this->possuiHorarioDiaOuCorrespondente(h, dia);
		if (!possuiHor)
			valid = false;
		h = calend->getProximoHorario(h);
	}

	return valid;
}

HorarioAula* TurnoIES::getHorarioDiaOuCorrespondente(Calendario* const calendario, HorarioAula* const h, int dia)
{
	return getHorarioDiaOuCorrespondente(calendario, h->getInicio(), dia);
}

HorarioAula* TurnoIES::getHorarioDiaOuCorrespondente(Calendario* const calendario, DateTime dti, int dia)
{
	auto itMapDia = mapDiaDateTime.find(dia);
	if (itMapDia != mapDiaDateTime.end())
	{
		auto itMapDateTime = itMapDia->second.find(dti);
		if (itMapDateTime != itMapDia->second.end())
			for (auto itHor = itMapDateTime->second.cbegin(); itHor != itMapDateTime->second.cend(); ++itHor)
				if ((*itHor)->getCalendario()->getId() == calendario->getId())
					return *itHor;
	}
	return nullptr;
}

bool TurnoIES::possuiHorarioDiaOuCorrespondente(Calendario* const calendario, DateTime dti, int dia)
{
	return (getHorarioDiaOuCorrespondente(calendario, dti, dia) != nullptr);
}

void TurnoIES::retornaHorariosDisponiveisNoDia(int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>>& horarios) const
{
	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
		for (auto itDti = finderDia->second.cbegin(); itDti != finderDia->second.cend(); itDti++)
			for (auto itHor = itDti->second.cbegin(); itHor != itDti->second.cend(); itHor++)
				horarios.add(*itHor);
}

//// retorna horarios disponiveis no dia
//void TurnoIES::retornaHorariosDisponiveisNoDia(int dia, std::map<DateTime, std::set<HorarioAula*>>* horarios) const
//{
//	horarios = nullptr;
//	auto finderDia = mapDiaDateTime.find(dia);
//	if (finderDia != mapDiaDateTime.end())
//		horarios = &finderDia->second;
//}

bool TurnoIES::get31Tempo(Calendario* const calendario, DateTime &dti) const
{
	auto itDia = mapDiaDateTime.cbegin();

	bool found = false;
	auto ptLastDti = itDia->second.rbegin();
	while (!found && ptLastDti != itDia->second.rend())
	{
		for (auto itHor = ptLastDti->second.cbegin(); itHor != ptLastDti->second.cend(); itHor++)
			if ((*itHor)->getCalendario() == calendario)
			{
				dti = ptLastDti->first;
				return true;
			}
		ptLastDti++;
	}
	CentroDados::printError("TurnoIES::get31Tempo", "31o DateTime nao encontrado!!!");
	return false;
}
