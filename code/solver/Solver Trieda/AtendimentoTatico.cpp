#include "AtendimentoTatico.h"
#include "InputMethods.h"
#include "CentroDados.h"

AtendimentoTatico::AtendimentoTatico( int idAtTatico, int idAtOferta )
{
   this->setId( idAtTatico );
   this->atendimento_oferta = new AtendimentoOferta( idAtOferta );

   this->qtde_creditos_teoricos = 0;
   this->qtde_creditos_praticos = 0;
}

AtendimentoTatico::AtendimentoTatico( void )
{
	this->setId( InputMethods::fakeId );
   this->atendimento_oferta = new AtendimentoOferta( InputMethods::fakeId );

   this->qtde_creditos_teoricos = 0;
   this->qtde_creditos_praticos = 0;
}

AtendimentoTatico::~AtendimentoTatico( void )
{
   this->setId( -1 );

   if ( atendimento_oferta != NULL )
   {
       delete atendimento_oferta;
   }
}

std::ostream & operator << ( std::ostream& out, AtendimentoTatico & tatico )
{
   out << "<AtendimentoTatico>" << std::endl;

   if ( tatico.atendimento_oferta )
   {
      out << "<atendimentoOferta>" << std::endl
		    << ( *tatico.atendimento_oferta )
		    << "</atendimentoOferta>" << std::endl;
   }
   else
   {
      out << "<atendimentoOferta />" << std::endl;
   }

   out << "<qtdeCreditosTeoricos>" << tatico.getQtdCreditosTeoricos()
	    << "</qtdeCreditosTeoricos>" << std::endl;

   out << "<qtdeCreditosPraticos>" << tatico.getQtdCreditosPraticos()
	    << "</qtdeCreditosPraticos>" << std::endl;
   
   GGroup<int> hors = tatico.getHorariosAula();   

   if ( hors.size() > 0 )
   {
	   out << "<horariosAula>" << std::endl;  
	   ITERA_GGROUP_N_PT( it, hors, int )
	   {
		   out << "<horarioAulaId>" << *it << "</horarioAulaId>" << std::endl;
	   }
	   out << "</horariosAula>" << std::endl;
   }

   out << "</AtendimentoTatico>" << std::endl;

   return out;
}

std::istream& operator >> ( std::istream &file, AtendimentoTatico* const &ptrAtendTatico )
{
	std::string line;
	while( !getline( file, line ).eof() && (line.find("</AtendimentoTatico>") == string::npos))
	{
		// ATENDIMENTOS OFERTA
		// --------------------------------------------------------------------------
		if(line.find("<AtendimentoOferta>") != string::npos)
		{
			AtendimentoOferta* const atendOferta = new AtendimentoOferta();
			file >> atendOferta;
			if(atendOferta->getId() == InputMethods::fakeId)
				throw "[EXC: AtendimentoTatico::operator>>] AtendimentoOferta*  nao carregado com sucesso!";

			ptrAtendTatico->atendimento_oferta = atendOferta;

			continue;
		}

		// CREDITOS TEORICOS
		// --------------------------------------------------------------------------
		if(line.find("<qtdeCreditosTeoricos>") != string::npos)
		{
			int credsTeoricos = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<qtdeCreditosTeoricos>", credsTeoricos);
			if(credsTeoricos != InputMethods::fakeId)
				ptrAtendTatico->setQtdCreditosTeoricos(credsTeoricos);
			else
				ptrAtendTatico->setQtdCreditosTeoricos(0);

			continue;
		}

		// CREDITOS PRATICOS
		// --------------------------------------------------------------------------
		if(line.find("<qtdeCreditosPraticos>") != string::npos)
		{
			int credsPraticos = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<qtdeCreditosPraticos>", credsPraticos);
			if(credsPraticos != InputMethods::fakeId)
				ptrAtendTatico->setQtdCreditosPraticos(credsPraticos);
			else
				ptrAtendTatico->setQtdCreditosPraticos(0);

			continue;
		}

		// HORARIOS AULA
		// --------------------------------------------------------------------------
		if(line.find("<horarioAulaId>") != string::npos)
		{
			int horarioId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<horarioAulaId>", horarioId);
			if(horarioId != InputMethods::fakeId)
				ptrAtendTatico->addHorarioAula(horarioId);

			continue;
		}
	}
	return file;
}