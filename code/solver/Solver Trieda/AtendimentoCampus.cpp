#include "AtendimentoCampus.h"

AtendimentoCampus::AtendimentoCampus(void)
{
}

AtendimentoCampus::~AtendimentoCampus(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoCampus& campus)
{
	//out << "<Id>" << campus.getId() << "</Id>" << endl;

   out << "<AtendimentoCampus>" << endl;

	out << "<campusId>" << campus.campus_id << "</campusId>" << endl;

	out << "<atendimentosUnidades>" << endl;

	GGroup<AtendimentoUnidade*>::GGroupIterator it_unidade = campus.atendimentos_unidades.begin();

	for(; it_unidade != campus.atendimentos_unidades.end(); it_unidade++)
	{
		out << **it_unidade;
	}

	out << "</atendimentosUnidades>" << endl;

  	out << "</AtendimentoCampus>" << endl;

	return out;
}