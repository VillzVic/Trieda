#include "Oferta.h"

Oferta::Oferta(void)
{
	curriculo = NULL;
	curso = NULL;
	turno = NULL;
	campus = NULL;
   receita = 0;
}

Oferta::~Oferta(void)
{

}

void Oferta::le_arvore( ItemOfertaCurso & elem )
{
	this->setId( elem.id() );

	curriculo_id = elem.curriculoId();
	curso_id = elem.cursoId();
	turno_id = elem.turnoId();
	campus_id = elem.campusId();
   receita = elem.receita();
}

bool Oferta::possuiDisciplina( Disciplina *d )
{
	/*GGroup< std::pair< int, Disciplina * > >::iterator it = curriculo->disciplinas_periodo.begin();
	for ( ; it != curriculo->disciplinas_periodo.end(); it++ )
	{
		if ( (*it).second->getId() == idDisc )
			return true;
	}
	return false;*/

	return curriculo->possuiDisciplina(d);
}

/*
	Retorna o periodo de uma disciplina no curriculo da oferta.
	Se o curriculo nao contiver a disciplina, retorna -1.
*/
int Oferta::periodoDisciplina( Disciplina *d )
{
	return curriculo->getPeriodo(d);
}

