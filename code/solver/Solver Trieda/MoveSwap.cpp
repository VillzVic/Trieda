#include "MoveSwap.h"

//MoveSwap::MoveSwap( Aula & _a1, Professor & _profA1,
//									Aula & _a2, Professor & _profA2 )
//: a1( _a1 ), profA1( _profA1 ), a2( _a2 ), profA2( _profA2 )
//{
//}

MoveSwap::MoveSwap(Aula & _a1, Aula & _a2): a1(_a1), a2(_a2) //a1(_a1), profA1(*_a1.bloco_aula.begin()->first), a2(_a2), profA2(*_a2.bloco_aula.begin()->first)
{
   //itBlocoAula_1 = solOp.blocoAulas.find(&a1);
   //itBlocoAula_2 = solOp.blocoAulas.find(&a2);

   //if( (itBlocoAula_1 == solOp.blocoAulas.end()) || (itBlocoAula_2 == solOp.blocoAulas.end()))
   //{
   //   std::cout << "MoveSwap -> Alguma aula nao foi encontrada no blocoAulas da solucao corrente." << std::endl;
   //   exit(1);
   //}

   //profA1 = itBlocoAula_1->second.first;
   //profA2 = itBlocoAula_2->second.first;
}

MoveSwap::~MoveSwap()
{

}

//bool MoveSwap::canBeApplied(const SolucaoOperacional & s)
//{
//   return true;
//}

Move & MoveSwap::apply( SolucaoOperacional & s )
{
   print();

   std::map<Aula*,std::pair<Professor*,std::vector<HorarioAula*> > >::iterator 
      itBlocoAula_1 = s.blocoAulas.find(&a1);

   std::map<Aula*,std::pair<Professor*,std::vector<HorarioAula*> > >::iterator 
      itBlocoAula_2 = s.blocoAulas.find(&a2);

   if( (itBlocoAula_1 == s.blocoAulas.end()) || (itBlocoAula_2 == s.blocoAulas.end()))
   {
      std::cout << "MoveSwap -> Alguma aula nao foi encontrada no blocoAulas da solucao corrente." << std::endl;
      exit(1);
   }

   profA1 = itBlocoAula_1->second.first;
   profA2 = itBlocoAula_2->second.first;

   // Para trocar dois blocos de aula de horários deve-se: 
   // 1 - Na solução, trocar as referências das aulas.
   // 2 - Nas aulas, trocar os vetores que indicam os blocos.

   s.toString2();

   //std::cout << "------------------------------------------" << std::endl;
   //std::cout << "ANTES" << std::endl;
   //a1.toString();
   //a2.toString();
   //std::cout << "------------------------------------------" << std::endl;

   std::vector<HorarioAula*>::iterator 
      itBlocoAula;

   // Alocando a aula 2 nos horários da aula 1
   itBlocoAula = itBlocoAula_1->second.second.begin();

   for(; itBlocoAula != itBlocoAula_1->second.second.end(); ++itBlocoAula)
   {
      int indice = (*itBlocoAula)->getId();
      int totalHorariosAula = s.getProblemData()->horarios_aula_ordenados.size();

      s.getMatrizAulas()->at( profA1->getIdOperacional() )->at( ( (a1.getDiaSemana()-1) * totalHorariosAula ) + indice ) = ( &a2 );
   }

   // Alocando a aula 1 nos horários da aula 2
   itBlocoAula = itBlocoAula_2->second.second.begin();
   for(; itBlocoAula != itBlocoAula_2->second.second.end(); ++itBlocoAula)
   {
      int indice = (*itBlocoAula)->getId();
      int totalHorariosAula = s.getProblemData()->horarios_aula_ordenados.size();

      s.getMatrizAulas()->at( profA2->getIdOperacional() )->at( ( (a2.getDiaSemana()-1) * totalHorariosAula ) + indice ) = ( &a1 );
   }

   // Trocando os professores e os horários.
   //itBlocoAula_1->swap(*itBlocoAula_2);

   itBlocoAula_1->second.first = profA2;
   itBlocoAula_2->second.first = profA1;

   itBlocoAula_1->second.second.swap(itBlocoAula_2->second.second);

   //std::cout << "------------------------------------------" << std::endl;
   //std::cout << "DEPOIS" << std::endl;
   //a1.toString();
   //a2.toString();
   //std::cout << "------------------------------------------" << std::endl;

   return ( *new MoveSwap( a2, a1 ) );
}

bool MoveSwap::operator ==( const Move & m ) const
{
   const MoveSwap & _m = ( const MoveSwap & ) m;

   //return ( (_m.a1 == a1) && (_m.a2 == a2) );
   return ( (_m.a1 == a1) && (_m.a2 == a2) || (_m.a1 == a2) && (_m.a2 == a1) );
}

void MoveSwap::print()
{
   std::cout << "MoveSwap" << std::endl;

   a1.toString();
   a2.toString();
}