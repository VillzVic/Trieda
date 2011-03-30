#include "Turno.h"

Turno::Turno(void)
{
}

Turno::~Turno(void)
{
}

void Turno::le_arvore(ItemTurno& elem)
{
	this->setId( elem.id() );
	nome = elem.nome();
	tempo_aula = elem.tempoAula();

	ITERA_SEQ(it_horarios,elem.HorariosAula(),HorarioAula)
	{
		HorarioAula* horario = new HorarioAula();
		horario->le_arvore(*it_horarios);
		horarios_aula.add(horario);
	}
}
