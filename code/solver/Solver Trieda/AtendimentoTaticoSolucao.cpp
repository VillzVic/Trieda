#ifndef _ATENDIMENTO_SALA_SOLUCAO_H_
#define _ATENDIMENTO_SALA_SOLUCAO_H_

#include "AtendimentoTaticoSolucao.h"

AtendimentoTaticoSolucao::AtendimentoTaticoSolucao(void)
{
}

AtendimentoTaticoSolucao::AtendimentoTaticoSolucao( AtendimentoTatico & at_Tatico )
{
   this->setId( at_Tatico.atendimento_oferta->oferta->getId() );

   qtdeCreditosTeoricos = at_Tatico.getQtdCreditosTeoricos();
   qtdeCreditosPraticos = at_Tatico.getQtdCreditosPraticos();

   atendimento_oferta = new AtendimentoOfertaSolucao( *at_Tatico.atendimento_oferta );
}

AtendimentoTaticoSolucao::~AtendimentoTaticoSolucao(void)
{
}

void AtendimentoTaticoSolucao::le_arvore(ItemAtendimentoTaticoSolucao& elem)
{
   this->setId(elem.atendimentoOferta().ofertaCursoCampiId());

   qtdeCreditosTeoricos = elem.qtdeCreditosTeoricos();
   qtdeCreditosPraticos = elem.qtdeCreditosPraticos();

   atendimento_oferta = new AtendimentoOfertaSolucao();
   atendimento_oferta->le_arvore(elem.atendimentoOferta());
}

#endif