#ifndef _AULA_HEUR_H_
#define _AULA_HEUR_H_

#include "UtilHeur.h"
#include "../Calendario.h"
#include "DateTime.h"

class HorarioAula;
class TurmaHeur;

class AulaHeur
{
	friend class TurmaHeur;
public:
	AulaHeur(int calId, int campId, int uniId, set<HorarioAula*> const &hors);
	AulaHeur(const AulaHeur& aula);
	~AulaHeur(void);

	int getId(void) const { return id_; }
	void setId(int id) { id_ = id;}

	const int calendarioId;
	const int campusId;
	const int unidadeId;
	const set<HorarioAula*> horarios;

	bool temCampusUnid(void) { return ((campusId > 0) && (unidadeId > 0));}

	bool temHorario(HorarioAula* const horario) const;

	void getPrimeiroHor (DateTime &time) const { (*horarios.begin())->getInicio(time); }
	void getLastHor (DateTime &time) const { (*horarios.rbegin())->getFinal(time); }

	bool intersectaHorario(HorarioAula* const horario, bool deslocavel = false, int campId = -1, int unidId = -1) const;
	bool intersectaHorario(DateTime const &dti, DateTime const &dtf, bool deslocavel = false, int campId = -1, int unidId = -1) const;
	int nrCreditos() const { return horarios.size();  } 

	virtual bool operator < (const AulaHeur &other) const
	{
		bool iniAntes = ((*horarios.begin())->getInicio().earlierTime((*other.horarios.begin())->getInicio()));
		if(iniAntes)
			return true;

		bool fimAnt = (*horarios.rbegin())->getFinal().earlierTime((*other.horarios.rbegin())->getFinal());
		if(fimAnt)
			return true;

		return id_ < other.id_;
	}

	virtual bool operator > (const AulaHeur &other) const
	{
		bool iniDepois = (*other.horarios.begin())->getInicio().earlierTime((*horarios.begin())->getInicio());
		if(iniDepois)
			return true;

		bool fimDepois = (*other.horarios.rbegin())->getFinal().earlierTime((*horarios.rbegin())->getFinal());
		if(fimDepois)
			return true;

		return id_ > other.id_;
	}

	virtual bool operator == (const AulaHeur& other) const
	{
		/*if(calendarioId != other.calendarioId ||
			campusId != other.campusId ||
			unidadeId != other.unidadeId)
		{
			return false;
		}

		if(nrCreditos() != other.nrCreditos())
			return false;

		for(auto it = horarios.begin(); it != horarios.end(); ++it)
		{
			bool tem = false;
			for(auto itDois = other.horarios.begin(); itDois != other.horarios.end(); ++itDois)
			{
				if(!(*it)->inicioFimIguais(*itDois))
					continue;

				tem = true;
				break;
			}
			if(!tem)
				return false;
		}

		return false;*/

		return id_ == other.id_;
	}

	virtual bool operator != (const AulaHeur& other) const
	{
		return !(*this == other);
	}

	// retorna valor hash. NOTA: considera que as aulas são continuas!
	size_t hashValue (void) const;

	// contar criações/destruições
	static int nrBuild;
	static int nrDestroy;

private:
	int id_;
};

// Comparador de pointers
namespace std
{
	template<>
	struct equal_to<AulaHeur*>
	{
		bool operator() (AulaHeur* const first, AulaHeur* const second) const
		{
			return first->getId() == second->getId();
		}
	};

	template<>
	struct hash<AulaHeur*>
	{
		size_t operator() (AulaHeur* const aula) const
		{
			return aula->getId();
		}
	};
};

#endif

