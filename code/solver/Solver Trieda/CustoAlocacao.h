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

   OBS.: Esse valor � necess�rio para o caso em que uma aula perten�a a mais de um bloco 
   curricular (de cursos compat�veis) simultaneamente. A ideia � SEMPRE alocar essa(s) aula(s) primeiro.
   Assim, garante-se (EMPIRICAMENTE e pelo solver T�TICO) que sempre existir� solu��o. Se, para um dado 
   dia, a aloca��o das aulas for realizada sem considerar esse custo espec�fico pode-se alocar as aulas
   de modo que resulte um conflito de blocos curriculares.

   Para um exemplo, ver planilha "OUTPUT-OPERACIONAL-CONFLITO-BLOCOCURRIC-DISC6-DIA5".

   Nessa planilha, analisa-se a aloca��o das aulas para o dia 5.

   Na primeira parte, temos a sa�da do solver, onde N�O � poss�vel alocar a aula 6.
   J� na segunda parte (solu��o manual que considera priorit�rias as aulas que pertencem a mais 
   blocos curriculares) temos uma sa�da em que todas as aulas s�o alocadas.
   */

   unsigned nivelPrioridade;

private:

   Professor & professor;
   Aula & aula;

   /* Armazena, separadamente, os custos de:
   0 - fixa��o do professor p a turma i.  {custoFixProfTurma}
   1 - prefer�ncia do professor p para lecionar na turma i. {custoPrefProfTurma}
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