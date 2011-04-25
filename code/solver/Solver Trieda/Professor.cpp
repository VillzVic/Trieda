#include "Professor.h"

//Professor::Professor()
Professor::Professor(bool eVirtual)
{
	cpf = nome = "";
	tipo_contrato_id = 0;
	ch_min = 0;
	ch_max = 0;
	ch_anterior = 0;
	titulacao_id = 0;
	area_id = 0;
	valor_credito = 0;

    tipo_contrato = NULL;
    titulacao = NULL;
    area = NULL;

	id_operacional = -1;

	// Parametro utilizado na função de
	// prioridade para o modelo operacional.
	custoDispProf = 0;

	is_virtual = eVirtual;
	if(is_virtual)
	{
       cpf = "000.000.000-00";
       nome = "PROFESSOR VIRTUAL";
       tipo_contrato_id = 1;
       ch_min = 0;
       valor_credito = 50000;

	   // 7 dias na semana, 4 creditos por dia
       ch_max = 28; 
	}
}

Professor::~Professor(void)
{

}

void Professor::le_arvore(ItemProfessor & elem)
{
	this->setId( elem.id() );

	if ( elem.areaTitulacaoId().present() )
	{
		area_id = elem.areaTitulacaoId().get();
	}

	tipo_contrato_id = elem.tipoContratoId();
	titulacao_id = elem.titulacaoId();

	cpf = elem.cpf();
	nome = elem.nome();
	ch_min = elem.chMin();
	ch_max = elem.chMax();
	ch_anterior = elem.credAnterior();
	valor_credito = elem.valorCred();

	ITERA_SEQ( it_mag, elem.disciplinas(),
			   ProfessorDisciplina )
	{
		Magisterio * m = new Magisterio();
		m->le_arvore( *it_mag );
		magisterio.add( m );
	}

	ITERA_SEQ( it_h, elem.horariosDisponiveis(),
			   Horario )
	{
		Horario * h = new Horario();
		h->le_arvore( *it_h );
		horarios.add( h );
	}
}
