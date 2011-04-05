#include "Professor.h"

//Professor::Professor()
Professor::Professor(bool eVirtual)
{
	cpf = nome = "";
	tipo_contrato_id = ch_min = ch_max = ch_anterior = titulacao_id = area_id = 0;
	valor_credito = 0;

	id_operacional = -1;

	/* Parametro utilizado na função de prioridade para o modelo operacional. */
	custoDispProf = 0;

	is_virtual = eVirtual;

	if(is_virtual)
	{
		nome = "PROFESSOR VIRTUAL";

		std::cout << "ToDo: <Professor> Definir com a equipe, o que sera necessario instanciar para o professor virtual." << std::endl;

		/*
		Um possivel erro. Se o Cleiton utiliza os dias disponiveis do professor para calcular alguma coisa, qdo se tratar de um novo prof, nao teremos esses dados.

		Solucao 1: Qdo criar um prof assim (por ex. os virtuais), criar todos os objetos que um prof tem que ter. dai, preenche-se, por ex., os dias letivos como se fossem todos.
		Solucao 2: Qdo o Cleiton for tratar de um professor novo (virtual), ele nao checa os dias, apenas admite que eh sempre compativel com tudo.
		*/

		exit(1);
	}
}

Professor::~Professor(void)
{
}

void Professor::le_arvore(ItemProfessor& elem)
{
	this->setId( elem.id() );

	if(elem.areaTitulacaoId().present())
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

	ITERA_SEQ(it_mag,elem.disciplinas(),ProfessorDisciplina)
	{
		Magisterio* m = new Magisterio();
		m->le_arvore(*it_mag);
		magisterio.add(m);
	}

	ITERA_SEQ(it_h,elem.horariosDisponiveis(),Horario)
	{
		Horario* h = new Horario();
		h->le_arvore(*it_h);
		horarios.add(h);
	}
}
