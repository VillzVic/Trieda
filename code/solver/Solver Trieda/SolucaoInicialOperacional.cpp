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
   /* Inicialmente, todas as aulas devem ser consideradas como não alocadas. */

   aulasNaoAlocadas = problemData.aulas;

   // ----------------------------------------------------------------------
   /* Armazena separadamente (seguindo os critérios estabelecidos abaixo) as fixações mais 
   restritivas(pré-processadas).*/

   std::map< std::vector< int /*ID's : prof, disc, sala, dia*/>,
      GGroup< Fixacao *, LessPtr< Fixacao > > > horariosFixados;

   // Armazena o total de créditos fixados (admitindo
   // apenas as fixações que pertencem à estrutura
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
	  // que é fortemente fixada. Ou seja, se existe alguma aula que
	  // foi fixada para um total de horários IGUAL ao total de créditos
	  // que ela dispõe. Assim, não há como tentar trocar de horário. 

      // Caso exista alguma aula nessa situação, deve-se removê-la da
	  // estrutura <aulas>. Em seguida, deve-se alocá-la à solução nos
	  // horários fixados. (Obs.: Não é necessário preocupar com a
	  // estrutura <aulasNaoRelacionadasProf> a estrutura <horariosFixados>
	  // trata apenas de fixações que possuem o campo Professor setado).

      // Para cada conjunto de horários fixados
      std::map< std::vector< int /*ID's : prof, disc, sala, dia*/ >,
				GGroup< Fixacao *, LessPtr< Fixacao > > >::iterator

      itHorariosFixados = horariosFixados.begin();
      for (; itHorariosFixados != horariosFixados.end(); ++itHorariosFixados )
      {
         // Como todas as fixações são relacionadas a mesma disc., sala ...  não tem problema.
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
					 // serem alocadas pela função de prioridade.
                     aulasNaoAlocadas.remove( itAula );

					 // Encontrei a aula, então PARO.
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
   // para poder auxiliar na alocação de aulas a professores.

   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
      custosAlocacaoAulaOrdenado.push_back( itCustoProfTurma->second );

   // ----------------------------------------------------------------------
   // Removendo os custos de alocação referentes a aulas que
   // possuam fixações do tipo: professor, dia e horário. Ou seja,
   // são fixações bastante restritivas que impedem qualquer tipo
   // de movimento por parte das estruturas de vizinhança.

   // ToDo !!! Fazer a alocação aqui msm e marcar a aula como alocada !!!

   // ----------------------------------------------------------------------
   // Ordenando os custos de alocação obtidos via execução da função de prioridade.

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
   // Verificar quais aulas são fixadas, para impedir que as mesmas
   // sejam trocadas no solveOperacional Obs.: As aulas que são
   // 'fixadas' aqui são apenas as fixações de professor, dia e horário.
   // Qualquer outro tipo de verificação de fixação deve ser feita
   // na realização de cada movimento.
   Fixacao * fixacao = NULL;

   // Fixações do tipo 'professor + disciplina + sala + dia + horário'
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

   // Fixações do tipo 'professor + disciplina + dia + horário'
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
   // Flag que indica se um professor já está associado ao bloco.
   bool encontrouProfBloco = false;

   // Para cada oferta da aula
   ITERA_GGROUP( itOferta, aula.ofertas, Oferta )
   {
      // Descobrindo o bloco da oferta em questão.
      BlocoCurricular * bloco_curricular = problemData.mapCursoDisciplina_BlocoCurricular
         [ std::make_pair( itOferta->curso, aula.getDisciplina() ) ];

      std::map< BlocoCurricular *, GGroup< Professor *, LessPtr< Professor > > >::iterator
         itBlocosProfs = blocosProfs.find( bloco_curricular );

      // Se encontrei o bloco
      if ( itBlocosProfs != blocosProfs.end() )
      {
         GGroup< Professor *, LessPtr< Professor > >::iterator 
            itProf = itBlocosProfs->second.find( &professor );

         // Se o professor em questão já está associado ao bloco
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
         // Contabilizando os horários disponíveis de um professor. 
         // Assim, toda vez que um CustoAlocacao for instanciado, o custo
         // referente à "disponibilidade do professor p" será atualizado.
         // Há necessidade de converter o valor para que seja somado corretamente.

         // Ex. 
         // custoDispProf_A = 10 -> 10 horarios disponiveis
         // custoDispProf_B = 5 -> 5 horarios disponiveis
         // custoDispProf_B tem prioridade MAIOR que custoDispProf_A.
         // prioridade = -(custoDispProf_A - (TOTALHORARIOSCAMPUS+1))

         // Dado que TOTALHORARIOSCAMPUS = 10, então:
         // prioridade(custoDispProf_A) = 1
         // prioridade(custoDispProf_A) = 6
         // Estamos adimitindo aqui que a inst. possui apenas um CAMPUS.
         // Não funcionará para multicampus.

         itProfessor->setCustoDispProf( itCampus->horarios.size() );

         ITERA_GGROUP( itMagisterio, itProfessor->magisterio, Magisterio )
         {
            Disciplina * discMinistradaProf = itMagisterio->disciplina;

            // Iterarndo sobre as aulas que ainda não foram alocadas.
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
                     // Para o primeiro custo da função de prioridade,
                     // tenho que testar agora se existe fixação desse
                     // professor para a disciplina da aula em questão.

                     std::pair< Professor *, Disciplina * > chave ( *itProfessor, discMinistradaProf );

                     std::map< std::pair< Professor *, Disciplina * >, GGroup< Fixacao * > >::iterator
                        itFixacoesProfDisc = problemData.fixacoesProfDisc.find( chave );

                     // Somente se existir, pelo menos, uma fixação de um professor a uma disciplina.
                     if ( itFixacoesProfDisc != problemData.fixacoesProfDisc.end() )
                     {
                        ITERA_GGROUP( itFixacao, itFixacoesProfDisc->second, Fixacao )
                        {
                           calculaCustoFixProf( **itProfessor, **itAula, 0 );
                        }
                     }

                     // Para o segundo custo considerado para o cálculo da
                     // função de prioridade, tenho que somar a nota (preferência)
                     // dada pelo professor para a disciplina em questão.

                     // Dado que a maior preferência recebe nota 1 e a
                     // menor recebe nota 10, basta subtrair a nota (preferência)
                     // por 11 e, em seguida, multiplicar o resultado por -1.
                     // Assim, somaremos um valor correto ao custo, já que o
                     // melhor custo total possuirá o maior valor atribuido.
                     calculaCustoFixProf( **itProfessor, **itAula, 1,
                        itMagisterio->getPreferencia() );

                     std::pair< int, int > chaveGamb ( itProfessor->getId(),
                        discMinistradaProf->getId() );

                     // Se o professor e a disciplina da aula em questão se relacionarem:
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

   // Criando, se necessário, um novo CustoAlocacao dada a chave em questão.
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
      // Aqui, compartilha-se a ideia da terceira restrição da função de prioridade.
      int custoDispProfTurma = -( custo - ( maxHorariosCP + 1 ) );
      custoProfTurma[ chave ]->addCustoDispProfTurma( custoDispProfTurma );
   }
   else
   {
      std::cout << "ERRO: idCusto (" << idCusto << ") INVALIDO." << std::endl;
      exit(0);
   }
}
