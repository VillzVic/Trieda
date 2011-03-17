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

   // Dado um par 'dia da semana' e 'hor�rio', retornar a coluna
	// correspondente a esse par na matriz (a linha � o professor atual)
   int getIndiceMatriz(int, Horario *);

   // Dado uma c�lula da matriz de solu��o (que
   // corresponde a um objeto 'Aula'), retornar
   // o hor�rio dessa aula
   Horario* getHorario(int, int);

   // Dado um �ndice que corresponde a uma linha da
   // matriz de solu��o, retorna o professor correspondente
   Professor* getProfessorMatriz(int);

   ProblemData* getProblemData() const;

   void setMatrizAulas(MatrizSolucao *);
   MatrizSolucao* getMatrizAulas() const;

   std::string toString() const;

   std::map<int, Professor*> mapProfessores;
   std::map<int, Unidade*> * mapUnidades;
   std::map<int, Campus*> * mapCampus;

private:
   MatrizSolucao* matrizAulas;
   ProblemData* problemData;
};

#endif