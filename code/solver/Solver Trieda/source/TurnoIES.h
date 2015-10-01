#ifndef _TURNO_H_
#define _TURNO_H_

#include "OFBase.h"

#include "GGroup.h"

#include "DateTime.h"

class Calendario;
class HorarioAula;

class TurnoIES : public OFBase
{
public:
	TurnoIES();

	void setNome(const std::string& s) { nome = s; }
	std::string getNome() const { return nome; }

	/* Dado um dia especifico, retorna o numero de horarios de aula disponiveis. */
	int getNroDeHorariosAula(int dia) const;

	/* retorna horarios disponiveis no dia */
	void retornaHorariosDisponiveisNoDia(int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>>& horarios) const;

	///* retorna horarios disponiveis no dia */
	//void retornaHorariosDisponiveisNoDia(int dia, std::map<DateTime, std::set<HorarioAula*>>* horarios) const;

	void addHorarioAula(HorarioAula* h);

	/* Procura o horarioDia nos horarios do turno, ou um igual exceto pelo id. */
	bool possuiHorarioDiaOuCorrespondente(HorarioAula *h, int dia) const;

	/* Procura o horarioDia nos horarios do turno */
	bool possuiHorarioDia(int dia, DateTime dti, DateTime dtf) const;

	/* Procura o horarioDia nos horarios do turno, ou um igual exceto pelo id. */
	GGroup<HorarioAula*, LessPtr<HorarioAula>> retornaHorarioDiaOuCorrespondente(HorarioAula *h, int dia) const;

	/* Verifica se todos os horarioDias entre hi e hf existem (iguais, exceto eventualmente pelo id) no turno. */
	bool possuiHorarioDiaOuCorrespondente(HorarioAula *hi, HorarioAula *hf, int dia);

	/* Procura o horarioDia no calendario, ou um igual exceto pelo id. */
	bool possuiHorarioDiaOuCorrespondente(Calendario* const calendario, DateTime dti, int dia);

	HorarioAula* getHorarioDiaOuCorrespondente(Calendario* const calendario, DateTime dti, int dia);

	/* Procura o horarioDia no calendario, ou um igual exceto pelo id. */
	HorarioAula* getHorarioDiaOuCorrespondente(Calendario* const calendario, HorarioAula* const h, int dia);

	bool get31Tempo(Calendario* const calendario, DateTime &dti) const;

	GGroup<HorarioAula*, LessPtr<HorarioAula>> horarios_aula;
	std::map<int/*dia*/, std::map<DateTime, std::set<HorarioAula*>>> mapDiaDateTime;

	/* numero de horarios na semana */
	int nrHorariosSemana;

private:
	std::string nome;
};

#endif
