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
      cout << "level " << level << ".." << endl;
   }

   // Atualiza o historico
   history.first.first = iter;
   history.first.second = level;
}


SolucaoOperacional & IteratedLocalSearchLevels::acceptanceCriterion(SolucaoOperacional & s1, SolucaoOperacional & s2, levelHistory & history)
{
   double eS1 = e.avaliaSolucao(s1);
   double eS2 = e.avaliaSolucao(s2);

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

   CPUTimer * timer = new CPUTimer();
   timer->start();

   levelHistory * history = &initializeHistory();

   // 's0' <- GenerateSolution
   // 's*' <- localSearch 's'


   localSearch(s, (timelimit - timer->getCPUTotalSecs()), target_f);

   SolucaoOperacional * sStar = new SolucaoOperacional(s);
   double eStar = e.avaliaSolucao(s);

   cout << "ILS starts: ";
   std::cout  << "Solution Evaluation: " << eStar << std::endl;

   do
   {
      SolucaoOperacional * s1 = new SolucaoOperacional(*sStar);
      double e1 = eStar;

      perturbation(*s1, (timelimit - timer->getCPUTotalSecs()), target_f, *history);

      localSearch(*s1, (timelimit - timer->getCPUTotalSecs()), target_f);

      SolucaoOperacional * s2 = s1;
      double e2 = e1;

      SolucaoOperacional * sStar1 = &acceptanceCriterion(*sStar, *s2, *history);

      delete sStar;
      delete s2;

      //sStar = sStar1;
      eStar = e.avaliaSolucao(*sStar);

   } while ( (target_f < eStar) && !terminationCondition(*history) && (timer->getCPUTotalSecs() < timelimit));

   s = *sStar;

   delete sStar;

   delete timer;
}