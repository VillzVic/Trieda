#ifndef CMD_LINE
#define CMD_LINE

#include <cstdio>
#include <string>

class CmdLine
{
public:

	CmdLine(int argc, char**& argv);
	~CmdLine();

	bool checkNArgs();
	void init();
	void setOutputNames();
	void setTipoSolver();

	int findArg(char* argFind);
	bool findInputId(int &inputId);
	void checkLoadPartialSol();
	bool checkExecHeuristica();
	void checkArgsHeuristica();
	bool checkMinReceitaCredito();
	bool checkTempoLimMIPs( );
	bool checkMudarSalasMIP();
	bool checkLigarMinAlunosInterno();
	bool checkSoMudarSalas();
	bool checkRelaxMinAlunos();
	bool loadSolucaoInicial(int &idx);

	int getArgc() const { return argc_; }
	char** getArgv() const { return argv_; }
	int getInputId() const { return inputId_; }
	char* getPath( char* path ) const;
	char* getInputName( char* inputFile ) const;
	char* getInputWithPath( char* inputPlusPath ) const;
	char* getTempOutputName( char* tempOutput ) const;
	char* getOutputName( char* outputFile ) const;
	int getTipoSolver() const { return tipoSolver_; }
	
	enum TipoSolver
	{
		EXEC_HEUR = 1,
		LOAD_HEUR = 2,
		LOAD_AND_EXEC_HEUR = 3,
		MIP = 4
	};

private:

	int argc_;
	char** argv_;

	int inputId_;
	char path_[ 1024 ];
	char inputFile_[ 1024 ];
	char tempOutput_[ 1024 ];
	char outputFile_[ 1024 ];
	
	int tipoSolver_;
};

#endif