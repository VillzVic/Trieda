#ifndef _ATENDIMENTO_UNIDADE_SOLUCAO_H_
#define _ATENDIMENTO_UNIDADE_SOLUCAO_H_

#include "AtendimentoUnidadeSolucao.h"

AtendimentoUnidadeSolucao::AtendimentoUnidadeSolucao(void)
{
}

AtendimentoUnidadeSolucao::~AtendimentoUnidadeSolucao(void)
{
}

void AtendimentoUnidadeSolucao::le_arvore(ItemAtendimentoUnidadeSolucao& elem)
{
   unidadeId = elem.unidadeId();
   unidadeCodigo = elem.unidadeCodigo();

   for (unsigned int i=0; i<elem.atendimentosSalas().AtendimentoSala().size(); i++)   	
   {
	   ItemAtendimentoSalaSolucao item = elem.atendimentosSalas().AtendimentoSala().at(i);

       AtendimentoSalaSolucao* sala = new AtendimentoSalaSolucao();
       sala->le_arvore(item);
       atendimentosSalas.add(sala);
   }
}

#endif