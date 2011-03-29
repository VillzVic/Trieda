#ifndef _CUSTO_ALOCACAO_H_
#define _CUSTO_ALOCACAO_H_

#include <vector>

#include "Professor.h"
#include "Aula.h"

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

   double custo_total;

   // Pesos associados a cada medida
   double alfa;  // Associado a medida de id 0
   double beta;  // Associado a medida de id 1
   double teta;  // Associado a medida de id 2
   double gamma; // Associado a medida de id 3
};

#endif // _CUSTO_ALOCACAO_H_