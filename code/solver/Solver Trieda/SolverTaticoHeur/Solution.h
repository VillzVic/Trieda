#pragma once

#include <iostream>
#include <hash_map>
#include <ctime>
#include <windows.h>

enum MoveType;

class Instance;
class Move;
class ShiftMove;
class SwapMove;
class SwapDivisionMove;
class DoubleShiftMove;
class DoubleSwapMove;
class SolverTatico;

typedef struct {
   int disciplina;
   int inicio;
   int fim;
   bool blank;
} Slot;

typedef struct {
   int fo;        // função objetivo

   int inv1;      // inviabilidades por conflito (disciplina, turma)
   int lambda1;   // peso da inviabilidade 1

   int inv2;      // inviabilidades de horários inválidos (hi, hf)
   int lambda2;   // peso da inviabilidade 2

   int imp1;      // incentiva a maximização de horários livres consecutivos
   int beta1;     // peso do imp1

   int imp2;      // incentiva a maximização de "colunas livres"
   int beta2;     // peso do imp2
} ObjectiveFunction;

class Solution
{
public:
   Solution(Instance* instance);

   ~Solution(void);

   void reset();
   void reset(int dia);

   inline Instance* getInstance() { return instance; }
   inline int** getSolution() { return solution; }
   inline ObjectiveFunction getObjectiveFunction() { return objectiveFunction; }

   int eval();
   bool move(Move* move);
   bool back(Move* move);

   ObjectiveFunction evalMove(int disciplina, int dia);
   ObjectiveFunction evalMove(Move* move);

   Move* getBestMove();
   Move* getBestMove(MoveType type);

   Move* getRandomMove();
   Move* getRandomMove(MoveType type);

   Move* getRandomMove(int dia);
   Move* getRandomMove(MoveType type, int dia);

   void sa(int maxIter, double To, double Tf, double alpha);
   void mrd(int maxIter);
   void mrd(int maxIter, int maxSide, double sideFactor);
   void multimrd(int maxIter, int mrdMaxIter);
   void multistart(int maxIter, int lsMaxIter, clock_t startTime, double timeLimit);
   void vrnd(int maxIter);

   void dailyMRD(int maxIter);
   void dailyVRND(int maxIter);

   void enhancedMRD(int maxIter);
   void enhancedVRND(int maxIter);

   void mrd(int maxIter, int dia);
   void vrnd(int maxIter, int dia);

   void md(clock_t startTime, double timeLimit);

   void ils(int maxIter, int kpMax, int kpStart, int kpInc, int mrdMaxIter, double incFactor, bool runUntilInfeasible, clock_t startTime, double timeLimit);
   void ils2(int maxIter, int kpMax, int kpStart, int kpInc, int mrdMaxIter, double incFactor, bool runUntilInfeasible, clock_t startTime, double timeLimit);
   void mathILS(int maxIter, bool runUntilInfeasible, SolverTatico* solver, double& elapsedTime);

   int getDivisao(int disciplina, int turma) { return divisoes[disciplina][turma]; }

   void alloc(int disciplina, int turma, int divisao, int dia, int horario, int tempo);
   void alloc(int disciplina, int turma, int dia, int hi, int hf);
   int freeslot(int disciplina, int dia, int turma, int tempo);

   inline bool infeasible() { return !feasible(); }
   inline bool feasible() { return (objectiveFunction.inv1 + objectiveFunction.inv2 == 0); }
   bool isEmpty();

   void improve(int target);
   void makeFeasible();

   bool unalloc();

   Slot readNextSlot(int disciplina, int h);
   std::pair<int, int> getNumeroDemandasAtendidas();
   std::vector<std::pair<int, int>>* getDemandasNaoAtendidas();
   std::string getFatorAtendimento();
   int getNumeroAlunosAtendidos();

   inline void setAlunosNaoAtendidos(int value) { alunosNaoAtendidos = value; }
   void updateAlunosNaoAtendidos() { alunosNaoAtendidos = totalAlunos - getNumeroAlunosAtendidos(); }

   int getMIPObjVal() { return (calculaInviabilidades() + alunosNaoAtendidos); }

   Solution* clone();
   std::string toString();

   bool operator =(const Solution& right);
   bool operator <(const Solution& right);

private:
   Instance* instance;

   ObjectiveFunction objectiveFunction;

   int** solution; // [disciplina][instante:(dia,horario)] = turma
   std::hash_map<int, int>* divisoes; // [disciplina][turma] = divisao

   int* imp2weights;
   int totalAlunos;

   int alunosNaoAtendidos;
   bool* turmasDisponiveis;

   void build();
   void destroy();

   int calculaInviabilidadesPorConflito();                  // número de inviabilidades por conflitos
   int calculaInviabilidadesPorConflito(int dia);
   int calculaInviabilidadesHorariosIndisponiveis();        // número de inviabilidades de horários indisponíveis
   int calculaInviabilidadesHorariosIndisponiveis(int disciplina, int dia);
   int calculaFreeSlots();                                  // incentivo para os horários livres consecutivos
   int calculaFreeSlots(int dia);
   int calculaFreeColumns();                                // incentivo para as "colunas" livres
   int calculaFreeColumns(int dia);

   int calculaInviabilidades();

   inline int getWeight(int value) { return imp2weights[value]; }

   Slot getRandomSlot();
   Slot getRandomSlot(int disciplina, int dia, int diff);
   Slot getSlot(int disciplina, int dia, int turma);
   int getRandomFreeSlot(int disciplina, int dia, int tempo, int turma);

   Move* getRandomShiftMove();
   Move* getRandomSwapMove();
   Move* getRandomDoubleShiftMove();
   Move* getRandomDoubleSwapMove();
   Move* getRandomSwapDivisionMove();

   Move* getRandomShiftMove(int dia);
   Move* getRandomSwapMove(int dia);
   Move* getRandomDoubleShiftMove(int dia);
   Move* getRandomDoubleSwapMove(int dia);

   Move* getBestShiftMove();
   Move* getBestShiftMove(int disciplina, int dia);
   Move* Solution::getBestShiftMove(Slot& slot);

   void shift(ShiftMove* move);
   void backShift(ShiftMove* move);

   void swap(SwapMove* move);
   void backSwap(SwapMove* move);

   void swapDivision(SwapDivisionMove* move);
   void backSwapDivision(SwapDivisionMove* move);

   void doubleShift(DoubleShiftMove* move);
   void backDoubleShift(DoubleShiftMove* move);

   void doubleSwap(DoubleSwapMove* move);
   void backDoubleSwap(DoubleSwapMove* move);

   ObjectiveFunction evalMove(ShiftMove* move);
   ObjectiveFunction evalMove(SwapMove* move);
   ObjectiveFunction evalMove(SwapDivisionMove* move);

   void clearslot(int disciplina, int inicio, int fim);
   void setslot(int disciplina, int inicio, int fim, int turma);

   bool hasSwapMove(int disciplina, int dia, int turma);
   bool hasSwapMove();

   void ilsPerturbation(int kp);

   int calculaFreeColumnsInstante(int instante);

   bool reincorporar(Solution* s, int disciplina, int turma, int target);

   std::pair<int, int> getMaiorInviabilidadePorConflito();
   std::pair<int, int> getMaiorInviabilidadeHorariosIndisponiveis();
   std::pair<int, int> getAlocacaoInviavel();
   std::pair<int, int> getMenorNumeroAlunos();
};
