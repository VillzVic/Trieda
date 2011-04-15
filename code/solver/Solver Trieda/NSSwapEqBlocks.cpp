#include "NSSwapEqBlocks.h"

NSSwapEqBlocks::NSSwapEqBlocks(ProblemData & _problemData) : problemData(_problemData)
{
}

NSSwapEqBlocks::~NSSwapEqBlocks()
{
}

std::pair<Aula*,Aula*> NSSwapEqBlocks::pickTwoClasses(const SolucaoOperacional& s)
{
   /*
   Critérios a serem respeitados para a escolha das 2 aulas.

   1 - Não se deve selecionar a mesma aula.

   2 - Os professores das aulas selecionadas não podem ser os mesmos. (ESSA CONDIÇÃO NÃO É 
   NECESSÁRIA. SÓ ESTÁ SENDO CONSIDERADA PARA FACILITAR A IMPLEMENTAÇÃO).

   3 - O total de créditos da aula <a1> é igual ao total de créditos da aula <a2>.

   4 - Somente serão trocados o(s) horário(s) das aulas se o novo horário assumido por cada
   aula não conflitar com algum horário de alguma outra aula do mesmo bloco.

   5 - As aulas selecionadas não podem ser aulas virtuais.

   6 - As aulas escolhidas tem que estar alocadas para o mesmo dia.

   7 - As aulas escolhidas tem que estar alocadas para a mesma sala.

   */

   Aula * a1 = NULL;
   Aula * a2 = NULL;

   do
   {
      GGroup<Aula*>::iterator itAula = problemData.aulas.begin();

      int maxIter = rand() % s.getMatrizAulas()->size();
      for(int i = 0; i < maxIter; ++i, ++itAula);

      a1 = *itAula;

      Professor & profA1 = *a1->bloco_aula.begin()->first;
      Professor & profA2 = profA1;

      while(
         (*itAula == a1) ||
         //(profA1 == profA2) ||
         (itAula->getTotalCreditos() != a1->getTotalCreditos()) ||
         a1->eVirtual() ||
         itAula->eVirtual() ||
         itAula->getDiaSemana() != a1->getDiaSemana() || // modo não otimizado. Poderia ter alguma estrutura que auxiliasse nessa escolha.
         itAula->getSala() != a1->getSala()
         )
      {
         itAula = problemData.aulas.begin();

         maxIter = rand() % s.getMatrizAulas()->size();
         for(int i = 0; i < maxIter; ++i, ++itAula);

         profA2 = *itAula->bloco_aula.begin()->first;

         if(a1->eVirtual())
         {
            a1 = *itAula;
            continue;
         }
      }

      a2 = *itAula;

   } while(!s.podeTrocarHorariosAulas(*a1,*a2));

   return std::make_pair<Aula*,Aula*> (a1,a2);
}

Move & NSSwapEqBlocks::move(const SolucaoOperacional& s)
{
   // Selecionando 2 aulas quaisquer.
   std::pair<Aula*,Aula*> aulas = pickTwoClasses(s);

   Aula & a1 = *aulas.first;
   
   Aula & a2 = *aulas.second;

   Professor & profA1 = *(a1.bloco_aula.begin()->first);

   Professor & profA2 = *(a2.bloco_aula.begin()->first);

   return *(new MoveSwapEqBlocks(a1,profA1,a2,profA2));
}

void NSSwapEqBlocks::print()
{
   std::cout << "NSSwapEqBlocks" << std::endl;
}
