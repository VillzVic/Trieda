#ifndef _SOLUCAO_OPERACIONAL_H_
#define _SOLUCAO_OPERACIONAL_H_

#include <map>
#include <vector>
#include <string>

#include "Aula.h"
#include "Horario.h"
#include "ProblemData.h"

typedef std::vector< std::vector< Aula* > * > MatrizSolucao;

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

   std::string toString() const;

   /*Já existem esses maps no problemData. Chama refCampus ... 
   onde vc passa o id do campus e ele retorna o ponteiro para o objeto instanciado.

   falar isso com o Cleiton.
   */
   std::map<int, Professor*> mapProfessores;
   std::map<int, Unidade*> * mapUnidades;
   std::map<int, Campus*> * mapCampus;

   /*
   TODO: Implementar o método abaixo.
   */
   bool alocaAula(Professor & professor, int dia, Horario & horario, Aula & aula);

private:
   MatrizSolucao* matrizAulas;
   ProblemData* problemData;
};

#endif