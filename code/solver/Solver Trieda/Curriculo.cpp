#include "Curriculo.h"

Curriculo::Curriculo()
{

}

Curriculo::~Curriculo(void)
{

}

void Curriculo::le_arvore( ItemCurriculo & elem )
{
	this->setId( elem.id() );
	codigo = elem.codigo();

	ITERA_SEQ( it_dp, elem.disciplinasPeriodo(), DisciplinaPeriodo )
	{
		int periodo = it_dp->periodo();
		int disc_id = it_dp->disciplinaId();

		ids_disciplinas_periodo.add( std::make_pair( periodo, disc_id ) );
	}
}

void Curriculo::refDisciplinaPeriodo(
    GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas )
{
	GGroup< std::pair< int, int > >::iterator
		it_ids = ids_disciplinas_periodo.begin();

	for (; it_ids != ids_disciplinas_periodo.end(); it_ids++ )
	{
		int id_periodo = ( *it_ids ).first;
		int id_disciplina = ( *it_ids ).second;

		GGroup< Disciplina *, LessPtr< Disciplina > >::iterator
			it_disc = disciplinas.begin();
		for (; it_disc != disciplinas.end(); it_disc++ )
		{
			if ( it_disc->getId() == id_disciplina )
			{
				disciplinas_periodo.add( std::make_pair( id_periodo, ( *it_disc ) ) );
				break;
			}
		}
	}

	ids_disciplinas_periodo.clear();
}
