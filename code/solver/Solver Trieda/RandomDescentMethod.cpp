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
   std::cout << "RDM exec(" << target_f << "," << timelimit << ")" << std::endl;

   CPUTimer * timer = new CPUTimer();
   timer->start();

   double tnow = timer->getCPUTotalSecs();
   double tini = timer->getCPUTotalSecs();

   unsigned int iter = 0;

   std::cout << "RDM starts:\tSolution Evaluation: " << evaluator.avaliaSolucao(s) << std::endl;

   while (iter < iterMax && ((tnow - tini) < timelimit))
   {
      Move & move = ns.move(s);

      double cost = 0;

      //if (move.canBeApplied(e, s))
      //{
         //cost = evaluator.moveCost(e, move, s);
         Move & revMove = move.apply(s);
         double newEvalSolution = evaluator.avaliaSolucao(s);

         Move & iniMove = revMove.apply(s);
         double oldEvalSolution = evaluator.avaliaSolucao(s);

         cost = newEvalSolution - oldEvalSolution;

         delete &revMove;
         delete &iniMove;
      //}
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
         std::cout << "RDM : Best fo: " << newEvalSolution << " on [iter " << iter << "]" << std::endl;

         move.apply(s);
         iter = 0;
      }

      delete &move;
      tnow = timer->getCPUTotalSecs();
   }

   std::cout << "RDM ends:\t Solution Evaluation: " << evaluator.avaliaSolucao(s) << std::endl;
}