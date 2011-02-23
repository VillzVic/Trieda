#include "AtendimentoSala.h"

AtendimentoSala::AtendimentoSala(void)
{
}

AtendimentoSala::~AtendimentoSala(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoSala& sala)
{
   out << "<AtendimentoSala>" << endl;

	//out << "<Id>" << sala.getId() << "</Id>" << endl;

	out << "<salaId>" << sala.getId() << "</salaId>" << endl;

   out << "<salaNome>" << sala.sala_id << "</salaNome>" << endl;

	out << "<atendimentosDiasSemana>" << endl;

	GGroup<AtendimentoDiaSemana*>::GGroupIterator it_diasSem = sala.atendimentos_dias_semana.begin();

	for(; it_diasSem != sala.atendimentos_dias_semana.end(); it_diasSem++)
	{
	//	if(it_diasSem->getIdSala() == sala.getId()) {
      out << **it_diasSem;
	//	}
	}

	out << "</atendimentosDiasSemana>" << endl;

   out << "</AtendimentoSala>" << endl;

	return out;
}
