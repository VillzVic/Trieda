#ifndef _SALA_H_
#define _SALA_H_

#include "OFBase.h"
#include "GGroup.h"
#include "Trio.h"
#include "DateTime.h"

class TipoSala;
class Horario;
class HorarioDia;
class CreditoDisponivel;
class Disciplina;
class ConjuntoSala;
class Calendario;
class HorarioAula;

class Sala : public OFBase
{
public:
	Sala() {}
	virtual ~Sala();

	GGroup<Horario*> horarios_disponiveis;

	GGroup<HorarioDia*> horariosDia;

	std::map<int, GGroup<HorarioDia*>> horariosDiaMap;

	std::map<int, std::map<DateTime, GGroup<DateTime>>> mapDiaDtiDtf;

	// Alterar para ref, depois
	GGroup<int> disciplinas_associadas;

	// Conjunto de disciplinas que foram associadas a sala em questão pelo usuario.
	GGroup<Disciplina*> disciplinas_Associadas_Usuario;

	// Conjunto de disciplinas que foram associadas a sala em questão
	// por um pré processamento realizado. Aqui, as associações são criadas
	// independentemente da unidade em que uma sala se encontra.
	GGroup<Disciplina*> disciplinas_Associadas_AUTOMATICA;

	// UNIAO dos conjuntos acima.
	GGroup<Disciplina*> disciplinasAssociadas;

	// Armazena os dias letivos para uma determinada sala
	GGroup<int> diasLetivos;
	TipoSala* tipo_sala;

	virtual void le_arvore(ItemSala &);
	int max_creds(int);
	int max_credsPorSL(int, Calendario*);

	// Métodos get
	int getTipoSalaId() const { return tipo_sala_id; }
	int getCapacidade() const { return capacidade; }
	int getIdUnidade() const { return id_unidade; }
	int getIdCampus() const { return id_campus; }
	std::string getCodigo() const { return codigo; }
	std::string getAndar() const { return andar; }
	std::string getNumero() const { return numero; }
	int getTempoDispPorDia(int dia);
	int getTempoDispPorDiaSL(int dia, Calendario* sl);
	int getNroCredCombinaSL(int k, Calendario*c, int dia);
	std::map<Trio<int, int, Calendario*>, int> getCombinaCredSL() const { return combinaCredSL; }
	std::map<int /*dia*/, int /*size*/> getCombinaCredSLSize() const { return combinaCredSLSize; }
	ConjuntoSala* getCjtSalaAssociado() const { return cjtSala; }
	const GGroup<HorarioDia*, Less<HorarioDia*>>& getHorariosDiaOcupados() const { return horariosDiaOcupados; }

	int getTempoMinSemanal(int turno);
	int getTempoMaxSemanal(int turno);

	bool ehLab() const;
	bool possuiHorariosNoDia(HorarioAula* hi, HorarioAula* hf, int dia);


	// Métodos set
	void setTipoSalaId(int id) { tipo_sala_id = id; }
	void setCapacidade(int cap) { capacidade = cap; }
	void setIdUnidade(int id) { id_unidade = id; }
	void setIdCampus(int id) { id_campus = id; }
	void setCodigo(const std::string& cod) { codigo = cod; }
	void setAndar(const std::string& a) { andar = a; }
	void setNumero(const std::string& n) { numero = n; }
	void setCombinaCredSL(int dia, int k, Calendario* sl, int n);
	void setCombinaCredSLSize(int dia, int size) { combinaCredSLSize[dia] = size; }
	void setCjtSalaAssociado(ConjuntoSala* cs){ cjtSala = cs; }

	// Alocações na sala
	void deleteHorarioDiaOcupados();
	void addHorarioDiaOcupado(HorarioDia* hd);
	void addHorarioDiaOcupado(GGroup<HorarioDia*> hds);
	bool sobrepoeHorarioDiaOcupado(HorarioAula* ha, int dia);
	bool sobrepoeAulaJaAlocada(HorarioAula* hi, HorarioAula* hf, int dia);
	bool ehViavelNaSala(Disciplina* disciplina, HorarioAula* hi, HorarioAula* hf, int dia);

	void calculaTempoDispPorTurno();

	void removeCombinaCredSL(Trio<int/*dia*/, int /*id*/, Calendario* /*sl*/> t);
	void construirCreditosHorarios(ItemSala &, const std::string&, bool);

	void calculaTempoDispPorDia();
	void calculaTempoDispPorDiaSL();
	int somaTempo(GGroup<HorarioDia*, Less<HorarioDia*>> horariosDia);
	int somaTempo(GGroup<HorarioAula*, Less<HorarioAula*>> horariosAula);
	GGroup<HorarioAula*> retornaHorariosDisponiveisNoDiaPorSL(int dia, Calendario* sl);

	int getMaxCredsPossiveis(int dia, Disciplina* disciplina);

	std::map<int, std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, Less<HorarioAula*>>>>>
		retornaHorariosAulaVagos();

	bool combinaCredSL_eh_dominado(int i, Calendario* sl1, int maxSL2, Calendario* sl2, int dia);
	bool combinaCredSL_domina(int i, Calendario* sl1, int j, Calendario* sl2, int dia);
	bool combinaCredSL_eh_repetido(int i, Calendario* sl1, int j, Calendario* sl2, int dia);
	std::map<Trio<int, int, Calendario*>, int> retornaCombinaCredSL_Dominados(int dia);

	virtual bool operator<(Sala & right) const { return (getId() < right.getId()); }
	virtual bool operator==(Sala & right) const { return (getId() == right.getId()); }

	// Associa a cada Calendario (sl) e tipo de combinação de creditos das semanas letivas,
	// o numero maximo de creditos possivel para cada dia da semana.
	std::map<Trio<int/*dia*/, int /*k_id*/, Calendario* /*sl*/>, int/*nroCreds*/> combinaCredSL;

private:

	// Dado um conjunto de horários, retorna o conjunto de créditos correspondentes
	GGroup<CreditoDisponivel*> converteHorariosParaCreditos();

	std::string codigo;
	std::string andar;
	std::string numero;

	// Alterar para ref, depois
	int tipo_sala_id;

	ConjuntoSala* cjtSala;

	int capacidade;
	int id_unidade;
	int id_campus;
	GGroup<Calendario*> temposSL; // lista os possiveis Calendarios existentes, obtido a partir dos horarios disponiveis da sala
	std::map<int, int> tempoDispPorDia; // mapeia para cada dia, o tempo total disponivel na sala, tratando intersecoes de semanas letivas
	std::map<std::pair<int, Calendario*>, int> tempoDispPorDiaSL; // mapeia para cada par <dia, Calendario>, o tempo disponivel na sala.

	std::map<int, std::pair<int, int>> tempoMinMaxDispPorTurno; // mapeia para cada turno o min e o max de tempo disponivel na sala na semana.

	void calculaTemposSL();
	void setTempoDispPorDia(int dia, int t){ tempoDispPorDia[dia] = t; }
	void setTempoDispPorDiaSL(int dia, Calendario* sl, int t){ tempoDispPorDiaSL[std::make_pair(dia, sl)] = t; }

	std::map<int /*dia*/, int /*size*/> combinaCredSLSize;

	GGroup<HorarioDia*, Less<HorarioDia*>> horariosDiaOcupados; // aulas alocadas. Preenchido após alocado por modelo tático / integrado.
};

#endif
