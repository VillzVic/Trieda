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

   int getIndiceMatriz(int, Horario *);
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