#include "AtendimentoDiaSemana.h"

AtendimentoDiaSemana::AtendimentoDiaSemana(void)
{
}

AtendimentoDiaSemana::~AtendimentoDiaSemana(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoDiaSemana& diaSem)
{
	out << "<DiaSemana>" << endl;

	//out << "<Id>" << diaSem.getId() << "</Id>" << endl;

	//out << "<diaSemana>" << diaSem.dia_semana << "</diaSemana>" << endl;

	out << "<diaSemana>" << diaSem.getId() << "</diaSemana>" << endl;

	if(diaSem.atendimentos_tatico.size() > 0)
	{
		out << "<TaticoSet>" << endl;

		GGroup<AtendimentoTatico*>::GGroupIterator it_tatico = diaSem.atendimentos_tatico.begin();

		for(; it_tatico != diaSem.atendimentos_tatico.end(); it_tatico++)
		{
			//out << (*it_tatico) << endl;
			out << **it_tatico;
		}

		out << "</TaticoSet>" << endl;
	}
	else if (diaSem.atendimentos_turno.size() > 0)
	{
		out << "<TurnoSet>" << endl;

		GGroup<AtendimentoTurno*>::GGroupIterator it_turno = diaSem.atendimentos_turno.begin();

		for(; it_turno != diaSem.atendimentos_turno.end(); it_turno++)
		{
			//out << (*it_turno) << endl;
			out << **it_turno;
		}

		out << "</TurnoSet>" << endl;
	}

	out << "</DiaSemana>" << endl;

	return out;
}