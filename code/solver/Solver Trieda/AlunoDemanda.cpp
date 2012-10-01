#include "AlunoDemanda.h"

AlunoDemanda::AlunoDemanda( void )
{
   this->setId( -1 );
   this->demanda = NULL;
   this->demandaId = -1;
   this->alunoId = -1;
   this->prioridade = -1;
}

AlunoDemanda::AlunoDemanda( int id, int alunoId, int prior, Demanda* demanda )
{
   this->setId( id );
   this->demanda = demanda;
   this->demandaId = demanda->getId();
   this->alunoId = alunoId;
   this->prioridade = prior;
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
   this->setPrioridade( elem.prioridade() );
}

std::ostream & operator << (
   std::ostream & out, AlunoDemanda & alunoDemanda )
{
   out << "<AlunoDemanda>" << std::endl;

   out << "<id>" << alunoDemanda.getId()
       << "</id>" << std::endl;

   out << "<alunoId>" << alunoDemanda.getAlunoId()
       << "</alunoId>" << std::endl;

   out << "<demandaId>" << alunoDemanda.demanda->getId()
	    << "</demandaId>" << std::endl;

   out << "<prioridade>" << alunoDemanda.getPrioridade()
	    << "</prioridade>" << std::endl;

   out << "</AlunoDemanda>" << std::endl;

   return out;
}