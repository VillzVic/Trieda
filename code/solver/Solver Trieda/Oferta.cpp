#include "Oferta.h"

Oferta::Oferta(void)
{
}

Oferta::~Oferta(void)
{
}
void Oferta::le_arvore(ItemOfertaCurso& elem) {
   curriculo_id = elem.curriculoId();
   curso_id = elem.cursoId();
   turno_id = elem.turnoId();
   campus_id = elem.campusId();
}
