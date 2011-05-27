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

   executaFuncaoPrioridade(**problemData.campi.begin(),problemData.campi.begin()->professores);

   // ----------------------------------------------------------------------
   // Limpando a estrutura de <aulasNaoAlocadas>.

   aulasNaoAlocadas.clear();

   // ----------------------------------------------------------------------
   // Separando os CustosAlocacao por nivelPrioridade.

   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
   {
      custoAlocacaoNiveisPrioridade[
         itCustoProfTurma->second->getNivelPrioridade()].add(
            itCustoProfTurma->second);
   }

   // ----------------------------------------------------------------------
   // Limpando a estrutura de <custoProfTurma>.

   custoProfTurma.clear();

   //----------------------------------------------------------------------

   moveValidator = new MoveValidator( &problemData );

   // ----------------------------------------------------------------------
}

SolucaoInicialOperacional::~SolucaoInicialOperacional()
{
   delete moveValidator;
}

SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial()
{
   // Alocando os CustoAlocacao por nivel de prioridade.
   for(unsigned nvPrd = custoAlocacaoNiveisPrioridade.size(); nvPrd > 0; --nvPrd)
   {
      std::map<unsigned /*Nivel de prioridade*/,
         GGroup< CustoAlocacao *, GreaterPtr<CustoAlocacao> > >::iterator

         itCstAlocNvPrd = custoAlocacaoNiveisPrioridade.find(nvPrd);

      if(itCstAlocNvPrd == custoAlocacaoNiveisPrioridade.end())
      { std::cout << "\n\nNivel de prioridade nao encontrado em SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial().\n\n"; exit(1); }

      // Inicializando a estrutura <aulasNaoAlocadas>.
      ITERA_GGROUP_GREATERPTR(itCustoAlocacaoNivelPrioridade,(itCstAlocNvPrd->second),CustoAlocacao)
      { aulasNaoAlocadas.add(&(itCustoAlocacaoNivelPrioridade->getAula())); }

      // Inicializando o GGroup de custos ordenados.
      custosAlocacaoAulaOrdenado = itCstAlocNvPrd->second;

      itCstAlocNvPrd->second.clear();

      /*
      Indica se é a primeira tentativa de alocação para o nível em questão.

      ********************

      Essa FLAG serve para sabermos o que se deve fazer quando estivermos iterando sobre 
      os custos de um dado nível de prioridade.

      Exemplo:

      Estamos iterando nos custos de alocação de algum nível de prioridade.

      Como todos os custos foram criados antes de iniciar o método. O algoritmo vai tentar alocar as aulas
      desse nível a professores reais. Até então, a FLAG não foi considerada.

      Se todos os custos forem analisados e, mesmo assim, ainda restarem aulas para alocar, faz-se o uso dessa
      FLAG. Se ela estiver marcada com TRUE, deve-se tentar alocar as aulas a professores virtuais que foram
      criados em iterações anteriores (Essa é a primeira tentativa de alocação DEPOIS de tentar os professores
      REAIS).  Se, msm assim ainda existirem aulas a serem alocadas, deve-se criar um novo professor virtual e 
      tentar alocar as aulas a ele. Repete-se esse último passo até que todos as aulas do nível em questão
      tenham sido alocadas.

      Pode acontecer (mais nas primeiras iterações) que nenhum professor virtual ainda tenha sido criado. Daí,
      testa-se para a primeira tentativa, se existe algum professor virtual criado. Se não existir, marca-se
      a FLAG com false e cria-se um novo professor virtual. Tenta-se então, alocar as aulas restantes.

      ********************

      */
      bool primeiraTentativaAlocacao = true;
      bool tentarManterViabilidade = true;

      alocaAulasRec(primeiraTentativaAlocacao,tentarManterViabilidade);

      // Pode ter restado algum custo durante a alocação.
      custosAlocacaoAulaOrdenado.clear();

      if(aulasNaoAlocadas.size() > 0)
      { std::cout << "\n\nA estrutura <aulasNaoAlocadas> deveria estar vazia em geraSolucaoInicial.\n\n"; exit(1); }
   }

   return * solIni;
}

void SolucaoInicialOperacional::alocaAulasRec(bool primeiraTentativaAlocacao,bool tentaV)
{
   bool tentarManterViabilidade = tentaV;

   ITERA_GGROUP_GREATERPTR(itCustosAlocacaoAulaOrdenado,custosAlocacaoAulaOrdenado,CustoAlocacao)
   {
      bool alocouAula = false;

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

         /* Checando se o professor selecionado já está ministrando alguma aula pertencente ao mesmo
         bloco curricular da aula em questão. */

         if(tentarManterViabilidade)
         { if(professorRepetido(professor,aula)) { continue; } }

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
         int hrFim = (aula.getTotalCreditos() - 1);

         while(hrFim < solIni->getTotalHorarios())
         {
            std::vector<int> seqHorarioAula;
            for(int hrAtual = hrIni; hrAtual <= hrFim; ++hrAtual)
               seqHorarioAula.push_back(hrAtual);

            if(solIni->seqHorarioLivre(professor.getIdOperacional(),aula.getDiaSemana(),seqHorarioAula))
            {
               std::vector<HorarioAula*>::iterator 
                  itHA = problemData.horarios_aula_ordenados.begin();

               // Obtendo referências para os horários de aula.
               std::vector<HorarioAula*> horariosAula ((itHA+hrIni),(itHA+hrFim+1));

               /* Agora que alguma sequência de horários aula livres de um dado professor foi encontrada,
               deve-se checar se, além da sala para a qual a aula está alocada possuir tais horários, se esses 
               horários estão desocupados. Também há necessidade de verificar se não vai acontecer algum conflito
               com as outras aulas do bloco curricular em questão.
               */

               /* Checando se a sala e a aula possuem, em comum, os horários demandados. Checando também se
               existe algum conflito em relação as outras aulas do bloco curricular da aula em questão. */

               if(moveValidator->checkClassAndLessonDisponibility(aula,horariosAula,*solIni))
               {
                  bool conflitoBloco = false;

                  if(tentarManterViabilidade)
                  { conflitoBloco = (moveValidator->checkBlockConflict(aula,horariosAula,*solIni)); }

                  if(!conflitoBloco)
                  {

                     /* Iterador utilizado nos testes abaixo. */
                     std::vector<HorarioAula*>::iterator itHorariosAula;

                     /* Checando se a sala está livre no horários indicados. */

                     bool horariosAulaSalaLivres = false;

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

                     /* Procedimentos para alocar a aula. */

                     // Alocando a aula na matriz solução.
                     solIni->alocaAulaProf(aula,professor,horariosAula);

                     // Removendo a aula da relação de aulas não alocadas.
                     aulasNaoAlocadas.remove(itAulasNaoAlocadas);

                     /* Atualizando a estrutura <horariosAulaSala>. */
                     itHorariosAula = horariosAula.begin();
                     for(; itHorariosAula != horariosAula.end(); ++itHorariosAula)
                        horariosAulaSala[aula.getSala()][aula.getDiaSemana()].add(*itHorariosAula);

                     /* Atualizando a estrutura <blocosProfs>. */
                     ITERA_GGROUP_LESSPTR( itOferta, aula.ofertas, Oferta )
                     {
                        // Descobrindo o bloco da oferta em questão.
                        BlocoCurricular * blocoCurricular = problemData.mapCursoDisciplina_BlocoCurricular
                           [ std::make_pair( itOferta->curso, aula.getDisciplina() ) ];

                        blocosProfs[blocoCurricular].add(&professor);
                     }

                     alocouAula = true;

                     break; // NÃO DEVE mais buscar intervalos de horários livres.

                  }
                  else
                  { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; } /* cmd continue desnecessário. Só para garantir que em futuras manutenções do código, nenhum comando seja executado após esse eles. */
               }
               else
               { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; } /* cmd continue desnecessário. Só para garantir que em futuras manutenções do código, nenhum comando seja executado após esse eles. */
            }
            else
            { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; } /* cmd continue desnecessário. Só para garantir que em futuras manutenções do código, nenhum comando seja executado após esse eles. */
         }

         /* Se não alocou a aula e era o último professor (sendo ele virtual) e todos os seus horários
         estava livres, parte-se para a geração de uma solução inicial com algumas inviabilidades. */
         if(!alocouAula)
         {
            if((solIni->getMatrizAulas()->size()-1) == professor.getIdOperacional())
            {
               bool agendaLivre = true;

               std::vector<Aula*>::iterator 
                  itHorariosDisponiveisProfessor = solIni->getMatrizAulas()->back()->begin();

               for(; itHorariosDisponiveisProfessor != solIni->getMatrizAulas()->back()->end(); ++itHorariosDisponiveisProfessor)
               {
                  if( (*itHorariosDisponiveisProfessor) )
                     if( !((*itHorariosDisponiveisProfessor)->eVirtual()) )
                     { agendaLivre = false; break; }
               }

               if(agendaLivre)
               { tentarManterViabilidade = false; }

               if(!tentarManterViabilidade)
               { std::cout << "\nGerando Solucao Inicial com alguma INVIABILIDADE.\n"; }
            }
         }
      }
   }

   /* Mesmo que tenha-se utilizado todos os custos de alocação, pode acontecer o caso em que uma, ou mais aulas
   não tenham sido alocadas. */
   if(aulasNaoAlocadas.size() > 0)
   {
      // Criando um GGroup para o novo professor.

      GGroup<Professor*,LessPtr<Professor> > professores;  

      if(primeiraTentativaAlocacao)
      {
         /* Pode ser que seja a primeira tentativa de alocação, mas que nenhum professor virtual
         tenha sido criado anteriormente. Então, deve-se criar um novo professor e tentar alocar
         as aulas que ainda restam. */

         if(problemData.professores_virtuais.empty())
         { professores.add( &addProfessor() ); }
         else
         {
            std::vector< Professor * >::iterator 
               itProfessoresVirtuais = problemData.professores_virtuais.begin();

            for(; itProfessoresVirtuais != problemData.professores_virtuais.end(); ++itProfessoresVirtuais)
            { professores.add(*itProfessoresVirtuais); }
         }

         primeiraTentativaAlocacao = false;
      }
      else
      { professores.add( &addProfessor() ); /* Não é a primeira tentativa, então, crio um novo professor. */ }

      /* 
      Agora que já temos um conjunto de professores, tenta-se alocar toda-as as aulas que 
      forem possíveis.

      Isso é realizado via execução recursiva da função de geração da solução inicial.
      */

      // Deletando os custos de alocação.

      ITERA_GGROUP_GREATERPTR(itCustosAlocacaoAulaOrdenado,custosAlocacaoAulaOrdenado,CustoAlocacao)
      { delete *itCustosAlocacaoAulaOrdenado; }

      // Limpando a estrutura <custosAlocacaoAulaOrdenado>

      custosAlocacaoAulaOrdenado.clear();

      // Limpando a estrutura <custoProfTurma>

      custoProfTurma.clear();

      // Função de prioridade.

      executaFuncaoPrioridade(**problemData.campi.begin(),professores);

      // Inicializando a estrutura <custosAlocacaoAulaOrdenado> para poder auxiliar na alocação de aulas a professores.

      std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
         itCustoProfTurma = custoProfTurma.begin();

      for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
         custosAlocacaoAulaOrdenado.add(itCustoProfTurma->second);

      /* Invocando recursivamente a função para geração da solução inicial. */
      alocaAulasRec(primeiraTentativaAlocacao,tentarManterViabilidade);
   }
}

Professor & SolucaoInicialOperacional::addProfessor()
{
   /* A ESTRUTURA <professores_virtuais> SO GUARDA UMA REFERENCIA DOS PROFESSORES VIRTUAIS CRIADOS.
   PARA DESCOBRIR A LINHA CORRESPONDENTE NA MATRIZ, BASTA USAR O GETIDOPERACIONAL. DEVE-SE TENTAR 
   ALOCAR AS AULAS PARA ELE E, SOMENTE SE FOR NECESSARIO, CRIAR OUTRO PROFESSOR VIRTUAL. */

   // Criando um professor virtual.

   Professor * novoProfessor = new Professor( true );

   // Setando alguns dados para o novo professor

   novoProfessor->tipo_contrato = ( *problemData.tipos_contrato.begin() );

   // Temporariamente vou setar os ids dos magistérios com valores negativos.
   int idMag = 0;

   ITERA_GGROUP_LESSPTR( itDisciplina, problemData.disciplinas, Disciplina )
   {
      Magisterio * mag = new Magisterio();

      mag->setId(--idMag);

      mag->disciplina = ( *itDisciplina );
      mag->setDisciplinaId( itDisciplina->getId() );
      mag->setNota( 10 );
      mag->setPreferencia( 1 );

      novoProfessor->magisterio.add( mag );
   }

   //ITERA_GGROUP_LESSPTR( itCampus, problemData.campi, Campus )
   {
      // Adicionando os dados do novo professor apenas ao PRIMEIRO CAMPUS listado.
      //itCampus->professores.add( novoProfessor );
      problemData.campi.begin()->professores.add( novoProfessor );

      // Copiando todos os horários do campus em questão para o professor.
      //ITERA_GGROUP( itHorario, itCampus->horarios, Horario )
      {
         //novoProfessor->horarios.add( *itHorario );
         novoProfessor->horarios = problemData.campi.begin()->horarios;
      }
   }

   // Criando a "agenda semanal" do novo professor
   std::vector< Aula * > * horariosNovoProfessor = new std::vector< Aula * >
      ( ( solIni->getTotalDias() * solIni->getTotalHorarios() ), NULL );

   /* Adicionando os dados do novo professor à solução.

   Essa função seta o id e o idOperacional do novo professor.
   */
   solIni->addProfessor( ( *novoProfessor ), ( *horariosNovoProfessor ) );

   problemData.professores_virtuais.push_back( novoProfessor );

   return *novoProfessor;
}

bool SolucaoInicialOperacional::professorRepetido( Professor & professor, Aula & aula )
{
   // Para cada oferta da aula
   ITERA_GGROUP_LESSPTR( itOferta, aula.ofertas, Oferta )
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

   return false;
}

void SolucaoInicialOperacional::executaFuncaoPrioridade(Campus & campus, GGroup<Professor*, LessPtr<Professor> > & professoresCP)
{
   //ITERA_GGROUP_LESSPTR( itCampus, problemData.campi, Campus )
   {
      //ITERA_GGROUP_LESSPTR( itProfessor, itCampus->professores, Professor )
      ITERA_GGROUP_LESSPTR( itProfessor, professoresCP, Professor )
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

         //itProfessor->setCustoDispProf( itCampus->horarios.size() );
         itProfessor->setCustoDispProf( campus.horarios.size() );

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
               //if ( itAula->ofertas.begin()->campus == ( *itCampus ) )
               if ( *itAula->ofertas.begin()->campus == campus )
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
                           //custo, itCampus->horarios.size() );
                           custo, campus.horarios.size() );
                     }
                  }
               }
               else
               {
                  std::cout << "Erro no calculo da funcao de prioridade." << std::endl;
                  exit(1);
               }
            }
         }
      }
   }
}

void SolucaoInicialOperacional::calculaCustoFixProf( Professor & prof , Aula & aula, unsigned idCusto, int custo, int maxHorariosCP )
{
   std::pair< Professor *, Aula * > chave ( & prof, & aula );
   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
      itCustoProfTurma = custoProfTurma.find( chave );

   // Criando, se necessário, um novo CustoAlocacao dada a chave em questão.
   if ( itCustoProfTurma == custoProfTurma.end() )
   {
      custoProfTurma[ chave ] = new CustoAlocacao( problemData, prof, aula );
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
