#include "MoveSwapEqBlocks.h"

MoveSwapEqBlocks::MoveSwapEqBlocks(Aula & _a1, Professor & _profA1, Aula & _a2, Professor & _profA2) : a1(_a1), profA1(_profA1), a2(_a2), profA2(_profA2)
{
}

MoveSwapEqBlocks::~MoveSwapEqBlocks()
{
}

Move & MoveSwapEqBlocks::apply(SolucaoOperacional & s)
{
   /* Para trocar dois blocos de aula de horários deve-se: 
   
   1 - Na solução, trocar as referências das aulas.
   2 - Nas aulas, trocar os vetores que indicam os blocos.

   */

   std::cout << "------------------------------------------" << std::endl;
   std::cout << "ANTES" << std::endl;
   a1.toString();
   a2.toString();
   std::cout << "------------------------------------------" << std::endl;

   std::vector< std::pair< Professor *, Horario * > >::iterator 
      itBlocoAula;
   
   itBlocoAula = a1.bloco_aula.begin();

   // Alocando a aula 2 nos horários da aula 1
   for(; itBlocoAula != a1.bloco_aula.end(); ++itBlocoAula)
   {
      int indice = (s.refHorarios.find(std::make_pair(itBlocoAula->second,a1.getDiaSemana())))->second;

      s.getMatrizAulas()->at(profA1.getIdOperacional())->at(indice) = &a2;
   }

   itBlocoAula = a2.bloco_aula.begin();

   // Alocando a aula 1 nos horários da aula 2
   for(; itBlocoAula != a2.bloco_aula.end(); ++itBlocoAula)
   {
      int indice = (s.refHorarios.find(std::make_pair(itBlocoAula->second,a2.getDiaSemana())))->second;

      s.getMatrizAulas()->at(profA2.getIdOperacional())->at(indice) = &a1;
   }

   // Trocando os horários.
   a1.bloco_aula.swap(a2.bloco_aula);

   std::cout << "------------------------------------------" << std::endl;
   std::cout << "DEPOIS" << std::endl;
   a1.toString();
   a2.toString();
   std::cout << "------------------------------------------" << std::endl;

   return *(new MoveSwapEqBlocks(a2,profA1,a1,profA2));
}

bool MoveSwapEqBlocks::operator==(const Move & m) const
{
   const MoveSwapEqBlocks & _m = (const MoveSwapEqBlocks &) m;

   return ((_m.a1 == a1) && (_m.a2 == a2));
}

void MoveSwapEqBlocks::print()
{
   std::cout << "MoveSwapEqBlocks" << std::endl;
}