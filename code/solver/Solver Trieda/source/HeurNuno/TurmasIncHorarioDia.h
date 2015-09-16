#ifndef _TURMAS_INC_HORARIO_DIA_
#define _TURMAS_INC_HORARIO_DIA_

#include <unordered_set>

using namespace std;

class HorarioAula;
class ConjUnidades;

class TurmasIncHorarioDia
{
public:
	TurmasIncHorarioDia(int d, HorarioAula* const hor, ConjUnidades* const cluster);
	~TurmasIncHorarioDia(void);

	unordered_set<int> colNrsTurmas;
	const int dia;
	HorarioAula* const horarioAula;
	ConjUnidades* const clusterUnids;
};

#endif

