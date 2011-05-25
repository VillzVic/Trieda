#ifndef _CUSTO_ALOCACAO_H_
#define _CUSTO_ALOCACAO_H_

#include <vector>

#include "Professor.h"
#include "Aula.h"
#include "ProblemData.h"

class CustoAlocacao
{
public:
   CustoAlocacao( ProblemData & problemData, Professor &, Aula & );
   CustoAlocacao( CustoAlocacao const & );
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
   unsigned getNivelPrioridade() const;

   // Metodos ADD
   void addCustoFixProfTurma(double);
   void addCustoPrefProfTurma(double);
   void addCustoDispProf(double);
   void addCustoDispProfTurma(double);

   // Metodos SET
   void setAlfa(double);
   void setBeta(double);
   void setTeta(double);
   void setGamma(double);

   // Operadores
   virtual bool operator < ( CustoAlocacao const & );
   virtual bool operator == ( CustoAlocacao const & );
   virtual bool operator > ( CustoAlocacao const & );
   //virtual bool operator >= ( CustoAlocacao const & );
   virtual CustoAlocacao & operator = ( CustoAlocacao const & );

protected:

   /*
   Total de blocos curriculares que a aula pertence.

   OBS.: Esse valor é necessário para o caso em que uma aula pertença a mais de um bloco 
   curricular (de cursos compatíveis) simultaneamente. A ideia é SEMPRE alocar essa(s) aula(s) primeiro.
   Assim, garante-se (EMPIRICAMENTE e pelo solver TÁTICO) que sempre existirá solução. Se, para um dado 
   dia, a alocação das aulas for realizada sem considerar esse custo específico pode-se alocar as aulas
   de modo que resulte um conflito de blocos curriculares.

   Para um exemplo, ver planilha "OUTPUT-OPERACIONAL-CONFLITO-BLOCOCURRIC-DISC6-DIA5".

   Nessa planilha, analisa-se a alocação das aulas para o dia 5.

   Na primeira parte, temos a saída do solver, onde NÃO é possível alocar a aula 6.
   Já na segunda parte (solução manual que considera prioritárias as aulas que pertencem a mais 
   blocos curriculares) temos uma saída em que todas as aulas são alocadas.
   */

   unsigned nivelPrioridade;

private:

   Professor & professor;
   Aula & aula;

   /* Armazena, separadamente, os custos de:
   0 - fixação do professor p a turma i.  {custoFixProfTurma}
   1 - preferência do professor p para lecionar na turma i. {custoPrefProfTurma}
   2 - disponibilidade do professor p. {custoDispProf}
   3 - disponibilidade do professor p para lecionar na turma i.   {custoDispProfTurma}
   */
   std::vector< double > custosAlocacao;

   double custo_total;

   // Pesos associados a cada medida
   double alfa;  // Associado a medida de id 0
   double beta;  // Associado a medida de id 1
   double teta;  // Associado a medida de id 2
   double gamma; // Associado a medida de id 3
};

#endif // _CUSTO_ALOCACAO_H_