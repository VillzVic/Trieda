#pragma once

#include <iostream>
#include <set>
#include <vector>

#include "IS_Campus.h"

#include "ProblemData.h"

#include "Variable.h"

#include "opt_lp.h"

using namespace std;

//#define FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE

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

   void geraSolIniSemDividirDem();

   //void geraSolIniDividindoDem();

   void generate_Initial_Solution();

   //pair<int*,double*> repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols);
   //void repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols);
   pair<vector<int>*,vector<double>*> repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols, OPT_LP & lp);

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
