#include "AtendimentoOferta.h"
#include "InputMethods.h"
#include "CentroDados.h"

int AtendimentoOferta::globalId_ = 0;

AtendimentoOferta::AtendimentoOferta( int id )
	: disciplina(nullptr)
{
   this->setId( id );
   this->oferta_curso_campi_id = "";
   this->disciplina_id = NULL;
   this->quantidade = 0;
   this->turma = 99999999;
   this->oferta = NULL;
   this->disciplina_substituta_id = NULL;
}

AtendimentoOferta::AtendimentoOferta( void )
	: disciplina(nullptr)
{
	this->setId( ++AtendimentoOferta::globalId_ );
   this->oferta_curso_campi_id = "";
   this->disciplina_id = NULL;
   this->quantidade = 0;
   this->turma = 99999999;
   this->oferta = NULL;
   this->disciplina_substituta_id = NULL;
}

AtendimentoOferta::~AtendimentoOferta( void )
{
}

std::ostream & operator << ( std::ostream & out, AtendimentoOferta & atOferta )
{
   out << "<ofertaCursoCampiId>" << atOferta.getOfertaCursoCampiId() << "</ofertaCursoCampiId>" << std::endl;
   if ( atOferta.getDisciplinaSubstitutaId() != NULL )
		out << "<disciplinaSubstitutaId>" << abs( atOferta.getDisciplinaSubstitutaId() ) << "</disciplinaSubstitutaId>" << std::endl;
   out << "<disciplinaId>" << abs( atOferta.getDisciplinaId() ) << "</disciplinaId>" << std::endl;
   out << "<quantidade>" << atOferta.getQuantidade() << "</quantidade>" << std::endl;
   out << "<turma>" << atOferta.getTurma() << "</turma>" << std::endl;
	
   out << "<alunosDemandasAtendidas>" << std::endl;
  
   ITERA_GGROUP_N_PT( it, atOferta.alunosDemandasAtendidas, int )
   {
	   out << "<id>" << *it << "</id>" << std::endl;
   }
   out << "</alunosDemandasAtendidas>" << std::endl;
   

   return out;
}

std::istream & operator >> ( std::istream &file, AtendimentoOferta* const &ptrAtendOferta )
{
	std::string line;
	while( !getline( file, line ).eof() && 
		   ((line.find("</AtendimentoOferta>") == string::npos) &&
		    (line.find("</atendimentoOferta>") == string::npos) ) )
	{
		// OFERTA CURSO CAMPI
		// --------------------------------------------------------------------------
		if(line.find("<ofertaCursoCampiId>") != string::npos)		   
		{
			int oftId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<ofertaCursoCampiId>", oftId);
			if(oftId != InputMethods::fakeId)
			{
				stringstream ss;
				ss << oftId;
				ptrAtendOferta->setOfertaCursoCampiId(ss.str());
				ptrAtendOferta->setId(oftId);
			}
			else // não veio com id correto. abortar!
				InputMethods::excCarregarCampo("AtendimentoOferta", "<ofertaCursoCampiId>", line);

			// get pointer para o objecto da oferta
			Oferta* const oferta = CentroDados::getOferta(oftId);
			if(oferta == nullptr)
			{
				cout << "Oferta id: " << oftId << endl;
				throw "[EXC: AtendimentoOferta::operator>>] Nenhuma oferta encontrada com este ID!";
			}
			// set horario aula
			ptrAtendOferta->oferta = oferta;

			continue;
		}

		// DISCIPLINA
		// --------------------------------------------------------------------------
		if(line.find("<disciplinaId>") != string::npos)
		{
			int discId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<disciplinaId>", discId);
			if(discId != InputMethods::fakeId)
				ptrAtendOferta->setDisciplinaId(discId);
			else // não veio com id correto. abortar!
				InputMethods::excCarregarCampo("AtendimentoOferta", "<disciplinaId>", line);

			if(ptrAtendOferta->disciplina == nullptr || ptrAtendOferta->disciplina == NULL)
			{
				Disciplina* const disciplina = CentroDados::getDisciplina(discId);
				if(disciplina == nullptr)
				{
					cout << "Disciplina id: " << discId << endl;
					throw "[EXC: AtendimentoOferta::operator>>] Nenhuma disciplina encontrada com este ID!";
				}
				// set disciplina
				ptrAtendOferta->disciplina = disciplina;
			}

			continue;
		}

		// DISCIPLINA SUBSTITUTA
		// --------------------------------------------------------------------------
		if(line.find("<disciplinaSubstitutaId>") != string::npos)
		{
			int discId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<disciplinaSubstitutaId>", discId);
			if(discId != InputMethods::fakeId)
				ptrAtendOferta->setDisciplinaSubstitutaId(discId);
			else // não veio com id correto. abortar!
				InputMethods::excCarregarCampo("AtendimentoOferta", "<disciplinaSubstitutaId>", line);

			Disciplina* const disciplina = CentroDados::getDisciplina(discId);
			if(disciplina == nullptr)
			{
				cout << "Disciplina id: " << discId << endl;
				throw "[EXC: AtendimentoOferta::operator>>] Nenhuma disciplina encontrada com este ID!";
			}
			// set disciplina
			ptrAtendOferta->disciplina = disciplina;

			continue;
		}

		// TURMA ID
		// --------------------------------------------------------------------------
		if(line.find("<turma>") != string::npos)
		{
			int turmaId = InputMethods::fakeId;
			InputMethods::getInlineAttrInt(line, "<turma>", turmaId);
			if(turmaId != InputMethods::fakeId)
				ptrAtendOferta->setTurma(turmaId);
			else // não veio com id correto. abortar!
				InputMethods::excCarregarCampo("AtendimentoOferta", "<turma>", line);
		}

		// ALUNOS DEMANDA ATENDIDOS
		// --------------------------------------------------------------------------
		if(line.find("<alunosDemandasAtendidas>") != string::npos)
		{
			// IDS
			while( !getline( file, line ).eof() && (line.find("</alunosDemandasAtendidas>") == string::npos))
			{
				if(line.find("<id>") != string::npos)
				{
					int alunoDemId = InputMethods::fakeId;
					InputMethods::getInlineAttrInt(line, "<id>", alunoDemId);
					if(alunoDemId != InputMethods::fakeId)
						ptrAtendOferta->alunosDemandasAtendidas.add(alunoDemId);
					else // não veio com id correto. abortar!
						InputMethods::excCarregarCampo("AtendimentoOferta", "<id>", line);
				}		
			}
		}
	}
	// set quantidade
	ptrAtendOferta->setQuantidade(ptrAtendOferta->alunosDemandasAtendidas.size());

	return file;
}