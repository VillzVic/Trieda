#include "AtendimentoOferta.h"

AtendimentoOferta::AtendimentoOferta(void)
{
}

AtendimentoOferta::~AtendimentoOferta(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoOferta& oferta)
{
   out << "<Oferta>" << endl;

   out << "<ofertaCursoCampiId>" << oferta.oferta_curso_campi_id << "</ofertaCursoCampiId>";

   out << "<disciplinaId>" << oferta.disciplina_id << "</disciplinaId>";

   out << "<quantidade>" << oferta.quantidade << "</quantidade>";

   out << "</Oferta>" << endl;

   return out;
}