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
   ///* Armazena separadamente (seguindo os critérios estabelecidos abaixo) as fixações mais 
   //restritivas(pré-processadas).*/

   //std::map<
   //   std::vector<int/*ID's : prof, disc, sala, dia*/>,
   //   GGroup<Fixacao*,LessPtr<Fixacao> > >

   //   horariosFixados;

   ///* Armazena o total de créditos fixados (admitindo apenas as fixações que pertencem à estrutura
   //fixacoes_Prof_Disc_Sala_Dia_Horario) para cada disciplina. */
   //std::map<Disciplina*,int/*Total Creds. Fixados*/,LessPtr<Disciplina> > discTTCredsFix;

   //// Separando as fixacoes por professor, disc, sala e dia.
   //ITERA_GGROUP_LESSPTR(itFixacao,problemData.fixacoes_Prof_Disc_Sala_Dia_Horario,Fixacao)
   //{
   //   std::vector<int/*ID's : prof, disc, sala, dia*/> chave (4,-1);

   //   chave.at(0) = itFixacao->getProfessorId();
   //   chave.at(1) = itFixacao->getDisciplinaId();
   //   chave.at(2) = itFixacao->getSalaId();
   //   chave.at(3) = itFixacao->getDiaSemana();

   //   horariosFixados[chave].add(*itFixacao);

   //   std::map<Disciplina*,int/*Total Creds. Fixados*/,LessPtr<Disciplina> >::iterator
   //      itDiscTTCredsFix = discTTCredsFix.find(itFixacao->disciplina);
   //   
   //   if(itDiscTTCredsFix != discTTCredsFix.end())
   //      itDiscTTCredsFix->second += 1;
   //   else
   //      discTTCredsFix[itFixacao->disciplina] = 1;
   //}

   //// Se a estrutura <horariosFixados> possui algum elemento.
   //if(horariosFixados.size() > 0)
   //{
   //   /* Fazer o processamento para verificar se existe alguma aula que é fortemente fixada. Ou seja,
   //   se existe alguma aula que foi fixada para um total de horários IGUAL ao total de créditos que 
   //   ela dispõe. Assim, não há como tentar trocar de horário. 
   //   
   //   Caso exista alguma aula nessa situação, deve-se removê-la da estrutura <aulas>. Em seguida,
   //   deve-se alocá-la à solução nos horários fixados. (Obs.: Não é necessário preocupar com a estrutura
   //   <aulasNaoRelacionadasProf> a estrutura <horariosFixados> trata apenas de fixações que possuem o 
   //   campo Professor setado).
   //   */

   //   // Para cada conjunto de horários fixados
   //   std::map<
   //      std::vector<int/*ID's : prof, disc, sala, dia*/>,
   //      GGroup<Fixacao*,LessPtr<Fixacao> > >::iterator

   //      itHorariosFixados = horariosFixados.begin();

   //   for(; itHorariosFixados != horariosFixados.end(); ++itHorariosFixados)
   //   {
   //      // Como todas as fixações são relacionadas a mesma disc., sala ...  não tem problema.
   //      Disciplina & disc = *(itHorariosFixados->second.begin()->disciplina);

   //      std::map<Disciplina*,int/*Total Creds. Fixados*/,LessPtr<Disciplina> >::iterator
   //         itDiscTTCredsFix = discTTCredsFix.find(&disc);

   //      if(itDiscTTCredsFix != discTTCredsFix.end())
   //      {
   //         if(itDiscTTCredsFix->second == (disc.getCredPraticos() + disc.getCredTeoricos()))
   //         {
   //            Sala & sala = *(itHorariosFixados->second.begin()->sala);

   //            Aula * aula = NULL;

   //            // Procurando a aula a ser removida.
   //            ITERA_GGROUP_LESSPTR(itAula,aulasNaoAlocadas,Aula)
   //            {
   //               if((*(itAula->getDisciplina()) == disc) &&
   //                  (*(itAula->getSala()) == sala) && 
   //                  itAula->getDiaSemana() == itHorariosFixados->first.at(3/*dia*/))
   //               {
   //                  aula = *itAula;

   //                  // Removendo a aula do grupo de aulas a serem alocadas pela função de prioridade.
   //                  aulasNaoAlocadas.remove(itAula);

   //                  break; // Encontrei a aula, então PARO.
   //               }
   //            }

   //            // Setando a aula como fixada
   //            aula->setAulaFixada(true);

   //            Professor & professor = *(itHorariosFixados->second.begin()->professor);

   //            std::vector<HorarioAula*> horariosAulaAlocar;

   //            ITERA_GGROUP_LESSPTR(itFixacao,itHorariosFixados->second,Fixacao)
   //            { horariosAulaAlocar.push_back(itFixacao->horario_aula); }

   //            solIni->alocaAulaProf(*aula,professor,horariosAulaAlocar);
   //         }
   //      }
   //   }
   //}
   ////else
   ////{
   //   /* Apesar da fixação ter sido bastante restritiva, pode ser que o usuário tenha fixado mais horários
   //   do que a disciplina precisa para poder alocar os créditos. Também podem ter sido feitas outras fixações,
   //   ou nenhuma. Portanto, pode-se tentar alocar todas as aulas. */

   //   /* Não deve-se fazer nada aqui (APENAS DEIXAR O COMENTÁRIO =]). 
   //   A estrutura <aulas> está pronta para ser utilizada no cálculo da função de prioridade. */
   ////}

   // ----------------------------------------------------------------------

   executaFuncaoPrioridade();

   // ----------------------------------------------------------------------
   // Inicializando a estrutura <custosAlocacaoAulaOrdenado> para poder auxiliar na alocação de aulas a professores.

   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
      custosAlocacaoAulaOrdenado.push_back( itCustoProfTurma->second );

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
   /* Tentando alocar as aulas seguindo a ordem dos custos que lhes foram associados. */

   std::vector< CustoAlocacao * >::iterator 
      itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();
   
   for(; itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
   {
      CustoAlocacao & custoAlocacao = **itCustosAlocacaoAulaOrdenado;
      Aula & aula = custoAlocacao.getAula();

      GGroup< Aula *, LessPtr< Aula > >::iterator 
         itAulasNaoAlocadas = aulasNaoAlocadas.find(&aula);

      // Somente se a aula do custo em questão não foi alocada.
      if (itAulasNaoAlocadas != aulasNaoAlocadas.end())
      {
         /* Teste simples para saber se uma aula que vamos tentar alocar já possui horários a serem alocados. */
         if(solIni->blocoAulas.find(&aula) != solIni->blocoAulas.end())
         {
            /* Não pode tentar alocar a aula mais de uma vez na solução inicial. Se cair aqui é pq tem algum 
            erro de lógica. Rever o algoritmo de geração da solução inicial. */
            std::cout << "\n\nERRO NA GERACAO DA SOLUCAO INICIAL: TENTANDO ALOCAR UMA AULA QUE JA FOI ALOCADA.\n\n";
            exit(1);
         }

         Professor & professor = custoAlocacao.getProfessor();

         /* Antes de tentar encontrar uma sequência de horários livres do professor em questão deve-se
         checar se existe uma fixação que especifique o professor e, possivelmente a sala, o dia e os 
         horários em que a aula deve ser ministrada.
         
         Toda vez que uma fixação que não permita nenhuma outra possibilidade de alocação da aula for 
         satisfeita, deve-se marcar a aula como FIXADA. Trata-se da fixação do Professor a uma aula em 
         um determinado dia nos horários especificados.

         POR ENQUANTO, NENHUMA FIXAÇÃO DO MÓDULO OPERACIONAL SERÁ CONSIDERADA. -> MÁRIO 19/05/2011
         */

         // Tentando todas as sequências de horários vagos do professor.
         int hrIni = 0;
         int hrFim = aula.getTotalCreditos();

         while(hrFim < solIni->getTotalHorarios())
         {
            std::vector<int> seqHorarioAula;
            for(int hrAtual = hrIni; hrAtual <= hrFim; ++hrAtual)
               seqHorarioAula.push_back(hrAtual);

            if(solIni->seqHorarioLivre(professor.getIdOperacional(),seqHorarioAula))
            {
               std::vector<HorarioAula*>::iterator 
                  itHA = problemData.horarios_aula_ordenados.begin();

               // Obtendo referências para os horários de aula.
               std::vector<HorarioAula*> horariosAula ((itHA+hrIni),(itHA+hrFim));

               /* Agora que alguma sequência de horários aula livres de um dado professor foi encontrada,
               deve-se checar se, além da sala para a qual a aula está alocada possuir tais horários, se esses 
               horários estão desocupados. Também há necessidade de verificar se não vai acontecer algum conflito
               com as outras aulas do bloco curricular em questão, ou se um estamos tentando alocar um professor
               que já está ministrando alguma outra aula do bloco curricular.
               */

               //professorRepetido(professor,aula);

               /* Iterador utilizado nos testes abaixo. */
               std::vector<HorarioAula*>::iterator itHorariosAula;

               /* Checando se a sala possui os horários de aula selecionados. */
               
               bool salaPossuiHorarios = true;

               itHorariosAula = horariosAula.begin();
               for(; itHorariosAula != horariosAula.end(); ++itHorariosAula)
               {
                  bool encontrouHorario = false;

                  ITERA_GGROUP(itHorario,aula.getSala()->horarios_disponiveis,Horario)
                  {
                     if( *(itHorario->horario_aula) == **itHorariosAula)
                     { encontrouHorario = true; break; }
                  }

                  if(!encontrouHorario)
                  { salaPossuiHorarios = false; break; }
               }

               if(!salaPossuiHorarios)
               { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; }

               /* Checando se a sala está livre no horários indicados. */

               bool horariosAulaSalaLivres = false;

               //std::map< Sala *, GGroup< std::pair< int /*Dia Semana*/, HorarioAula * > >, LessPtr<Sala> >::iterator
               //   itHorariosAulaSala = horariosAulaSala.find(aula.getSala());

               std::map< Sala*, 
                  std::map< int /*Dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > >,
                  LessPtr<Sala> >::iterator 
                  
                  itHorariosAulaSala = horariosAulaSala.find(aula.getSala());

               if(itHorariosAulaSala == horariosAulaSala.end())
               { 
                  // Significa que não existe nenhum registro de horário de aula alocado para a sala em questão.
                  horariosAulaSalaLivres = true;
               }
               else
               {
                  std::map< int /*Dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > >::iterator
                     itDiaHorariosAulaSala = itHorariosAulaSala->second.find(aula.getDiaSemana());

                  if(itDiaHorariosAulaSala == itHorariosAulaSala->second.end())
                  {
                     /* Embora exista registro de um, ou mais horários alocados para a sala, para o dia 
                     em questão todos os horários estão livres. */
                     horariosAulaSalaLivres = true;
                  }
                  else
                  {
                     bool encontrouHorario = false;

                     itHorariosAula = horariosAula.begin();
                     for(; itHorariosAula != horariosAula.end(); ++itHorariosAula)
                     {
                        /* Basta que um horário esteja ocupado para concluir que a sequência de horários a ser analisada não é válida. */
                        if(itDiaHorariosAulaSala->second.find(*itHorariosAula) != itDiaHorariosAulaSala->second.end())
                        { encontrouHorario = true; break; }
                     }

                     if(!encontrouHorario)
                     { horariosAulaSalaLivres = true; }
                  }
               }

               if(!horariosAulaSalaLivres)
               { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; }

               /* Checando se trata-se de um professor repetido ou se há conflito de bloco curricular. */

               if(moveValidator->checkBlockConflict(aula,horariosAula,*solIni) &&
                  moveValidator->checkClassAndLessonDisponibility(aula,horariosAula,*solIni))
               {
                  /* Procedimentos para alocar a aula. */

                  solIni->alocaAulaProf(aula,professor,horariosAula);

                  // Removendo a aula da relação de aulas não alocadas.
                  aulasNaoAlocadas.remove(itAulasNaoAlocadas);

                  /* Atualizando a estrutura <horariosAulaSala>. */
                  itHorariosAula = horariosAula.begin();
                  for(; itHorariosAula != horariosAula.end(); ++itHorariosAula)
                     horariosAulaSala[aula.getSala()][aula.getDiaSemana()].add(*itHorariosAula);
                  
                  break; // NÃO DEVE mais buscar intervalos de horários livres.
               }
            }
         }
      }
   }

   /* Mesmo que tenha-se utilizado todos os custos de alocação, pode acontecer o caso em que uma, ou mais aulas
   não tenham sido alocadas. */
   if(aulasNaoAlocadas.size() > 0)
   {

   }

   return * solIni;
}

bool SolucaoInicialOperacional::alocaAulaSeq( SolucaoOperacional * solucao, std::vector< Aula * >::iterator itHorariosProf,
                                             int totalHorariosConsiderados, Professor & professor, Aula & aula )
{
   return false;
}
//{
//   bool alocou = false;
//
//   int dia_semana = aula.getDiaSemana();
//   int idOperacionalProf = professor.getIdOperacional();
//   const int total_horarios = solucao->getTotalHorarios();
//
//   // O objetivo aqui é iterar sobre os horários letivos do professor,
//   // para o dia em questão até que o primeiro horário livre seja
//   // encontrado (pode ser que o professor não possua nenhum horário
//   // livre). Uma vez encontrado o horário livre, a ideia agora é obter,
//   // sequencialmente, o total de horários livres demandados pela
//   // disciplina da aula em questão. Só então uma alocação pode ser concluída.
//
//   bool sequenciaDeHorariosLivres = false;
//   std::vector< Aula * >::iterator itInicioHorariosAlocar = itHorariosProf;
//   int totalCredsAlocar = aula.getTotalCreditos();
//
//   // Marca a posição do vector de aulas do
//   // professor onde serão alocadas as aulas
//   int coluna_matriz = 0;
//   int id_horario_aula = 0;
//   HorarioAula * horario_aula = NULL;
//   Horario * horario = NULL;
//
//   for ( int indice_horario = 0; indice_horario < totalHorariosConsiderados;
//      ++indice_horario, ++itHorariosProf )
//   {
//      // Se a aula não for 'NULL'
//      if ( *itHorariosProf )
//      {
//         sequenciaDeHorariosLivres = false;
//         totalCredsAlocar = aula.getTotalCreditos();
//         continue;
//      }
//
//      // --------------------------------------------------------------------------------------
//      // Checando se o horário em questão está disponível na sala.
//
//      // Garantido que vou sempre achar o horario aula id que procuro pq nunca
//      // procuro um horario que já esteja preenchido. Portanto, não há como acontecer
//      // um caso em que o horário procurado não pertença a sala.
//      int idHorarioAula = problemData.horarios_aula_ordenados.at( indice_horario )->getId();
//
//      GGroup< Horario * >::iterator itHorarioSala
//         = aula.getSala()->horarios_disponiveis.begin();
//
//      for(; itHorarioSala != aula.getSala()->horarios_disponiveis.end(); ++itHorarioSala )
//      {
//         if ( itHorarioSala->horario_aula->getId() == idHorarioAula )
//         {
//            break;
//         }
//      }
//
//      // Caso tenha chegado até o final e não encontrado o horário aula
//      // disponível na sala, então não posso alocar a aula nessa sala e horário.
//      if ( itHorarioSala == aula.getSala()->horarios_disponiveis.end() )
//      {
//         sequenciaDeHorariosLivres = false;
//         totalCredsAlocar = aula.getTotalCreditos();
//         continue;
//      }
//
//      std::map< Sala *, GGroup< std::pair< HorarioAula *, int > > >::iterator 
//         it_sala_horarios_alocados = sala_horarios_alocados.find( aula.getSala() );
//
//      // Se a sala já possui algum registro de alocação.
//      if ( it_sala_horarios_alocados != sala_horarios_alocados.end() )
//      {
//         bool sala_ocupada = false;
//
//         // Verifica se a sala já está ocupada no
//         // horário em que queremos alocar a aula atual
//         GGroup< std::pair< HorarioAula *, int > > * horarios_sala
//            = &( it_sala_horarios_alocados->second );
//
//         GGroup< std::pair< HorarioAula *, int > >::iterator it_horarios
//            = horarios_sala->begin();
//
//         for (; it_horarios != horarios_sala->end(); it_horarios++ )
//         {
//            std::pair< HorarioAula *, int > pair_horario_dia = ( *it_horarios );
//
//            // Verifica se a aula está ocupada no
//            // horário de aula atual e no memso dia da semana
//            if ( pair_horario_dia.first == itHorarioSala->horario_aula
//               && pair_horario_dia.second == dia_semana )
//            {
//               sala_ocupada = true;
//               break;
//            }
//         }
//
//         // A sala está alocada no horário em questão
//         if ( sala_ocupada )
//         {
//            sequenciaDeHorariosLivres = false;
//            totalCredsAlocar = aula.getTotalCreditos();
//            continue;
//         }
//      }
//      // --------------------------------------------------------------------------------------
//
//      // --------------------------------------------------------------------------------------
//      // Chegando aqui, sabemos que a sala que a
//      // aula está tentando alocar está disponível
//      if ( totalCredsAlocar == aula.getTotalCreditos() )
//      {
//         itInicioHorariosAlocar = itHorariosProf;
//      }
//
//      // Se a disciplina possuir apenas um crédito
//      // para o dia em questão, basta alocá-la.
//      if ( aula.getTotalCreditos() == 1 )
//      {
//         std::cout << "Saindo qdo uma aula tem apenas 1 horario para ser alocado. (MARIO)" << std::endl;
//         exit(1);
//
//         // Procura a coluna da aula para a
//         // qual o iterador está apontando
//         std::vector< Aula * >::iterator it_aula
//            = solucao->getMatrizAulas()->at( idOperacionalProf )->begin();
//
//         coluna_matriz = ( std::distance( it_aula, itHorariosProf ) );
//         id_horario_aula = ( coluna_matriz % total_horarios );
//
//         // Recupera o objeto 'HorarioAula' do horário atual
//         horario_aula = problemData.horarios_aula_ordenados[ id_horario_aula ];
//
//         GGroup< Horario * >::iterator it_horario = professor.horarios.begin();
//
//         for (; it_horario != professor.horarios.end();	it_horario++ )
//         {
//            horario = ( *it_horario );
//
//            // Checando se encontrei o horário que estou buscando. 
//            if ( horario->dias_semana.find( dia_semana ) != horario->dias_semana.end()
//               && horario->horario_aula->getId() == horario_aula->getId() )
//            {
//               std::vector<HorarioAula*> hA (1,horario_aula);
//               
//               solucao->blocoAulas[&aula] = 
//                  std::make_pair(&professor,hA);
//
//               std::map<Aula*,std::pair<Professor*,std::vector<HorarioAula*> > >::iterator
//                  itBlocoAulas = solucao->blocoAulas.end();
//
//               itBlocoAulas = solucao->blocoAulas.find(&aula);
//
//               if ( !( moveValidator->checkBlockConflict( aula, itBlocoAulas->second.second, *solucao ) ) &&
//                  ( moveValidator->checkClassAndLessonDisponibility( aula, itBlocoAulas->second.second, *solucao ) ) )
//               {
//                  if ( *itHorariosProf )
//                  {
//                     std::cout << "<I> Sobreposicao de CREDITO" << std::endl;
//                     exit(1);
//                  }
//                  else
//                  {
//                     sequenciaDeHorariosLivres = false;
//                     ( *itHorariosProf ) = &( aula );
//
//                     // Informa que a sala está ocupada nesse dia da semana e nesse horário de aula
//                     std::pair< HorarioAula *, int /*Dia*/ > horario_dia = std::make_pair( horario->horario_aula, dia_semana );
//
//                     sala_horarios_alocados[ aula.getSala() ].add( horario_dia );
//                  }
//
//                  //aula.bloco_aula = novoHorarioAula;
//
//                  alocou = true;
//                  break;
//               }
//            }
//         }
//      }
//      else if ( aula.getTotalCreditos() > 1 )
//      {
//         // Caso a disciplina possua mais de um crédito
//         // a ser alocado, a ideia aqui é aloca-los em 
//         // sequência. Portanto, devo verificar se os
//         // créditos demandados estão livres.
//
//         // Atualizo o total de créditos alocados.
//         --totalCredsAlocar;
//
//         if ( totalCredsAlocar == 0 )
//         {
//            sequenciaDeHorariosLivres = true;
//
//            // Posso parar a busca pq já
//            // encontrei o total de créditos necessários.
//            break;
//         }
//      }
//   }
//
//   // Se encontrei uma sequência de horários livres, aloco.
//   if ( sequenciaDeHorariosLivres )
//   {
//      //std::map<Aula*,std::pair<Professor*,std::vector<HorarioAula*> > >::iterator
//      //   itBlocoAulas = solucao->blocoAulas.find(&aula);
//
//      //if(itBlocoAulas == solucao->blocoAulas.end())
//      //{
//      //   //std::cout << "AULA NAO ENCONTRADA EM SolucaoInicialOperacional.\n";
//      //   //exit(1);
//      //}
//
//      //bool blockConflict = moveValidator->checkBlockConflict( aula, itBlocoAulas->second.second, *solucao);
//      bool blockConflict = moveValidator->checkBlockConflict( aula, itBlocoAulas->second.second, *solucao);
//      bool classAndLessonDisponibility = moveValidator->checkBlockConflict( aula, itBlocoAulas->second.second, *solucao);
//
//      //if ( !( moveValidator->checkBlockConflict( aula, solucao->blocoAulas.find(&aula)->second.second, *solucao) ) &&
//      //   ( moveValidator->checkClassAndLessonDisponibility( aula, solucao->blocoAulas.find(&aula)->second.second, *solucao ) ) )
//      //if ( !blockConflict && classAndLessonDisponibility )
//      {
//
//         // Procura a coluna da aula para a qual o iterador está apontando.
//         std::vector< Aula * >::iterator it_aula
//            = solucao->getMatrizAulas()->at( idOperacionalProf )->begin();
//
//         coluna_matriz = ( std::distance( it_aula, itInicioHorariosAlocar ) );
//
//         id_horario_aula = ( coluna_matriz % total_horarios );
//
//         for ( int i = 0; i < aula.getTotalCreditos(); i++, id_horario_aula++ )
//         {
//            // Recupera o objeto 'HorarioAula' do horário atual
//            id_horario_aula %= ( total_horarios );
//
//            horario_aula = problemData.horarios_aula_ordenados[ id_horario_aula ];
//
//            GGroup< Horario * >::iterator it_horario = professor.horarios.begin();
//
//            // Verificando se o professor possui 
//            for (; it_horario != professor.horarios.end(); it_horario++ )
//            {
//               horario = ( *it_horario );
//
//               if ( horario->dias_semana.find( dia_semana ) != horario->dias_semana.end()
//                  && horario->horario_aula->getId() == horario_aula->getId() )
//               {
//
//// VALIDAR AQUI. utilizar o <horario_aula>.
//
//                  std::map<Aula*,std::pair<Professor*,std::vector<HorarioAula*> > >::iterator
//                     itBlocoAulas = solucao->blocoAulas.find(&aula);
//
//                  if(itBlocoAulas == solucao->blocoAulas.end())
//                     solucao->blocoAulas[&aula] = std::make_pair(&professor,std::vector<HorarioAula*>(1,horario_aula));
//                  else
//                     solucao->blocoAulas[&aula].second.push_back(horario_aula);
//
//                  // Informa que a sala está ocupada nesse dia da semana e nesse horário de aula
//                  std::pair< HorarioAula *, int /*Dia*/ > horario_dia
//                     = std::make_pair( horario->horario_aula, dia_semana );
//
//                  sala_horarios_alocados[ aula.getSala() ].add( horario_dia );
//
//                  break;
//               }
//            }
//         }
//      }
//
//      //if ( !( moveValidator->checkBlockConflict( aula, solucao->blocoAulas.find(&aula)->second.second, *solucao) ) &&
//      //   ( moveValidator->checkClassAndLessonDisponibility( aula, solucao->blocoAulas.find(&aula)->second.second, *solucao ) ) )
//      //{
//      //   for ( int h = 0; h < aula.getTotalCreditos();
//      //      ++h, ++itInicioHorariosAlocar )
//      //   {
//      //      if ( *itInicioHorariosAlocar )
//      //      {
//      //         std::cout << "<II> Sobreposicao de CREDITO" << std::endl;
//      //         exit(1);
//      //      }
//      //      else
//      //      {
//      //         // Alocação da aula
//      //         ( *itInicioHorariosAlocar ) = &aula;
//      //      }
//      //   }
//
//      //   //aula.bloco_aula = novosHorariosAula;
//      //   alocou = true;
//      //}
//   }
//
//   return alocou;
//}

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
      ITERA_GGROUP_LESSPTR( itProfessor, itCampus->professores, Professor )
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

         ITERA_GGROUP_LESSPTR( itMagisterio, itProfessor->magisterio, Magisterio )
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
