#ifndef _HEURISTICA_NUNO_H_
#define _HEURISTICA_NUNO_H_

#include <unordered_map>
#include <unordered_set>
#include <stdio.h>
#include <ctime>

using namespace std;

class ProblemData;
class DadosHeuristica;
class HorarioAula;
class ProblemSolution;
class AtendimentoCampus;
class SolucaoHeur;
class TurmaHeur;
class SalaHeur;
class TurnoIES;

const unsigned long CLOCKS_PER_MINUTE = ( CLOCKS_PER_SEC * 60 );

class HeuristicaNuno
{
public:
	HeuristicaNuno(void);
	~HeuristicaNuno(void);

	virtual void foo(void) = 0;	// classe abstracta

	static SolucaoHeur* solucaoHeur;
	static ProblemData* probData;

	// se true, est� num estado de melhoramento de solu��o fixada. Caso contr�rio est� a construir do in�cio
	static bool improveSolucao;
	// se true, em modo improve s� realoca salas
	static bool soRealocSalas;

	// set input info
	static void setInputInfo(char* inpPath, char* instName, int inputId);
	// setup
	static void setup(ProblemData* const problemData);
	// correr heur�stica
	static void run (void);
	// clean memory
	static void clean(void);

	// load solu��o
	static void loadSolucao(ProblemSolution* const probSolution);
	// output solucao
	static void outputSolucao(SolucaoHeur* const solucao, char* append);

	// get solu��o
	static ProblemSolution* getSolucao(void);

	// load indisponibilidades extra das salas
	static unordered_map<int, unordered_map<int, unordered_set<TurnoIES*>>> indispExtraSalas;
	static void loadIndispExtraSalas(char* const fullPath);

	// limite de itera��es / tempo usando quando se quer gerar mais de uma solu��o de raiz. 
	// [OBS: set limIterHeur = 1 para uma s� solu��o]
	static const int limIterHeur = 1000;
	static const int limMinHeur = 2;

	#pragma region [LOGS]

	static void logMsg(std::string msg, int lvl);
	static void logMsgInt(std::string msg, int nr, int lvl);
	static void logMsgLong(std::string msg, long nr, int lvl);
	static void logMsgDouble(std::string msg, double nr, int lvl);
	static void logHorario(std::string msg, HorarioAula const &horario, int lvl);
	static void excepcao(std::string metodo, std::string msg);
	static void warning(std::string metodo, std::string msg);

	// relatorios falta input
	static void logDiscSemDivisao(int id, bool teorico, bool compSec);
	static void logDiscSemSala(int id, bool teorico, bool compSec);
	static void logEquivMultiOrig(int alunoId, int equiv, unordered_set<int> origs);
	static void logDemandasEquiv(int alunoId, int dem, int equiv);

	// relatorios mudan�as em solu��o fixada
	// ATEN��O: Fechamento de turmas deve ser registado logo ap�s ocorrer sen�o n�o h� forma de conseguir informa��o
	static void logFecharTurmaLoad(const SolucaoHeur* const solucao, TurmaHeur* const turma);
	static void logNovaSala(TurmaHeur* const turma, SalaHeur* const oldSala, SalaHeur* const newSala);

	// relatorio para ordena��o das ofertas-disciplinas -> ordem de atendimento
	static void logOrdemOfertas( std::string msg );

	#pragma endregion

private:
	// input info
	static char path [1024];
	static char instance [1024];
	static int inputId;

	// Solu��o inicial fixada carregada
	static SolucaoHeur* solutionLoaded;

	// run solution from zero
	static void runNewSolucao_(void);
	// run improve solution
	static void runImprovSolucao_(void);
	// run completing initial solution
	static void runCompleteSolucao_(void);

	// [LOG]
	static const int logLevel_ = 1;		// 0-console 1-essencial, 2-extenso, 3- debug
	static const int consoleLogLevel_ = 1;
	static const bool cutLog_ = true;
	static const int cutRows_ = 1000000;

	static std::ofstream* logFile_;
	static void closeLog_(std::ofstream* logFile);
	static int nrLinhasLog_;

	// relatorios falta de input
	static std::ofstream* logDiscSemDiv_;
	static unordered_map<int, unordered_set<bool>> discSemDiv_;
	static std::ofstream* logDiscSemSala_;
	static unordered_map<int, unordered_set<bool>> discSemSala_;
	static std::ofstream* logCheckEquiv_;

	// write output
	static void writeOutput( ProblemSolution * solution, char * outputFile, char * tempOutput );

	// relatorios mudan�as em solu��o fixada
	static std::ofstream* logFecharTurmas_;
	static std::ofstream* logMudancaAlunos_;
	static std::ofstream* logNovaSala_;
	
	// log para analise da ordem de resolucao dos atendimentos
	static std::ofstream* logOrdemOfertas_;

	// abrir logs sugest�o
	static void abrirLogsSugestao_(void);

	// � chamado quando � loggado o fechamento de uma turma
	static void logMudancaAlunoFix(const SolucaoHeur* const solucao, int alunoId, TurmaHeur* const oldTurma);
};

#endif