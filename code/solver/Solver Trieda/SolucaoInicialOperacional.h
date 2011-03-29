#ifndef SOLUCAOINICIALOPERACIONAL_H
#define SOLUCAOINICIALOPERACIONAL_H

#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#include "Professor.h"
#include "ProblemData.h"
#include "Aula.h"

#include "SolucaoOperacional.h"

class CustoAlocacao
{
public:
   CustoAlocacao(Professor & professor, Aula & aula);
   CustoAlocacao(CustoAlocacao const & custoAlocacao);
   virtual ~CustoAlocacao();

   // Metodos GET
   Professor & getProfessor() const;
   Aula & CustoAlocacao::getAula() const;
   double getCustoFixProfTurma() const;
   double getCustoPrefProfTurma() const;
   double getCustoDispProf() const;
   double getCustoDispProfTurma() const;
   double getCustoTotal() const;
   double getAlfa() const;
   double getBeta() const;
   double getTeta() const;
   double getGamma() const;

   // Metodos ADD
   void addCustoFixProfTurma(double c);
   void addCustoPrefProfTurma(double c);
   void addCustoDispProf(double c);
   void addCustoDispProfTurma(double c);

   // Metodos SET
   void setAlfa(double p);
   void setBeta(double p);
   void setTeta(double p);
   void setGamma(double p);

   // Operadores
   virtual bool operator < (CustoAlocacao const & right);
   virtual bool operator == (CustoAlocacao const & right);
   virtual bool operator > (CustoAlocacao const & right);
   virtual CustoAlocacao & operator = (CustoAlocacao const & right);

private:

   Professor & professor;
   Aula & aula;

   /* Armazena, separadamente, os custos de alocar um professor a uma turma.
   0 - fixação do professor p a turma i.  {custoFixProfTurma}
   1 - preferência do professor p para lecionar na turma i. {custoPrefProfTurma}
   2 - disponibilidade do professor p. {custoDispProf}
   3 - disponibilidade do professor p para lecionar na turma i.   {custoDispProfTurma}
   */
   vector<double> custosAlocacao;

   double custoTotal;

   /* Pesos associados a cada medida */
   double alfa; // Associado a medida de id 0
   double beta; // Associado a medida de id 1
   double teta; // Associado a medida de id 2
   double gamma; // Associado a medida de id 3
};

// ============================================================================
// ============================================================================

class SolucaoInicialOperacional
{
public:
   SolucaoInicialOperacional(ProblemData & problemData);
   virtual ~SolucaoInicialOperacional();

   SolucaoOperacional & geraSolucaoInicial();

   bool alocaAula(SolucaoOperacional & solucaoOperacional, Professor & professor, Aula & aula);

   //bool alocaAula(SolucaoOperacional & solucaoOperacional, 
   //   Professor & professor, 
   //   int dia, 
   //   Horario & horario, Aula & aula);

private:

   ProblemData & problemData;
   
   /*
   Estrutura que armazena o custo de alocar um professor a uma dada aula.
   */
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

   /* Armazena, em ordem decrescente, os Custos de Alocação para cada Aula. */
   std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > > custosAlocacaoAulaOrdenado;

   /* Estrutura que armazena todas as aulas que não foram associadas a nenhum professor.
   
   Baseando-se nessa estrutura, pode-se dizer se será necessário criar professores virtuais,
   ou não.

   Est. VAZIA -> a princípio, não precisa de prof. virtual.
   Est. com um, ou mais elementos -> com crtz é necessário criar um, ou mais, professores virtuais.
   */
   GGroup<Aula*> aulasNaoRelacionadasProf;

   void executaFuncaoPrioridade();

   // Função de Prioridade.
   //void preencheEstruturaCustoProfTurmaOrdenado();

   // Funções auxilares à função de prioridade;
   /*Calcula o custo dados um professor, uma aula e o id do custo em questão. */
   void calculaCustoFixProf(Professor & prof ,Aula & aula, unsigned idCusto, int custo = 0, int maxHorariosCP = 0);
   
};

#endif /* SOLUCAOINICIALOPERACIONAL_H */