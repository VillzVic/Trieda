#ifndef _POLISH_H_
#define _POLISH_H_

#include <fstream>
#include <vector>
#include <set>
#include <map>

#include <ctime>
#include "CPUTimerWin.h"

#include "VariableMIPUnico.h"

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#endif

#ifdef SOLVER_GUROBI
#include "MIPCallback.h"
#include "opt_gurobi.h"
#endif

class GoalStatus;

class Polish
{
public:

	#ifdef SOLVER_CPLEX
		Polish( OPT_CPLEX * &lp, VariableMIPUnicoHash const &, string originalLogFile, int phase, double maxFOAddValue=0 );		
	#elif SOLVER_GUROBI 
		Polish( OPT_GUROBI * &lp, VariableMIPUnicoHash const &, string originalLogFile, int phase, double maxFOAddValue=0 );		
	#endif

	~Polish();

	bool polish(double* xSol, double maxTime, int percIni, double maxTempoSemMelhora, GoalStatus* const goal);

	enum MODULE
	{
		TATICO = 1,
		OPERACIONAL = 2
	};
	
private:
	
	// ---------------------------------------------------------------------------------
	
	void updateGoal();

	// Initializing
	void init();
	void mapVariables();
	void mapVariablesTat();
	void loadUnidades();

	// Solution
	int getSolValue(int col);

	// Professors
	void getAllProfessors();

	// Clustering unidades per professor
	void clusterUnidadesByProfs();
	void mapProfUnidFromVariables();
	void calculaNrProfComumParUnid(map<Unidade*, map<Unidade*, int>> &parUnidNrProfComum);
	void calculaClustersProfsComuns(set<Unidade*> &unidsAddedToSomeCluster,
								map<Unidade*, map<Unidade*, int>> const &parUnidNrProfComum);
	void removeDuplicatedClusters();
	static bool equals(set<Unidade*> const & c1, set<Unidade*> const & c2);
	void includeSingleClusters(set<Unidade*> const &unidsAddedToSomeCluster);
	void getProfsAssocCluster(set<Unidade*> const &cluster, set<Professor*> &clusterProfs);
	void addCluster(set<Unidade*> const & cluster);

	// Fix variables
	void fixVars();
	void fixVarsTatico();
	
	// Fix variable type 1
	void clearVarsToFixType1();
	bool ehMarreta(VariableMIPUnico v);
	void decideVarsToFixMarreta();
	void decideVarsToFixOther();
	void decideVarsToFixByPhase();
	void fixVarsProfType1();
	void fixVarsType1();
	void fixVarsType1Tatico();

	// Fix variable type 2
	void fixVarsType2Tatico();
	
	// Fix variable type 3
	void fixVarsType3Tatico();
	void fixVarsType3();

	// Fix variable type 4
	void decideVarsToFixVarsType4(unordered_set<Professor*> &fixedProfs);
	void fixVarsType4Prof(unordered_set<Professor*> const &fixedProfs);

	// Setting unidades to fix or to free
	void fixUnidsTatico();
	void fixVarsDifUnidade();
	void chooseRandUnidade();
	int getPercUnidCurrentFixed();
	int getNrFreeUnidade();
	int getNrUnidToBeFree();
	int getFOValueProfGap(Professor* p);
	int getFOValueClusterUnidade_MinPV(int idxCluster);
	int getFOValueClusterUnidade_MinGapProf(int idxCluster);
	int getFOValueClusterUnidade(int idxCluster);
	void fillClusterIdxToBeChosen();
	void reorderClusterIdxToBeChosen();
	void chooseRandClusterFreeUnidade();
	void chooseWorstClusterFreeUnidade();
	void chooseClusterFreeUnidade();
	void chooseClusterAndSetFreeUnidade();
	void chooseRandAndSetFreeUnidade();
	void chooseAndSetFreeUnidades();
	void decideTypeOfUnidToFree();
	void setNextRandFreeUnidade(int adjustPercUnid=0);
	Unidade* getUnidadeAt(int at);
	void adjustPercFreeUnid(int adjustment);
	void clearFreeUnidade();
	void addFreeUnid(Unidade* unid);
	void setAllFreeUnidade();
	bool isFree(Unidade* const unid);
	bool allUnidadesAreFree();
	bool thereIsFreeUnid();

	void optimize();
	void getSolution(double &objN);
	void setMelhora(double objN);
	void updatePercAndTimeIter();
	void updatePercAndTimeIterSmallGap();
	void updatePercAndTimeIterBigGap();
	void adjustPercOrUnid();
	void decreasePercOrFreeUnid(int percToSubtract);
	void decreasePerc(int percToSubtract);
	void adjustTime();
	void increaseTime();
	void decreaseTime();
	void adjustOkIter();
	bool allFree();
	void setAllFreeConfig();
	bool globalOptimal(double objN);
	void resetIterSemMelhoraConsec();
	void resetIterSemMelhora();
	void updateIterSemMelhora();
	bool checkDecreaseDueToIterSemMelhora();
	void checkEndDueToIterSemMelhora();
	void chgFixType();
	double getTimeWithoutImprov();
	void resetTimeWithoutImprov();
	double getLastTimeWithoutImprov();
	void checkTimeWithoutImprov();
	void updateObj(double objN);
	void checkTimeLimit();
	void unfixBounds();
	void unfixBoundsTatHash(VariableMIPUnicoHash const & hashVar);
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
	   CBData cbData_;
	#endif

	   // Variables
	   bool okIter_;
	   double timeIter_;
	   int perc_;
	   int status_;
	   double objAtual_;
	   double gapAtual_;
	   bool melhorou_;
	   double melhora_;
	   double runtime_;
	   double timeLeft_;
	   int nrIterSemMelhoraConsec_;
	   int nrIterSemMelhora_;
	   int nrIter_;

	   // Unidade to leave free
	   set<Unidade*> unidadeslivres_;
	   int percUnidFixed_;
	   bool tryBranch_;
	   // Cluster of unidades to leave free
	   bool useFreeBlockPerCluster_;
	   int clusterIdxFreeUnid_;
	   // <FO value associado, idxs>
	   std::map<int,std::set<int>> clusterIdxToBeChosen_;
	   
	   // teste tipo 3
	   int idFreeUnid_;

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
	   int fixTypeAnt_;
	   double maxTime_;
	   double maxTempoSemMelhora_;
	   int maxIterSemMelhora_; 
	   bool fixarVarsTatProf_;
	   double minOptValue_;
	   bool useFixationByUnid_;

	   // Solution
	   double *xSol_;
	   GoalStatus* goal_;

	   std::set<int> fixedCols_;

	   // Identify classes to fix
	   std::set<std::pair<int,Disciplina*> > paraFixarUm_;
	   std::set<std::pair<int,Disciplina*> > paraFixarZero_;

	   // Hash which associates the column number with the VariableTatico object.
	   VariableMIPUnicoHash const vHashTatico_;	   
	   VariableMIPUnicoHash vHashTatV_;
	   VariableMIPUnicoHash vHashTatX_;
	   VariableMIPUnicoHash vHashTatZ_;
	   VariableMIPUnicoHash vHashTatK_;
	   VariableMIPUnicoHash vHashTatY_;
	   VariableMIPUnicoHash vHashTatFolgaProfGap_;
	   	   
	   // Unidades
	   set<Unidade*> unidades_;
	   map<int,set<Unidade*>> unidClustersByProfs_;
	   map<int,set<Professor*>> clusterIdxProfs_;
	   map<Professor*, set<Unidade*>> profUnidcluster_;
	   map<Unidade*, set<Professor*>> unidProfs_;
	   
	   // Professors
	   unordered_set<Professor*> professors_;


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