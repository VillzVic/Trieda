#ifndef _HORARIO_AULA_H_
#define _HORARIO_AULA_H_

#include "OFBase.h"

#include <iostream>

#include "GGroup.h"
#include "DateTime.h"

class Calendario;

class HorarioAula : public OFBase
{
public:
	HorarioAula();
	virtual ~HorarioAula() {};

	virtual void le_arvore(ItemHorarioAula &);

	GGroup<int> dias_semana; // Dias referentes à disponibilidade do Calendario! Para outras estruturas, os dias devem estar em Horario...

	DateTime getInicio() const { return inicio; }
	void getInicio(DateTime& date) const { date = inicio; }
	void setInicio(DateTime dt) { inicio = dt; }

	DateTime getFinal() const;
	void getFinal(DateTime& date) const;

	Calendario * getCalendario() const { return calendario; }
	void setCalendario(Calendario * c) { calendario = c; }

	int getTempoAula() const { return tempo_aula; }
	void setTempoAula(int value) { tempo_aula = value; }

	int getTurnoIESId() const { return turnoIES; }
	void setTurnoIESId(int v) { turnoIES = v; }

	int getFaseDoDia() const {
		if (faseDoDia == -1)
			std::cout << "\nErro, fase do dia nao setada para horAula id" << this->getId();
		return faseDoDia;
	}
	void setFaseDoDia(int fase) { faseDoDia = fase; }

	bool horarioDisponivel(int dia);
	bool sobrepoe(HorarioAula const &h);
	bool sobrepoe(HorarioAula* const &h);
	bool sobrepoe(DateTime const &dti, DateTime const &dtf) const;

	// retorna nr minutos do inicio e fim
	int getInicioTimeMin(void) const { return inicio.timeMin(); }
	int getFinalTimeMin(void) const { return inicio.timeMin() + tempo_aula; }

	bool comparaMenor(const HorarioAula& right) const
	{
		bool mesmoInicio = (inicio.sameTime(right.inicio));

		// Horários de início de aula diferentes
		if (!mesmoInicio)
			return (inicio.earlierTime(right.inicio));

		// Quando os horários_aula começam no mesmo
		// instante, comparamos o horário de final da aula
		int diff = tempo_aula - right.tempo_aula;

		return (diff < 0);
	}

	bool operator<(const HorarioAula& right) const
	{
		bool mesmoInicio = (inicio.sameTime(right.inicio));

		// Horários de início de aula diferentes
		if (!mesmoInicio)
			return (inicio.earlierTime(right.inicio));

		// Quando os horários_aula começam no mesmo
		// instante, comparamos o horário de final da aula
		int diff = tempo_aula - right.tempo_aula;
		if (diff == 0)
			return (getId() < right.getId());
		else
			return (diff < 0);
	}

	bool operator>(const HorarioAula& right) const
	{
		return (right < *this);
	}

	bool operator==(const HorarioAula& right) const
	{
		return (!(*this < right) && !(right < *this));
	}

	bool inicioFimIguais(HorarioAula const & right) const;
	bool inicioFimIguais(HorarioAula* const &outro) const;


private:
	DateTime inicio;
	Calendario * calendario;
	int tempo_aula;

	int turnoIES;

	int faseDoDia; // fase do dia
};

typedef GGroup< HorarioAula *, LessPtr< HorarioAula > > HorarioAulaGroup;

namespace std
{
	template<>
	struct less < HorarioAula* >
	{
		bool operator() (HorarioAula* const first, HorarioAula* const second) const
		{
			DateTime fstIni = first->getInicio();
			DateTime scdIni = second->getInicio();
			bool mesmoInicio = (fstIni.sameTime(scdIni));

			// Horários de início de aula diferentes
			if (!mesmoInicio)
				return (fstIni.earlierTime(scdIni));

			// Quando os horários_aula começam no mesmo
			// instante, comparamos o horário de final da aula
			int diff = first->getTempoAula() - second->getTempoAula();
			if (diff != 0)
				return (diff < 0);

			return (first->getId() < second->getId());
		}
	};
}

#endif