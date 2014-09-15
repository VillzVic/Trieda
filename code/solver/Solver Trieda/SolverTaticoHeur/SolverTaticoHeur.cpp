#include "SolverTaticoHeur.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <ctime>
#include <windows.h>

#include "GUtil.h"
#include "SolutionBuilder.h"
#include "CPUTimerWin.h"
#include "SolverTatico.h"
#include "opt_lp.h"

#include "../ProblemData.h"

int SolverTaticoHeur::runId = 0;

SolverTaticoHeur::SolverTaticoHeur(ProblemData* problemData, Campus* campus)
{
   this->problemData = problemData;
   this->campus = campus;

   vSetTatico = new VariableTaticoSet();

   instance = NULL;
   solution = NULL;
   bestObjVal = 1e10;
   xSol = NULL;
   lp = NULL;
   vHashTatico = NULL;

   testingPhase = false;
}

SolverTaticoHeur::~SolverTaticoHeur(void)
{
   if (vSetTatico != NULL)
      delete vSetTatico;
   vSetTatico = NULL;

   if (instance != NULL)
      delete instance;
   instance = NULL;

   if (solution != NULL)
      delete solution;
   instance = NULL;

   if (xSol != NULL)
      delete xSol;
   xSol = NULL;
}

bool SolverTaticoHeur::hasVariable(VariableTatico& v)
{
   return (vSetTatico->find(v) != vSetTatico->end());
}

bool SolverTaticoHeur::addVariable(VariableTatico& v)
{
   vSetTatico->insert(v);
   return true;
}

void SolverTaticoHeur::build()
{
   std::cout << "Mapping data...\n";

   this->dias.clear();
   this->horarios.clear();

   if (vSetTatico == NULL || vSetTatico->empty())
   {
      std::cout << "No data found.\n";
      return;
   }

   if (instance != NULL)
      delete instance;
   this->instance = new Instance();

   std::set<int> tdias;
   std::set<Disciplina*> tdisciplinas;
   std::hash_map<int, std::set<int>> tturmas;
   std::set<DateTime> thorarios;
   std::hash_map<int, std::hash_map<int, std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>>> tdhorarios;

   std::hash_map<int, int> mapDia;
   std::hash_map<int, int> mapDisciplina;

   for (VariableTaticoSet::iterator it = vSetTatico->begin(); it != vSetTatico->end(); ++it)
   {
      int vdisciplina = it->getDisciplina()->getId();
      int vturma = it->getTurma();
      int vdia = it->getDia();

      std::hash_map<int, std::hash_map<int, std::vector<VariableTaticoSet::iterator>>>& mapVariablesD = mapVariables[vdisciplina];
      std::hash_map<int, std::vector<VariableTaticoSet::iterator>>& mapVariablesDI = mapVariablesD[vturma];
      std::vector<VariableTaticoSet::iterator>& mapVariablesDIT = mapVariablesDI[vdia];
      mapVariablesDIT.push_back(it);
   }

   //std::ofstream fout("variaveis_tatico_heur.txt", std::ios_base::out);
   //fout << "Disciplina\tTurma\tDia\tSala\tHorarioInicial\tHorarioFinal\n";

   for (VariableTaticoSet::iterator it = vSetTatico->begin(); it != vSetTatico->end(); ++it)
   {
      int turma = it->getTurma();                                 // i
      Disciplina* disciplina = it->getDisciplina();               // d
      Unidade* unidade = it->getUnidade();                        // u
      ConjuntoSala* cjtSala = it->getSubCjtSala();                // tps
      int dia = it->getDia();                                     // t
      HorarioAula* horarioInicial = it->getHorarioAulaInicial();  // hi
      HorarioAula* horarioFinal = it->getHorarioAulaFinal();      // hf

      std::set<int>& dturmas = tturmas[disciplina->getId()];
      dturmas.insert(turma);
      tdias.insert(dia);

      if (tdisciplinas.find(disciplina) == tdisciplinas.end())
      {
         HDisciplina hdisciplina;
         hdisciplina.disciplina = disciplina;
         hdisciplina.disciplinaAssoc = -1;

         tdisciplinas.insert(disciplina);
         instance->getDisciplinas()->push_back(hdisciplina);
      }

      std::hash_map<int, std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>>& ddhorarios = tdhorarios[disciplina->getId()];
      std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>& tfhorarios = ddhorarios[dia];
      std::vector<std::pair<DateTime, DateTime>>& dhorarios = tfhorarios[turma];
      dhorarios.push_back(std::pair<DateTime, DateTime>(horarioInicial->getInicio(), horarioFinal->getInicio()));

      Sala* sala = cjtSala->salas.begin()->second;

      std::hash_map<int, std::vector<std::pair<int, int>>>& mapTurmasSala = mapTurmasDiaSala[dia];
      std::vector<std::pair<int, int>>& mapTurmas = mapTurmasSala[sala->getId()];
      mapTurmas.push_back(std::pair<int, int>(disciplina->getId(), turma));

      //fout << disciplina->getId() << "\t" << turma << "\t" << dia << "\t" << sala->getId() << "\t" << GUtil::intToString(horarioInicial->getInicio().getHour(), 2, '0', '0') << ":" << GUtil::intToString(horarioInicial->getInicio().getMinute(), 2, '0', '0') << "\t" << GUtil::intToString(horarioFinal->getInicio().getHour(), 2, '0', '0') << ":" << GUtil::intToString(horarioFinal->getInicio().getMinute(), 2, '0', '0') << "\n";
   }
   //fout.close();

   struct dless_operator
   {
      inline bool operator() (const HDisciplina& left, const HDisciplina& right)
      {
         int d1 = abs(left.disciplina->getId());
         int d2 = abs(right.disciplina->getId());
         if (d1 != d2)
            return (d1 < d2);
         return (-left.disciplina->getId() < -right.disciplina->getId());
      }
   };

   sort(instance->getDisciplinas()->begin(), instance->getDisciplinas()->end(), dless_operator());

   for (int d=0; d < (int)instance->getDisciplinas()->size(); d++)
   {
      HDisciplina& hdisciplina = (*instance->getDisciplinas())[d];
      mapDisciplina[hdisciplina.disciplina->getId()] = d;
   }

   for (std::set<int>::iterator it = tdias.begin(); it != tdias.end(); ++it)
   {
      mapDia[*it] = (int)dias.size() + 1;
      dias.push_back(*it);
   }
   instance->setNumeroDias((int)dias.size());

   for (std::vector<HDisciplina>::iterator it = instance->getDisciplinas()->begin(); it != instance->getDisciplinas()->end(); ++it)
   {
      HDisciplina& hdisciplina = *it;
      std::set<int>& turmas = tturmas[hdisciplina.disciplina->getId()];

      for (std::set<int>::iterator it2 = turmas.begin(); it2 != turmas.end(); ++it2)
      {
         int turma = *it2;
         int nroAlunos = problemData->existeTurmaDiscCampus(turma, hdisciplina.disciplina->getId(), campus->getId());
         hdisciplina.turmas.push_back(turma);
         hdisciplina.numAlunos.push_back(nroAlunos);
      }

      hdisciplina.numTurmas = (int)hdisciplina.turmas.size();
      hdisciplina.numCreditos = hdisciplina.disciplina->getTotalCreditos();

      std::hash_map<int, std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>>& ddhorarios = tdhorarios[hdisciplina.disciplina->getId()];
      for (GGroup<Horario*, LessPtr<Horario>>::iterator it2 = hdisciplina.disciplina->horarios.begin(); it2 != hdisciplina.disciplina->horarios.end(); ++it2)
      {
         Horario* horario = *it2;
         for (std::vector<int>::iterator itD = dias.begin(); itD != dias.end(); ++itD)
         {
            std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>& tfhorarios = ddhorarios[*itD];
            for (std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>::iterator itDT = tfhorarios.begin(); itDT != tfhorarios.end(); ++itDT)
            {
               std::vector<std::pair<DateTime, DateTime>>& horarios = itDT->second;
               if (isHorarioDisponivel(horario->horario_aula->getInicio(), horarios))
               {
                  thorarios.insert(horario->horario_aula->getInicio());
                  break;
               }
            }
         }
      }
   }

   for (std::set<DateTime>::iterator it = thorarios.begin(); it != thorarios.end(); ++it)
   {
      mapHorario[*it] = (int)horarios.size();
      horarios.push_back(*it);
   }

   instance->setNumeroHorarios((int)horarios.size());

   for (std::vector<HDisciplina>::iterator it = instance->getDisciplinas()->begin(); it != instance->getDisciplinas()->end(); ++it)
   {
      HDisciplina& hdisciplina = *it;

      std::hash_map<int, std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>>& ddhorarios = tdhorarios[hdisciplina.disciplina->getId()];
      for (GGroup<Horario*, LessPtr<Horario>>::iterator it2 = hdisciplina.disciplina->horarios.begin(); it2 != hdisciplina.disciplina->horarios.end(); ++it2)
      {
         Horario* horario = *it2;
         int h = mapHorario[horario->horario_aula->getInicio()];
         for (std::vector<int>::iterator itD = dias.begin(); itD != dias.end(); ++itD)
         {
            int dia = *itD;
            std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>& tfhorarios = ddhorarios[dia];
            for (std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>::iterator itDT = tfhorarios.begin(); itDT != tfhorarios.end(); ++itDT)
            {
               std::vector<std::pair<DateTime, DateTime>>& horarios = itDT->second;
               if (isHorarioDisponivel(horario->horario_aula->getInicio(), horarios))
                  hdisciplina.horarios[mapDia[dia]].insert(h);
            }
         }
      }

      std::vector<std::vector<std::pair<int, int>>>& divisoes = hdisciplina.disciplina->combinacao_divisao_creditos;
      if (divisoes.size() > 0)
      {
         for (int i=0; i<(int)divisoes.size(); i++)
         {
            bool valid = true;

            Divisao divisao;
            std::vector<std::pair<int, int>>& divisoesi = divisoes[i];
            for (int j=0; j<(int)divisoesi.size(); j++)
            {
               if (divisoesi[j].second > 0)
               {
                  int dia = divisoesi[j].first;
                  int creditos = divisoesi[j].second;
                  if (!isValidDivisao(hdisciplina.disciplina, dia, creditos))
                  {
                     valid = false;
                     break;
                  }
                  divisao.dia.push_back(mapDia[dia]);
                  divisao.creditos.push_back(creditos);
               }
            }
            if (valid)
               hdisciplina.divisoes.push_back(divisao);
         }
      }
   }

   instance->buildRefTurma();

   std::vector<HDisciplina>* disciplinas = instance->getDisciplinas();

   for (std::vector<HDisciplina>::iterator it = disciplinas->begin(); it != disciplinas->end(); ++it)
   {
      HDisciplina& hdisciplina = *it;

      std::hash_map<int, std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>>& ddhorarios = tdhorarios[hdisciplina.disciplina->getId()];

      for (std::vector<int>::iterator itD = dias.begin(); itD != dias.end(); ++itD)
      {
         int dia = *itD;
         std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>& tfhorarios = ddhorarios[dia];
         for (std::hash_map<int, std::vector<std::pair<DateTime, DateTime>>>::iterator itDT = tfhorarios.begin(); itDT != tfhorarios.end(); ++itDT)
         {
            int turma = itDT->first;
            std::vector<std::pair<DateTime, DateTime>>& horarios = itDT->second;
            for (std::vector<std::pair<DateTime, DateTime>>::iterator itHiHf = horarios.begin(); itHiHf != horarios.end(); ++itHiHf)
            {
               std::pair<DateTime, DateTime>& period = *itHiHf;
               if (mapHorario.find(period.first) == mapHorario.end() || mapHorario.find(period.second) == mapHorario.end())
                  continue;

               int inicio = mapHorario[period.first] + 1;
               int fim = mapHorario[period.second] + 1;
               int vdia = mapDia[dia];
               
               int vturma = -1;
               for (int k=0; k<(int)hdisciplina.turmas.size(); k++)
               {
                  if (hdisciplina.turmas[k] == turma)
                  {
                     vturma = hdisciplina.refTurma + k;
                     break;
                  }
               }
               if (vturma != -1)
               {
                  std::hash_map<int, std::hash_map<int, std::hash_set<int>>>& dhihfvalidos = hdisciplina.hihfvalidos[vdia];
                  std::hash_map<int, std::hash_set<int>>& thihfvalidos = dhihfvalidos[vturma];
                  std::hash_set<int>& dhfvalidos = thihfvalidos[inicio];
                  dhfvalidos.insert(fim);
               }
            }
         }
      }
   }

   // Analisar conflitos entre turmas
   int end1 = (int)disciplinas->size() - 1;
   int end2 = (int)disciplinas->size();
   for (int d1=0; d1 < end1; d1++)
   {
      HDisciplina& hdisciplina1 = (*disciplinas)[d1];
      Disciplina* disciplina1 = hdisciplina1.disciplina;

      int nt1 = (int)hdisciplina1.turmas.size();
      for (int i1=0; i1 < nt1; i1++)
      {
         int turma1 = hdisciplina1.turmas[i1];
         for (int d2=i1+1; d2 < end2; d2++)
         {
            HDisciplina& hdisciplina2 = (*disciplinas)[d2];
            Disciplina* disciplina2 = hdisciplina2.disciplina;

            int nt2 = (int)hdisciplina2.turmas.size();
            for (int i2=0; i2 < nt2; i2++)
            {
               int turma2 = hdisciplina2.turmas[i2];

               bool possuiAlunosEmComum = problemData->possuiAlunosEmComum(turma1, disciplina1, campus, turma2, disciplina2, campus);
               if (possuiAlunosEmComum)
               {
                  int hturma1 = instance->getTurma(d1, i1);
                  int hturma2 = instance->getTurma(d2, i2);
                  instance->addConflito(hturma1, hturma2);
               }
            }
         }
      }
   }

   for (std::hash_map<int, std::hash_map<int, std::vector<std::pair<int, int>>>>::iterator it1 = mapTurmasDiaSala.begin(); it1 != mapTurmasDiaSala.end(); ++it1)
   {
      int dia = mapDia[it1->first];
      std::hash_map<int, std::vector<std::pair<int, int>>>& mapTurmasSala = it1->second;
      for (std::hash_map<int, std::vector<std::pair<int, int>>>::iterator it2 = mapTurmasSala.begin(); it2 != mapTurmasSala.end(); ++it2)
      {
         int sala = it2->first;
         std::vector<std::pair<int, int>>& mapTurmas = it2->second;
         for (std::vector<std::pair<int, int>>::iterator it3 = mapTurmas.begin(); it3 != mapTurmas.end(); ++it3)
         {
            std::pair<int, int>& dturma1 = *it3;
            int disciplina1 = dturma1.first;
            int turma1 = dturma1.second;

            int d1 = mapDisciplina[disciplina1];
            HDisciplina& hdisciplina = (*instance->getDisciplinas())[d1];
            int t1 = -1;
            for (int i=0; i < (int)hdisciplina.turmas.size(); i++)
            {
               if (hdisciplina.turmas[i] == turma1)
               {
                  t1 = i;
                  break;
               }
            }

            if (t1 != -1)
            {
               for (std::vector<std::pair<int, int>>::iterator it4 = mapTurmas.begin(); it4 != mapTurmas.end(); ++it4)
               {
                  std::pair<int, int>& dturma2 = *it4;
                  int disciplina2 = dturma2.first;
                  int turma2 = dturma2.second;

                  int d2 = mapDisciplina[disciplina2];
                  HDisciplina& hdisciplina = (*instance->getDisciplinas())[d2];
                  int t2 = -1;
                  for (int i=0; i < (int)hdisciplina.turmas.size(); i++)
                  {
                     if (hdisciplina.turmas[i] == turma2)
                     {
                        t2 = i;
                        break;
                     }
                  }

                  if (t2 != -1)
                  {
                     int tt1 = instance->getTurma(d1, t1);
                     int tt2 = instance->getTurma(d2, t2);
                     instance->addConflito(dia, tt1, tt2);
                  }
               }
            }
         }
      }
   }

   instance->buildDivisoesDefault();
   instance->buildDivisoes();
   instance->preprocess();

   // Print Data
   /*std::cout << "\nDias = ";
   for (int i=0; i<(int)dias.size(); i++)
      std::cout << i << ":" << dias[i] << "; ";

   std::cout << "\n\nDisciplinas:";
   for (std::vector<HDisciplina>::iterator it = instance->getDisciplinas()->begin(); it != instance->getDisciplinas()->end(); ++it)
   {
      HDisciplina& hdisciplina = *it;
      std::cout << "\n - Disciplina " << hdisciplina.disciplina->getId() << "\n"
         << "   -> Numero de turmas: " << hdisciplina.numTurmas << " ( ";
      for (int i=0; i<(int)hdisciplina.turmas.size(); i++)
         std::cout << hdisciplina.turmas[i] << " ";
      std::cout << ")\n";

      std::cout << "   -> Numero de creditos: " << hdisciplina.numCreditos << "\n";

      std::cout << "   -> Horarios:\n";
      for (std::hash_map<int, std::hash_set<int>>::iterator it2 = hdisciplina.horarios.begin(); it2 != hdisciplina.horarios.end(); ++it2)
      {
         for (std::hash_set<int>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
         {
            std::cout << "      . [" << it2->first << "]: " << GUtil::intToString(*it3, 2) << ": " << horarios[*it3] << "\n";
         }
      }

      std::cout << "   -> Divisao de creditos: " << hdisciplina.numCreditos << "\n";
      std::vector<std::vector<std::pair<int, int>>>& divisoes = hdisciplina.disciplina->combinacao_divisao_creditos;
      for (int i=0; i<(int)divisoes.size(); i++)
      {
         std::cout << "      . " << GUtil::intToString(i, 2) << ": ";
         for (int j=0; j<(int)divisoes[i].size(); j++)
         {
            std::cout << "(" << divisoes[i][j].first << ", " << divisoes[i][j].second << ") ";
         }
         std::cout << "\n";
      }
   }

   std::cout << "\nHorarios: " << horarios.size() << "\n";
   for (int i=0; i<(int)horarios.size(); i++)
      std::cout << " - " << GUtil::intToString(i, 2) << ": " << horarios[i] << ";\n";

   std::cout << instance->toString();*/

   std::cout << "Done.\n";
}

void SolverTaticoHeur::writeSolution(std::vector<VariableTatico>& solution)
{
   if (instance == NULL || instance->isEmpty())
      return;

   std::vector<Atendimento> atendimentos;
   loadAtendimentos(atendimentos);

   std::hash_map<int, std::hash_set<int>> errturmas;

   for (std::vector<Atendimento>::iterator it = atendimentos.begin(); it != atendimentos.end(); ++it)
   {
      Atendimento& atendimento = *it;
      
      HDisciplina& hdisciplina = (*instance->getDisciplinas())[atendimento.disciplina];

      Disciplina* disciplina = hdisciplina.disciplina;
      int turma = hdisciplina.turmas[atendimento.turma-hdisciplina.refTurma];
      int dia = dias[instance->getDia(atendimento.inicio)-1];
      int inicio = instance->getHorario(atendimento.inicio) - 1;
      int fim = instance->getHorario(atendimento.fim) - 1;
      if (fim == 0)
         fim = inicio;
      else if (fim >= instance->getNumeroHorarios())
         continue;
      DateTime& horarioInicial = horarios[inicio];
      DateTime& iniHorarioFinal = horarios[fim];

      std::hash_set<int>& derrturmas = errturmas[disciplina->getId()];
      if (derrturmas.find(turma) != derrturmas.end())
         continue;

      VariableTaticoSet::iterator vit = getVariableTatico(disciplina, turma, dia, horarioInicial, iniHorarioFinal);

      if (vit != vSetTatico->end())
         solution.push_back(*vit);
      else
         derrturmas.insert(turma);
   }
}

void SolverTaticoHeur::writeSolution(std::string filename)
{
   std::vector<Atendimento> atendimentos;
   loadAtendimentos(atendimentos);

   std::hash_map<int, std::hash_set<int>> errturmas;

   std::vector<std::string> variables;

   for (std::vector<Atendimento>::iterator it = atendimentos.begin(); it != atendimentos.end(); ++it)
   {
      Atendimento& atendimento = *it;
      
      HDisciplina& hdisciplina = (*instance->getDisciplinas())[atendimento.disciplina];

      Disciplina* disciplina = hdisciplina.disciplina;
      int turma = hdisciplina.turmas[atendimento.turma-hdisciplina.refTurma];
      int dia = dias[instance->getDia(atendimento.inicio)-1];
      int inicio = instance->getHorario(atendimento.inicio) - 1;
      int fim = instance->getHorario(atendimento.fim) - 1;
      if (fim == 0)
         fim = inicio;
      else if (fim >= instance->getNumeroHorarios())
         continue;
      DateTime& horarioInicial = horarios[inicio];
      DateTime& iniHorarioFinal = horarios[fim];

      std::hash_set<int>& derrturmas = errturmas[disciplina->getId()];
      if (derrturmas.find(turma) != derrturmas.end())
         continue;

      /*VariableTaticoSet::iterator vit = getVariableTatico(disciplina, turma, dia, horarioInicial, iniHorarioFinal);

      if (vit != vSetTatico->end())
         variables.push_back(*vit);
      else
         derrturmas.insert(turma);*/
   }
}

VariableTaticoSet::iterator SolverTaticoHeur::getVariableTatico(Disciplina* disciplina, int turma, int dia, DateTime& horarioInicial, DateTime& horarioFinal)
{
   VariableTaticoSet::iterator vit = vSetTatico->end();

   std::hash_map<int, std::hash_map<int, std::vector<VariableTaticoSet::iterator>>>& mapVariablesD = mapVariables[disciplina->getId()];
   std::hash_map<int, std::vector<VariableTaticoSet::iterator>>& mapVariablesDI = mapVariablesD[turma];
   std::vector<VariableTaticoSet::iterator>& mapVariablesDIT = mapVariablesDI[dia];

   for (int i=0; i<(int)mapVariablesDIT.size(); i++)
   {
      VariableTaticoSet::iterator& it = mapVariablesDIT[i];
      int vdisciplina = it->getDisciplina()->getId();
      int vturma = it->getTurma();
      int vdia = it->getDia();

      if (vdisciplina != disciplina->getId() || vturma != turma || vdia != dia)
         continue;

      DateTime& inicio = it->getHorarioAulaInicial()->getInicio();
      DateTime& fim = it->getHorarioAulaFinal()->getInicio();

      if (inicio == horarioInicial && fim >= horarioFinal)
      {
         if (vit == vSetTatico->end() || it->getHorarioAulaFinal()->getInicio() < vit->getHorarioAulaFinal()->getInicio())
            vit = it;
      }
   }

   return vit;
}

void SolverTaticoHeur::loadAtendimentos(std::vector<Atendimento>& atendimentos)
{
   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();

   for (int d=0; d<numDisciplinas; d++)
   {
      int h = 0;
      int* sd = solution->getSolution()[d];

      while (h < numInstantes)
      {
         Slot slot = solution->readNextSlot(d, h);
         if (slot.blank)
            break;
         
         Atendimento atendimento;
         atendimento.disciplina = d;
         atendimento.turma = sd[slot.inicio];
         atendimento.inicio = slot.inicio;
         atendimento.fim = slot.fim;

         atendimentos.push_back(atendimento);

         h = slot.fim + 1;
      }
   }
}

bool SolverTaticoHeur::isHorarioDisponivel(DateTime& horario, std::vector<std::pair<DateTime, DateTime>>& horarios)
{
   for (std::vector<std::pair<DateTime, DateTime>>::iterator it = horarios.begin(); it != horarios.end(); ++it)
      if (horario >= it->first && horario <= it->second)
         return true;
   return false;
}

void SolverTaticoHeur::printSolution()
{
   ObjectiveFunction f = solution->getObjectiveFunction();

   std::cout << "[Solucao]\n";
   std::cout << " - Funcao Objetivo: { FO = " << f.fo << "\t INV1 = " << f.inv1 << " [" << f.lambda1 << "]\t INV2 = " << f.inv2 << " [" << f.lambda2 << "]\t IMP1 = " << f.imp1 << " [" << f.beta1 << "]\t IMP2 = " << f.imp2 << " [" << f.beta2 << "] }\n";

   int numDisciplinas = instance->getNumeroDisciplinas();
   int numInstantes = instance->getNumeroInstantes();
   int numDias = instance->getNumeroDias();
   int numHorarios = instance->getNumeroHorarios();

   int slotSize = 5;
   for (std::vector<HDisciplina>::iterator it = instance->getDisciplinas()->begin(); it != instance->getDisciplinas()->end(); ++it)
   {
      HDisciplina& hdisciplina = *it;
      std::string idstr = GUtil::intToString(hdisciplina.disciplina->getId());
      if ((int)idstr.size() > slotSize)
         slotSize = (int)idstr.size();
   }

   std::string line(slotSize + 2, ' ');
   line += "+";
   for (int dia=0; dia<numDias; dia++)
      line += std::string(numHorarios * (slotSize+1) + 1, '-') + "+";
   line += "\n";

   std::cout << " - Representacao:\n" << line << std::string(slotSize-1, ' ') << "d: | ";
   for (int dia=0; dia<numDias; dia++)
      std::cout << GUtil::intToString(dias[dia], slotSize) << std::string((numHorarios-1) * (slotSize+1) + 1, ' ') + "| ";
   std::cout << "\n" << line << std::string(slotSize-1, ' ') << "h: | ";

   for (int dia=0; dia<numDias; dia++)
   {
      for (int horario=0; horario<numHorarios; horario++)
      {
         DateTime& time = horarios[horario];
         std::cout << GUtil::intToString(time.getHour(), 2, '0', '0') << ":" << GUtil::intToString(time.getMinute(), 2, '0', '0') << " ";
      }
      std::cout << "| ";
   }
   std::cout << "\n" << line;

   for (int d=0; d<numDisciplinas; d++)
   {
      HDisciplina& hdisciplina = (*instance->getDisciplinas())[d];
      std::cout << " " << GUtil::intToString(hdisciplina.disciplina->getId(), slotSize) + " : ";
      for (int dia=1; dia<=numDias; dia++)
      {
         for (int horario=1; horario<=numHorarios; horario++)
         {
            int t = instance->getInstante(dia, horario);
            int turma = solution->getSolution()[d][t];
            if (turma < 0)
               std::cout << std::string(slotSize, '.') << " ";
            else if (turma == 0)
               std::cout << std::string(slotSize, ' ') << " ";
            else
            {
               int ref = hdisciplina.turmas[turma-hdisciplina.refTurma];
               std::cout << GUtil::intToString(ref, slotSize) << " ";
            }
         }
         std::cout << "| ";
      }
      std::cout << "\n";
   }

   std::string doubleline(slotSize + 2, ' ');
   doubleline += "+";
   for (int dia=0; dia<numDias; dia++)
      doubleline += std::string(numHorarios * (slotSize+1) + 1, '=') + "+";
   doubleline += "\n";

   std::cout << doubleline;

   std::vector<Atendimento> atendimentos;
   loadAtendimentos(atendimentos);

   //std::vector<Atendimento*> mappingerrors;
   for (int d=0; d<numDisciplinas; d++)
   {
      HDisciplina& hdisciplina = (*instance->getDisciplinas())[d];
      std::cout << " " << GUtil::intToString(hdisciplina.disciplina->getId(), slotSize) + " : ";
      for (int dia=1; dia<=numDias; dia++)
      {
         for (int horario=1; horario<=numHorarios; horario++)
         {
            int t = instance->getInstante(dia, horario);
            int turma = solution->getSolution()[d][t];
            if (turma < 0)
               std::cout << std::string(slotSize, '.') << " ";
            else if (turma == 0)
               std::cout << std::string(slotSize, ' ') << " ";
            else
            {
               int ref = hdisciplina.turmas[turma-hdisciplina.refTurma];

               Atendimento* atendimento = getAtendimento(atendimentos, d, turma, t);
               if (atendimento == NULL)
               {
                  std::cout << std::string(slotSize, '?') << " ";
                  //mappingerrors.push_back(atendimento);
                  continue;
               }
               
               int hi = instance->getHorario(atendimento->inicio) - 1;
               int hf = instance->getHorario(atendimento->fim) - 1;

               VariableTaticoSet::iterator vit = getVariableTatico(hdisciplina.disciplina, ref, dias[dia-1], horarios[hi], horarios[hf]);
               if (vit == vSetTatico->end())
               {
                  std::cout << std::string(slotSize, '?') << " ";
                  //mappingerrors.push_back(atendimento);
                  continue;
               }

               int sala = vit->getSubCjtSala()->salas.begin()->second->getId();

               std::cout << GUtil::intToString(sala, slotSize) << " ";
            }
         }
         std::cout << "| ";
      }
      std::cout << "\n";
   }
   std::cout << line << "\n";

   /*if ((int)mappingerrors.size() > 0)
   {
      std::cout << "Erro de mapeamento nos dados. Atendimentos afetados:\n";
      for (std::vector<Atendimento*>::iterator it = mappingerrors.begin(); it != mappingerrors.end(); ++it)
      {
         Atendimento* atendimento = *it;
         HDisciplina& hdisciplina = (*instance->getDisciplinas())[atendimento->disciplina];


         int ref = hdisciplina.turmas[atendimento->turma-hdisciplina.refTurma];

         int hi = instance->getHorario(atendimento->inicio) - 1;
         int hf = instance->getHorario(atendimento->fim) - 1;

         int dia = instance->getDia(atendimento->inicio);

         DateTime& inicio = horarios[hi];
         DateTime& fim = horarios[hf];

         std::cout << " - { Disciplina = " << atendimento->disciplina << " [" << hdisciplina.disciplina->getId() << "]\t Turma = " << atendimento->turma << " [" << ref << "]\t Inicio = " << atendimento->inicio << " [" << GUtil::intToString(inicio.getHour(), 2, '0', '0') << ":" << GUtil::intToString(inicio.getMinute(), 2, '0', '0') << "]\t Fim = " << atendimento->fim << " [" << GUtil::intToString(fim.getHour(), 2, '0', '0') << ":" << GUtil::intToString(fim.getMinute(), 2, '0', '0') << "] }\n";
      }
   }*/
}

Atendimento* SolverTaticoHeur::getAtendimento(std::vector<Atendimento>& atendimentos, int disciplina, int turma, int instante)
{
   for (std::vector<Atendimento>::iterator it = atendimentos.begin(); it != atendimentos.end(); ++it)
   {
      Atendimento& atendimento = *it;
      if (atendimento.disciplina == disciplina && atendimento.turma == turma && instante >= atendimento.inicio && instante <= atendimento.fim)
         return &atendimento;
   }
   return NULL;
}

bool SolverTaticoHeur::isValidDivisao(Disciplina* disciplina, int dia, int creditos)
{
   int vdisciplina = disciplina->getId();

   std::hash_map<int, std::hash_map<int, std::vector<VariableTaticoSet::iterator>>>& mapVariablesD = mapVariables[vdisciplina];
   if ((int)mapVariablesD.size() == 0)
      return false;

   for (std::hash_map<int, std::hash_map<int, std::vector<VariableTaticoSet::iterator>>>::iterator it1 = mapVariablesD.begin(); it1 != mapVariablesD.end(); ++it1)
   {
      int vturma = it1->first;
      std::hash_map<int, std::vector<VariableTaticoSet::iterator>>& mapVariablesDI = it1->second;
      std::vector<VariableTaticoSet::iterator>& mapVariablesDIT = mapVariablesDI[dia];

      bool found = false;
      for (std::vector<VariableTaticoSet::iterator>::iterator it2 = mapVariablesDIT.begin(); it2 != mapVariablesDIT.end(); ++it2)
      {
         VariableTaticoSet::iterator vit = *it2;
         DateTime& inicio = vit->getHorarioAulaInicial()->getInicio();
         DateTime& fim = vit->getHorarioAulaFinal()->getInicio();
         std::map<DateTime, int>::iterator hiit = mapHorario.find(inicio);
         std::map<DateTime, int>::iterator hfit = mapHorario.find(fim);
         if (hiit == mapHorario.end() || hfit == mapHorario.end())
            return false;
         int hi = hiit->second;
         int hf = hfit->second;
         if (hf - hi + 1 == creditos)
         {
            found = true;
            break;
         }
      }
      if (!found)
         return false;
   }

   return true;
}

void SolverTaticoHeur::writeInstance(std::string filename)
{
	if (instance != NULL)
		instance->writeInstance(filename);
}

bool SolverTaticoHeur::solve(double timeLimit)
{
	clock_t startSolver = clock();

   setSeed(1366857000);
   std::cout << "Seed: " << getSeed() << "\n";

   std::cout << "Starting solver...\n";

   if (solution != NULL)
      delete solution;

   if (instance == NULL || instance->isEmpty())
      return false;

   solution = SolutionBuilder::createRandomSolution(instance, 0.0);

   if (solution == NULL)
      return false;

   //std::cout << "Initial solution: \n" << solution->toString();

   int multiMaxIter  = 5;     // número de iterações do Multi Start MRD
   int ilsMaxIter    = 10;    // número máximo de iterações para o primeiro nível do ILS
   int kpMax         = 1;     // nível máximo de perturbação
   int kpStart       = 1;     // nível inicial da perturbação
   int kpInc         = 1;     // fator de incremento da perturbação
   int lsMaxIter     = 30;    // número de iterações da busca local
   int frequency     = 50;    // frequência de execução do algoritmo de realocação de turmas
   double incFactor  = -0.5;  // fator de incremento do número de iterações do ILS, ao aumentar o nível de perturbação

   bool infeasible = true;
   bool ilsimp = false;

   std::cout << "Starting Iterated Local Search...\n";

   int ilsStart = 0;
   int impStart = 0;

   int count = 0;

   do
   {
      double elapsedTime = GUtil::getDiffClock(clock(), startSolver);

      if (solution->isEmpty() || elapsedTime > timeLimit)
         break;

      solution->ils(ilsMaxIter, kpMax, kpStart, kpInc, lsMaxIter, incFactor, true, startSolver, timeLimit);

      count++;

      if (++impStart % frequency == 0)
      {
         int target = solution->getObjectiveFunction().fo;
         solution->improve(target);
      }
      
      if (solution->feasible())
      {
         ilsimp = true;
         infeasible = false;
      }
      else
      {
         int invi = solution->getObjectiveFunction().inv1 + solution->getObjectiveFunction().inv2;
         if (!solution->unalloc())
            break;
         int invf = solution->getObjectiveFunction().inv1 + solution->getObjectiveFunction().inv2;
         
         elapsedTime = GUtil::getDiffClock(clock(), startSolver);
         std::cout << "Improvement [" << GUtil::intToString(count, 4) << "]: " << GUtil::intToString(invi-invf, 6) << " - FO: " << GUtil::intToString(solution->getObjectiveFunction().fo, 8) << " { INV1: " << GUtil::intToString(solution->getObjectiveFunction().inv1, 6) << " INV2: " << GUtil::intToString(solution->getObjectiveFunction().inv2, 6) << " } - " << solution->getFatorAtendimento() <<  " - Elapsed time: " << GUtil::doubleToString(elapsedTime, 2) << " sec.\n";
      }
   } while (infeasible);

   solution->improve(0);

   if (solution->infeasible())
      solution->makeFeasible();

   std::cout << "MIP Obj Val: " << solution->getMIPObjVal() << "\n";
   //std::cout << solution->toString() << "\n";

   std::string outputfile = "solutionHeur" + GUtil::intToString(getSeed()) + "_" + GUtil::intToString(++runId);

   std::cout << "Wrting solution to file '" << outputfile << "'...\n";
   std::ofstream fout(outputfile.c_str(), std::ios_base::out);
   fout << solution->toString();
   fout.close();

   //if (!isTestingPhase())
   //   printSolution();

   double elapsedTime = GUtil::getDiffClock(clock(), startSolver);
   std::cout << "Total time: " << GUtil::doubleToString(elapsedTime, 2) << " sec.\n";
   std::cout << "Done \n";

   return (solution->feasible());
}

bool SolverTaticoHeur::solve2(double timeLimit)
{
	clock_t startSolver = clock();

   //setSeed(1366857000);
   std::cout << "Seed: " << getSeed() << "\n";

   std::cout << "Starting solver...\n";

   if (solution != NULL)
      delete solution;

   if (instance == NULL || instance->isEmpty())
      return false;

   solution = SolutionBuilder::createRandomSolution(instance, 0.0);

   if (solution == NULL)
      return false;

   //std::cout << "Initial solution: \n" << solution->toString();

   int multiMaxIter  = 5;     // número de iterações do Multi Start MRD
   int ilsMaxIter    = 2;     // número máximo de iterações para o primeiro nível do ILS
   int kpMax         = 1;     // nível máximo de perturbação
   int kpStart       = 1;     // nível inicial da perturbação
   int kpInc         = 1;     // fator de incremento da perturbação
   int lsMaxIter     = 30;    // número de iterações da busca local
   int frequency     = 75;    // frequência de execução do algoritmo de realocação de turmas
   int mipFrequency  = 10;    // frequência de execução do MIP
   double incFactor  = -0.5;  // fator de incremento do número de iterações do ILS, ao aumentar o nível de perturbação

   bool infeasible = true;
   bool ilsimp = false;

   std::cout << "Starting Iterated Local Search...\n";

   int ilsStart = 0;
   int impStart = 0;
   int mipStart = 0;

   int count = 0;

   do
   {
      double elapsedTime = GUtil::getDiffClock(clock(), startSolver);

      if (solution->isEmpty() || elapsedTime > timeLimit)
         break;

      solution->ils(ilsMaxIter, kpMax, kpStart, kpInc, lsMaxIter, incFactor, true, startSolver, timeLimit);

      count++;

      if (++impStart % frequency == 0)
      {
         int target = solution->getObjectiveFunction().fo;
         solution->improve(target);
      }

      if (solution->feasible())
      {
         ilsimp = true;
         infeasible = false;
      }
      else
      {
         if (mipStart++ % mipFrequency == 0)
         {
            double cObjVal = bestObjVal;
            std::cout << "Starting MIP Local Optimization...";
            bool hasSolution = localopt(solution, 300);
            std::cout << "MIP Local Optimization ";
            if (hasSolution)
            {
               if (bestObjVal < cObjVal && bestObjVal < 1e10)
                  std::cout << "has improved the solution by " << (bestObjVal - cObjVal) << " - ObjVal: " << GUtil::intToString((int)cObjVal, 6) << " => " << GUtil::intToString((int)bestObjVal, 6) << "\n";
               else
                  std::cout << "found a solution - ObjVal: " << bestObjVal << "\n";
            }
            else
            {
               std::cout << "does not found a solution - Best ObjVal: " << GUtil::intToString((int)bestObjVal, 6) << "\n";
            }
         }

         int invi = solution->getObjectiveFunction().inv1 + solution->getObjectiveFunction().inv2;
         if (!solution->unalloc())
            break;
         int invf = solution->getObjectiveFunction().inv1 + solution->getObjectiveFunction().inv2;
         
         elapsedTime = GUtil::getDiffClock(clock(), startSolver);
         std::cout << "Improvement [" << GUtil::intToString(count, 4) << "]: " << GUtil::intToString(invi-invf, 6) << " - FO: " << GUtil::intToString(solution->getObjectiveFunction().fo, 8) << " { INV1: " << GUtil::intToString(solution->getObjectiveFunction().inv1, 6) << " INV2: " << GUtil::intToString(solution->getObjectiveFunction().inv2, 6) << " } - " << solution->getFatorAtendimento() <<  " - Elapsed time: " << GUtil::doubleToString(elapsedTime, 2) << " sec.\n";
      }
   } while (infeasible);

   solution->improve(0);

   if (solution->infeasible())
      solution->makeFeasible();

   std::cout << "MIP Obj Val: " << solution->getMIPObjVal() << "\n";

   localopt(solution, 300);

   std::string outputfile = "solutionHeur" + GUtil::intToString(getSeed()) + "_" + GUtil::intToString(++runId);

   std::cout << "Wrting solution to file '" << outputfile << "'...\n";
   std::ofstream fout(outputfile.c_str(), std::ios_base::out);
   fout << solution->toString();
   fout.close();

   double elapsedTime = GUtil::getDiffClock(clock(), startSolver);
   std::cout << "Total time: " << GUtil::doubleToString(elapsedTime, 2) << " sec.\n";
   std::cout << "Done \n";

   return (solution->feasible());
}

bool SolverTaticoHeur::localopt(Solution* s, double timeLimit)
{
   if (vHashTatico == NULL || lp == NULL)
      return false;

   Solution* local = s->clone();
   
   local->makeFeasible();

   std::vector<VariableTatico> solution;
   writeSolution(solution);

   delete local;

   int cntHeur = (int)solution.size();
   if (cntHeur == 0)
      return false;

   // Fixa as variáveis
   for (int vth=0; vth < cntHeur; vth++)
   {
      VariableTatico& vh = solution[vth];
      int turma = vh.getTurma();                                 // i
      Disciplina* disciplina = vh.getDisciplina();               // d
      Unidade* unidade = vh.getUnidade();                        // u
      ConjuntoSala* cjtSala = vh.getSubCjtSala();                // tps
      int dia = vh.getDia();                                     // t
      HorarioAula* horarioInicial = vh.getHorarioAulaInicial();  // hi
      HorarioAula* horarioFinal = vh.getHorarioAulaFinal();      // hf

      VariableTaticoHash::iterator vhit = vHashTatico->find(vh);
      if (vhit != vHashTatico->end())
      {
         int col = vhit->second;
         double value = 1.0;
         lp->chgLB(col, 1.0);
         lp->chgUB(col, 1.0);
      }
   }

   OPTSTAT status = lp->optimize(METHOD_MIP);
   
   bool hasSolution = (status == OPTSTAT_FEASIBLE || status == OPTSTAT_MIPOPTIMAL || status == OPTSTAT_LPOPTIMAL);

   if (hasSolution)
   {
      double fo = lp->getObjVal();
      if (fo < bestObjVal)
      {
         if (xSol == NULL)
            xSol = new double[lp->getNumCols()];
         lp->getX(xSol);
         bestObjVal = fo;
      }
   }

   // Libera os bounds das variáveis
   for (int vth=0; vth < cntHeur; vth++)
   {
      VariableTatico& vh = solution[vth];
      int turma = vh.getTurma();                                 // i
      Disciplina* disciplina = vh.getDisciplina();               // d
      Unidade* unidade = vh.getUnidade();                        // u
      ConjuntoSala* cjtSala = vh.getSubCjtSala();                // tps
      int dia = vh.getDia();                                     // t
      HorarioAula* horarioInicial = vh.getHorarioAulaInicial();  // hi
      HorarioAula* horarioFinal = vh.getHorarioAulaFinal();      // hf

      VariableTaticoHash::iterator vhit = vHashTatico->find(vh);
      if (vhit != vHashTatico->end())
      {
         int col = vhit->second;
         double value = 1.0;
         lp->chgLB(col, 0.0);
         lp->chgUB(col, 1.0);
      }
   }

   return hasSolution;
}
