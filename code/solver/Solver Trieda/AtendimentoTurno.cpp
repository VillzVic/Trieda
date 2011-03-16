#include "AtendimentoTurno.h"

AtendimentoTurno::AtendimentoTurno(void)
{
   //atendimentos_horarios_aula = NULL;
   atendimentos_horarios_aula = new GGroup<AtendimentoHorarioAula*>();
}

AtendimentoTurno::~AtendimentoTurno(void)
{
   if( atendimentos_horarios_aula != NULL )
   {
      atendimentos_horarios_aula->deleteElements();
      delete atendimentos_horarios_aula;
   }
}

std::ostream& operator << (std::ostream& out, AtendimentoTurno& turno)
{
   out << "<AtendimentoTurno>" << endl;

   out << "<turnoId>" << turno.turno_id << "</turnoId>" << endl;

   out << "<atendimentosHorariosAula>" << endl;

   //GGroup<AtendimentoHorarioAula*>::GGroupIterator it_horario_aula = turno.atendimentos_horarios_aula.begin();
   GGroup<AtendimentoHorarioAula*>::GGroupIterator it_horario_aula = turno.atendimentos_horarios_aula->begin();

   //for(; it_horario_aula != turno.atendimentos_horarios_aula.end(); it_horario_aula++)
   for(; it_horario_aula != turno.atendimentos_horarios_aula->end(); it_horario_aula++)
   {
	   out << **it_horario_aula;
   }

   out << "</atendimentosHorariosAula>" << endl;

   out << "</AtendimentoTurno>" << endl;

   return out;
}