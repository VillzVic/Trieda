#include "Oferta.h"

Oferta::Oferta(void)
{
	curriculo = NULL;
	curso = NULL;
	turno = NULL;
	campus = NULL;
}

Oferta::~Oferta(void)
{
}
void Oferta::le_arvore(ItemOfertaCurso& elem) {
	id = elem.id();
	curriculo_id = elem.curriculoId();
	curso_id = elem.cursoId();
	turno_id = elem.turnoId();
	campus_id = elem.campusId();
}
