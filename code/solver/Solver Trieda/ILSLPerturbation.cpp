#include "ILSLPerturbation.h"

ILSLPerturbationLPlus2::ILSLPerturbationLPlus2(Avaliador & e, int _pMax, NS& _ns) : evaluator(e), pMax(_pMax)
{
   ns.push_back(&_ns);
}

ILSLPerturbationLPlus2::~ILSLPerturbationLPlus2()
{
}

void ILSLPerturbationLPlus2::add_ns(NS & _ns)
{
   ns.push_back(&_ns);
}

void ILSLPerturbationLPlus2::perturb(SolucaoOperacional & s, double timelimit, double target_f, int level)
{
   //int f = 0; // number of failures
   int a = 0; // number of appliable moves

   level += 2; // level 0 applies 2 moves
   std::cout << "Perturbation level: " << level << std::endl;

   int x = rand() % ns.size();

   //std::cout << "Using ";
   //ns[x]->print();

   //while ((a < level) && (f < pMax))
   while (a < level)
   {
      Move & m = ns[x]->move(s);

      //if (m.canBeApplied(e, s))
      {
         a++;
         delete &(m.apply(s));
         s.validaSolucao("\tValidando a solucao depois do aplicar um movimento.");
      }
      //else
         //f++;

      delete &m;
   }

   //if (f == pMax)
      //cout << "ILS Warning: perturbation had no effect in " << pMax << " tries!" << endl;

   //evaluator.evaluate(e, s); // updates 'e'

}