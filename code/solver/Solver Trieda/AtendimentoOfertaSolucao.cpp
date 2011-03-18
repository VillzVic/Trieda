#ifndef _ATENDIMENTO_SALA_SOLUCAO_H_
#define _ATENDIMENTO_SALA_SOLUCAO_H_

#include "AtendimentoOfertaSolucao.h"

AtendimentoOfertaSolucao::AtendimentoOfertaSolucao(void)
{
}

AtendimentoOfertaSolucao::AtendimentoOfertaSolucao(AtendimentoOferta & at_Oft)
{
   ofertaCursoCampiId = at_Oft.oferta->curso_id;
   disciplinaId= at_Oft.disciplina_id;
   quantidade= at_Oft.quantidade;
   
   stringstream str;
   str << at_Oft.turma;
   
   turma = str.str();
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