#include "AtendimentoTatico.h"

AtendimentoTatico::AtendimentoTatico(void)
{
   //atendimento_oferta = NULL;
   atendimento_oferta = new AtendimentoOferta();

   qtde_creditos_teoricos = 0;
   qtde_creditos_praticos = 0;
}

AtendimentoTatico::~AtendimentoTatico(void)
{
   if( atendimento_oferta != NULL )
   {
      //atendimento_oferta->deleteElements();
      delete atendimento_oferta;
   }
}

std::ostream& operator << (std::ostream& out, AtendimentoTatico& tatico)
{
   out << "<AtendimentoTatico>" << endl;

   if(tatico.atendimento_oferta) {
      out << "<atendimentoOferta>" << endl << (*tatico.atendimento_oferta) << "</atendimentoOferta>" << endl;
   }
   else {
      out << "<atendimentoOferta />" << endl;
   }

   out << "<qtdeCreditosTeoricos>" << tatico.qtde_creditos_teoricos << "</qtdeCreditosTeoricos>" << endl;

   out << "<qtdeCreditosPraticos>" << tatico.qtde_creditos_praticos << "</qtdeCreditosPraticos>" << endl;

   out << "</AtendimentoTatico>" << endl;

   return out;
}