#include "ProfessorVirtualOutput.h"

ProfessorVirtualOutput::ProfessorVirtualOutput()
{
	id = -1;
	ch_min = -1;
	ch_max = -1;
	titulacao_id = -1;
	area_titulacao_id = -1;
}

ProfessorVirtualOutput::~ProfessorVirtualOutput()
{

}

std::ostream & operator << ( std::ostream & out, ProfessorVirtualOutput & professor_virtual )
{
   out << "<ProfessorVirtual>" << std::endl;

   out << "<Id>" << professor_virtual.getId()
	   << "</Id>" << std::endl;

   out << "<chMin>" << professor_virtual.getChMin()
	   << "</chMin>" << std::endl;

   out << "<chMax>" << professor_virtual.getChMax()
	   << "</chMax>" << std::endl;

   out << "<titulacaoId>" << professor_virtual.getTitulacaoId()
	   << "</titulacaoId>" << std::endl;

   if ( professor_virtual.getAreaTitulacaoId() > 0 )
   {
		out << "<areaTitulacaoId>"
			<< professor_virtual.getAreaTitulacaoId()
			<< "</areaTitulacaoId>" << std::endl;
   }

   out << "<disciplinas>" << std::endl;
   GGroup< int >::iterator it_disciplina
	   = professor_virtual.disciplinas.begin();
   for(; it_disciplina != professor_virtual.disciplinas.end();
		 it_disciplina++ )
   {
		out << "<id>";
		out << ( *it_disciplina );
		out << "</id>" << std::endl;
   }
   out << "</disciplinas>" << std::endl;

   out << "</ProfessorVirtual>" << std::endl;

   return out;
}