#include "AlunoDemanda.h"

AlunoDemanda::AlunoDemanda( void )
{
   this->setId( -1 );
   this->demanda = NULL;
   this->demandaId = -1;
}

AlunoDemanda::~AlunoDemanda( void )
{
   this->setId( -1 );
   this->demanda = NULL;
   this->demandaId = -1;
}

void AlunoDemanda::le_arvore( ItemAlunoDemanda & elem )
{
   this->setId( elem.alunoId() );
   this->setDemandaId( elem.demandaId() );
   this->setNomeAluno( elem.nomeAluno() );
}

std::ostream & operator << ( std::ostream & out, AlunoDemanda & alunoDemanda )
{
   out << "<AlunoDemanda>" << std::endl;

   out << "<alunoId>" << alunoDemanda.getId()
       << "</alunoId>" << std::endl;

   out << "<nomeAluno>" << alunoDemanda.getNomeAluno()
	    << "</nomeAluno>" << std::endl;

   out << "<demandaId>" << alunoDemanda.demanda->getId()
	    << "</demandaId>" << std::endl;

   out << "</AlunoDemanda>" << std::endl;

   return out;
}