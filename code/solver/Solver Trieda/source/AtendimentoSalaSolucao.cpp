#include "AtendimentoSalaSolucao.h"

AtendimentoSalaSolucao::AtendimentoSalaSolucao(void)
{
}

AtendimentoSalaSolucao::AtendimentoSalaSolucao(AtendimentoSala & at_Sala)
{
//	std::cout<<"\nAtendimentoSalaSolucao... ";
   this->setId(at_Sala.sala->getId());
   salaId = at_Sala.sala->getId();
   salaNome = at_Sala.sala->getCodigo();

   auto it_At_Dia_Sem = at_Sala.atendimentos_dias_semana->begin();
   for(; it_At_Dia_Sem != at_Sala.atendimentos_dias_semana->end(); ++it_At_Dia_Sem)
   {
//	   std::cout<<"\nit_At_Dia_Sem... " << it_At_Dia_Sem->getId(); fflush(0);

      atendimentosDiasSemana.add(new AtendimentoDiaSemanaSolucao(**it_At_Dia_Sem));
   }
}

AtendimentoSalaSolucao::~AtendimentoSalaSolucao(void)
{
}

void AtendimentoSalaSolucao::le_arvore(ItemAtendimentoSalaSolucao& elem)
{
   this->setId(elem.salaId());
   salaId = elem.salaId();
   salaNome = elem.salaNome();

   for (unsigned int i=0; i<elem.atendimentosDiasSemana().AtendimentoDiaSemana().size(); i++)   	
   {
	   ItemAtendimentoDiaSemanaSolucao item = elem.atendimentosDiasSemana().AtendimentoDiaSemana().at(i);

       AtendimentoDiaSemanaSolucao* diaSemana = new AtendimentoDiaSemanaSolucao();
       diaSemana->le_arvore(item);
       atendimentosDiasSemana.add(diaSemana);
   }
}
