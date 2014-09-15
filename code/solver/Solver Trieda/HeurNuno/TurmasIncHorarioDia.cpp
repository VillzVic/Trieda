#include "TurmasIncHorarioDia.h"
#include "../HorarioAula.h"
#include "TurmaHeur.h"

TurmasIncHorarioDia::TurmasIncHorarioDia(int d, HorarioAula* const hor, ConjUnidades* const cluster)
	: dia(d), horarioAula(hor), clusterUnids(cluster)
{
}


TurmasIncHorarioDia::~TurmasIncHorarioDia(void)
{
}
