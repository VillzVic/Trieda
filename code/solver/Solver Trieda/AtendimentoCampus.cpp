#include "AtendimentoCampus.h"

AtendimentoCampus::AtendimentoCampus(void)
{
   atendimentos_unidades = new GGroup< AtendimentoUnidade * >();
   campus = NULL;
}

AtendimentoCampus::~AtendimentoCampus(void)
{
   if( atendimentos_unidades != NULL )
   {
      atendimentos_unidades->deleteElements();
      delete atendimentos_unidades;
   }
}

std::ostream & operator << ( std::ostream & out, AtendimentoCampus & campus )
{
   out << "<AtendimentoCampus>" << std::endl;
   out << "<campusId>" << campus.getId() << "</campusId>" << std::endl;
   out << "<campusCodigo>" << campus.campus_id << "</campusCodigo>" << std::endl;
   out << "<atendimentosUnidades>" << std::endl;

   GGroup< AtendimentoUnidade * >::GGroupIterator it_unidade
	   = campus.atendimentos_unidades->begin();
   for(; it_unidade != campus.atendimentos_unidades->end();
		 it_unidade++ )
   {
      out << ( **it_unidade );
   }

   out << "</atendimentosUnidades>" << std::endl;
   out << "</AtendimentoCampus>" << std::endl;

   return out;
}