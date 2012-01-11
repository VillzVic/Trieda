#include "BlocoCurricular.h"

BlocoCurricular::BlocoCurricular( void )
{
   periodo = -1;
   curso = NULL;
   campus = NULL;
   total_turmas = -1;
   curriculo = NULL;
}

BlocoCurricular::~BlocoCurricular( void )
{

}

int BlocoCurricular::getMaxCredsNoDia(int dia)
{	
	std::map< int, int >::iterator it = max_creds_dia.begin();
	it = this->max_creds_dia.find( dia );

	if (it != this->max_creds_dia.end())
	{
		return( it->second );
	}

	return (0);
}

void BlocoCurricular::preencheMaxCredsPorDia()
{
	ITERA_GGROUP_N_PT( it_Dia_Letivo, this->diasLetivos, int )
    { 
		max_creds_dia[*it_Dia_Letivo] = curriculo->getMaxCreds(*it_Dia_Letivo);
    }
}