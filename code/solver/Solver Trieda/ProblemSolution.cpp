#include "ProblemSolution.h"

ProblemSolution::ProblemSolution()
{
}

std::ostream& operator << (std::ostream& out, ProblemSolution& solution )
{
	/**
	ToDo:
	The XML that describes the output should be written here
	**/

	out << "<Output>" << endl;

	out << "<CampusSet>" << endl;

	GGroup<AtendimentoCampus*>::GGroupIterator it_campus = solution.atendimento_campus.begin();

	for(; it_campus != solution.atendimento_campus.end(); it_campus++)
	{
		out << **it_campus;
	}

	out << "</CampusSet>" << endl;

	out << "</Output>" << endl;

	return out;
}
