#ifndef _ATENDIMENTO_SALA_SOLUCAO_H_
#define _ATENDIMENTO_SALA_SOLUCAO_H_

#include "AtendimentoOfertaSolucao.h"

AtendimentoOfertaSolucao::AtendimentoOfertaSolucao(void)
{
}

AtendimentoOfertaSolucao::~AtendimentoOfertaSolucao(void)
{
}

void AtendimentoOfertaSolucao::le_arvore(ItemAtendimentoOfertaSolucao& elem)
{
	ofertaCursoCampiId = elem.ofertaCursoCampiId();
	disciplinaId = elem.disciplinaId();
	quantidade = elem.quantidade();
	turma = elem.turma();
}

#endif