#include "AtendimentoTatico.h"

AtendimentoTatico::AtendimentoTatico(void)
{
   atendimento_oferta = NULL;

   qtde_creditos_teoricos = 0;
   qtde_creditos_praticos = 0;
}

AtendimentoTatico::~AtendimentoTatico(void)
{
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