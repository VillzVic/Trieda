#pragma once

#include <set>
#include <vector>
#include <ctime>
#include <hash_map>

#include "Solution.h"
#include "Instance.h"
#include "../VariableTatico.h"

typedef std::set<VariableTatico> VariableTaticoSet;

class Instance;
class Solution;
class ProblemData;
class Campus;
class OPT_LP;

typedef struct {
   int disciplina;
   int turma;
   int inicio;
   int fim;
} Atendimento;

class SolverTaticoHeur
{
public:
   SolverTaticoHeur(ProblemData* problemData, Campus* campus);
   ~SolverTaticoHeur(void);

   inline unsigned int getSeed() { return seed; }
   inline int getNumeroVariaveis() { return (int)vSetTatico->size(); }
   inline VariableTaticoSet* getVariableTaticoSet() { return vSetTatico; }
   inline void setInstance(Instance* instance) { this->instance = instance; }
   inline void enableTestingPhase() { testingPhase = true; }
   inline void disableTestingPhase() { testingPhase = false; }
   inline bool isTestingPhase() { return testingPhase; }
   inline void setSeed(unsigned int seed) { this->seed = seed; srand(seed); }
   void useRandomSeed() { setSeed((unsigned int)(time(NULL))); }

   bool hasVariable(VariableTatico& v);
   bool addVariable(VariableTatico& v);

   void build();
   bool solve(double timeLimit);
   bool solve2(double timeLimit);

   bool localopt(Solution* s, double timeLimit);
   
   void setVariableTaticoHash(VariableTaticoHash* vHashTatico) { this->vHashTatico = vHashTatico; }
   void setOptLP(OPT_LP* lp) { this->lp = lp; }
   inline double getObjVal() { return bestObjVal; }
   inline double* getX() { return xSol; }
   void delX() { if (xSol != NULL) delete xSol; xSol = NULL; }

   void writeSolution(std::vector<VariableTatico>& solution);
   void writeSolution(std::string filename);

   void writeInstance(std::string filename);

   void printSolution();

private:
   static int runId;

   unsigned int seed;
   bool testingPhase;

   ProblemData* problemData;
   Campus* campus;
   VariableTaticoSet* vSetTatico;

   VariableTaticoHash* vHashTatico;
   OPT_LP* lp;
   double* xSol;
   double bestObjVal;
   
   Instance* instance;
   Solution* solution;

   std::vector<int> dias;     // vetor de associação de dias
   std::vector<DateTime> horarios; // vetor de associação de horários
   std::map<DateTime, int> mapHorario;

   std::hash_map<int, std::hash_map<int, std::hash_map<int, std::vector<VariableTaticoSet::iterator>>>> mapVariables;
   std::hash_map<int, std::hash_map<int, std::vector<std::pair<int, int>>>> mapTurmasDiaSala;

   void loadAtendimentos(std::vector<Atendimento>& atendimentos);
   Slot readNextSlot(int disciplina, int h);
   bool isHorarioDisponivel(DateTime& horario, std::vector<std::pair<DateTime, DateTime>>& horarios);
   VariableTaticoSet::iterator getVariableTatico(Disciplina* disciplina, int turma, int dia, DateTime& horarioInicial, DateTime& horarioFinal);
   Atendimento* getAtendimento(std::vector<Atendimento>& atendimentos, int disciplina, int turma, int instante);
   bool isValidDivisao(Disciplina* disciplina, int dia, int creditos);
};

