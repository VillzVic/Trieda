#include "Solution.h"

#include <hash_map>

#include "Instance.h"
#include "GUtil.h"
#include "Move.h"
#include "ShiftMove.h"
#include "SwapMove.h"
#include "SwapDivisionMove.h"
#include "DoubleShiftMove.h"
#include "DoubleSwapMove.h"
#include "SolutionBuilder.h"
#include "../Disciplina.h"
#include "CPUTimerWin.h"
#include "SolverTatico.h"
#include "TimeLogger.h"

#define DISPLAY_ILS
//#define ALLOW_INFEASIBLE_SLOT
//#define LOG_TIME

Solution::Solution(Instance* instance)
{
   this->instance = instance;
   this->solution = NULL;
   this->divisoes = NULL;
   this->imp2weights = NULL;

   this->totalAlunos = instance->getTotalAlunos();
   this->alunosNaoAtendidos = 0;
   this->turmasDisponiveis = new bool[instance->getNumeroTurmas()+1];

   build();
}

Solution::~Solution(void)
{
   destroy();
}

void Solution::build()
{
   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();

   this->solution = new int*[numDisciplinas];

   for (int d=0; d<numDisciplinas; d++)
      solution[d] = new int[numInstantes];

   this->divisoes = new std::hash_map<int, int>[numDisciplinas];
   this->imp2weights = new int[numDisciplinas];

   reset();
}

void Solution::reset()
{
   objectiveFunction.fo = INT_MAX;
   objectiveFunction.inv1 = 0;
   objectiveFunction.lambda1 = 1;//30 * (instance->getNumeroHorarios() * instance->getNumeroHorarios() + instance->getNumeroHorarios());
   objectiveFunction.inv2 = 0;
   objectiveFunction.lambda2 = 1;//10 * (instance->getNumeroHorarios() * instance->getNumeroHorarios() + instance->getNumeroHorarios());
   objectiveFunction.imp1 = 0;
   objectiveFunction.beta1 = -1;
   objectiveFunction.imp2 = 0;
   objectiveFunction.beta2 = -1;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();

   for (int d=0; d<numDisciplinas; d++)
   {
      for (int t=0; t<numInstantes; t++)
#ifdef ALLOW_INFEASIBLE_SLOT
         solution[d][t] = 0;
#else
         solution[d][t] = (instance->isHorarioDisponivel(d, t) ? 0 : -1);
#endif
      divisoes[d].clear();
   }

   imp2weights[0] = 0;
   for (int d=1; d<numDisciplinas; d++)
      imp2weights[d] = imp2weights[d-1] + (int)(sqrt((double)d));
}

void Solution::destroy()
{
   if (solution != NULL)
   {
      int numDisciplinas = instance->getNumeroDisciplinas();
      for (int d=0; d<numDisciplinas; d++)
      {
         int* sd = solution[d];
         delete [] sd;
      }
      delete [] solution;
   }
   solution = NULL;

   if (divisoes != NULL)
      delete [] divisoes;
   divisoes = NULL;

   if (imp2weights != NULL)
      delete [] imp2weights;
   imp2weights = NULL;

   delete [] turmasDisponiveis;
}

Solution* Solution::clone()
{
   Solution* solution = new Solution(this->instance);
   *solution = *this;
   return solution;
}

bool Solution::operator =(const Solution& right)
{
   this->objectiveFunction = right.objectiveFunction;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();

   this->alunosNaoAtendidos = right.alunosNaoAtendidos;

   for (int d=0; d<numDisciplinas; d++)
   {
      for (int t=0; t<numInstantes; t++)
            solution[d][t] = right.solution[d][t];
      divisoes[d] = right.divisoes[d];
   }

   return true;
}

bool Solution::operator <(const Solution& right)
{
   return (this->objectiveFunction.fo < right.objectiveFunction.fo);
}

int Solution::eval()
{
#ifdef LOG_TIME   
   TimeLogger::start("eval");
#endif

   /*objectiveFunction.inv1 = calculaInviabilidades();
   objectiveFunction.inv1 = calculaInviabilidadesPorConflito();
   objectiveFunction.inv2 = 0;//calculaInviabilidadesHorariosIndisponiveis();
   objectiveFunction.imp1 = 0;//calculaFreeSlots();
   objectiveFunction.imp2 = 0;//calculaFreeColumns();

   objectiveFunction.fo = objectiveFunction.lambda1 * objectiveFunction.inv1 + 
                          objectiveFunction.lambda2 * objectiveFunction.inv2 + 
                          objectiveFunction.beta1 * objectiveFunction.imp1 +
                          objectiveFunction.beta2 * objectiveFunction.imp2;*/

   objectiveFunction.inv1 = calculaInviabilidades();
   objectiveFunction.fo = objectiveFunction.inv1 + alunosNaoAtendidos;

#ifdef LOG_TIME   
   TimeLogger::stop("eval");
#endif

   return objectiveFunction.fo;
}

ObjectiveFunction Solution::evalMove(int disciplina, int dia)
{
#ifdef LOG_TIME
   TimeLogger::start("eval");
#endif
   ObjectiveFunction value;
   value.inv1 = calculaInviabilidadesPorConflito(dia);
   value.inv2 = calculaInviabilidadesHorariosIndisponiveis(disciplina, dia);
   value.imp1 = 0;//calculaFreeSlots(dia);
   value.imp2 = 0;//calculaFreeColumns(dia);
   //value.beta1 = objectiveFunction.beta1;
   //value.beta2 = objectiveFunction.beta2;
   //value.lambda1 = objectiveFunction.lambda1;
   //value.lambda2 = objectiveFunction.lambda2;

   /*value.fo = value.lambda1 * value.inv1 + 
              value.lambda2 * value.inv2 + 
              value.beta1 * value.imp1 +
              value.beta2 * value.imp2;*/

   value.fo = value.lambda1 * value.inv1;

#ifdef LOG_TIME
   TimeLogger::stop("eval");
#endif

   return value;
}

ObjectiveFunction Solution::evalMove(Move* move)
{
   if (move != NULL)
   {
      switch (move->getType())
      {
      case SHIFT: return evalMove((ShiftMove*)move);
      case SWAP: return evalMove((SwapMove*)move);
      case SWAP_DIVISION: return evalMove((SwapDivisionMove*)move);
      }
   }

   ObjectiveFunction fo = getObjectiveFunction();
   fo.fo = INT_MAX;
   fo.inv1 = INT_MAX;
   fo.inv2 = INT_MAX;
   return fo;
}

ObjectiveFunction Solution::evalMove(ShiftMove* move)
{
   int dia = instance->getDia(move->getInicio1());
   ObjectiveFunction partialFo = evalMove(move->getDisciplina(), dia);
   return partialFo;
}

ObjectiveFunction Solution::evalMove(SwapMove* move)
{
   int dia = instance->getDia(move->getInicio1());
   ObjectiveFunction partialFo = evalMove(move->getDisciplina(), dia);
   return partialFo;
}

ObjectiveFunction Solution::evalMove(SwapDivisionMove* move)
{
   std::set<int> dias;
   for (std::vector<int>::iterator it = move->getInicio1().begin(); it != move->getInicio1().end(); ++it)
      dias.insert(instance->getDia(*it));
   for (std::vector<int>::iterator it = move->getInicio2().begin(); it != move->getInicio2().end(); ++it)
      dias.insert(instance->getDia(*it));
   
   ObjectiveFunction partialFo;
   partialFo.beta1 = objectiveFunction.beta1;
   partialFo.lambda1 = objectiveFunction.lambda1;
   partialFo.fo = 0;
   partialFo.inv1 = 0;
   partialFo.inv2 = 0;
   partialFo.imp1 = 0;
   partialFo.imp2 = 0;
   for (std::set<int>::iterator it = dias.begin(); it != dias.end(); ++it)
   {
      int dia = *it;
      ObjectiveFunction sub = evalMove(move->getDisciplina(), dia);
      partialFo.fo += sub.fo;
      partialFo.inv1 += sub.inv1;
      partialFo.inv2 += sub.inv2;
      partialFo.imp1 += sub.imp1;
      partialFo.imp2 += sub.imp2;
   }
   return partialFo;
}

bool Solution::move(Move* move)
{
   if (move == NULL)
      return false;

#ifdef LOG_TIME   
   TimeLogger::start("move");
#endif

   move->setObjectiveFunction(objectiveFunction);

   //ObjectiveFunction partialFo1 = evalMove(move);

   switch (move->getType())
   {
   case SHIFT: shift((ShiftMove*)move); break;
   case SWAP: swap((SwapMove*)move); break;
   case SWAP_DIVISION: swapDivision((SwapDivisionMove*)move); break;
   case DOUBLE_SHIFT: doubleShift((DoubleShiftMove*)move); break;
   case DOUBLE_SWAP: doubleSwap((DoubleSwapMove*)move); break;
   default: return false;
   }

   /*ObjectiveFunction partialFo2 = evalMove(move);
   objectiveFunction.fo = objectiveFunction.fo - partialFo1.fo + partialFo2.fo;
   objectiveFunction.inv1 = objectiveFunction.inv1 - partialFo1.inv1 + partialFo2.inv1;
   objectiveFunction.inv2 = objectiveFunction.inv2 - partialFo1.inv2 + partialFo2.inv2;
   objectiveFunction.imp1 = objectiveFunction.imp1 - partialFo1.imp1 + partialFo2.imp1;
   objectiveFunction.imp2 = objectiveFunction.imp2 - partialFo1.imp2 + partialFo2.imp2;*/

   eval();

   move->setValue(objectiveFunction.fo);

#ifdef LOG_TIME   
   TimeLogger::stop("move");
#endif

   return true;
}

bool Solution::back(Move* move)
{
   if (move == NULL)
      return false;

#ifdef LOG_TIME   
   TimeLogger::start("back");
#endif

   switch (move->getType())
   {
   case SHIFT: backShift((ShiftMove*)move); break;
   case SWAP: backSwap((SwapMove*)move); break;
   case SWAP_DIVISION: backSwapDivision((SwapDivisionMove*)move); break;
   default: return false;
   }

   objectiveFunction = move->getObjectiveFunction();

#ifdef LOG_TIME   
   TimeLogger::stop("back");
#endif

   return true;
}

void Solution::alloc(int disciplina, int turma, int divisao, int dia, int horario, int tempo)
{
   int t = instance->getInstante(dia, horario);
   for (int h=0; h<tempo; h++)
      solution[disciplina][t+h] = turma;
   divisoes[disciplina][turma] = divisao;
}

int Solution::freeslot(int disciplina, int dia, int turma, int tempo)
{
   int start = instance->getInstante(dia, 1);
   int end = start + instance->getNumeroHorarios() - 1;
   
   int* sd = solution[disciplina];

   int iend = end - tempo + 2;
   for (int i=start; i<iend; i++)
   {
      bool found = true;
      int jend = i + tempo;
      for (int j=i; j<jend; j++)
      {
#ifdef ALLOW_INFEASIBLE_SLOT
         if (sd[j] != 0)
#else
         if (sd[j] != 0 || !instance->isHorarioDisponivel(disciplina, j))
#endif
         {
            found = false;
            break;
         }
      }
      if (found)
      {
#ifndef ALLOW_INFEASIBLE_SLOT
         int hi = instance->getHorario(i);
         int hf = instance->getHorario(i+tempo-1);
         if (instance->isHorarioDisponivel(disciplina, dia, turma, hi, hf))
#endif
            return i;
      }
   }

   return -1;
}

int Solution::calculaInviabilidadesPorConflito()
{
   int inv = 0;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDisciplinas1 = numDisciplinas - 1;
   int numInstantes = instance->getNumeroInstantes();

   for (int t=0; t<numInstantes; t++)
   {
      int dia = instance->getDia(t);
      for (int d1=0; d1<numDisciplinas1; d1++)
      {
         int turma1 = solution[d1][t];
         if (turma1 <= 0)
            continue;
         int vt1 = turma1 - (*instance->getDisciplinas())[d1].refTurma;
         for (int d2=d1+1; d2<numDisciplinas; d2++)
         {
            int turma2 = solution[d2][t];
            if (turma2 <= 0)
               continue;
            int vt2 = turma2 - (*instance->getDisciplinas())[d2].refTurma;
            int numAlunos1 = (*instance->getDisciplinas())[d1].numAlunos[vt1];
            int numAlunos2 = (*instance->getDisciplinas())[d2].numAlunos[vt2];
            if (instance->hasConflito(turma1, turma2) || instance->hasConflito(dia, turma1, turma2))
               inv += numAlunos1 + numAlunos2;
         }
      }
   }

   return inv;
}

int Solution::calculaInviabilidadesPorConflito(int dia)
{
   int inv = 0;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDisciplinas1 = numDisciplinas - 1;
   int numInstantes = instance->getNumeroInstantes();

   int inicio = instance->getInstante(dia, 1);
   int fim = inicio + instance->getNumeroHorarios();

   for (int t=inicio; t<fim; t++)
   {
      for (int d1=0; d1<numDisciplinas1; d1++)
      {
         int turma1 = solution[d1][t];
         if (turma1 <= 0)
            continue;
         int vt1 = turma1 - (*instance->getDisciplinas())[d1].refTurma;
         for (int d2=d1+1; d2<numDisciplinas; d2++)
         {
            int turma2 = solution[d2][t];
            if (turma2 <= 0)
               continue;
            int vt2 = turma2 - (*instance->getDisciplinas())[d2].refTurma;
            int numAlunos1 = (*instance->getDisciplinas())[d1].numAlunos[vt1];
            int numAlunos2 = (*instance->getDisciplinas())[d2].numAlunos[vt2];
            if (instance->hasConflito(turma1, turma2) || instance->hasConflito(dia, turma1, turma2))
               inv += numAlunos1 + numAlunos2;
         }
      }
   }

   return inv;
}

int Solution::calculaFreeSlots()
{
   int freeslots = 0;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDias = instance->getNumeroDias();
   int numHorarios = instance->getNumeroHorarios();

   for (int d=0; d<numDisciplinas; d++)
   {
      int *sd = solution[d];
      int subtotal = 0;
      bool empty = true;
      for (int t=0; t<numDias; t++)
      {
         int instante = t * numHorarios;
         int count = 0;
         for (int h=1; h<numHorarios; h++)
         {
            int v1 = sd[instante];
            int v2 = sd[++instante];
            if (v1 > 0 || v2 > 0)
               empty = false;
            if (v2 == 0 && v2 == v1)
               subtotal += imp2weights[count++];
         }
      }
      if (!empty)
         freeslots += subtotal;
   }

   return freeslots;
}

int Solution::calculaFreeSlots(int dia)
{
   int freeslots = 0;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numHorarios = instance->getNumeroHorarios();
   int numInstantes = instance->getNumeroInstantes();

   for (int d=0; d<numDisciplinas; d++)
   {
      int *sd = solution[d];
      int count = 0;
      int instante = (dia - 1) * numHorarios;
      bool empty = true;
      for (int h=0; h<numInstantes; h++)
      {
         if (sd[h] > 0)
         {
            empty = false;
            break;
         }
      }
      if (!empty)
      {
         for (int h=1; h<numHorarios; h++)
         {
            int v1 = sd[instante];
            int v2 = sd[++instante];
            if (v2 == 0 && v2 == v1)
               freeslots += imp2weights[count++];
         }
      }
   }

   return freeslots;
}

std::string Solution::getFatorAtendimento()
{
   //std::pair<int, int> atendimento = getNumeroDemandasAtendidas();
   //std::pair<int, int> total = instance->getDemandaTotal();
   int numAlunos = getNumeroAlunosAtendidos();
   //double pdisc = ((double)atendimento.first / (double)total.first) * 100.0;
   //double pturm = ((double)atendimento.second / (double)total.second) * 100.0;
   double palun = ((double)numAlunos / (double)totalAlunos) * 100.0;

   //std::string out = "FA: { D: " + GUtil::intToString(atendimento.first, 4) + "/" + GUtil::intToString(total.first, 4) + " (" + GUtil::doubleToString(pdisc, 2) + "%) T: " + GUtil::intToString(atendimento.second, 4) + "/" + GUtil::intToString(total.second, 4) + " (" + GUtil::doubleToString(pturm, 2) + "%)";
   std::string out = "FA: { A: " + GUtil::intToString(numAlunos, 5) + "/" + GUtil::intToString(totalAlunos, 5) + " (" + GUtil::doubleToString(palun, 2) + "%) [" + GUtil::intToString(totalAlunos - numAlunos, 5) + "] }";

   return out;
}

std::string Solution::toString()
{
   std::string out = "[Solucao]\n - FO: " + GUtil::intToString(objectiveFunction.fo) + "\n - INV1: " + GUtil::intToString(objectiveFunction.inv1) + " [" + GUtil::intToString(objectiveFunction.lambda1) + + "]\n - INV2: " + GUtil::intToString(objectiveFunction.inv2) + " [" + GUtil::intToString(objectiveFunction.lambda2) + "]\n - IMP1: " + GUtil::intToString(objectiveFunction.imp1) + " [" + GUtil::intToString(objectiveFunction.beta1) + "]\n - IMP2: " + GUtil::intToString(objectiveFunction.imp2) + " [" + GUtil::intToString(objectiveFunction.beta2) + "]\n";

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();
   int numDias = instance->getNumeroDias();
   int numHorarios = instance->getNumeroHorarios();

   int colsize = 4;

   for (int d=0; d<numDisciplinas; d++)
   {
      out += " - Disciplina " + GUtil::intToString(d+1) + " [" + GUtil::intToString(d) + "]: ";
      for (std::hash_map<int, int>::iterator it=divisoes[d].begin(); it != divisoes[d].end(); ++it)
         out += "(" + GUtil::intToString(it->first) + " = " + GUtil::intToString(it->second) + ") ";
      out += "\n";
   }

   std::string line = "       +";
   for (int dia=0; dia<numDias; dia++)
      line += std::string(numHorarios * (colsize + 1) + 1, '-') + "+";
   line += "\n";

   out += "\n - Representacao:\n" + line + "    d: | ";
   for (int dia=0; dia<numDias; dia++)
   {
      out += GUtil::intToString(dia+1, colsize) + " ";
      for (int horario=0; horario<numHorarios-1; horario++)
         out += std::string(colsize+1, ' ');
      out += "| ";
   }
   out += "\n    h: | ";

   for (int dia=0; dia<numDias; dia++)
   {
      for (int horario=0; horario<numHorarios; horario++)
         out += GUtil::intToString(horario+1, colsize) + " ";
      out += "| ";
   }
   out += "\n    t: | ";

   for (int dia=1; dia<=numDias; dia++)
   {
      for (int horario=1; horario<=numHorarios; horario++)
      {
         int t = instance->getInstante(dia, horario);
         out += GUtil::intToString(t, colsize) + " ";
      }
      out += "| ";
   }

   out += "\n" + line;

   for (int d=0; d<numDisciplinas; d++)
   {
      out += "   " + GUtil::intToString(d+1, colsize) + " : ";

      for (int dia=1; dia<=numDias; dia++)
      {
         for (int horario=1; horario<=numHorarios; horario++)
         {
            int t = instance->getInstante(dia, horario);
            if (solution[d][t] < 0)
               out += std::string(colsize, '.') + " ";
            else
               out += GUtil::intToString(solution[d][t], colsize, ' ', ' ') + " ";
         }
         out += "| ";
      }
      out += "\n";
   }

   out += line + "\n";

   out += " - " + getFatorAtendimento() + "\n";

   return out;
}

Move* Solution::getRandomMove()
{
   //MoveType types[5] = { SHIFT, SWAP, SWAP_DIVISION, DOUBLE_SHIFT, DOUBLE_SWAP };
   //const int N = 3;
   MoveType type = SWAP_DIVISION;
   int p = rand() % 100;
   if (p < 30)
      type = SHIFT;
   else if (p < 60)
      type = SWAP;

   return getRandomMove(type);
}

Move* Solution::getRandomMove(MoveType type)
{
   switch (type)
   {
      case SHIFT: return getRandomShiftMove();
      case SWAP: return getRandomSwapMove();
      case SWAP_DIVISION: return getRandomSwapDivisionMove();
      case DOUBLE_SHIFT: return getRandomDoubleShiftMove();
      case DOUBLE_SWAP: return getRandomDoubleSwapMove();
      default: return getRandomShiftMove();
   }
}

Move* Solution::getRandomMove(int dia)
{
   MoveType types[2] = { SHIFT, SWAP };
   MoveType type = types[(rand() % 2)];
   return getRandomMove(type, dia);
}

Move* Solution::getRandomMove(MoveType type, int dia)
{
   switch (type)
   {
      case SHIFT: return getRandomShiftMove(dia);
      case SWAP: return getRandomSwapMove(dia);
      case DOUBLE_SHIFT: return getRandomDoubleShiftMove(dia);
      case DOUBLE_SWAP: return getRandomDoubleSwapMove(dia);
      default: return getRandomShiftMove(dia);
   }
}

Move* Solution::getRandomShiftMove()
{
#ifdef LOG_TIME   
   TimeLogger::start("getRandomShiftMove");
#endif

   ShiftMove* move = NULL;

   bool invalid = true;
   int count = 0;
   do
   {
      Slot slot = getRandomSlot();

      int disciplina = slot.disciplina;
      int dia = instance->getDia(slot.inicio);
      int tempo = slot.fim - slot.inicio + 1;
      int turma = solution[disciplina][slot.inicio];

      int free = getRandomFreeSlot(disciplina, dia, tempo, turma);

      if (free >= 0)
      {
         move = new ShiftMove();
         move->setDisciplina(disciplina);
         move->setTurma(turma);
         move->setInicio1(slot.inicio);
         move->setFim1(slot.fim);
         move->setInicio2(free);
         move->setFim2(free + tempo - 1);
         invalid = false;
      }
   } while (invalid && ++count < 50);

#ifdef LOG_TIME   
   TimeLogger::stop("getRandomShiftMove");

   if (move == NULL)
      TimeLogger::inc("getRandomShiftMove:NULL");
   else
      TimeLogger::inc("getRandomShiftMove:VALID");
#endif

   return (Move*)move;
}

Move* Solution::getRandomShiftMove(int dia)
{
   ShiftMove* move = NULL;

   int count = 0;

   bool invalid = true;
   do
   {
      Slot slot = getRandomSlot(-1, dia, 0);

      int disciplina = slot.disciplina;
      int tempo = slot.fim - slot.inicio + 1;
      int turma = solution[disciplina][slot.inicio];

      int free = getRandomFreeSlot(disciplina, dia, tempo, turma);

      if (free >= 0)
      {
         move = new ShiftMove();
         move->setDisciplina(disciplina);
         move->setTurma(turma);
         move->setInicio1(slot.inicio);
         move->setFim1(slot.fim);
         move->setInicio2(free);
         move->setFim2(free + tempo - 1);
         invalid = false;
      }
   } while (invalid && ++count < 50);

   return (Move*)move;
}

Move* Solution::getRandomDoubleShiftMove()
{
#ifdef LOG_TIME   
   TimeLogger::start("getRandomDoubleShiftMove");
#endif

   int dia = rand() % instance->getNumeroDias() + 1;
   Move* move = getRandomDoubleShiftMove(dia);

#ifdef LOG_TIME   
   TimeLogger::stop("getRandomDoubleShiftMove");

   if (move == NULL)
      TimeLogger::inc("getRandomDoubleShiftMove:NULL");
   else
      TimeLogger::inc("getRandomDoubleShiftMove:VALID");
#endif

   return move;
}

Move* Solution::getRandomDoubleShiftMove(int dia)
{
   DoubleShiftMove* move = NULL;

   int count = 0;

   do
   {
      ShiftMove* move1 = (ShiftMove*)getRandomShiftMove(dia);
      ShiftMove* move2 = (ShiftMove*)getRandomShiftMove(dia);

      if (move1 != NULL && move2 != NULL && move1->getDisciplina() != move2->getDisciplina())
      {
         move = new DoubleShiftMove();
         move->setMove1(move1);
         move->setMove2(move2);
         break;
      }

      if (move1 == NULL)
         delete move1;
      if (move2 == NULL)
         delete move2;
   } while (++count < 50);

   return (Move*)move;
}

Move* Solution::getRandomDoubleSwapMove()
{
#ifdef LOG_TIME   
   TimeLogger::start("getRandomDoubleSwapMove");
#endif

   int dia = rand() % instance->getNumeroDias() + 1;
   Move* move = getRandomDoubleSwapMove(dia);

#ifdef LOG_TIME   
   TimeLogger::stop("getRandomDoubleSwapMove");

   if (move == NULL)
      TimeLogger::inc("getRandomDoubleSwapMove:NULL");
   else
      TimeLogger::inc("getRandomDoubleSwapMove:VALID");
#endif

   return move;
}

Move* Solution::getRandomDoubleSwapMove(int dia)
{
   DoubleSwapMove* move = NULL;

   int count = 0;

   do
   {
      SwapMove* move1 = (SwapMove*)getRandomSwapMove(dia);
      SwapMove* move2 = (SwapMove*)getRandomSwapMove(dia);

      if (move1 != NULL && move2 != NULL && move1->getDisciplina() != move2->getDisciplina())
      {
         move = new DoubleSwapMove();
         move->setMove1(move1);
         move->setMove2(move2);
         break;
      }

      if (move1 == NULL)
         delete move1;
      if (move2 == NULL)
         delete move2;
   } while (++count < 50);

   return (Move*)move;
}

Slot Solution::getRandomSlot()
{
   return getRandomSlot(-1, -1, 0);
}

Slot Solution::getRandomSlot(int disciplina, int dia, int diff)
{
   Slot slot = { -1, -1, -1, true };

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numHorarios = instance->getNumeroHorarios();
   int numInstantes = instance->getNumeroInstantes();

   int st = (dia > 0 ? instance->getInstante(dia, 1) : 0);
   int md = (dia > 0 ? numHorarios : numInstantes);

   do
   {
      int d = (disciplina < 0 ? rand() % numDisciplinas : disciplina);
      int p = st + rand() % md;
      
      int turma = solution[d][p];

      if (turma > 0 && turma != diff)
      {
         slot.disciplina = d;
         slot.inicio = p;
         slot.fim = p;
         for (int i=p-1; i>=0; i--)
         {
            if (solution[d][i] == turma)
               slot.inicio = i;
            else
               break;
         }
         for (int j=p+1; j<numInstantes; j++)
         {
            if (solution[d][j] == turma)
               slot.fim = j;
            else
               break;
         }
         slot.blank = false;
      }
      
   } while (slot.blank);

   return slot;
}

int Solution::getRandomFreeSlot(int disciplina, int dia, int tempo, int turma)
{
   int start = instance->getInstante(dia, 1);
   int range = instance->getNumeroHorarios() - tempo + 1;

   int* sd = solution[disciplina];

   bool* alloc = new bool[range+1];
   GUtil::fill(alloc, range, true);

   do
   {
      int pos = rand() % range;
      if (alloc[pos])
      {
         alloc[pos] = false;
         if (sd[start+pos] == 0 || sd[start+pos] == turma)
         {
            int hi = instance->getHorario(start+pos);
            int hf = instance->getHorario(start+pos+tempo-1);
#ifndef ALLOW_INFEASIBLE_SLOT
            if (instance->isHorarioDisponivel(disciplina, dia, turma, hi, hf))
            {
#endif
               bool free = true;
               for (int k=1; k<tempo; k++)
               {
                  if (pos + k < range + 1)
                     alloc[pos+k] = false;
                  if (sd[start+pos+k] != 0 && sd[start+pos+k] != turma)
                  {
                     free = false;
                     break;
                  }
               }
               if (free)
               {
                  delete [] alloc;
                  return (start + pos);
               }
#ifndef ALLOW_INFEASIBLE_SLOT
            }
#endif
         }
      }
   } while (GUtil::or(alloc, range));

   delete [] alloc;

   return -1;
}

void Solution::clearslot(int disciplina, int inicio, int fim)
{
   setslot(disciplina, inicio, fim, 0);
}

void Solution::setslot(int disciplina, int inicio, int fim, int turma)
{
   int* s = solution[disciplina];
   int end = fim + 1;
   for (int i=inicio; i<end; i++)
      s[i] = turma;
}

void Solution::shift(ShiftMove* move)
{
   if (move == NULL)
      return;

   clearslot(move->getDisciplina(), move->getInicio1(), move->getFim1());
   setslot(move->getDisciplina(), move->getInicio2(), move->getFim2(), move->getTurma());
}

void Solution::backShift(ShiftMove* move)
{
   if (move == NULL)
      return;

   clearslot(move->getDisciplina(), move->getInicio2(), move->getFim2());
   setslot(move->getDisciplina(), move->getInicio1(), move->getFim1(), move->getTurma());
}

void Solution::swap(SwapMove* move)
{
   if (move == NULL)
      return;

   int disciplina = move->getDisciplina();
   clearslot(disciplina, move->getInicio1(), move->getFim1());
   clearslot(disciplina, move->getInicio2(), move->getFim2());
   setslot(disciplina, move->getInicio1D(), move->getFim1D(), move->getTurma1());
   setslot(disciplina, move->getInicio2D(), move->getFim2D(), move->getTurma2());
}

void Solution::backSwap(SwapMove* move)
{
   if (move == NULL)
      return;

   int disciplina = move->getDisciplina();
   clearslot(disciplina, move->getInicio1D(), move->getFim1D());
   clearslot(disciplina, move->getInicio2D(), move->getFim2D());
   setslot(disciplina, move->getInicio1(), move->getFim1(), move->getTurma1());
   setslot(disciplina, move->getInicio2(), move->getFim2(), move->getTurma2());
}

void Solution::doubleShift(DoubleShiftMove* move)
{
   if (move == NULL)
      return;

   shift(move->getMove1());
   shift(move->getMove2());
}

void Solution::backDoubleShift(DoubleShiftMove* move)
{
   if (move == NULL)
      return;

   backShift(move->getMove2());
   backShift(move->getMove1());
}

void Solution::doubleSwap(DoubleSwapMove* move)
{
   if (move == NULL)
      return;

   swap(move->getMove1());
   swap(move->getMove2());
}

void Solution::backDoubleSwap(DoubleSwapMove* move)
{
   if (move == NULL)
      return;

   backSwap(move->getMove2());
   backSwap(move->getMove1());
}

Move* Solution::getRandomSwapMove()
{
#ifdef LOG_TIME   
   TimeLogger::start("getRandomSwapMove");
#endif

   SwapMove* move = NULL;

   bool invalid = true;

   int count = 0;

   do
   {
      Slot slot1 = getRandomSlot();

      int disciplina = slot1.disciplina;
      int dia = instance->getDia(slot1.inicio);
      int turma1 = solution[disciplina][slot1.inicio];

      if (!hasSwapMove(disciplina, dia, turma1))
      {
         count++;
         continue;
      }

      Slot slot2 = getRandomSlot(disciplina, dia, turma1);

      int turma2 = solution[disciplina][slot2.inicio];

      if (turma1 == turma2)
      {
         count++;
         continue;
      }

      int tempo1 = slot1.fim - slot1.inicio + 1;
      int tempo2 = slot2.fim - slot2.inicio + 1;

      clearslot(disciplina, slot1.inicio, slot1.fim);
      clearslot(disciplina, slot2.inicio, slot2.fim);

      int free1 = -1;
      int free2 = -1;

      do
      {
         count++;
         free1 = getRandomFreeSlot(disciplina, dia, tempo1, turma1);
         if (free1 >= 0)
         {
            setslot(disciplina, free1, free1+tempo1-1, turma1);
            free2 = getRandomFreeSlot(disciplina, dia, tempo2, turma2);
            clearslot(disciplina, free1, free1+tempo1-1);
         }
      } while ((free1 < 0 || free2 < 0) && count < 50);

      setslot(disciplina, slot1.inicio, slot1.fim, turma1);
      setslot(disciplina, slot2.inicio, slot2.fim, turma2);

      if (free1 >= 0 && free2 >= 0)
      {
         move = new SwapMove();
         move->setDisciplina(disciplina);
         move->setTurma1(turma1);
         move->setInicio1(slot1.inicio);
         move->setFim1(slot1.fim);
         move->setInicio1D(free1);
         move->setFim1D(free1+tempo1-1);
         move->setTurma2(turma2);
         move->setInicio2(slot2.inicio);
         move->setFim2(slot2.fim);
         move->setInicio2D(free2);
         move->setFim2D(free2+tempo2-1);
         invalid = false;
      }
   } while (invalid && count < 50);

#ifdef LOG_TIME   
   TimeLogger::stop("getRandomSwapMove");

   if (move == NULL)
      TimeLogger::inc("getRandomSwapMove:NULL");
   else
      TimeLogger::inc("getRandomSwapMove:VALID");
#endif

   return (Move*)move;
}

Move* Solution::getRandomSwapMove(int dia)
{
   SwapMove* move = NULL;

   bool invalid = true;

   int count = 0;

   do
   {
      Slot slot1 = getRandomSlot(-1, dia, 0);

      int disciplina = slot1.disciplina;
      int turma1 = solution[disciplina][slot1.inicio];

      if (!hasSwapMove(disciplina, dia, turma1))
      {
         count++;
         continue;
      }

      Slot slot2 = getRandomSlot(disciplina, dia, turma1);

      int turma2 = solution[disciplina][slot2.inicio];

      if (turma1 == turma2)
      {
         count++;
         continue;
      }

      int tempo1 = slot1.fim - slot1.inicio + 1;
      int tempo2 = slot2.fim - slot2.inicio + 1;

      clearslot(disciplina, slot1.inicio, slot1.fim);
      clearslot(disciplina, slot2.inicio, slot2.fim);

      int free1;
      int free2;

      do
      {
         count++;
         free1 = getRandomFreeSlot(disciplina, dia, tempo1, turma1);
         if (free1 >= 0)
         {
            setslot(disciplina, free1, free1+tempo1-1, turma1);
            free2 = getRandomFreeSlot(disciplina, dia, tempo2, turma2);
            clearslot(disciplina, free1, free1+tempo1-1);
         }
      } while ((free1 < 0 || free2 < 0) && count < 50);

      setslot(disciplina, slot1.inicio, slot1.fim, turma1);
      setslot(disciplina, slot2.inicio, slot2.fim, turma2);

      if (free1 >= 0 && free2 >= 0)
      {
         move = new SwapMove();
         move->setDisciplina(disciplina);
         move->setTurma1(turma1);
         move->setInicio1(slot1.inicio);
         move->setFim1(slot1.fim);
         move->setInicio1D(free1);
         move->setFim1D(free1+tempo1-1);
         move->setTurma2(turma2);
         move->setInicio2(slot2.inicio);
         move->setFim2(slot2.fim);
         move->setInicio2D(free2);
         move->setFim2D(free2+tempo2-1);
         invalid = false;
      }
   } while (invalid && count < 50);

   return (Move*)move;
}

bool Solution::hasSwapMove()
{
   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDias = instance->getNumeroDias();
   int numHorarios = instance->getNumeroHorarios();

   for (int d=0; d<numDisciplinas; d++)
   {
      for (int t=1; t<=numDias; t++)
      {
         int turma = -1;
         for (int h=1; h<=numHorarios; h++)
         {
            int k = instance->getInstante(t, h);
            if (solution[d][k] > 0)
            {
               if (turma == -1)
                  turma = solution[d][k];
               else if (turma != solution[d][k])
                  return true;
            }
         }
      }
   }
   return false;
}

void Solution::multimrd(int maxIter, int mrdMaxIter)
{
   int iter = 0;

   std::cout << "Starting Multi Start VRND...\n";

   while (iter++ < maxIter)
   {
      Solution* current = SolutionBuilder::createRandomSolution(instance, 0.0);
      current->mrd(mrdMaxIter);

      std::cout << "[" << GUtil::intToString(iter, 3) << "/" << GUtil::intToString(maxIter, 3) << "]: FO = " << GUtil::intToString(current->getObjectiveFunction().fo, 6) << " BEST = " << GUtil::intToString(objectiveFunction.fo, 6) << " " << current->getFatorAtendimento();

      int fa1 = this->getNumeroAlunosAtendidos();
      int fa2 = current->getNumeroAlunosAtendidos();

      if (fa2 > fa1 || (fa1 == fa2 && *current < *this))
      {
         iter = 0;
         *this = *current;
         std::cout << " {improved}";
      }

      std::cout << "\n";

      delete current;
   }

   std::cout << "Done (Multi MRD).\n";
}

void Solution::multistart(int maxIter, int lsMaxIter, clock_t startTime, double timeLimit)
{
   int iter = 0;

   std::cout << "Starting Multi Start ILS...\n";

   while (iter++ < maxIter)
   {
      Solution* current = SolutionBuilder::createRandomSolution(instance, 0.0);
      current->ils2(lsMaxIter, 1, 1, 1, 200, 0.0, true, startTime, timeLimit);
      current->makeFeasible();
      current->improve(0);

      std::cout << "[" << GUtil::intToString(iter, 3) << "/" << GUtil::intToString(maxIter, 3) << "]: FO = " << GUtil::intToString(current->getObjectiveFunction().fo, 6) << " BEST = " << GUtil::intToString(objectiveFunction.fo, 6) << " " << current->getFatorAtendimento();

      if (*current < *this)
      {
         iter = 0;
         *this = *current;
         std::cout << " {improved}";
      }

      std::cout << "\n";

      delete current;
   }

   std::cout << "Done (Multi Start ILS).\n";
}

void Solution::mrd(int maxIter)
{
   mrd(maxIter, 0, 0.0);
}

void Solution::dailyMRD(int maxIter)
{
   int numDias = instance->getNumeroDias();
   for (int dia=1; dia<=numDias; dia++)
      mrd(maxIter, dia);
}

void Solution::enhancedMRD(int maxIter)
{
   int best;
   do
   {
      dailyMRD(maxIter);

      best = objectiveFunction.fo;

      int iter = 0;

      while (iter++ < maxIter)
      {
         Move* move = getRandomMove(SWAP_DIVISION);
         this->move(move);

         int fo = objectiveFunction.fo;

         if (fo < best)
         {
            if (move != NULL)
               delete move;
            break;
         }
         else
            back(move);

         if (move != NULL)
            delete move;
      }
   } while (objectiveFunction.fo < best);
}

void Solution::mrd(int maxIter, int maxSide, double sideFactor)
{
   int iter = 0;
   int side = 0;

   //std::cout << "Starting MRD (" << maxIter << ")...\n";

   while (iter++ < maxIter)
   {
      int best = objectiveFunction.fo;

      Move* move = getRandomMove();
      this->move(move);

      int fo = objectiveFunction.fo;

      //std::cout << "[" << GUtil::intToString(iter, 3) << "/" << GUtil::intToString(maxIter, 3) << "]: FO = " << fo << "\t BEST = " << best << "\t [" << (move == NULL ? "NULL" : (move->getType() == SHIFT ? "SHIFT" : (move->getType() == SWAP ? "SWAP" : "SWAP_DIVISION"))) << "] ";

      if (fo < best)
      {
#ifdef LOG_TIME   
         TimeLogger::inc((move->getType() == SHIFT ? "SHIFT" : (move->getType() == SWAP ? "SWAP" : "SWAP_DIVISION")));
#endif

         iter = 0;
         side = 0;
         //std::cout << "{improved}";
      }
      else if (fo == best && side < maxSide)
      {
         side++;
         iter = (int)(sideFactor * (double)(iter));
         //std::cout << "{side:" << side << "/" << iter << "}";
      }
      else
         back(move);

      //std::cout << "\n";

      if (move != NULL)
         delete move;
   }

   //std::cout << "Done MRD.\n";
}

void Solution::mrd(int maxIter, int dia)
{
   int iter = 0;

   while (iter++ < maxIter)
   {
      int best = objectiveFunction.fo;

      Move* move = getRandomMove(dia);
      this->move(move);

      int fo = objectiveFunction.fo;

      if (fo < best)
         iter = 0;
      else
         back(move);

      if (move != NULL)
         delete move;
   }
}

void Solution::dailyVRND(int maxIter)
{
   int numDias = instance->getNumeroDias();
   for (int dia=1; dia<=numDias; dia++)
      vrnd(maxIter, dia);
}

void Solution::vrnd(int maxIter)
{
   int k = 0;
   MoveType NS[5] = { SWAP, SHIFT, SWAP_DIVISION, DOUBLE_SHIFT, DOUBLE_SWAP };
   int N = 3;

   for (int k=0; k<15; k++)
   {
      int i = k % N;
      int j = rand() % N;
      MoveType aux = NS[i];
      NS[i] = NS[j];
      NS[j] = aux;
   }

   //std::cout << "Starting VNRD (" << maxIter << ")...\n";

   while (k < N)
   {
      int iter = 0;
      while (iter++ < maxIter)
      {
         int best = objectiveFunction.fo;

         Move* move = getRandomMove(NS[k]);
         this->move(move);

         int fo = objectiveFunction.fo;

         //std::cout << "VRND:[" << k+1 << "/" << N << "] [" << GUtil::intToString(iter, 3) << "/" << GUtil::intToString(maxIter, 3) << "]: FO = " << fo << "\t BEST = " << best << "\t [" << (move->getType() == SHIFT ? "SHIFT" : (move->getType() == SWAP ? "SWAP" : "SWAP_DIVISION")) << "] ";

         if (fo < best)
         {
#ifdef LOG_TIME   
         TimeLogger::inc((move->getType() == SHIFT ? "SHIFT" : (move->getType() == SWAP ? "SWAP" : "SWAP_DIVISION")));
#endif

            //std::cout << "VRND:[" << k+1 << "/" << N << "] [" << GUtil::intToString(iter, 3) << "/" << GUtil::intToString(maxIter, 3) << "]: FO = " << fo << "\t BEST = " << best << "\t [" << (move->getType() == SHIFT ? "SHIFT" : (move->getType() == SWAP ? "SWAP" : "SWAP_DIVISION")) << "] ";
            //std::cout << "{improved}";
            //std::cout << "\n";
            iter = 0;
            k = 0;
         }
         else
            back(move);

         //std::cout << "\n";

         if (move != NULL)
            delete move;
      }
      k++;
   }

   //std::cout << "Done VRND.\n";
}

void Solution::vrnd(int maxIter, int dia)
{
   int k = 0;
   MoveType NS[2] = { SHIFT, SWAP };
   int N = 2;

   while (k < N)
   {
      int iter = 0;
      while (iter++ < maxIter)
      {
         int best = objectiveFunction.fo;

         Move* move = getRandomMove(NS[k], dia);
         this->move(move);

         int fo = objectiveFunction.fo;

         if (fo < best)
         {
            iter = 0;
            k = 0;
         }
         else
            back(move);

         if (move != NULL)
            delete move;
      }
      k++;
   }
}

void Solution::enhancedVRND(int maxIter)
{
   int best;
   do
   {
      dailyVRND(maxIter);

      best = objectiveFunction.fo;

      int iter = 0;

      while (iter++ < maxIter)
      {
         Move* move = getRandomMove(SWAP_DIVISION);
         this->move(move);

         int fo = objectiveFunction.fo;

         if (fo < best)
         {
            if (move != NULL)
               delete move;
            break;
         }
         else
            back(move);

         if (move != NULL)
            delete move;
      }
   } while (objectiveFunction.fo < best);
}

Slot Solution::getSlot(int disciplina, int dia, int turma)
{
   Slot slot = { -1, -1, -1, true };

   int inicio = instance->getInstante(dia, 1);
   int fim = inicio + instance->getNumeroHorarios();

   int *sd = solution[disciplina];

   for (int h=inicio; h<fim; h++)
   {
      if (sd[h] == turma)
      {
         slot.inicio = h;
         break;
      }
   }

   if (slot.inicio != -1)
   {
      for (int h=slot.inicio; h<fim; h++)
      {
         if (sd[h] != turma)
         {
            slot.fim = h - 1;
            slot.blank = false;
            break;
         }
      }
      if (slot.fim == -1)
      {
         slot.fim = fim - 1;
         slot.blank = false;
      }
   }

   return slot;
}

void Solution::ils(int maxIter, int kpMax, int kpStart, int kpInc, int mrdMaxIter, double incFactor, bool runUntilInfeasible, clock_t startTime, double timeLimit)
{
#ifdef LOG_TIME   
   TimeLogger::start("ILS");
#endif
   int kp = kpStart;

   mrd(mrdMaxIter);
   
   Solution* current = this->clone();
   //Solution* best = this->clone();

   int bestMipObjVal = current->getMIPObjVal();

   int totalIter = 0;

   while (kp <= kpMax)
   {
      double time = GUtil::getDiffClock(clock(), startTime);
      if (time > timeLimit)
         break;

      if (runUntilInfeasible && feasible())
         break;

      int iter = 0;
      int max = (int)((double)maxIter * (1.0 + incFactor * (double)(kp - 1)));
      while (iter++ < max)
      {
         time = GUtil::getDiffClock(clock(), startTime);

         if (time > timeLimit)
            break;

         if (runUntilInfeasible && feasible())
            break;

         totalIter++;
#ifdef LOG_TIME   
         TimeLogger::start("ILS-Perturbation");
#endif
         //current->ilsPerturbation(kp);
         MoveType type = UNKNOWN;
         Move* move = current->getRandomMove();
         current->move(move);
         if (move != NULL)
         {
            type = move->getType();
            delete move;
         }

#ifdef LOG_TIME   
         TimeLogger::stop("ILS-Perturbation");
#endif

#ifdef LOG_TIME   
         TimeLogger::start("ILS-LS");
#endif
         //current->vrnd(mrdMaxIter);
         current->mrd(mrdMaxIter);
         //current->md(startTime, timeLimit);
#ifdef LOG_TIME   
         TimeLogger::stop("ILS-LS");
#endif

         //std::cout << "ILS:[" << GUtil::intToString(kp, 2) << "/" << GUtil::intToString(kpMax, 2) << "][" << GUtil::intToString(iter, 4) << "/" << GUtil::intToString(max, 4) << "][" << GUtil::intToString(totalIter, 4) << "]: CURRENT = " << GUtil::intToString(current->getObjectiveFunction().fo, 8) << "]: FO = " << GUtil::intToString(getObjectiveFunction().fo, 8) << " { INV1: " << GUtil::intToString(objectiveFunction.inv1, 4) << " } Elapsed Time: " << GUtil::doubleToString(time, 2) << " sec.\n";

         if (*current < *this)
         {
#ifdef LOG_TIME   
         TimeLogger::inc((move->getType() == SHIFT ? "ILS-Perturbation:SHIFT" : (move->getType() == SWAP ? "ILS-Perturbation:SWAP" : "ILS-Perturbation:SWAP_DIVISION")));
#endif
            
            int mipObjVal = current->getMIPObjVal();
            *this = *current;

#ifdef DISPLAY_ILS
            double elapsedTime = GUtil::getDiffClock(clock(), startTime);
            std::cout << "ILS:[" << GUtil::intToString(kp, 2) << "/" << GUtil::intToString(kpMax, 2) << "][" << GUtil::intToString(iter, 4) << "/" << GUtil::intToString(max, 4) << "][" << GUtil::intToString(totalIter, 4) << "]: FO = " << GUtil::intToString(current->getObjectiveFunction().fo, 8) << " { INV1: " << GUtil::intToString(objectiveFunction.inv1, 4) << " INV2: " << GUtil::intToString(objectiveFunction.inv2, 4) << " } MIP Obj Val: " << GUtil::intToString(mipObjVal, 4)  <<  " Elapsed Time: " << GUtil::doubleToString(elapsedTime, 2) << " sec.\n";

#endif
#ifdef LOG_TIME   
            TimeLogger::summary();
#endif

            kp = kpStart;
            iter = 0;
         }
         else
         {
            *current = *this;
         }
      }
      kp += kpInc;
   }

#ifdef LOG_TIME   
   TimeLogger::stop("ILS");
#endif

   delete current;
}

void Solution::ils2(int maxIter, int kpMax, int kpStart, int kpInc, int mrdMaxIter, double incFactor, bool runUntilInfeasible, clock_t startTime, double timeLimit)
{
   int kp = kpStart;

   vrnd(mrdMaxIter);
   
   Solution* current = this->clone();

   int totalIter = 0;

   while (kp <= kpMax)
   {
      double time = GUtil::getDiffClock(clock(), startTime);
      if (time > timeLimit)
         break;

      if (runUntilInfeasible && feasible())
         break;

      int iter = 0;
      int max = (int)((double)maxIter * (1.0 + incFactor * (double)(kp - 1)));
      while (iter++ < max)
      {
         time = GUtil::getDiffClock(clock(), startTime);

         if (time > timeLimit)
            break;

         if (runUntilInfeasible && feasible())
            break;

         totalIter++;

         bool oscilation = (iter > 0.7 * max);

         if (oscilation)
            current->unalloc();
         else
            current->ilsPerturbation(kp);
         if (rand() % 2 == 0)
            current->vrnd(mrdMaxIter);
         else
            current->mrd(mrdMaxIter);

         //std::cout << "ILS:[" << GUtil::intToString(kp, 2) << "/" << GUtil::intToString(kpMax, 2) << "][" << GUtil::intToString(iter, 4) << "/" << GUtil::intToString(max, 4) << "][" << GUtil::intToString(totalIter, 4) << "]: CURRENT = " << GUtil::intToString(current->getObjectiveFunction().fo, 8) << "]: FO = " << GUtil::intToString(getObjectiveFunction().fo, 8) << " { INV1: " << GUtil::intToString(objectiveFunction.inv1, 4) << " } Elapsed Time: " << GUtil::doubleToString(time, 2) << " sec.\n";

         if (*current < *this)
         {
            *this = *current;
            if (oscilation)
               improve(this->getObjectiveFunction().fo);
#ifdef DISPLAY_ILS
            double elapsedTime = GUtil::getDiffClock(clock(), startTime);
            std::cout << "ILS:[" << GUtil::intToString(kp, 2) << "/" << GUtil::intToString(kpMax, 2) << "][" << GUtil::intToString(iter, 4) << "/" << GUtil::intToString(max, 4) << "][" << GUtil::intToString(totalIter, 4) << "]: FO = " << GUtil::intToString(current->getObjectiveFunction().fo, 8) << " { INV1: " << GUtil::intToString(objectiveFunction.inv1, 4) << " } Elapsed Time: " << GUtil::doubleToString(elapsedTime, 2) << " sec.\n";
#endif
            kp = kpStart;
            iter = 0;
         }
         else
            *current = *this;
      }
      kp += kpInc;
   }

   delete current;
}

void Solution::mathILS(int maxIter, bool runUntilInfeasible, SolverTatico* solver, double& elapsedTime)
{
   solver->solve(this);
   
   Solution* current = this->clone();

   int totalIter = 0;
   
   CPUTimer timer;

   timer.start();

   int iter = 0;
   while (iter++ < maxIter)
   {
      if (runUntilInfeasible && feasible())
         break;
      totalIter++;
      
      Move* move = current->getRandomMove(SWAP_DIVISION);
      current->move(move);
      if (move != NULL)
         delete move;

      solver->solve(current);

      if (*current < *this)
      {
         *this = *current;
         timer.stop();
         elapsedTime += timer.getCronoCurrSecs();
         std::cout << "ILS:[" << GUtil::intToString(iter, 4) << "/" << GUtil::intToString(maxIter, 4) << "][" << GUtil::intToString(totalIter, 4) << "]: FO = " << GUtil::intToString(current->getObjectiveFunction().fo, 8) << " { INV1: " << GUtil::intToString(objectiveFunction.inv1, 4) << " } Elapsed Time: " << GUtil::doubleToString(elapsedTime, 2) << " sec.\n";
         timer.start();
         iter = 0;
      }
      else
         *current = *this;
   }

   timer.stop();
   elapsedTime += timer.getCronoCurrSecs();

   delete current;
}

bool Solution::hasSwapMove(int disciplina, int dia, int turma)
{
   int start = instance->getInstante(dia, 1);
   int end = start + instance->getNumeroHorarios();
   int* sd = solution[disciplina];

   for (int h=start; h<end; h++)
      if (sd[h] > 0 && sd[h] != turma)
         return true;
   return false;
}

Move* Solution::getRandomSwapDivisionMove()
{
#ifdef LOG_TIME   
   TimeLogger::start("getRandomSwapDivisionMove");
#endif

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();

   SwapDivisionMove* move = new SwapDivisionMove();

   bool invalid = true;
   int count = 0;

   do
   {
      int disciplina = rand() % numDisciplinas;
      HDisciplina& disc = (*instance->getDisciplinas())[disciplina];

      if (disc.divisoes.size() > 1)
      {
         int turma = rand() % disc.numTurmas + disc.refTurma;
         int divisao1 = getDivisao(disciplina, turma);
         int divisao2 = GUtil::random(disc.divisoes.size(), divisao1);

         std::vector<int>& inicio1 = move->getInicio1();
         std::vector<int>& inicio2 = move->getInicio2();
         std::vector<int>& fim1 = move->getFim1();
         std::vector<int>& fim2 = move->getFim2();

         inicio1.clear();
         inicio2.clear();
         fim1.clear();
         fim2.clear();

         bool valid = true;

         Divisao& div1 = disc.divisoes[divisao1];
         for (int k=0; k<(int)div1.dia.size(); k++)
         {
            int dia = div1.dia[k];
            Slot slot = getSlot(disciplina, dia, turma);
            if (slot.blank)
            {
               valid = false;
               break;
            }
            inicio1.push_back(slot.inicio);
            fim1.push_back(slot.fim);
         }

         if (!valid)
         {
            count++;
            continue;
         }

         Divisao& div2 = disc.divisoes[divisao2];
         for (int k=0; k<(int)div2.dia.size(); k++)
         {
            int dia = div2.dia[k];
            int tempo = div2.creditos[k];
            int free = getRandomFreeSlot(disciplina, dia, tempo, turma);
            if (free < 0)
            {
               valid = false;
               break;
            }
            inicio2.push_back(free);
            fim2.push_back(free + tempo - 1);
         }

         if (valid)
         {
            move->setDisciplina(disciplina);
            move->setTurma(turma);
            move->setDivisao1(divisao1);
            move->setDivisao2(divisao2);
            invalid = false;
         }
      }
   } while (invalid && count++ < 10);

   if (invalid)
   {
      delete move;
      return NULL;
   }

#ifdef LOG_TIME   
   TimeLogger::stop("getRandomSwapDivisionMove");

   if (move == NULL)
      TimeLogger::inc("getRandomSwapDivisionMove:NULL");
   else
      TimeLogger::inc("getRandomSwapDivisionMove:VALID");
#endif

   return (Move*)move;
}

void Solution::swapDivision(SwapDivisionMove* move)
{
   if (move == NULL)
      return;

   int disciplina = move->getDisciplina();
   int turma = move->getTurma();

   std::vector<int>& inicio1 = move->getInicio1();
   std::vector<int>& inicio2 = move->getInicio2();
   std::vector<int>& fim1 = move->getFim1();
   std::vector<int>& fim2 = move->getFim2();

   if (inicio1.size() != fim1.size() || inicio2.size() != fim2.size())
      return;

   divisoes[disciplina][turma] = move->getDivisao2();

   for (int k=0; k<(int)inicio1.size(); k++)
      clearslot(disciplina, inicio1[k], fim1[k]);

   for (int k=0; k<(int)inicio2.size(); k++)
      setslot(disciplina, inicio2[k], fim2[k], turma);
}

void Solution::backSwapDivision(SwapDivisionMove* move)
{
   if (move == NULL)
      return;

   int disciplina = move->getDisciplina();
   int turma = move->getTurma();

   std::vector<int>& inicio1 = move->getInicio1();
   std::vector<int>& inicio2 = move->getInicio2();
   std::vector<int>& fim1 = move->getFim1();
   std::vector<int>& fim2 = move->getFim2();

   if (inicio1.size() != fim1.size() || inicio2.size() != fim2.size())
      return;

   divisoes[disciplina][turma] = move->getDivisao1();

   for (int k=0; k<(int)inicio2.size(); k++)
      clearslot(disciplina, inicio2[k], fim2[k]);

   for (int k=0; k<(int)inicio1.size(); k++)
      setslot(disciplina, inicio1[k], fim1[k], turma);
}

void Solution::ilsPerturbation(int kp)
{
   for (int i=0; i<kp; i++)
   {
      Move* move = getRandomMove();
      this->move(move);
      if (move != NULL)
         delete move;
   }
}

std::pair<int, int> Solution::getAlocacaoInviavel()
{
   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDisciplinas1 = numDisciplinas - 1;
   int numInstantes = instance->getNumeroInstantes();
   int numDias = instance->getNumeroDias();

   int disciplina = -1;
   int turma = -1;

   std::map<std::pair<int, int>, int> inviabilidades;

   for (int dia=1; dia<=numDias; dia++)
   {
      int inicio = instance->getInstante(dia, 1);
      int fim = inicio + instance->getNumeroHorarios();

      for (int t=inicio; t<fim; t++)
      {
         for (int d1=0; d1<numDisciplinas1; d1++)
         {
            int turma1 = solution[d1][t];
            if (turma1 <= 0)
               continue;
            int vt1 = turma1 - (*instance->getDisciplinas())[d1].refTurma;
            for (int d2=d1+1; d2<numDisciplinas; d2++)
            {
               int turma2 = solution[d2][t];
               if (turma2 <= 0)
                  continue;
               int vt2 = turma2 - (*instance->getDisciplinas())[d2].refTurma;

               if (instance->hasConflito(turma1, turma2) || instance->hasConflito(dia, turma1, turma2))
               {
                  disciplina = d2;
                  turma = turma2;

                  inviabilidades[std::pair<int, int>(disciplina, turma)]++;

                  //break;
               }
            }
         }
      }

      if (disciplina == -1)
      {
         for (int d=0; d<numDisciplinas; d++)
         {
            int h = instance->getInstante(dia, 1);

            int* sd = solution[d];

            while (h < numInstantes)
            {
               Slot slot = readNextSlot(d, h);
               if (slot.blank)
                  break;

               if (instance->getDia(slot.inicio) != dia || instance->getDia(slot.fim) != dia)
                  break;

               int t = sd[slot.inicio];
               int hi = instance->getHorario(slot.inicio);
               int hf = instance->getHorario(slot.fim);

               if (!instance->isHorarioDisponivel(d, dia, t, hi, hf))
               {
                  disciplina = d;
                  turma = t;

                  inviabilidades[std::pair<int, int>(disciplina, turma)]++;

                  //break;
               }

               bool infeasible = false;
               for (int h1=slot.inicio; h1<=slot.fim; h1++)
               {
                  if (!instance->isHorarioDisponivel(d, h1))
                  {
                     infeasible = true;
                     break;
                  }
               }

               if (infeasible)
               {
                  disciplina = d;
                  turma = t;

                  inviabilidades[std::pair<int, int>(disciplina, turma)]++;

                  //break;
               }

               h = slot.fim + 1;
            }
         }
      }
   }

   int max = 0;
   std::map<std::pair<int, int>, int>::iterator it = inviabilidades.begin();
   for (std::map<std::pair<int, int>, int>::iterator it1 = inviabilidades.begin(); it1 != inviabilidades.end(); ++it1)
   {
      if (it1->second > max)
      {
         max = it1->second;
         it = it1;
      }
   }
   
   //return std::pair<int, int>(disciplina, turma);
   return it->first;
}

std::pair<int, int> Solution::getMaiorInviabilidadePorConflito()
{
   std::map<std::pair<int, int>, int> tinvs;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDisciplinas1 = numDisciplinas - 1;
   int numInstantes = instance->getNumeroInstantes();

   for (int t=0; t<numInstantes; t++)
   {
      int dia = instance->getDia(t);
      for (int d1=0; d1<numDisciplinas1; d1++)
      {
         int turma1 = solution[d1][t];
         if (turma1 <= 0)
            continue;
         int vt1 = turma1 - (*instance->getDisciplinas())[d1].refTurma;
         for (int d2=d1+1; d2<numDisciplinas; d2++)
         {
            int turma2 = solution[d2][t];
            if (turma2 <= 0)
               continue;
            int vt2 = turma2 - (*instance->getDisciplinas())[d2].refTurma;
            int numAlunos1 = (*instance->getDisciplinas())[d1].numAlunos[vt1];
            int numAlunos2 = (*instance->getDisciplinas())[d2].numAlunos[vt2];
            if (instance->hasConflito(turma1, turma2))
            {
               std::pair<int, int> key1(d1, turma1);
               std::pair<int, int> key2(d2, turma2);
               if (tinvs.find(key1) == tinvs.end())
                  tinvs[key1] = numAlunos1;
               else
                  tinvs[key1] += numAlunos1;
               if (tinvs.find(key2) == tinvs.end())
                  tinvs[key2] = numAlunos2;
               else
                  tinvs[key2] = numAlunos2;
            }
            if (instance->hasConflito(dia, turma1, turma2))
            {
               std::pair<int, int> key1(d1, turma1);
               std::pair<int, int> key2(d2, turma2);
               if (tinvs.find(key1) == tinvs.end())
                  tinvs[key1] = numAlunos1;
               else
                  tinvs[key1] += numAlunos1;
               if (tinvs.find(key2) == tinvs.end())
                  tinvs[key2] = numAlunos2;
               else
                  tinvs[key2] = numAlunos2;
            }
         }
      }
   }

   std::map<std::pair<int, int>, int>::iterator itmax = tinvs.end();

   for (std::map<std::pair<int, int>, int>::iterator it = tinvs.begin(); it != tinvs.end(); ++it)
   {
      if (itmax == tinvs.end() || itmax->second > it->second)
         itmax = it;
   }

   if (itmax != tinvs.end())
      return itmax->first;
   return std::pair<int, int>(-1, -1);
}

std::pair<int, int> Solution::getMaiorInviabilidadeHorariosIndisponiveis()
{
   std::map<std::pair<int, int>, int> tinvs;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();

   for (int d=0; d<numDisciplinas; d++)
   {
      int h = 0;
      int* sd = solution[d];

      while (h < numInstantes)
      {
         Slot slot = readNextSlot(d, h);
         if (slot.blank)
            break;

         int turma = sd[slot.inicio];
         int dia = instance->getDia(slot.inicio);
         int hi = instance->getHorario(slot.inicio);
         int hf = instance->getHorario(slot.fim);
         if (!instance->isHorarioDisponivel(d, dia, turma, hi, hf))
         {
            std::pair<int, int> key(d, turma);

            int vt = turma - (*instance->getDisciplinas())[d].refTurma;
            int numAlunos = (*instance->getDisciplinas())[d].numAlunos[vt];

            if (tinvs.find(key) == tinvs.end())
               tinvs[key] = numAlunos;
            else
               tinvs[key] += numAlunos;
         }

         h = slot.fim + 1;
      }
   }

   std::map<std::pair<int, int>, int>::iterator itmax = tinvs.end();

   for (std::map<std::pair<int, int>, int>::iterator it = tinvs.begin(); it != tinvs.end(); ++it)
   {
      if (itmax == tinvs.end() || itmax->second > it->second)
         itmax = it;
   }

   if (itmax != tinvs.end())
      return itmax->first;
   return std::pair<int, int>(-1, -1);
}

bool Solution::unalloc()
{
   std::pair<int, int> max = getAlocacaoInviavel();
   if (max.first >= 0)
   {
      int* sd = solution[max.first];
      int turma = max.second;
      HDisciplina& disciplina = (*instance->getDisciplinas())[max.first];

      int numInstantes = instance->getNumeroInstantes();
      for (int h=0; h<numInstantes; h++)
      {
         if (sd[h] == turma || disciplina.disciplinaAssoc >= 0)
#ifdef ALLOW_INFEASIBLE_SLOT
            sd[h] = 0;
#else
            sd[h] = (instance->isHorarioDisponivel(max.first, h) ? 0 : -1);
#endif
      }
      if (disciplina.disciplinaAssoc >= 0)
      {
         HDisciplina& disciplina2 = (*instance->getDisciplinas())[disciplina.disciplinaAssoc];
         int* sd2 = solution[disciplina.disciplinaAssoc];
         for (int h=0; h<numInstantes; h++)
#ifdef ALLOW_INFEASIBLE_SLOT
            sd2[h] = 0;
#else
            sd2[h] = (instance->isHorarioDisponivel(disciplina.disciplinaAssoc, h) ? 0 : -1);
#endif
      }
      updateAlunosNaoAtendidos();
      eval();
      return true;
   }
   return false;
}

Slot Solution::readNextSlot(int disciplina, int h)
{
   Slot slot;
   slot.disciplina = disciplina;
   slot.blank = true;

   int numInstantes = instance->getNumeroInstantes();
   int turma = 0;

   int* sd = solution[disciplina];
   for (int k = h; k < numInstantes; k++)
   {
      if (sd[k] > 0)
      {
         slot.inicio = k;
         turma = sd[k];
         break;
      }
   }

   if (turma > 0)
   {
      for (int k = slot.inicio + 1; k < numInstantes; k++)
      {
         if (sd[k] != turma)
         {
            slot.fim = k - 1;
            slot.blank = false;
            break;
         }
         else if (sd[k] == turma && k == numInstantes - 1)
         {
            slot.fim = k;
            slot.blank = false;
            break;
         }
      }
   }

   return slot;
}

int Solution::calculaInviabilidadesHorariosIndisponiveis(int disciplina, int dia)
{
   int numInstantes = instance->getNumeroInstantes();

   int h = instance->getInstante(dia, 1);
   int* sd = solution[disciplina];

   int inv2 = 0;

   while (h < numInstantes)
   {
      Slot slot = readNextSlot(disciplina, h);
      if (slot.blank)
         break;

      if (instance->getDia(slot.inicio) != dia || instance->getDia(slot.fim) != dia)
         break;

      int turma = sd[slot.inicio];
      int hi = instance->getHorario(slot.inicio);
      int hf = instance->getHorario(slot.fim);

      if (!instance->isHorarioDisponivel(disciplina, dia, turma, hi, hf))
      {
         int vt = turma - (*instance->getDisciplinas())[disciplina].refTurma;
         int numAlunos = (*instance->getDisciplinas())[disciplina].numAlunos[vt];
         
         inv2 += numAlunos;
      }

      h = slot.fim + 1;
   }

   return inv2;
}

int Solution::calculaInviabilidadesHorariosIndisponiveis()
{
   int numDias = instance->getNumeroDias();
   int numDisciplinas = instance->getNumeroDisciplinas();

   int inv2 = 0;

   for (int d=0; d<numDisciplinas; d++)
      for (int t=1; t<=numDias; t++)
         inv2 += calculaInviabilidadesHorariosIndisponiveis(d, t);

   return inv2;
}

bool Solution::isEmpty()
{
   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();
   for (int d=0; d<numDisciplinas; d++)
      for (int h=0; h<numInstantes; h++)
         if (solution[d][h] > 0)
            return false;
   return true;
}

std::pair<int, int> Solution::getNumeroDemandasAtendidas()
{
   int disciplinasAtendidas = 0;
   std::hash_set<int> turmasAtendidas;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();
   for (int d=0; d<numDisciplinas; d++)
   {
      bool discAtendida = false;
      for (int h=0; h<numInstantes; h++)
      {
         int turma = solution[d][h];
         if (solution[d][h] > 0)
         {
            discAtendida = true;
            turmasAtendidas.insert(turma);
         }
      }
      if (discAtendida)
         disciplinasAtendidas++;
   }

   return std::pair<int, int>(disciplinasAtendidas, (int)turmasAtendidas.size());
}

int Solution::calculaFreeColumns()
{
   int imp2 = 0;

   int numDias = instance->getNumeroDias();
   for (int t=1; t<=numDias; t++)
      imp2 += calculaFreeColumns(t);

   return imp2;
}

int Solution::calculaFreeColumns(int dia)
{
   int imp2 = 0;

   int inicio = instance->getInstante(dia, 1);
   int fim = inicio + instance->getNumeroHorarios();
   for (int h=inicio; h<fim; h++)
      imp2 += calculaFreeColumnsInstante(h);

   return imp2;
}

int Solution::calculaFreeColumnsInstante(int instante)
{
   int count = 0;
   int numDisciplinas = instance->getNumeroDisciplinas();
   for (int d=0; d<numDisciplinas; d++)
      if (solution[d][instante] == 0)
         count++;

   return getWeight(count);
}

std::vector<std::pair<int, int>>* Solution::getDemandasNaoAtendidas()
{
   std::vector<std::pair<int, int>>* demandas = new std::vector<std::pair<int, int>>();

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();

   for (int d=0; d<numDisciplinas; d++)
   {
      HDisciplina& hdisciplina = (*instance->getDisciplinas())[d];
      int* sd = solution[d];
      if (hdisciplina.numTurmas > 0)
      {
         bool* alloc = new bool[hdisciplina.numTurmas];
         GUtil::fill(alloc, hdisciplina.numTurmas, false);
         for (int h=0; h<numInstantes; h++)
         {
            int turma = sd[h];
            if (turma > 0)
            {
               int ref = turma - hdisciplina.refTurma;
               alloc[ref] = true;
            }
         }
         for (int k=0; k<hdisciplina.numTurmas; k++)
         {
            if (!alloc[k])
            {
               int vturma = hdisciplina.refTurma + k;
               demandas->push_back(std::pair<int, int>(d, vturma));
            }
         }

         delete [] alloc;
      }
   }

   return demandas;
}

std::pair<int, int> Solution::getMenorNumeroAlunos()
{
   std::map<std::pair<int, int>, int> tinvs;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDisciplinas1 = numDisciplinas - 1;
   int numInstantes = instance->getNumeroInstantes();

   for (int t=0; t<numInstantes; t++)
   {
      int dia = instance->getDia(t);
      for (int d1=0; d1<numDisciplinas1; d1++)
      {
         int turma1 = solution[d1][t];
         if (turma1 <= 0)
            continue;
         int vt1 = turma1 - (*instance->getDisciplinas())[d1].refTurma;
         for (int d2=d1+1; d2<numDisciplinas; d2++)
         {
            int turma2 = solution[d2][t];
            if (turma2 <= 0)
               continue;
            int vt2 = turma2 - (*instance->getDisciplinas())[d2].refTurma;
            int numAlunos1 = (*instance->getDisciplinas())[d1].numAlunos[vt1];
            int numAlunos2 = (*instance->getDisciplinas())[d2].numAlunos[vt2];
            if (instance->hasConflito(turma1, turma2) || instance->hasConflito(dia, turma1, turma2))
            {
               std::pair<int, int> key1(d1, turma1);
               std::pair<int, int> key2(d2, turma2);
               if (tinvs.find(key1) == tinvs.end())
                  tinvs[key1] = -numAlunos1;
               if (tinvs.find(key2) == tinvs.end())
                  tinvs[key2] = -numAlunos2;
            }
         }
      }
   }

   std::map<std::pair<int, int>, int>::iterator itmax = tinvs.end();

   for (std::map<std::pair<int, int>, int>::iterator it = tinvs.begin(); it != tinvs.end(); ++it)
   {
      if (itmax == tinvs.end() || itmax->second > it->second)
         itmax = it;
   }

   if (itmax != tinvs.end())
      return itmax->first;
   return std::pair<int, int>(-1, -1);
}

int Solution::getNumeroAlunosAtendidos()
{
   int numAlunos = 0;
   std::hash_set<int> turmasAtendidas;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();
   for (int d=0; d<numDisciplinas; d++)
   {
      int* sd = solution[d];
      for (int h=0; h<numInstantes; h++)
      {
         int turma = sd[h];
         if (sd[h] > 0 && turmasAtendidas.find(turma) == turmasAtendidas.end())
         {
            turmasAtendidas.insert(turma);
            int i = turma - (*instance->getDisciplinas())[d].refTurma;
            int alunos = (*instance->getDisciplinas())[d].numAlunos[i];
            numAlunos += alunos;
         }
      }
   }

   return numAlunos;
}

void Solution::sa(int maxIter, double To, double Tf, double alpha)
{
   double T = To;

   Solution* current = this->clone();

   while (T > Tf)
   {
      std::cout << "[" << GUtil::intToString(T, 4) << "/" << GUtil::intToString(Tf, 3) << "]: BEST = " << GUtil::intToString(objectiveFunction.fo, 6) << "\n";
      int iterT = 0;
      while (iterT++ < maxIter)
      {
         int best = current->getObjectiveFunction().fo;

         Move* move = current->getRandomMove();
         current->move(move);

         double delta = current->getObjectiveFunction().fo - best;
         //std::cout << "[" << GUtil::intToString(iterT, 4) << "/" << GUtil::intToString(maxIter, 3) << "]: FO = " << GUtil::intToString(current->getObjectiveFunction().fo, 6) << " BEST = " << GUtil::intToString(objectiveFunction.fo, 6) << "\n";

         if (delta < 0.0001)
         {
            if (*current < *this)
               *this = *current;
         }
         else
         {
				double x = ((double)(rand() % 1000)) / 1000.0;
				double e = exp(-delta/T);
				if (x >= e)
					current->back(move);
         }

         if (move != NULL)
            delete move;
      }
      T *= alpha;
   }

   delete current;
}

void Solution::reset(int dia)
{
   int numDisciplinas = instance->getNumeroDisciplinas();
   int inicio = instance->getInstante(dia, 1);
   int fim = inicio + instance->getNumeroHorarios();

   for (int d=0; d<numDisciplinas; d++)
      for (int t=inicio; t<fim; t++)
#ifdef ALLOW_INFEASIBLE_SLOT
         solution[d][t] = 0;
#else
         solution[d][t] = (instance->isHorarioDisponivel(d, t) ? 0 : -1);
#endif
}

void Solution::alloc(int disciplina, int turma, int dia, int hi, int hf)
{
   int inicio = instance->getInstante(dia, hi);
   int fim = instance->getInstante(dia, hf);

   int* sd = solution[disciplina];
   for (int h=inicio; h<=fim; h++)
      sd[h] = turma;
}

void Solution::makeFeasible()
{
   while (infeasible())
   {
      if (!unalloc())
         break;
   }
}

void Solution::improve(int target)
{
   std::vector<std::pair<int, int>>* demandas = getDemandasNaoAtendidas();

   if (demandas == NULL)
      return;

   Solution* current = clone();

   int count = 0;

   while (demandas->size() > 0)
   {
      int p = rand() % (int)demandas->size();
      std::pair<int, int> demanda = (*demandas)[p];
      demandas->erase(demandas->begin() + p);

      bool success = reincorporar(current, demanda.first, demanda.second, target);
      if (success)
      {
         *this = *current;
         std::cout << "A turma " << demanda.second << " da disciplina " << demanda.first << " foi realocada com sucesso - " << getFatorAtendimento() << " - MIP Obj Val: " << getMIPObjVal() << "\n";
         count++;
         target = objectiveFunction.fo;
      }
      else
      {
         *current = *this;
      }
   }

   delete current;
   delete demandas;

   std::cout << "Total de turmas realocadas: " << count << "\n";

}

bool Solution::reincorporar(Solution* s, int disciplina, int turma, int target)
{
   bool success = SolutionBuilder::insert(s, disciplina, turma);
   if (success)
   {
      s->eval();
      s->mrd(30);
      int inv = s->getObjectiveFunction().fo;
      return (inv <= target);
   }
   return false;
}

Move* Solution::getBestShiftMove()
{
   Move* best = (Move*)new ShiftMove();
   best->setValue(INT_MAX);

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDias = instance->getNumeroDias();

   for (int d=0; d<numDisciplinas; d++)
   {
      for (int dia=1; dia<=numDias; dia++)
      {
         Move* move = getBestShiftMove(d, dia);
         if (move != NULL)
         {
            if (move->getValue() < best->getValue())
            {
               delete best;
               best = move;

               if (((Move*)best)->getValue() < objectiveFunction.fo)
                  break;
            }
            else
            {
               delete move;
            }
         }
      }
      if (((Move*)best)->getValue() < objectiveFunction.fo)
         break;
   }

   if (best->getValue() == INT_MAX)
   {
      delete best;
      best = NULL;
   }

   return best;
}

Move* Solution::getBestShiftMove(int disciplina, int dia)
{
   Move* best = (Move*)new ShiftMove();
   best->setValue(INT_MAX);

   int h = instance->getInstante(dia, 1);
   int end = h + instance->getNumeroHorarios() + 1;
   int* sd = solution[disciplina];

   while (h < end)
   {
      Slot slot = readNextSlot(disciplina, h);
      if (slot.blank)
         break;

      if (instance->getDia(slot.inicio) != dia || instance->getDia(slot.fim) != dia)
         break;

      Move* move = getBestShiftMove(slot);
      if (move != NULL)
      {
         if (move->getValue() < best->getValue())
         {
            delete best;
            best = move;
         }
         else
         {
            delete move;
         }

         if (((Move*)best)->getValue() < objectiveFunction.fo)
            break;
      }

      h = slot.fim + 1;
   }

   if (best->getValue() == INT_MAX)
   {
      delete best;
      best = NULL;
   }

   return best;
}

Move* Solution::getBestShiftMove(Slot& slot)
{
   ShiftMove* best = new ShiftMove();
   ShiftMove* move = new ShiftMove();

   ((Move*)best)->setValue(INT_MAX);

   int disciplina = slot.disciplina;
   int* sd = solution[disciplina];
   int turma = sd[slot.inicio];
   int dia = instance->getDia(slot.inicio);
   int tempo = slot.fim - slot.inicio + 1;

   int inicio = instance->getInstante(dia, 1);
   int fim = inicio + instance->getNumeroHorarios() - tempo + 1;

   move->setDisciplina(disciplina);
   move->setTurma(turma);
   move->setInicio1(slot.inicio);
   move->setFim1(slot.fim);

   best->setDisciplina(disciplina);
   best->setTurma(turma);
   best->setInicio1(slot.inicio);
   best->setFim1(slot.fim);

   for (int hi=inicio; hi<fim; hi++)
   {
      int hf = hi + tempo - 1;
      if ((sd[hi] != 0 && sd[hi] != turma) || (sd[hf] != 0 && sd[hf] != turma))
         continue;

      int rhi = hi - inicio + 1;
      int rhf = hf - inicio + 1;
      if (instance->isHorarioDisponivel(disciplina, dia, turma, rhi, rhf))
      {
         bool free = true;
         for (int h=hi; h<=hf; h++)
         {
            if (sd[h] != 0 && sd[h] != turma && !instance->isHorarioDisponivel(disciplina, h))
            {
               free = false;
               break;
            }
         }
         if (free)
         {
            move->setInicio2(hi);
            move->setFim2(hf);

            this->move((Move*)move);
            if (((Move*)move)->getValue() < ((Move*)best)->getValue())
            {
               best->setInicio2(hi);
               best->setFim2(hf);
               ((Move*)best)->setValue(((Move*)move)->getValue());

               if (((Move*)best)->getValue() < objectiveFunction.fo)
                  break;
            }
            this->back((Move*)move);
         }
      }
   }

   delete move;

   if (((Move*)best)->getValue() == INT_MAX)
   {
      delete best;
      best = NULL;
   }

   return (Move*)best;
}

Move* Solution::getBestMove()
{
   //MoveType types[5] = { SHIFT, SWAP, SWAP_DIVISION, DOUBLE_SHIFT, DOUBLE_SWAP };
   //const int N = 1;
   //MoveType type = types[(rand() % N)];
   MoveType type = SHIFT;
   return getBestMove(type);
}

Move* Solution::getBestMove(MoveType type)
{
   switch (type)
   {
   case SHIFT: return getBestShiftMove();
   default: return getBestShiftMove();
   }
}

void Solution::md(clock_t startTime, double timeLimit)
{
   while (true)
   {
      int best = objectiveFunction.fo;

      Move* move = getBestMove();

      int fo = objectiveFunction.fo;

      double elapsedTime = GUtil::getDiffClock(clock(), startTime);
      std::cout << "[MD] FO = " << GUtil::intToString(move->getValue(), 6) << " BEST = " << GUtil::intToString(fo, 6) << " Elapsed Time: " << GUtil::doubleToString(elapsedTime, 2) << " sec.\n";

      if (move != NULL && move->getValue() < fo)
      {
         this->move(move);
         delete move;
      }
      else
      {
         if (move != NULL)
            delete move;
         break;
      }
   }
}

int Solution::calculaInviabilidades()
{
   int inv = 0;

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numDisciplinas1 = numDisciplinas - 1;
   int numInstantes = instance->getNumeroInstantes();
   int numDias = instance->getNumeroDias() + 1;
   int numTurmas = instance->getNumeroTurmas() + 1;
   int numHorarios = instance->getNumeroHorarios();

   GUtil::fill(turmasDisponiveis, numTurmas, true);

   std::vector<HDisciplina>& disciplinas = *instance->getDisciplinas();

   for (int d1=0; d1<numDisciplinas1; ++d1)
   {
      for (int d2=d1+1; d2<numDisciplinas; ++d2)
      {
         if (!instance->hasConflitoDisciplina(d1, d2))
            continue;

         int sd1 = instance->getSize(d1);
         int sd2 = instance->getSize(d2);
         if (sd1 == 0 || sd2 == 0)
            continue;

         int* d1h = instance->getInstantes(d1);
         int* d2h = instance->getInstantes(d2);
         int p1 = 0;
         int p2 = 0;

         //for (int h=0; h<numInstantes; ++h)
         //{

         while (p1 < sd1 && p2 < sd2)
         {
            int h = 0;

            bool found = false;
            while (p1 < sd1 && p2 < sd2)
            {
               int h1 = d1h[p1];
               int h2 = d2h[p2];
               if (h1 == h2)
               {
                  h = h1;
                  found = true;
                  p1++;
                  p2++;
                  break;
               }
               else if (h1 < h2)
                  p1++;
               else
                  p2++;
            }

            if (!found)
               break;

            int turma1 = solution[d1][h];
            if (turma1 < 1)
               continue;

            int turma2 = solution[d2][h];
            if (turma2 < 1)
               continue;

            if (instance->hasConflito(turma1, turma2) || instance->hasConflito((h / numHorarios + 1), turma1, turma2))
            {
               if (turmasDisponiveis[turma1])
               {
                  turmasDisponiveis[turma1] = false;
                  inv += disciplinas[d1].numAlunos[turma1 - disciplinas[d1].refTurma];
               }
               if (turmasDisponiveis[turma2])
               {
                  turmasDisponiveis[turma2] = false;
                  inv += disciplinas[d2].numAlunos[turma2 - disciplinas[d2].refTurma];
               }
            }
         }
      }
   }

   for (int disciplina=0; disciplina<numDisciplinas; disciplina++)
   {
      int* sd = solution[disciplina];

      int h = 0;
      while (h < numInstantes)
      {
         Slot slot = readNextSlot(disciplina, h);
         if (slot.blank)
            break;

         int dia = instance->getDia(slot.inicio);
         int turma = sd[slot.inicio];
         int hi = instance->getHorario(slot.inicio);
         int hf = instance->getHorario(slot.fim);

         if (!instance->isHorarioDisponivel(disciplina, dia, turma, hi, hf))
         {
            if (turmasDisponiveis[turma])
            {
               inv += disciplinas[disciplina].numAlunos[turma - disciplinas[disciplina].refTurma];
               turmasDisponiveis[turma] = false;
            }
         }

         h = slot.fim + 1;
      }
   }

   return inv;
}

