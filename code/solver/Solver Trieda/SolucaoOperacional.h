#ifndef _SOLUCAO_OPERACIONAL_H_
#define _SOLUCAO_OPERACIONAL_H_

#include <map>
#include <vector>

#include "Aula.h"
#include "Horario.h"
#include "Professor.h"
#include "ProblemData.h"

typedef vector< vector<Aula*> * > MatrizSolucao;

class SolucaoOperacional
{
public:
   SolucaoOperacional(ProblemData *);
   virtual ~SolucaoOperacional(void);

   void carregaSolucaoInicial();

   // Dado um par 'dia da semana' e 'horário', retornar a coluna
   // correspondente a esse par na matriz (a linha é o professor atual)
   int getIndiceMatriz(int, Horario *);

   // Dado uma célula da matriz de solução (que
   // corresponde a um objeto 'Aula'), retornar
   // o horário dessa aula
   Horario* getHorario(int, int);

   // Dado um índice que corresponde a uma linha da
   // matriz de solução, retorna o professor correspondente
   Professor* getProfessorMatriz(int);

   ProblemData* getProblemData() const;

   void setMatrizAulas(MatrizSolucao *);
   MatrizSolucao* getMatrizAulas() const;

   void toString();

   std::map< int, Professor * > mapProfessores;

   // Retorna um iterator para o primeiro horario de um dado dia de um professor.
   // Dias considerados: 
   // 1: domingo
   // 2: segunda
   // 3: terça
   // 4: quarta
   // 5: quinta
   // 6: sexta
   // 7: sábado
   vector<Aula*>::iterator getHorariosDia(Professor & professor, int dia);

   int getHorariosProfDia(Professor & professor, int dia);

   // Retorna um vetor com todos os horários de um dado professor.
   // vector<Aula*> & getHorarios(Professor & professor);
   int getTotalHorarios() const;

   // bool alocaAula(Professor *, Aula *, int, Horario *);

   int getTotalDeProfessores() const;
   int getTotalDias() const;

private:
   int total_dias;
   int total_horarios;
   int total_professores;

   MatrizSolucao * matriz_aulas;
   ProblemData* problem_data;

   // Verifica se o professor tem disponível
   // um determinado horário em um dia da semana
   bool horarioDisponivelProfessor(Professor*, int, int);
};

#endif