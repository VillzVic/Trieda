#include "AtendimentoUnidade.h"
#include "AtendimentoSala.h"
#include "InputMethods.h"
#include "CentroDados.h"

AtendimentoUnidade::AtendimentoUnidade( int id )
{
   this->setId( id );
   atendimentos_salas = new GGroup< AtendimentoSala*, LessPtr<AtendimentoSala> >();
   unidade = NULL;
   this->setCodigoUnidade("");
}

AtendimentoUnidade::AtendimentoUnidade( void )
{
   this->setId( InputMethods::fakeId );
   atendimentos_salas = new GGroup< AtendimentoSala*, LessPtr<AtendimentoSala>  >();
   unidade = NULL;
   this->setCodigoUnidade("");
}

AtendimentoUnidade::~AtendimentoUnidade( void )
{
   this->setId( -1 );

   if ( atendimentos_salas != NULL )
   {
      atendimentos_salas->deleteElements();
      delete atendimentos_salas;
   }
}

// procura o atendimento unidade com um determinado id. se não encontra cria um novo e retorna-o
AtendimentoSala* AtendimentoUnidade::getAddAtendSala (int id)
{
	auto itSala = atendimentos_salas->begin();
	for(; itSala != atendimentos_salas->end(); ++itSala)
	{
		if(itSala->getId() == id)
			return *itSala;
	}

	AtendimentoSala* atendSala = new AtendimentoSala(id);
	atendimentos_salas->add(atendSala);

	return atendSala;
}

std::ostream&  operator << ( std::ostream & out, AtendimentoUnidade & unidade )
{
   out << "<AtendimentoUnidade>" << std::endl;
   out << "<unidadeId>" << unidade.getId() << "</unidadeId>" << std::endl;
   out << "<unidadeCodigo>" << unidade.getCodigoUnidade() << "</unidadeCodigo>" << std::endl;
   out << "<atendimentosSalas>" << std::endl;

   auto it_sala  = unidade.atendimentos_salas->begin();
   for (; it_sala != unidade.atendimentos_salas->end();
	       it_sala++ )
   {
      out << ( **it_sala );
   }

   out << "</atendimentosSalas>" << std::endl;
   out << "</AtendimentoUnidade>" << std::endl;

   return out;
}

std::istream& operator >> ( std::istream &file, AtendimentoUnidade* const &ptrAtendUnidade)
{
	std::string line;
	while( !getline( file, line ).eof() && (line.find("</AtendimentoUnidade>") == string::npos))
	{
		// UNIDADE
		// --------------------------------------------------------------------------
		if(line.find("<unidadeId>") != string::npos)
		{
			int unidId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<unidadeId>", unidId);
			if(unidId != InputMethods::fakeId)
				ptrAtendUnidade->setId(unidId);
			else // não veio com id de campus. abortar!
				InputMethods::excCarregarCampo("AtendimentoUnidade", "<unidadeId>", line);

			// get pointer para o objecto do campus
			Unidade* const unidade = CentroDados::getUnidade(unidId);
			if(unidade == nullptr)
			{
				cout << "Unidade id: " << unidId << endl;
				throw "[EXC: AtendimentoUnidade::operator>>] Nenhuma unidade encontrada com este ID!";
			}
			// set unidade
			ptrAtendUnidade->unidade = unidade;
			// não setar codigo para replicar output (teste)
			//trAtendUnidade->setCodigoUnidade(unidade->getCodigo());

			continue;
		}

		// ATENDIMENTOS SALA
		// --------------------------------------------------------------------------
		if(line.find("<AtendimentoSala>") != string::npos)
		{
			AtendimentoSala* const atendSala = new AtendimentoSala();
			file >> atendSala;
			if(atendSala->getId() == InputMethods::fakeId)
				throw "[EXC: AtendimentoUnidade::operator>>] AtendimentoSala*  nao carregado com sucesso!";
			ptrAtendUnidade->atendimentos_salas->add(atendSala);

			continue;
		}
	}
	return file;
}
