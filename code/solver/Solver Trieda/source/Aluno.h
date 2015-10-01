#ifndef _ALUNO_H_
#define _ALUNO_H_

#include <map>
#include <unordered_set>

#include "OFBase.h"
#include "Trio.h"
#include "DateTime.h"

class AlunoDemanda;
class Curso;
class Oferta;
class Calendario;
class HorarioDia;
class HorarioAula;
class Demanda;
class Disciplina;
class TurnoIES;

class Aluno
{
public:
	Aluno(void);
	Aluno(int id, std::string nome, bool formando, Oferta* oft);
	virtual ~Aluno(void);

	void setAlunoId(int value) { this->alunoId = value; }
	void setNomeAluno(std::string s) { this->nomeAluno = s; }
	void setFormando(bool value) { this->formando = value; }
	void setPrioridadeDoAluno(int value) { this->prioridadeDoAluno = value; }

	void setOferta(Oferta* o) { this->oferta = o; }	// TODO: isso perde o sentido assim que um aluno puder pedir por discs de diferentes ofertas
	void setOfertaId(int oftId) { this->ofertaId = oftId; } // TODO: isso perde o sentido assim que um aluno puder pedir por discs de diferentes ofertas
	void setCalouro(bool c) { this->calouro = c; }

	void setCombinaCredSL(int dia, int k, Calendario* sl, int n);
	void setCombinaCredSLSize(int dia, int size) { combinaCredSLSize[dia] = size; }
	void addCargaHorariaOrigRequeridaP1(int ch) { cargaHorariaOrigRequeridaP1 += ch; }
	void addNroCredsOrigRequeridosP1(int ch) { nroCredsOrigRequeridosP1 += ch; }

	void addNroDiscsOrigRequeridosP1(int addNroDiscs) { this->nroDiscsOrigRequeridosP1 += addNroDiscs; }
	void setNroCreditosJaAlocados(int nroCreds) { this->nCredsJaAlocados = nroCreds; }
	void addNroCreditosJaAlocados(int addNroCreds) { this->nCredsJaAlocados += addNroCreds; }


	void addHorarioDiaOcupado(HorarioDia* hd);
	void addHorarioDiaOcupado(GGroup<HorarioDia*> hds);
	void removeHorarioDiaOcupado(HorarioDia* hd);

	void clearHorariosDiaOcupados(){ horariosDiaOcupados.clear(); }

	Calendario* getCalendario(Demanda* demanda) const;

	// Retorna calendario associado a alguma das demandas do aluno.
	// Isso n�o � o ideal, mas atualmente n�o existe forma precisa de obter isso.
	Calendario* getCalendario() const;

	int getAlunoId() const { return alunoId; }
	std::string getNomeAluno() const { return nomeAluno; }

	Oferta* getOferta(Demanda* demanda) const;
	int getOfertaId(Demanda* demanda) const;

	bool hasCampusId(int cpId){
		if (campusIds.find(cpId) != campusIds.end())
			return true;
		else return false;
	}

	GGroup<int> getCampusIds() const { return campusIds; }

	/* preenche campusIds a partir dos AlunoDemandas */
	void fillCampusIds();

	// IMPLEMENTAR!!!!
	int getPriorAluno() { return prioridadeDoAluno; }

	Curso* getCurso() const;

	double getReceita(Disciplina* disciplina);

	int getNroDiscsOrigRequeridosP1() const { return this->nroDiscsOrigRequeridosP1; }
	int getNroCredsOrigRequeridosP1() const { return this->nroCredsOrigRequeridosP1; }
	int getCargaHorariaOrigRequeridaP1() const { return this->cargaHorariaOrigRequeridaP1; }
	int getNroCreditosJaAlocados() const { return this->nCredsJaAlocados; }
	bool totalmenteAtendido();
	int getNroCreditosNaoAtendidos();

	int getNroMaxCredCombinaSL(int k, Calendario *c, int dia);
	double getNroCreditosJaAlocados(Calendario* c, int dia);
	AlunoDemanda* getAlunoDemanda(int disciplinaId);
	AlunoDemanda* getAlunoDemandaEquiv(Disciplina* disciplina);
	std::map< int /*dia*/, int /*size*/ > getCombinaCredSLSize() const { return combinaCredSLSize; }
	void removeCombinaCredSL(Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t);
	std::map< int /*tuplaId*/, GGroup< int/*discId*/ > > getCorrequisitos() const { return ids_discs_correquisito; }

	GGroup< Calendario*, Less<Calendario*> > retornaSemanasLetivas();

	bool combinaCredSL_eh_dominado(int i, Calendario *sl1, int maxSL2, Calendario *sl2, int dia);
	bool combinaCredSL_domina(int i, Calendario *sl1, int j, Calendario *sl2, int dia);
	bool combinaCredSL_eh_repetido(int i, Calendario *sl1, int j, Calendario *sl2, int dia);
	std::map< Trio<int, int, Calendario*>, int > retornaCombinaCredSL_Dominados(int dia);

	std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >, int/*nroCreds*/ > combinaCredSL;

	bool demandaDisciplina(int idDisc);

	GGroup< AlunoDemanda*, Less<AlunoDemanda*> > demandas;
	// demandas P2. Povoadas no inicio da heuristica
	std::unordered_set<AlunoDemanda*> demandasP2;
	AlunoDemanda* getAlunoDemandaEquivP2(Disciplina *disciplina);

	bool sobrepoeHorarioDiaOcupado(HorarioDia *hd);
	bool sobrepoeHorarioDiaOcupado(HorarioAula *ha, int dia);
	bool sobrepoeAulaJaAlocada(HorarioAula *hi, HorarioAula *hf, int dia);

	bool ehFormando() const { return formando; }
	bool ehCalouro() const { return calouro; }

	void defineSeEhCalouro();

	std::string getHorariosOcupadosStr(int dia);

	// abordagem diferente (Heuristica)
	void addDemP1(void) { nrDemsP1++; }
	int getNrDemsP1(void) { return nrDemsP1; }
	void addNrCredsReqP1(int creds) { nrCredsReqP1 += creds; }
	int getNrCredsReqP1(void) { return nrCredsReqP1; }

	// Usado para tentar distribuir bem os cr�ditos do aluno (MIP-Escola)
	void setNrMedioCredsDia();
	int getNrMedioCredsDia();

	bool possuiEquivForcada();

	void setTurnoPrincipal();
	TurnoIES* getTurnoPrinc() const;
	bool estaEmContraTurno(Disciplina* const disciplina);
	bool ehContraTurno(TurnoIES* turno);

	bool get31Tempo(DateTime &dt);	// caso marreta 1

	bool chEhIgualDisponibSemanaLetiva(bool ignoraContraTurno = true);

	void le_arvore(ItemAluno & elem);

	bool operator<(const Aluno& right) const { return (alunoId < right.alunoId); }
	bool operator>(const Aluno& right) const { return (right < *this); }
	bool operator==(const Aluno& right) const { return !((*this < right) || (right < *this)); }

private:

	int alunoId;
	std::string nomeAluno;
	Oferta *oferta;
	int ofertaId;
	bool formando;
	bool calouro;
	int prioridadeDoAluno;

	// Media de creditos por dia, de acordo com a demanda do aluno.
	// Usado no MIP-Escola para tentar equilibrar a aloca��o do aluno na semana
	int nrCredsMedioDia_;

	std::map< int /*dia*/, int /*size*/ > combinaCredSLSize; // informa o numero total de combina��es existentes para cada dia

	// s� usado a partir de prioridade 2 para a primeira heuristica (modelo tatico sem horarios)
	std::map< Calendario*, std::map< int /*dia*/, double /*nCreds*/> > nCredsAlocados;

	std::map< int /*dia*/, std::map< DateTime, HorarioDia*> > horariosDiaOcupados;

	int nroDiscsOrigRequeridosP1;
	int nroCredsOrigRequeridosP1;
	int cargaHorariaOrigRequeridaP1;
	int nCredsJaAlocados;	// s� come�a ser preenchido ap�s t�tico (hor�rios/dias definidos)

	// diferente
	int nrCredsReqP1;
	int nrDemsP1;

	// tuplas de disciplinas com correquisito filtradas de acordo com a demanda do aluno
	std::map< int /*tuplaId*/, GGroup< int/*discId*/ > > ids_discs_correquisito;

	GGroup<int> campusIds;

	TurnoIES* turnoPrincipal_;
};

std::ostream & operator << (std::ostream &, Aluno &);

#endif