#include "AtendimentoCampus.h"

AtendimentoCampus::AtendimentoCampus(void)
{
}

AtendimentoCampus::~AtendimentoCampus(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoCampus& campus)
{
	out << "<Campus>" << endl;

	//out << "<Id>" << campus.getId() << "</Id>" << endl;

	//out << "<CampusId>" << campus.campus_id << "</CampusId>" << endl;

	out << "<CampusId>" << campus.getId() << "</CampusId>" << endl;

	out << "<UnidadeSet>" << endl;

	GGroup<AtendimentoUnidade*>::GGroupIterator it_unidade = campus.atendimentos_unidades.begin();

	for(; it_unidade != campus.atendimentos_unidades.end(); it_unidade++)
	{
		out << **it_unidade;
	}

	out << "</UnidadeSet>" << endl;

	out << "</Campus>" << endl;

	return out;
}