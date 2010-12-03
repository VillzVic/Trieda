#include "AtendimentoUnidade.h"

AtendimentoUnidade::AtendimentoUnidade(void)
{
}

AtendimentoUnidade::~AtendimentoUnidade(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoUnidade& unidade)
{
   out << "<AtendimentoUnidade>" << endl;

   //out << "<Id>" << unidade.getId() << "</Id>" << endl;

   out << "<unidadeId>" << unidade.getId() << "</unidadeId>" << endl;

   out << "<unidadeCodigo>" << unidade.unidade_id << "</unidadeCodigo>" << endl;

   out << "<atendimentosSalas>" << endl;

   GGroup<AtendimentoSala*>::GGroupIterator it_sala = unidade.atendimentos_salas.begin();

   for(; it_sala != unidade.atendimentos_salas.end(); it_sala++)
   {
      out << **it_sala;
   }

   out << "</atendimentosSalas>" << endl;

   out << "</AtendimentoUnidade>" << endl;

   return out;
}