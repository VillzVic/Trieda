#include "Professor.h"

Professor::Professor( bool eVirtual )
{
   this->cpf = nome = "";
	this->tipo_contrato_id = 0;
	this->ch_min = 0;
	this->ch_max = 0;
	this->ch_anterior = 0;
	this->titulacao_id = 0;
	this->area_id = 0;
	this->valor_credito = 0;

   this->tipo_contrato = NULL;
   this->titulacao = NULL;
   this->area = NULL;

	this->id_operacional = -1;

	// Parametro utilizado na função de
	// prioridade para o modelo operacional.
	this->custoDispProf = 0;

	this->is_virtual = eVirtual;
	if ( this->is_virtual )
	{
      this->cpf = "000.000.000-00";
      this->nome = "PROFESSOR VIRTUAL";
      this->tipo_contrato_id = 1;
      this->ch_min = 0;
      this->valor_credito = 50000;
      this->titulacao_id = 1;

	   // 7 dias na semana, 4 creditos por dia
      this->ch_max = 28; 
	}
}

Professor::~Professor( void )
{

}

void Professor::le_arvore( ItemProfessor & elem )
{
	this->setId( elem.id() );

	if ( elem.areaTitulacaoId().present() )
	{
		this->area_id = elem.areaTitulacaoId().get();
	}

	this->tipo_contrato_id = elem.tipoContratoId();
	this->titulacao_id = elem.titulacaoId();

	this->cpf = elem.cpf();
	this->nome = elem.nome();
	this->ch_min = elem.chMin();
	this->ch_max = elem.chMax();
	this->ch_anterior = elem.credAnterior();
	this->valor_credito = elem.valorCred();

	int id_magisterio = 1;

	ITERA_SEQ( it_mag, elem.disciplinas(), ProfessorDisciplina )
	{
		Magisterio * m = new Magisterio();

		m->setId( id_magisterio );
		id_magisterio++;

		m->le_arvore( *it_mag );
		this->magisterio.add( m );
	}

	ITERA_SEQ( it_h, elem.horariosDisponiveis(), Horario )
	{
		Horario * h = new Horario();
		h->le_arvore( *it_h );
		this->horarios.add( h );
	}
}


HorarioAula* Professor::getPrimeiroHorarioDisponivelDia( int dia )
{
	HorarioAula *hi;
	bool ACHEI = false;

	// Acha um horario inicial qualquer no dia
	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			hi = h->getHorarioAula();
			ACHEI = true;
			break;
		}
	}

	if ( !ACHEI )
	{
		std::cout<<"\nATENCAO em getPrimeiroHorarioDisponivelDia: nenhum horario disponivel no dia foi encontrado.\n";
		return NULL;
	}

	// Seleciona o menor horario existente no dia
	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			if ( h->getHorarioAula()->getInicio() < hi->getInicio() )
				hi = h->getHorarioAula();
		}
	}

	return hi;
}

HorarioAula* Professor::getUltimoHorarioDisponivelDia( int dia )
{
	HorarioAula *hf;
	bool ACHEI = false;

	// Acha um horario inicial qualquer no dia
	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			hf = h->getHorarioAula();
			ACHEI = true;
			break;
		}
	}

	if ( !ACHEI )
	{
		std::cout<<"\nATENCAO em getUltimoHorarioDisponivelDia: nenhum horario disponivel no dia foi encontrado.\n";
		return NULL;
	}

	// Seleciona o maior horario existente no dia
	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			if ( h->getHorarioAula()->getInicio() > hf->getInicio() )
				hf = h->getHorarioAula();
		}
	}

	return hf;
}
