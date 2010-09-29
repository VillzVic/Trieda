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

   out << "<horarioAulaId>" << horario_aula.horario_aula_id << "</horarioAulaId>" << endl;

   out << "<professorId>" << horario_aula.professor_id << "</professorId>" << endl;

   out << "<creditoTeorico>" << horario_aula.credito_teorico << "</creditoTeorico>" << endl;

   out << "<OfertaSet>" << endl;

   GGroup<AtendimentoOferta*>::GGroupIterator it_oferta = horario_aula.atendimentos_ofertas.begin();

   for(; it_oferta != horario_aula.atendimentos_ofertas.end(); it_oferta++)
   {
	   out << **it_oferta;
   }

   out << "</OfertaSet>" << endl;

   out << "</HorarioAula>" << endl;

   return out;
}
