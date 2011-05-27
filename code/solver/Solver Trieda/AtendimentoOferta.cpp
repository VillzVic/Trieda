#include "AtendimentoOferta.h"

AtendimentoOferta::AtendimentoOferta(void)
{
   oferta_curso_campi_id = "";
   disciplina_id = -1;
   quantidade = 0;
   turma = 99999999;

   oferta = NULL;
}

AtendimentoOferta::~AtendimentoOferta(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoOferta& oferta)
{
   out << "<AtendimentoOferta>" << std::endl;
   out << "<ofertaCursoCampiId>" << oferta.getOfertaCursoCampiId() << "</ofertaCursoCampiId>" << std::endl;
   out << "<disciplinaId>" << oferta.getDisciplinaId() << "</disciplinaId>" << std::endl;
   out << "<quantidade>" << oferta.getQuantidade() << "</quantidade>" << std::endl;
   out << "<turma>" << oferta.getTurma() << "</turma>" << std::endl;
   out << "</AtendimentoOferta>" << std::endl;

   return out;
}