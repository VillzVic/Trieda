#include "AulaHeur.h"
#include "../HorarioAula.h"
#include "UtilHeur.h"
#include "EntidadeAlocavel.h"
#include "HeuristicaNuno.h"

int AulaHeur::nrBuild = 0;
int AulaHeur::nrDestroy = 0;

AulaHeur::AulaHeur(int calId, int campId, int uniId, set<HorarioAula*> const &hors)
	 : calendarioId(calId), campusId(campId), unidadeId(uniId), horarios(hors), id_(0)
{
	++AulaHeur::nrBuild;
}

AulaHeur::AulaHeur(const AulaHeur& aula)
	: calendarioId(aula.calendarioId), campusId(aula.campusId), unidadeId(aula.unidadeId),
	horarios(aula.horarios), id_(aula.id_)
{
	++AulaHeur::nrBuild;
}

AulaHeur::~AulaHeur(void)
{
	++AulaHeur::nrDestroy;
}

bool AulaHeur::temHorario(HorarioAula* const horario) const
{
	for(auto it = horarios.cbegin(); it != horarios.cend(); ++it)
	{
		if((*it)->inicioFimIguais(horario))
			return true;
	}
	return false;
}

bool AulaHeur::intersectaHorario(HorarioAula* const horario, bool deslocavel, int campId, int unidId) const 
{
	for(auto it = horarios.cbegin(); it != horarios.cend(); ++it)
	{
		if((*it)->sobrepoe(horario))
			return true;
	}

	if(deslocavel && campId > 0 && unidId > 0)
	{
		// verificar se viola deslocacao
		DateTime inicioAula;
		getPrimeiroHor (inicioAula);
		DateTime fimHor;
		horario->getFinal(fimHor);

		if(fimHor.earlierTime(inicioAula))
			return EntidadeAlocavel::violaDeslocacao(horario, campId, unidId, (*horarios.begin()), this->campusId, this->unidadeId);
		else
			return EntidadeAlocavel::violaDeslocacao((*horarios.rbegin()), this->campusId, this->unidadeId, horario, campId, unidId);
	}

	return false;
}

bool AulaHeur::intersectaHorario(DateTime const &dti, DateTime const &dtf, bool deslocavel, int campId, int unidId) const 
{
	for(auto it = horarios.cbegin(); it != horarios.cend(); ++it)
	{
		if((*it)->sobrepoe(dti,dtf))
			return true;
	}

	//if(deslocavel && campId > 0 && unidId > 0)
	//{
	//	// verificar se viola deslocacao
	//	DateTime inicioAula;
	//	getPrimeiroHor (inicioAula);
	//	DateTime fimHor;
	//	horario->getFinal(fimHor);

	//	if(fimHor.earlierTime(inicioAula))
	//		return EntidadeAlocavel::violaDeslocacao(horario, campId, unidId, (*horarios.begin()), this->campusId, this->unidadeId);
	//	else
	//		return EntidadeAlocavel::violaDeslocacao((*horarios.rbegin()), this->campusId, this->unidadeId, horario, campId, unidId);
	//}

	return false;
}


size_t AulaHeur::hashValue(void) const
{
	using boost::hash_combine;

	// Start with a hash value of 0    .
	size_t seed = 0;

	hash_combine(seed, calendarioId);
	//hash_combine(seed, dia_);
	hash_combine(seed, campusId);
	hash_combine(seed, unidadeId);

	// início e fim
	HorarioAula* const primeiro = *(horarios.begin());
	HorarioAula* const ultimo = *(horarios.rbegin());
	hash_combine(seed, primeiro->getInicioTimeMin());
	hash_combine(seed, ultimo->getFinalTimeMin());

	// nr creditos
	hash_combine(seed, nrCreditos());

	return seed;
}