#ifndef _PARAMETROS_HEURISTICA_H_
#define _PARAMETROS_HEURISTICA_H_

class Professor;
class ProblemData;

class ParametrosHeuristica
{
public:
	ParametrosHeuristica(void);
	~ParametrosHeuristica(void);

	virtual void foo () = 0;	// abstract

	// setup dados
	static void setup(ProblemData* const problemData);

	static void clean() { delete professorVirtual; }

	static Professor* professorVirtual;

	// Vers�o r�pida da heur�stica, n�o usa MIP
	static const bool versaoFast;

	// Local Search
	static const bool localSearch;
	static const int timeLocalSearch;	// segundos
	
	// esquema de abertura de pratica
	enum abrePraticas { UM_UM = 1, UM_N = 2, N_N = 3 };
	static abrePraticas relacPraticas;

	// set relac praticas
	static void setRelacPraticas(ProblemData* const data);

	// desligar m�nimo de alunos interno
	static bool desligarMinAlunosInterno;
	// reduzir calendarios. remover dos calendarios da disciplina aqueles que s�o abrangidos por outros
	static const bool reduzirCalendarios;
	// indica se gerar de novo todas as combina��es de cr�ditos ou n�o
	static const bool gerarAllCombsCreds;

	// Prioriza mais o uso de prof real
	static const bool priorForteProfVirt;
	// Controla se pode ser aberta turma com prof virtual na fase heur�stica
	static const bool permitirProfVirt;

	// --------------------------------------- ABRIDOR TURMAS / GERAIS --------------------------------------------

	// Max cr�ditos para al�m dos P1 demandados que podem ser alocados.
	static const int maxCredsExcedentes;

	// Deixar abrir quando � turma de componente
	// M�ximo intervalo aulas (min)
	static const int maxIntervAulas;
	// Minimo alunos turma (quando n�o definido!)
	static int minAlunosTurma;
	static int limMinAlunosTurma;
	// get min alunos
	static int getMinAlunos(bool usaLab);
	static bool temMins(void);

	// Flexibiliza��o da regra de minimo de n�mero de alunos por turma, para abrir algumas turmas a mais pelo abridor.
	static double relaxMinAlunosTurma;

	// Utiliza��o m�nima para abrir uma turma numa sala maior do que a sua capacidade
	static const double minCapacidadeUsoSala;
	// Determina o desvio m�ximo do valor uma turma potencial relativamente ao m�ximo para poder ser considerada na escolha randomizada
	static const double desvioMaximoValorTurma;	
	
	// Determina se depois de alocar as turmas principais de uma turma, � corrido um algoritmo de recoloca��o de alunos para tentar
	// abrir capacidade para demandas n�o atendidas
	static const bool heurRealocAlunos;
	// Determina se � usado o mecanismo de eliminar turmas potenciais dominadas
	static const bool dominanciaTurmas;

	// limitar numero de turmas simultaneas de uma disciplina
	static const bool limTurmasSimultaneas;
	static const int slackTurmasSimult;
	static const int slackTurmasSimultSemProf;

	// M�ximo de turmas potenciais guardadas
	static const int maxTurmasPotenciais;

	// Utilizar indicadores de demanda das salas/labs
	static const bool indicDemSalas;

	// m�ximo de itera��es na abertura de turmas final
	static const int maxIterAbrirTurmasFinal;

	// [ENDREGION]


	// Id Professor Virtual
	static const int profVirtualId;


	// --------------------------------------- MIP ALOC --------------------------------------------

	// time limit (segundos). se for menor que zero, n�o � considerado
	static int timeLimitMIP;
	// enfase heuristica
	static const double heurFreqMIP;

	// [ENDREGION]


	// --------------------------------------- MIP ALOC ALUNOS --------------------------------------------

	// Considerar P2
	static const bool alocarP2;

	// fechar turmas carregadas
	static const bool fecharTurmasCarregadas;

	// Mudar salas??
	static bool mipRealocSalas;

	// Coeficientes MIPAlocarAlunos
	static const double coefDemAtendP1;
	static const double coefDemAtendP2;
	static const double coefEquivalenciaMIP;	// coeficiente pelo qual multiplicamos o peso de uma demanda equivalente
	static const double pesoExtraFormando;		// valor para multiplicar o coeficiente de demanda atendida de um formando
	static const double pesoExtraCalouro;		// valor para multiplicar o coeficiente de demanda atendida de um calouro
	// valor para multiplicar o coeficiente de demanda atendida de um aluno que j� tinha essa demanda atendida na solu��o carregada
	static const double pesoExtraCarregado;		
	static const double coefAbrirTurmaMIP;
	static const double coefAbrirTurmaMIPCreds;	// quando maximizar creditos

	// MIP Gap
	static const double realocarMipGapTolerance;

	// minimo imposto no MIPAlocarAlunos para o r�cio (cr�ditos aluno / cr�ditos prof)
	static double minProdutividadeCred;

	// coef da vari�vel de folga m�nimo de alunos por turma (s� em modo Improve Solu��o !)
	static const double coefPenalViolaMinAlunos;

	// [ENDREGION]


	// --------------------------------------- MIP ALOC PROFS --------------------------------------------

	// considerar CH semestre anterior?
	static const bool considerarChAnterior;
	// usar professores virtuais individualizados
	static const bool profsVirtuaisIndiv;
	// coeficiente alocar turma a prof real
	static const double coefProfReal;
	// coeficiente alocar turma a prof virtual
	static const double coefProfVirtual;
	// coeficiente alocar turma a prof virtual �nico
	static const double coefProfVirtualUnico;
	// coeficiente contratar prof virtual individualizado
	static const double coefHireProfVirtual;
	// minimo de carga hor�ria semanal � restri��o forte ou fraca ?
	static const bool minChForte;
	// penaliza��o por cada cr�dito que foi alocado a menos a um professor, se o m�nimo n�o tiver sido atingido.
	static const double penalCredMinChSemana;
	// penaliza��o por cada cr�dito que foi alocado a menos a um professor, se a % da ch anterior n�o tiver sido atingida.
	static const double penalCredChAnterior;
	// % da CH anterior desejada. 1 = 100%.
	static double percChAnterior;
	// coeficiente associado a alocar um professor a pelo menos uma aula num dia da semana
	static const double coefProfDia;
	// Proibe gap para professor virtual, al�m dos reais
	static const bool proibirGapProfVirt;

	// -------------------------------------------------------------------------------------------------
	// Solu��o inicial

	enum InitialSolLoadMode
	{
		LoadInitSolAndComplete,
		LoadInitSolAndImprove
	};

	static const InitialSolLoadMode initSolLoadMode;
	
	// -------------------------------------------------------------------------------------------------

	// Destrui��o da solu��o final para testar carregamento [deve tar estar FALSE no funcionamento normal]
	static const bool finalDestroy;
};

#endif
