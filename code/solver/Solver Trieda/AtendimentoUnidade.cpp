#include "AtendimentoUnidade.h"

AtendimentoUnidade::AtendimentoUnidade(void)
{
   //atendimentos_salas = NULL;
   atendimentos_salas = new GGroup<AtendimentoSala*>();

   unidade = NULL;
}

AtendimentoUnidade::~AtendimentoUnidade(void)
{
   if( atendimentos_salas != NULL )
   {
      atendimentos_salas->deleteElements();
      delete atendimentos_salas;
   }
}

std::ostream& operator << ( std::ostream & out, AtendimentoUnidade & unidade )
{
   out << "<AtendimentoUnidade>" << std::endl;
   out << "<unidadeId>" << unidade.getId() << "</unidadeId>" << std::endl;
   out << "<unidadeCodigo>" << unidade.getCodigoUnidade() << "</unidadeCodigo>" << std::endl;
   out << "<atendimentosSalas>" << std::endl;

   GGroup< AtendimentoSala * >::GGroupIterator it_sala
	   = unidade.atendimentos_salas->begin();

   for (; it_sala != unidade.atendimentos_salas->end();
	       it_sala++ )
   {
      out << ( **it_sala );
   }

   out << "</atendimentosSalas>" << std::endl;
   out << "</AtendimentoUnidade>" << std::endl;

   return out;
}