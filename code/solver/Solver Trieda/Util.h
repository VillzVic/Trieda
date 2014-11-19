#ifndef _UTIL_H_
#define _UTIL_H_

class HorarioAula;
class DateTime;
class Aula;

// Get fim1 and inicio2
void getFim1Inicio2(HorarioAula *const h1, int nCreds1, HorarioAula *const h2, int nCreds2, DateTime &fim1, DateTime &inicio2);

// Returns the minutes between dti and dt2
int minutosIntervalo( DateTime dt1, DateTime dt2 );

// Returns if the lessons with the specified starting times overlap each other
bool sobrepoem(Aula * const aula1, HorarioAula* const h1, Aula * const aula2, HorarioAula* const h2);


#endif