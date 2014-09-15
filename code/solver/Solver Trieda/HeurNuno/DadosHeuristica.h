#ifndef _DADOS_HEURISTICA_H_
#define _DADOS_HEURISTICA_H_

#include <unordered_map>
#include <unordered_set>
#include <set>
#include "Node.h"

using namespace std;

class ProblemData;
class Disciplina;
class AlunoDemanda;
class Campus;
class AlunoHeur;
class ProfessorHeur;
class SalaHeur;
class Aluno;
class ConjUnidades;
class Curso;
class TurmaHeur;

class DadosHeuristica
{
public:
	DadosHeuristica(void);
	~DadosHeuristica(void);

	// classe abstrata
	virtual void foo() = 0;

	// foi inicializado?
	static bool foiInicializado(void) { return inicializado_; }

	// Pre-processamento dos dados
	static void prepararDados(void);

	static bool getComponentePratica(int id, Disciplina* &pratica);

	// get demandas agregadas
	static unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<int, unordered_set<AlunoDemanda *> > > >* demandasAgregadas(void)
	{
		return &demandasAgregadas_;
	}
	// get demandas agregadas por curso!
	static unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<Curso*, unordered_map<int, unordered_set<AlunoDemanda *>>>>>* demandasAgregadasPorCurso(void) { return &demandasAgregadasPorCurso_; }

	// nr demandas por prioridade
	static const unordered_map<int, int> nrDemandasPrioridade(void) { return nrDemandasPrioridade_; }
	// prioridade de uma demanda
	static int getPrioridade(Campus* const campus, Disciplina* const disciplina, int alunoId);
	static bool getPrioridadeDemanda(Campus* const campus, Disciplina* const disciplina, int alunoId, pair<int, AlunoDemanda*> &par);
	// tem demanda?
	static bool temDemanda(Campus* const campus, Disciplina* const disciplina, int prioridade, int alunoId);

	// get mapEquivOrig para aluno
	static void getMapEquivAluno(Aluno* const aluno, unordered_map<int, unordered_set<int>> &mapEquivOrig);

	// verifica se esta disciplina pode ser considerada equivalente de outra
	static bool checkEquivalencia(AlunoDemanda* const demandaOrig, Campus* const campus, Disciplina* const discEquiv, int alunoId);

	// nr demandas registadas (diferente de originais!)
	static int nrDemandas(int prior, bool equiv = false);

	// todos os clusters de unidades
	static unordered_set<ConjUnidades*> clustersUnidades;

	// prioridades de alunos registadas (por ordem)
	static set<int> prioridadesAlunos;

	// co-requisitos por aluno.id
	static unordered_map<int, vector<vector<AlunoDemanda*>>> coRequisitosPorAluno;
	// demandas do aluno com co-req (pos-processado)
	static unordered_map<int, unordered_set<int>> demandasAlunoCoReq;				// aluno.id -> demanda.id
	static bool temCoReq(int alunoId, int demandaId);

	// get cursosCompatCampusDisc_
	static unordered_map<Campus*, unordered_map<Disciplina*, vector<set<Curso*>>>>* getCursosCompatCampusDisc(void) { return &cursosCompatCampusDisc_;}

private:
	// indica se os dados já foram pre-processados
	static bool inicializado_;

	// [ID][disciplina]
	static unordered_map<int, Disciplina *> componentesPraticas_;

	//[alunoId -> [campus -> [disciplina -> prioridade]]]	(prioridade -1 = equivalencia)
	static unordered_map<int, unordered_map<Campus *, unordered_map<Disciplina *, pair<int, AlunoDemanda*>>>> prioridadeDemandasAlunos;

	// Nr de demandas por prioridade
	static unordered_map<int, int> nrDemandasPrioridade_;

	//[campus -> [disciplina -> [prioridade -> [aluno.demanda]]]]	(prioridade -1 = equivalencia)
	static unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<int, unordered_set<AlunoDemanda *>>>> demandasAgregadas_;
	// NOVO -> por curso
	static unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<Curso*, unordered_map<int, unordered_set<AlunoDemanda *>>>>> demandasAgregadasPorCurso_;

	// guardar demandas P2
	static void guardarDemandasP2_(void);

	// agregar as demandas não atendidas
	static void agregarDemandas_(void);
	static bool adicionarPrioridade_(AlunoDemanda* const alunoDemanda, Campus* const campus, Disciplina* const disciplina, Curso* const &curso, 
										const int p);
	static void adicionarDemanda_(AlunoDemanda* const alunoDemanda, Campus* const campus, Disciplina* const disciplina, const int p);
	static void removeDemanda_(int alunoId, Campus* const campus, Disciplina* const disciplina, const int p);

	// agregar as demandas não atendidas por curso!
	static void adicionarDemandaPorCurso_(AlunoDemanda* const &alunoDemanda, Campus* const &campus, Disciplina* const &disciplina, 
											Curso* const &curso, const int p);
	static void removeDemandaPorCurso_(const int alunoId, Campus* const &campus, Disciplina* const &disciplina, Curso* const &curso, 
										const int p);

	// juntar grupos de cursos compatíveis relevantes por <Campus, Disciplina> com base nos cursos que têm demanda
	static unordered_map<Campus*, unordered_map<Disciplina*, vector<set<Curso*>>>> cursosCompatCampusDisc_;
	static void setCursosCompatCampusDisc(void);

	#pragma region [Calculador de Cliques para Compatibilidade de cursos (algoritmo de Bron–Kerbosch)]

	// gerar cliques máximos de cursos
	static void getMaxCliquesCursos(vector<Curso*> const &cursos, vector<set<Curso*>> &cliquesCursos); 

	// criar nodes turmas (ordem crescente de grau)
	static void criarNodesCursos(vector<Curso*> const &cursos, set<Node*, compNodes> &nodes); 


	#pragma endregion

	// associar componentes teoricas e praticas
	static void mapearComponentesPraticas_(void);

	// criar clusters de unidades
	static void criarClustersUnidades_(void);
	// máxima deslocacao entre campus e unidades em minutos
	static int maxDeslocMin_;

	// preencher prioridades alunos
	static void fillPrioridadesAlunos_(void);

	// set calendarios reduzidos
	static void setCalsReduzidos_(void);
	static void setCalsReduzidosDisc_(Disciplina* const disciplina);
	// calcular numero de horarios por semana para cada turno
	static void calcNrHorsSemanaTurnos_(void);

	// pre-processar co-requisitos
	static void preProcCoRequisitos_(void);
	static void regCoRequisitosAluno_(int alunoId, unordered_map<int, vector<AlunoDemanda*>> const &coReqs);

	// verificar as demandas e as equivalencias
	static void checkDemandasEquiv_(void);
	static void checkDemEquivAluno_(Aluno* const aluno);

	// INVESTIGAR
	static void checkPrintTurno_(AlunoDemanda* const demanda, int alunoId, int discId);
};

#endif

