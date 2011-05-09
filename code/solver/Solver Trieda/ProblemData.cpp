#include "ProblemData.h"
#include <iostream>

// Macro baseada na ITERA_SEQ para facilitar a leitura dos dados no
// ProblemData
#define LE_SEQ(ggroup,addr,type) \
   for (Grupo##type##::##type##_iterator __##type##_it = \
   (addr).type##().begin(); __##type##_it != (addr).type##().end(); \
   ++ __##type##_it) { \
   type *__obj_##type## = new type; \
   __obj_##type##->le_arvore(*__##type##_it); \
   ggroup.add(__obj_##type); } 

ProblemData::ProblemData()
{
   atendimentosTatico = NULL;
}

ProblemData::~ProblemData()
{

}

void ProblemData::le_arvore( TriedaInput & raiz )
{
   calendario = new Calendario();
   calendario->le_arvore( raiz.calendario() );

   ITERA_SEQ( it_campi, raiz.campi(), Campus )
   {
      Campus * campus = new Campus;
      campus->le_arvore( *it_campi );
      campi.add( campus );
   }

   ITERA_SEQ( it_tsalas, raiz.tiposSala(), TipoSala )
   {
      TipoSala * tipo_sala = new TipoSala;
      tipo_sala->le_arvore( *it_tsalas );
      tipos_sala.add( tipo_sala );
   }

   LE_SEQ( tipos_sala, raiz.tiposSala(), TipoSala );
   LE_SEQ( tipos_contrato, raiz.tiposContrato(), TipoContrato );
   LE_SEQ( tipos_titulacao, raiz.tiposTitulacao(), TipoTitulacao );
   LE_SEQ( areas_titulacao, raiz.areasTitulacao(), AreaTitulacao );
   LE_SEQ( tipos_disciplina, raiz.tiposDisciplina(), TipoDisciplina );
   LE_SEQ( niveis_dificuldade, raiz.niveisDificuldade(), NivelDificuldade );
   LE_SEQ( tipos_curso, raiz.tiposCurso(), TipoCurso );
   LE_SEQ( regras_div, raiz.regrasDivisaoCredito(), DivisaoCreditos );
   LE_SEQ( tempo_campi, raiz.temposDeslocamentosCampi(), Deslocamento );
   LE_SEQ( tempo_unidades, raiz.temposDeslocamentosUnidades(), Deslocamento );
   LE_SEQ( disciplinas, raiz.disciplinas(), Disciplina );
   LE_SEQ( cursos, raiz.cursos(), Curso );
   LE_SEQ( demandas, raiz.demandas(), Demanda );

   int id = 1;
   Demanda * demanda = NULL;
   ITERA_GGROUP( it_demanda, demandas, Demanda )
   {
	   demanda = *(it_demanda);
	   demanda->setId( id );
	   id++;
   }

   ITERA_SEQ( it_oferta, raiz.ofertaCursosCampi(), OfertaCurso )
   {
      Oferta * oferta = new Oferta;
      oferta->le_arvore( *it_oferta );
      ofertas.add( oferta );
   }

   parametros = new ParametrosPlanejamento;
   parametros->le_arvore( raiz.parametrosPlanejamento() );

   // Se a tag existir (mesmo que esteja em branco) no xml de entrada
   if ( raiz.atendimentosTatico().present() )
   {
      atendimentosTatico = new GGroup< AtendimentoCampusSolucao * > ();

      for ( unsigned int i = 0;
		    i < raiz.atendimentosTatico().get().AtendimentoCampus().size(); i++ )
      {
         ItemAtendimentoCampusSolucao * it_atendimento
            = &( raiz.atendimentosTatico().get().AtendimentoCampus().at(i) );

         AtendimentoCampusSolucao * item = new AtendimentoCampusSolucao();
         item->le_arvore( *(it_atendimento) );
         atendimentosTatico->add( item );
      }
   }

   LE_SEQ( fixacoes, raiz.fixacoes(), Fixacao );

   // Monta um 'map' para recuperar cada 'ItemSala'
   // do campus a partir de seu respectivo id de sala
   std::map< int, ItemSala * > mapItemSala;
   ITERA_SEQ( it_campi, raiz.campi(), Campus )
   {
      ITERA_SEQ( it_unidade, it_campi->unidades(), Unidade )
      {
         ITERA_SEQ( it_sala, it_unidade->salas(), Sala )
         {
            mapItemSala[ it_sala->id() ] = &(*(it_sala));
         }
      }
   }

   //-------------------------------------------------------------------------------
   // Primeiro caso : executar o
   // solver apenas com a entrada do tático
   bool primeiroCaso = ( parametros->modo_otimizacao == "TATICO" );

   // Segundo caso  : executar o solver com
   // a saída do tático e a entrada do operacional
   bool segundoCaso  = ( parametros->modo_otimizacao == "OPERACIONAL"
							&& raiz.atendimentosTatico().present() == true );

   // Terceiro caso : executar o solver apenas
   // com a entrada do operacional (sem saída do tático)
   bool terceiroCaso = ( parametros->modo_otimizacao == "OPERACIONAL"
							&& raiz.atendimentosTatico().present() == false );

   // Informa o modo de otimização que será execuado
   if (primeiroCaso)
   {
	   std::cout << "TATICO" << std::endl;
   }
   else if (segundoCaso)
   {
	   std::cout << "OPERACIONAL COM OUTPUT TATICO" << std::endl;
   }
   else if (terceiroCaso)
   {
	   std::cout << "OPERACIONAL SEM OUTPUT TATICO" << std::endl;
   }
	else
	{
		// ERRO no XML de entrada
		std::cout << "ERROR!!! input inválido para os campos:"
				  << "\n'horariosDisponiveis' e/ou 'creditosDisponiveis'"
				  << "\n\nSando." << std::endl;

		exit(1);
	}

   // Prencher os horários e/ou créditos das salas
   ITERA_GGROUP( it_campi, campi, Campus )
   {
      ITERA_GGROUP( it_unidade, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP( it_sala, it_unidade->salas, Sala )
         {
			 std::map< int, ItemSala * >::iterator it
				 = mapItemSala.find(it_sala->getId());

            if ( it != mapItemSala.end() )
            {
               // Objeto de entrada do XML
               ItemSala * elem = it->second;

               it_sala->construirCreditosHorarios(
					*(elem), parametros->modo_otimizacao,
					raiz.atendimentosTatico().present() );
            }
         }
      }
   }
   //-------------------------------------------------------------------------------
}
