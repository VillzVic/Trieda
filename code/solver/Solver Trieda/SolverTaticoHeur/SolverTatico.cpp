#include "SolverTatico.h"

#include <iostream>

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#endif
#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#endif

#include "Instance.h"
#include "GUtil.h"
#include "Solution.h"

SolverTatico::SolverTatico(void)
{
   lp = NULL;
}


SolverTatico::~SolverTatico(void)
{
}

void SolverTatico::solve(Solution* solution)
{
   this->solution = solution;

   Instance* instance = solution->getInstance();
   int numDias = instance->getNumeroDias();

   for (int dia=1; dia<=numDias; dia++)
   {
      vHash.clear();
      cHash.clear();
      turmas.clear();

	  #ifdef SOLVER_CPLEX
		lp = new OPT_CPLEX();
	  #elif defined SOLVER_GUROBI
		lp = new OPT_GUROBI();
	  #endif

      //std::cout << "Otimizando a solucao para o dia " << dia << "...\n";

      if (!createLP(dia))
         continue;

      //lp->setNumIntSols(0);
      //lp->setTimeLimit( 7200 );
      lp->setPreSolve(OPT_TRUE);
      //lp->setHeurFrequency(1.0);
      lp->setMIPScreenLog(0);
      lp->setMIPEmphasis(4);
      //lp->setPolishAfterNode(1);
      //lp->setPolishAfterTime(1800);
      //lp->setSymetry(0);
      //lp->setNoCuts();

      //std::string filename = "solverTaticoHeur_dia" + GUtil::intToString(dia);
      //lp->writeProbLP((char*)filename.c_str());

      OPTSTAT status = lp->optimize(METHOD_MIP);

      if (status == OPTSTAT_FEASIBLE || status == OPTSTAT_MIPOPTIMAL || status == OPTSTAT_LPOPTIMAL)
      {
         //std::cout << "Feasible - ObjCoef: " << lp->getObjVal() << "\n";
         double* xSol = new double[lp->getNumCols()];
         lp->getX(xSol);

         solution->reset(dia);

         for (STVariableHash::iterator it = vHash.begin(); it != vHash.end(); ++it)
         {
            const STVariable& v = it->first;
            int col = it->second;

            double value = xSol[col];
            if (v.getType() == STVariable::V_ASSIGN && value > 0.0001)
               solution->alloc(v.getDisciplina1(), v.getTurma1(), dia, v.getHorarioInicial(), v.getHorarioFinal());
         }

         solution->updateAlunosNaoAtendidos();
         solution->eval();

         delete [] xSol;
      }
      else
      {
         //std::cout << "Infeasible.\n";
      }

      if (lp != NULL)
         delete lp;
      lp = NULL;
   }

   //std::cout << "Done.\n"; getchar();
}

bool SolverTatico::createLP(int dia)
{
   int varNum = 0;
   int varTotal = 0;
   int constNum = 0;
   int constTotal = 0;

   std::string problemName = "TriedaTaticoHeur_"+GUtil::intToString(dia);
   bool status = lp->createLP((char*)problemName.c_str(), OPTSENSE_MINIMIZE, PROB_MIP);

   if (!status)
   {
      std::cout << "An error occurred on createLP method.\n";
      return false;
   }

   //std::cout << "Criando variaveis...\n";

   varNum = createVarAssign_X(dia);
   //std::cout << "Variaveis de alocacao (X): " << varNum << "\n";
   varTotal += varNum;

   varNum = createVarSlack_FH(dia);
   //std::cout << "Variaveis de folga (FH): " << varNum << "\n";
   varTotal += varNum;

   varNum = createVarSlack_FT();
   //std::cout << "Variaveis de folga (FT): " << varNum << "\n";
   varTotal += varNum;

   //std::cout << "Total de variaveis: " << varTotal << "\n";

   lp->updateLP();

   //std::cout << "Criando restricoes...\n";

   constNum = createConstAssign();
   //std::cout << "Restricao de alocacao de turmas: " << constNum << "\n";
   constTotal += constNum;

   constNum = createConstConflicts(dia);
   //std::cout << "Restricao de conflitos de turmas: " << constNum << "\n";
   constTotal += constNum;

   constNum = createConstAtend();
   //std::cout << "Restricao de atendimento: " << constNum << "\n";
   constTotal += constNum;

   lp->updateLP();

   //std::cout << "Total de restricoes: " << constTotal << "\n";

   return true;
}

int SolverTatico::createVarAssign_X(int dia)
{
   int varNum = 0;
   STVariable v;
   STVariableHash::iterator vit;

   Instance* instance = solution->getInstance();

   int** s = solution->getSolution();
   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();
   int numHorarios = instance->getNumeroHorarios();

   for (int disciplina=0; disciplina<numDisciplinas; disciplina++)
   {
      int* sd = s[disciplina];
      int h = instance->getInstante(dia, 1);

      while (h < numInstantes)
      {
         Slot slot = solution->readNextSlot(disciplina, h);
         if (slot.blank)
            break;

         if (instance->getDia(slot.inicio) != dia || instance->getDia(slot.fim) != dia)
            break;

         int turma = sd[slot.inicio];
         int tempo = slot.fim - slot.inicio + 1;
         int hend = numHorarios - tempo + 2;

         for (int hi=1; hi<hend; hi++)
         {
            int hf = hi + tempo - 1;
            if (instance->isHorarioDisponivel(disciplina, hi) && instance->isHorarioDisponivel(disciplina, dia, turma, hi, hf))
            {
               v.reset();
               v.setType(STVariable::V_ASSIGN);
               v.setDisciplina1(disciplina);
               v.setTurma1(turma);
               v.setHorarioInicial(hi);
               v.setHorarioFinal(hf);

               vit = vHash.find(v);
               if (vit == vHash.end())
               {
                  std::pair<int, int> dt(disciplina, turma);
                  turmas.insert(dt);

                  vHash[v] = lp->getNumCols();

                  OPT_COL col(OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0, (char *)v.toString().c_str());
                  lp->newCol(col);

                  varNum++;
               }
            }
         }
         
         h = slot.fim + 1;
      }
   }

   return varNum;   
}

int SolverTatico::createVarSlack_FH(int dia)
{
   int varNum = 0;
   STVariable v;
   STVariableHash::iterator vit;

   Instance* instance = solution->getInstance();
   int numHorarios = instance->getNumeroHorarios();

   for (int h=1; h<=numHorarios; h++)
   {
      for (std::set<std::pair<int, int>>::iterator it1 = turmas.begin(); it1 != turmas.end(); ++it1)
      {
         int disciplina1 = it1->first;
         int turma1 = it1->second;
         for (std::set<std::pair<int, int>>::iterator it2 = turmas.begin(); it2 != turmas.end(); ++it2)
         {
            int disciplina2 = it2->first;
            int turma2 = it2->second;

            if (disciplina1 == disciplina2 && turma1 == turma2)
               continue;

            if (turma1 > turma2)
               continue;

            if (instance->hasConflito(turma1, turma2) || instance->hasConflito(dia, turma1, turma2))
            {
               v.reset();
               v.setType(STVariable::V_SLACK_H);
               v.setHorarioInicial(h);
               v.setDisciplina1(disciplina1);
               v.setDisciplina2(disciplina2);
               v.setTurma1(turma1);
               v.setTurma2(turma2);

               vit = vHash.find(v);
               if (vit == vHash.end())
               {
                  vHash[v] = lp->getNumCols();

                  OPT_COL col(OPT_COL::VAR_INTEGRAL, 0.0, 0.0, OPT_INF, (char *)v.toString().c_str());
                  lp->newCol(col);

                  varNum++;
               }
            }
         }
      }
   }

   return varNum;
}

int SolverTatico::createVarSlack_FT()
{
   int varNum = 0;
   STVariable v;
   STVariableHash::iterator vit;

   Instance* instance = solution->getInstance();
   int numHorarios = instance->getNumeroHorarios();

   for (std::set<std::pair<int, int>>::iterator it = turmas.begin(); it != turmas.end(); ++it)
   {
      int disciplina = it->first;
      int turma = it->second;

      v.reset();
      v.setType(STVariable::V_SLACK_T);
      v.setDisciplina1(disciplina);
      v.setTurma1(turma);

      vit = vHash.find(v);
      if (vit == vHash.end())
      {
         vHash[v] = lp->getNumCols();

         int vt = turma - (*instance->getDisciplinas())[disciplina].refTurma;
         int numAlunos = (*instance->getDisciplinas())[disciplina].numAlunos[vt];

         double coef = (double)numAlunos;

         OPT_COL col(OPT_COL::VAR_BINARY, coef, 0.0, 1.0, (char *)v.toString().c_str());
         lp->newCol(col);

         varNum++;
      }
   }

   return varNum;
}

int SolverTatico::createConstAssign()
{
   int constNum = 0;
   STConstraint c;
   STConstraintHash::iterator cit;

   Instance* instance = solution->getInstance();

   int numHorarios = instance->getNumeroHorarios();

   for (std::set<std::pair<int, int>>::iterator it = turmas.begin(); it != turmas.end(); ++it)
   {
      int disciplina = it->first;
      int turma = it->second;

      c.reset();
      c.setType(STConstraint::C_ASSIGN);
      c.setDisciplina1(disciplina);
      c.setTurma1(turma);

      cit = cHash.find(c);
      if (cit != cHash.end())
         continue;

      OPT_ROW row(10, OPT_ROW::EQUAL, 1, (char*)c.toString().c_str());
      //OPT_ROW row(10, OPT_ROW::EQUAL, 0, (char*)c.toString().c_str());

      for (STVariableHash::iterator vit = vHash.begin(); vit != vHash.end(); ++vit)
      {
         const STVariable& v = vit->first;

         if (v.getType() == STVariable::V_ASSIGN && v.getDisciplina1() == disciplina && v.getTurma1() == turma)
         {
            int col = vit->second;
            double coef = 1.0;

            row.insert(col, coef);
         }
      }

      if (row.getnnz() > 0)
      {
         /*STVariable vs;
         vs.setType(STVariable::V_SLACK_T);
         vs.setDisciplina1(disciplina);
         vs.setTurma1(turma);

         STVariableHash::iterator vit = vHash.find(vs);
         if (vit != vHash.end())
            row.insert(vit->second, -1.0);*/

         cHash[c] = lp->getNumRows();
         lp->addRow(row);
         constNum++;
      }
   }

   return constNum;
}

int SolverTatico::createConstConflicts(int dia)
{
   int constNum = 0;
   STConstraint c;
   STConstraintHash::iterator cit;

   for (STVariableHash::iterator vit = vHash.begin(); vit != vHash.end(); ++vit)
   {
      const STVariable& v = vit->first;

      if (v.getType() != STVariable::V_SLACK_H)
         continue;

      c.reset();
      c.setType(STConstraint::C_CONFLICT);
      c.setHorario(v.getHorarioInicial());
      c.setDisciplina1(v.getDisciplina1());
      c.setTurma1(v.getTurma1());
      c.setDisciplina2(v.getDisciplina2());
      c.setTurma2(v.getDisciplina2());

      cit = cHash.find(c);
      if (cit != cHash.end())
         continue;

      int horario = v.getHorarioInicial();
      int turma1 = v.getTurma1();
      int turma2 = v.getTurma2();

      OPT_ROW row(10, OPT_ROW::EQUAL, 0, (char*)c.toString().c_str());

      for (STVariableHash::iterator vit2 = vHash.begin(); vit2 != vHash.end(); ++vit2)
      {
         const STVariable& v1 = vit2->first;

         if (v1.getType() != STVariable::V_ASSIGN)
            continue;

         int disciplina = v1.getDisciplina1();
         int turma = v1.getTurma1();
         int hi = v1.getHorarioInicial();
         int hf = v1.getHorarioFinal();

         if (horario >= hi && horario <= hf && (turma == turma1 || turma == turma2))
         {
            double coef = 1.0;
            row.insert(vit2->second, coef);
         }
      }

      if (row.getnnz() > 0)
      {
         row.insert(vit->second, -1.0);

         cHash[c] = lp->getNumRows();
         lp->addRow(row);
         constNum++;
      }
   }

   return constNum;
}

int SolverTatico::createConstAtend()
{
   int constNum = 0;
   STConstraint c;
   STConstraintHash::iterator cit;

   Instance* instance = solution->getInstance();

   for (STVariableHash::iterator vit = vHash.begin(); vit != vHash.end(); ++vit)
   {
      const STVariable& v = vit->first;

      if (v.getType() == STVariable::V_SLACK_T)
      {
         int disciplina = v.getDisciplina1();
         int turma = v.getTurma1();

         c.reset();
         c.setType(STConstraint::C_ATEND);
         c.setDisciplina1(v.getDisciplina1());
         c.setTurma1(v.getTurma1());

         int row = 0;
         int col = vit->second;

         cit = cHash.find(c);
         if (cit == cHash.end())
         {
            row = lp->getNumRows();
            cHash[c] = row;
            OPT_ROW crow(10, OPT_ROW::LESS, 0, (char*)c.toString().c_str());
            lp->addRow(crow);
            constNum++;
         }
         else
            row = cit->second;

         double coef = -1.0e6;

         lp->chgCoef(row, col, coef);
      }
      else if (v.getType() == STVariable::V_SLACK_H)
      {
         int disciplina1 = v.getDisciplina1();
         int turma1 = v.getTurma1();

         c.reset();
         c.setType(STConstraint::C_ATEND);
         c.setDisciplina1(disciplina1);
         c.setTurma1(turma1);

         int row = 0;
         int col = vit->second;

         cit = cHash.find(c);
         if (cit == cHash.end())
         {
            row = lp->getNumRows();
            cHash[c] = row;
            OPT_ROW crow(10, OPT_ROW::LESS, 0, (char*)c.toString().c_str());
            lp->addRow(crow);
            constNum++;
         }
         else
            row = cit->second;

         lp->chgCoef(row, col, 1.0);

         int disciplina2 = v.getDisciplina2();
         int turma2 = v.getTurma2();

         c.reset();
         c.setType(STConstraint::C_ATEND);
         c.setDisciplina1(disciplina2);
         c.setTurma1(turma2);

         cit = cHash.find(c);
         if (cit == cHash.end())
         {
            row = lp->getNumRows();
            cHash[c] = row;
            OPT_ROW crow(10, OPT_ROW::LESS, 0, (char*)c.toString().c_str());
            lp->addRow(crow);
            constNum++;
         }
         else
            row = cit->second;

         lp->chgCoef(row, col, 1.0);
      }
   }

   return constNum;
}
