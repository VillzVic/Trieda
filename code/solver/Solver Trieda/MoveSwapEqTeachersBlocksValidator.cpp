#include "MoveSwapEqTeachersBlocksValidator.h"

MoveSwapEqTeachersBlocksValidator::MoveSwapEqTeachersBlocksValidator(ProblemData * pD) : MoveValidator(pD)
{
}

MoveSwapEqTeachersBlocksValidator::~MoveSwapEqTeachersBlocksValidator()
{
}

bool MoveSwapEqTeachersBlocksValidator::isValid(Aula & aX, Aula & aY, SolucaoOperacional & solOp)
{
   // --------------------------------------------------------------------------
   // Comparing the teachers.
   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::iterator 
      itBlocoAula_X = solOp.blocoAulas.find(&aX);

   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::iterator 
      itBlocoAula_Y = solOp.blocoAulas.find(&aY);

   if((itBlocoAula_X == solOp.blocoAulas.end()) || (itBlocoAula_Y == solOp.blocoAulas.end()))
   {
      std::cout << "Nao encontrei a aula desejada na estrutura de blocoAula de uma dada solucao. ERRO.\n";
      exit(1);
   }

   if(*itBlocoAula_X->second.first != *itBlocoAula_Y->second.first)
      return false;

   // --------------------------------------------------------------------------

   return(
      (canSwapSchedule(aX,aY,solOp)) &&

      (aX.getTotalCreditos() == aY.getTotalCreditos()) &&

      (aX.getDiaSemana() == aY.getDiaSemana()) &&

      (aX.getSala() == aY.getSala()) &&

      (aX != aY) &&

      !(aX.getDisciplina()->eLab()) &&
      !(aY.getDisciplina()->eLab()) &&

      !(aX.eVirtual()) &&
      !(aY.eVirtual()) //&&

      //(*aX.bloco_aula.begin()->first) != (*aY.bloco_aula.begin()->first) // Comparing the teachers.
      );
}