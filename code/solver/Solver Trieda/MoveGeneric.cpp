#include "MoveGeneric.h"

MoveGeneric::MoveGeneric( Aula & _a1, Professor & _profA1,
									Aula & _a2, Professor & _profA2 )
: a1( _a1 ), profA1( _profA1 ), a2( _a2 ), profA2( _profA2 )
{
}

MoveGeneric::MoveGeneric(Aula & _a1, Aula & _a2) : a1(_a1), profA1(*_a1.bloco_aula.begin()->first), a2(_a2), profA2(*_a2.bloco_aula.begin()->first)
{
}

MoveGeneric::~MoveGeneric()
{

}

//bool MoveGeneric::canBeApplied(const SolucaoOperacional & s)
//{
//   return true;
//}

Move & MoveGeneric::apply( SolucaoOperacional & s )
{
   // Para trocar dois blocos de aula de hor�rios deve-se: 
   // 1 - Na solu��o, trocar as refer�ncias das aulas.
   // 2 - Nas aulas, trocar os vetores que indicam os blocos.

   //std::cout << "------------------------------------------" << std::endl;
   //std::cout << "ANTES" << std::endl;
   //a1.toString();
   //a2.toString();
   //std::cout << "------------------------------------------" << std::endl;

   std::vector< std::pair< Professor *, Horario * > >::iterator 
      itBlocoAula;

   // Alocando a aula 2 nos hor�rios da aula 1
   itBlocoAula = a1.bloco_aula.begin();
   for(; itBlocoAula != a1.bloco_aula.end(); ++itBlocoAula)
   {
      int indice = 0;
      int totalHorariosAula
		  = s.getProblemData()->horarios_aula_ordenados.size();
      for(; indice < totalHorariosAula; ++indice )
      {
         if ( s.getProblemData()->horarios_aula_ordenados.at( indice )
				== itBlocoAula->second->horario_aula )
		 {
            break;
		 }
      }

      if ( indice >= totalHorariosAula )
      {
         std::cout << "ERRO em <MoveGeneric::apply(SolucaoOperacional & s)>. "
				   << "Indice de horario nao encontrado." << std::endl;

         exit(1);
      }

      //s.getMatrizAulas()->at( profA1.getIdOperacional() )->at( indice ) = ( &a2 );
      s.getMatrizAulas()->at( profA1.getIdOperacional() )->at( ( (a1.getDiaSemana()-1) * totalHorariosAula ) + indice ) = ( &a2 );
   }

   // Alocando a aula 1 nos hor�rios da aula 2
   itBlocoAula = a2.bloco_aula.begin();
   for(; itBlocoAula != a2.bloco_aula.end(); ++itBlocoAula )
   {
      int indice = 0;
      int totalHorariosAula
		  = s.getProblemData()->horarios_aula_ordenados.size();      
      for(; indice < totalHorariosAula; ++indice )
      {
         if ( s.getProblemData()->horarios_aula_ordenados.at( indice )
				== itBlocoAula->second->horario_aula )
		 {
            break;
		 }
      }

      if ( indice >= totalHorariosAula )
      {
         std::cout << "ERRO em <MoveGeneric::apply(SolucaoOperacional & s)>. "
				   << "Indice de horario nao encontrado." << std::endl;

         exit(1);
      }

      //s.getMatrizAulas()->at( profA2.getIdOperacional() )->at( indice ) = ( &a1 );
      s.getMatrizAulas()->at( profA2.getIdOperacional() )->at( ( (a2.getDiaSemana()-1) * totalHorariosAula ) + indice ) = ( &a1 );
   }

   // Trocando os hor�rios.
   a1.bloco_aula.swap( a2.bloco_aula );

   //std::cout << "------------------------------------------" << std::endl;
   //std::cout << "DEPOIS" << std::endl;
   //a1.toString();
   //a2.toString();
   //std::cout << "------------------------------------------" << std::endl;

   return ( *new MoveGeneric( a2, profA1, a1, profA2 ) );
}

bool MoveGeneric::operator ==( const Move & m ) const
{
   const MoveGeneric & _m = ( const MoveGeneric & ) m;

   return ( (_m.a1 == a1) && (_m.a2 == a2) );
}

void MoveGeneric::print()
{
   std::cout << "MoveGeneric" << std::endl;
}