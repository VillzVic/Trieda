#ifndef _TURMA_POTENCIAL_H_
#define _TURMA_POTENCIAL_H_

//#include "UtilHeur.h"
//#include "AlunoHeur.h"
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "AlunoHeur.h"
#include "SalaHeur.h"

class OfertaDisciplina;
class ProfessorHeur;
class AulaHeur;
class Campus;
class Unidade;

class TurmaPotencial
{
public:
	TurmaPotencial(int turmaId, OfertaDisciplina* const oft, Calendario * const cal, bool tipo, unordered_map<int, AulaHeur*> aul,
			  ProfessorHeur* const prof, SalaHeur* const sal, set<AlunoHeur*> aluns, int alunDisp, bool algPotProf);
	TurmaPotencial(void);
	~TurmaPotencial(void);

	int id;
	OfertaDisciplina* ofertaDisc;
	Calendario* calendario;
	bool tipoAula;		// false -> prática / true -> teórica
	unordered_map<int, AulaHeur*> aulas;
	ProfessorHeur* professor;
	SalaHeur* sala;
	set<AlunoHeur*> alunos;
	bool algumPotencialProf;					// Se existe pelo menos um prof com disponibilidade de horário nesta data

	unsigned int getGlobalId(void) const { return globalId_; }
	double getValor(void) const { return valor_; }
	// 0 - prof real / 1 - prof virtual mas horarios disp / 2 - prof virtual e horarios nao disp
	int getTipoTurma(void) const;
	int getUnidadeId(void) const { return sala != nullptr ? sala->unidadeId() : -1; }
	int getCampusId(void) const { return sala != nullptr ? sala->campusId() : -1; }

	int nrAlunos (void) const { return alunos.size(); }

	bool usaSabado(void) const { return aulas.find(7) != aulas.end(); }

	bool ok;

	/* 
	 * Verifica se uma turma potencial domina outra. A dominância de A sobre B verifica-se quando ambas têm a mesma 
	 * divisão de créditos e:
     * 
	 * 1. Todas as aulas de A são dadas à mesma hora ou mais cedo que as aulas de B e o valor da turma é igual ou maior.
	 *
	 * OBJ: compactar grades tentando marcar aulas o mais cedo possível
	 */
	bool domina(const TurmaPotencial& outra) const;
	bool domina(const TurmaPotencial* const outra) const;

	// contar criações/destruições
	static int nrBuild;
	static int nrDestroy;

	virtual bool operator == (const TurmaPotencial &other) const
	{
		//return (id == other.id) && (tipoAula == other.tipoAula) && ((*ofertaDisc) == *(other.ofertaDisc));
		
		// só sao geradas turmas potenciais de um tipo de cada vez!
		return (id == other.id);
	}

	virtual bool operator != (const TurmaPotencial &other) const
	{
		return !(*this == other);;
	}

	virtual bool operator < (const TurmaPotencial &other) const
	{
		double diff = valor_ - other.getValor();
		if(std::abs(diff) > 0.00001)
			return diff > 0;

		return id < other.id;
	}

	// assignment operator
	virtual void operator= (const TurmaPotencial &other);

	// reset global id count
	static void resetGlobalIdCount(void) { globalIdCount_ = 0; }

private:

	double valor_;		// Valor atribuido à turma potencial que vai ser usado para escolher para comparar opções

	void calcularValor_(int alunosDisp);

	// Global ID
	const unsigned int globalId_;
	static unsigned int globalIdCount_;
};


// Comparador de pointers
namespace std
{
	template<>
	struct less<TurmaPotencial*>
	{
		bool operator() (TurmaPotencial* const first, TurmaPotencial* const second) const
		{
			double diff = first->getValor() - second->getValor();
			if(std::abs(diff) != 0)
				return diff > 0;

			return first->id < second->id;
		}
	};

	template<>
	struct less<const TurmaPotencial>
	{
		bool operator() (const TurmaPotencial &first, const TurmaPotencial &second) const
		{
			return first < second;
		}
	};

	template<>
	struct std::equal_to<const TurmaPotencial>
	{
		bool operator() (const TurmaPotencial &turma, const TurmaPotencial &other) const
		{
			return turma.id == other.id;
		}
	};

	template<>
	struct std::hash<const TurmaPotencial>
	{
		size_t operator() (const TurmaPotencial &turma) const
		{
			return turma.getGlobalId();
		}
	};

	/*template<>
	struct equal_to<TurmaPotencial*>
	{
		bool operator() (TurmaPotencial* const first, TurmaPotencial* const second)
		{
			return *first == *second;
		}
	};*/
};

#endif
