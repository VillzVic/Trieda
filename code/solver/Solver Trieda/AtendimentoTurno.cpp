#include "AtendimentoTurno.h"

AtendimentoTurno::AtendimentoTurno(void)
{
}

AtendimentoTurno::~AtendimentoTurno(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoTurno& turno)
{
   out << "<Turno>" << endl;

   out << "<turnoId>" << turno.turno_id << "</turnoId>" << endl;

   out << "<HorarioAulaSet>" << endl;

   GGroup<AtendimentoHorarioAula*>::GGroupIterator it_horario_aula = turno.atendimentos_horarios_aula.begin();

   for(; it_horario_aula != turno.atendimentos_horarios_aula.end(); it_horario_aula++)
   {
	   out << **it_horario_aula;
   }

   out << "</HorarioAulaSet>" << endl;

   out << "</Turno>" << endl;

   return out;
}