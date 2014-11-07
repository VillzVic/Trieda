#ifndef _ABRIDOR_TURMAS_H_
#define _ABRIDOR_TURMAS_H_

#include <unordered_map>
#include <unordered_set>
#include <set>
#include "ProfessorHeur.h"
#include "SalaHeur.h"
#include "TurmaPotencial.h"
#include "OfertaDisciplina.h"

using namespace std;

class SolucaoHeur;
class TurmaPotencial;
class ImproveMethods;
class RecursivePreProc;

// Turmas Potenciais indexadas por tipo
typedef unordered_map<int, set<const TurmaPotencial*>> turmasPotOrd;

// usado no preprocessamento
typedef pair< unordered_set<AlunoHeur*>, unordered_set<SalaHeur*> > parAlunosSalaDisp;
typedef unordered_map<int, unordered_map<int, unordered_map<AulaHeur*, parAlunosSalaDisp> > > mapDispDiaCredAula;

// comparador das ofertas disciplina
class AbridorTurmas
{
public:
	AbridorTurmas(SolucaoHeur* const solucao, bool equiv, bool usarVirtual, bool realoc, bool anySize, int priorAluno = 0, 
					double relaxLim = 1.0);
	~AbridorTurmas(void);

	// abrir turmas [ir actualizando a ordem das ofertas e abrir uma turma principal de cada vez]
	template<typename T>
	void abrirTurmas(set<OfertaDisciplina*, T> &setOrd);

	// abrir turmas [ordem randomica]
	void abrirTurmasRandom(vector<OfertaDisciplina*> &setOfts);

	// TESTE (para comparar alunos)
	static OfertaDisciplina* currOferta;

	// set blacklist de alunos
	void setBlacklist(unordered_map<OfertaDisciplina*, unordered_set<int>> const &black) { blacklist_ = black; }

private:
	SolucaoHeur* const solucao_;	// solução a ser trabalhada
	const bool equiv_;				// considerar demandas equivalentes
	double relaxMin_;			// percentagem de relaxamento do mínimo
	const bool abrirVirtual;		// abrir turmas com virtual ?
	const bool realocar;			// realocar alunos ?
	const bool anySize_;		// turmas com formando podem ter qualquer tamanho?
	const int priorAluno_;			// prioridade maxima de aluno que será considerada

	// stats
	int nrTurmas_;


	// alunos incompletos removidos são incluidos aqui. se eles já estiverem nos removidos, entao sao adicionados à blacklist
	unordered_map<OfertaDisciplina*, unordered_set<int>> removidos_;
	// impede alunos de serem adicionados a turma principal da oferta disciplina
	unordered_map<OfertaDisciplina*, unordered_set<int>> blacklist_;

	static long nrCombAnalise;   //  nr de combinações analisado em cada oferta
	static int nrGeraUnids_;	// contador do nr de vezes que geraTurmasUnidade_ foi chamado.

	// pré-pos abrir turmas de uma oferta disciplina
	bool preAbrirPosTurmasOfertaDisc_(OfertaDisciplina* const oferta);
	// abrir turmas de uma oferta disciplina
	bool abrirTurmasOfertaDisc_(OfertaDisciplina* const ofertaDisc);
	// abrir turmas de uma componente de uma oferta disciplina
	bool abrirTurmasOfertaDiscComp_(OfertaDisciplina* const ofertaDisc, bool teorico);
	// abrir uma turma
	void abrirTurmaPotencial_(OfertaDisciplina* const ofertaDisc, const TurmaPotencial* &turmaPot, const bool compSec);

	// verifica se vale a pena tentar abrir turmas para esta oferta disciplina, i.e. se tem demanda suficiente
	bool tentarAbrirTurmas_(OfertaDisciplina* const ofertaDisc);

	#pragma region [CRIAÇÃO DE TURMAS POTENCIAIS]

	// gerar turmas potenciais para uma oferta disciplina. se for a componente secundária da disciplina, i.e. parte prática de uma disciplina
	// prática + teórica, só poderá usar os alunos já alocados!
	void geraTurmasPotenciais_(OfertaDisciplina* const ofertaDisc, bool teorico, turmasPotOrd &turmasPotenciais, const bool compSec);

	// gera turmas potenciais com preprocessamento
	void geraTurmasComPreProc_(OfertaDisciplina* const ofertaDisc, Calendario* const calendario, 
							bool teorico, vector<vector<pair<int,int>>> const &divisoesCreditos, 
							unordered_set<SalaHeur*> const &salasAssoc, unordered_set<ProfessorHeur*> const &profsAssoc,
							unordered_set<AlunoHeur*> const &alunosDem,
							turmasPotOrd &turmasPotenciais, const bool componenteSec);

	// preenche para cada tuplo (dia, creds, aula) as salas e alunos disponiveis, se houverem)
	void fillMapDisponibilidade_(OfertaDisciplina* const ofertaDisc, bool teorico, Calendario * const calendario, 
									unordered_set<SalaHeur*> const &salasAssoc, unordered_set<AlunoHeur*> const &alunosDem,
									unordered_map<int, unordered_set<int>> const &credsPossDia,
									mapDispDiaCredAula &disponibilidades);

	// verifica se a disciplina pode ter uma aula nesses dias.
	static bool checkAulaDisciplina(Disciplina* const disciplina, int dia, AulaHeur* const aula);

	// adiciona parAlunosSalaDisp ao mapa de disponibilidade
	static void addParAlunosSalaDisp_(int dia, int nrCreds, AulaHeur* const aula, parAlunosSalaDisp const &parDisp, 
										mapDispDiaCredAula &disponibilidades);


	// gera turmas para uma divisao depois do pre-processamento
	void geraTurmasDivisaoPosPreProc_(OfertaDisciplina* const &ofertaDisc, Calendario* const &calendario, 
										vector<pair<int,int>> const &divisao, mapDispDiaCredAula const &disponibilidades, 
										unordered_set<ProfessorHeur*> const &profsAssoc, bool const &teorico, bool const &usaLab, 
										turmasPotOrd &turmasPotenciais, unordered_set<const TurmaPotencial*> &turmasDivisao);

	// versão que usa pointers para objetos criados na heap
	void geraTurmasDivisaoRecHeap_(OfertaDisciplina* const &ofertaDisc, Calendario* const &calendario, 
							vector<pair<int,int>> const &divisao, mapDispDiaCredAula const &disponibilidades, 
							unordered_set<ProfessorHeur*> const &profsAssoc,
							bool const &teorico, bool const &usaLab,
							turmasPotOrd &turmasPotenciais, unordered_set<const TurmaPotencial*> &turmasDivisao,
							RecursivePreProc* const step, vector<RecursivePreProc*> &queueSteps);

	// gera turmas potenciais ofertadisciplina, unidade, componente, calendario, divisao e aulas possiveis
	void geraTurmasUnidade_(OfertaDisciplina* const ofertaDisc, const ConjUnidades* const clusterUnid, Calendario * const calendario, 
							bool teorico, unordered_map<int, AulaHeur*> const &aulas,  unordered_set<AlunoHeur*> const &alunosDisp,
							unordered_set<SalaHeur*> const &salasDisp, unordered_set<ProfessorHeur*> const &profsDisp,
							bool &algumProfPotencial, turmasPotOrd &turmas, unordered_set<const TurmaPotencial*> &turmasDivisao,
							const bool componenteSec, bool checkProfs = false);

	// [ESCOLHA DE TURMA POTENCIAL]

	// com base nas turmas potenciais (separadas em várias lista de prioridade) escolher qual abrir
	bool escolherTurmaPotencial_(turmasPotOrd const &turmasPotenciais, const TurmaPotencial* &turmaEscolhida);
	// escolher qual turma potencial abrir de uma lista
	bool escolherTurmaPotencialLista_(set<const TurmaPotencial*> const &turmasPotenciais, const TurmaPotencial* &turmaEscolhida);

	// [DISPONIBILIDADE ALUNOS]

	// obter alunos que demandam a disciplina
	void getAlunosDemandaNaoAtend_(OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*>& alunosDem, bool compSec);
	// obter alunos disponiveis no calendario
	void getAlunosDispCalendario_(OfertaDisciplina* const ofertaDisc, Calendario* const calendario, unordered_set<AlunoHeur*> const &alunos, 
									unordered_set<AlunoHeur*> &alunosDisp, bool &temFormando, bool &temCoReq);
	// obter alunos (só considera alunos prioridade 1) que estejam disponíveis para aquelas aulas
	void getAlunosDisponiveis_ (OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*> const &alunosDem, 
								const unordered_map<int, AulaHeur*> &aulas, bool &temFormando, bool &temCoReq,
								const bool teorico, unordered_set<AlunoHeur*>& alunosDisponiveis);
	// obter alunos que estejam disponíveis para aquelas aulas, de entre um conjunto dado à partida pois é tida em conta a deslocação
	void getAlunosDispUnidade_ (OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*> const &alunos, const unordered_map<int, AulaHeur*> &aulas, 
								bool &temFormando, bool &temCoReq, set<AlunoHeur*>& alunosDisponiveis);

	// [DISPONIBILIDADE SALAS]

	// obter salas que tenham um numero de horarios disponiveis suficiente para atender a divisão
	void getSalasDispDivisao_(Calendario* const calendario, unordered_set<SalaHeur*> const &salasAssoc, vector<pair<int, int>> const &divisao,
								unordered_set<SalaHeur*> &salasDisp);
	// obter salas que estejam disponíveis naquele horário
	void getSalasDisponiveis_(unordered_set<SalaHeur*> const &salasAssoc, const unordered_map<int, AulaHeur*> &aulas, bool teorico, 
								unordered_set<SalaHeur*>& salasDisponiveis);
	// recebe as salas que estam disponíveis naquele horário e verifica quais pertencem ao conjunto de unidades
	void getSalasDispUnidade_(unordered_set<SalaHeur*> const &salasDisponiveis, const ConjUnidades* const clusterUnid, 
									const unordered_map<int, AulaHeur*> &aulas, bool teorico, unordered_set<SalaHeur*>& salasDispUnid);

	// [DISPONIBILIDADE PROFESSORES]

	// obter professores disponiveis para divisao
	void getProfessoresDispDivisao_(Calendario* const calendario, unordered_set<ProfessorHeur*> const &profsAssoc, vector<pair<int, int>> const &divisao,
								unordered_set<ProfessorHeur*> &profsDisp);
	// obter professores que estejam disponiveis naquele horario
	void getProfessoresDisponiveis_(unordered_set<ProfessorHeur*> const &profsAssoc, const unordered_map<int, AulaHeur*> &aulas, 
									bool &algumPotencial, unordered_set<ProfessorHeur*>& profsDisponiveis);
	// recebe os profs que estam disponíveis naquele horário e verifica quais estão disponiveis tendo em conta a deslocação
	void getProfessoresDispUnidade_(unordered_set<ProfessorHeur*> const &profsDisp, const unordered_map<int, AulaHeur*> &aulas, set<ProfessorHeur*>& profsDispUnidade);

	// nr turmas simultaneas da disciplina
	int nrTurmasSimultaneasDisc_(Disciplina* const disciplina, int dia, AulaHeur* const aula) const;


	// verifica se a turma pode ser aberta
	bool podeAbrirTurma_(int nrAlunos, int nrCreds, bool temFormando, bool usaLab, bool temCoReq);
	// escolher a sala de entre as disponíveis
	SalaHeur* escolherSala_(unordered_set<SalaHeur*> const &salas, int nrAlunos);
	// get salas com capacidade e indice de demanda igual a essa. OBS: salas têm que estar ordenadas por capacidade
	void salasCapIgual_(set<SalaHeur*> const &salas, int capacidade, double indicDem, set<SalaHeur*> &result, bool ascend);

	// escolher o professor de entre os disponíveis
	ProfessorHeur* escolherProfessor_(set<ProfessorHeur*> const &professores);
	// escolher os alunos de entre os disponíveis
	void escolherAlunos_(set<AlunoHeur*> &alunos, int max);

	// add turma potencial
	bool addTurmaPotencial(const TurmaPotencial* turma, turmasPotOrd &turmasPot, unordered_set<const TurmaPotencial*> &turmasDivisao);
	// add turma potencial
	bool addTurmaPotencialTipo(const TurmaPotencial* turma, turmasPotOrd &turmasPot, set<const TurmaPotencial*> &turmasPotTipo, unordered_set<const TurmaPotencial*> &turmasDivisao);
	// verifica se a turma é dominada por outras turmas já geradas. retorna true se a nova turma for dominada
	bool verificaDominancia_(const TurmaPotencial* novaTurma, turmasPotOrd &turmas, unordered_set<const TurmaPotencial*> &turmasDivisao);
	// remove randomicamente uma turma das piores
	void removePiorRandom(set<const TurmaPotencial*> &turmasPot, unordered_set<const TurmaPotencial*> &turmasDivisao);
	// remove as tuas cujo desvio do melhor valor excede o maximo
	void removeBadTurmas(set<const TurmaPotencial*> &turmasPot, unordered_set<const TurmaPotencial*> &turmasDivisao, double valorMax);
	// limpar turmas potenciais. limpa memória
	void limparTurmasPotenciais_(turmasPotOrd &turmas);
	

	#pragma endregion

	#pragma region [UTIL]

	// adicionar a blacklist
	void addToBlackList(OfertaDisciplina* const oferta, unordered_set<AlunoHeur*> const &alunos);

	// nr profs assoc da disciplina
	int nrProfsAssociadosHorario(OfertaDisciplina* const ofertaDisc, int dia, AulaHeur* const aula) const;

	#pragma endregion
};

#endif

