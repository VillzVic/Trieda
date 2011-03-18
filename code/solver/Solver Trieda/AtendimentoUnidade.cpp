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

std::ostream& operator << (std::ostream& out, AtendimentoUnidade& unidade)
{
   out << "<AtendimentoUnidade>" << endl;

   //out << "<Id>" << unidade.getId() << "</Id>" << endl;

   out << "<unidadeId>" << unidade.getId() << "</unidadeId>" << endl;

   out << "<unidadeCodigo>" << unidade.unidade_id << "</unidadeCodigo>" << endl;

   out << "<atendimentosSalas>" << endl;

   //GGroup<AtendimentoSala*>::GGroupIterator it_sala = unidade.atendimentos_salas.begin();
   GGroup<AtendimentoSala*>::GGroupIterator it_sala = unidade.atendimentos_salas->begin();

   //for(; it_sala != unidade.atendimentos_salas.end(); it_sala++)
   for(; it_sala != unidade.atendimentos_salas->end(); it_sala++)
   {
      out << **it_sala;
   }

   out << "</atendimentosSalas>" << endl;

   out << "</AtendimentoUnidade>" << endl;

   return out;
}