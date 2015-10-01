#include "Campus.h"
#include "Oferta.h"

#include "ConjUnidades.h"

Campus::Campus(void)
{
	totalSalas = 0;
	maiorSala = 0;
	custo = 0;
	possuiAlunoFormando = false;
}

Campus::~Campus(void)
{
	// apagar clusters unidades
	for (auto it = clustersUnidades.begin(); it != clustersUnidades.end(); it++)
	{
		ConjUnidades* cjt = *it;
		if (cjt != nullptr) delete cjt;
	}
	clustersUnidades.clear();
	cout << "\n campus-clustersUnidades was deleted!"; fflush(0);

	// apagar unidades
	unidades.deleteElements();

	cout << "\n campus-unidades was deleted!"; fflush(0);

	// apagar professores
	//professores.deleteElements(); //TODO: Verificar quem vai ser o responsável por remover os professores virtuais...

	cout << "\n campus-professores was deleted! (or not... =D)"; fflush(0);

	// apagar horarios
	horarios.deleteElements();
	cout << "\n campus-horarios was deleted!"; fflush(0);
}

void Campus::le_arvore(ItemCampus & elem)
{
	this->setId(elem.id());
	this->setCodigo(elem.codigo());
	this->setNome(elem.nome());
	this->setCusto(elem.custo());

	ITERA_SEQ(it_unidades, elem.unidades(), Unidade)
	{
		Unidade * unidade = new Unidade();
		unidade->le_arvore(*it_unidades);
		unidades.add(unidade);
	}

	// O campo 'id_operacional_professor' é utilizado na classe
	// que representa a 'SolucaoOperacional', para acessar a matriz
	// da solução. O id_operacional deve ser preenchido de forma
	// sequencial, começando a partir de zero.
	int id_operacional_professor = 0;

	ITERA_SEQ(it_professores, elem.professores(), Professor)
	{
		Professor * professor = new Professor();

		professor->le_arvore(*it_professores);
		professor->setIdOperacional(id_operacional_professor);
		id_operacional_professor++;

		professores.add(professor);
	}

	ITERA_SEQ(it_horarios, elem.horariosDisponiveis(), Horario)
	{
		Horario * horario = new Horario();
		horario->le_arvore(*it_horarios);
		horarios.add(horario);
	}
}

/*
	Dado um id de curso e um id de disciplina, retorna as ofertas existentes
	no campus para o curso especificado, cujos currículos contêm a disciplina
	especificada.
	*/
GGroup<Oferta*, Less<Oferta*>> Campus::retornaOfertasComCursoDisc(int idCurso, Disciplina *d)
{
	GGroup<Oferta*, Less<Oferta*>> ofts;
	for (auto itOft = ofertas.begin(); itOft != ofertas.end(); itOft++)
	{
		Oferta* oft = *itOft;
		if (oft->getCursoId() == idCurso)
			if (oft->possuiDisciplina(d))
				ofts.add(oft);
	}
	return ofts;
}

GGroup<Calendario*> Campus::getCalendarios()
{
	GGroup<Calendario*> calendarios;
	for (auto itHor = horarios.begin(); itHor != horarios.end(); ++itHor)
		calendarios.add(itHor->horario_aula->getCalendario());
	return calendarios;
}
