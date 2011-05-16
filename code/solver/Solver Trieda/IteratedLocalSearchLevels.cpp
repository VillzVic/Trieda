#include "IteratedLocalSearchLevels.h"

IteratedLocalSearchLevels::IteratedLocalSearchLevels(Avaliador & _e, Heuristic & _h, ILSLPerturbation & _p, int _iterMax, int _levelMax) : e(_e), h(_h), p(_p), iterMax(_iterMax), levelMax(_levelMax)
{
}

IteratedLocalSearchLevels::~IteratedLocalSearchLevels()
{
}

levelHistory & IteratedLocalSearchLevels::initializeHistory()
{
   pair<int, int> vars(0, 0);

   // IterMax e LevelMax
   pair<int, int> maxs(iterMax, levelMax);

   return *new levelHistory(vars, maxs);
}

void IteratedLocalSearchLevels::localSearch(SolucaoOperacional & s, double timelimit, double target_f)
{
   h.exec(s, timelimit, target_f);
}

void IteratedLocalSearchLevels::perturbation(SolucaoOperacional & s, double timelimit, double target_f, levelHistory& history)
{
   int iter = history.first.first;
   int level = history.first.second;
   int iterMax = history.second.first;

   p.perturb(s, timelimit, target_f, level);

   // Incrementa a iteracao
   iter++;

   if (iter >= iterMax)
   {
      iter = 0;
      level++;
      cout << ">> Level: " << level << ".." << endl;
   }

   // Atualiza o historico
   history.first.first = iter;
   history.first.second = level;
}


SolucaoOperacional & IteratedLocalSearchLevels::acceptanceCriterion(SolucaoOperacional & s1, SolucaoOperacional & s2, levelHistory & history)
{
   s1.validaSolucao("Validando s1 ANTES de avaliar em acceptanceCriterion.");
   s2.validaSolucao("Validando s2 ANTES de avaliar em acceptanceCriterion.");

   double eS1 = e.avaliaSolucao(s1);
   double eS2 = e.avaliaSolucao(s2);

   s1.validaSolucao("Validando s1 DEPOIS de avaliar em acceptanceCriterion.");
   s2.validaSolucao("Validando s2 DEPOIS de avaliar em acceptanceCriterion.");

   if (eS2 < eS1)
   {
      // =======================
      //   Melhor solucao: 's2'
      // =======================
      cout << "Best fo: " << eS2;
      cout << " on [iter " << history.first.first << " of level " << history.first.second << "]" << endl;
      //   delete &e;

      // =======================
      //  Atualiza o historico
      // =======================
      // iter = 0
      history.first.first = 0;
      // level = 0
      history.first.second = 0;

      // =======================
      //    Retorna s2
      // =======================
      return *new SolucaoOperacional(s2);
   }
   else
      return *new SolucaoOperacional(s1);
}

bool IteratedLocalSearchLevels::terminationCondition(levelHistory& history)
{
   int level = history.first.second;
   int levelMax = history.second.second;

   return (level >= levelMax);
}


void IteratedLocalSearchLevels::exec(SolucaoOperacional & s, double timelimit, double target_f)
{
   cout << "ILS exec(" << target_f << "," << timelimit << ")" << endl;
   //getchar();

   CPUTimer * timer = new CPUTimer();
   timer->start();

   levelHistory * history = &initializeHistory();

   // 's0' <- GenerateSolution
   // 's*' <- localSearch 's'

   timer->stop();
   double elapsedTime = timer->getCPUTotalSecs();
   timer->start();

   localSearch(s, (timelimit - elapsedTime), target_f);

   //s.validaSolucao();

   //std::cout << "GETCHAR PROGRAMADO - SOL INI!!!" << std::endl;
   //getchar();

   SolucaoOperacional * sStar = new SolucaoOperacional(s);
   double eStar = e.avaliaSolucao(*sStar);

   sStar->validaSolucao("Validando sStar");
   //getchar();

   cout << "ILS starts:\tSolution Evaluation: " << eStar << std::endl;

   do
   {
      SolucaoOperacional * s1 = new SolucaoOperacional(*sStar);
      double e1 = eStar;

      timer->stop();
      elapsedTime = timer->getCPUTotalSecs();
      timer->start();

      s1->validaSolucao("Validando a solucao antes de realizar a perturbacao.");
      perturbation(*s1, (timelimit - elapsedTime), target_f, *history);

      timer->stop();
      elapsedTime = timer->getCPUTotalSecs();
      timer->start();

      s1->validaSolucao("Validando a solucao depois de realizar a perturbacao e antes de fazer a busca local.");
      localSearch(*s1, (timelimit - elapsedTime), target_f);
      s1->validaSolucao("Validando a solucao depois de fazer a busca local.");

      //SolucaoOperacional * s2 = s1;
      //double e2 = e1;

      sStar->validaSolucao("Validando sStar antes de acceptanceCriterion ser executado.");
      SolucaoOperacional * sStar1 = &acceptanceCriterion(*sStar, *s1, *history);
      sStar->validaSolucao("Validando sStar depois de acceptanceCriterion ser executado.");
      sStar1->validaSolucao("Validando a solucao (sStar1) depois de executar o criterio de aceitacao.");

      delete sStar;
      //delete s2;
      delete s1;

      sStar = sStar1;
      eStar = e.avaliaSolucao(*sStar);

      timer->stop();
      elapsedTime = timer->getCPUTotalSecs();
      timer->start();

      //std::cout << "GETCHAR PROGRAMADO - ILS !!!" << std::endl;
      //getchar();

      std::cout << "\n\n";

   } while ( (target_f < eStar) && !terminationCondition(*history) && (elapsedTime < timelimit));

   s = *sStar;

   delete sStar;

   delete timer;

   std::cout << "ILS ends:\tSolution Evaluation: " << eStar << std::endl;
   std::cout << "Elapsed time in ILS exec:\t" << elapsedTime << " secs.\n";
}