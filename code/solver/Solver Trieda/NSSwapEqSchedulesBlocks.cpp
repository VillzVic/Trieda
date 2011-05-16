#include "NSSwapEqSchedulesBlocks.h"

NSSwapEqSchedulesBlocks::NSSwapEqSchedulesBlocks(ProblemData & _problemData) : problemData(_problemData)
{
   moveValidator = new MoveSwapEqSchedulesBlocksValidator (&problemData);
}

NSSwapEqSchedulesBlocks::~NSSwapEqSchedulesBlocks()
{
   delete moveValidator;
}

std::pair<Aula*,Aula*> NSSwapEqSchedulesBlocks::pickTwoClasses(SolucaoOperacional& s)
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

   GGroup<Aula*>::iterator itAula = problemData.aulas.begin();

   int maxIter = rand() % (s.getMatrizAulas()->size()-1);
   for(int i = 0; i < maxIter; ++i, ++itAula);

   a1 = *itAula;
   a2 = *itAula;

   int attempt = -1;

   while(!moveValidator->isValid(*a1,*a2,s) && ++attempt < MAX_ATTEMPTS)
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

   if(attempt >= MAX_ATTEMPTS)
   {
      std::cout << "Warnning: No valid move generated after MAX_ATTEMPTS (" << MAX_ATTEMPTS << ") iterations." << std::endl;

      a1 = ( *itAula );
      a2 = ( *itAula );
   }

   return std::make_pair<Aula*,Aula*> (a1,a2);
}

Move & NSSwapEqSchedulesBlocks::move(SolucaoOperacional& s)
{
   std::pair<Aula*,Aula*> aulas = pickTwoClasses(s);

   Aula & a1 = *aulas.first;
   Aula & a2 = *aulas.second;

   //Professor & profA1 = *(a1.bloco_aula.begin()->first);
   //Professor & profA2 = *(a2.bloco_aula.begin()->first);

   return *(new MoveSwap(a1,a2));
}

void NSSwapEqSchedulesBlocks::print()
{
   std::cout << "NSSwapEqSchedulesBlocks" << std::endl;
}

