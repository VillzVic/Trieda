#include "Curriculo.h"

Curriculo::Curriculo()
{

}

Curriculo::~Curriculo(void)
{

}

void Curriculo::le_arvore(ItemCurriculo& elem)
{
	this->setId( elem.id() );
	codigo = elem.codigo();

	ITERA_SEQ( it_dp, elem.disciplinasPeriodo(), DisciplinaPeriodo )
	{
		int periodo = it_dp->periodo();
		int disc_id = it_dp->disciplinaId();
		disciplinas_periodo.add( std::make_pair( periodo, disc_id ) );
	}
}
