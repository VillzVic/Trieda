#ifndef _ATENDIMENTO_SALA_SOLUCAO_H_
#define _ATENDIMENTO_SALA_SOLUCAO_H_

#include "AtendimentoTaticoSolucao.h"

AtendimentoTaticoSolucao::AtendimentoTaticoSolucao(void)
{
}

AtendimentoTaticoSolucao::~AtendimentoTaticoSolucao(void)
{
}

void AtendimentoTaticoSolucao::le_arvore(ItemAtendimentoTaticoSolucao& elem)
{
   qtdeCreditosTeoricos = elem.qtdeCreditosTeoricos();
   qtdeCreditosPraticos = elem.qtdeCreditosPraticos();

   atendimento_oferta = new AtendimentoOfertaSolucao();
   atendimento_oferta->le_arvore(elem.atendimentoOferta());
}

#endif