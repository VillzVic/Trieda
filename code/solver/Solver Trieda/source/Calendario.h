#ifndef _CALENDARIO_H_
#define _CALENDARIO_H_

#include "OFBase.h"

#include <unordered_set>

#include "GGroup.h"
#include "DateTime.h"
#include "TurnoIES.h"

class HorarioAula;

class Calendario : public OFBase
{
public:

	// Atributos ----------------------------------------------

	std::map< int /*id*/, HorarioAula* > mapProximoHorarioAula;

	GGroup< HorarioAula *, LessPtr< HorarioAula > > horarios_aula;

	std::map< int/*dia*/, std::map< DateTime, HorarioAula* > > mapDiaDateTime;

	std::map<int, GGroup<int>> mapTurnos; // <turno, dias>

	GGroup<TurnoIES*, LessPtr<TurnoIES>> turnosIES; // Cada turnoIES contem todos os horários do turnoIES, não somente os do calendario

	std::map< int/*dia*/, std::map< int/*faseDoDia*/, int/*duracaoInterv*/ > > mapDiaFaseSomaInterv;

	std::map< int/*dia*/, int/*duracaoInterv*/ > mapDiaSomaInterv;

	// Métodos ----------------------------------------------

	void addTurnoIES(TurnoIES *tt) { turnosIES.add(tt); }

	virtual void le_arvore(ItemCalendario &);

	void setCodigo(std::string value) { codigo = value; }
	void setDescricao(std::string value) { descricao = value; }
	void setTempoAula(int value) { tempo_aula = value; }

	std::string getCodigo() const { return codigo; }
	std::string getDescricao() const { return descricao; }
	int getTempoAula() const { return tempo_aula; }
	int getTurnoIES(DateTime dt);

	GGroup<int> getTurnos() {
		GGroup<int> t;
		std::map<int, GGroup<int>>::iterator itMap = mapTurnos.begin();
		for (; itMap != mapTurnos.end(); itMap++)
		{
			t.add(itMap->first);
		}
		return t;
	}

	int getNroDeHorariosAula(int dia);

	double getTempoTotal(int dia);

	int getMaxInterv(int dia, int fase);

	void calculaProximosHorarioAula();
	HorarioAula * getProximoHorario(HorarioAula *h) const;
	HorarioAula * getHorarioAnterior(HorarioAula *h);

	int retornaNroCreditosEntreHorarios(HorarioAula *hi, HorarioAula *hf);

	bool intervaloEntreHorarios(HorarioAula *hi, HorarioAula *hf);

	bool possuiHorario(HorarioAula *h);

	bool possuiHorarioDia(HorarioAula *h, int dia);
	HorarioAula* possuiHorarioDiaOuCorrespondente(int duracao, DateTime dti, int dia);
	HorarioAula* possuiHorarioDiaOuCorrespondente(HorarioAula *h, int dia);
	bool possuiHorarioDiaOuCorrespondente(HorarioAula *hi, HorarioAula *hf, int dia);

	GGroup<HorarioAula*, LessPtr<HorarioAula>> retornaHorariosDisponiveisNoDia(int dia);
	std::unordered_set<HorarioAula*> retornaHorariosDisponiveis(int dia, int turnoIdIES);

	HorarioAula * getHorarioMaisNCreds(HorarioAula *h, int nCreds);

	std::map< int, GGroup<HorarioAula*, Less<HorarioAula*>> > retornaDiaHorariosEmComum(GGroup<Calendario*, Less<Calendario*>> calendarios);

	GGroup<int> dias(int t);
	bool possuiTurno(int t, int dia);
	bool possuiTurno(int t);
	void addTurno(int t, int dia) { mapTurnos[t].add(dia); }
	void addTurno(int t, GGroup<int> dias) { mapTurnos[t].add(dias); }

	bool restringeAulaComIntervalo() const { return RESTRINGE_AULA_COM_INTERVALO; }

	void calculaDiaFaseSomaInterv();
	int getSomaInterv(int dia, int fase);

	void calculaDiaSomaInterv();
	int getSomaInterv(int dia);

private:
	std::string codigo;
	std::string descricao;
	int tempo_aula;

	std::map< std::pair<HorarioAula*, HorarioAula*>, int > horarios_nroCreds;

	std::map< int, std::map< int, int > > mapDiaFaseMaxInterv;

	bool RESTRINGE_AULA_COM_INTERVALO;
};

#endif