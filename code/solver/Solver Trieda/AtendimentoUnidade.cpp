#include "AtendimentoUnidade.h"

AtendimentoUnidade::AtendimentoUnidade(void)
{
}
 
AtendimentoUnidade::~AtendimentoUnidade(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoUnidade& unidade)
{
   out << "<Unidade>" << endl;

   out << "<unidadeId>" << unidade.unidade_id << "</unidadeId>";

   out << "<SalaSet>" << endl;

   GGroup<AtendimentoSala*>::GGroupIterator it_sala = unidade.atendimentos_salas.begin();

   for(; it_sala != unidade.atendimentos_salas.end(); it_sala++)
   {
      out << (*it_sala) << endl;
   }

   out << "</SalaSet>" << endl;

   out << "</Unidade>" << endl;

   return out;
}