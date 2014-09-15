#include "AtendimentoHorarioAula.h"
#include "AtendimentoOferta.h"
#include "Demanda.h"
#include "InputMethods.h"
#include "CentroDados.h"
#include "HeurNuno\ParametrosHeuristica.h"

AtendimentoHorarioAula::AtendimentoHorarioAula( int id )
{
   this->setId( id );
   this->atendimentos_ofertas = new GGroup< AtendimentoOferta*, LessPtr<AtendimentoOferta> >();
   _profVirtual = false;
}

AtendimentoHorarioAula::AtendimentoHorarioAula( void )
{
	this->setId( InputMethods::fakeId );
   this->atendimentos_ofertas = new GGroup< AtendimentoOferta*, LessPtr<AtendimentoOferta> >();
   _profVirtual = false;
}


AtendimentoHorarioAula::~AtendimentoHorarioAula( void )
{
   this->setId( -1 );

   if ( atendimentos_ofertas != NULL )
   {
      atendimentos_ofertas->deleteElements();
      delete atendimentos_ofertas;
   }
}

// procura o atendimento horario aula com um determinado id. se não encontra cria um novo e retorna-o
AtendimentoOferta* AtendimentoHorarioAula::getAddAtendOferta (int id_oferta)
{
	auto itOft = atendimentos_ofertas->begin();
	for(; itOft != atendimentos_ofertas->end(); ++itOft)
	{
		if(itOft->getId() == id_oferta)
			return *itOft;
	}

	// criar atendimento oferta com o id da demanda
	AtendimentoOferta* atendOft = new AtendimentoOferta(id_oferta);
	atendimentos_ofertas->add(atendOft);

	return atendOft;
}

std::ostream& operator << ( std::ostream & out, AtendimentoHorarioAula & horario_aula )
{
   out << "<AtendimentoHorarioAula>" << std::endl;

   out << "<horarioAulaId>" << horario_aula.getHorarioAulaId() << "</horarioAulaId>" << std::endl;
   out << "<professorId>" << horario_aula.getProfessorId() << "</professorId>" << std::endl;
   out << "<virtual>" << ( ( horario_aula.profVirtual() ) ? "true" : "false" ) << "</virtual>" << std::endl;
   out << "<creditoTeorico>" << horario_aula.getCreditoTeorico() << "</creditoTeorico>" << std::endl;

   out << "<atendimentosOfertas>" << std::endl;

   auto it_oferta = horario_aula.atendimentos_ofertas->begin();
   for (; it_oferta != horario_aula.atendimentos_ofertas->end(); it_oferta++ )
   {
      out << "<AtendimentoOferta>" << std::endl;
      out << ( **it_oferta );
      out << "</AtendimentoOferta>" << std::endl;
   }
   out << "</atendimentosOfertas>" << std::endl;

   out << "</AtendimentoHorarioAula>" << std::endl;

   return out;
}

std::istream & operator >> ( std::istream &file, AtendimentoHorarioAula* const &ptrAtendHoraAula )
{
	std::string line;
	while( !getline( file, line ).eof() && (line.find("</AtendimentoHorarioAula>") == string::npos))
	{
		// HORÁRIO AULA
		// --------------------------------------------------------------------------
		if(line.find("<horarioAulaId>") != string::npos)
		{
			int horarioId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<horarioAulaId>", horarioId);
			if(horarioId != InputMethods::fakeId)
			{
				ptrAtendHoraAula->setHorarioAulaId(horarioId);
				ptrAtendHoraAula->setId(horarioId);
			}
			else // não veio com id de horario aula. abortar!
				InputMethods::excCarregarCampo("AtendimentoHorarioAula", "<horarioAulaId>", line);

			// get pointer para o objecto do horario aula
			HorarioAula* const horario = CentroDados::getHorarioAula(horarioId);
			if(horario == nullptr)
			{
				cout << "HorarioAula id: " << horarioId << endl;
				throw "[EXC: AtendimentoHorarioAula::operator>>] Nenhum horário aula encontrado com este ID!";
			}
			// set horario aula
			ptrAtendHoraAula->horario_aula = horario;

			continue;
		}

		// PROFESSOR
		// --------------------------------------------------------------------------
		if(line.find("<professorId>") != string::npos)
		{
			int profId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<professorId>", profId);
			if(profId != InputMethods::fakeId)
				ptrAtendHoraAula->setProfessorId(profId);
			else
				InputMethods::excCarregarCampo("AtendimentoHorarioAula", "<professorId>", line);

			Professor* const professor = CentroDados::getProfessor(profId);
			if(professor == nullptr)
			{
				// IMPLEMENTAR EXCEPÇÃO DEPOIS DE CRIAÇÃO DE VIRTUAIS COM BASE NO OUTPUT.
				// para já, se não encontrar considerar virtual único
				ptrAtendHoraAula->professor = ParametrosHeuristica::professorVirtual;
				ptrAtendHoraAula->setProfVirtual(true);
				continue;
			}

			ptrAtendHoraAula->professor = professor;
			ptrAtendHoraAula->setProfVirtual(professor->eVirtual());
		}

		// CRÉDITO TEÓRICO
		// --------------------------------------------------------------------------
		if(line.find("<creditoTeorico>") != string::npos)
		{
			bool credTeorico = true;
			InputMethods::getInlineAttrBool(line, "<creditoTeorico>", credTeorico);
			ptrAtendHoraAula->setCreditoTeorico(credTeorico);
		}

		// ATENDIMENTOS OFERTA
		// --------------------------------------------------------------------------
		if(line.find("<AtendimentoOferta>") != string::npos)
		{
			AtendimentoOferta* const atendOferta = new AtendimentoOferta();
			file >> atendOferta;
			if(atendOferta->getId() == InputMethods::fakeId)
				throw "[EXC: AtendimentoHorarioAula::operator>>] AtendimentoOferta*  nao carregado com sucesso!";
			ptrAtendHoraAula->atendimentos_ofertas->add(atendOferta);

			continue;
		}
	}
	return file;
}
