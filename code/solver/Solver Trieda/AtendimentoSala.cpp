#include "AtendimentoSala.h"

AtendimentoSala::AtendimentoSala(void)
{
}

AtendimentoSala::~AtendimentoSala(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoSala& sala)
{
   out << "<Sala>" << endl;

   out << "<salaId>" << sala.sala_id << "</CampusId>";

   out << "<DiasSemanaSet>" << endl;

   GGroup<AtendimentoDiaSemana*>::GGroupIterator it_diasSem = sala.atendimentos_dias_semana.begin();

   for(; it_diasSem != sala.atendimentos_dias_semana.end(); it_diasSem++)
   {
      out << (*it_diasSem) << endl;
   }

   out << "</DiasSemanaSet>" << endl;

   out << "</Sala>" << endl;

   return out;
}
