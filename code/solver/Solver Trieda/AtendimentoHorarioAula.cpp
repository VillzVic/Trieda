#include "AtendimentoHorarioAula.h"

AtendimentoHorarioAula::AtendimentoHorarioAula(void)
{
}

AtendimentoHorarioAula::~AtendimentoHorarioAula(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoHorarioAula& horario_aula)
{
   out << "<HorarioAula>" << endl;

   out << "<horarioAulaId>" << horario_aula.horario_aula_id << "</horarioAulaId>";

   out << "<professorId>" << horario_aula.professor_id << "</professorId>";

   out << "<creditoTeorico>" << horario_aula.credito_teorico << "</creditoTeorico>";

   out << "<OfertaSet>" << endl;

   GGroup<AtendimentoOferta*>::GGroupIterator it_oferta = horario_aula.atendimentos_ofertas.begin();

   for(; it_oferta != horario_aula.atendimentos_ofertas.end(); it_oferta++)
   {
      out << (*it_oferta) << endl;
   }

   out << "</OfertaSet>" << endl;

   out << "</HorarioAula>" << endl;

   return out;
}
