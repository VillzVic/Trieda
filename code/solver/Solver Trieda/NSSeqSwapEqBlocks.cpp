#include "NSSeqSwapEqBlocks.h"

NSSeqSwapEqBlocks::NSSeqSwapEqBlocks(ProblemData & _problemData) : problemData(_problemData)
{
   moveValidator = new MoveSwapEqBlocksValidator (&problemData);
}

NSSeqSwapEqBlocks::~NSSeqSwapEqBlocks()
{
   delete moveValidator;
}

std::pair<Aula*,Aula*> NSSeqSwapEqBlocks::pickTwoClasses(const SolucaoOperacional& s)
{
   /*
   Crit�rios a serem respeitados para a escolha das 2 aulas.

   1 - N�o se deve selecionar a mesma aula.

   2 - Os professores das aulas selecionadas n�o podem ser os mesmos. (ESSA CONDI��O N�O � 
   NECESS�RIA. N�O EST� SENDO CONSIDERADA).

   3 - O total de cr�ditos da aula <a1> � igual ao total de cr�ditos da aula <a2>.

   4 - Somente ser�o trocados o(s) hor�rio(s) das aulas se o novo hor�rio assumido por cada
   aula n�o conflitar com algum hor�rio de alguma outra aula do mesmo bloco.

   5 - As aulas selecionadas n�o podem ser aulas virtuais.

   6 - As aulas escolhidas tem que estar alocadas para o mesmo dia.

   7 - As aulas escolhidas tem que estar alocadas para a mesma sala.

   8 - As aulas a serem trocadas s� podem ser do tipo te�ricas (ou seja, a disciplina
   da aula em quest�o deve ter o eLab == False ).

   */

   Aula * a1 = NULL;
   Aula * a2 = NULL;

   //do
   //{
      GGroup<Aula*>::iterator itAula = problemData.aulas.begin();

      int maxIter = rand() % (s.getMatrizAulas()->size()-1);
      //int maxIter = 7;
      for(int i = 0; i < maxIter; ++i, ++itAula);

      a1 = *itAula;

   //   Professor & profA1 = *a1->bloco_aula.begin()->first;
   //   Professor & profA2 = profA1;

   //   while(
   //      (*itAula == a1) ||
   //      //(profA1 == profA2) ||
   //      (itAula->getTotalCreditos() != a1->getTotalCreditos()) ||
   //      a1->eVirtual() ||
   //      itAula->eVirtual() ||
   //      itAula->getDiaSemana() != a1->getDiaSemana() || // modo n�o otimizado. Poderia ter alguma estrutura que auxiliasse nessa escolha.
   //      itAula->getSala() != a1->getSala() ||
   //      itAula->getDisciplina()->eLab() ||
   //      a1->getDisciplina()->eLab()
   //      )
   //   {
   //      itAula = problemData.aulas.begin();

   //      maxIter = rand() % (s.getMatrizAulas()->size()-1);
   //      //maxIter = 6;
   //      for(int i = 0; i < maxIter; ++i, ++itAula);

   //      profA2 = *itAula->bloco_aula.begin()->first;

   //      if(a1->eVirtual() ||
   //         a1->getDisciplina()->eLab()
   //         )
   //      {
   //         a1 = *itAula;
   //         continue;
   //      }
   //   }

      a2 = *itAula;

   //} while(!s.podeTrocarHorariosAulas(*a1,*a2));

      while(!moveValidator->isValid(*a1,*a2))
      {
         itAula = problemData.aulas.begin();
         maxIter = rand() % (s.getMatrizAulas()->size()-1);
         for(int i = 0; i < maxIter; ++i, ++itAula);
         a1 = *itAula;

         itAula = problemData.aulas.begin();
         maxIter = rand() % (s.getMatrizAulas()->size()-1);
         for(int i = 0; i < maxIter; ++i, ++itAula);
         a2 = *itAula;
      }

   return std::make_pair<Aula*,Aula*> (a1,a2);
}

Move & NSSeqSwapEqBlocks::move(const SolucaoOperacional& s)
{
   // Selecionando 2 aulas quaisquer.
   std::pair<Aula*,Aula*> aulas = pickTwoClasses(s);

   Aula & a1 = *aulas.first;
   
   Aula & a2 = *aulas.second;

   Professor & profA1 = *(a1.bloco_aula.begin()->first);

   Professor & profA2 = *(a2.bloco_aula.begin()->first);

   return *(new MoveSwapEqBlocks(a1,profA1,a2,profA2));
}

NSIterator & NSSeqSwapEqBlocks::getIterator(const SolucaoOperacional & s)
{
   return *new NSIteratorSwapEqBlocks(*s.getProblemData(),s.getProblemData()->aulas);
}

void NSSeqSwapEqBlocks::print()
{
   std::cout << "NSSeqSwapEqBlocks" << std::endl;
}