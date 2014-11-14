#ifndef _POLISH_H_
#define _POLISH_H_

#include <fstream>
#include <vector>
#include <set>

#include <ctime>
#include "CPUTimerWin.h"

#include "VariableMIPUnico.h"

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#endif

class Polish
{
public:

	#ifdef SOLVER_CPLEX
		Polish( OPT_CPLEX * &lp, VariableMIPUnicoHash const &, string originalLogFile );
	#elif SOLVER_GUROBI 
		Polish( OPT_GUROBI * &lp, VariableMIPUnicoHash const &, string originalLogFile );
	#endif

		~Polish();

	bool polish(double* xSol, double maxTime, int percIni, double maxTempoSemMelhora);


private:
	
	// ---------------------------------------------------------------------------------

	void init();

	void decideVarsToFix( std::set<std::pair<int,Disciplina*> > &paraFixarUm, 
						  std::set<std::pair<int,Disciplina*> > &paraFixarZero );

	void fixVarsType1( std::set<std::pair<int,Disciplina*> > const &paraFixarUm, 
				  std::set<std::pair<int,Disciplina*> > const &paraFixarZero );

	void fixVarsType2();

	void optimize();
	void getSolution(double &objN, double &gap);
	void updatePercAndTimeIter( bool &okIter, double objN, double gap );
	void checkTimeWithoutImprov( bool &okIter, double objN );
	void updateObj(double objN);
	void checkTimeLimit(bool &okIter);
	void unfixBounds();

	void logIter(double perc, double tempoIter);

	void setLpPrePasses();
	void setParams(double tempoIter);
	void chgParams();
	void setNewHeurFreq();
	bool needsPolish();
	bool optimized();
	bool infeasible();
	void checkFeasibility();

	void guaranteeSol();

	void initLogFile();
	void printLog( string msg );
	void setOptLogFile(std::ofstream &file, string name, bool clear=true);
	void closeLogFile();
	void restoreOriginalLogFile();

	// ---------------------------------------------------------------------------------

	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp_;
	#elif SOLVER_GUROBI 
	   OPT_GUROBI* lp_;
	#endif
	   
	   // Vars
	   int tempoIter;
	   int perc;
	   int status;
	   double objAtual;

	   // Gurobi parameters
	   int nrPrePasses_;
	   double heurFreq_;

	   // Lp's variables' bounds and values
	   int *idxs;
	   double *vals;
	   BOUNDTYPE *bds;	   
	   double *ubVars;
	   double *lbVars;
	   int *idxSol;

	   // Constants
	   int tempoIni; 
	   int fixType;
	   double maxTime_;
	   double maxTempoSemMelhora_;

	   double *xSol;
	   
	   // Hash which associates the column number with the VariableTatico object.
	   VariableMIPUnicoHash const vHashTatico;
	   
	   // Log file
	   bool hasOrigFile;
	   string originalLogFileName;
	   ofstream polishFile;

	   // Timers
	   CPUTimer tempoPol;
	   CPUTimer tempoSemMelhora;
};




#endif