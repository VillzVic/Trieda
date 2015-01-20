#include "AlunoDemanda.h"
#include "TurnoIES.h"
#include "Calendario.h"

AlunoDemanda::AlunoDemanda( void )
{
   this->setId( -1 );
   this->demanda = NULL;
   this->demandaId = -1;
   this->alunoId = -1;
   this->prioridade = -1;
   this->demandaOriginal = NULL;
   this->exigeEquivalenciaForcada = false;
   this->alDemParTeorPrat = nullptr;
}

AlunoDemanda::AlunoDemanda( int id, int alunoId, int prior, Demanda* demanda, bool equivalenciaForcada )
{
   this->setId( id );
   this->demanda = demanda;
   this->demandaId = demanda->getId();
   this->alunoId = alunoId;
   this->prioridade = prior;
   this->demandaOriginal = NULL;
   this->exigeEquivalenciaForcada = equivalenciaForcada;
   this->alDemParTeorPrat = nullptr;
}


AlunoDemanda::~AlunoDemanda( void )
{
   this->setId( -1 );
   this->demanda = NULL;
   this->demandaId = -1;
   this->alunoId = -1;
   this->demandaOriginal = NULL;
   this->exigeEquivalenciaForcada = false;
   this->alDemParTeorPrat = nullptr;
}

void AlunoDemanda::le_arvore( ItemAlunoDemanda & elem )
{
   this->setId( elem.id() );
   this->setAlunoId( elem.alunoId() );
   this->setDemandaId( elem.demandaId() );
   this->setPrioridade( elem.prioridade() );
   this->setExigeEquivalenciaForcada( elem.exigeEquivalenciaForcada() );
}

bool AlunoDemanda::podeNoHorario(HorarioAula* const h, int dia) const
{ 
	TurnoIES* const turnoAlDem = demanda->getTurnoIES();
	Calendario* const calendAlDem = demanda->getCalendario();

	return turnoAlDem->possuiHorarioDiaOuCorrespondente(calendAlDem, h->getInicio(), dia);
}

std::ostream & operator << (
   std::ostream & out, AlunoDemanda & alunoDemanda )
{
   out << "<AlunoDemanda>" << std::endl;

   out << "<id>" << alunoDemanda.getId()
       << "</id>" << std::endl;

   out << "<alunoId>" << alunoDemanda.getAlunoId()
       << "</alunoId>" << std::endl;

   out << "<demandaId>" << alunoDemanda.demanda->getId()
	    << "</demandaId>" << std::endl;

   if ( alunoDemanda.demandaOriginal != NULL )
   {
	   out << "<demandaOriginalId>" << alunoDemanda.demandaOriginal->getId()
			<< "</demandaOriginalId>" << std::endl;
   }

   out << "<prioridade>" << alunoDemanda.getPrioridade()
	    << "</prioridade>" << std::endl;

   out << "<exigeEquivalenciaForcada>" << ( alunoDemanda.getExigeEquivalenciaForcada() ? "true" : "false" )
	    << "</exigeEquivalenciaForcada>" << std::endl;

   out << "</AlunoDemanda>" << std::endl;

   return out;
}