#include "SolutionPool.h"

SolutionPool::SolutionPool(int maxPool, int aSolSize)
{
   solSize = aSolSize;
   maxPoolSize = maxPool;
   pool.clear();
   poolVal.clear();
}

SolutionPool::~SolutionPool()
{
   for (int i=0; i < (int)pool.size(); i++)
   {
      if ( pool[i] != NULL )
      {
         delete[] pool[i];
         pool[i] = NULL;
      }
   }

   pool.clear();
   poolVal.clear();
}

void SolutionPool::addSolution(double *xSol, double val)
{
   if ( (int)(pool.size()) >= maxPoolSize )
   {
      int idxW = -1;
      double wObj = -10000000000000.0;
      for (int i=0; i < (int)pool.size(); i++)
      {
         if ( poolVal[i] >= wObj && rand() % 3 == 0 )
         {
            idxW = i;
            wObj = poolVal[i];
         }
      }

      if ( val < wObj )
      {
         poolVal[idxW] = val;
         for (int i=0; i < solSize; i++)
         {
            pool[idxW][i] = xSol[i];
         }
      }
   }
   else
   {
      int pos = (int)(pool.size());
      pool.push_back(NULL);
      poolVal.push_back(val);
      pool[pos] = new double[solSize];
      for (int i=0; i < solSize; i++)
      {
         pool[pos][i] = xSol[i];
      }
   }
}