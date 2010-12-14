#pragma once

#include <iostream>
#include <set>

#include "IS_Campus.h"

#include "ProblemData.h"

using namespace std;

class InitialSolution
{
public:
   InitialSolution(ProblemData & problem_Data);

   // Copy Constructor
   InitialSolution(InitialSolution const & init_sol);

   virtual ~InitialSolution(void);

   ProblemData & get_Problem_Data() const;

   void generate_Initial_Solution();

private:

   ProblemData * problem_Data;

   GGroup<IS_Campus*> solucao;

   vector<Demanda*> vt_Demandas;

   //bool ordena_dec_demanda(Demanda const & left, Demanda const & right);

};
