#include "AtendimentoCampus.h"
#include "AtendimentoUnidade.h"
#include "InputMethods.h"
#include "CentroDados.h"

AtendimentoCampus::AtendimentoCampus( int id )
{
   this->setId( id );
   this->atendimentos_unidades = new GGroup< AtendimentoUnidade*, LessPtr<AtendimentoUnidade> >();
   this->campus = NULL;
   this->setCampusId("");
}

AtendimentoCampus::AtendimentoCampus( void )
{
	this->setId( InputMethods::fakeId );
   this->atendimentos_unidades = new GGroup< AtendimentoUnidade*, LessPtr<AtendimentoUnidade> >();
   this->campus = NULL;
   this->setCampusId("");
}

AtendimentoCampus::~AtendimentoCampus( void )
{
   this->setId( -1 );

   if ( atendimentos_unidades != NULL )
   {
      this->atendimentos_unidades->deleteElements();
      delete this->atendimentos_unidades;
   }
}

// procura o atendimento unidade com um determinado id. se não encontra cria um novo e retorna-o
AtendimentoUnidade* AtendimentoCampus::getAddAtendUnidade (int id)
{
	auto itUnid = atendimentos_unidades->begin();
	for(; itUnid != atendimentos_unidades->end(); ++itUnid)
	{
		if(itUnid->getId() == id)
			return *itUnid;
	}

	AtendimentoUnidade* atendUnid = new AtendimentoUnidade(id);
	atendimentos_unidades->add(atendUnid);

	return atendUnid;
}


std::ostream & operator << ( std::ostream & out, AtendimentoCampus & campus )
{
   out << "<AtendimentoCampus>" << std::endl;
   out << "<campusId>" << campus.getId() << "</campusId>" << std::endl;
   out << "<campusCodigo>" << campus.getCampusId() << "</campusCodigo>" << std::endl;
   out << "<atendimentosUnidades>" << std::endl;

   auto it_unidade  = campus.atendimentos_unidades->begin();
   for (; it_unidade != campus.atendimentos_unidades->end(); it_unidade++ )
   {
      out << ( **it_unidade );
   }

   out << "</atendimentosUnidades>" << std::endl;
   out << "</AtendimentoCampus>" << std::endl;

   return out;
}

std::istream& operator >> ( std::istream &file, AtendimentoCampus* const &ptrAtendCampus)
{
	std::string line;
	while( !getline( file, line ).eof() && (line.find("</AtendimentoCampus>") == string::npos))
	{
		// CAMPUS
		// --------------------------------------------------------------------------
		if(line.find("<campusId>") != string::npos)
		{
			int campusId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<campusId>", campusId);
			if(campusId != InputMethods::fakeId)
				ptrAtendCampus->setId(campusId);
			else // não veio com id de campus. abortar!
				InputMethods::excCarregarCampo("AtendimentoCampus", "<campusId>", line);

			// get pointer para o objecto do campus
			Campus* const campus = CentroDados::getCampus(campusId);
			if(campus == nullptr)
			{
				cout << "Campus id: " << campusId << endl;
				throw "[EXC: AtendimentoCampus::operator>>] Nenhum campus encontrado com este ID!";
			}
			// set campus
			ptrAtendCampus->campus = campus;
			// não setar codigo para replicar output (teste)
			//ptrAtendCampus->setCampusId(campus->getCodigo());

			continue;
		}

		// ATENDIMENTOS UNIDADE
		// --------------------------------------------------------------------------
		if(line.find("<AtendimentoUnidade>") != string::npos)
		{
			AtendimentoUnidade* const atendUnidade = new AtendimentoUnidade();
			file >> atendUnidade;
			if(atendUnidade->getId() == InputMethods::fakeId)
				throw "[EXC: AtendimentoCampus::operator>>] AtendimentoUnidade*  nao carregado com sucesso!";
			ptrAtendCampus->atendimentos_unidades->add(atendUnidade);

			continue;
		}
	}
	return file;
}