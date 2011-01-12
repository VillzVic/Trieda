#ifndef SolutionLoaderH
#define SolutionLoaderH

#include <map>

#include "GGroup.h"

class ProblemData;
class ProblemSolution;
class Variable;
class Constraint;

class FolgasSolution;

class SolutionLoader
{
public:
   SolutionLoader(ProblemData* aProblemData, ProblemSolution* aProblemSolution);
   ~SolutionLoader();

   void setFolgas(Variable* v);

private:
   ProblemData* problemData;
   ProblemSolution* problemSolution;

};
#endif