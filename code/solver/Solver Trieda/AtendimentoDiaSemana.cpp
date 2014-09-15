#include "AtendimentoDiaSemana.h"
#include "AtendimentoTatico.h"
#include "AtendimentoTurno.h"
#include "InputMethods.h"
#include "CentroDados.h"

AtendimentoDiaSemana::AtendimentoDiaSemana( int id )
{
   this->setId( id );
   this->atendimentos_tatico = new GGroup< AtendimentoTatico*, LessPtr<AtendimentoTatico> >();
   this->atendimentos_turno = new GGroup< AtendimentoTurno*, LessPtr<AtendimentoTurno> >();
}

AtendimentoDiaSemana::AtendimentoDiaSemana( void )
{
	this->setId( InputMethods::fakeId );
   this->atendimentos_tatico = new GGroup< AtendimentoTatico*, LessPtr<AtendimentoTatico> >();
   this->atendimentos_turno = new GGroup< AtendimentoTurno*, LessPtr<AtendimentoTurno> >();
}

AtendimentoDiaSemana::~AtendimentoDiaSemana( void )
{
   this->setId( -1 );

   if ( atendimentos_tatico != NULL )
   {
      atendimentos_tatico->deleteElements();
      delete atendimentos_tatico;
   }

   if ( atendimentos_turno != NULL )
   {
      atendimentos_turno->deleteElements();
      delete atendimentos_turno;
   }
}

// procura o atendimento turno com um determinado id. se não encontra cria um novo e retorna-o
AtendimentoTurno* AtendimentoDiaSemana::getAddAtendTurno (int id_turno)
{
	auto itTurno = atendimentos_turno->begin();
	for(; itTurno != atendimentos_turno->end(); ++itTurno)
	{
		if(itTurno->getTurnoId() == id_turno)
			return *itTurno;
	}

	AtendimentoTurno* atendTurno = new AtendimentoTurno(id_turno);
	atendTurno->setTurnoId(id_turno);
	atendimentos_turno->add(atendTurno);

	return atendTurno;
}

std::ostream & operator << ( std::ostream & out, AtendimentoDiaSemana & diaSem )
{
   out << "<AtendimentoDiaSemana>" << std::endl;
   out << "<diaSemana>" << diaSem.getDiaSemana() << "</diaSemana>" << std::endl;

   if ( diaSem.atendimentos_tatico != NULL
      && diaSem.atendimentos_tatico->size() > 0 )
   {
      out << "<atendimentosTatico>" << std::endl;

      auto it_tatico  = diaSem.atendimentos_tatico->begin();
      for (; it_tatico != diaSem.atendimentos_tatico->end(); it_tatico++ )
      {
         out << ( **it_tatico );
      }

      out << "</atendimentosTatico>" << std::endl;
   }
   else if ( diaSem.atendimentos_turno != NULL
            && diaSem.atendimentos_turno->size() > 0 )
	{
      out << "<atendimentosTurnos>" << std::endl;

      auto it_turno = diaSem.atendimentos_turno->begin();
      for (; it_turno != diaSem.atendimentos_turno->end(); it_turno++ )
      {
         out << ( **it_turno );
      }

      out << "</atendimentosTurnos>" << std::endl;
	}

	out << "</AtendimentoDiaSemana>" << std::endl;

	return out;
}

std::istream& operator >> ( std::istream &file, AtendimentoDiaSemana* const &ptrAtendDiaSem)
{
	std::string line;
	while( !getline( file, line ).eof() && (line.find("</AtendimentoDiaSemana>") == string::npos))
	{
		// DIA
		// --------------------------------------------------------------------------
		if(line.find("<diaSemana>") != string::npos)
		{
			int dia = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<diaSemana>", dia);
			if(dia != InputMethods::fakeId)
			{
				ptrAtendDiaSem->setId(dia);
				ptrAtendDiaSem->setDiaSemana(dia);
			}
			else // não veio com id de campus. abortar!
				InputMethods::excCarregarCampo("AtendimentoDiaSemana", "<diaSemana>", line);

			continue;
		}

		// ATENDIMENTOS TÁTICO
		// --------------------------------------------------------------------------
		if(line.find("<AtendimentoTatico>") != string::npos)
		{
			AtendimentoTatico* const atendTatico = new AtendimentoTatico();
			file >> atendTatico;
			if(atendTatico->getId() == InputMethods::fakeId)
				throw "[EXC: AtendimentoDiaSemana::operator>>] AtendimentoTatico* nao carregado com sucesso!";
			ptrAtendDiaSem->atendimentos_tatico->add(atendTatico);

			continue;
		}

		// ATENDIMENTOS TURNO
		// --------------------------------------------------------------------------
		if(line.find("<AtendimentoTurno>") != string::npos)
		{
			AtendimentoTurno* const atendTurno = new AtendimentoTurno();
			file >> atendTurno;
			if(atendTurno->getId() == InputMethods::fakeId)
				throw "[EXC: AtendimentoDiaSemana::operator>>] AtendimentoTurno* nao carregado com sucesso!";
			ptrAtendDiaSem->atendimentos_turno->add(atendTurno);

			continue;
		}
	}
	return file;
}