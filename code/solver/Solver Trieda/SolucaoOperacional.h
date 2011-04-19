#ifndef _SOLUCAO_OPERACIONAL_H_
#define _SOLUCAO_OPERACIONAL_H_

#include <map>
#include <vector>

#include "Aula.h"
#include "Horario.h"
#include "Professor.h"
#include "ProblemData.h"

#include "input.h"

typedef vector< vector< Aula * > * > MatrizSolucao;

class SolucaoOperacional
{
public:
   SolucaoOperacional(ProblemData *);
   virtual ~SolucaoOperacional(void);

   void carregaSolucaoInicial();

   // Dado um par 'dia da semana' e 'horário',
   // retornar a coluna correspondente a esse
   // par na matriz (a linha é o professor atual)
   int getIndiceMatriz(int, Horario *);

   // Dado um índice que corresponde a uma linha
   // de um dado professor na matriz de solução,
   // o dia da semana e o horário de aula desejado,
   // retorna o horário correspondente à aula
   // alocada (ou NULL, caso não haja aula alocada)
   Horario * getHorario(int, int, int);

   // Dado um índice que corresponde a uma linha da
   // matriz de solução, retorna o professor correspondente
   Professor * getProfessorMatriz(int);

   ProblemData * getProblemData() const;

   void setMatrizAulas(MatrizSolucao *);
   MatrizSolucao * getMatrizAulas() const;

   void toString();
   void toString2();

   // Dado o ID do professor, retorna um
   // ponteiro para o objeto 'Professor'
   std::map< int, Professor * > mapProfessores;

   // Retorna um iterator para o horario
   // desejado de um dado dia de um professor.
   // Dias considerados: 
   // 1: domingo
   // 2: segunda
   // 3: terça
   // 4: quarta
   // 5: quinta
   // 6: sexta
   // 7: sábado
   //int getItHorariosProf(Professor & professor, int dia = 2, int horario = 0);
   std::vector< Aula * >::iterator getItHorariosProf(Professor &, int = 2, int = 0);

   // Adiciona um novo professor à solução. O inteiro retornado
   // é referente à posição (linha) que o novo  professor ocupa na solução.
   int addProfessor(Professor &, vector< Aula * > &);

   // FUTURAMENTE, VAI SER NECESSARIO CRIAR UM METODO PARA REMOVER UM PROFESSOR.

   // TODO: Implementar o método abaixo.
   // bool alocaAula(Professor & professor, int dia, Horario & horario, Aula & aula);
   // bool alocaAula(Professor & professor, Aula & aula, vector<Aula*> & horarios);

   int getTotalDias() const;
   int getTotalHorarios() const;
   int getTotalDeProfessores() const;

   // Armazena os índices para cada dia de cada horário.
   std::map< std::pair< Horario *, int /*dia*/>, int /*col - idOperacional*/ > refHorarios;

   // Verifica se as aula <aX> e <aY> podem
   // ter trocados os seus respectivos horários.
   bool podeTrocarHorariosAulas(Aula & aX, Aula & aY) const;

   // Função auxiliar à função podeTrocarHorariosAulas
   bool checkConflitoBlocoCurricular(Aula &, std::vector< std::pair< Professor *, Horario * > > &) const;

private:
   int total_dias;
   int total_horarios;
   int total_professores;

   MatrizSolucao * matriz_aulas;
   ProblemData* problem_data;

   // Verifica se o professor tem disponível
   // um determinado horário em um dia da semana
   bool horarioDisponivelProfessor(Professor *, int, int);
};

#endif