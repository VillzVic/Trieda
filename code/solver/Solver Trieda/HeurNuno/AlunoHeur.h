#ifndef _ALUNO_HEUR_H_
#define _ALUNO_HEUR_H_

//#include "UtilHeur.h"
#include <unordered_map>
#include "EntidadeAlocavel.h"
#include "../Aluno.h"
#include "../Disciplina.h"
#include "ParametrosHeuristica.h"

using namespace std;

class AlunoDemanda;
class Aula;
class ProblemData;
class OfertaDisciplina;
class HorarioDia;
class TurmaHeur;
class Calendario;
class AulaHeur;

class alunoHeurComp;

enum flagComp { MAIS_CRED, MENOS_CRED };

class AlunoHeur : public EntidadeAlocavel
{
public:
	AlunoHeur(Aluno* const aluno);
	~AlunoHeur(void);

	Aluno* getAluno() const { return aluno_; }
	int getId() const { return aluno_->getAlunoId(); }
	Curso* getCurso() const { return aluno_->getCurso(); }

	int getPriorAluno(void) const { return aluno_->getPriorAluno(); }

	AlunoDemanda* getDemandaOriginal(Campus* const campus, Disciplina* const disciplina, bool exc = true) const;
	AlunoDemanda* getDemandaOriginal(OfertaDisciplina* const oferta, bool exc = true) const;
	// nr de demandas P1
	int getNrDemsP1 (void) { return aluno_->getNrDemsP1(); }
	// nr de demandas P1 atendidas
	int getNrDemAtendP1 (void) { return nrDemAtendP1_; }
	// nr creditos P1 requeridos
	int getNrCredsReqP1(void) const { return aluno_->getNrCredsReqP1(); }

	// max creditos que podem ser alocados
	//int maxCredsAloc(void) const { return aluno_->getNrCredsReqP1(); }
	int maxCredsAloc(void) const { return aluno_->getNroCredsOrigRequeridosP1() + ParametrosHeuristica::maxCredsExcedentes; }

	// Controlo de ofertas às quais está associado
	void addOferta(OfertaDisciplina* const oferta);
	void removeOferta(OfertaDisciplina* const oferta);
	bool temTurmaOferta(OfertaDisciplina* const oferta) const;
	// get ofertas em que está incluido
	bool alocCompleto(void) const;

	// já alocado?
	bool jaAlocDemanda(AlunoDemanda* const &demanda) const;

	// procurar uma turma de uma disciplina ou equivalente e mesmo tipo
	void getTurmaTipoEquiv(OfertaDisciplina* const oferta, bool teorico, TurmaHeur* &turma) const;

	// excede max creditos ?
	bool excedeMaxCreds (int creds) const { return (getNrCreditosAlocados() + creds > maxCredsAloc()); }

	// Disponibilidade
	bool estaDisponivelHorarios(OfertaDisciplina* const oferta, int dia, AulaHeur* const aula) const;
	bool estaDisponivel(OfertaDisciplina* const oferta, int dia, AulaHeur* const aula) const ;
	bool estaDisponivel(OfertaDisciplina* const oferta, unordered_map<int, AulaHeur*> const &aulas) const;
	bool estaDisponivel(TurmaHeur* const turma) const;
	bool estaDisponivelAlguma(unordered_set<TurmaHeur*> const &turmas) const;
	// verifica se o aluno está disponivel para um novo conjunto de aulas desta turma
	bool estaDisponivelRealoc(TurmaHeur* const turma) const;

	// diz se tem nr de horários sequenciais disponiveis num dia para uma disciplina
	bool temHorsSeqDisponivel(int nrHors, OfertaDisciplina* const oferta, int dia);

	bool ehFormando(void) const { return aluno_->ehFormando(); }
	bool ehCalouro(void) const { return aluno_->ehCalouro(); }
	// é calouro ou inadimplente ?
	bool ehCalInad(void) const { return aluno_->ehCalouro(); }

	// co-requisitos
	void getCoRequisitos (vector<vector<AlunoDemanda*>> &coReq) const;
	bool temCoRequisito (OfertaDisciplina* const &oferta) const;
	bool temCoReqsIncompletos (void) const;
	void getOfertasCoReqsIncompletos (vector<OfertaDisciplina*> &ofertas) const;

	// imprimir demandas atendidas (disciplina id requerida + disciplina id da oferta)
	void printDemsAtendidas(void) const;

	// verificar links com as turmas
	bool checkLinks(void) const;

	// [COMPARADORES]

	// da prioridade a calouros, depois formandos, depois alunos com mais creditos
	static bool compAlunosI(AlunoHeur* const first, AlunoHeur* const second);
	// da prioridade a calouros, depois formandos, depois alunos com MENOS creditos
	static bool compAlunosII(AlunoHeur* const first, AlunoHeur* const second);

	// flag que é usada para mudar qual o comparador a usar
	enum flagComp { MAIS_CRED, MENOS_CRED };
	static flagComp flagCompAlunos;

	virtual bool operator == ( const AlunoHeur &outro ) const
	{
	   return ( aluno_->getAlunoId() == outro.getAluno()->getAlunoId() );
	} 

	virtual bool operator != ( const AlunoHeur &outro ) const
	{
	   return ( aluno_->getAlunoId() != outro.getAluno()->getAlunoId() );
	} 


private:
	Aluno* const aluno_;
	unordered_map<int, OfertaDisciplina*> demandasAtendidas_;	// aluno demanda id -> oferta.disciplina

	// nr demandas atend P1
	int nrDemAtendP1_;

	// [Comparadores singulares]

	// check formandos calouros
	static bool checkFormCal(AlunoHeur* const first, AlunoHeur* const second, bool &veredito);

	// da prioridade a alunos que demandam por turnos mais pequenos
	static bool checkTurnoDisc(AlunoHeur* const first, AlunoHeur* const second, bool &veredito);

	// da prioridade a demanda original sobre equivalente
	static bool checkDemOrig(AlunoHeur* const first, AlunoHeur* const second, bool &veredito);
};

// Override templates
namespace std
{
	template<>
	struct less<AlunoHeur*>
	{
		bool operator() (AlunoHeur* const first, AlunoHeur* const second) const
		{
			switch(AlunoHeur::flagCompAlunos)
			{
				case AlunoHeur::flagComp::MAIS_CRED:
					return AlunoHeur::compAlunosI(first, second);
				case AlunoHeur::flagComp::MENOS_CRED:
					return AlunoHeur::compAlunosII(first, second);
				default:
					return AlunoHeur::compAlunosI(first, second);
			}
		}
	};

	template<>
	struct equal_to<AlunoHeur*>
	{
		bool operator() (AlunoHeur* const first, AlunoHeur* const second) const
		{
			if(first == second)
				return true;

			return first->getId() == second->getId();
		}
	};

	template<>
	struct hash<AlunoHeur*>
	{
		size_t operator() (AlunoHeur* const aluno) const
		{
			return aluno->getId();
		}
	};
};



#endif