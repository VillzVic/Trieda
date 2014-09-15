#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <hash_map>
#include <hash_set>
#include <DateTime.h>

class Disciplina;

typedef struct {
   std::vector<int> dia;
   std::vector<int> creditos;
} Divisao;

typedef struct {
   int refTurma;
   int numTurmas;
   int numCreditos;
   int disciplinaAssoc;
   std::vector<Divisao> divisoes;

   Disciplina* disciplina;
   std::vector<int> turmas;
   std::vector<int> numAlunos;
   std::hash_map<int, std::hash_set<int>> horarios;
   std::hash_map<int, std::hash_map<int, std::hash_map<int, std::hash_set<int>>>> hihfvalidos;
} HDisciplina;

class Instance
{
public:
   Instance(void);
   ~Instance(void);

   void destroy();

   inline int getNumeroDias() { return numDias; }
   inline int getNumeroHorarios() { return numHorarios; }
   inline int getNumeroDisciplinas() { return (disciplinas != NULL ? disciplinas->size() : 0); }
   inline int getNumeroInstantes() { return (numDias * numHorarios); }
   inline int getDia(int instante) { return instante / numHorarios + 1; }
   inline int getHorario(int instante) { return instante % numHorarios + 1; }
   inline int getInstante(int dia, int horario) { return (dia - 1) * numHorarios + horario - 1; }
   inline std::vector<HDisciplina>* getDisciplinas() { return disciplinas; }
   int getTurma(int disciplina, int turmaId) { return ((*disciplinas)[disciplina].refTurma + turmaId); }
   inline int getTurno(int horario) { return (horario < 7 ? 1 : (horario < 13 ? 2 : 3)); }
   inline int getNumeroTurmas() { return numTurmas; }

   inline void setNumeroDias(int n) { numDias = n; }
   inline void setNumeroHorarios(int n) { numHorarios = n; }
   void addConflito(int turma1, int turma2);
   void addConflito(int dia, int turma1, int turma2);

   inline bool hasConflito(int turma1, int turma2) { return matrizConflitos2[turma1][turma2]; }
   inline bool hasConflito(int dia, int turma1, int turma2) { return matrizConflitosDia2[dia][turma1][turma2]; }
   inline bool hasConflitoDisciplina(int disciplina1, int disciplina2) { return conflitosDisciplinas[disciplina1][disciplina2]; }
   std::hash_set<int>* getConflitos(int turma);
   std::hash_set<int>* getConflitos(int dia, int turma);
   bool isHorarioDisponivel(int disciplina, int dia, int horario);
   bool isHorarioDisponivel(int disciplina, int dia, int turma, int hi, int hf);
   bool isHorarioDisponivel(int disciplina, int instante);
   bool isTurnoDisponivel(int disciplina, int turno);

   inline int* getInstantes(int disciplina) { return instantes[disciplina]; }
   inline int getSize(int disciplina) { return sizes[disciplina]; }

   bool isEmpty();

   void reset();
   void buildRefTurma();

   std::pair<int, int> getDemandaTotal();
   int getTotalAlunos();

   std::string toString();

   void writeInstance(std::string filename);
   void readInstance(std::string filename);

   void buildDivisoesDefault();

   void buildDivisoes();

   void preprocess();

private:
   std::hash_map<int, std::vector<Divisao>> divisoesDefault;

   int numDias;
   int numHorarios;
   int numTurmas;
   std::vector<HDisciplina>* disciplinas;
   std::hash_map<int, std::hash_set<int>*>* matrizConflitos;
   std::hash_map<int, std::hash_map<int, std::hash_set<int>*>*>* matrizConflitosDia;

   bool** matrizConflitos2;
   bool*** matrizConflitosDia2;
   bool** conflitosDisciplinas;

   int** instantes;
   int* sizes;
};
