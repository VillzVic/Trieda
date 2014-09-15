#pragma once

class Solution;
class Instance;

class SolutionBuilder
{
public:
   static Solution* createRandomSolution(Instance* instance, double alpha);
   static bool insert(Solution* solution, int d, int turma);
};

