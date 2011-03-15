#ifndef _ATENDIMENTO_SALA_SOLUCAO_H_
#define _ATENDIMENTO_SALA_SOLUCAO_H_

#include "AtendimentoDiaSemanaSolucao.h"

AtendimentoDiaSemanaSolucao::AtendimentoDiaSemanaSolucao(void)
{
}

AtendimentoDiaSemanaSolucao::~AtendimentoDiaSemanaSolucao(void)
{
}

void AtendimentoDiaSemanaSolucao::le_arvore(ItemAtendimentoDiaSemanaSolucao& elem)
{
   diaSemana = elem.diaSemana();

   for (unsigned int i=0; i<elem.atendimentosTatico().AtendimentoTatico().size(); i++)   	
   {
	   ItemAtendimentoTaticoSolucao item = elem.atendimentosTatico().AtendimentoTatico().at(i);

       AtendimentoTaticoSolucao* tatico = new AtendimentoTaticoSolucao();
       tatico->le_arvore(item);
	   atendimentosTatico.add(tatico);
   }
}

#endif