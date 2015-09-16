#ifndef _UTILIDADE_
#define _UTILIDADE_

#include <string>

class HorarioAula;
class DateTime;
class Aula;

class Utilidade
{
public:
	Utilidade();
	~Utilidade(void);

	// classe abstrata
	virtual void foo(void) = 0;

	static std::string getTimeStr(int timeSec);
	
	// Get fim1 and inicio2
	static void getFim1Inicio2(HorarioAula *const h1, int nCreds1, HorarioAula *const h2, int nCreds2, DateTime &fim1, DateTime &inicio2);

	// Returns the minutes between dti and dt2
	static int minutosIntervalo( DateTime dt1, DateTime dt2 );

	// Returns if the lessons with the specified starting times overlap each other
	static bool sobrepoem(Aula * const aula1, HorarioAula* const h1, Aula * const aula2, HorarioAula* const h2);
	static bool sobrepoem(int nCreds1, HorarioAula* const h1, int nCreds2, HorarioAula* const h2);
	
	static bool stringContem(std::string str, std::string contem);

};

#endif
