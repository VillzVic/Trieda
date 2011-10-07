#include "AtendimentoDiaSemana.h"

AtendimentoDiaSemana::AtendimentoDiaSemana( int id )
{
   this->setId( id );
   this->atendimentos_tatico = new GGroup< AtendimentoTatico * >();
   this->atendimentos_turno = new GGroup< AtendimentoTurno * >();
}

AtendimentoDiaSemana::~AtendimentoDiaSemana( void )
{
   this->setId( -1 );

   if ( atendimentos_tatico != NULL )
   {
      atendimentos_tatico->deleteElements();
      delete atendimentos_tatico;
   }

   if ( atendimentos_turno != NULL )
   {
      atendimentos_turno->deleteElements();
      delete atendimentos_turno;
   }
}

std::ostream & operator << ( std::ostream & out, AtendimentoDiaSemana & diaSem )
{
   out << "<AtendimentoDiaSemana>" << std::endl;
   out << "<diaSemana>" << diaSem.getDiaSemana() << "</diaSemana>" << std::endl;

   if ( diaSem.atendimentos_tatico != NULL
      && diaSem.atendimentos_tatico->size() > 0 )
   {
      out << "<atendimentosTatico>" << std::endl;

      GGroup< AtendimentoTatico * >::GGroupIterator it_tatico
         = diaSem.atendimentos_tatico->begin();

      for (; it_tatico != diaSem.atendimentos_tatico->end();
            it_tatico++ )
      {
         out << ( **it_tatico );
      }

      out << "</atendimentosTatico>" << std::endl;
   }
   else if ( diaSem.atendimentos_turno != NULL
            && diaSem.atendimentos_turno->size() > 0 )
	{
      out << "<atendimentosTurnos>" << std::endl;

      GGroup< AtendimentoTurno * >::GGroupIterator it_turno
         = diaSem.atendimentos_turno->begin();

      for (; it_turno != diaSem.atendimentos_turno->end();
            it_turno++ )
      {
         out << ( **it_turno );
      }

      out << "</atendimentosTurnos>" << std::endl;
	}

	out << "</AtendimentoDiaSemana>" << std::endl;

	return out;
}