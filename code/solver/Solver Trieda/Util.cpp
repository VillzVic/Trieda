#include "Util.h"

#include "HorarioAula.h"
#include "DateTime.h"
#include "Calendario.h"
#include "Aula.h"


// Considering 2 different lessons which begin at h1 and has nCreds1 and at h2 and has nCreds2,
// returns at "fim1" the end of the chronologically first one
// and at "inicio2" the beginning of the chronologically second one.
void getFim1Inicio2(HorarioAula *const h1, int nCreds1, HorarioAula *const h2, int nCreds2, DateTime &fim1, DateTime &inicio2)
{					
	DateTime dti1 = h1->getInicio();
	DateTime dti2 = h2->getInicio();
	if (dti1 < dti2)
	{
		Calendario *c1 = h1->getCalendario();
		HorarioAula *hf1 = c1->getHorarioMaisNCreds(h1, nCreds1-1);		
		fim1 = hf1->getFinal();

		inicio2 = dti2;
	}
	else
	{
		Calendario *c2 = h2->getCalendario();
		HorarioAula *hf2 = c2->getHorarioMaisNCreds(h2, nCreds2-1);		
		fim1 = hf2->getFinal();

		inicio2 = dti1;
	}
}

// Returns the minutes between dti and dt2
int minutosIntervalo( DateTime dt1, DateTime dt2 )
{
   DateTime back = ( dt1 - dt2 );
   int minutes = ( back.getHour() * 60 + back.getMinute() );
   return minutes;
}

bool sobrepoem(Aula * const aula1, HorarioAula* const h1, Aula * const aula2, HorarioAula* const h2)
{
	int nCreds1 = aula1->getTotalCreditos();
	int nCreds2 = aula2->getTotalCreditos();					
	DateTime fim1;
	DateTime inicio2;					
	getFim1Inicio2(h1,nCreds1,h2,nCreds2,fim1,inicio2);

	if ( fim1 > inicio2 )
	{
		return true;
	}

	return false;
}