#ifndef _SOLUCAO_HEUR_H_
#define _SOLUCAO_HEUR_H_

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include "AlunoHeur.h"
#include "ProfessorHeur.h"
#include "OfertaDisciplina.h"
#include "HeuristicaNuno.h"
#include "StatsSolucaoHeur.h"

class SalaHeur;
class ObjectiveFunctionHeur;
class TurmaPotencialHeur;
class StatsSolucaoHeur;
class DadosHeuristica;
class Campus;
class Disciplina;
class AlunoDemanda;
class AulaHeur;
class Calendario;
class TurmaHeur;
class MIPAlocarAlunos;
class AbridorTurmas;
class AlocarEquiv;
class RealocAlunosOferta;
class ProblemSolution;
class ProfessorVirtualOutput;
class AtendimentoCampus;
class AtendimentoSala;
class AtendimentoHorarioAula;
class AtendimentoOferta;
class AtendimentoTatico;
class Curso;
class SaveSolucao;
struct AtendFixacao;

struct compOftDisc;
struct compOftDiscEquiv;

// Aliases
typedef unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<int, unordered_set<AlunoDemanda *>>>> conjDemandas;
typedef unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<Curso*, unordered_map<int, unordered_set<AlunoDemanda *>>>>> conjDemandasPorCurso;
typedef unordered_map<Campus*, unordered_map<Disciplina*, OfertaDisciplina*>> conjOfertas;
typedef unordered_map<Campus*, unordered_map<Disciplina*, OfertaDisciplina*>>::iterator itCampODs;

class SolucaoHeur
{
	// Improvers
	friend class TurmaHeur;
	friend class OfertaDisciplina;
	friend class AbridorTurmas;
	friend class AlocarEquiv;
	friend class MIPAloc;
	friend class MIPAlocarAlunos;
	friend class MIPAlocarProfs;
	friend class ImproveMethods;

public:
	SolucaoHeur(void);
	~SolucaoHeur(void);
	
	// estruturas de alocação
	unordered_map<int, AlunoHeur*> alunosHeur;
	unordered_map<Curso*, unordered_set<AlunoHeur*>> alunosPorCurso;
	unordered_map<int, ProfessorHeur*> professoresHeur;
	unordered_map<int, SalaHeur*> salasHeur;

	// perfis de professores virtuais
	unordered_map<int, ProfessorHeur*> profsVirtuais;
	unordered_map<PerfilVirtual*, set<ProfessorHeur*, cmpProfIds>> profsVirtuaisCursoPerfil;

	// gerar solução inicial
	static SolucaoHeur* gerarSolucaoInicial(void);
	// gera solução completa, fixando o atendimento da solução passada
	static SolucaoHeur* gerarSolucaoInicial(ProblemSolution * const partialSol);
	// melhorar solução carregada
	static void improveSolucaoFixada(SolucaoHeur* const solucao);
	// tentar só fazer trocas de sala (e fechar turmas caso necessario) de uma solução carregada
	static void realocSalasSolucaoFix(SolucaoHeur* const solucao);

	// gerar ProblemSolution
	ProblemSolution* getProblemSolution(void) const;

	// load ProblemSolution
	static SolucaoHeur* carregarSolucao(ProblemSolution* const solution);

	// desalocar 'perc' % dos alunos. perc > 0 && per < 100. default: 30
	void desalocarAlunosRandom(int perc);

	// get turmas de uma disciplina
	void getTurmasDisciplina(Disciplina* const disciplina, unordered_set<TurmaHeur*> &turmas) const;

	// print stats
	void printStats(void) const;
	// print demandas atendidas
	void printDemandasAtendidas(char* append) const;

	// is loading?
	bool isLoading(void) const { return loading_; }

	// eh melhor?
	bool ehMelhor(SolucaoHeur* const solucao) const;

	#pragma region [ACESSORES]

	// acesso a estatísticas / FO
	//const ObjectiveFunctionHeur* getObjFunction (void) const { return objFunction_; }
	int nrDiscDuasComp (void) const { return stats_->nrDiscDuasComp_; }
	int nrTurmas (void) const { return stats_->nrTurmasAbertas_; }
	int nrTurmasProfReal (void) const { return stats_->nrTurmasProfReal_; }
	int nrTurmasAbertasCompSec (void) const { return stats_->nrTurmasAbertasCompSec_; }
	int nrAlunosIncompletos (void) const { return stats_->nrAlunosIncompletos_; }
	int nrTurmasInvalidas (void) const { return stats_->nrTurmasInvalidas_; }
	int nrDemandasAtendidas(int prioridade, bool equiv = true) const;
	int nrCreditosAluno(void) const { return stats_->nrCreditosAlunos_; }
	int nrCreditosProf(void) const { return stats_->nrCreditosProfessores_; }
	int nrCreditosProfVirtual(void) const { return stats_->nrCreditosProfsVirtuais_; }

	#pragma endregion

	// get ODs ordenadas
	template<typename T>
	void getOfertasDisciplinaOrd_(set<OfertaDisciplina*, T>& setOrd, int priorAluno, bool equiv = true) 
	{
		for(auto itCampus = ofertasDisciplina_.begin(); itCampus != ofertasDisciplina_.end(); itCampus++)
		{
			for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end(); itDisc++)
			{
				OfertaDisciplina* const oferta = itDisc->second;
				unordered_map<int, unordered_set<AlunoDemanda *>> demandasNaoAtend;
				getDemandasNaoAtendidas(oferta->getCampus(), oferta->getDisciplina(), demandasNaoAtend, true, priorAluno);
				oferta->setDemandasNaoAtend(demandasNaoAtend, equiv);
				setOrd.insert(oferta);
			}
		}
	//	if (CentroDados::getPrintLogs())
	//	{
	//	imprimeOfertasDisc(setOrd,priorAluno);
	//	}
	}
	// reordenar disciplinas
	template<typename T>
	void reordenarOfertasDisciplina_(set<OfertaDisciplina*, T>& setOrd, int priorAluno, bool equiv = true)
	{
		int size = setOrd.size();
		set<OfertaDisciplina*, T> copia = setOrd;
		setOrd.clear();
	
		for(auto it = copia.begin(); it != copia.end(); ++it)
		{
			OfertaDisciplina* const oferta = *it;
			unordered_map<int, unordered_set<AlunoDemanda *>> demandasNaoAtend;
			getDemandasNaoAtendidas(oferta->getCampus(), oferta->getDisciplina(), demandasNaoAtend, true, priorAluno);
			oferta->setDemandasNaoAtend(demandasNaoAtend, equiv);
			setOrd.insert(oferta);
		}
	
		if(setOrd.size() != size)
			HeuristicaNuno::excepcao("SolucaoHeur::reordenarOfertasDisciplina_", "Erro na copia!");

		//if (CentroDados::getPrintLogs())
		//{
		//imprimeOfertasDisc(setOrd,priorAluno);
		//}
	}

	#pragma region [MÉTODOS DE VERIFICAÇÃO]

	// verifica a consistência da solução
	void verificacao_(bool incompletos = true) const;

	// verificar se os objectos estão devidamente linkados
	bool checkLinks(bool incompletos) const;
	// verificar se há conflitos nos horários dos professores/salas/alunos
	bool checkConflitos(void) const;
	// verificar alocação completa e max creditos
	bool checkMaxCredsAlocComp(void) const;
	// [só 1x1 e 1xN] verificar se relação é cumprida
	bool checkRelacTeorPrat(void) const;
	// verificar se as disciplinas que exigem aulas continuas em prat/teor são respeitadas
	bool checkAllAulasContinuas(void) const;
	bool checkAulasContPorTurmaTeor(OfertaDisciplina* const oferta, TurmaHeur* const teor) const;
	bool checkAulasContinuas(unordered_map<int, AulaHeur*> const &aulasp, 
								unordered_map<int, AulaHeur*> const &aulast) const;

	// recalcular stats
	void checkStats(bool print = true) const;
	// verificar e loggar turmas carregadas que tenham sido fechadas
	void checkClosedTurmasLoad(void) const;
	// verificar e loggar mudanças de sala em turmas carregadas de solução fixada
	void checkLogMudancasSala(void) const;

	#pragma endregion

	#pragma region [UTIL]

	// retorna um conjunto de alunosheur associado a um set de alunodemanda
	void getSetAlunosFromSetAlunoDemanda(unordered_set<AlunoDemanda *> const &alunosDemanda, unordered_set<AlunoHeur*> &alunos);
	// retorna um conjunto de alunosheur associado a um set de alunodemanda (nao deixar adicionar aluno na blacklist)
	void getSetAlunosFromSetAlunoDemanda(unordered_set<AlunoDemanda *> const &alunosDemanda, unordered_set<int> const &blacklist,
										unordered_set<AlunoHeur*> &alunos);
	// retorna um conjunto de alunosheur ordenado associado a um set de alunodemanda
	void getSetAlunosFromSetAlunoDemanda(unordered_set<AlunoDemanda *> const &alunosDemanda, set<AlunoHeur*> &alunos);

	// limpar turmas assoc ofertas disciplina
	void limparTurmasAssocAllOfertasPreMIP(void);

	#pragma endregion
	

	#pragma region [IMPRESSÕES]

	// imprime resumo de todas as oferta-disciplinas na ordem corrente
	template<typename T>
	void imprimeOfertasDisc( set<OfertaDisciplina*, T> const&, int );
	
	void printSolucaoLog_() const;

	#pragma endregion
	
private:

	//[campus -> [disciplina -> [oferta]]]
	conjOfertas ofertasDisciplina_;
	vector<OfertaDisciplina*> allOfertasDisc_;

	// POR CURSO
	// ofertas que podem atender demandas por <campus, disciplina, curso>
	unordered_map<Campus*, unordered_map<Disciplina*, unordered_map<Curso*, vector<OfertaDisciplina*>>>> ofertasDiscPorCurso_;

	// nr créditos total -> oferta
	//map<int, unordered_set<OfertaDisciplina*>> ofertasDiscCreditos_;
	//[campus -> [disciplina -> [prioridade -> [aluno.demanda]]]]	(prioridade -1 = equivalencia)
	conjDemandas demandasNaoAtendidas_;

	// POR CURSO!
	conjDemandasPorCurso demsNaoAtendPorCurso_;

	// Turmas de uma determinada disciplina
	unordered_map<Disciplina*, unordered_set<TurmaHeur*>> turmasPorDisc_;

	// Turmas carregadas que foram fechadas
	unordered_set<TurmaHeur*> loadedTurmasClosed;

	// função objectivo
	//ObjectiveFunctionHeur objFunction_;

	// Stats / KPIs
	StatsSolucaoHeur* stats_;

	// indica se estamos a carregar a solução ou não
	bool loading_;

	// id profs virtuais
	static int idVirtual_;

	// [[[MÉTODOS]]]

	// develop solução
	void developSolucao(void);
	// develop solução por prioridades (começar pelos mais prioritarios, etc..)
	void developSolucaoPrior(void);

	// fixa turmas e alunos
	void fixSolucao(void);

	// check versão fast. se for fast imprimir solução e retornar
	bool checkFast(void);
	// check turmas invalidas
	void checkTurmasInv(string const &metodo);

	#pragma region [INICIALIZAR DADOS]

	// criar estruturas salaheur, alunoheur, profheur
	void prepararDados(void);
	// criar a estrutura demandasNaoAtendidas_
	void inicializarDemandasNaoAtendidas_(unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<int, unordered_set<AlunoDemanda *>>>>* const dados);
	// inicializar estrutura demsNaoAtendPorCurso_
	void inicializarDemandasNaoAtendidasPorCurso_(unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<Curso*, unordered_map<int, unordered_set<AlunoDemanda *>>>>>* const dados);

	#pragma endregion

	#pragma region [CONSTRUÇÃO GERAL]

	// criar ofertas disciplina iniciais
	void criarOfertasDisciplina_(bool limpar=true);
	// com base na disciplina original identifica a teorica e a pratica
	void associarDisciplinas_(Disciplina* const disciplina, Disciplina* &discTeorica, Disciplina* &discPratica);

	// POR CURSO!
	void criarOfertasDisciplinaPorCurso_(void);

	// criar turmas ordenando as ofertas disciplina.
	void abrirTurmas_(bool equiv, bool usarVirtual, bool realoc, bool formAnySize, int priorAluno = 0, double relaxMin = 1);
	// criar turmas dando uma ordem randomica às ofertas disciplina.
	void abrirTurmasRandom_(bool equiv, bool usarVirtual, bool realoc, bool formAnySize, int priorAluno = 0, double relaxMin = 1);
	// abrir turmas numa fase final, que tem que ficar viável.
	void abrirTurmasFinal_(bool equiv, bool usarVirtual, bool realoc);

	// local search (destroi e reconstroi, só com turmas com professores reais)
	void destroiReconstroi_(bool usarVirtual, int priorAluno, int maxSec, bool randomConstroi, double maxPerc, double minPerc = 0.01);
	// desaloca totalmente uma percentagem das ofertasDisciplina
	void destroi_(double perc, vector<OfertaDisciplina*> &oftsDesalocadas);
	// guardar solução
	void saveSolucao_(unordered_map<OfertaDisciplina*, unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*>>>> &currSolucao);
	// volta a abrir turmas (só com professores reais)
	void reconstroi_(bool usarVirtual, int priorAluno, bool random);
	// check reconstroi. se solução tiver piorado, repor.
	bool acceptReconstroi_(int oldDemAtendP1, double oldCustoCredito);
	// [rejeitada] repor solução anterior.
	void reporSolucao_(unordered_map<OfertaDisciplina*, unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*>>>> const &oldSolucao);
	// [aceite] deleta os objectos que já não serão usados
	void deleteOldTurmas_(unordered_map<OfertaDisciplina*, unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*>>>> &currSolucao);

	// escolher randomicamente uma percentagem no intervalo [minPerc, maxPerc]
	static double getRandPerc(double maxPerc, double minPerc);

	// get nome do proximo mip a ser impresso
	static string getProxMIPNome(string nomeBase);
	// re-alocar alunos e fechar turmas inválidas (so considerar demandas com prior <= priorAluno)
	void realocarAlunosMIP_(SaveSolucao* const &solucaoAtual, bool realocSalas, int minRecCred, int priorAluno = 0, bool alocarP2 = true);
	// alocar professores no final
	void alocarProfessores(void);
	// re-alocar professores
	void realocarProfsMIP_(bool profsVirtuaisIndiv);

	// try aloca nao atendidos em turmas já abertas
	void tryAlocNaoAtendidos_(bool mudarSala, bool equiv, bool realocar, int priorAluno);
	// tentar alocar P2 heuristicamente
	void tryAlocP2_(int priorAluno = 0);
	// get alunos com demanda P2 nao atendida
	void getAlunosDemP2_(OfertaDisciplina* const oferta, int priorAluno, unordered_set<AlunoHeur*> &alunos);

	// reset indice demanda das salas
	void resetIndicDemSalas_(void);
	// reset indice demandas das ODs
	void resetCountDemsOfts_(void);

	#pragma endregion

	#pragma region [UPDATE DEMANDAS]

	// pega na demanda não atendida original, apaga essa e todas as outras demandas substitutas
	void apagarDemandaNaoAtendEquiv_(Campus* const campus, Disciplina* const disciplina, AlunoHeur* const aluno);
	// apaga demanda não atendida
	void apagarDemandaNaoAtend_(Campus* const campus, Disciplina* const disciplina, int prioridade, int alunoId); 

	// apaga demanda não atendida por curso!
	void apagarDemandaNaoAtendPorCurso_(Campus* const campus, Disciplina* const disciplina, Curso* const curso, int prioridade, int alunoId);

	// pega na demanda não atendida original, adiciona essa e todas as outras demandas substitutas
	void addDemandaNaoAtendEquiv_(Campus * const campus, Disciplina * const disciplina, AlunoHeur* const aluno);
	// adiciona demanda não atendida
	void addDemandaNaoAtend_(Campus * const campus, Disciplina * const disciplina, int prioridade, AlunoHeur* const aluno,
								AlunoDemanda* const demanda);

	// adiciona demanda não atendida por curso!
	void addDemandaNaoAtendPorCurso_(Campus * const campus, Disciplina * const disciplina, int prioridade, AlunoHeur* const aluno,
								AlunoDemanda* const demanda);

	#pragma endregion

	#pragma region [AJUSTAMENTO DA SOLUÇÃO]

	// dar ajustes à solução para eliminar casos que a façam inviável / incompleta
	void acertarSolucao_(bool fixados = false, double relaxMin = 1.0);
	void acertarSolucao_(unordered_map<OfertaDisciplina*, unordered_set<int>> &coReqsInc, bool fixados = false, double relaxMin = 1.0);
	// remover alunos que tenham os correquisitos 
	bool removerCoRequisitosIncompletos_(unordered_map<OfertaDisciplina*, unordered_set<int>> &removidos, bool fixados = false);
	void registarRemocao_(unordered_map<OfertaDisciplina*, unordered_set<int>> &removidos, OfertaDisciplina* const &oferta, int alunoId);
	// remover alunos que estão alocados em uma turma teórica mas não estão alocados à prática ou viceversa
	void removerAllAlunosIncompletos_(void);
	
	// remover todos os alunos das turmas já abertas
	void removerTodosAlunos_(bool fixados = true);
	// guardar alocação actual dos alunos
	void guardarSolucaoAtualAlunos_(SaveSolucao* const &saver) const;

	// get todas as turmas e prof (pré MIPAlocarProfs) e set to virtual se necessário
	void getAllTurmasProf_(unordered_map<TurmaHeur*, ProfessorHeur*> &turmasProfsAtual, bool setVirtual);

	// tentar reduzir o tamanho das salas alocadas às turmas já abertas
	void tryReduzirSalas_(void);
	// tentar mudar a sala em turmas em que a sala tem horario indisponivel
	bool tryMudarSalasIndisp_(void);
	// fechar turmas vazias
	void fecharTurmasVazias_(void);

	#pragma endregion

	#pragma region [PROFESSORES VIRTUAIS INDIVIDUALIZADOS]

	// criar copias dos perfis de professores virtuais
	void criarCopiasPerfisProfsVirtuaisPorCurso(void);
	// criar copias de perfis. (todos os campi!)
	void criaProfessoresVirtuaisPorCurso(int n, TipoTitulacao* const titulacao, TipoContrato* const contrato, Curso* const curso, 
										unordered_set<Disciplina*> const &discsCurr);
	// adicionar horarios de uma discipina
	void addHorariosDiscProf(Professor* const professor, Disciplina* const disciplina);
	// retorna nr de turmas de disciplinas de um determinado curso
	int nrTurmasCurso(Curso* const curso, bool soProfVirtual = false);
	// get titulacoes/tipo contrato curso
	static void getTitulacoesContratosCurso(Curso* const curso, unordered_set<TipoTitulacao*> &titulacoes, 
											unordered_set<TipoContrato*> &contratos);

	#pragma endregion

	#pragma region [CRIAR OUTPUT]
	
	// cria output de atendimentos
	void criarOutput_(ProblemSolution* const solution) const;
	
	void criarOutputNovo_(ProblemSolution* const solution) const;

	// criar output para a turma
	void criarTurmaOutput_(AtendimentoCampus &atendCampus, TurmaHeur* const turma) const;
	
	// criar output para a aula
	void criarAulaOutput_(AtendimentoSala &atendSala, TurmaHeur* const turma, ProfessorHeur* const professor,
		AulaHeur* const aula, int dia, unordered_map<Demanda*, set<AlunoDemanda*>> const &alunosDemanda) const;

	// cria output de professores virtuais.
	void criarOutProfsVirtuais_(ProblemSolution* const solution) const;

	#pragma endregion

	#pragma region [CARREGAR OUTPUT]

	// atendimento campus
	void loadAtendimentoCampus(AtendimentoCampus* const atendCampus, unordered_map<TurmaHeur*, 
								unordered_set<AlunoHeur*>>* const &turmasAlunos,
								unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios);
	// atendimento sala
	void loadAtendimentoSala(AtendimentoSala* const atendSala, itCampODs const &itCampus, 
							unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
							unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios);
	// atendimento horario aula
	void loadAtendimentoHorarioAula(AtendimentoHorarioAula* const atendHoraAula, itCampODs const &itCampus, SalaHeur* const &sala,
									int const &dia, unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
									unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios);
	// atendimento oferta
	void loadAtendimentoOferta(AtendimentoOferta* const atendOferta, itCampODs const &itCampus, SalaHeur* const &sala, int const &dia,
								HorarioAula* const &horario, ProfessorHeur* const &professor, bool const &teorico,
								unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
								unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios);
	
	// atendimento tatico
	void loadAtendimentoTatico(AtendimentoTatico* const atendTatico, itCampODs const &itCampus, 
											SalaHeur* const &sala, int const &dia, 
											unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
											unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios);

	// get alunos atendidos no atendimento oferta
	void getAlunosAtendidos(AtendimentoOferta* const atendOferta, unordered_set<AlunoHeur*> &alunos);
	// get oferta disciplina se já foi criada. se não criar e retornar
	OfertaDisciplina* getAddOfertaDisciplina(Disciplina* const &disciplina, itCampODs const &itCampus);
	// get oferta disciplina se já foi criada. se não retornar nullptr
	OfertaDisciplina* getOfertaDisciplina(Disciplina* const &disciplina, Campus* const campus);
	// get turma se já foi criada, se não, criar e retornar
	TurmaHeur* getAddTurma(OfertaDisciplina* const &ofertaDisc, int const &turmaId, bool const &teorico,
						  SalaHeur* const &sala, ProfessorHeur* const &professor, 
						unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
						unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios,
						const AtendFixacao &fixacoes);

	// carregar as aulas para as turmas, define o calendário e associa a turma a professor e sala
	void loadAulasTurmas(unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios);
	// find calendario turma com base nos horarios
	void findSetCalendarioTurma(TurmaHeur* const turma, unordered_map<int, set<HorarioAula*>> const &aulas);

	// carregar alunos para as turmas (colocando a flag de alunos fixados!)
	void loadAlunosFixadosTurmas(unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos);

	#pragma endregion

	#pragma region [UTIL]

	// retorna as demandas de um certo tuplo (campus, disciplina). retorna false se não encontrar
	bool getDemandasNaoAtendidas(Campus* const campus, Disciplina* const disciplina, unordered_map<int, unordered_set<AlunoDemanda *>> &demandas,
								bool filtrar = true, int priorAluno = 0);
	// retorna as demandas de um certo tuplo (campus, disciplina, prioridade). retorna false se não encontrar
	bool getDemandasNaoAtendidas(Campus* const campus, Disciplina* const disciplina, int prioridade, unordered_set<AlunoDemanda *> &demandas,
								bool filtrar = true, int priorAluno = 0);

	// POR CURSO
	bool getDemandasNaoAtendidas(OfertaDisciplina* const oferta, int prioridade, unordered_set<AlunoDemanda *> &demandas, bool filtrar = true, 
								int priorAluno = 0);

	// filtrar das demandas não atendidas alunos que já não têm margem de créditos para cursar essa disciplina.
	// filtrar tambem demandas de alunos calouros ou inadimplentes caso calInd esteja false !
	void filtrarDemsNaoAtendidas(int nrCredsDisc, int priorAluno, unordered_set<AlunoDemanda *> &demandas);

	// retorna o número de demandas não atendidas (original)
	int nrDemandasNaoAtendidas(int prior = 1) const;
	// retorna o número de demandas não atendidas de um campus (só de prioridade 1 ou todas)
	int nrDemandasNaoAtendidas(Campus* const campus, bool soPrioridadeUm) const;
	// retorna o número de demandas atendidas para um determinado campus, disciplina e prioridade
	int nrDemandasNaoAtendidas(Campus* const campus, Disciplina* const disciplina, int prioridade) const;

	// por curso
	void getMapDemandasNaoAtend(Campus* const campus, Disciplina* const disciplina, set<Curso*> const &cursos, unordered_map<int, int> &demsNaoAtend) const;

	#pragma endregion

	// [MEMORY]

	template<class T>
	void limparMap_ (unordered_map<int, T*> &map)
	{
		typename unordered_map<int, T*>::iterator it = map.begin(); 
		for(; it != map.end();)
		{
			T* obj = it->second;
			it = map.erase(it);
			delete obj;
		}
	}

	void limparOfertasDisciplina_ (void);

};

#endif
