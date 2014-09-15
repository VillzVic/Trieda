#include "AtendimentoSala.h"
#include "AtendimentoDiaSemana.h"
#include "Sala.h"
#include "InputMethods.h"
#include "CentroDados.h"

AtendimentoSala::AtendimentoSala( int id )
{
   this->setId( id );
   this->atendimentos_dias_semana = new GGroup<AtendimentoDiaSemana*, LessPtr<AtendimentoDiaSemana> >();

   sala = NULL;
   this->setSalaId("");
}

AtendimentoSala::AtendimentoSala( void )
{
	this->setId( InputMethods::fakeId );
   this->atendimentos_dias_semana = new GGroup<AtendimentoDiaSemana*, LessPtr<AtendimentoDiaSemana> >();

   sala = NULL;
   this->setSalaId("");
}


AtendimentoSala::~AtendimentoSala(void)
{
   this->setId( -1 );

   if( atendimentos_dias_semana != NULL )
   {
      atendimentos_dias_semana->deleteElements();
      delete atendimentos_dias_semana;
   }
}

// procura o atendimento diasemana para um determinado dia. se não encontra cria um novo e retorna-o
AtendimentoDiaSemana* AtendimentoSala::getAddAtendDiaSemana (int dia)
{
	auto itDia = atendimentos_dias_semana->begin();
	for(; itDia != atendimentos_dias_semana->end(); ++itDia)
	{
		if(itDia->getDiaSemana() == dia)
			return *itDia;
	}

	AtendimentoDiaSemana* atendDia = new AtendimentoDiaSemana(dia);
	atendDia->setDiaSemana(dia);
	atendimentos_dias_semana->add(atendDia);

	return atendDia;
}

std::ostream & operator << ( std::ostream & out, AtendimentoSala & sala )
{
   out << "<AtendimentoSala>" << std::endl;

   out << "<salaId>" << sala.getId() << "</salaId>" << std::endl;
   out << "<salaNome>" << sala.getSalaId() << "</salaNome>" << std::endl;

   out << "<atendimentosDiasSemana>" << std::endl;

   auto it_diasSem = sala.atendimentos_dias_semana->begin();
   for(; it_diasSem != sala.atendimentos_dias_semana->end();
	     it_diasSem++ )
   {
      out << ( **it_diasSem );
   }

   out << "</atendimentosDiasSemana>" << std::endl;
   out << "</AtendimentoSala>" << std::endl;

   return out;
}

std::istream& operator >> ( std::istream &file, AtendimentoSala* const &ptrAtendSala)
{
	std::string line;
	while( !getline( file, line ).eof() && (line.find("</AtendimentoSala>") == string::npos))
	{
		// SALA
		// --------------------------------------------------------------------------
		if(line.find("<salaId>") != string::npos)
		{
			int salaId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<salaId>", salaId);
			if(salaId != InputMethods::fakeId)
				ptrAtendSala->setId(salaId);
			else // não veio com id de campus. abortar!
				InputMethods::excCarregarCampo("AtendimentoSala", "<salaId>", line);

			// get pointer para o objecto do campus
			Sala* const sala = CentroDados::getSala(salaId);
			if(sala == nullptr)
			{
				cout << "Sala id: " << salaId << endl;
				throw "[EXC: AtendimentoSala::operator>>] Nenhuma sala encontrada com este ID!";
			}
			// set sala
			ptrAtendSala->sala = sala;
			// não setar codigo para replicar output (teste)
			//ptrAtendSala->setSalaId(sala->getCodigo());

			continue;
		}

		// ATENDIMENTOS DIA SEMANA
		// --------------------------------------------------------------------------
		if(line.find("<AtendimentoDiaSemana>") != string::npos)
		{
			AtendimentoDiaSemana* const atendDiaSem = new AtendimentoDiaSemana();
			file >> atendDiaSem;
			if(atendDiaSem->getId() == InputMethods::fakeId)
				throw "[EXC: AtendimentoSala::operator>>] AtendimentoDiaSemana*  nao carregado com sucesso!";
			ptrAtendSala->atendimentos_dias_semana->add(atendDiaSem);

			continue;
		}
	}
	return file;
}
