#include "AtendimentoUnidadeSolucao.h"

AtendimentoUnidadeSolucao::AtendimentoUnidadeSolucao(void)
{

}

AtendimentoUnidadeSolucao::AtendimentoUnidadeSolucao(AtendimentoUnidade & at_Und)
{
//	std::cout<<"\nAtendimentoUnidadeSolucao... ";
   this->setId(at_Und.unidade->getId());
   unidadeId = at_Und.unidade->getId();
   unidadeCodigo = at_Und.unidade->getCodigo();

   for(auto it_At_Sala = at_Und.atendimentos_salas->begin(); it_At_Sala != at_Und.atendimentos_salas->end(); ++it_At_Sala)
   {
      atendimentosSalas.add(new AtendimentoSalaSolucao(**it_At_Sala));
   }
}

AtendimentoUnidadeSolucao::~AtendimentoUnidadeSolucao(void)
{
}

void AtendimentoUnidadeSolucao::le_arvore(ItemAtendimentoUnidadeSolucao& elem)
{
   this->setId(elem.unidadeId());
   unidadeId = elem.unidadeId();
   unidadeCodigo = elem.unidadeCodigo();

   for (unsigned int i=0; i<elem.atendimentosSalas().AtendimentoSala().size(); i++)   	
   {
	   ItemAtendimentoSalaSolucao item = elem.atendimentosSalas().AtendimentoSala().at(i);

       AtendimentoSalaSolucao* sala = new AtendimentoSalaSolucao();
       sala->le_arvore(item);
       atendimentosSalas.add(sala);
   }
}
