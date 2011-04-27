#include "RandomDescentMethod.h"

RandomDescentMethod::RandomDescentMethod(Avaliador & _eval, NS & _ns, unsigned int _iterMax) :
evaluator(_eval), ns(_ns), iterMax(_iterMax)
{
}

RandomDescentMethod::~RandomDescentMethod()
{
}

void RandomDescentMethod::exec(SolucaoOperacional & s, double timelimit, double target_f)
{
   CPUTimer * timer = new CPUTimer();
   timer->start();

   double tnow = timer->getCPUTotalSecs();
   double tini = timer->getCPUTotalSecs();

   unsigned int iter = 0;

   while (iter < iterMax && ((tnow - tini) < timelimit))
   {
      Move & move = ns.move(s);

      double cost = 0;

      //if (move.canBeApplied(e, s))
      {
         //cost = evaluator.moveCost(e, move, s);
         Move & revMove = move.apply(s);
         double eRevMove = evaluator.avaliaSolucao(s);

         Move & iniMove = revMove.apply(s);
         double eIniMove = evaluator.avaliaSolucao(s);

         cost = eRevMove - eIniMove;

         delete &revMove;
         delete &iniMove;
      }
      //else
      //{
         //iter++;
         //delete &move;
         //tnow = time(NULL);
         //continue;
      //}

      iter++;

      if (cost < 0)
      {
         move.apply(s);
         iter = 0;
      }

      delete &move;
      tnow = timer->getCPUTotalSecs();
   }
}