#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoOferta.h"

class AtendimentoTatico:
	public OFBase
{
public:
	AtendimentoTatico(void);
	virtual ~AtendimentoTatico(void);

	AtendimentoOferta * atendimento_oferta;

   void setQtdCreditosTeoricos(int value) { qtde_creditos_teoricos = value; }
   void setQtdCreditosPraticos(int value) { qtde_creditos_praticos = value; }

   int getQtdCreditosTeoricos() { return qtde_creditos_teoricos; }
   int getQtdCreditosPraticos() { return qtde_creditos_praticos; }

private:
	int qtde_creditos_teoricos;
	int qtde_creditos_praticos;
};

std::ostream& operator << (std::ostream& out, AtendimentoTatico& tatico);