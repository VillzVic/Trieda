#ifndef _PROFESSOR_HEUR_H_
#define _PROFESSOR_HEUR_H_

#include "../Professor.h"
#include "EntidadeAlocavel.h"

class Calendario;
class TipoTitulacao;

class ProfessorHeur :
	public EntidadeAlocavel
{
public:
	ProfessorHeur(void);
	ProfessorHeur(Professor* const professor);
	~ProfessorHeur(void);

	Professor* getProfessor() const { return professor_; }
	bool ehVirtualUnico() const { return ehVirtualUnico_; }
	bool ehVirtual() const { return professor_->eVirtual(); }
	int getId() const { return professor_->getId(); }
	TipoTitulacao* tipoTitulacao() const { return professor_->titulacao; }
	TipoContrato* tipoContrato() const { return professor_->tipo_contrato; }
	double getValorCredito() const { return professor_->getValorCredito(); }

	// disponibilidade
	bool estaDisponivelHorarios(int dia, AulaHeur* const aula) const;	
	bool estaDisponivelHorarios(unordered_map<int, AulaHeur*> const &aulas) const;
	bool estaDisponivel(int dia, AulaHeur* const aula) const;
	bool estaDisponivel(unordered_map<int, AulaHeur*> const &aulas) const;

	int nrCreditosAlocado(void) const;

	// verificar links com as turmas
	bool checkLinks(void) const;

	// verifica se duas turmas violam a interjornada
	static bool violaInterjornada(TurmaHeur* const turma1, TurmaHeur* const turma2);
	// verifica se duas aulas violam a interjornada
	static bool violaInterjornada(AulaHeur* const aula1, int dia1, AulaHeur* const aula2, int dia2);
	// verifica se dois horarios violam a interjornada
	static bool violaInterjornada(HorarioAula* const hor1, int dia1, HorarioAula* const hor2, int dia2);

	virtual bool operator == ( const ProfessorHeur &outro) const
	{
		return getId() == outro.getId();
	}

	virtual bool operator != ( const ProfessorHeur &outro) const
	{
		return !(*this == outro);
	}

	// Ordena professores por número de créditos alocados
	virtual bool operator < ( const ProfessorHeur &outro) const
	{
		return getId() < outro.getId();
	}

private:
	Professor* const professor_;
	bool ehVirtualUnico_;

	/*
	unordered_map<int, GGroup< HorarioAula *, LessPtr<HorarioAula>>> horariosDisponivel_;
	unordered_map<Calendario*, std::map<int, std::vector<HorarioAula*>>> horariosDisponivelCalendario_;

	void addHorariosDisponivel();
	unordered_map<int, std::vector<HorarioAula*>> getHorariosLivresCalendario (const Calendario &Calendario);*/
};

// Comparador de IDs

// compara ofertas disciplina com base na demandas demandas de prioridade 1 (equivalente e nao)
struct cmpProfIds
{
	bool operator()(ProfessorHeur* const profUm, ProfessorHeur* const profDois) const
    {   
		int idUm = profUm->getId();
		bool umPos = (idUm >= 0);
		int idDois = profDois->getId();
		bool doisPos = (idDois >= 0);

		if(!umPos && !doisPos)
			return idUm > idDois;
		else if(umPos != doisPos)	// positivo vem antes
			return umPos;

		return idUm < idDois;
    }   
};

// Comparador de pointers
namespace std
{
	template<>
	struct less<ProfessorHeur*>
	{
		bool operator() (ProfessorHeur* const first, ProfessorHeur* const second)
		{
			int credsFst = first->getNrCreditosAlocados();
			int credsScd = second->getNrCreditosAlocados();

			// já atingiu o nr creditos alocados semestre anterior
			bool jaFirst = (credsFst >= first->getProfessor()->getChAnterior());
			bool jaScd = (credsScd >= second->getProfessor()->getChAnterior());
			if(jaFirst != jaScd)
				return !jaFirst;

			// menor custo
			double diffCost = first->getValorCredito() - second->getValorCredito();
			if(diffCost != 0.0)
				return diffCost < 0;

			// menos magisterios first
			int diffMag = first->getProfessor()->magisterio.size() - second->getProfessor()->magisterio.size();
			if(diffMag != 0)
				return diffMag < 0;

			// menos creditos alocados first
			int diff = credsFst - credsScd;
			if(diff != 0)
				return diff < 0;

			// menos disponibilidade
			int diffDisp = first->getProfessor()->horariosDia.size() - second->getProfessor()->horariosDia.size();
			if(diff != 0)
				return diff < 0;

			return first < second;
		}
	};

	template<>
	struct equal_to<ProfessorHeur*>
	{
		bool operator() (ProfessorHeur* const first, ProfessorHeur* const second) const
		{
			return first->getId() == second->getId();
		}
	};

	template<>
	struct hash<ProfessorHeur*>
	{
		size_t operator() (ProfessorHeur* const prof) const
		{
			return prof->getId();
		}
	};
};

// Perfil Professor Virtual

class PerfilVirtual
{
public:
	
	// construtor
	PerfilVirtual(int tit, int con, Curso* cur) 
		: titulacao(tit), contrato(con), curso(cur) { } 

	const int titulacao;
	const int contrato;
	Curso* const curso;

	virtual bool operator ==(PerfilVirtual const &other) const 
	{ 
		return (titulacao == other.titulacao) && (contrato == other.contrato) && (curso == other.curso); 
	}
};

#endif