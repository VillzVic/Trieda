#include "AtendimentoDiaSemana.h"

AtendimentoDiaSemana::AtendimentoDiaSemana(void)
{
}

AtendimentoDiaSemana::~AtendimentoDiaSemana(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoDiaSemana& diaSem)
{
   out << "<DiaSemana>" << endl;

   out << "<diaSemana>" << diaSem.dia_semana << "</diaSemana>";

   // ver com o luis, como testar isso.. .
   if(diaSem.atendimentos_tatico.size() > 0)
   {
      out << "<TaticoSet>" << endl;

      GGroup<AtendimentoTatico*>::GGroupIterator it_tatico = diaSem.atendimentos_tatico.begin();

      for(; it_tatico != diaSem.atendimentos_tatico.end(); it_tatico++)
      {
         out << (*it_tatico) << endl;
      }

      out << "</TaticoSet>" << endl;
   }
   else if (diaSem.atendimentos_turno.size() > 0)
   {
      out << "<TurnoSet>" << endl;

      GGroup<AtendimentoTurno*>::GGroupIterator it_turno = diaSem.atendimentos_turno.begin();

      for(; it_turno != diaSem.atendimentos_turno.end(); it_turno++)
      {
         out << (*it_turno) << endl;
      }

      out << "</TurnoSet>" << endl;
   }

   out << "</DiaSemana>" << endl;

   return out;
}