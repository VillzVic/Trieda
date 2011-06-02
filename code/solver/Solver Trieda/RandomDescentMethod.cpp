#include "RandomDescentMethod.h"

RandomDescentMethod::RandomDescentMethod( Avaliador & _eval, NS & _ns, unsigned int _iterMax )
   : evaluator( _eval ), ns( _ns ), iterMax( _iterMax )
{
}

RandomDescentMethod::~RandomDescentMethod()
{

}

void RandomDescentMethod::exec( SolucaoOperacional & s, double timelimit, double target_f )
{
   std::cout << "RDM exec(" << target_f << "," << timelimit << ")" << std::endl;
   ns.print();
   std::cout << std::endl;

   CPUTimer * timer = new CPUTimer();
   timer->start();

   timer->stop();
   double elapsedTime = timer->getCPUTotalSecs();
   timer->start();

   unsigned int iter = 0;

   // Teste
   // unsigned int iterBKP = 0;
   // std::cout << "RDM starts:\tSolution Evaluation: " << evaluator.avaliaSolucao(s) << std::endl;

   while ( iter < iterMax && elapsedTime < timelimit )
   {
      // 1std::cout << "\n\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
      // 1std::cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
      // 1std::cout << "\n\nstart iter " << iter << "\n\n\n";

      Move & move = ns.move( s );

      double cost = 0;

      //if (move.canBeApplied(e, s))
      //{
      //cost = evaluator.moveCost(e, move, s);

      // Teste
      //double oldEvalSolutionBKP = evaluator.avaliaSolucao(s);

      //ITERA_GGROUP(itAula,s.getProblemData()->aulas,Aula)
      //{
      //   if((itAula->getDisciplina()->getId() == 13) && (itAula->getDiaSemana() == 5))
      //   {
      //      std::cout << "Aula antes de aplicar o movimento:\n";
      //      itAula->toString();
      //   }
      //}

      //1s.toString2();
      //1s.validaSolucao("Validando a solucao antes do movimento ser aplicado.");

      //1std::cout << "\n\nMovimento aplicado.\n\n";
      Move & revMove = move.apply( s );

      //1s.toString2();
      //1s.validaSolucao("Validando a solucao apos do movimento ser aplicado.");

      //ITERA_GGROUP(itAula,s.getProblemData()->aulas,Aula)
      //{
      //   if((itAula->getDisciplina()->getId() == 13) && (itAula->getDiaSemana() == 5))
      //   {
      //      std::cout << "Possivel erro no movimento anterior.\nAula depois de aplicar o movimento:\n";
      //      itAula->toString();

      //      //s.toString2();

      //      //s.validaSolucao("Validando a solucao apos do movimento ser aplicado.");

      //      //getchar();
      //   }
      //}

      //1std::cout << "\n\nAvaliando movimento:\n";
      double newEvalSolution = evaluator.avaliaSolucao( s );

      // 1std::cout << "\n\nAplicando movimento reverso e avaliando:\n";
      Move & iniMove = revMove.apply( s );

      // 1s.toString2();
      // 1s.validaSolucao("Validando a solucao apos o movimento reverso ser aplicado.");

      double oldEvalSolution = evaluator.avaliaSolucao( s );

      if ( move != iniMove )
      {
         std::cout << "\n\n\nmove e inimove diferentes !!!\n\n\n";
         move.print();
         std::cout << "\n\n";
         iniMove.print();
         exit(1);
      }

      //if(oldEvalSolutionBKP != oldEvalSolution)
      //{
      //   /* Se forem iguais, nada pode-se afirmar sobre a corretude. Porém, se forem
      //   diferentes, pode-se afirmar que está errado. */
      //   std::cout << "ERRO EM RDM" << std::endl;
      //   exit(1);
      //}

      cost = ( newEvalSolution - oldEvalSolution );

      //delete &revMove;
      //delete &iniMove;
      //}
      //else
      //{
      //iter++;
      //delete &move;
      //tnow = time(NULL);
      //continue;
      //}

      iter++;
      // iterBKP++;

      if ( cost < 0 )
      {
         // std::cout << "RDM : Best fo: " << newEvalSolution << " on [iter " << iter << "]" << std::endl;
         std::cout << "RDM : Best fo: " << newEvalSolution << " on [iter "
                   << iter << "]\t Old fo: " << oldEvalSolution << std::endl;

         // 1std::cout << "\n\nAplicando movimento e avaliando, pois MELHOROU:\n";

         // delete &(move.apply(s));
         delete &( iniMove.apply( s ) );

         // 1s.validaSolucao();

         iter = 0;
      }

      // 1std::cout << "\nDeleted:";
      delete &revMove;
      // 1std::cout << " revMove,";

      delete &iniMove;
      // 1std::cout << " iniMove,";

      delete &move;
      // 1std::cout << " move.\n";

      timer->stop();
      elapsedTime = timer->getCPUTotalSecs();
      timer->start();

      // 1std::cout << "\n\nend iter " << iter << "\n\n";
   }

   // std::cout << "RDM ends:\t Solution Evaluation: " << evaluator.avaliaSolucao(s) << std::endl;
   // std::cout << "Elapsed time in RDM exec:\t" << elapsedTime << " secs.\n";
}