#pragma once

#include <vector>
#include <set>

#include "opt_lp.h"

#include "STVariable.h"
#include "STConstraint.h"

class Solution;

class SolverTatico
{
public:
   SolverTatico();
   ~SolverTatico(void);

   void solve(Solution* solution);

private:
   OPT_LP *lp;

   STVariableHash vHash;
   STConstraintHash cHash;

   std::set<std::pair<int, int>> turmas;

   Solution* solution;

   bool createLP(int dia);

   int createVarAssign_X(int dia);
   int createVarSlack_FH(int dia);
   int createVarSlack_FT();

   int createConstAssign();
   int createConstConflicts(int dia);
   int createConstAtend();
};

