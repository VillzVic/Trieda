#ifndef _CENTRO_DADOS_
#define _CENTRO_DADOS_

#include <fstream>
#include <ctime>
#include "CPUTimerWin.h"

class ProblemData;
class Campus;
class Unidade;
class Sala;
class TurnoIES;
class HorarioAula;
class Professor;
class Oferta;
class Disciplina;
class Aluno;
class Calendario;
class DateTime;

/*
 *	Centro de Dados: classe abstracta usada para conter ProblemData e outras variáveis estáticas para acesso a informação
 *	de qualquer ponto do código
 */
class CentroDados
{
public:
	CentroDados(void);
	~CentroDados(void);

	virtual void foo(void) = 0;	// classe abstrata

	// Get Problem data (lança excepção se nenhum estiver setado!)
	static ProblemData* getProblemData(void);

	// Set Problem Data (evitar que pointer seja mudado por engano)
	static void setProblemData(ProblemData* const probData);
	
	// libertar a memória de problemData e set nullptr
	static void clearProblemData(void);
	
	static void setLoadPartialSol(bool load) { READ_SOLUTION = load; }	
	static bool getLoadPartialSol() { return READ_SOLUTION; }
		
	static void setPrintLogs(bool print) { PRINT_LOGS = print; }	
	static bool getPrintLogs() { return PRINT_LOGS; }
	
	static void openErrorFile();
	static void openWarnFile();
	static void openTestFile();
	static void closeErrorFile();
	static void closeWarnFile();
	static void closeTestFile();
	static void closeFiles();
	
	// Print test message
	static void printTest( std::string method, std::string msg );

	// Print warning message
	static void printWarning( std::string method, std::string msg );
	
	// Print error message
	static void printError( std::string method, std::string msg );

	// Start timer
	static void startTimer();
	
	// Returns the time spent since the last start moment
	static double getLastRunTime();
	
	// Stop timer and returns the time spent between start and stop moments
	static double stopTimer();

	
	// ------------------------------------------------------------------------------------------------------------
	// [PROCURAR PTRS OBJETOS]

	// campus
	static Campus* getCampus(int id);
	// unidade
	static Unidade* getUnidade(int id);
	// sala
	static Sala* getSala(int id);
	// turno
	static TurnoIES* getTurno(int id);
	// horario aula
	static HorarioAula* getHorarioAula(int id);
	// professor [USAR futuro refProfessores]
	static Professor* getProfessor(int id);
	// oferta
	static Oferta* getOferta(int id);
	// disciplina
	static Disciplina* getDisciplina(int id);
	// calendario
	static Calendario* getCalendario(int id);
	
	// get aluno from aluno demanda id [USAR futuro ref alunosDemanda]
	static Aluno* getAlunoIdFromAlunoDemanda(int demandaId);

	
	// ------------------------------------------------------------------------------------------------------------
	// [PROCURAR INFO]
	static DateTime getFimDaFase(int fase);
	// fase do dia (1=Manha, 2=Tarde, 3=Noite)
	static int getFaseDoDia(DateTime dt);

	static bool stringContem(std::string str, std::string contem);

private:

	// Dados do Problema: acessiveis de qualquer lugar
	static ProblemData* problemData;

	static bool READ_SOLUTION;
	static bool PRINT_LOGS;
	
	static std::ofstream *fOutTest;
	static std::ofstream *fOutWarn;
	static std::ofstream *fOutError;

	static CPUTimer timer;
};

#endif
