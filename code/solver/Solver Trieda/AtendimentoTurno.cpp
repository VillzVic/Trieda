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
   out << "<AtendimentoTurno>" << std::endl;

   out << "<turnoId>" << turno.getTurnoId() << "</turnoId>" << std::endl;

   out << "<atendimentosHorariosAula>" << std::endl;
   GGroup< AtendimentoHorarioAula * >::GGroupIterator it_horario_aula
	   = turno.atendimentos_horarios_aula->begin();
   for(; it_horario_aula != turno.atendimentos_horarios_aula->end();
	     it_horario_aula++ )
   {
	   out << ( **it_horario_aula );
   }
   out << "</atendimentosHorariosAula>" << std::endl;

   out << "</AtendimentoTurno>" << std::endl;

   return out;
}