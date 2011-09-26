#include "AlunoDemanda.h"

AlunoDemanda::AlunoDemanda( void )
{
   this->setId( -1 );
   this->demanda = NULL;
   this->demandaId = -1;
   this->alunoId = -1;
}

AlunoDemanda::~AlunoDemanda( void )
{
   this->setId( -1 );
   this->demanda = NULL;
   this->demandaId = -1;
   this->alunoId = -1;
}

void AlunoDemanda::le_arvore( ItemAlunoDemanda & elem )
{
   this->setId( elem.id() );
   this->setAlunoId( elem.alunoId() );
   this->setDemandaId( elem.demandaId() );
   this->setNomeAluno( elem.nomeAluno() );
}

std::ostream & operator << (
   std::ostream & out, AlunoDemanda & alunoDemanda )
{
   out << "<AlunoDemanda>" << std::endl;

   out << "<id>" << alunoDemanda.getId()
       << "</id>" << std::endl;

   out << "<alunoId>" << alunoDemanda.getAlunoId()
       << "</alunoId>" << std::endl;

   out << "<nomeAluno>" << alunoDemanda.getNomeAluno()
	    << "</nomeAluno>" << std::endl;

   out << "<demandaId>" << alunoDemanda.demanda->getId()
	    << "</demandaId>" << std::endl;

   out << "</AlunoDemanda>" << std::endl;

   return out;
}