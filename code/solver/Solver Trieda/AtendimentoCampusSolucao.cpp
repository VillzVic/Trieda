#ifndef _ATENDIMENTO_CAMPUS_SOLUCAO_H_
#define _ATENDIMENTO_CAMPUS_SOLUCAO_H_

#include "ofbase.h"
#include "AtendimentoCampusSolucao.h"

AtendimentoCampusSolucao::AtendimentoCampusSolucao(void)
{
}

AtendimentoCampusSolucao::~AtendimentoCampusSolucao(void)
{
}

void AtendimentoCampusSolucao::le_arvore(ItemAtendimentoCampusSolucao& elem)
{
   campusId = elem.campusId();
   campusCodigo = elem.campusCodigo();

   for (unsigned int i=0; i<elem.atendimentosUnidades().AtendimentoUnidade().size(); i++)   	
   {
	   ItemAtendimentoUnidadeSolucao item = elem.atendimentosUnidades().AtendimentoUnidade().at(i);

       AtendimentoUnidadeSolucao* unidade = new AtendimentoUnidadeSolucao();
       unidade->le_arvore(item);
       atendimentosUnidades.add(unidade);
   }
}

#endif