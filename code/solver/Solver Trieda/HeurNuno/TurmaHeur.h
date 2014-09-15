#ifndef _TURMA_HEUR_H_
#define _TURMA_HEUR_H_

#include <unordered_set>
#include <unordered_map>
#include <set>

using namespace std;

class SalaHeur;
class Calendario;
class ProfessorHeur;
class AulaHeur;
class AlunoHeur;
class TurmaPotencial;
class Demanda;
class AlunoDemanda;
class OfertaDisciplina;
class HorarioAula;
class Campus;
class Unidade;

class TurmaHeur
{
	friend class OfertaDisciplina;
public:
	// construtor com base em turma potencial
	TurmaHeur(const TurmaPotencial* &turmaPot, int turmaId);
	// construtor usado no carregamento de solução apartir da problem solution
	TurmaHeur(OfertaDisciplina* const oferta, bool teorico, int turmaId, SalaHeur* const sala, ProfessorHeur* const professor);

	virtual ~TurmaHeur(void);

	const int id;
	const bool tipoAula;		// false -> prática / true -> teórica
	const bool carregada;		// true -> foi carregada de uma solução fixada / false -> foi aberta pelo abridor
	OfertaDisciplina* const ofertaDisc;
	Calendario* getCalendario(void) const { return calendario; }
	void setCalendario(Calendario* const cal) { calendario = cal; }
	unsigned int getGlobalId(void) const { return globalId_; }
	static unsigned int getGlobalIdCount(void) { return globalIdCount_; }
	SalaHeur* const salaLoaded;
	bool discEhLab(void) const;

	// manter turma mesmo removendo da oferta
	bool keep (void) const { return keep_; }
	void setKeep (bool flag) { keep_ = flag; }
	// obrigar a abrir no mip
	bool mustAbrirMIP(void) const;
	void setMustAbrirMIP(bool flag) { mustAbrirMIP_ = flag; }

	// ID (unico dentro das turmas da OfertaDisciplina associada)
	bool ehCompSec(void) const;
	bool ehTeoricaTag(void) const;
	bool getAulaDia(int dia, AulaHeur* &aula) const;
	int nrDias (void) const { return aulas_.size(); }

	// professor
	void setProfessor(ProfessorHeur* const professor);
	ProfessorHeur* getProfessor() const { return professor_; }
	void removeProfessor();

	// sala, unidade
	void setSala(SalaHeur* const sala);
	SalaHeur* getSala() const { return sala_; }
	int unidadeId(void) const;
	void setNewAulas(unordered_map<int, AulaHeur*> &aulas);

	// add turma to prof e sala
	void addTurmaProfSala (void);

	// get aulas da turma
	void getAulas (unordered_map<int, AulaHeur*>& aulas) { aulas = aulas_; }

	// capacidade
	int getNrAlunos(void) const { return nrAlunos_; }
	int getNrAlunosFix(void) const { return alunosFixados_.size(); }
	int getCapacidadeRestante() const { return capacidadeRestante_; }
	double getOcupacao();
	int getNrCreditos() const;
	bool temAluno(int alunoId);
	bool temAluno(AlunoHeur* const aluno);
	int getNrFormados(void) const { return nrFormandos_; }
	int getNrCoRequisito(void) const { return nrCoRequisito_; }
	// verifica se ainda tem o numero de alunos abaixo do máximo para a disciplina
	bool podeTerMaisAlunos (void) const;
	// verifica se um aluno está fixado a esta turma
	bool ehFixado(int alunoId) { return (alunosFixados_.find(alunoId) != alunosFixados_.end()); }
	
	// alunos
	void getAlunos (unordered_set<AlunoHeur*> &alunos);
	void getDemandasAlunos (unordered_map<Demanda*, set<AlunoDemanda*>>& demandas);
	void getAlunosFix (unordered_set<int> &alunosFix) const { alunosFix = alunosFixados_; }

	// set/get nrDisp_
	void setNrDisp(int nr) { nrDisp_ = nr; }
	int getNrDisp(void) const { return nrDisp_; }
	void incNrDisp(void) { nrDisp_++; }
	void decNrDisp(void) { nrDisp_--; }

	// compatibilidade
	bool ehIncompativel(TurmaHeur* const outra, bool deslocavel) const;
	bool ehIncompativel(unordered_set<TurmaHeur*> const &outras, bool deslocavel) const;
	bool ehIncompativel(int dia, HorarioAula* const horario, bool deslocavel = false, int campId = -1, int unidId = -1) const;
	bool ehIncompativel(int dia, AulaHeur* const aula, bool deslocavel) const;

	// [Load Solução!] Add Aula
	void addAula(int dia, AulaHeur* const aula);

	// verificação
	bool alocacaoCompleta() const;			// verifica se já todas as aulas suficientes estão marcadas

	bool ehIlegal(double relaxMin = 1.0) const;					// verifica se tem o mínimo de alunos para formar uma turma

	void checkCreditos(void) const;

	// obter coeficiente de abertura de turma na FO com base no nr de alunos que mantém
	double getCoefMIPAbrir(void) const;

	static bool checkLink (TurmaHeur* const turma);

	virtual bool operator == (const TurmaHeur &other) const;

	virtual bool operator != (const TurmaHeur &other) const
	{
		return !(*this == other);
	}

	virtual bool operator < (const TurmaHeur &other) const;

private:
	ProfessorHeur* professor_;
	SalaHeur* sala_;
	Calendario* calendario;
	int nrAlunos_;
	unordered_map<int, AulaHeur*> aulas_;
	int capacidadeRestante_;
	int nrFormandos_;				// nr de alunos na turma que são formandos
	int nrCoRequisito_;				// nr de alunos na turma com co-requisito nesta oferta-disciplina

	// protege de ser deletada
	bool keep_;
	// obriga a abrir no MIP
	bool mustAbrirMIP_;

	// alunos fixados
	unordered_set<int> alunosFixados_;

	// Global ID
	const unsigned int globalId_;
	static unsigned int globalIdCount_;

	// add/remove alunos
	void addAluno(AlunoHeur* const aluno, bool fixado = false);
	void removeAluno(AlunoHeur* const aluno);

	// fechar a turma
	void fecharTurma_();

	// nr alunos disponiveis
	int nrDisp_;

	// nr formandos
	static bool temFormandos(unordered_set<AlunoHeur*> const &alunos);
};

// comparador tipo less que da prioridade a turmas ilegais e depois turmas com menos alunos disponiveis
struct compTurmaRealoc
{
	bool operator() (TurmaHeur* const first, TurmaHeur* const second)
	{
		bool fstIleg = first->ehIlegal();
		bool scdIleg = second->ehIlegal();
		if(fstIleg != scdIleg)
			return fstIleg;

		int diff = first->getNrDisp() - second->getNrDisp();
		if(diff != 0)
			return diff < 0;

		return first->id < second->id;
	}
};

// comparador tipo less que da prioridade a turmas com menor ocupação da sala, depois indice demanda da sala atual
struct compTurmaOcup
{
	bool operator() (TurmaHeur* const first, TurmaHeur* const second);
};

// comparador tipo less
struct compNrAlunos
{
	bool operator() (TurmaHeur* const first, TurmaHeur* const second);
};


#endif

