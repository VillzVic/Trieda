#ifndef _ATENDIMENTO_SALA_SOLUCAO_H_
#define _ATENDIMENTO_SALA_SOLUCAO_H_

#include "AtendimentoSalaSolucao.h"

AtendimentoSalaSolucao::AtendimentoSalaSolucao(void)
{
}

AtendimentoSalaSolucao::~AtendimentoSalaSolucao(void)
{
}

void AtendimentoSalaSolucao::le_arvore(ItemAtendimentoSalaSolucao& elem)
{
   salaId = elem.salaId();
   salaNome = elem.salaNome();

   for (unsigned int i=0; i<elem.atendimentosDiasSemana().AtendimentoDiaSemana().size(); i++)   	
   {
	   ItemAtendimentoDiaSemanaSolucao item = elem.atendimentosDiasSemana().AtendimentoDiaSemana().at(i);

       AtendimentoDiaSemanaSolucao* diaSemana = new AtendimentoDiaSemanaSolucao();
       diaSemana->le_arvore(item);
       atendimentosDiasSemana.add(diaSemana);
   }
}

#endif