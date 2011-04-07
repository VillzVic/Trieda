#ifndef _SOLUCAO_INICIAL_OPERACIONAL_H_
#define _SOLUCAO_INICIAL_OPERACIONAL_H_

#include <vector>
#include <map>
#include <algorithm>

#include "Professor.h"
#include "ProblemData.h"
#include "Aula.h"

#include "SolucaoOperacional.h"
#include "CustoAlocacao.h"

class SolucaoInicialOperacional
{
public:
   SolucaoInicialOperacional(ProblemData & problemData);
   virtual ~SolucaoInicialOperacional();

   SolucaoOperacional & geraSolucaoInicial();

private:

   bool alocaAula(vector<Aula*>::iterator itHorariosProf, int totalHorariosConsiderados, Professor & professor, Aula & aula);

   ProblemData & problemData;

   // Estrutura que armazena o custo de alocar um professor a uma dada aula.
   map<pair<Professor*,Aula*>,CustoAlocacao*> custoProfTurma;

   // Armazena, em ordem decrescente, os Custos de Alocação para cada Aula.
   //std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > > custosAlocacaoAulaOrdenado;

   // Armazena, em ordem decrescente, os Custos de Alocação.
   std::vector<CustoAlocacao*> custosAlocacaoAulaOrdenado;

   /*
   Estrutura que armazena todas as aulas que não foram associadas a nenhum professor.
   Baseando-se nessa estrutura, pode-se dizer se será necessário criar professores virtuais,
   ou não.

   Est. VAZIA -> a princípio, não precisa de prof. virtual.
   Est. com um, ou mais elementos -> com crtz é necessário criar um, ou mais, professores virtuais.
   */
   GGroup<Aula*> aulasNaoRelacionadasProf;

   void executaFuncaoPrioridade();

   // Funções auxilares à função de prioridade;
   // Calcula o custo dados um professor, uma aula e o id do custo em questão.
   void calculaCustoFixProf(Professor & prof ,Aula & aula, unsigned idCusto, int custo = 0, int maxHorariosCP = 0);
};

#endif // _SOLUCAO_INICIAL_OPERACIONAL_H_
