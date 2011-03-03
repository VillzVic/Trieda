#include <vector>
#include <string>

#include "Aula.h"
#include "Horario.h"

#include "ProblemData.h"

typedef std::vector<std::vector<Aula*>* > MatrizSolucao;

class SolucaoOperacional
{
public:
   //SolucaoOperacional(void);
   SolucaoOperacional(ProblemData * problemData);

   virtual ~SolucaoOperacional(void);

   void carregaSolucaoInicial();

   void setMatrizAulas(MatrizSolucao *);

   MatrizSolucao* getMatrizAulas() const;

   std::string toString() const;

   int getIndiceMatriz(int, Horario *);

private:
   MatrizSolucao* matrizAulas;

   ProblemData * problemData;
};
