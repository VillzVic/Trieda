#include "ProfessorVirtualOutput.h"
#include "AlocacaoProfVirtual.h"
#include "InputMethods.h"
#include "CentroDados.h"

ProfessorVirtualOutput::ProfessorVirtualOutput( int valueId )
{
	id = valueId;
	ch_min = -1;
	ch_max = -1;
	titulacao_id = -1;
	area_titulacao_id = -1;
	curso_id = -1;
	contrato_id = -1;
}

ProfessorVirtualOutput::ProfessorVirtualOutput()
{
	id = -99999;
	ch_min = -1;
	ch_max = -1;
	titulacao_id = -1;
	area_titulacao_id = -1;
	curso_id = -1;
	contrato_id = -1;
}

ProfessorVirtualOutput::~ProfessorVirtualOutput()
{
	this->alocacoes.deleteElements();
}

AlocacaoProfVirtual* ProfessorVirtualOutput::getAlocacao( int discId, int turmaNr, int cpId, bool ehPrat )
{
	AlocacaoProfVirtual *alocacao = new AlocacaoProfVirtual( discId, turmaNr, cpId, ehPrat );

	GGroup< AlocacaoProfVirtual *, LessPtr<AlocacaoProfVirtual> >::iterator itFinder = 
		this->alocacoes.find( alocacao );
	delete alocacao;

	alocacao = nullptr;
	if ( itFinder != this->alocacoes.end() )
		alocacao = *itFinder;
	return alocacao;
}

bool ProfessorVirtualOutput::operator < ( const ProfessorVirtualOutput &right ) const
{
	if ( this->getId() < right.getId() )
		return true;
	return false;
}

bool ProfessorVirtualOutput::operator == ( const ProfessorVirtualOutput &right ) const
{
	if ( this->getId() == right.getId() )
		return true;
	return false;
}

bool ProfessorVirtualOutput::operator != ( const ProfessorVirtualOutput &right ) const
{
	if ( *this == right )
		return false;
	return true;
}



std::ostream & operator << ( std::ostream & out, ProfessorVirtualOutput & professor_virtual )
{
   out << "<ProfessorVirtual>" << std::endl;

   out << "<id>" << professor_virtual.getId() << "</id>" << std::endl;

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
   
//   out << "<cursoId>" << professor_virtual.getCursoAssociadoId()
//	    << "</cursoId>" << std::endl;

   out << "<contratoId>" << professor_virtual.getContratoId()
	   << "</contratoId>" << std::endl;

   out << "<disciplinas>" << std::endl;
   GGroup< int >::iterator it_disciplina = professor_virtual.disciplinas.begin();

   for (; it_disciplina != professor_virtual.disciplinas.end();
		    it_disciplina++ )
   {
		out << "<id>";
		out << ( *it_disciplina );
		out << "</id>" << std::endl;	   
   }
   out << "</disciplinas>" << std::endl;

   
   out << "<alocacoes>" << std::endl;
   ITERA_GGROUP_LESSPTR( it_alocacoes, professor_virtual.alocacoes, AlocacaoProfVirtual )
   {
		out << ( **it_alocacoes );
   }
   out << "</alocacoes>" << std::endl;
   

   out << "</ProfessorVirtual>" << std::endl;

   return out;
}

// NAO TERMINADO!
//std::istream & operator >> ( std::istream &file, ProfessorVirtualOutput* const &ptrProfVirtual )
//{
//	std::string line;
//	while( !getline( file, line ).eof() && (line.find("</ProfessorVirtual>") == string::npos))
//	{
//		// ID
//		// --------------------------------------------------------------------------
//		if(line.find("<id>") != string::npos)
//		{
//			int id = InputMethods::fakeId;
//			InputMethods::getInlineAttrInt(line, "<id>", id);
//			if(id != InputMethods::fakeId)
//				ptrProfVirtual->setId(id);
//			else // não veio com id. abortar!
//				InputMethods::excCarregarCampo("ProfessorVirtualOutput", "<id>", line);
//
//			continue;
//		}
//
//		// CARGA HORÁRIA MÍNIMA SEMANAL
//		// --------------------------------------------------------------------------
//		if(line.find("<chMin>") != string::npos)
//		{
//			int chMin = InputMethods::fakeId;
//			InputMethods::getInlineAttrInt(line, "<chMin>", chMin);
//			if(chMin != InputMethods::fakeId)
//				ptrProfVirtual->setChMin(chMin);
//			else // default
//				ptrProfVirtual->setChMin(0);
//
//			continue;
//		}
//
//		// CARGA HORÁRIA MÁXIMA SEMANAL
//		// --------------------------------------------------------------------------
//		if(line.find("<chMax>") != string::npos)
//		{
//			int chMax = InputMethods::fakeId;
//			InputMethods::getInlineAttrInt(line, "<chMax>", chMax);
//			if(chMax != InputMethods::fakeId)
//				ptrProfVirtual->setChMax(chMax);
//			else // default
//				ptrProfVirtual->setChMax(1000);
//
//			continue;
//		}
//
//		// TITULAÇÃO
//		// --------------------------------------------------------------------------
//		if(line.find("<titulacaoId>") != string::npos)
//		{
//			int titId = InputMethods::fakeId;
//			InputMethods::getInlineAttrInt(line, "<titulacaoId>", titId);
//			if(titId != InputMethods::fakeId)
//				ptrProfVirtual->setTitulacaoId(titId);
//
//			continue;
//		}
//
//		// TERMINAR!!
//	}
//}
