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
   ///* Armazena separadamente (seguindo os crit�rios estabelecidos abaixo) as fixa��es mais 
   //restritivas(pr�-processadas).*/

   //std::map<
   //   std::vector<int/*ID's : prof, disc, sala, dia*/>,
   //   GGroup<Fixacao*,LessPtr<Fixacao> > >

   //   horariosFixados;

   ///* Armazena o total de cr�ditos fixados (admitindo apenas as fixa��es que pertencem � estrutura
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
   //   /* Fazer o processamento para verificar se existe alguma aula que � fortemente fixada. Ou seja,
   //   se existe alguma aula que foi fixada para um total de hor�rios IGUAL ao total de cr�ditos que 
   //   ela disp�e. Assim, n�o h� como tentar trocar de hor�rio. 
   //   
   //   Caso exista alguma aula nessa situa��o, deve-se remov�-la da estrutura <aulas>. Em seguida,
   //   deve-se aloc�-la � solu��o nos hor�rios fixados. (Obs.: N�o � necess�rio preocupar com a estrutura
   //   <aulasNaoRelacionadasProf> a estrutura <horariosFixados> trata apenas de fixa��es que possuem o 
   //   campo Professor setado).
   //   */

   //   // Para cada conjunto de hor�rios fixados
   //   std::map<
   //      std::vector<int/*ID's : prof, disc, sala, dia*/>,
   //      GGroup<Fixacao*,LessPtr<Fixacao> > >::iterator

   //      itHorariosFixados = horariosFixados.begin();

   //   for(; itHorariosFixados != horariosFixados.end(); ++itHorariosFixados)
   //   {
   //      // Como todas as fixa��es s�o relacionadas a mesma disc., sala ...  n�o tem problema.
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

   //                  // Removendo a aula do grupo de aulas a serem alocadas pela fun��o de prioridade.
   //                  aulasNaoAlocadas.remove(itAula);

   //                  break; // Encontrei a aula, ent�o PARO.
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
   //   /* Apesar da fixa��o ter sido bastante restritiva, pode ser que o usu�rio tenha fixado mais hor�rios
   //   do que a disciplina precisa para poder alocar os cr�ditos. Tamb�m podem ter sido feitas outras fixa��es,
   //   ou nenhuma. Portanto, pode-se tentar alocar todas as aulas. */

   //   /* N�o deve-se fazer nada aqui (APENAS DEIXAR O COMENT�RIO =]). 
   //   A estrutura <aulas> est� pronta para ser utilizada no c�lculo da fun��o de prioridade. */
   ////}

   // ----------------------------------------------------------------------

   executaFuncaoPrioridade();

   // ----------------------------------------------------------------------
   // Inicializando a estrutura <custosAlocacaoAulaOrdenado> para poder auxiliar na aloca��o de aulas a professores.

   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
      custosAlocacaoAulaOrdenado.push_back( itCustoProfTurma->second );

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
   /* Tentando alocar as aulas seguindo a ordem dos custos que lhes foram associados. */

   std::vector< CustoAlocacao * >::iterator 
      itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();
   
   for(; itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
   {
      CustoAlocacao & custoAlocacao = **itCustosAlocacaoAulaOrdenado;
      Aula & aula = custoAlocacao.getAula();

      GGroup< Aula *, LessPtr< Aula > >::iterator 
         itAulasNaoAlocadas = aulasNaoAlocadas.find(&aula);

      // Somente se a aula do custo em quest�o n�o foi alocada.
      if (itAulasNaoAlocadas != aulasNaoAlocadas.end())
      {
         /* Teste simples para saber se uma aula que vamos tentar alocar j� possui hor�rios a serem alocados. */
         if(solIni->blocoAulas.find(&aula) != solIni->blocoAulas.end())
         {
            /* N�o pode tentar alocar a aula mais de uma vez na solu��o inicial. Se cair aqui � pq tem algum 
            erro de l�gica. Rever o algoritmo de gera��o da solu��o inicial. */
            std::cout << "\n\nERRO NA GERACAO DA SOLUCAO INICIAL: TENTANDO ALOCAR UMA AULA QUE JA FOI ALOCADA.\n\n";
            exit(1);
         }

         Professor & professor = custoAlocacao.getProfessor();

         /* Antes de tentar encontrar uma sequ�ncia de hor�rios livres do professor em quest�o deve-se
         checar se existe uma fixa��o que especifique o professor e, possivelmente a sala, o dia e os 
         hor�rios em que a aula deve ser ministrada.
         
         Toda vez que uma fixa��o que n�o permita nenhuma outra possibilidade de aloca��o da aula for 
         satisfeita, deve-se marcar a aula como FIXADA. Trata-se da fixa��o do Professor a uma aula em 
         um determinado dia nos hor�rios especificados.

         POR ENQUANTO, NENHUMA FIXA��O DO M�DULO OPERACIONAL SER� CONSIDERADA. -> M�RIO 19/05/2011
         */

         // Tentando todas as sequ�ncias de hor�rios vagos do professor.
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

               // Obtendo refer�ncias para os hor�rios de aula.
               std::vector<HorarioAula*> horariosAula ((itHA+hrIni),(itHA+hrFim));

               /* Agora que alguma sequ�ncia de hor�rios aula livres de um dado professor foi encontrada,
               deve-se checar se, al�m da sala para a qual a aula est� alocada possuir tais hor�rios, se esses 
               hor�rios est�o desocupados. Tamb�m h� necessidade de verificar se n�o vai acontecer algum conflito
               com as outras aulas do bloco curricular em quest�o, ou se um estamos tentando alocar um professor
               que j� est� ministrando alguma outra aula do bloco curricular.
               */

               //professorRepetido(professor,aula);

               /* Iterador utilizado nos testes abaixo. */
               std::vector<HorarioAula*>::iterator itHorariosAula;

               /* Checando se a sala possui os hor�rios de aula selecionados. */
               
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

               /* Checando se a sala est� livre no hor�rios indicados. */

               bool horariosAulaSalaLivres = false;

               //std::map< Sala *, GGroup< std::pair< int /*Dia Semana*/, HorarioAula * > >, LessPtr<Sala> >::iterator
               //   itHorariosAulaSala = horariosAulaSala.find(aula.getSala());

               std::map< Sala*, 
                  std::map< int /*Dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > >,
                  LessPtr<Sala> >::iterator 
                  
                  itHorariosAulaSala = horariosAulaSala.find(aula.getSala());

               if(itHorariosAulaSala == horariosAulaSala.end())
               { 
                  // Significa que n�o existe nenhum registro de hor�rio de aula alocado para a sala em quest�o.
                  horariosAulaSalaLivres = true;
               }
               else
               {
                  std::map< int /*Dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > >::iterator
                     itDiaHorariosAulaSala = itHorariosAulaSala->second.find(aula.getDiaSemana());

                  if(itDiaHorariosAulaSala == itHorariosAulaSala->second.end())
                  {
                     /* Embora exista registro de um, ou mais hor�rios alocados para a sala, para o dia 
                     em quest�o todos os hor�rios est�o livres. */
                     horariosAulaSalaLivres = true;
                  }
                  else
                  {
                     bool encontrouHorario = false;

                     itHorariosAula = horariosAula.begin();
                     for(; itHorariosAula != horariosAula.end(); ++itHorariosAula)
                     {
                        /* Basta que um hor�rio esteja ocupado para concluir que a sequ�ncia de hor�rios a ser analisada n�o � v�lida. */
                        if(itDiaHorariosAulaSala->second.find(*itHorariosAula) != itDiaHorariosAulaSala->second.end())
                        { encontrouHorario = true; break; }
                     }

                     if(!encontrouHorario)
                     { horariosAulaSalaLivres = true; }
                  }
               }

               if(!horariosAulaSalaLivres)
               { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; }

               /* Checando se trata-se de um professor repetido ou se h� conflito de bloco curricular. */

               if(moveValidator->checkBlockConflict(aula,horariosAula,*solIni) &&
                  moveValidator->checkClassAndLessonDisponibility(aula,horariosAula,*solIni))
               {
                  /* Procedimentos para alocar a aula. */

                  solIni->alocaAulaProf(aula,professor,horariosAula);

                  // Removendo a aula da rela��o de aulas n�o alocadas.
                  aulasNaoAlocadas.remove(itAulasNaoAlocadas);

                  /* Atualizando a estrutura <horariosAulaSala>. */
                  itHorariosAula = horariosAula.begin();
                  for(; itHorariosAula != horariosAula.end(); ++itHorariosAula)
                     horariosAulaSala[aula.getSala()][aula.getDiaSemana()].add(*itHorariosAula);
                  
                  break; // N�O DEVE mais buscar intervalos de hor�rios livres.
               }
            }
         }
      }
   }

   /* Mesmo que tenha-se utilizado todos os custos de aloca��o, pode acontecer o caso em que uma, ou mais aulas
   n�o tenham sido alocadas. */
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
//   // O objetivo aqui � iterar sobre os hor�rios letivos do professor,
//   // para o dia em quest�o at� que o primeiro hor�rio livre seja
//   // encontrado (pode ser que o professor n�o possua nenhum hor�rio
//   // livre). Uma vez encontrado o hor�rio livre, a ideia agora � obter,
//   // sequencialmente, o total de hor�rios livres demandados pela
//   // disciplina da aula em quest�o. S� ent�o uma aloca��o pode ser conclu�da.
//
//   bool sequenciaDeHorariosLivres = false;
//   std::vector< Aula * >::iterator itInicioHorariosAlocar = itHorariosProf;
//   int totalCredsAlocar = aula.getTotalCreditos();
//
//   // Marca a posi��o do vector de aulas do
//   // professor onde ser�o alocadas as aulas
//   int coluna_matriz = 0;
//   int id_horario_aula = 0;
//   HorarioAula * horario_aula = NULL;
//   Horario * horario = NULL;
//
//   for ( int indice_horario = 0; indice_horario < totalHorariosConsiderados;
//      ++indice_horario, ++itHorariosProf )
//   {
//      // Se a aula n�o for 'NULL'
//      if ( *itHorariosProf )
//      {
//         sequenciaDeHorariosLivres = false;
//         totalCredsAlocar = aula.getTotalCreditos();
//         continue;
//      }
//
//      // --------------------------------------------------------------------------------------
//      // Checando se o hor�rio em quest�o est� dispon�vel na sala.
//
//      // Garantido que vou sempre achar o horario aula id que procuro pq nunca
//      // procuro um horario que j� esteja preenchido. Portanto, n�o h� como acontecer
//      // um caso em que o hor�rio procurado n�o perten�a a sala.
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
//      // Caso tenha chegado at� o final e n�o encontrado o hor�rio aula
//      // dispon�vel na sala, ent�o n�o posso alocar a aula nessa sala e hor�rio.
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
//      // Se a sala j� possui algum registro de aloca��o.
//      if ( it_sala_horarios_alocados != sala_horarios_alocados.end() )
//      {
//         bool sala_ocupada = false;
//
//         // Verifica se a sala j� est� ocupada no
//         // hor�rio em que queremos alocar a aula atual
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
//            // Verifica se a aula est� ocupada no
//            // hor�rio de aula atual e no memso dia da semana
//            if ( pair_horario_dia.first == itHorarioSala->horario_aula
//               && pair_horario_dia.second == dia_semana )
//            {
//               sala_ocupada = true;
//               break;
//            }
//         }
//
//         // A sala est� alocada no hor�rio em quest�o
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
//      // aula est� tentando alocar est� dispon�vel
//      if ( totalCredsAlocar == aula.getTotalCreditos() )
//      {
//         itInicioHorariosAlocar = itHorariosProf;
//      }
//
//      // Se a disciplina possuir apenas um cr�dito
//      // para o dia em quest�o, basta aloc�-la.
//      if ( aula.getTotalCreditos() == 1 )
//      {
//         std::cout << "Saindo qdo uma aula tem apenas 1 horario para ser alocado. (MARIO)" << std::endl;
//         exit(1);
//
//         // Procura a coluna da aula para a
//         // qual o iterador est� apontando
//         std::vector< Aula * >::iterator it_aula
//            = solucao->getMatrizAulas()->at( idOperacionalProf )->begin();
//
//         coluna_matriz = ( std::distance( it_aula, itHorariosProf ) );
//         id_horario_aula = ( coluna_matriz % total_horarios );
//
//         // Recupera o objeto 'HorarioAula' do hor�rio atual
//         horario_aula = problemData.horarios_aula_ordenados[ id_horario_aula ];
//
//         GGroup< Horario * >::iterator it_horario = professor.horarios.begin();
//
//         for (; it_horario != professor.horarios.end();	it_horario++ )
//         {
//            horario = ( *it_horario );
//
//            // Checando se encontrei o hor�rio que estou buscando. 
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
//                     // Informa que a sala est� ocupada nesse dia da semana e nesse hor�rio de aula
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
//         // Caso a disciplina possua mais de um cr�dito
//         // a ser alocado, a ideia aqui � aloca-los em 
//         // sequ�ncia. Portanto, devo verificar se os
//         // cr�ditos demandados est�o livres.
//
//         // Atualizo o total de cr�ditos alocados.
//         --totalCredsAlocar;
//
//         if ( totalCredsAlocar == 0 )
//         {
//            sequenciaDeHorariosLivres = true;
//
//            // Posso parar a busca pq j�
//            // encontrei o total de cr�ditos necess�rios.
//            break;
//         }
//      }
//   }
//
//   // Se encontrei uma sequ�ncia de hor�rios livres, aloco.
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
//         // Procura a coluna da aula para a qual o iterador est� apontando.
//         std::vector< Aula * >::iterator it_aula
//            = solucao->getMatrizAulas()->at( idOperacionalProf )->begin();
//
//         coluna_matriz = ( std::distance( it_aula, itInicioHorariosAlocar ) );
//
//         id_horario_aula = ( coluna_matriz % total_horarios );
//
//         for ( int i = 0; i < aula.getTotalCreditos(); i++, id_horario_aula++ )
//         {
//            // Recupera o objeto 'HorarioAula' do hor�rio atual
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
//                  // Informa que a sala est� ocupada nesse dia da semana e nesse hor�rio de aula
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
//      //         // Aloca��o da aula
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
      ITERA_GGROUP_LESSPTR( itProfessor, itCampus->professores, Professor )
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

         ITERA_GGROUP_LESSPTR( itMagisterio, itProfessor->magisterio, Magisterio )
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
