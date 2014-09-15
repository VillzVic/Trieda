#ifndef SOLUTIONPOOL_H_
#define SOLUTIONPOOL_H_

#include<vector>

class SolutionPool
{
public:
   SolutionPool(int maxPool, int aSolSize);
   ~SolutionPool();

   void addSolution(double *xSol, double val);
   int getPoolSize() { return (int)(pool.size()); }
   int getMaxPoolSize() { return maxPoolSize; }
   int getSolSize() { return solSize; }
   double *getSolution(int i) { return pool[i]; }

   double getSolVal(int i) { return poolVal[i]; }
   double getSolIdx(int i, int j) { return pool[i][j]; }


private:

   int solSize;
   int maxPoolSize;
   std::vector<double*> pool;
   std::vector<double> poolVal;
};

#endif