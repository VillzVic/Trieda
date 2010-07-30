#include "Solver.h"
#include "ProblemData.h"
#include <cstdio>

Solver::Solver(ProblemData *aProblemData)
{
   problemData = aProblemData;
}

Solver::~Solver()
{
   problemData = NULL;
}