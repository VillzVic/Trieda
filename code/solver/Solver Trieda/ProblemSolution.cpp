#include "ProblemSolution.h"
#include "ErrorHandler.h"

ProblemSolution::ProblemSolution()
{
   folgas = new RestricaoVioladaGroup();
   atendimento_campus = new GGroup< AtendimentoCampus * >();
}

ProblemSolution::~ProblemSolution()
{
   if ( folgas != NULL )
   {
      folgas->deleteElements();
      delete folgas;
   }

   if( atendimento_campus != NULL )
   {
      atendimento_campus->deleteElements();
      delete atendimento_campus;
   }
}

std::ostream & operator << ( std::ostream & out, ProblemSolution & solution )
{
   // TATICO
   if ( solution.solucao_operacional == NULL )
   {
      out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>" << endl;
      out << "<TriedaOutput>" << endl;

	  //-----------------------------------------------------------------------
      out << "<atendimentos>" << endl;
      GGroup< AtendimentoCampus * >::GGroupIterator it_campus
		  = solution.atendimento_campus->begin();
      for(; it_campus != solution.atendimento_campus->end(); it_campus++)
      {
         out << **it_campus;
      }
      out << "</atendimentos>" << endl;
	  //-----------------------------------------------------------------------

	  //-----------------------------------------------------------------------
      // Folgas:
      out << "<restricoesVioladas>\n";
	  RestricaoVioladaGroup::iterator it
		  = solution.getFolgas()->begin();
      for (; it != solution.getFolgas()->end();  ++it)
	  {
         out << **it;
	  }
      out << "</restricoesVioladas>\n";
	  //-----------------------------------------------------------------------

	  //-----------------------------------------------------------------------
      // Erros e warnings:
      out << *ErrorHandler::getInstance();
	  //-----------------------------------------------------------------------

      out << "</TriedaOutput>" << endl;
   }
   // OPERACIONAL
   else
   {
      out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>" << endl;
      out << "<TriedaOutput>" << endl;

	  //-----------------------------------------------------------------------
      out << "<atendimentos>" << endl;
	  if ( solution.atendimento_campus != NULL )
	  {
		  GGroup< AtendimentoCampus * >::GGroupIterator it_campus
			  = solution.atendimento_campus->begin();
		  for(; it_campus != solution.atendimento_campus->end(); it_campus++)
		  {
			 out << **it_campus;
		  }
		  out << "</atendimentos>" << endl;
	  }
	  //-----------------------------------------------------------------------

	  out << "</TriedaOutput>" << endl;
   }

   return out;
}
