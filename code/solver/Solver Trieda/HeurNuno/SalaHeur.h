#ifndef _SALA_HEUR_H_
#define _SALA_HEUR_H_

#include <unordered_set>
#include <unordered_map>
#include "../Sala.h"
#include "EntidadeAlocavel.h"
#include "../ConjUnidades.h"

class EntidadeAlocavel;

using namespace std;

class SalaHeur : public EntidadeAlocavel
{
public:
	SalaHeur(void);
	SalaHeur(Sala* const sala);
	~SalaHeur(void);

	int getId() const { return sala_->getId(); }
	Sala* getSala() const { return sala_; }
	bool ehVirtual() const { return ehVirtual_; }

	int getCapacidade() const;
	int campusId() const { return sala_->getIdCampus(); }
	int unidadeId() const { return sala_->getIdUnidade(); }
	bool ehLab() const { return sala_->ehLab(); }
	const ConjUnidades* getConjUnidades(void) const;

	// disponibilidade
	bool estaDisponivelHorarios(int dia, AulaHeur* const aula) const ;
	bool estaDisponivelHorarios(TurmaHeur* const turma) const ;
	bool estaDisponivel(int dia, AulaHeur* const aula) const ;
	bool estaDisponivel(unordered_map<int, AulaHeur*> const &aulas) const ;
	bool estaDisponivel(TurmaHeur* const turma) const;

	// verifica indisponibilidades extra carregadas
	bool indisponivelExtra(int dia, HorarioAula* const horario) const;

	// check
	bool mesmoLocal(SalaHeur* const sala) const;
	// verificar links com as turmas
	bool checkLinks(void) const;

	// obter novo conjunto de aulas para nova sala
	static void getNovasAulas(unordered_map<int, AulaHeur*> const &oldAulas, SalaHeur* const sala, unordered_map<int, AulaHeur*>& newAulas);

	// get / set indicador demanda
	double getIndicDem(void) { return indicDem_; }
	void resetIndicDem(void) { indicDem_ = 0; }
	double setIndicDem(double credsAlunos) { indicDem_ = credsAlunos/getCapacidade(); return indicDem_; }
	double decIndicDem(double decCredsAlunos) { indicDem_ -= (decCredsAlunos/getCapacidade()); return indicDem_;}
	double incIndicDem(double incCredsAlunos) { indicDem_ += (incCredsAlunos/getCapacidade()); return indicDem_;}

	virtual bool operator == (const SalaHeur &other) const
	{
		return sala_->getId() == other.getSala()->getId();
	}

	virtual bool operator != (const SalaHeur &other) const
	{
		return !(*this == other);
	}

	virtual bool operator < (const SalaHeur &other) const
	{
		return getCapacidade() < other.getCapacidade();
	}

private:
	Sala* const sala_;
	bool ehVirtual_;

	// indicador de demanda. (nr de créditos de aluno demandados / capacidade da sala)
	double indicDem_;
};

// Comparador de pointers
namespace std
{
	template<>
	struct less<SalaHeur*>
	{
		bool operator() (SalaHeur* const first, SalaHeur* const second)
		{
			// less is pratica
			if(first->ehLab() != second->ehLab())
				return !(first->ehLab());

			// less is menor capacidade
			int diffCap = first->getCapacidade() - second->getCapacidade();
			if(diffCap != 0)
				return diffCap < 0;

			// less is menor indice demanda
			int diffDem = first->getIndicDem() - second->getIndicDem();
			if(diffDem != 0)
				return diffDem < 0;

			return first < second;
		}
	};

	template<>
	struct equal_to<SalaHeur*>
	{
		bool operator() (SalaHeur* const first, SalaHeur* const second) const
		{
			return first->getId() == second->getId();
		}
	};

	template<>
	struct hash<SalaHeur*>
	{
		size_t operator() (SalaHeur* const sala) const
		{
			return sala->getId();
		}
	};
};

#endif

