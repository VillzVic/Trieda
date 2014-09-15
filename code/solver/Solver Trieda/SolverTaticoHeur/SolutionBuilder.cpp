#include "SolutionBuilder.h"

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "Solution.h"
#include "Instance.h"
#include "GUtil.h"

Solution* SolutionBuilder::createRandomSolution(Instance* instance, double alpha)
{
   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();
   int numDias = instance->getNumeroDias();
   int numHorarios = instance->getNumeroHorarios();

   if (numDisciplinas == 0 || numInstantes == 0 || numDias == 0 || numHorarios == 0)
      return NULL;

   Solution* solution = new Solution(instance);

   int** solstruct = solution->getSolution();

   std::vector<HDisciplina>& disciplinas = *instance->getDisciplinas();

   int turmaId = 0;

   //std::cout << "Gerando uma solucao inicial aleatoria...\n";

   typedef struct {
      int disciplina;
      int turma;
      int numAlunos;
   } Tuple;

   std::vector<Tuple> turmas;

   for (int d=0; d<numDisciplinas; d++)
   {
      HDisciplina& disciplina = disciplinas[d];
      for (int i=0; i<disciplina.numTurmas; i++)
      {
         Tuple tuple;
         tuple.disciplina = d;
         tuple.turma = ++turmaId;
         tuple.numAlunos = disciplina.numAlunos[i];
         turmas.push_back(tuple);
      }
   }

   struct dless_operator
   {
      inline bool operator() (const Tuple& left, const Tuple& right)
      {
         return (left.numAlunos > right.numAlunos);
      }
   };

   sort(turmas.begin(), turmas.end(), dless_operator());

   std::vector<Tuple> turmasNaoAlocadas;

   for (std::vector<Tuple>::iterator it = turmas.begin(); it != turmas.end(); ++it)
   {
      Tuple& tuple = *it;
      if (!insert(solution, tuple.disciplina, tuple.turma))
         turmasNaoAlocadas.push_back(tuple);
   }

   if (turmasNaoAlocadas.size() > 0)
   {
      int totalAlunos = 0;
      std::cout << "[warning] As seguintes turmas nao foram alocadas:\n(disciplina, turma, numAlunos) => ";
      for (std::vector<Tuple>::iterator it = turmasNaoAlocadas.begin(); it != turmasNaoAlocadas.end(); ++it)
      {
         Tuple& tuple = *it;
         std::cout << "(" << tuple.disciplina << ", " << tuple.turma << ", " << tuple.numAlunos << ") ";
         totalAlunos += tuple.numAlunos;
      }
      std::cout << "\nTotal de alunos nao atendidos: " << totalAlunos << "\n\n";
   }

   /*for (int d=0; d<numDisciplinas; d++)
   {
      HDisciplina& disciplina = disciplinas[d];
      //if (disciplina.disciplinaAssoc >= 0)
      //{
      //   turmaId += disciplina.numTurmas;
      //   continue;
      //}
      for (int i=0; i<disciplina.numTurmas; i++)
      {
         turmaId++;

         if (!insert(solution, d, turmaId))
         {
            //std::cout << "Refinando a solucao para alocar a turma " << turmaId << " (" << i << ") da disciplina " << (d+1) << "...\n";
            //solution->mrd(100);
            //if (insert(solution, d, turmaId))
            //   std::cout << "A turma " << turmaId << " (" << i << ") da disciplina " << (d+1) << " foi alocada com sucesso.\n";
            //else
               std::cout << "[warning] A turma " << turmaId << " (" << i << ") da disciplina " << (d+1) << " nao foi alocada.\n";
         }
      }
   }*/

   solution->updateAlunosNaoAtendidos();
   solution->eval();

   return solution;
}

bool SolutionBuilder::insert(Solution* solution, int d, int turma)
{
   Instance* instance = solution->getInstance();
   HDisciplina& disciplina = (*instance->getDisciplinas())[d];

   if (disciplina.divisoes.empty())
      return false;

   bool* alloc = new bool[disciplina.divisoes.size()];
   GUtil::fill(alloc, disciplina.divisoes.size(), true);

   std::vector<int> horarios;

   do
   {
      int div = rand() % disciplina.divisoes.size();
      if (alloc[div])
      {
         Divisao& divisao = disciplina.divisoes[div];
         for (int k=0; k<(int)divisao.dia.size(); k++)
         {
            int h = solution->freeslot(d, divisao.dia[k], turma, divisao.creditos[k]);
            if (h < 0)
            {
               horarios.clear();
               break;
            }
            else
               horarios.push_back(instance->getHorario(h));
         }
         if (horarios.size() > 0)
         {
            for (int k=0; k<(int)divisao.dia.size(); k++)
            {
               int h = horarios[k];
               int dia = divisao.dia[k];
               int tempo = divisao.creditos[k];
               solution->alloc(d, turma, div, dia, h, tempo);
            }
            break;
         }
         alloc[div] = false;
      }
   } while (GUtil::or(alloc, disciplina.divisoes.size()));

   delete [] alloc;

   return !horarios.empty();
}