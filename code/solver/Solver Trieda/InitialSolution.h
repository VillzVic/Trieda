#pragma once

#include <iostream>
#include <set>

#include "IS_Campus.h"

#include "ProblemData.h"

#include "Variable.h"

using namespace std;

class InitialSolution
{
public:
   InitialSolution(ProblemData & problem_Data);

   // Copy Constructor
   InitialSolution(InitialSolution const & init_sol);

   virtual ~InitialSolution(void);

   ProblemData & get_Problem_Data() const;

   GGroup<IS_Campus*> const & getInitialSolution() const;

   int getNumDemandasAtendidas() const;

   int getNumDemandas_NAO_Atendidas() const;

   void generate_Initial_Solution();

   pair<int*,double*> repSolIniParaVariaveis(VariableHash & v_Hash, int lp_Cols);

private:

   ProblemData * problem_Data;

   GGroup<IS_Campus*> solucao;

   // ---

   /* Armazena um ponteiro para a demanda em questão e uma cópia da variável
   <quantidade> encontrada na demanda.
   
   Assim, para saber quanto de uma demanda foi atendida, basta subtrair de
   <quantidade> da demanda o valor do segundo elemento do par:
   
   pair<Demanda*,int>.FIRST->QUANTIDADE - pair<Demanda*,int>.SECOND

      Caso o resultado seja igual a 0, significa que toda a demanda foi atendida.

      Caso o resultado seja maior do que 0, significa que a demanda foi atendida parcialmente. 
      Nesse caso, essa diferença encontrada informa a parte da demanda que não foi atendida.

   */
   vector<pair<Demanda*,int> > vt_Demandas;

   // ---

   int num_Demandas_Atendidas;
   
   int num_Demandas_NAO_Atendidas;

   // METODOS
   
   bool todasDemandasAtendidas() const;

   bool tentouAtenderTodasDemandas() const;

};
