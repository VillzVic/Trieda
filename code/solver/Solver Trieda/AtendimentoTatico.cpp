#include "AtendimentoTatico.h"

AtendimentoTatico::AtendimentoTatico(void)
{
}

AtendimentoTatico::~AtendimentoTatico(void)
{
}

std::ostream& operator << (std::ostream& out, AtendimentoTatico& tatico)
{
   out << "<Tatico>" << endl;

   out << "<Oferta>" << (*tatico.atendimento_oferta) << "</Oferta>" << endl;

   out << "<qtdeCreditosTeoricos>" << tatico.qtde_creditos_teoricos << "</qtdeCreditosTeoricos>";

   out << "<qtdeCreditosPraticos>" << tatico.qtde_creditos_praticos << "</qtdeCreditosPraticos>";

   out << "</Tatico>" << endl;

   return out;
}