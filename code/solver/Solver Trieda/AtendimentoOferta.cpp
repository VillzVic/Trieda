#include "AtendimentoOferta.h"

AtendimentoOferta::AtendimentoOferta( int id )
{
   this->setId( id );
   this->oferta_curso_campi_id = "";
   this->disciplina_id = -1;
   this->quantidade = 0;
   this->turma = 99999999;
   this->oferta = NULL;
}

AtendimentoOferta::~AtendimentoOferta( void )
{
   this->setId( -1 );
   this->oferta_curso_campi_id = "";
   this->disciplina_id = -1;
   this->quantidade = 0;
   this->turma = 99999999;
   this->oferta = NULL;
}

std::ostream & operator << ( std::ostream & out, AtendimentoOferta & oferta )
{
   out << "<ofertaCursoCampiId>" << oferta.getOfertaCursoCampiId() << "</ofertaCursoCampiId>" << std::endl;
   out << "<disciplinaId>" << abs( oferta.getDisciplinaId() ) << "</disciplinaId>" << std::endl;
   out << "<quantidade>" << oferta.getQuantidade() << "</quantidade>" << std::endl;
   out << "<turma>" << oferta.getTurma() << "</turma>" << std::endl;

   return out;
}