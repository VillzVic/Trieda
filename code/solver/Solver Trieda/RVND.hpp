#ifndef RVND_HPP_
#define RVND_HPP_

#include <algorithm>

#include "Heuristic.hpp"

#include "Avaliador.h"

#include "CPUTimerWin.h"

class RVND: public Heuristic
{
public:
   using Heuristic::exec; // prevents name hiding

   RVND(Avaliador & _ev, vector<Heuristic*> _neighbors) :
   ev(_ev), neighbors(_neighbors)
   {
   }

   virtual void exec(SolucaoOperacional & s, double timelimit, double target_f)
   {
      CPUTimer * timer = new CPUTimer();
      timer->start();

      timer->stop();
      double elapsedTime = timer->getCPUTotalSecs(); 
      timer->start();

      std::random_shuffle(neighbors.begin(), neighbors.end()); // shuffle elements
      int r = neighbors.size();

      int k = 1;

      timer->stop();
      elapsedTime = timer->getCPUTotalSecs(); 
      timer->start();

      double e = ev.avaliaSolucao(s);

      while ((target_f < e) && (k <= r) && (elapsedTime < timelimit))
      {
         SolucaoOperacional * s0 = new SolucaoOperacional(s);
         //Evaluation<M>* e0 = &e.clone();

         neighbors[k - 1]->exec(*s0, (timelimit-elapsedTime), target_f);

         double e0 = ev.avaliaSolucao(*s0);
         
         if (e0 < e)
         {
            //s = *s0;
            delete &s;
            s = *new SolucaoOperacional(*s0);
            //e = *e0;
            e = e0;
            delete s0;
            //delete e0;
            k = 1;
         }
         else
         {
            delete s0;
            //delete e0;
            k = k + 1;
         }

         //e = ev.avaliaSolucao(s);

         timer->stop();
         elapsedTime = timer->getCPUTotalSecs(); 
         timer->start();
      }
   }

private:
   vector<Heuristic*> neighbors;
   Avaliador & ev;
};

#endif /*RVND_HPP_*/