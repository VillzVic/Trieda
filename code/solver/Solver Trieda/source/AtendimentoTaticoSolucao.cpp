#include "AtendimentoTaticoSolucao.h"

AtendimentoTaticoSolucao::AtendimentoTaticoSolucao(void)
{
    fixaAbertura = false;
    fixaSala = false;
    fixaDia = false;
    fixaHorarios = false;
}

AtendimentoTaticoSolucao::AtendimentoTaticoSolucao( AtendimentoTatico & at_Tatico )
{
   qtdeCreditosTeoricos = at_Tatico.getQtdCreditosTeoricos();
   qtdeCreditosPraticos = at_Tatico.getQtdCreditosPraticos();
   fixaAbertura = false;
   fixaSala = false;
   fixaDia = false;
   fixaHorarios = false;

   GGroup<int> hors = at_Tatico.getHorariosAula();
   ITERA_GGROUP_N_PT( itHorId, hors, int )
   {
		horariosAula.add( *itHorId );
   }

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

   ITERA_NSEQ( itHorId,elem.horariosAula(), id, Identificador )
   {
		horariosAula.add( *itHorId );
   }
   
   fixaAbertura = false;
   fixaSala = false;
   fixaDia = false;
   fixaHorarios = false;

   if ( elem.fixaAbertura().present() )
	   fixaAbertura = elem.fixaAbertura().get();
   if ( elem.fixaSala().present() )
	   fixaSala = elem.fixaSala().get();
   if ( elem.fixaDia().present() )
	   fixaDia = elem.fixaDia().get();
   if ( elem.fixaHorarios().present() )
	   fixaHorarios = elem.fixaHorarios().get();

   atendimento_oferta = new AtendimentoOfertaSolucao();
   atendimento_oferta->le_arvore(elem.atendimentoOferta());

   if ( atendimento_oferta->getDisciplinaId() == 13548 && atoi( atendimento_oferta->getTurma().c_str() ) == 2 )
   {	   
	   std::cout << "\ndisc13548 e turma2: ";
	   for( GGroup< int >::iterator it= horariosAula.begin(); it!= horariosAula.end(); it++ )
	   {
			std::cout << " " << *it;
	   }
   }

}
