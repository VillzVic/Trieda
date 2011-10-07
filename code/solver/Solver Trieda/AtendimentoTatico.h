#ifndef _ATENDIMENTO_TATICO_H_
#define _ATENDIMENTO_TATICO_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoOferta.h"

class AtendimentoTatico:
	public OFBase
{
public:
	AtendimentoTatico( int, int );
	virtual ~AtendimentoTatico( void );

	AtendimentoOferta * atendimento_oferta;

   void setQtdCreditosTeoricos( int value ) { this->qtde_creditos_teoricos = value; }
   void setQtdCreditosPraticos( int value ) { this->qtde_creditos_praticos = value; }

   int getQtdCreditosTeoricos() const { return this->qtde_creditos_teoricos; }
   int getQtdCreditosPraticos() const { return this->qtde_creditos_praticos; }

private:
	int qtde_creditos_teoricos;
	int qtde_creditos_praticos;
};

std::ostream& operator << ( std::ostream &, AtendimentoTatico & );

#endif
