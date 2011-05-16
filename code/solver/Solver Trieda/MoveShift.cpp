#include "MoveShift.h"

MoveShift::MoveShift(
                     Aula & _aula,
                     Professor & _novoProfAula, 
                     std::vector<HorarioAula*> _blocoHorariosVagos
                     ) : aula(_aula), novoProfAula(_novoProfAula)
{
   blocoHorariosVagos = _blocoHorariosVagos;
}

MoveShift::~MoveShift(void)
{
}

Move & MoveShift::apply( SolucaoOperacional & s )
{
   print();
   // Para realocar uma aula deve-se: 
   // 1 - Na solução, referenciar os novos horários da aula e apontar para NULL os horários antigos.
   // 2 - Na aula, trocar o vetor que indica o bloco.

   std::map<Aula*,std::pair<Professor*,std::vector<HorarioAula*> > >::iterator 
      itBlocoAula = s.blocoAulas.find(&aula);

   if(itBlocoAula == s.blocoAulas.end())
   {
      std::cout << "MoveShift -> Aula nao encontrada no blocoAulas da solucao corrente." << std::endl;
      exit(1);
   }

   profAula = itBlocoAula->second.first;

   //std::cout << "MOVE SHIFT - INIT\n\n";

   //std::cout << "------------------------------------------" << std::endl;
   //std::cout << "ANTES" << std::endl;
   //aula.toString();

   //std::cout << "\nNovo professor (idOperacional): " << novoProfAula.getIdOperacional() << std::endl;
   //std::cout << "\nNovo(s) Horario(s):\n\t";

   //std::vector< std::pair< Professor *, Horario * > >::iterator
   //   itBA = blocoHorariosVagos.begin();
   //for(; itBA != blocoHorariosVagos.end();
   //   ++itBA )
   //{
   //   HorarioAula * horario_aula
   //      = itBA->second->horario_aula;
   //   std::cout << horario_aula->getInicio() << "\n\t";
   //}
   //std::cout << std::endl;
   //std::cout << "------------------------------------------" << std::endl;

   std::vector<HorarioAula*>::iterator
      itHorarioAula = itBlocoAula->second.second.begin();

   // Desalocando os horários da aula em questão.
   for(; itHorarioAula != itBlocoAula->second.second.end(); ++itHorarioAula)
   {
      int indice = (*itHorarioAula)->getId();
      int totalHorariosAula = s.getProblemData()->horarios_aula_ordenados.size();

      s.getMatrizAulas()->at( profAula->getIdOperacional() )->at( ( (aula.getDiaSemana()-1) * totalHorariosAula ) + indice ) = ( NULL );
   }

   // Alocando os novos horários para a aula em questão.
   itHorarioAula =  blocoHorariosVagos.begin();

   for(; itHorarioAula != blocoHorariosVagos.end(); ++itHorarioAula )
   {
      int indice = (*itHorarioAula)->getId();
      int totalHorariosAula = s.getProblemData()->horarios_aula_ordenados.size();      

      s.getMatrizAulas()->at( novoProfAula.getIdOperacional() )->at( ( (aula.getDiaSemana()-1) * totalHorariosAula ) + indice ) = ( &aula );
   }

   // Trocando o professor e os horários.
   itBlocoAula->second.first = &novoProfAula;

   itBlocoAula->second.second.swap(blocoHorariosVagos);

   //std::cout << "------------------------------------------" << std::endl;
   //std::cout << "DEPOIS" << std::endl;
   //aula.toString();

   //std::cout << "\nAntigo professor (idOperacional): " << profAula.getIdOperacional() << std::endl;
   //std::cout << "\nAntigo(s) Horario(s):\n\t";
   ////std::vector< std::pair< Professor *, Horario * > >::iterator
   //itBA = blocoHorariosVagos.begin();
   //for(; itBA != blocoHorariosVagos.end();
   //   ++itBA )
   //{
   //   HorarioAula * horario_aula
   //      = itBA->second->horario_aula;
   //   std::cout << horario_aula->getInicio() << "\n\t";
   //}
   //std::cout << std::endl;
   //std::cout << "------------------------------------------" << std::endl;

   //std::cout << "MOVE SHIFT - END\n\n";

   return ( *new MoveShift( aula, *profAula, blocoHorariosVagos ) );
}

bool MoveShift::operator ==( const Move & m ) const
{
   const MoveShift & _m = ( const MoveShift & ) m;

   if (_m.aula != aula)
   {
      //std::cout << "AULAS DIFERENTES!\n";
      return false;
   }

   //std::cout << "AULAS IGUAIS!\n";

   return ( _m.novoProfAula == novoProfAula );
}

void MoveShift::print()
{
   std::cout << "MoveShift" << std::endl;

   //aula.toString();
   //std::cout << "Professor atual (idOp): " << profAula.getIdOperacional() << std::endl;
   //std::cout << "Professor candidato (idOp): " << novoProfAula.getIdOperacional() << std::endl;
}
