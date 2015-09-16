#include "TipoContrato.h"


TipoContrato::TipoContrato( int regime )
{
	contrato = regime;

	if( contrato != TipoContrato::Integral && 
		contrato != TipoContrato::Parcial && 
		contrato != TipoContrato::Outro )
	{
		contrato = TipoContrato::Error;
	}
}

TipoContrato::TipoContrato(void)
{
}

TipoContrato::~TipoContrato(void)
{
}

void TipoContrato::le_arvore(ItemTipoContrato& elem)
{
   this->setId( elem.id() );
   nome = elem.nome();
	
   nome.erase( remove(nome.begin(),nome.end(),' '), nome.end() );

   setContrato();
}

void TipoContrato::setContrato()
{
	std::string str = nome;
	
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	str.erase(remove(str.begin(),str.end(),' '),str.end());

	if( strcmp( str.c_str(), "tempointegral" ) == 0 )
	{
		contrato = TipoContrato::Integral;
	}
	else if( strcmp( str.c_str(), "tempoparcial" ) == 0 )
	{
		contrato = TipoContrato::Parcial;	
	}
	else
	{
		contrato = TipoContrato::Outro;	
	}

}

void TipoContrato::operator = ( TipoContrato const & right )
{
	this->contrato = right.getContrato();
	this->nome = right.nome;
}

bool TipoContrato::operator < ( TipoContrato const & right ) const
{
    if ( this->getContrato() < right.getContrato() )
		return true;
	else
		return false;
}

bool TipoContrato::operator > ( TipoContrato const & right ) const
{ 
    return ( right < *this );
}

bool TipoContrato::operator == ( TipoContrato const & right ) const
{ 
	return ( !( *this < right ) && !( right < *this ) );
}
