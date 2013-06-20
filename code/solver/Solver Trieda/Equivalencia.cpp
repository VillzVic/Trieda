#include "Equivalencia.h"

Equivalencia::Equivalencia( void )
{
   this->setId( -1 );
   this->setDisciplinaCursouId(-1);
   this->setDisciplinaEliminaId(-1);
   this->geral = false;
}

Equivalencia::Equivalencia( int id, int cursou, int elimina, bool geral )
{
   this->setId( id );
   this->setDisciplinaCursouId(cursou);
   this->setDisciplinaEliminaId(elimina);
   this->setGeral(geral);
}

Equivalencia::~Equivalencia( void )
{
}

void Equivalencia::le_arvore( ItemEquivalencia & elem )
{
	this->setId( elem.equivId() );
	this->setDisciplinaCursouId( elem.disciplinaCursouId() );
	this->setDisciplinaEliminaId( elem.disciplinaEliminaId() );
    this->setGeral( elem.geral() );
}

std::ostream & operator << (
   std::ostream & out, Equivalencia & equiv )
{
   out << "<Equivalencia>" << std::endl;
      
   out << "<id>" << equiv.getId()
       << "</id>" << std::endl;
      
   out << "<disciplinaCursouId>" << equiv.getDisciplinaCursouId()
       << "</disciplinaCursouId>" << std::endl;

   out << "<disciplinaEliminaId>" << equiv.getDisciplinaEliminaId()
       << "</disciplinaEliminaId>" << std::endl;

   out << "<geral>" << equiv.getGeral()
       << "</geral>" << std::endl;

   return out;
}

