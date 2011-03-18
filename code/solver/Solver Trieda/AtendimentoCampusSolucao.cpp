#ifndef _ATENDIMENTO_CAMPUS_SOLUCAO_H_
#define _ATENDIMENTO_CAMPUS_SOLUCAO_H_

#include "ofbase.h"
#include "AtendimentoCampusSolucao.h"

AtendimentoCampusSolucao::AtendimentoCampusSolucao(void)
{
}

AtendimentoCampusSolucao::AtendimentoCampusSolucao(AtendimentoCampus & at_Campus)
{
   this->setId(at_Campus.campus->getId());
   campusId = at_Campus.campus->getId();
   campusCodigo = at_Campus.campus->codigo;

   ITERA_GGROUP(it_At_Und,*at_Campus.atendimentos_unidades,AtendimentoUnidade)
   {
      atendimentosUnidades.add(new AtendimentoUnidadeSolucao(**it_At_Und));
   }
}

AtendimentoCampusSolucao::~AtendimentoCampusSolucao(void)
{
}

void AtendimentoCampusSolucao::le_arvore(ItemAtendimentoCampusSolucao& elem)
{
   this->setId(elem.campusId());
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