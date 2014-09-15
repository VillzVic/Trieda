#include "HorarioDia.h"
#include "HorarioAula.h"

HorarioDia::HorarioDia()
{
   dia = -1;
   id = -1;
   horarioAula = NULL;
   idHorarioAula = -1;
}

HorarioDia::~HorarioDia()
{
   horarioAula = NULL;
}

HorarioDia::HorarioDia( const HorarioDia & right )
{
   dia = right.dia;
   id = right.id;
   horarioAula = right.horarioAula;
   idHorarioAula = right.idHorarioAula;
}

bool HorarioDia::operator < ( const HorarioDia & right ) const 
{
   if ( dia != right.dia )
   {
	   return dia < right.dia;
   }

   return (*horarioAula) < (*right.horarioAula);
}

bool HorarioDia::operator == ( HorarioDia const & right ) const
{ 
	return ( (dia == right.dia) && (horarioAula->getId() == right.horarioAula->getId()) );
}


/*
	Não considera ids
*/
bool HorarioDia::igual ( const HorarioDia & right ) const 
{
	return (dia == right.dia) && horarioAula->inicioFimIguais(right.horarioAula);
}
