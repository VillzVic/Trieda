#ifndef _POLISH_H_
#define _POLISH_H_

#include <fstream>
#include <vector>
#include <set>

#include <ctime>
#include "CPUTimerWin.h"

#include "VariableMIPUnico.h"
#include "VariableOp.h"

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
		Polish( OPT_CPLEX * &lp, VariableMIPUnicoHash const &, string originalLogFile, int phase=Polish::PH_OTHER );
		Polish( OPT_CPLEX * &lp, VariableOpHash const &, string originalLogFile, int phase=Polish::PH_OTHER );
	#elif SOLVER_GUROBI 
		Polish( OPT_GUROBI * &lp, VariableMIPUnicoHash const &, string originalLogFile, int phase=Polish::PH_OTHER );
		Polish( OPT_GUROBI * &lp, VariableOpHash const &, string originalLogFile, int phase=Polish::PH_OTHER );
	#endif

	~Polish();

	bool polish(double* xSol, double maxTime, int percIni, double maxTempoSemMelhora);

	enum MODULE
	{
		TATICO = 1,
		OPERACIONAL = 2
	};

	enum PHASE
	{
		PH_MAX_ATEND = 1,
		PH_MIN_PV = 2,
		PH_MIN_GAP = 3,
		PH_OTHER = 4
	};

private:
	
	// ---------------------------------------------------------------------------------

	void init();

	void decideVarsToFixType1();
	void fixVarsProfType1();
	void fixVarsType1();

	void fixVarsType2();
	void fixVarsType2Tatico();
	void fixVarsType2Op();

	void optimize();
	void getSolution(double &objN, double &gap);
	void setMelhora( double objN );
	void updatePercAndTimeIter( bool &okIter, double objN, double gap );
	void updatePercAndTimeIterSmallGap( bool &okIter, double objN );
	void updatePercAndTimeIterBigGap( double objN );
	void adjustTime();
	void increaseTime();
	void decreaseTime();
	void resetIterSemMelhora();
	void checkIterSemMelhora();
	void checkEndDueToIterSemMelhora();
	void chgFixType();
	void checkTimeWithoutImprov( bool &okIter, double objN );
	void updateObj(double objN);
	void checkTimeLimit(bool &okIter);
	void unfixBounds();
	void unfixBoundsTatico();
	void unfixBoundsOp();

	void logIter(double perc, double tempoIter);

	void setLpPrePasses();
	void chgLpRootRelax();
	void setParams(double tempoIter);
	void chgParams();
	void setNewHeurFreq();
	double getTotalElapsedTime();
	double getLeftTime();
	bool needsPolish();
	bool optimized();
	bool infeasible();
	bool optimal();
	bool unoptimized();
	bool timeLimitReached();
	void checkFeasibility();

	void guaranteeSol();

	void initLogFile();
	void printLog( string msg );
	void setOptLogFile(std::ofstream &file, string name, bool clear=true);
	void closeLogFile();
	void restoreOriginalLogFile();
	
	// ---------------------------------------------------------------------------------
	// Local branch
	
	void mainLocalBranching();
	bool localBranching();
	void doLocalBranch();
	void firstLBConstr();
	void updateLBConstr();
	void getLocalBranchVariables(int* idxsVars0, int* idxsVars1, double* coefs0, double* coefs1, int &numIdx0, int &numIdx1);
	void removeLBConstr();
	void optimizeLB();

	double bestObjLB_;
	bool melhorouLB_;
	double timeIterLB_;
	double runtimeLB_;
	int lbRowNr_;
	int k_;

	// ---------------------------------------------------------------------------------

	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX *lp_;
	#elif SOLVER_GUROBI 
	   OPT_GUROBI* lp_;
	#endif
	   
	   // Vars
	   double timeIter_;
	   int perc_;
	   int status_;
	   double objAtual_;
	   bool melhorou_;
	   double melhora_;
	   double runtime_;
	   double timeLeft_;
	   int nrIterSemMelhora_;

	   // Gurobi parameters
	   int nrPrePasses_;
	   double heurFreq_;

	   // Lp's variables' bounds and values
	   int *idxs_;
	   double *vals_;
	   BOUNDTYPE *bds_;	   
	   double *ubVars_;
	   double *lbVars_;
	   int *idxSol_;

	   // Constants
	   int tempoIni_; 
	   int fixType_;
	   double maxTime_;
	   double maxTempoSemMelhora_;
	   int maxIterSemMelhora_; 
	   
	   // Solution
	   double *xSol_;
	   
	   // Identify classes to fix
	   std::set<std::pair<int,Disciplina*> > paraFixarUm_;
	   std::set<std::pair<int,Disciplina*> > paraFixarZero_;

	   // Hash which associates the column number with the VariableTatico object.
	   VariableMIPUnicoHash const vHashTatico_;
	   
	   // Hash which associates the column number with the VariableOp object.
	   VariableOpHash const vHashOp_;
	   
	   MODULE const module_;
	   int const phase_;

	   // Log file
	   bool hasOrigFile_;
	   string originalLogFileName_;
	   ofstream polishFile_;

	   // Timers
	   CPUTimer tempoPol_;
	   CPUTimer tempoSemMelhora_;
};




#endif