#include "AtendimentoUnidade.h"

AtendimentoUnidade::AtendimentoUnidade(void)
{
}

AtendimentoUnidade::~AtendimentoUnidade(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoUnidade& unidade)
{
	out << "<Unidade>" << endl;

	out << "<Id>" << unidade.getId() << "</Id>" << endl;

	out << "<unidadeId>" << unidade.unidade_id << "</unidadeId>" << endl;

	//out << "<unidadeId>" << unidade.getId() << "</unidadeId>" << endl;

	out << "<SalaSet>" << endl;

	GGroup<AtendimentoSala*>::GGroupIterator it_sala = unidade.atendimentos_salas.begin();

	for(; it_sala != unidade.atendimentos_salas.end(); it_sala++)
	{
		//out << (*it_sala) << endl;
		out << **it_sala;
	}

	out << "</SalaSet>" << endl;

	out << "</Unidade>" << endl;

	return out;
}