#include "AtendimentoTatico.h"

AtendimentoTatico::AtendimentoTatico(void)
{
   atendimento_oferta = new AtendimentoOferta();

   qtde_creditos_teoricos = 0;
   qtde_creditos_praticos = 0;
}

AtendimentoTatico::~AtendimentoTatico(void)
{
   if( atendimento_oferta != NULL )
   {
       delete atendimento_oferta;
   }
}

std::ostream & operator << ( std::ostream& out, AtendimentoTatico & tatico )
{
   out << "<AtendimentoTatico>" << std::endl;

   if ( tatico.atendimento_oferta )
   {
      out << "<atendimentoOferta>" << std::endl
		  << ( *tatico.atendimento_oferta )
		  << "</atendimentoOferta>" << std::endl;
   }
   else
   {
      out << "<atendimentoOferta />" << std::endl;
   }

   out << "<qtdeCreditosTeoricos>" << tatico.getQtdCreditosTeoricos()
	   << "</qtdeCreditosTeoricos>" << std::endl;

   out << "<qtdeCreditosPraticos>" << tatico.getQtdCreditosPraticos()
	   << "</qtdeCreditosPraticos>" << std::endl;

   out << "</AtendimentoTatico>" << std::endl;

   return out;
}