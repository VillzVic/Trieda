#include "AlocacaoProfVirtual.h"

AlocacaoProfVirtual::AlocacaoProfVirtual()
{
	this->setDisciplinaId( -1 );
	this->setCampusId( -1 );
	this->setEhPratica( false );
	this->setTurma( -1 );
}

AlocacaoProfVirtual::AlocacaoProfVirtual( int discId, int turmaNr, int cpId, bool ehPrat )
{
	this->setDisciplinaId( discId );
	this->setCampusId( cpId );
	this->setEhPratica( ehPrat );
	this->setTurma( turmaNr );
}

AlocacaoProfVirtual::~AlocacaoProfVirtual( void )
{
}

void AlocacaoProfVirtual::addDescricaoDoMotivo( int profRealId, std::string descricao )
{
	this->motivosDeUso[profRealId].push_back( descricao );
}

void AlocacaoProfVirtual::addDicaEliminacao( int profRealId, std::string dica )
{
	this->dicasDeEliminacao[profRealId].push_back( dica );
}

bool AlocacaoProfVirtual::operator < ( AlocacaoProfVirtual const &right ) const
{
	if ( this->getDisciplinaId() < right.getDisciplinaId() )
	{
		return true;
	}
	else if ( this->getDisciplinaId() > right.getDisciplinaId() )
	{
		return false;
	}

	if ( this->getTurma() < right.getTurma() )
	{
		return true;
	}
	else if ( this->getTurma() > right.getTurma() )
	{
		return false;
	}
	
	if ( this->getCampusId() < right.getCampusId() )
	{
		return true;
	}
	else if ( this->getCampusId() > right.getCampusId() )
	{
		return false;
	}
	
	if ( !this->ehPratica() && right.ehPratica() )
	{
		return true;
	}
	else if ( this->ehPratica() && !right.ehPratica() )
	{
		return false;
	}
	
	return false;
}

bool AlocacaoProfVirtual::operator == ( AlocacaoProfVirtual const &right ) const
{
	if ( this->getDisciplinaId() == right.getDisciplinaId() &&
		 this->getTurma() == right.getTurma() &&
		 this->getCampusId() == right.getCampusId() &&
		 this->ehPratica() == right.ehPratica() )
	{
		return true;
	}
	return false;
}

bool AlocacaoProfVirtual::operator != ( AlocacaoProfVirtual const &right ) const
{
	if ( *this == right )
		return false;
	return true;
}

std::ostream & operator << ( std::ostream & out, AlocacaoProfVirtual & alocacaoPV )
{
   out << "<Alocacao>" << std::endl;
   out << "<turma>" << alocacaoPV.getTurma() << "</turma>" << std::endl;
   out << "<disciplinaId>" << alocacaoPV.getDisciplinaId() << "</disciplinaId>" << std::endl;
   out << "<campusId>" << alocacaoPV.getCampusId() << "</campusId>" << std::endl;
   out << "<pratica>" << ( alocacaoPV.ehPratica() ? "true" : "false" ) << "</pratica>" << std::endl;
   


   out << "<motivosDeUso>" << std::endl;
   std::map< int /*Prof-Real-Id*/, std::vector<std::string> >::iterator
	   itProfReal = alocacaoPV.motivosDeUso.begin();
   for ( ; itProfReal != alocacaoPV.motivosDeUso.end(); itProfReal++ )
   {
	   out << "<motivo>" << std::endl;
	   if ( itProfReal->first != -1 )
	   {
		   out << "<profRealId>" << itProfReal->first << "</profRealId>" << std::endl;
	   }

	   out << "<descricoes>" << std::endl;
	   for ( int i = 0; i < itProfReal->second.size(); i++ )
	   {
		   out << "<descricao>";
		   out << itProfReal->second[i].c_str();
		   out << "</descricao>" << std::endl;
	   }
	   out << "</descricoes>" << std::endl;
	   out << "</motivo>" << std::endl;
   }
   out << "</motivosDeUso>" << std::endl;



   out << "<dicasEliminacao>" << std::endl;
   itProfReal = alocacaoPV.dicasDeEliminacao.begin();
   for ( ; itProfReal != alocacaoPV.dicasDeEliminacao.end(); itProfReal++ )
   {
	   out << "<dicaEliminacao>" << std::endl;
	   out << "<profRealId>" << itProfReal->first << "</profRealId>" << std::endl;
	   out << "<alteracoesNecessarias>" << std::endl;
	   for ( int i = 0; i < itProfReal->second.size(); i++ )
	   {
		   out << "<alteracao>";
		   out << itProfReal->second[i].c_str();
		   out << "</alteracao>" << std::endl;
	   }
	   out << "</alteracoesNecessarias>" << std::endl;
	   out << "</dicaEliminacao>" << std::endl;
   }
   out << "</dicasEliminacao>" << std::endl;


   
   out << "</Alocacao>" << std::endl;

   return out;
}
