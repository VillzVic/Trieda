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

   bool alocaAula(SolucaoOperacional & solucaoOperacional, Professor & professor, Aula & aula);

   ProblemData & problemData;

   // Estrutura que armazena o custo de alocar um professor a uma dada aula.
   map<pair<Professor*,Aula*>,CustoAlocacao*> custoProfTurma;

   /* PAREI AQUI !!!!

      PREENCHER A ESTRUTURA ABAIXO E, EM SEGUIDA, ORDENA-LA DE ACORDO
      COM O MAIOR CUSTO ASSOCIADO A CADA AULA.

      EM SEGUIDA, FAZER A ALOCACAO UTILIZANDO O ALGORITMO QUE PENSEI.

      QDO O ALGORITMO TERMINAR PODE SER QUE EXISTAM AULAS QUE NÃO FORAM ALOCADAS.
      HÁ, ENTÃO, A NECESSIDADE DE CRIAR UM PROFESSOR VIRTUAL.

      DEPOIS DE TUDO ISSO, AINDA TENHO QUE TESTAR SE EXISTE AULA QUE NAO TEM PROF ASSOCIADO,
      OU SEJA, NAO TEM CUSTO. SE ISSO ACONTECER, CRIA-SE UM PROFESSOR VIRTUAL E VAI ALOCANDO
      AULA PRA ELE ATÉ NÃO DER MAIS. SE PRECISAR, CRIA-SE OUTRO PROFESSOR E POR AI VAI.

      JA TEM A ESTRUTURA <aulasNaoRelacionadasProf> QUE EU CHECO ELA NO METODO <geraSolucaoInicial>
   */

   // Armazena, em ordem decrescente, os Custos de Alocação para cada Aula.
   //std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > > custosAlocacaoAulaOrdenado;
   std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> * > > custosAlocacaoAulaOrdenado;

   // Estrutura que armazena todas as aulas que não foram associadas a nenhum professor.
   // Baseando-se nessa estrutura, pode-se dizer se será necessário criar professores virtuais,
   // ou não.

   // Est. VAZIA -> a princípio, não precisa de prof. virtual.
   // Est. com um, ou mais elementos -> com crtz é necessário criar um, ou mais, professores virtuais.
   GGroup<Aula*> aulasNaoRelacionadasProf;

   void executaFuncaoPrioridade();

   // Funções auxilares à função de prioridade;
   // Calcula o custo dados um professor, uma aula e o id do custo em questão.
   void calculaCustoFixProf(Professor & prof ,Aula & aula, unsigned idCusto, int custo = 0, int maxHorariosCP = 0);
};

#endif // _SOLUCAO_INICIAL_OPERACIONAL_H_