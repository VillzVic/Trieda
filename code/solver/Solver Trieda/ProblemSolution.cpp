#include "ProblemSolution.h"
#include "ErrorHandler.h"

ProblemSolution::ProblemSolution()
{
	folgas = new RestricaoVioladaGroup();
}

ProblemSolution::~ProblemSolution()
{
	if ( folgas != NULL )
	{
		folgas->deleteElements();
		delete folgas;
	}
}

std::ostream& operator << (std::ostream& out, ProblemSolution& solution )
{
	/**
	ToDo:
	The XML that describes the output should be written here
	**/

   //out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
   out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>" << endl;

   //out << "<TriedaOutput xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" << endl;
   out << "<TriedaOutput>" << endl;

	out << "<atendimentos>" << endl;

	GGroup<AtendimentoCampus*>::GGroupIterator it_campus = solution.atendimento_campus.begin();

	for(; it_campus != solution.atendimento_campus.end(); it_campus++)
	{
		out << **it_campus;
	}

	out << "</atendimentos>" << endl;

	// Folgas:
	out << "<restricoesVioladas>\n";
	for (RestricaoVioladaGroup::iterator it = solution.getFolgas()->begin();
	it != solution.getFolgas()->end(); 
	++it)
	out << **it;
	out << "</restricoesVioladas>\n";


	// Erros e warnings:
	out << *ErrorHandler::getInstance();

    out << "</TriedaOutput>" << endl;

	return out;
}
