#include "Instance.h"

#include <iostream>
#include <fstream>

#include "GUtil.h"
#include "..\Disciplina.h"

Instance::Instance(void)
{
   reset();

   this->disciplinas = new std::vector<HDisciplina>();
   this->matrizConflitos = new std::hash_map<int, std::hash_set<int>*>();
   this->matrizConflitosDia = new std::hash_map<int, std::hash_map<int, std::hash_set<int>*>*>();
}

Instance::~Instance(void)
{
   destroy();

   reset();
}

void Instance::destroy()
{
   if (conflitosDisciplinas != NULL)
   {
      int numDisciplinas = getNumeroDisciplinas();
      for (int d=0; d<numDisciplinas; d++)
         delete [] conflitosDisciplinas[d];
      delete [] conflitosDisciplinas;
   }
   this->conflitosDisciplinas = NULL;

   if (matrizConflitos2 != NULL)
   {
      HDisciplina& last = disciplinas->back();
      int n = last.refTurma + last.numTurmas + 1;
      for (int i=0; i<n; i++)
         delete [] matrizConflitos2[i];
      delete [] matrizConflitos2;
   }
   this->matrizConflitos2 = NULL;

   if (matrizConflitosDia2 != NULL)
   {
      int numDias = getNumeroDias() + 1;
      HDisciplina& last = disciplinas->back();
      int n = last.refTurma + last.numTurmas + 1;
      for (int d=0; d<numDias; d++)
      {
         for (int i=0; i<n; i++)
            delete [] matrizConflitosDia2[d][i];
         delete [] matrizConflitosDia2[d];
      }
      delete [] matrizConflitosDia2;
   }
   this->matrizConflitos2 = NULL;

   if (this->matrizConflitos != NULL)
   {
      for (std::hash_map<int, std::hash_set<int>*>::iterator it=matrizConflitos->begin(); it != matrizConflitos->end(); ++it)
      {
         std::hash_set<int>* value = it->second;
         if (value != NULL)
            delete value;
      }
      delete matrizConflitos;
   }
   this->matrizConflitos = NULL;

   if (this->matrizConflitosDia != NULL)
   {
      for (std::hash_map<int, std::hash_map<int, std::hash_set<int>*>*>::iterator it1=matrizConflitosDia->begin(); it1 != matrizConflitosDia->end(); ++it1)
      {
         std::hash_map<int, std::hash_set<int>*>* matrizConflitos1 = it1->second;
         for (std::hash_map<int, std::hash_set<int>*>::iterator it2=matrizConflitos1->begin(); it2 != matrizConflitos1->end(); ++it2)
         {
            std::hash_set<int>* value = it2->second;
            if (value != NULL)
               delete value;
         }
         delete matrizConflitos1;
      }
      delete matrizConflitosDia;
   }
   this->matrizConflitosDia = NULL;

   if (instantes != NULL)
   {
      int numDisciplinas = getNumeroDisciplinas();
      for (int d=0; d<numDisciplinas; d++)
         if (instantes[d] != NULL)
            delete [] instantes[d];
      delete [] instantes;
   }

   if (this->sizes != NULL)
      delete [] sizes;
   this->sizes = NULL;

   if (this->disciplinas != NULL)
      delete disciplinas;
   this->disciplinas = NULL;

}

void Instance::addConflito(int turma1, int turma2)
{
   if (turma1 == turma2)
      return;

   if (matrizConflitos2 == NULL)
   {
      HDisciplina& last = disciplinas->back();
      int n = last.refTurma + last.numTurmas + 1;
      matrizConflitos2 = new bool*[n];
      for (int i=0; i<n; i++)
      {
         matrizConflitos2[i] = new bool[n];
         for (int j=0; j<n; j++)
            matrizConflitos2[i][j] = (i == j);
      }
   }

   matrizConflitos2[turma1][turma2] = true;
   matrizConflitos2[turma2][turma1] = true;

   //int t1 = (turma1 < turma2 ? turma1 : turma2);
   //int t2 = (turma1 < turma2 ? turma2 : turma1);

   /*std::hash_set<int>* conflitos = NULL;

   std::hash_map<int, std::hash_set<int>*>::iterator it = matrizConflitos->find(t1);
   if (it == matrizConflitos->end())
   {
      conflitos = new std::hash_set<int>();
      (*matrizConflitos)[t1] = conflitos;
   }
   else
      conflitos = it->second;

   conflitos->insert(t2);*/
}

std::hash_set<int>* Instance::getConflitos(int turma)
{
   std::hash_map<int, std::hash_set<int>*>::iterator it = matrizConflitos->find(turma);
   if (it != matrizConflitos->end())
      return it->second;
   return NULL;
}

void Instance::addConflito(int dia, int turma1, int turma2)
{
   if (turma1 == turma2)
      return;

   if (matrizConflitosDia2 == NULL)
   {
      int numDias = getNumeroDias() + 1;
      HDisciplina& last = disciplinas->back();
      int n = last.refTurma + last.numTurmas + 1;
      matrizConflitosDia2 = new bool**[numDias];
      for (int d=0; d<numDias; d++)
      {
         matrizConflitosDia2[d] = new bool*[n];
         for (int i=0; i<n; i++)
         {
            matrizConflitosDia2[d][i] = new bool[n];
            for (int j=0; j<n; j++)
               matrizConflitosDia2[d][i][j] = (i == j);
         }
      }
   }

   matrizConflitosDia2[dia][turma1][turma2] = true;
   matrizConflitosDia2[dia][turma2][turma1] = true;

   /*int t1 = (turma1 < turma2 ? turma1 : turma2);
   int t2 = (turma1 < turma2 ? turma2 : turma1);

   std::hash_map<int, std::hash_set<int>*>* matrizConflitos = NULL;

   std::hash_map<int, std::hash_map<int, std::hash_set<int>*>*>::iterator it1 = matrizConflitosDia->find(dia);
   if (it1 == matrizConflitosDia->end())
   {
      matrizConflitos = new std::hash_map<int, std::hash_set<int>*>();
      (*matrizConflitosDia)[dia] = matrizConflitos;
   }
   else
      matrizConflitos = it1->second;

   std::hash_set<int>* conflitos = NULL;

   std::hash_map<int, std::hash_set<int>*>::iterator it2 = matrizConflitos->find(t1);
   if (it2 == matrizConflitos->end())
   {
      conflitos = new std::hash_set<int>();
      (*matrizConflitos)[t1] = conflitos;
   }
   else
      conflitos = it2->second;

   conflitos->insert(t2);*/
}

std::hash_set<int>* Instance::getConflitos(int dia, int turma)
{
   std::hash_map<int, std::hash_map<int, std::hash_set<int>*>*>::iterator it1 = matrizConflitosDia->find(dia);
   if (it1 == matrizConflitosDia->end())
      return NULL;
   
   std::hash_map<int, std::hash_set<int>*>::iterator it2 = it1->second->find(turma);
   if (it2 != it1->second->end())
      return it2->second;
   return NULL;
}

void Instance::reset()
{
   this->numDias = 0;
   this->numHorarios = 0;
   this->matrizConflitos2 = NULL;
   this->matrizConflitosDia2 = NULL;
   this->conflitosDisciplinas = NULL;
   this->instantes = NULL;
   this->sizes = NULL;

}

std::string Instance::toString()
{
   std::string out = "[Instance]\n";
   out += " - Numero de disciplinas: " + GUtil::intToString(getNumeroDisciplinas()) + "\n";
   out += " - Numero de dias: " + GUtil::intToString(getNumeroDias()) + "\n";
   out += " - Numero de horarios: " + GUtil::intToString(getNumeroHorarios()) + "\n";
   out += " - Numero de instantes: " + GUtil::intToString(getNumeroInstantes()) + "\n";
   out += " - Disciplinas: \n";
   int turmaId = 1;
   for (int i=0; i<(int)disciplinas->size(); i++)
   {
      HDisciplina& disciplina = (*disciplinas)[i];
      out += "   > Disciplina " + GUtil::intToString(i+1) + "\n";
      out += "     # Creditos: " + GUtil::intToString(disciplina.numCreditos) + "\n";
      out += "     # Turmas: " + GUtil::intToString(disciplina.numTurmas) + " { ";
      for (int l=0; l<disciplina.numTurmas; l++)
      {
         out += GUtil::intToString(turmaId) + " ";
         turmaId++;
      }
      out += "}\n";
      out += "     # Divisoes: \n";
      for (int j=0; j<(int)disciplina.divisoes.size(); j++)
      {
         out += "       ~ ";
         for (int k=0; k<(int)disciplina.divisoes[j].dia.size(); k++)
            out += "(" + GUtil::intToString(disciplina.divisoes[j].dia[k]) + ", " + GUtil::intToString(disciplina.divisoes[j].creditos[k]) + ") ";
         out += "\n";
      }
   }
   out += " - Conflitos: \n";

   for (std::hash_map<int, std::hash_set<int>*>::iterator it = matrizConflitos->begin(); it != matrizConflitos->end(); ++it)
   {
      int hturma1 = it->first;
      out += "    > Turma " + GUtil::intToString(hturma1) + ": ";
      std::hash_set<int>* tconflitos = it->second;
      for (std::hash_set<int>::iterator it2 = tconflitos->begin(); it2 != tconflitos->end(); ++it2)
      {
         int hturma2 = *it2;
         out += GUtil::intToString(hturma2) + " ";
      }
      out += "\n";
   }

   out += " - Conflitos Dia: \n";

   for (std::hash_map<int, std::hash_map<int, std::hash_set<int>*>*>::iterator it1 = matrizConflitosDia->begin(); it1 != matrizConflitosDia->end(); ++it1)
   {
      out += "    > Dia " + GUtil::intToString(it1->first);
      for (std::hash_map<int, std::hash_set<int>*>::iterator it = it1->second->begin(); it != it1->second->end(); ++it)
      {
         int hturma1 = it->first;
         out += ": Turma " + GUtil::intToString(hturma1) + ": ";
         std::hash_set<int>* tconflitos = it->second;
         for (std::hash_set<int>::iterator it2 = tconflitos->begin(); it2 != tconflitos->end(); ++it2)
         {
            int hturma2 = *it2;
            out += GUtil::intToString(hturma2) + " ";
         }
         out += "\n";
      }
   }

   return out;
}

void Instance::buildRefTurma()
{
   int ref = 1;
   for (int i=0; i<(int)disciplinas->size(); i++)
   {
      HDisciplina& disciplina = (*disciplinas)[i];
      disciplina.refTurma = ref;
      disciplina.disciplinaAssoc = -1;
      ref += disciplina.numTurmas;
   }

   int n1 = (int)disciplinas->size() - 1;
   int n2 = (int)disciplinas->size();
   for (int i1=0; i1<n1; i1++)
   {
      HDisciplina& disciplina1 = (*disciplinas)[i1];
      int d1 = abs(disciplina1.disciplina->getId());
      for (int i2=i1+1; i2<n2; i2++)
      {
         HDisciplina& disciplina2 = (*disciplinas)[i2];
         int d2 = abs(disciplina2.disciplina->getId());
         if (d1 == d2)
         {
            disciplina1.disciplinaAssoc = i2;
            disciplina2.disciplinaAssoc = i1;
            break;
         }
      }
   }
}

bool Instance::isHorarioDisponivel(int disciplina, int instante)
{
   int dia = getDia(instante);
   int horario = getHorario(instante);
   return isHorarioDisponivel(disciplina, dia, horario);
}

bool Instance::isHorarioDisponivel(int disciplina, int dia, int horario)
{
   HDisciplina& disc = (*disciplinas)[disciplina];
   std::hash_set<int>& dhorarios = disc.horarios[dia];
   return (dhorarios.find(horario - 1) != dhorarios.end());
}

bool Instance::isHorarioDisponivel(int disciplina, int dia, int turma, int hi, int hf)
{
   HDisciplina& disc = (*disciplinas)[disciplina];
   std::hash_map<int, std::hash_map<int, std::hash_set<int>>>& dhihfvalidos = disc.hihfvalidos[dia];
   std::hash_map<int, std::hash_set<int>>& thihfvalidos = dhihfvalidos[turma];
   std::hash_set<int>& dhfvalidos = thihfvalidos[hi];
   return (dhfvalidos.find(hf) != dhfvalidos.end());
}

std::pair<int, int> Instance::getDemandaTotal()
{
   int numDisciplinas = getNumeroDisciplinas();
   int numTurmas = (*this->disciplinas)[numDisciplinas-1].refTurma + (*this->disciplinas)[numDisciplinas-1].numTurmas - 1;
   return std::pair<int, int>(numDisciplinas, numTurmas);
}

int Instance::getTotalAlunos()
{
   int numAlunos = 0;
   for (std::vector<HDisciplina>::iterator it=disciplinas->begin(); it != disciplinas->end(); ++it)
   {
      HDisciplina& disciplina = *it;
      for (std::vector<int>::iterator it2 = disciplina.numAlunos.begin(); it2 != disciplina.numAlunos.end(); ++it2)
         numAlunos += *it2;
   }
   return numAlunos;
}

void Instance::writeInstance(std::string filename)
{
   if (isEmpty())
      return;

   std::ofstream fout(filename.c_str(), std::ios_base::out);
   fout << numDias << "\n";
   fout << numHorarios << "\n";
   fout << disciplinas->size() << "\n";
   for (int d=0; d<(int)disciplinas->size(); d++)
   {
      HDisciplina& disciplina = (*disciplinas)[d];
      fout << disciplina.refTurma << "\n";
      fout << disciplina.numTurmas << "\n";
      fout << disciplina.numCreditos << "\n";
      fout << disciplina.disciplinaAssoc << "\n";
      fout << disciplina.divisoes.size() << "\n";
      for (std::vector<Divisao>::iterator it = disciplina.divisoes.begin(); it != disciplina.divisoes.end(); ++it)
      {
         Divisao& divisao = *it;
         fout << divisao.dia.size() << "\n";
         for (int k=0; k<(int)divisao.dia.size(); k++)
            fout << divisao.dia[k] << "\t";
         fout << "\n";
         for (int k=0; k<(int)divisao.dia.size(); k++)
            fout << divisao.creditos[k] << "\t";
         fout << "\n";
      }
      fout << disciplina.turmas.size() << "\n";
      for (std::vector<int>::iterator it = disciplina.turmas.begin(); it != disciplina.turmas.end(); ++it)
         fout << *it << "\t";
      fout << "\n";
      fout << disciplina.numAlunos.size() << "\n";
      for (std::vector<int>::iterator it = disciplina.numAlunos.begin(); it != disciplina.numAlunos.end(); ++it)
         fout << *it << "\t";
      fout << "\n";
      fout << disciplina.horarios.size() << "\n";
      for (std::hash_map<int, std::hash_set<int>>::iterator it1 = disciplina.horarios.begin(); it1 != disciplina.horarios.end(); ++it1)
      {
         fout << it1->first << "\n";
         fout << it1->second.size() << "\n";
         std::hash_set<int>& shorarios = it1->second;
         for (std::hash_set<int>::iterator it2 = shorarios.begin(); it2 != shorarios.end(); ++it2)
            fout << *it2 << "\t";
         fout << "\n";
      }
      fout << disciplina.hihfvalidos.size() << "\n";
         std::hash_map<int, std::hash_map<int, std::hash_map<int, std::hash_set<int>>>> hihfvalidos;

      for (std::hash_map<int, std::hash_map<int, std::hash_map<int, std::hash_set<int>>>>::iterator it1 = disciplina.hihfvalidos.begin(); it1 != disciplina.hihfvalidos.end(); ++it1)
      {
         fout << it1->first << "\n";
         fout << it1->second.size() << "\n";
         std::hash_map<int, std::hash_map<int, std::hash_set<int>>>& hihfvalidos1 = it1->second;

         for (std::hash_map<int, std::hash_map<int, std::hash_set<int>>>::iterator it2 = hihfvalidos1.begin(); it2 != hihfvalidos1.end(); ++it2)
         {
            fout << it2->first << "\n";
            fout << it2->second.size() << "\n";
            std::hash_map<int, std::hash_set<int>>& hihfvalidos2 = it2->second;

            for (std::hash_map<int, std::hash_set<int>>::iterator it3 = hihfvalidos2.begin(); it3 != hihfvalidos2.end(); ++it3)
            {
               fout << it3->first << "\n";
               fout << it3->second.size() << "\n";
               std::hash_set<int>& hihfvalidos3 = it3->second;
               for (std::hash_set<int>::iterator it4 = hihfvalidos3.begin(); it4 != hihfvalidos3.end(); ++it4)
                  fout << *it4 << "\t";
               fout << "\n";
            }
         }
      }
   }

   fout << matrizConflitos->size() << "\n";
   for (std::hash_map<int, std::hash_set<int>*>::iterator it = matrizConflitos->begin(); it != matrizConflitos->end(); ++it)
   {
      fout << it->first << "\n";
      fout << it->second->size() << "\n";
      std::hash_set<int>* matrizConflitos1 = it->second;
      for (std::hash_set<int>::iterator it2 = matrizConflitos1->begin(); it2 != matrizConflitos1->end(); ++it2)
         fout << *it2 << "\t";
      fout << "\n";
   }

   fout << matrizConflitosDia->size() << "\n";
   for (std::hash_map<int, std::hash_map<int, std::hash_set<int>*>*>::iterator it1 = matrizConflitosDia->begin(); it1 != matrizConflitosDia->end(); ++it1)
   {
      fout << it1->first << "\n";
      fout << it1->second->size() << "\n";
      std::hash_map<int, std::hash_set<int>*>* matrizConflitosDia1 = it1->second;
      for (std::hash_map<int, std::hash_set<int>*>::iterator it2 = matrizConflitosDia1->begin(); it2 != matrizConflitosDia1->end(); ++it2)
      {
         fout << it2->first << "\n";
         fout << it2->second->size() << "\n";
         std::hash_set<int>* matrizConflitosDia2 = it2->second;
         for (std::hash_set<int>::iterator it3 = matrizConflitosDia2->begin(); it3 != matrizConflitosDia2->end(); ++it3)
            fout << *it3 << "\t";
         fout << "\n";
      }
   }

   fout.close();
}

void Instance::readInstance(std::string filename)
{
   destroy();

   this->disciplinas = new std::vector<HDisciplina>();
   this->matrizConflitos = new std::hash_map<int, std::hash_set<int>*>();
   this->matrizConflitosDia = new std::hash_map<int, std::hash_map<int, std::hash_set<int>*>*>();
   this->matrizConflitos2 = NULL;
   this->matrizConflitosDia2 = NULL;

   int numDisciplinas = 0;

   std::ifstream fin(filename.c_str(), std::ios_base::in);
   fin >> numDias;
   fin >> numHorarios;
   fin >> numDisciplinas;

   for (int d=0; d<numDisciplinas; d++)
   {
      HDisciplina disciplina;
      fin >> disciplina.refTurma;
      fin >> disciplina.numTurmas;
      fin >> disciplina.numCreditos;
      fin >> disciplina.disciplinaAssoc;

      int numDivisoes = 0;
      fin >> numDivisoes;

      for (int k=0; k<numDivisoes; k++)
      {
         Divisao divisao;
         int nd = 0;
         fin >> nd;
         for (int k=0; k<nd; k++)
         {
            int dia = 0;
            fin >> dia;
            divisao.dia.push_back(dia);
         }
         for (int k=0; k<nd; k++)
         {
            int creditos = 0;
            fin >> creditos;
            divisao.creditos.push_back(creditos);
         }
         disciplina.divisoes.push_back(divisao);
      }

      int numTurmas = 0;
      fin >> numTurmas;
      for (int k=0; k<numTurmas; k++)
      {
         int turma = 0;
         fin >> turma;
         disciplina.turmas.push_back(turma);
      }

      int numAlunos = 0;
      fin >> numAlunos;
      for (int k=0; k<numAlunos; k++)
      {
         int alunos = 0;
         fin >> alunos;
         disciplina.numAlunos.push_back(alunos);
      }

      int numHorariosD = 0;
      fin >> numHorariosD;
      for (int k=0; k<numHorariosD; k++)
      {
         int key1 = 0;
         int size1 = 0;
         fin >> key1;
         fin >> size1;
         std::hash_set<int>& shorarios = disciplina.horarios[key1];
         for (int k1=0; k1<size1; k1++)
         {
            int value = 0;
            fin >> value;
            shorarios.insert(value);
         }
      }

      int hihfsize = 0;
      fin >> hihfsize;
      for (int k=0; k<hihfsize; k++)
      {
         int key1 = 0;
         int size1 = 0;
         fin >> key1;
         fin >> size1;
         std::hash_map<int, std::hash_map<int, std::hash_set<int>>>& hihfvalidos1 = disciplina.hihfvalidos[key1];

         for (int k1=0; k1<size1; k1++)
         {
            int key2 = 0;
            int size2 = 0;
            fin >> key2;
            fin >> size2;
            std::hash_map<int, std::hash_set<int>>& hihfvalidos2 = hihfvalidos1[key2];

            for (int k2=0; k2<size2; k2++)
            {
               int key3 = 0;
               int size3 = 0;
               fin >> key3;
               fin >> size3;
               std::hash_set<int>& hihfvalidos3 = hihfvalidos2[key3];
               for (int k3=0; k3<size3; k3++)
               {
                  int value = 0;
                  fin >> value;
                  hihfvalidos3.insert(value);
               }
            }
         }
      }

      disciplinas->push_back(disciplina);
   }

   int matrizConflitosSize = 0;
   fin >> matrizConflitosSize;
   for (int k=0; k<matrizConflitosSize; k++)
   {
      int key1 = 0;
      int size1 = 0;
      fin >> key1;
      fin >> size1;
      std::hash_set<int>* matrizConflitos1 = new std::hash_set<int>();
      (*matrizConflitos)[key1] = matrizConflitos1;
      for (int k1=0; k1<size1; k1++)
      {
         int value = 0;
         fin >> value;
         matrizConflitos1->insert(value);
         addConflito(key1, value);
      }
   }

   int matrizConflitosDiaSize = 0;
   fin >> matrizConflitosDiaSize;
   for (int k=0; k<matrizConflitosDiaSize; k++)
   {
      int key1 = 0;
      int size1 = 0;
      fin >> key1;
      fin >> size1;
      std::hash_map<int, std::hash_set<int>*>* matrizConflitosDia1 = new std::hash_map<int, std::hash_set<int>*>();
      (*matrizConflitosDia)[key1] = matrizConflitosDia1;
      for (int k1=0; k1<size1; k1++)
      {
         int key2 = 0;
         int size2 = 0;
         fin >> key2;
         fin >> size2;

         std::hash_set<int>* matrizConflitosDia2 = new std::hash_set<int>();
         (*matrizConflitosDia1)[key2] = matrizConflitosDia2;

         for (int k2=0; k2<size2; k2++)
         {
            int value = 0;
            fin >> value;
            matrizConflitosDia2->insert(value);
            addConflito(key1, key2, value);
         }
      }
   }

   fin.close();

   buildDivisoesDefault();
   buildDivisoes();
   preprocess();
}

void Instance::buildDivisoesDefault()
{
   int numDias = getNumeroDias();

   // Divisão trivial
   for (int creditos=1; creditos<=12; creditos++)
   {
      std::vector<Divisao>& divisoes = divisoesDefault[creditos];
      for (int dia=1; dia<=numDias; dia++)
      {
         Divisao divisao11;
         divisao11.dia.push_back(dia);   divisao11.creditos.push_back(creditos);
         divisoes.push_back(divisao11);
      }
   }

   // Número de créditos: 3
   std::vector<Divisao>& divisao3 = divisoesDefault[3];
   for (int dia1=1; dia1<=numDias; dia1++)
   {
      for (int dia2=1; dia2<=numDias; dia2++)
      {
         if (dia1 == dia2)
            continue;

         Divisao divisao21;
         divisao21.dia.push_back(dia1);  divisao21.creditos.push_back(1);
         divisao21.dia.push_back(dia2);  divisao21.creditos.push_back(2);
         divisao3.push_back(divisao21);
      }
   }

   // Número de créditos: 4
   std::vector<Divisao>& divisao4 = divisoesDefault[4];
   for (int dia1=1; dia1<=numDias; dia1++)
   {
      for (int dia2=1; dia2<=numDias; dia2++)
      {
         if (dia1 == dia2)
            continue;

         if (dia1 < dia2)
         {
            Divisao divisao21;
            divisao21.dia.push_back(dia1);  divisao21.creditos.push_back(2);
            divisao21.dia.push_back(dia2);  divisao21.creditos.push_back(2);
            divisao4.push_back(divisao21);
         }
      }
   }

   // Número de créditos: 5
   std::vector<Divisao>& divisao5 = divisoesDefault[5];
   for (int dia1=1; dia1<=numDias; dia1++)
   {
      for (int dia2=1; dia2<=numDias; dia2++)
      {
         if (dia1 == dia2)
            continue;

         Divisao divisao21;
         divisao21.dia.push_back(dia1); divisao21.creditos.push_back(2);
         divisao21.dia.push_back(dia2); divisao21.creditos.push_back(3);
         divisao5.push_back(divisao21);

         Divisao divisao22;
         divisao22.dia.push_back(dia1); divisao22.creditos.push_back(1);
         divisao22.dia.push_back(dia2); divisao22.creditos.push_back(4);
         divisao5.push_back(divisao22);

         for (int dia3=1; dia3<=numDias; dia3++)
         {
            if (dia1 == dia3 || dia2 == dia3)
               continue;

            if (dia2 < dia3)
            {
               Divisao divisao31;
               divisao31.dia.push_back(dia1); divisao31.creditos.push_back(1);
               divisao31.dia.push_back(dia2); divisao31.creditos.push_back(2);
               divisao31.dia.push_back(dia3); divisao31.creditos.push_back(2);
               divisao5.push_back(divisao31);
            }
         }
      }
   }

   // Número de créditos: 6
   std::vector<Divisao>& divisao6 = divisoesDefault[6];
   for (int dia1=1; dia1<=numDias; dia1++)
   {
      for (int dia2=1; dia2<=numDias; dia2++)
      {
         if (dia1 == dia2)
            continue;

         Divisao divisao21;
         divisao21.dia.push_back(dia1); divisao21.creditos.push_back(2);
         divisao21.dia.push_back(dia2); divisao21.creditos.push_back(4);
         divisao6.push_back(divisao21);

         if (dia1 < dia2)
         {
            Divisao divisao22;
            divisao22.dia.push_back(dia1); divisao22.creditos.push_back(3);
            divisao22.dia.push_back(dia2); divisao22.creditos.push_back(3);
            divisao6.push_back(divisao22);
         }

         for (int dia3=1; dia3<=numDias; dia3++)
         {
            if (dia1 == dia3 || dia2 == dia3)
               continue;

            if (dia1 < dia2 && dia2 < dia3)
            {
               Divisao divisao31;
               divisao31.dia.push_back(dia1); divisao31.creditos.push_back(2);
               divisao31.dia.push_back(dia2); divisao31.creditos.push_back(2);
               divisao31.dia.push_back(dia3); divisao31.creditos.push_back(2);
               divisao6.push_back(divisao31);
            }
         }
      }
   }

   // Número de créditos: 7
   std::vector<Divisao>& divisao7 = divisoesDefault[7];
   for (int dia1=1; dia1<=numDias; dia1++)
   {
      for (int dia2=1; dia2<=numDias; dia2++)
      {
         if (dia1 == dia2)
            continue;

         Divisao divisao21;
         divisao21.dia.push_back(dia1); divisao21.creditos.push_back(1);
         divisao21.dia.push_back(dia2); divisao21.creditos.push_back(6);
         divisao7.push_back(divisao21);

         Divisao divisao22;
         divisao22.dia.push_back(dia1); divisao22.creditos.push_back(2);
         divisao22.dia.push_back(dia2); divisao22.creditos.push_back(5);
         divisao7.push_back(divisao22);

         Divisao divisao23;
         divisao23.dia.push_back(dia1); divisao23.creditos.push_back(3);
         divisao23.dia.push_back(dia2); divisao23.creditos.push_back(4);
         divisao7.push_back(divisao23);

         for (int dia3=1; dia3<=numDias; dia3++)
         {
            if (dia1 == dia3 || dia2 == dia3)
               continue;

            Divisao divisao31;
            divisao31.dia.push_back(dia1); divisao31.creditos.push_back(1);
            divisao31.dia.push_back(dia2); divisao31.creditos.push_back(2);
            divisao31.dia.push_back(dia3); divisao31.creditos.push_back(4);
            divisao7.push_back(divisao31);

            if (dia2 < dia3)
            {
               Divisao divisao32;
               divisao32.dia.push_back(dia1); divisao32.creditos.push_back(1);
               divisao32.dia.push_back(dia2); divisao32.creditos.push_back(3);
               divisao32.dia.push_back(dia3); divisao32.creditos.push_back(3);
               divisao7.push_back(divisao32);
            }

            if (dia1 < dia2)
            {
               Divisao divisao33;
               divisao33.dia.push_back(dia1); divisao33.creditos.push_back(2);
               divisao33.dia.push_back(dia2); divisao33.creditos.push_back(2);
               divisao33.dia.push_back(dia3); divisao33.creditos.push_back(3);
               divisao7.push_back(divisao33);
            }
         }
      }
   }

   // Número de créditos: 8
   std::vector<Divisao>& divisao8 = divisoesDefault[8];
   for (int dia1=1; dia1<=numDias; dia1++)
   {
      for (int dia2=1; dia2<=numDias; dia2++)
      {
         if (dia1 == dia2)
            continue;

         Divisao divisao21;
         divisao21.dia.push_back(dia1); divisao21.creditos.push_back(2);
         divisao21.dia.push_back(dia2); divisao21.creditos.push_back(6);
         divisao8.push_back(divisao21);

         Divisao divisao22;
         divisao22.dia.push_back(dia1); divisao22.creditos.push_back(3);
         divisao22.dia.push_back(dia2); divisao22.creditos.push_back(5);
         divisao8.push_back(divisao22);

         if (dia1 < dia2)
         {
            Divisao divisao23;
            divisao23.dia.push_back(dia1); divisao23.creditos.push_back(4);
            divisao23.dia.push_back(dia2); divisao23.creditos.push_back(4);
            divisao8.push_back(divisao23);
         }

         for (int dia3=1; dia3<=numDias; dia3++)
         {
            if (dia1 == dia3 || dia2 == dia3)
               continue;

            if (dia1 < dia2)
            {
               Divisao divisao31;
               divisao31.dia.push_back(dia1); divisao31.creditos.push_back(2);
               divisao31.dia.push_back(dia2); divisao31.creditos.push_back(2);
               divisao31.dia.push_back(dia3); divisao31.creditos.push_back(4);
               divisao8.push_back(divisao31);
            }

            if (dia2 < dia3)
            {
               Divisao divisao32;
               divisao32.dia.push_back(dia1); divisao32.creditos.push_back(2);
               divisao32.dia.push_back(dia2); divisao32.creditos.push_back(3);
               divisao32.dia.push_back(dia3); divisao32.creditos.push_back(3);
               divisao8.push_back(divisao32);
            }

            for (int dia4=1; dia4<=numDias; dia4++)
            {
               if (dia1 == dia4 || dia2 == dia4 || dia3 == dia4)
                  continue;

               if (dia1 < dia2 && dia2 < dia3 && dia3 < dia4)
               {
                  Divisao divisao41;
                  divisao41.dia.push_back(dia1); divisao41.creditos.push_back(2);
                  divisao41.dia.push_back(dia2); divisao41.creditos.push_back(2);
                  divisao41.dia.push_back(dia3); divisao41.creditos.push_back(2);
                  divisao41.dia.push_back(dia4); divisao41.creditos.push_back(2);
                  divisao8.push_back(divisao41);
               }
            }
         }
      }
   }

   // Número de créditos: 12
   std::vector<Divisao>& divisao12 = divisoesDefault[12];
   for (int dia1=1; dia1<=numDias; dia1++)
   {
      for (int dia2=1; dia2<=numDias; dia2++)
      {
         if (dia1 == dia2)
            continue;

         Divisao divisao21;
         divisao21.dia.push_back(dia1); divisao21.creditos.push_back(2);
         divisao21.dia.push_back(dia2); divisao21.creditos.push_back(10);
         divisao12.push_back(divisao21);

         Divisao divisao22;
         divisao22.dia.push_back(dia1); divisao22.creditos.push_back(3);
         divisao22.dia.push_back(dia2); divisao22.creditos.push_back(9);
         divisao12.push_back(divisao22);

         Divisao divisao23;
         divisao23.dia.push_back(dia1); divisao23.creditos.push_back(4);
         divisao23.dia.push_back(dia2); divisao23.creditos.push_back(8);
         divisao12.push_back(divisao23);

         Divisao divisao24;
         divisao24.dia.push_back(dia1); divisao24.creditos.push_back(5);
         divisao24.dia.push_back(dia2); divisao24.creditos.push_back(7);
         divisao12.push_back(divisao24);

         if (dia1 < dia2)
         {
            Divisao divisao25;
            divisao25.dia.push_back(dia1); divisao25.creditos.push_back(6);
            divisao25.dia.push_back(dia2); divisao25.creditos.push_back(6);
            divisao12.push_back(divisao25);
         }

         for (int dia3=1; dia3<=numDias; dia3++)
         {
            if (dia1 == dia3 || dia2 == dia3)
               continue;

            if (dia1 < dia2)
            {
               Divisao divisao31;
               divisao31.dia.push_back(dia1); divisao31.creditos.push_back(2);
               divisao31.dia.push_back(dia2); divisao31.creditos.push_back(2);
               divisao31.dia.push_back(dia3); divisao31.creditos.push_back(8);
               divisao12.push_back(divisao31);
            }

            Divisao divisao32;
            divisao32.dia.push_back(dia1); divisao32.creditos.push_back(2);
            divisao32.dia.push_back(dia2); divisao32.creditos.push_back(3);
            divisao32.dia.push_back(dia3); divisao32.creditos.push_back(7);
            divisao12.push_back(divisao32);

            Divisao divisao33;
            divisao33.dia.push_back(dia1); divisao33.creditos.push_back(2);
            divisao33.dia.push_back(dia2); divisao33.creditos.push_back(4);
            divisao33.dia.push_back(dia3); divisao33.creditos.push_back(6);
            divisao12.push_back(divisao33);

            if (dia2 < dia3)
            {
               Divisao divisao34;
               divisao34.dia.push_back(dia1); divisao34.creditos.push_back(2);
               divisao34.dia.push_back(dia2); divisao34.creditos.push_back(5);
               divisao34.dia.push_back(dia3); divisao34.creditos.push_back(5);
               divisao12.push_back(divisao34);
            }

            if (dia1 < dia2)
            {
               Divisao divisao35;
               divisao35.dia.push_back(dia1); divisao35.creditos.push_back(3);
               divisao35.dia.push_back(dia2); divisao35.creditos.push_back(3);
               divisao35.dia.push_back(dia3); divisao35.creditos.push_back(6);
               divisao12.push_back(divisao35);
            }

            Divisao divisao36;
            divisao36.dia.push_back(dia1); divisao36.creditos.push_back(3);
            divisao36.dia.push_back(dia2); divisao36.creditos.push_back(4);
            divisao36.dia.push_back(dia3); divisao36.creditos.push_back(5);
            divisao12.push_back(divisao36);

            for (int dia4=1; dia4<=numDias; dia4++)
            {
               if (dia1 == dia4 || dia2 == dia4 || dia3 == dia4)
                  continue;

               if (dia1 < dia2 && dia2 < dia3)
               {
                  Divisao divisao41;
                  divisao41.dia.push_back(dia1); divisao41.creditos.push_back(2);
                  divisao41.dia.push_back(dia2); divisao41.creditos.push_back(2);
                  divisao41.dia.push_back(dia3); divisao41.creditos.push_back(2);
                  divisao41.dia.push_back(dia4); divisao41.creditos.push_back(6);
                  divisao12.push_back(divisao41);
               }

               if (dia1 < dia2)
               {
                  Divisao divisao42;
                  divisao42.dia.push_back(dia1); divisao42.creditos.push_back(2);
                  divisao42.dia.push_back(dia2); divisao42.creditos.push_back(2);
                  divisao42.dia.push_back(dia3); divisao42.creditos.push_back(3);
                  divisao42.dia.push_back(dia4); divisao42.creditos.push_back(5);
                  divisao12.push_back(divisao42);
               }

               if (dia1 < dia2 && dia3 < dia4)
               {
                  Divisao divisao43;
                  divisao43.dia.push_back(dia1); divisao43.creditos.push_back(2);
                  divisao43.dia.push_back(dia2); divisao43.creditos.push_back(2);
                  divisao43.dia.push_back(dia3); divisao43.creditos.push_back(4);
                  divisao43.dia.push_back(dia4); divisao43.creditos.push_back(4);
                  divisao12.push_back(divisao43);
               }

               if (dia2 < dia3)
               {
                  Divisao divisao44;
                  divisao44.dia.push_back(dia1); divisao44.creditos.push_back(2);
                  divisao44.dia.push_back(dia2); divisao44.creditos.push_back(3);
                  divisao44.dia.push_back(dia3); divisao44.creditos.push_back(3);
                  divisao44.dia.push_back(dia4); divisao44.creditos.push_back(4);
                  divisao12.push_back(divisao44);
               }

               if (dia1 < dia2 && dia2 < dia3 && dia3 < dia4)
               {
                  Divisao divisao45;
                  divisao45.dia.push_back(dia1); divisao45.creditos.push_back(3);
                  divisao45.dia.push_back(dia2); divisao45.creditos.push_back(3);
                  divisao45.dia.push_back(dia3); divisao45.creditos.push_back(3);
                  divisao45.dia.push_back(dia4); divisao45.creditos.push_back(3);
                  divisao12.push_back(divisao45);
               }

               for (int dia5=1; dia5<=numDias; dia5++)
               {
                  if (dia1 == dia5 || dia2 == dia5 || dia3 == dia5 || dia4 == dia5)
                     continue;

                  if (dia1 < dia2 && dia2 < dia3 && dia3 < dia4)
                  {
                     Divisao divisao51;
                     divisao51.dia.push_back(dia1); divisao51.creditos.push_back(2);
                     divisao51.dia.push_back(dia2); divisao51.creditos.push_back(2);
                     divisao51.dia.push_back(dia3); divisao51.creditos.push_back(2);
                     divisao51.dia.push_back(dia4); divisao51.creditos.push_back(2);
                     divisao51.dia.push_back(dia5); divisao51.creditos.push_back(4);
                     divisao12.push_back(divisao51);
                  }

                  if (dia1 < dia2 && dia2 < dia3 && dia4 < dia5)
                  {
                     Divisao divisao52;
                     divisao52.dia.push_back(dia1); divisao52.creditos.push_back(2);
                     divisao52.dia.push_back(dia2); divisao52.creditos.push_back(2);
                     divisao52.dia.push_back(dia3); divisao52.creditos.push_back(2);
                     divisao52.dia.push_back(dia4); divisao52.creditos.push_back(3);
                     divisao52.dia.push_back(dia5); divisao52.creditos.push_back(3);
                     divisao12.push_back(divisao52);
                  }

                  for (int dia6=1; dia6<=numDias; dia6++)
                  {
                     if (dia1 == dia6 || dia2 == dia6 || dia3 == dia6 || dia4 == dia6 || dia5 == dia6)
                        continue;

                     if (dia1 < dia2 && dia2 < dia3 && dia3 < dia4 && dia4 < dia5 && dia5 < dia6)
                     {
                        Divisao divisao61;
                        divisao61.dia.push_back(dia1); divisao61.creditos.push_back(2);
                        divisao61.dia.push_back(dia2); divisao61.creditos.push_back(2);
                        divisao61.dia.push_back(dia3); divisao61.creditos.push_back(2);
                        divisao61.dia.push_back(dia4); divisao61.creditos.push_back(2);
                        divisao61.dia.push_back(dia5); divisao61.creditos.push_back(2);
                        divisao61.dia.push_back(dia6); divisao61.creditos.push_back(2);
                        divisao12.push_back(divisao61);
                     }
                  }
               }
            }
         }
      }
   }
}

void Instance::buildDivisoes()
{
   for (std::vector<HDisciplina>::iterator it = disciplinas->begin(); it != disciplinas->end(); ++it)
   {
      HDisciplina& disciplina = *it;
      if (disciplina.divisoes.empty())
      {
         std::vector<Divisao>& divisoes = divisoesDefault[disciplina.numCreditos];
         if (!divisoes.empty())
            disciplina.divisoes = divisoes;         
      }
   }
}

void Instance::preprocess()
{
   int numInstantes = getNumeroInstantes();
   int numDisciplinas = getNumeroDisciplinas();

   if (numInstantes == 0 || numDisciplinas == 0)
      return;

   this->numTurmas = disciplinas->back().refTurma + disciplinas->back().numTurmas;

   conflitosDisciplinas = new bool*[numDisciplinas];
   for (int d=0; d<numDisciplinas; d++)
      conflitosDisciplinas[d] = new bool[numDisciplinas];
   
   for (int d1=0; d1<numDisciplinas-1; d1++)
   {
      HDisciplina& disciplina1 = (*disciplinas)[d1];
      for (int d2=d1+1; d2<numDisciplinas; d2++)
      {
         HDisciplina& disciplina2 = (*disciplinas)[d2];

         bool conflicts = false;

         for (int dia=1; dia<=numDias; dia++)
         {
            for (int t1=disciplina1.refTurma; t1<disciplina1.refTurma+disciplina1.numTurmas; t1++)
            {
               for (int t2=disciplina2.refTurma; t2<disciplina2.refTurma+disciplina2.numTurmas; t2++)
               {
                  if (hasConflito(t1, t2) || hasConflito(dia, t1, t2))
                  {
                     conflicts = true;
                     break;
                  }
               }
               if (conflicts)
                  break;
            }
            if (conflicts)
               break;
         }

         conflitosDisciplinas[d1][d2] = conflicts;
         conflitosDisciplinas[d2][d1] = conflicts;
      }
   }

   instantes = new int*[numDisciplinas];
   sizes = new int[numDisciplinas];
   for (int d=0; d<numDisciplinas; d++)
   {
      std::vector<int> instantesDisciplina;
      for (int h=0; h<numInstantes; h++)
      {
         int dia = getDia(h);
         int horario = getHorario(h);
         if (isHorarioDisponivel(d, h) && isHorarioDisponivel(d, dia, horario))
            instantesDisciplina.push_back(h);
      }
      int size = (int)instantesDisciplina.size();
      sizes[d] = size;
      if (instantesDisciplina.empty())
         instantes[d] = NULL;
      else
      {
         instantes[d] = new int[size];
         for (int i=0; i<size; i++)
            instantes[d][i] = instantesDisciplina[i];
      }
   }
}

bool Instance::isTurnoDisponivel(int disciplina, int turno)
{
   int numDias = getNumeroDias() + 1;
   int numHorarios = getNumeroHorarios() + 1;

   for (int dia=1; dia<numDias; dia++)
   {
      for (int h=1; h<numHorarios; h++)
      {
         if (turno == getTurno(h) && isHorarioDisponivel(disciplina, getInstante(dia, h)))
            return true;
      }
   }
   return false;
}

bool Instance::isEmpty()
{
   return (getNumeroDisciplinas() == 0 || getNumeroInstantes() == 0 || getNumeroTurmas() == 0);
}
