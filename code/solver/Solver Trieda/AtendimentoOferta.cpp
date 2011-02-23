#include "AtendimentoOferta.h"

AtendimentoOferta::AtendimentoOferta(void)
{
   oferta_curso_campi_id = "";
   disciplina_id = -1;
   quantidade = 0;
   turma = 99999999;
}

AtendimentoOferta::~AtendimentoOferta(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoOferta& oferta)
{
   //out << "<AtendimentoOferta>" << endl;

   out << "<ofertaCursoCampiId>" << oferta.oferta_curso_campi_id << "</ofertaCursoCampiId>" << endl;

   out << "<disciplinaId>" << oferta.disciplina_id << "</disciplinaId>" << endl;

   out << "<quantidade>" << oferta.quantidade << "</quantidade>" << endl;

   out << "<turma>" << oferta.turma << "</turma>" << endl;

   //out << "</AtendimentoOferta>" << endl;

   return out;
}