#include "TipoTitulacao.h"

TipoTitulacao::TipoTitulacao(void)
{
}

TipoTitulacao::TipoTitulacao( int tit )
{
	titulacao = tit;

	if( tit != Licenciado && 
		tit != Bacharel && 
		tit != Especialista &&
		tit != Mestre &&
		tit != Doutor )
	{
		titulacao = Error;
	}
}

TipoTitulacao::~TipoTitulacao(void)
{
}

void TipoTitulacao::le_arvore(ItemTipoTitulacao& elem)
{
   this->setId( elem.id() );
   nome = elem.nome();

   setTitulacao();
}

void TipoTitulacao::setTitulacao()
{
	std::string str = nome;
	
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if( strcmp( str.c_str(), "licenciado" ) == 0 )
	{
		titulacao = Licenciado;
	}
	else if( strcmp( str.c_str(), "bacharel" ) == 0 )
	{
		titulacao = Bacharel;	
	}
	else if( strcmp( str.c_str(), "especialista" ) == 0 )
	{
		titulacao = Especialista;	
	}
	else if( strcmp( str.c_str(), "mestre" ) == 0 )
	{
		titulacao = Mestre;	
	}
	else if( strcmp( str.c_str(), "doutor" ) == 0 )
	{
		titulacao = Doutor;	
	}
	else titulacao = Error;
}

void TipoTitulacao::operator = ( TipoTitulacao const & right )
{
    this->titulacao = right.getTitulacao();
	this->nome = right.nome;
}

bool TipoTitulacao::operator < ( TipoTitulacao const & right ) const
{
    return ( this->getTitulacao() < right.getTitulacao() );
}

bool TipoTitulacao::operator > ( TipoTitulacao const & right ) const
{ 
    return ( right < *this );
}

bool TipoTitulacao::operator == ( TipoTitulacao const & right ) const
{ 
	return ( !( *this < right ) && !( right < *this ) );
}