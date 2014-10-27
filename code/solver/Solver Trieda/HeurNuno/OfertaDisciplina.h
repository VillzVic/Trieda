#ifndef _OFERTA_DISCIPLINA_H_
#define _OFERTA_DISCIPLINA_H_

#include <unordered_set>
#include <unordered_map>
#include <set>
#include "../Disciplina.h"
#include "AlunoHeur.h"
#include "ProfessorHeur.h"
#include "SalaHeur.h"

class Calendario;
class Campus;
class TurmaHeur;
class HorarioAula;
class ProblemData;
class AlunoDemanda;
class DadosHeuristica;
class SolucaoHeur;
class ConjUnidades;
class Curso;
struct AtendFixacao;

class OfertaDisciplina
{
	friend class TurmaHeur;
public:
	OfertaDisciplina(SolucaoHeur* const solucao, Disciplina* const disciplinaTeorica, Disciplina* const disciplinaPratica, Campus* const campus);

	// com cursos associados!
	OfertaDisciplina(SolucaoHeur* const solucao, Disciplina* const disciplinaTeorica, Disciplina* const disciplinaPratica, Campus* const campus,
					set<Curso*> const &curs);

	virtual ~OfertaDisciplina(void);

	// cursos associados!
	const set<Curso*> cursos;

	int getNextTurmaId(bool teorica);
	int getLastTurmaId(void) { return turmaId_; }
	unsigned int getGlobalId(void) const { return (unsigned int)globalId_; }

	// flag indica se todas as componentes têm divisões registradas
	bool temDivs(void) const { return temDivs_; }
	// gera divisoes caso seja necessário
	void gerarDivs(void);

	// define número de demandas não atendidas (por prioridade) que podem ser satisfeitas por esta oferta
	// também actualiza os indicadores de demanda das salas (se activado)
	void setDemandasNaoAtend(unordered_map<int, unordered_set<AlunoDemanda *>> const &demandas, bool equiv = false);
	void setDemandasNaoAtend(unordered_map<int, int> const &demandasPrior, bool equiv = false);
	// get nrDemandas nao atendidas para uma prioridade
	int getNrDemandasNaoAtend(int prioridade, bool equiv = false) const;
	// retorna o valor para comparação entre ofertas disciplina
	double valorDemandasNaoAtendP1Equiv(void) const;
	// só considera prioridade 1 (s/ equiv) e coloca peso no credito de alunos.
	double valorDemandasNaoAtendV2(int prioridade = 1) const;
	// apaga demandas nao atend
	void clearDemandasNaoAtend(void) { nrDemandasNaoAtend_.clear(); }

	Disciplina* getDisciplina (void) const;
	Disciplina* getDisciplina(bool teorico) const 
	{ 
		if(teorico)
			return disciplinaTeorica_;
		else
			return disciplinaPratica_;
	}
	Campus* getCampus() const { return campus_; }
	void getAlunos(unordered_set<AlunoHeur*>& alunos) const;

	// dando flag compSec retorna se é teorico ou pratico
	bool getTeoricoCompSec(bool compSec) const;
	// from flag get tipo turma
	bool tipoTurmaFromTag(bool tag) const;

	// verificar se oferta disciplina tem demanda suficiente
	bool podeAbrir (void);

	// abrir turma potencial
	TurmaHeur* abrirTurma(const TurmaPotencial* &turmaPot);
	// abrir turma com um determinado id e numa determinada sala [Load solução].
	// OBS: sala e professor já carregados mas associados à turma no fim do carregamento da solução
	TurmaHeur* abrirTurma(bool teorico, int id, SalaHeur* const &sala, ProfessorHeur* const &professor, const AtendFixacao &fixacoes);
	// registrar abertura de turma
	void regAberturaTurma(TurmaHeur* const turma);
	// fechar turma
	void fecharTurma(TurmaHeur* &turma, bool remAssoc = true);
	// fechar all turmas
	void fecharAllTurmas(bool remAssoc = true);

	// vai removendo incompletos e fechando turmas até não haver mais nada para remover
	bool acertarOferta(double relaxMin = 1.0, bool fixados = false);
	// relaxando o mínimo
	bool fecharTurmasInv(double relaxMin, bool fixadas = false);

	// get maximo de alunos por turma num tipo de aula
	int getMaxAlunos (bool teorico) const { return maxAlunosTipoAula.at(teorico); }

	// adicionar aluno à turma
	void addAlunoTurma(AlunoHeur* const aluno, TurmaHeur* const turma, string metodo = "?",bool fix = false);
	// remove aluno da turma. Só usar quando se remove aluno de turma única. Se for para remover da oferta toda usar "removeAluno"
	// @fixado: define se queremos remover o aluno mesmo se for fixado
	void removeAlunoTurma(AlunoHeur* const aluno, TurmaHeur* const turma, bool fixado = false, bool removeAlunoTot = false);
	// remove o aluno da oferta disciplina e todas as turmas e estruturas
	// @fixado: define se queremos remover o aluno mesmo se for fixado
	void removeAluno(AlunoHeur* const aluno, bool fixado = false);	


	// verifica se o aluno está alocado na oferta
	bool temAluno(AlunoHeur* const aluno) const { return alunosTurma_.find(aluno) != alunosTurma_.end(); }
	// verifica se o aluno está alocado a alguma turma desse tipo
	bool temAlunoComp(AlunoHeur* const aluno, bool teorico) const;

	// tem aluno de um curso na turma
	bool temAlunoCurso(Curso* const curso, TurmaHeur* const turma) const;

	bool temAlunosIncompleto(void) { return alunosIncompleto_.size() > 0; }

	bool temComp(bool teorico) const { if(teorico) { return disciplinaTeorica_ != nullptr; } else { return disciplinaPratica_ != nullptr; }}
	bool temCompTeorica() const { return disciplinaTeorica_ != nullptr; }
	bool temCompPratica() const { return disciplinaPratica_ != nullptr; }

	bool disciplinaTeoricaDisponivel(int dia, HorarioAula* horario) const  
	{ 
		return disciplinaTeorica_->possuiHorarioDiaOuAnalogo(dia, horario); 
	}
	bool disciplinaPraticaDisponivel(int dia, HorarioAula* horario) const 
	{ 
		return disciplinaPratica_->possuiHorarioDiaOuAnalogo(dia, horario); 
	}

	// get salas associadas
	unordered_map<bool, unordered_set<SalaHeur*>> salasAssoc_;
	void preencheSalasAssoc();
	void preencheSalasAssoc(bool teorico);
	void getSalasAssociadas (unordered_set<SalaHeur*> &set, bool teorico) const;	// retorna todas as salas em que uc pode ser dada, teoricas ou praticas
	void getSalasAssociadas (set<SalaHeur*> &conj, bool teorico) const;

	// get profs associados
	void getProfessoresAssociados (unordered_set<ProfessorHeur*> &profsAssoc) const;
		
    // Horarios-Dias comuns com cada professor real habilitado na OfertaDisciplina.
	int getNroCredsLivresProfsEstimados() const;
	std::unordered_map< ProfessorHeur*,
		std::map< int/*dia*/,	std::map<DateTime, std::set<DateTime>> > > mapProfDiaHorComum_;
    void preencheMapDisponibComumProf();
	int getTempoComumProf( ProfessorHeur* const prof,
		std::map< int/*dia*/, std::map<DateTime, std::set<DateTime>> > &mapDiaHorComum ) const;

	int getDisponibProfDisc() const;
	int getNroCredsLivresProf( ProfessorHeur* const prof, 
			std::map< int/*dia*/, std::map<DateTime, 
				std::set<DateTime>> > const * const & mapDiaHorComum ) const;
	// teste com sala
	int getDisponibProfSalaDisc() const;
	int getDisponibProfSalaDisc( unordered_set<SalaHeur*> const &salas, bool teorico ) const;
	int getNroCredsLivresProfSala( ProfessorHeur* const prof, SalaHeur* const sala,
			std::map< int/*dia*/, std::map<DateTime, 
				std::set<DateTime>> > const * const & mapDiaHorComum ) const;
	

	// verifica se a unidade tem algum tipo de sala associada desse tipo no cluster de unidades
	bool algumaSalaCluster(const ConjUnidades* const cluster, bool teorico) const;

	int getNrCreds(void) const;
	int getNrCreds(bool teorico) const;
	int nrLabsAssoc(void) const { return nrLabsAssoc_; }

	int nrAlunosAloc(void) const { return alunosTurma_.size(); }
	int nrAlunosCompleto(void) const;
	int nrAlunosIncompleto(void) const { return alunosIncompleto_.size(); }

	// retorna todos os alunos que estão alocados numa componente mas não noutra, e.g. tão numa turma teórica mas não numa prática
	unordered_set<AlunoHeur*> getAlunosIncompleto(void) { return alunosIncompleto_; }
	void getAlunosIncompleto(unordered_set<AlunoHeur*>& alunosInc) const;
	// @fixados: define se queremos remover os alunos mesmo se forem fixados
	void removerAlunosIncompleto(unordered_set<AlunoHeur*> &alunosRemovidos, bool fixados = false);
	void removerAlunosIncompleto(bool fixados = false);
	void removerTodosAlunos(unordered_set<AlunoHeur*> &alunosRemovidos, bool fixados = false);
	void removerTodosAlunos(bool teorico, unordered_set<AlunoHeur*> &alunosRemovidos, bool fixados = false);

	// verifica se está fixado a alguma das turmas a que está alocado
	bool ehFixado(AlunoHeur* const aluno) const;
	bool ehIncompleto(AlunoHeur* const aluno) const { return (alunosIncompleto_.find(aluno) != alunosIncompleto_.end()); }

	// manter alunos completos atuais. não remover para o MIP.
	void setKeepAlunos(void);
	bool keepAluno (int alunoId) const { return keepAlunos_.find(alunoId) != keepAlunos_.end();}

	// preenche mapa com a alocacao atual
	void getAlocacaoAtual(unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>> &alocOft) const;
	void getAlocacaoAtual(unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>>* const alocOft) const;

	// get turmas
	void getTurmas(unordered_set<TurmaHeur*>& turmas);
	unordered_map<int, TurmaHeur*> getTurmasTipo(bool teorico) const;
	void getTurmasTipo(bool teorico, unordered_set<TurmaHeur*> &turmas) const;
	unordered_map<int, TurmaHeur*> getTurmasPrincipal(void) const
	{
		if(temCompTeorica())
			return getTurmasTipo(true);
		else
			return getTurmasTipo(false);
	}
	
	// procura turma por id e tipo
	TurmaHeur* getTurma(bool teorico, int id) const;

	// retorna as turmas a que o aluno está alocado
	void getTurmasAluno(AlunoHeur* const aluno, vector<TurmaHeur*> &turmas) const;
	// get todas as associações turma aluno
	void getTurmasSalasAlunos(unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*> > > &turmasSalasAlunos) const;
	// get turmas alunos
	void getTurmasAlunos(unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>> &turmasAlunos) const { turmasAlunos = turmasAlunos_; }

	template<typename T>
	void getTurmasOrd(set<TurmaHeur*, T>& turmasOrd) const
	{
		for(auto itTeor = turmas_.cbegin(); itTeor != turmas_.cend(); itTeor++)
		{
			for(auto itId = itTeor->second.cbegin(); itId != itTeor->second.cend(); itId++)
			{
				turmasOrd.insert(itId->second);
			}
		}
	}

	// verifica se tem turmas ilegais
	bool temTurmasIlegais(void) const;

	// retorna turmas a qual esta turma está associada
	void getTurmasAssoc(TurmaHeur* const turma, unordered_set<TurmaHeur*> &turmasAssoc) const;
	void getAllTurmasAssoc(unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> &turmasAssoc) const { turmasAssoc = turmasAssoc_; }
	bool temAssoc(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois) const;
	bool temAnyAssoc(TurmaHeur* const turma) const;
	// limpar associações de turmas (exceto as que must abrir no MIP)
	void limparTurmasAssocPreMIP(void);

	int nrTurmas(bool teorico) const;
	// retorna o numero de turmas (
	int nrTurmasVirt(bool soProfVirtual) const;
	bool temTurmaTipo(bool teorico) const;
	// retorna a aula do tipo a que o aluno está alocado. Se não encontrar retorna false;
	bool getTurmaAlunoTipo(AlunoHeur* const aluno, bool teorico, TurmaHeur* &turma) const;
	// verifica se tem esta turma registada
	bool temTurmaReg (unsigned int id) const { return (turmasGlobalIds_.find(id) != turmasGlobalIds_.end()); }

	int nrTiposAula() const;	
	bool duasComp(void) const { return (nrTiposAula() == 2); }
	int nrDivisoes(bool teorico) const;

	// print numero de combinacoes previstas
	void printNrCombPrev(void);

	// numero de combinacoes previstas para analise na abertura de turmas de um tipo
	long nrCombinacoesPrev(bool teorico, bool porTurno);

	// increase stat prof real
	void incProfReal(int val);

	// verificar se a estrutura está consistente
	bool checkConsistencia(void);
	bool checkCompletos(void);
	// [1x1 ou 1xN] verificar se 1x1 ou 1xN é satisfeito
	bool checkRelacTeoricasPraticas(void) const;
	bool checkRelacTipo(bool teorico) const;
	// verificar se todos os alunos da turma prática estão na mesma disciplina teórica
	// obs: só 1x1 se verifica as teoricas
	bool checkRelacTurma(TurmaHeur* const turma) const;

	// escrever alocacoes para ficheiro text
	void exportarAlocs(std::ofstream &file) const;

	// print info
	void printInfo(void) const;

	// [COMPARADORES]

	// valor demandas nao atend >> nr creds >> nr tipos de aula >> nr labs
	static bool compOfDisc(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade);
	// nr creds >> nr tipos de aula >> nr creditos praticos >> valor demandas nao atend >> nr labs
	static bool compOfDiscII(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade);
	// tem pratico >> nr labs >> nr creditos praticos >> nr tipos de aula >> nr creds >> valor demandas nao atend
	static bool compOfDiscIII(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade);
	// TESTE: nr demandas não atendidas >> nr creditos
	static bool compOfDiscIV(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade = 1);
	// nr demandas nao atendidas p1 >> nr creds >> prof real disponiv >> operator<
	static bool compOfDiscV(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade = 1);
	// TESTE: nr demandas não atendidas / profs reais >> nr demandas nao atendidas
	static bool compOfDiscVI(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade = 1);

	virtual bool operator == ( const OfertaDisciplina &outra) const
	{
		//return getDisciplina()->getId() == outra.getDisciplina()->getId() && campus_->getId() == outra.getCampus()->getId();
		return globalId_ == outra.globalId_;
	}

	virtual bool operator != ( const OfertaDisciplina &outra) const
	{
		return !(*this == outra);
	}

	// Ordena ofertas disciplina por ordem decrescente de valor de demandas não atendidas
	virtual bool operator < ( const OfertaDisciplina &outra) const
	{
		// valor primeiro dado aos créditos aluno não atendidos!
		double diff = this->valorDemandasNaoAtendV2() - outra.valorDemandasNaoAtendV2();
		if(std::abs(diff) > 0.0001)
			return this->valorDemandasNaoAtendV2() > outra.valorDemandasNaoAtendV2();

		// dar prioridade a disciplinas com mais creditos
		int difCred = this->getNrCreds() - outra.getNrCreds();
		if(difCred != 0)
			return this->getNrCreds() > outra.getNrCreds();

		// nr de tipos de aula
		int frstTipos = this->nrTiposAula();
		int scdTipos = outra.nrTiposAula();
		// dar prioridade a ofertas com duas componentes
		if(frstTipos != scdTipos)
			return frstTipos > scdTipos;

		// prioridade a ofertas com 2 tipos de aula que tenham mais carga pratica
		if(frstTipos == 2)
		{
			int credsPrat = this->getNrCreds(false);
			int credsPratOutro = outra.getNrCreds(false);
			if(credsPrat != credsPratOutro)
				return credsPrat > credsPratOutro;
		}

		bool pratUm = this->temCompPratica();
		bool pratDois = outra.temCompPratica();
		if(pratUm != pratDois)
			return pratUm;

		/*int frstTipos = this->nrTiposAula();
		int scdTipos = outra.nrTiposAula();
		// dar prioridade a ofertas com duas componentes
		if(frstTipos != scdTipos)
			return frstTipos > scdTipos;*/

		// dar prioridade às disciplinas com menos profs disponiveis
		//diff = this->getDisponibProfDisc() - outra.getDisponibProfDisc();
		//if(diff != 0)
		//	return diff < 0;		
		
		// Estimativa! cálculo mais rapido.
		// dar prioridade às disciplinas com menos profs disponiveis
		diff = this->getNroCredsLivresProfsEstimados() - outra.getNroCredsLivresProfsEstimados();
		if(diff != 0)
			return diff < 0;

		return (globalId_ < outra.getGlobalId());
		//return false;
	}

private:
	SolucaoHeur* const solucao_;
	Disciplina* const disciplinaTeorica_;
	Disciplina* const disciplinaPratica_;
	Campus* const campus_;

	// flags que indicam se tem todos os dados de input necessarios para abrir
	bool temDivs_;
	bool temSalas_;

	// nr laboratorios associados
	int nrLabsAssoc_;
	void setLabsAssoc(void);

	// maximo de alunos
	unordered_map<bool, int> maxAlunosTipoAula;

	// manter alunos MIP
	unordered_set<int> keepAlunos_;

	// id turmas
	int turmaId_;

	// Global ID
	const size_t globalId_;
	static size_t globalIdCount_;
	
	unordered_map<AlunoHeur*, unordered_map<bool, TurmaHeur*>> alunosTurma_;		// aluno.id -> tipoAula (true: teorico, false: pratico) -> turma
	unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>> turmasAlunos_;
	unordered_set<AlunoHeur*> alunosIncompleto_;					// ex: alunos que tão alocados numa turma teórica mas não na prática
	
	//unordered_set<int> alunosEquivalente_;				// Alunos alocados nesta oferta-disciplina mas que na verdade querem outra
	unordered_map<bool, unordered_map<int, TurmaHeur*>> turmas_;			// teorico? -> turma.Id -> turma
	unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> turmasAssoc_;		// associação entre turmas teoricas e praticas;
	unordered_set<unsigned int> turmasGlobalIds_;

	// nr demandas ainda não satisfeitas que podem sê-lo por esta oferta
	unordered_map<int, int> nrDemandasNaoAtend_;

	// verificar se o numero de créditos da turma bate com o número de créditos da componente.  lança excepção caso contrario
	void checkCredsTurma(TurmaHeur* const turma, std::string const &method) const;

	// add to estrutura alunosTurma_; return true se completo
	bool addToAlunosTurma_(AlunoHeur* const aluno, TurmaHeur* const turma, string metodo);
	// remove da estrutura alunosTurma_;
	void removeFromAlunosTurma_(AlunoHeur* const aluno, TurmaHeur* const turma);
	// verificar se o aluno tem demanda equivalente
	void addToTurmasAluno_(AlunoHeur* const aluno, TurmaHeur* const turma);
	// remover from turmas aluno
	void removeFromTurmasAluno_(AlunoHeur* const aluno, TurmaHeur* const turma);

	// faz associação de turmas e verifica se é violada a regra de associação de turmas
	void addAlunoCheckAssocTurmas_(AlunoHeur* const aluno, TurmaHeur* const turma);
	// faz associação de turmas e verifica se é violada a regra de associação de turmas
	void addCheckAssocTurmas_(TurmaHeur* const teorica, TurmaHeur* const pratica);
	// remover associacoes a uma turma
	void removeAssocTurma_(TurmaHeur* const turma);

	// increase/decrease indicadores demanda de salas associadas!
	// increase: true -> increase; false -> decrease
	void decIncIndicDem_(bool increase, bool equiv) const;
	void decIncIndicDemComp_(bool increase, bool teorico, bool equiv) const;
		
	unordered_set<SalaHeur*> const * getSalasAssocRef(bool teorico) const;

	// Comparadores singulares
	static bool compDemNaoAtendV2(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade, bool &veredito);
};

std::ostringstream& operator<< ( std::ostringstream &out, OfertaDisciplina const &ofDisc );

// compara ofertas disciplina com base na demandas demandas de prioridade 1 (equivalente e nao)
struct compOftDisc
{
	bool operator()(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois) const
    {   
		//return OfertaDisciplina::compOfDiscII(ofertaUm, ofertaDois, 0);  

		//return OfertaDisciplina::compOfDiscIV(ofertaUm, ofertaDois);  
		return OfertaDisciplina::compOfDiscV(ofertaUm, ofertaDois); 
		//return OfertaDisciplina::compOfDiscVI(ofertaUm, ofertaDois);   
    }   
};

// compara ofertas disciplina com base na demanda equivalente de prioridade 1 não atendida
struct compOftDiscEquiv
{
	bool operator()(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois) const
    {    
		//return OfertaDisciplina::compOfDisc(ofertaUm, ofertaDois, -1);
		return OfertaDisciplina::compOfDiscII(ofertaUm, ofertaDois, -1);
		//return OfertaDisciplina::compOfDiscIII(ofertaUm, ofertaDois, -1);
    }   
};

// ordena por ordem crescente de numero de demandas
struct invNrDemandas
{
	bool operator()(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois) const
    {   
		int nrUm = ofertaUm->getNrDemandasNaoAtend(1) + ofertaUm->getNrDemandasNaoAtend(-1);
		int nrDois = ofertaDois->getNrDemandasNaoAtend(1) + ofertaDois->getNrDemandasNaoAtend(-1);
		if(nrUm != nrDois)
			return nrUm < nrDois;

		return ofertaUm < ofertaDois;
    }   
};

// ordena ofertas por ordem decrescente de demanda P2
struct compOftDiscP2
{
	bool operator()(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois) const
    {   
		//return OfertaDisciplina::compOfDiscII(ofertaUm, ofertaDois, 0);  

		//return OfertaDisciplina::compOfDiscIV(ofertaUm, ofertaDois, 2);  
		return OfertaDisciplina::compOfDiscV(ofertaUm, ofertaDois,2);
		//return OfertaDisciplina::compOfDiscVI(ofertaUm, ofertaDois, 2);   
    }   
};

// Comparador de pointers
namespace std
{
	template<>
	struct less<OfertaDisciplina*>
	{
		bool operator() (OfertaDisciplina* const first, OfertaDisciplina* const second)
		{
			//return OfertaDisciplina::compOfDisc(first, second, 1);
			//return OfertaDisciplina::compOfDiscII(first, second, 1);
			//return OfertaDisciplina::compOfDiscIII(first, second, 1);
			//return OfertaDisciplina::compOfDiscIV(first, second);
			return OfertaDisciplina::compOfDiscV(first, second);
			//return OfertaDisciplina::compOfDiscVI(first, second);   
		}
	};

	/*
	template<>
	struct equal_to<OfertaDisciplina*>
	{
		bool operator() (OfertaDisciplina* const first, OfertaDisciplina* const second) const
		{
			return (first->getGlobalId() == second->getGlobalId());
		}
	};

	template<>
	struct hash<OfertaDisciplina*>
	{
		size_t operator() (OfertaDisciplina* const oft) const
		{
			return oft->getGlobalId();
		}
	};
	*/
};

#endif