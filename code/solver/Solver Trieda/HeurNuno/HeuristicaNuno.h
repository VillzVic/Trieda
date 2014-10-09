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

	// se true, está num estado de melhoramento de solução fixada. Caso contrário está a construir do início
	static bool improveSolucao;
	// se true, em modo improve só realoca salas
	static bool soRealocSalas;

	// set input info
	static void setInputInfo(char* inpPath, char* instName, int inputId);
	// setup
	static void setup(ProblemData* const problemData);
	// correr heurística
	static void run (void);
	// clean memory
	static void clean(void);

	// load solução
	static void loadSolucao(ProblemSolution* const probSolution);
	// output solucao
	static void outputSolucao(SolucaoHeur* const solucao, char* append);

	// get solução
	static ProblemSolution* getSolucao(void);

	// load indisponibilidades extra das salas
	static unordered_map<int, unordered_map<int, unordered_set<TurnoIES*>>> indispExtraSalas;
	static void loadIndispExtraSalas(char* const fullPath);

	// limite de iterações / tempo usando quando se quer gerar mais de uma solução de raiz. 
	// [OBS: set limIterHeur = 1 para uma só solução]
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

	// relatorios mudanças em solução fixada
	// ATENÇÃO: Fechamento de turmas deve ser registado logo após ocorrer senão não há forma de conseguir informação
	static void logFecharTurmaLoad(const SolucaoHeur* const solucao, TurmaHeur* const turma);
	static void logNovaSala(TurmaHeur* const turma, SalaHeur* const oldSala, SalaHeur* const newSala);

	// relatorio para ordenação das ofertas-disciplinas -> ordem de atendimento
	static void logOrdemOfertas( std::string msg );

	#pragma endregion

private:
	// input info
	static char path [1024];
	static char instance [1024];
	static int inputId;

	// Solução inicial fixada carregada
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

	// relatorios mudanças em solução fixada
	static std::ofstream* logFecharTurmas_;
	static std::ofstream* logMudancaAlunos_;
	static std::ofstream* logNovaSala_;
	
	// log para analise da ordem de resolucao dos atendimentos
	static std::ofstream* logOrdemOfertas_;

	// abrir logs sugestão
	static void abrirLogsSugestao_(void);

	// é chamado quando é loggado o fechamento de uma turma
	static void logMudancaAlunoFix(const SolucaoHeur* const solucao, int alunoId, TurmaHeur* const oldTurma);
};

#endif