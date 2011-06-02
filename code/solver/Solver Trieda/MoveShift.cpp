#include "MoveShift.h"

MoveShift::MoveShift( Aula & _aula, Professor & _novoProfAula,
                      std::vector< HorarioAula * > _blocoHorariosVagos )
                      : aula( _aula ), novoProfAula( _novoProfAula )
{
   blocoHorariosVagos = _blocoHorariosVagos;
}

MoveShift::~MoveShift( void )
{

}

Move & MoveShift::apply( SolucaoOperacional & s )
{
   // Para realocar uma aula deve-se: 
   // 1 - Na solução, referenciar os novos horários da aula e apontar para NULL os horários antigos.
   // 2 - Na solução, trocar o vetor que indica o bloco.

   this->print();
   int indice = 0, linha = 0, coluna = 0;

   std::map< Aula *, std::pair< Professor * , std::vector< HorarioAula * > >, LessPtr< Aula > >::iterator 
      itBlocoAula = s.blocoAulas.find( &aula );

   if ( itBlocoAula == s.blocoAulas.end() )
   {
      std::cout << "MoveShift -> Aula nao encontrada no "
                << "blocoAulas da solucao corrente." << std::endl;

      exit(1);
   }

   profAula = itBlocoAula->second.first;
   int totalHorariosAula = s.getProblemData()->horarios_aula_ordenados.size();      
   std::vector< HorarioAula * >::iterator itHorarioAula;

   // Desalocando os horários da aula em questão.
   itHorarioAula = itBlocoAula->second.second.begin();
   for (; itHorarioAula != itBlocoAula->second.second.end(); ++itHorarioAula )
   {
      indice = s.posicao_horario_aula( ( *itHorarioAula )->getId() );

      linha = profAula->getIdOperacional();
      coluna = ( aula.getDiaSemana() - 1 ) * totalHorariosAula + indice;

      s.getMatrizAulas()->at( linha )->at( coluna ) = ( NULL );
   }

   // Alocando os novos horários para a aula em questão.
   itHorarioAula =  blocoHorariosVagos.begin();
   for (; itHorarioAula != blocoHorariosVagos.end(); ++itHorarioAula )
   {
      indice = s.posicao_horario_aula( ( *itHorarioAula )->getId() );

      linha = novoProfAula.getIdOperacional();
      coluna = ( aula.getDiaSemana() - 1 ) * totalHorariosAula + indice;

      s.getMatrizAulas()->at( linha )->at( coluna ) = ( &aula );
   }

   // Trocando o professor e os horários.
   itBlocoAula->second.first = &( novoProfAula );
   itBlocoAula->second.second.swap( blocoHorariosVagos );

   return ( *new MoveShift( aula, *profAula, blocoHorariosVagos ) );
}

bool MoveShift::operator == ( const Move & m ) const
{
   const MoveShift & _m = ( const MoveShift & ) m;
   if ( _m.aula != aula )
   {
      return false;
   }

   return ( _m.novoProfAula == novoProfAula );
}

void MoveShift::print()
{
   // std::cout << "\n<---> MoveShift <--->\n" << std::endl;
   // aula.toString();
   // std::cout << "Professor atual (idOp): " << profAula.getIdOperacional() << std::endl;
   // std::cout << "Professor candidato (idOp): " << novoProfAula.getIdOperacional() << std::endl;
}
