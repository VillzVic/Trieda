#include "AtendimentoSala.h"

AtendimentoSala::AtendimentoSala( int id )
{
   this->setId( id );
   this->atendimentos_dias_semana = new GGroup<AtendimentoDiaSemana*>();

   sala = NULL;
}

AtendimentoSala::~AtendimentoSala(void)
{
   this->setId( -1 );

   if( atendimentos_dias_semana != NULL )
   {
      atendimentos_dias_semana->deleteElements();
      delete atendimentos_dias_semana;
   }
}

std::ostream & operator << ( std::ostream & out, AtendimentoSala & sala )
{
   out << "<AtendimentoSala>" << std::endl;

   out << "<salaId>" << sala.getId() << "</salaId>" << std::endl;
   out << "<salaNome>" << sala.getSalaId() << "</salaNome>" << std::endl;

   out << "<atendimentosDiasSemana>" << std::endl;

   GGroup< AtendimentoDiaSemana * >::GGroupIterator it_diasSem
	   = sala.atendimentos_dias_semana->begin();

   for(; it_diasSem != sala.atendimentos_dias_semana->end();
	     it_diasSem++ )
   {
      out << ( **it_diasSem );
   }

   out << "</atendimentosDiasSemana>" << std::endl;
   out << "</AtendimentoSala>" << std::endl;

   return out;
}
