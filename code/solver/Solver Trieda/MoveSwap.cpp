#include "MoveSwap.h"

MoveSwap::MoveSwap( Aula & _a1, Aula & _a2 )
   : a1( _a1 ), a2( _a2 )
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

Move & MoveSwap::apply( SolucaoOperacional & s )
{
   print();

   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::iterator 
      itBlocoAula_1 = s.blocoAulas.find( &a1 );
   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::iterator 
      itBlocoAula_2 = s.blocoAulas.find( &a2 );

   if( ( itBlocoAula_1 == s.blocoAulas.end() ) || ( itBlocoAula_2 == s.blocoAulas.end() ) )
   {
      std::cout << "MoveSwap -> Alguma aula nao foi encontrada no blocoAulas da solucao corrente." << std::endl;
      exit(1);
   }

   profA1 = itBlocoAula_1->second.first;
   profA2 = itBlocoAula_2->second.first;

   // Para trocar dois blocos de aula de horários deve-se: 
   // 1 - Na solução, trocar as referências das aulas.
   // 2 - Nas aulas, trocar os vetores que indicam os blocos.

   int indice = 0, linha = 0, coluna = 0;
   int totalHorariosAula = s.getProblemData()->horarios_aula_ordenados.size();

   std::vector< HorarioAula * >::iterator itBlocoAula;

   // Alocando a aula 2 nos horários da aula 1
   itBlocoAula = itBlocoAula_1->second.second.begin();
   for (; itBlocoAula != itBlocoAula_1->second.second.end(); ++itBlocoAula )
   {
      indice = s.posicao_horario_aula( ( *itBlocoAula )->getId() );

	  linha = profA1->getIdOperacional();
	  coluna = ( a1.getDiaSemana() - 1 ) * totalHorariosAula + indice;

      s.getMatrizAulas()->at( linha )->at( coluna ) = ( &a2 );
   }

   // Alocando a aula 1 nos horários da aula 2
   itBlocoAula = itBlocoAula_2->second.second.begin();
   for (; itBlocoAula != itBlocoAula_2->second.second.end(); ++itBlocoAula )
   {
      indice = s.posicao_horario_aula( ( *itBlocoAula )->getId() );

	  linha = profA2->getIdOperacional();
	  coluna = ( a2.getDiaSemana() - 1 ) * totalHorariosAula + indice;

      s.getMatrizAulas()->at( linha )->at( coluna ) = ( &a1 );
   }

   itBlocoAula_1->second.first = profA2;
   itBlocoAula_2->second.first = profA1;

   itBlocoAula_1->second.second.swap( itBlocoAula_2->second.second );

   return ( *new MoveSwap( a2, a1 ) );
}

bool MoveSwap::operator == ( const Move & m ) const
{
   const MoveSwap & _m = ( const MoveSwap & ) m;

   return ( ( _m.a1 == a1 ) && ( _m.a2 == a2 ) || ( _m.a1 == a2 ) && ( _m.a2 == a1 ) );
}

void MoveSwap::print()
{
   // std::cout << "MoveSwap" << std::endl;
   // a1.toString();
   // a2.toString();
}