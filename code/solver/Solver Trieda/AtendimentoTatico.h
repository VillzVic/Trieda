#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoOferta.h"

using namespace std;

class AtendimentoTatico:
	public OFBase
{
public:
	AtendimentoTatico(void);
	~AtendimentoTatico(void);

	AtendimentoOferta *atendimento_oferta;
	int qtde_creditos_teoricos;
	int qtde_creditos_praticos;

	//virtual void escreve_arvore(ItemAtendimentoUnidade& elem);

	std::pair<int/*id_sala*/,int/*dia*/> key;


};

std::ostream& operator << (std::ostream& out, AtendimentoTatico& tatico);