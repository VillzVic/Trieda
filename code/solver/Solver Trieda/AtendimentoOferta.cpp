#include "AtendimentoOferta.h"

AtendimentoOferta::AtendimentoOferta( int id )
{
   this->setId( id );
   this->oferta_curso_campi_id = "";
   this->disciplina_id = NULL;
   this->quantidade = 0;
   this->turma = 99999999;
   this->oferta = NULL;
   this->disciplina_substituta_id = NULL;
}

AtendimentoOferta::~AtendimentoOferta( void )
{
}

std::ostream & operator << ( std::ostream & out, AtendimentoOferta & atOferta )
{
   out << "<ofertaCursoCampiId>" << atOferta.getOfertaCursoCampiId() << "</ofertaCursoCampiId>" << std::endl;
   if ( atOferta.getDisciplinaSubstitutaId() != NULL )
		out << "<disciplinaSubstitutaId>" << abs( atOferta.getDisciplinaSubstitutaId() ) << "</disciplinaSubstitutaId>" << std::endl;
   out << "<disciplinaId>" << abs( atOferta.getDisciplinaId() ) << "</disciplinaId>" << std::endl;
   out << "<quantidade>" << atOferta.getQuantidade() << "</quantidade>" << std::endl;
   out << "<turma>" << atOferta.getTurma() << "</turma>" << std::endl;
	
   out << "<alunosDemandasAtendidas>" << std::endl;
  
   ITERA_GGROUP_N_PT( it, atOferta.alunosDemandasAtendidas, int )
   {
	   out << "<id>" << *it << "</id>" << std::endl;
   }
   out << "</alunosDemandasAtendidas>" << std::endl;
   

   return out;
}