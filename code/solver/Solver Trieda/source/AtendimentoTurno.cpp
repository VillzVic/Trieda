#include "AtendimentoTurno.h"
#include "AtendimentoHorarioAula.h"
#include "InputMethods.h"
#include "CentroDados.h"

AtendimentoTurno::AtendimentoTurno( int id )
{
   this->setId( id );
   atendimentos_horarios_aula = new GGroup< AtendimentoHorarioAula*, LessPtr<AtendimentoHorarioAula> >();
}

AtendimentoTurno::AtendimentoTurno( void )
{
	this->setId( InputMethods::fakeId );
   atendimentos_horarios_aula = new GGroup< AtendimentoHorarioAula*, LessPtr<AtendimentoHorarioAula> >();
}

AtendimentoTurno::~AtendimentoTurno( void )
{
   this->setId( -1 );

   if( atendimentos_horarios_aula != NULL )
   {
      atendimentos_horarios_aula->deleteElements();
      delete atendimentos_horarios_aula;
   }
}

// procura o atendimento horario aula com um determinado id. se não encontra cria um novo e retorna-o
AtendimentoHorarioAula* AtendimentoTurno::getAddAtendHorarioAula (int id)
{
	for(auto itHor = atendimentos_horarios_aula->begin(); itHor != atendimentos_horarios_aula->end(); ++itHor)
	{
		if(itHor->getId() == id)
			return (*itHor);
	}

	AtendimentoHorarioAula* atendHor = new AtendimentoHorarioAula(id);
	atendHor->setHorarioAulaId(id);
	atendimentos_horarios_aula->add(atendHor);

	return atendHor;
}

std::ostream& operator << (std::ostream& out, AtendimentoTurno& turno)
{
   out << "<AtendimentoTurno>" << std::endl;

   out << "<turnoId>" << turno.getTurnoId() << "</turnoId>" << std::endl;

   out << "<atendimentosHorariosAula>" << std::endl;

   auto it_horario_aula = turno.atendimentos_horarios_aula->begin();
   for(; it_horario_aula != turno.atendimentos_horarios_aula->end();
	     it_horario_aula++ )
   {
	   out << ( **it_horario_aula );
   }
   out << "</atendimentosHorariosAula>" << std::endl;

   out << "</AtendimentoTurno>" << std::endl;

   return out;
}

std::istream & operator >> ( std::istream &file, AtendimentoTurno* const &ptrAtendTurno )
{
	std::string line;
	while( !getline( file, line ).eof() && (line.find("</AtendimentoTurno>") == string::npos))
	{
		// TURNO
		// --------------------------------------------------------------------------
		if(line.find("<turnoId>") != string::npos)
		{
			int turnoId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<turnoId>", turnoId);
			if(turnoId != InputMethods::fakeId)
			{
				ptrAtendTurno->setTurnoId(turnoId);
				ptrAtendTurno->setId(turnoId);
			}
			else // não veio com id de campus. abortar!
				InputMethods::excCarregarCampo("AtendimentoTurno", "<turnoId>", line);

			// get pointer para o objecto do campus
			TurnoIES* const turno = CentroDados::getTurno(turnoId);
			if(turno == nullptr)
			{
				cout << "Turno id: " << turnoId << endl;
				throw "[EXC: AtendimentoTurno::operator>>] Nenhum turno encontrada com este ID!";
			}
			// set turno
			ptrAtendTurno->turno = turno;

			continue;
		}

		// ATENDIMENTO HORÁRIO AULA
		// --------------------------------------------------------------------------
		if(line.find("<AtendimentoHorarioAula>") != string::npos)
		{
			AtendimentoHorarioAula* const atendHoraAula = new AtendimentoHorarioAula();
			file >> atendHoraAula;
			if(atendHoraAula->getId() == InputMethods::fakeId)
				throw "[EXC: AtendimentoTurno::operator>>] AtendimentoHorarioAula*  nao carregado com sucesso!";

			ptrAtendTurno->atendimentos_horarios_aula->add(atendHoraAula);

			continue;
		}
	}
	return file;
}