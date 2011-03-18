#ifndef SOLUCAOINICIALOPERACIONAL_H
#define SOLUCAOINICIALOPERACIONAL_H

#include <vector>
#include <map>

using namespace std;

#include "Professor.h"
#include "ProblemData.h"
#include "Aula.h"

#include "SolucaoOperacional.h"

class CustoAlocacao
{
public:
   CustoAlocacao(Professor & professor, Aula & aula);
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

   // Metodos SET
   void setCustoFixProfTurma(double c);
   void setCustoPrefProfTurma(double c);
   void setCustoDispProf(double c);
   void setCustoDispProfTurma(double c);
   void setAlfa(double p);
   void setBeta(double p);
   void setTeta(double p);
   void setGamma(double p);

   // Operadores
   virtual bool operator < (CustoAlocacao const & right);
   virtual bool operator == (CustoAlocacao const & right);

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

   bool alocaAula(SolucaoOperacional & solucaoOperacional, 
      Professor & professor, 
      int dia, 
      Horario & horario, Aula & aula);

private:

   ProblemData & problemData;
   
   /*
   Estrutura que armazena o custo de alocar um professor a uma dada aula.
   */
   //map<pair<Professor*,Aula*>,CustoAlocacao*> custoProfTurma;

   vector<CustoAlocacao*> custosAlocacao;

//   void executaFuncaoPrioridade();
};

#endif /* SOLUCAOINICIALOPERACIONAL_H */