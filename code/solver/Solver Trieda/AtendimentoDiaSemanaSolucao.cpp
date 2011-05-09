#include "AtendimentoDiaSemanaSolucao.h"

AtendimentoDiaSemanaSolucao::AtendimentoDiaSemanaSolucao(void)
{
}

AtendimentoDiaSemanaSolucao::AtendimentoDiaSemanaSolucao(AtendimentoDiaSemana & at_Dia_Sem)
{
   this->setId( at_Dia_Sem.getDiaSemana() );
   diaSemana = at_Dia_Sem.getDiaSemana();

   ITERA_GGROUP( it_At_Tatico, *at_Dia_Sem.atendimentos_tatico,
				 AtendimentoTatico )
   {
      atendimentosTatico.add( new AtendimentoTaticoSolucao( **it_At_Tatico ) );
   }
}

AtendimentoDiaSemanaSolucao::~AtendimentoDiaSemanaSolucao(void)
{

}

void AtendimentoDiaSemanaSolucao::le_arvore( ItemAtendimentoDiaSemanaSolucao & elem )
{
   this->setId( elem.diaSemana() );
   diaSemana = elem.diaSemana();

   for (unsigned int i = 0; i<elem.atendimentosTatico().AtendimentoTatico().size(); i++)   	
   {
      ItemAtendimentoTaticoSolucao item = elem.atendimentosTatico().AtendimentoTatico().at(i);

      AtendimentoTaticoSolucao* tatico = new AtendimentoTaticoSolucao();
      tatico->le_arvore(item);
      atendimentosTatico.add(tatico);
   }
}
