#include "CreditoDisponivel.h"

CreditoDisponivel::CreditoDisponivel(void)
{
}

CreditoDisponivel::~CreditoDisponivel(void)
{
}
void CreditoDisponivel::le_arvore(ItemCreditoDisponivel& elem) {
   turno_id = elem.turnoId();
   dia_semana = elem.diaSemana();
   max_creditos = elem.maxCreditos();
}