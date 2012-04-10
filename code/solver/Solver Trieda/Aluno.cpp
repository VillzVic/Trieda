#include "Aluno.h"

Aluno::Aluno( void )
{
   this->setAlunoId( -1 );
   this->oferta = NULL;
   this->ofertaId = -1;
}

Aluno::Aluno( int id, std::string nome, Oferta* oft )
{
   this->setAlunoId( id );
   this->setNomeAluno( nome );
   this->oferta = oft;
   this->ofertaId = oft->getId();
}

Aluno::~Aluno( void )
{
}

bool Aluno::demandaDisciplina( int idDisc )
{
	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->getDisciplinaId() == idDisc )
		{
			return true;
		}
	}
	return false;
}

std::ostream & operator << (
   std::ostream & out, Aluno & aluno )
{
   out << "<Aluno>" << std::endl;
      
   out << "<alunoId>" << aluno.getAlunoId()
       << "</alunoId>" << std::endl;

   out << "<nomeAluno>" << aluno.getNomeAluno()
	    << "</nomeAluno>" << std::endl;
   
   out << "<ofertaId>" << aluno.getOfertaId()
       << "</ofertaId>" << std::endl;

   out << "<Demandas>" << std::endl;
   ITERA_GGROUP_LESSPTR( itAlDemandas, aluno.demandas, AlunoDemanda )
   {
	   out << "<demandaId>" << itAlDemandas->demanda->getId()
			<< "</demandaId>" << std::endl;
   }  
   out << "</Demandas>" << std::endl;
      
   out << "</AlunoDemanda>" << std::endl;

   return out;
}