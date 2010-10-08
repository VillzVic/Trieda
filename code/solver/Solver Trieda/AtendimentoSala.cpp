#include "AtendimentoSala.h"

AtendimentoSala::AtendimentoSala(void)
{
}

AtendimentoSala::~AtendimentoSala(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoSala& sala)
{
	out << "<Sala>" << endl;

	out << "<Id>" << sala.getId() << "</Id>" << endl;

	out << "<salaId>" << sala.sala_id.c_str() << "</salaId>" << endl;

	//out << "<salaId>" << sala.getId() << "</salaId>" << endl;

	out << "<DiasSemanaSet>" << endl;

	GGroup<AtendimentoDiaSemana*>::GGroupIterator it_diasSem = sala.atendimentos_dias_semana.begin();

	for(; it_diasSem != sala.atendimentos_dias_semana.end(); it_diasSem++)
	{
		if(it_diasSem->getIdSala() == sala.getId()) {
			out << **it_diasSem;
		}
	}

	out << "</DiasSemanaSet>" << endl;

	out << "</Sala>" << endl;

	return out;
}
