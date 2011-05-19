#include "SolucaoInicialOperacional.h"

bool ordenaCustosAlocacao( CustoAlocacao * left, CustoAlocacao * right )
{
   bool result = ( *left > *right );
   return result;
}

SolucaoInicialOperacional::SolucaoInicialOperacional( ProblemData & _problemData )
: problemData( _problemData )
{
   // ----------------------------------------------------------------------

   solIni = new SolucaoOperacional( &problemData );

   // ----------------------------------------------------------------------
   /* Inicialmente, todas as aulas devem ser consideradas como n�o alocadas. */

   aulasNaoAlocadas = problemData.aulas;

   // ----------------------------------------------------------------------
   /* Armazena separadamente (seguindo os crit�rios estabelecidos abaixo) as fixa��es mais 
   restritivas(pr�-processadas).*/

   std::map< std::vector< int /*ID's : prof, disc, sala, dia*/>,
      GGroup< Fixacao *, LessPtr< Fixacao > > > horariosFixados;

   // Armazena o total de cr�ditos fixados (admitindo
   // apenas as fixa��es que pertencem � estrutura
   // fixacoes_Prof_Disc_Sala_Dia_Horario) para cada disciplina.
   std::map< Disciplina *, int /*Total Creds. Fixados*/, LessPtr< Disciplina > > discTTCredsFix;

   // Separando as fixacoes por professor, disc, sala e dia.
   ITERA_GGROUP_LESSPTR( itFixacao, problemData. fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
   {
      std::vector< int /*ID's : prof, disc, sala, dia*/ > chave ( 4, -1 );

      chave.at( 0 ) = itFixacao->getProfessorId();
      chave.at( 1 ) = itFixacao->getDisciplinaId();
      chave.at( 2 ) = itFixacao->getSalaId();
      chave.at( 3 ) = itFixacao->getDiaSemana();

      horariosFixados[chave].add(*itFixacao);

      std::map< Disciplina *, int /*Total Creds. Fixados*/, LessPtr< Disciplina > >::iterator
         itDiscTTCredsFix = discTTCredsFix.find( itFixacao->disciplina );

      if ( itDiscTTCredsFix != discTTCredsFix.end() )
	  {
         itDiscTTCredsFix->second += 1;
	  }
      else
	  {
         discTTCredsFix[ itFixacao->disciplina ] = 1;
	  }
   }

   // Se a estrutura <horariosFixados> possui algum elemento.
   if ( horariosFixados.size() > 0 )
   {
      // Fazer o processamento para verificar se existe alguma aula
	  // que � fortemente fixada. Ou seja, se existe alguma aula que
	  // foi fixada para um total de hor�rios IGUAL ao total de cr�ditos
	  // que ela disp�e. Assim, n�o h� como tentar trocar de hor�rio. 

      // Caso exista alguma aula nessa situa��o, deve-se remov�-la da
	  // estrutura <aulas>. Em seguida, deve-se aloc�-la � solu��o nos
	  // hor�rios fixados. (Obs.: N�o � necess�rio preocupar com a
	  // estrutura <aulasNaoRelacionadasProf> a estrutura <horariosFixados>
	  // trata apenas de fixa��es que possuem o campo Professor setado).

      // Para cada conjunto de hor�rios fixados
      std::map< std::vector< int /*ID's : prof, disc, sala, dia*/ >,
				GGroup< Fixacao *, LessPtr< Fixacao > > >::iterator

      itHorariosFixados = horariosFixados.begin();
      for (; itHorariosFixados != horariosFixados.end(); ++itHorariosFixados )
      {
         // Como todas as fixa��es s�o relacionadas a mesma disc., sala ...  n�o tem problema.
         Disciplina & disc = *( itHorariosFixados->second.begin()->disciplina );

         std::map< Disciplina *, int /*Total Creds. Fixados*/, LessPtr< Disciplina > >::iterator
            itDiscTTCredsFix = discTTCredsFix.find( &disc );

         if ( itDiscTTCredsFix != discTTCredsFix.end() )
         {
            if ( itDiscTTCredsFix->second
					== ( disc.getCredPraticos() + disc.getCredTeoricos() ) )
            {
               Aula * aula = NULL;
               Sala & sala = *( itHorariosFixados->second.begin()->sala );

               // Procurando a aula a ser removida.
               ITERA_GGROUP_LESSPTR( itAula, aulasNaoAlocadas, Aula )
               {
                  if (   ( *( itAula->getDisciplina() ) == disc )
					  && ( *( itAula->getSala()) == sala )
					  &&  itAula->getDiaSemana() == itHorariosFixados->first.at( 3 ) )
                  {
                     aula = ( *itAula );

                     // Removendo a aula do grupo de aulas a
					 // serem alocadas pela fun��o de prioridade.
                     aulasNaoAlocadas.remove( itAula );

					 // Encontrei a aula, ent�o PARO.
                     break;
                  }
               }

               // Setando a aula como fixada
               aula->setAulaFixada( true );

               Professor & professor = *( itHorariosFixados->second.begin()->professor );
               std::vector< HorarioAula * > horariosAulaAlocar;

               ITERA_GGROUP_LESSPTR( itFixacao, itHorariosFixados->second, Fixacao )
               {
				   horariosAulaAlocar.push_back( itFixacao->horario_aula );
			   }

               solIni->alocaAulaProf( *aula, professor, horariosAulaAlocar );
            }
         }
      }
   }
   // ----------------------------------------------------------------------

   executaFuncaoPrioridade();

   // ----------------------------------------------------------------------
   // Inicializando a estrutura <custosAlocacaoAulaOrdenado>
   // para poder auxiliar na aloca��o de aulas a professores.

   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
      custosAlocacaoAulaOrdenado.push_back( itCustoProfTurma->second );

   // ----------------------------------------------------------------------
   // Removendo os custos de aloca��o referentes a aulas que
   // possuam fixa��es do tipo: professor, dia e hor�rio. Ou seja,
   // s�o fixa��es bastante restritivas que impedem qualquer tipo
   // de movimento por parte das estruturas de vizinhan�a.

   // ToDo !!! Fazer a aloca��o aqui msm e marcar a aula como alocada !!!

   // ----------------------------------------------------------------------
   // Ordenando os custos de aloca��o obtidos via execu��o da fun��o de prioridade.

   std::sort( custosAlocacaoAulaOrdenado.begin(),custosAlocacaoAulaOrdenado.end(), ordenaCustosAlocacao );

   // ----------------------------------------------------------------------

   moveValidator = new MoveValidator( &problemData );

   // ----------------------------------------------------------------------
}

SolucaoInicialOperacional::~SolucaoInicialOperacional()
{
   delete moveValidator;
}

SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial()
{
   return * solIni;
}

bool SolucaoInicialOperacional::possui_fixacao_professor_dia_horario(
     Professor & professor, int dia_semana, int horario_aula_id )
{
   // Verificar quais aulas s�o fixadas, para impedir que as mesmas
   // sejam trocadas no solveOperacional Obs.: As aulas que s�o
   // 'fixadas' aqui s�o apenas as fixa��es de professor, dia e hor�rio.
   // Qualquer outro tipo de verifica��o de fixa��o deve ser feita
   // na realiza��o de cada movimento.
   Fixacao * fixacao = NULL;

   // Fixa��es do tipo 'professor + disciplina + sala + dia + hor�rio'
   ITERA_GGROUP_LESSPTR( it_fixacao,
      problemData.fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
   {
      fixacao = ( *it_fixacao );

      if ( fixacao->getDiaSemana() == dia_semana
			&& fixacao->getHorarioAulaId() == horario_aula_id
			&& fixacao->professor->getId() == professor.getId() )
      {
         return true;
      }
   }

   // Fixa��es do tipo 'professor + disciplina + dia + hor�rio'
   ITERA_GGROUP_LESSPTR( it_fixacao,
      problemData.fixacoes_Prof_Disc_Dia_Horario, Fixacao )
   {
      fixacao = ( *it_fixacao );

      if ( fixacao->getDiaSemana() == dia_semana
			&& fixacao->getHorarioAulaId() == horario_aula_id
			&& fixacao->professor->getId() == professor.getId() )
      {
         return true;
      }
   }

   return false;
}

bool SolucaoInicialOperacional::aulaFixadaProfDiaHorario (Aula const & aula) const
{
   ITERA_GGROUP_LESSPTR( itFixacoes, problemData.fixacoes, Fixacao )
   {
		// TODO
   }

   return false;
}

//TRIEDA-896
bool SolucaoInicialOperacional::alocaAulaSeq( SolucaoOperacional * solucao, std::vector< Aula * >::iterator itHorariosProf,
                                              int totalHorariosConsiderados, Professor & professor, Aula & aula )
{
   return false;
}

bool SolucaoInicialOperacional::professorRepetido( Professor & professor, Aula & aula )
{
   // Flag que indica se um professor j� est� associado ao bloco.
   bool encontrouProfBloco = false;

   // Para cada oferta da aula
   ITERA_GGROUP( itOferta, aula.ofertas, Oferta )
   {
      // Descobrindo o bloco da oferta em quest�o.
      BlocoCurricular * bloco_curricular = problemData.mapCursoDisciplina_BlocoCurricular
         [ std::make_pair( itOferta->curso, aula.getDisciplina() ) ];

      std::map< BlocoCurricular *, GGroup< Professor *, LessPtr< Professor > > >::iterator
         itBlocosProfs = blocosProfs.find( bloco_curricular );

      // Se encontrei o bloco
      if ( itBlocosProfs != blocosProfs.end() )
      {
         GGroup< Professor *, LessPtr< Professor > >::iterator 
            itProf = itBlocosProfs->second.find( &professor );

         // Se o professor em quest�o j� est� associado ao bloco
         if ( itProf != itBlocosProfs->second.end() )
         {
            return true;
         }
      }
   }

   return encontrouProfBloco;
}

void SolucaoInicialOperacional::executaFuncaoPrioridade()
{
   ITERA_GGROUP_LESSPTR( itCampus, problemData.campi, Campus )
   {
      ITERA_GGROUP( itProfessor, itCampus->professores, Professor )
      {
         // Contabilizando os hor�rios dispon�veis de um professor. 
         // Assim, toda vez que um CustoAlocacao for instanciado, o custo
         // referente � "disponibilidade do professor p" ser� atualizado.
         // H� necessidade de converter o valor para que seja somado corretamente.

         // Ex. 
         // custoDispProf_A = 10 -> 10 horarios disponiveis
         // custoDispProf_B = 5 -> 5 horarios disponiveis
         // custoDispProf_B tem prioridade MAIOR que custoDispProf_A.
         // prioridade = -(custoDispProf_A - (TOTALHORARIOSCAMPUS+1))

         // Dado que TOTALHORARIOSCAMPUS = 10, ent�o:
         // prioridade(custoDispProf_A) = 1
         // prioridade(custoDispProf_A) = 6
         // Estamos adimitindo aqui que a inst. possui apenas um CAMPUS.
         // N�o funcionar� para multicampus.

         itProfessor->setCustoDispProf( itCampus->horarios.size() );

         ITERA_GGROUP( itMagisterio, itProfessor->magisterio, Magisterio )
         {
            Disciplina * discMinistradaProf = itMagisterio->disciplina;

            // Iterarndo sobre as aulas que ainda n�o foram alocadas.
            ITERA_GGROUP_LESSPTR(itAula,aulasNaoAlocadas,Aula)
            {
               // Como todoas as ofertas de uma aula devem
               // ser para o mesmo campus, podemos comparar
               // a primeira oferta. Desse modo, estamos
               // tratando apenas das aulas de um dado campus.
               if ( itAula->ofertas.begin()->campus == ( *itCampus ) )
               {
                  Disciplina * discAula = itAula->getDisciplina();

                  if ( discMinistradaProf == discAula )
                  {
                     // Para o primeiro custo da fun��o de prioridade,
                     // tenho que testar agora se existe fixa��o desse
                     // professor para a disciplina da aula em quest�o.

                     std::pair< Professor *, Disciplina * > chave ( *itProfessor, discMinistradaProf );

                     std::map< std::pair< Professor *, Disciplina * >, GGroup< Fixacao * > >::iterator
                        itFixacoesProfDisc = problemData.fixacoesProfDisc.find( chave );

                     // Somente se existir, pelo menos, uma fixa��o de um professor a uma disciplina.
                     if ( itFixacoesProfDisc != problemData.fixacoesProfDisc.end() )
                     {
                        ITERA_GGROUP( itFixacao, itFixacoesProfDisc->second, Fixacao )
                        {
                           calculaCustoFixProf( **itProfessor, **itAula, 0 );
                        }
                     }

                     // Para o segundo custo considerado para o c�lculo da
                     // fun��o de prioridade, tenho que somar a nota (prefer�ncia)
                     // dada pelo professor para a disciplina em quest�o.

                     // Dado que a maior prefer�ncia recebe nota 1 e a
                     // menor recebe nota 10, basta subtrair a nota (prefer�ncia)
                     // por 11 e, em seguida, multiplicar o resultado por -1.
                     // Assim, somaremos um valor correto ao custo, j� que o
                     // melhor custo total possuir� o maior valor atribuido.
                     calculaCustoFixProf( **itProfessor, **itAula, 1,
                        itMagisterio->getPreferencia() );

                     std::pair< int, int > chaveGamb ( itProfessor->getId(),
                        discMinistradaProf->getId() );

                     // Se o professor e a disciplina da aula em quest�o se relacionarem:
                     if ( problemData.prof_Disc_Dias.find( chaveGamb )
                        != problemData.prof_Disc_Dias.end() )
                     {
                        int custo = problemData.prof_Disc_Dias[ chaveGamb ].size();
                        calculaCustoFixProf( **itProfessor, **itAula, 3,
                           custo, itCampus->horarios.size() );
                     }
                  }
               }
            }
         }
      }
   }
}

void SolucaoInicialOperacional::calculaCustoFixProf( Professor & prof , Aula & aula,
                                                    unsigned idCusto, int custo, int maxHorariosCP )
{
   std::pair< Professor *, Aula * > chave ( & prof, & aula );
   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
      itCustoProfTurma = custoProfTurma.find( chave );

   // Criando, se necess�rio, um novo CustoAlocacao dada a chave em quest�o.
   if ( itCustoProfTurma == custoProfTurma.end() )
   {
      custoProfTurma[ chave ] = new CustoAlocacao( prof, aula );
   }

   // custoFixProfTurma
   if ( idCusto == 0 )
   {
      custoProfTurma[ chave ]->addCustoFixProfTurma( 100 );
   }
   else if ( idCusto == 1 )
   {
      int preferenciaProfDisc = ( custo - 11 ) * ( -1 );
      custoProfTurma[ chave ]->addCustoPrefProfTurma( preferenciaProfDisc );
   }
   else if ( idCusto == 3 )
   {
      // Aqui, compartilha-se a ideia da terceira restri��o da fun��o de prioridade.
      int custoDispProfTurma = -( custo - ( maxHorariosCP + 1 ) );
      custoProfTurma[ chave ]->addCustoDispProfTurma( custoDispProfTurma );
   }
   else
   {
      std::cout << "ERRO: idCusto (" << idCusto << ") INVALIDO." << std::endl;
      exit(0);
   }
}
