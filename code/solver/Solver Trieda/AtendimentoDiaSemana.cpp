#include "AtendimentoDiaSemana.h"

AtendimentoDiaSemana::AtendimentoDiaSemana(void)
{
	key = std::make_pair<int,int>(-1,-1);
}

AtendimentoDiaSemana::~AtendimentoDiaSemana(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoDiaSemana& diaSem)
{
   out << "<AtendimentoDiaSemana>" << endl;

	out << "<diaSemana>" << diaSem.getIdDia() << "</diaSemana>" << endl;

	if(diaSem.atendimentos_tatico.size() > 0)
	{
		out << "<atendimentosTatico>" << endl;

		GGroup<AtendimentoTatico*>::GGroupIterator it_tatico = diaSem.atendimentos_tatico.begin();

		for(; it_tatico != diaSem.atendimentos_tatico.end(); it_tatico++)
		{
			out << **it_tatico;
		}

		out << "</atendimentosTatico>" << endl;
	}
	else if (diaSem.atendimentos_turno.size() > 0)
	{
		out << "<atendimentosTurnos>" << endl;

		GGroup<AtendimentoTurno*>::GGroupIterator it_turno = diaSem.atendimentos_turno.begin();

		for(; it_turno != diaSem.atendimentos_turno.end(); it_turno++)
		{
			//out << (*it_turno) << endl;
			out << **it_turno;
		}

		out << "</atendimentosTurnos>" << endl;
	}

	out << "</AtendimentoDiaSemana>" << endl;

	return out;
}