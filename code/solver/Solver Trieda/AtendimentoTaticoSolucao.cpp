#include "AtendimentoTaticoSolucao.h"

AtendimentoTaticoSolucao::AtendimentoTaticoSolucao(void)
{

}

AtendimentoTaticoSolucao::AtendimentoTaticoSolucao( AtendimentoTatico & at_Tatico )
{
   qtdeCreditosTeoricos = at_Tatico.getQtdCreditosTeoricos();
   qtdeCreditosPraticos = at_Tatico.getQtdCreditosPraticos();

   atendimento_oferta = new AtendimentoOfertaSolucao( *at_Tatico.atendimento_oferta );
}

AtendimentoTaticoSolucao::~AtendimentoTaticoSolucao(void)
{

}

void AtendimentoTaticoSolucao::le_arvore( ItemAtendimentoTaticoSolucao & elem )
{
   this->setId( elem.atendimentoOferta().ofertaCursoCampiId() );

   qtdeCreditosTeoricos = elem.qtdeCreditosTeoricos();
   qtdeCreditosPraticos = elem.qtdeCreditosPraticos();

   ITERA_NSEQ( itAlunoDemId,elem.horariosAula(), id, Identificador )
   {
		horariosAula.add( *itAlunoDemId );
   }
   
   atendimento_oferta = new AtendimentoOfertaSolucao();
   atendimento_oferta->le_arvore(elem.atendimentoOferta());
}
