#include "MoveShift.h"

MoveShift::MoveShift(
                     Aula & _aula,
                     Professor & _novoProfAula,
                     vector<pair<Professor*,Horario*> > _blocoHorariosVagos
                     ) : aula(_aula), profAula(*aula.bloco_aula.begin()->first), novoProfAula(_novoProfAula)
{
   blocoHorariosVagos = _blocoHorariosVagos;
}

MoveShift::~MoveShift(void)
{
}

Move & MoveShift::apply( SolucaoOperacional & s )
{
   // Para realocar uma aula deve-se: 
   // 1 - Na solução, referenciar os novos horários da aula e apontar para NULL os horários antigos.
   // 2 - Na aula, trocar o vetor que indica o bloco.

   //std::cout << "------------------------------------------" << std::endl;
   //std::cout << "ANTES" << std::endl;
   //a1.toString();
   //a2.toString();
   //std::cout << "------------------------------------------" << std::endl;

   vector< pair< Professor *, Horario * > >::iterator
      itBlocoAula = aula.bloco_aula.begin();

   // Desalocando os horários da aula em questão.
   for(; itBlocoAula != aula.bloco_aula.end(); ++itBlocoAula)
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
         std::cout << "ERRO em <MoveShift::apply(SolucaoOperacional & s)>. "
				   << "Indice de horario nao encontrado." << std::endl;
         exit(1);
      }
      
      s.getMatrizAulas()->at( profAula.getIdOperacional() )->at( ( (aula.getDiaSemana()-1) * totalHorariosAula ) + indice ) = ( NULL );
   }

   // Alocando os novos horários para a aula em questão.
   itBlocoAula =  blocoHorariosVagos.begin();
   for(; itBlocoAula != blocoHorariosVagos.end(); ++itBlocoAula )
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
         std::cout << "ERRO em <MoveShift::apply(SolucaoOperacional & s)>. "
				   << "Indice de horario nao encontrado." << std::endl;
         exit(1);
      }

      s.getMatrizAulas()->at( novoProfAula.getIdOperacional() )->at( ( (aula.getDiaSemana()-1) * totalHorariosAula ) + indice ) = ( &aula );
   }

   // Trocando os horários.
   aula.bloco_aula.swap(blocoHorariosVagos);

   //std::cout << "------------------------------------------" << std::endl;
   //std::cout << "DEPOIS" << std::endl;
   //a1.toString();
   //a2.toString();
   //std::cout << "------------------------------------------" << std::endl;

   return ( *new MoveShift( aula, profAula, blocoHorariosVagos ) );
}

bool MoveShift::operator ==( const Move & m ) const
{
   const MoveShift & _m = ( const MoveShift & ) m;

   if (_m.aula != aula)
      return false;

   // Checando se os horários são os mesmos.
   vector<pair<Professor*,Horario*> >::const_iterator itBlocoHorariosVagos = blocoHorariosVagos.begin();

   for(; itBlocoHorariosVagos != blocoHorariosVagos.end(); ++itBlocoHorariosVagos)
   {
      bool encontrou = false;

      vector<pair<Professor*,Horario*> >::iterator
         itHorariosAula = aula.bloco_aula.begin();

      for(; itHorariosAula != aula.bloco_aula.end(); ++itHorariosAula)
      {
         if(*(itHorariosAula->second) == *(itBlocoHorariosVagos->second))
         {
            encontrou = true;
            break;
         }
      }

      if(!encontrou)
         return false;
   }

   /*Se chegou aqui, eh pq os horarios sao iguais*/
   return ( profAula == novoProfAula );
}

void MoveShift::print()
{
   std::cout << "MoveShift" << std::endl;
}
