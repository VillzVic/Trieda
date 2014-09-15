#include "AtendimentoDiaSemanaSolucao.h"

#include "AtendimentoHorarioAula.h"
#include "AtendimentoOferta.h"
#include "AtendimentoOfertaSolucao.h"

AtendimentoDiaSemanaSolucao::AtendimentoDiaSemanaSolucao(void)
{
}

AtendimentoDiaSemanaSolucao::AtendimentoDiaSemanaSolucao(AtendimentoDiaSemana & at_Dia_Sem)
{
	//std::cout<<"\nAtendimentoDiaSemanaSolucao 0";fflush(0);

   this->setId( at_Dia_Sem.getDiaSemana() );
   diaSemana = at_Dia_Sem.getDiaSemana();

   	//std::cout<<"\nAtendimentoDiaSemanaSolucao 1";fflush(0);
   auto it_At_Tatico = at_Dia_Sem.atendimentos_tatico->begin();
   for(; it_At_Tatico != at_Dia_Sem.atendimentos_tatico->end(); ++it_At_Tatico)
   {
      atendimentosTatico.add( new AtendimentoTaticoSolucao( **it_At_Tatico ) );
   }

   //	std::cout<<"\nAtendimentoDiaSemanaSolucao 2";fflush(0);
   /*
   auto it_At_Turno = at_Dia_Sem.atendimentos_turno->begin();
   for(; it_At_Turno != at_Dia_Sem.atendimentos_turno->end(); ++it_At_Turno)
   {
	   multiAtendimentoTaticoSolucao( **it_At_Turno );
   }*/
}

void AtendimentoDiaSemanaSolucao::multiAtendimentoTaticoSolucao( AtendimentoTurno & at_Turno )
{
	int nroCredPrat=0;
	int nroCredTeor=0;
	
//	std::cout<<"\n\tmultiAtendimentoTaticoSolucao 1";fflush(0);
	
	std::map< bool, std::map< AtendimentoOfertaSolucao*, GGroup<int>, LessPtr<AtendimentoOfertaSolucao> > > mapTipoCredAtendOftSolHors;

	ITERA_GGROUP_LESSPTR( itAtendHorAula, *(at_Turno.atendimentos_horarios_aula), AtendimentoHorarioAula )
	{
		AtendimentoHorarioAula *atendimentoHorarioAula = (*itAtendHorAula);
		
		int hId = atendimentoHorarioAula->getHorarioAulaId();
		bool teorico = atendimentoHorarioAula->getCreditoTeorico();
		
		std::map< AtendimentoOfertaSolucao*, GGroup<int>, LessPtr<AtendimentoOfertaSolucao> > 
			*mapAtendOftSol = &mapTipoCredAtendOftSolHors[teorico];

		ITERA_GGROUP_LESSPTR( itAtendOft, *(atendimentoHorarioAula->atendimentos_ofertas), AtendimentoOferta )
		{
			AtendimentoOfertaSolucao *atendimento_oferta = new AtendimentoOfertaSolucao( **itAtendOft );
			
			std::map< AtendimentoOfertaSolucao*, GGroup<int>, LessPtr<AtendimentoOfertaSolucao> >::iterator
				itAtendOftSol = mapAtendOftSol->find( atendimento_oferta );
			if ( itAtendOftSol == mapAtendOftSol->end() )
			{
				mapTipoCredAtendOftSolHors[teorico][atendimento_oferta].add(hId);
			}
			else 
			{
				itAtendOftSol->second.add(hId);
				delete atendimento_oferta;
			}
		}
	}

	//std::cout<<"\n\tmultiAtendimentoTaticoSolucao 2";fflush(0);

	map< bool, map< AtendimentoOfertaSolucao*, GGroup<int>, LessPtr<AtendimentoOfertaSolucao> > >::iterator
		itMapTipoCred = mapTipoCredAtendOftSolHors.begin();
	for ( ; itMapTipoCred != mapTipoCredAtendOftSolHors.end(); itMapTipoCred++ )
	{
		bool teorica = itMapTipoCred->first;

		map< AtendimentoOfertaSolucao*, GGroup<int>, LessPtr<AtendimentoOfertaSolucao> > 
			*mapAtendOftSol = &itMapTipoCred->second;
		map< AtendimentoOfertaSolucao*, GGroup<int>, LessPtr<AtendimentoOfertaSolucao> >::iterator
			itMapAtendOftSol = mapAtendOftSol->begin();
		for ( ; itMapAtendOftSol != mapAtendOftSol->end(); itMapAtendOftSol++  )
		{
			int nroCreds = itMapAtendOftSol->second.size();
			int nroCredTeor = teorica? nroCreds : 0;
			int nroCredPrat = !teorica? nroCreds : 0;

			AtendimentoTaticoSolucao *atendimentoTatico = new AtendimentoTaticoSolucao();
			atendimentoTatico->atendimento_oferta = itMapAtendOftSol->first;
			atendimentoTatico->setQtdeCreditosTeoricos( nroCredTeor );
			atendimentoTatico->setQtdeCreditosPraticos( nroCredPrat );
			ITERA_GGROUP_N_PT( itHorId, itMapAtendOftSol->second, int )
			{
				atendimentoTatico->addHorariosAula( *itHorId );
			}
		}	
	}

	//std::cout<<"\n\tmultiAtendimentoTaticoSolucao 3";fflush(0);
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
