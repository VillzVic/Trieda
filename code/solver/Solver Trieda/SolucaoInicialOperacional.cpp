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
      Indica se � a primeira tentativa de aloca��o para o n�vel em quest�o.

      ********************

      Essa FLAG serve para sabermos o que se deve fazer quando estivermos iterando sobre 
      os custos de um dado n�vel de prioridade.

      Exemplo:

      Estamos iterando nos custos de aloca��o de algum n�vel de prioridade.

      Como todos os custos foram criados antes de iniciar o m�todo. O algoritmo vai tentar alocar as aulas
      desse n�vel a professores reais. At� ent�o, a FLAG n�o foi considerada.

      Se todos os custos forem analisados e, mesmo assim, ainda restarem aulas para alocar, faz-se o uso dessa
      FLAG. Se ela estiver marcada com TRUE, deve-se tentar alocar as aulas a professores virtuais que foram
      criados em itera��es anteriores (Essa � a primeira tentativa de aloca��o DEPOIS de tentar os professores
      REAIS).  Se, msm assim ainda existirem aulas a serem alocadas, deve-se criar um novo professor virtual e 
      tentar alocar as aulas a ele. Repete-se esse �ltimo passo at� que todos as aulas do n�vel em quest�o
      tenham sido alocadas.

      Pode acontecer (mais nas primeiras itera��es) que nenhum professor virtual ainda tenha sido criado. Da�,
      testa-se para a primeira tentativa, se existe algum professor virtual criado. Se n�o existir, marca-se
      a FLAG com false e cria-se um novo professor virtual. Tenta-se ent�o, alocar as aulas restantes.

      ********************

      */
      bool primeiraTentativaAlocacao = true;
      bool tentarManterViabilidade = true;

      alocaAulasRec(primeiraTentativaAlocacao,tentarManterViabilidade);

      // Pode ter restado algum custo durante a aloca��o.
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

         /* Checando se o professor selecionado j� est� ministrando alguma aula pertencente ao mesmo
         bloco curricular da aula em quest�o. */

         if(tentarManterViabilidade)
         { if(professorRepetido(professor,aula)) { continue; } }

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

               // Obtendo refer�ncias para os hor�rios de aula.
               std::vector<HorarioAula*> horariosAula ((itHA+hrIni),(itHA+hrFim+1));

               /* Agora que alguma sequ�ncia de hor�rios aula livres de um dado professor foi encontrada,
               deve-se checar se, al�m da sala para a qual a aula est� alocada possuir tais hor�rios, se esses 
               hor�rios est�o desocupados. Tamb�m h� necessidade de verificar se n�o vai acontecer algum conflito
               com as outras aulas do bloco curricular em quest�o.
               */

               /* Checando se a sala e a aula possuem, em comum, os hor�rios demandados. Checando tamb�m se
               existe algum conflito em rela��o as outras aulas do bloco curricular da aula em quest�o. */

               if(moveValidator->checkClassAndLessonDisponibility(aula,horariosAula,*solIni))
               {
                  bool conflitoBloco = false;

                  if(tentarManterViabilidade)
                  { conflitoBloco = (moveValidator->checkBlockConflict(aula,horariosAula,*solIni)); }

                  if(!conflitoBloco)
                  {

                     /* Iterador utilizado nos testes abaixo. */
                     std::vector<HorarioAula*>::iterator itHorariosAula;

                     /* Checando se a sala est� livre no hor�rios indicados. */

                     bool horariosAulaSalaLivres = false;

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

                     /* Procedimentos para alocar a aula. */

                     // Alocando a aula na matriz solu��o.
                     solIni->alocaAulaProf(aula,professor,horariosAula);

                     // Removendo a aula da rela��o de aulas n�o alocadas.
                     aulasNaoAlocadas.remove(itAulasNaoAlocadas);

                     /* Atualizando a estrutura <horariosAulaSala>. */
                     itHorariosAula = horariosAula.begin();
                     for(; itHorariosAula != horariosAula.end(); ++itHorariosAula)
                        horariosAulaSala[aula.getSala()][aula.getDiaSemana()].add(*itHorariosAula);

                     /* Atualizando a estrutura <blocosProfs>. */
                     ITERA_GGROUP_LESSPTR( itOferta, aula.ofertas, Oferta )
                     {
                        // Descobrindo o bloco da oferta em quest�o.
                        BlocoCurricular * blocoCurricular = problemData.mapCursoDisciplina_BlocoCurricular
                           [ std::make_pair( itOferta->curso, aula.getDisciplina() ) ];

                        blocosProfs[blocoCurricular].add(&professor);
                     }

                     alocouAula = true;

                     break; // N�O DEVE mais buscar intervalos de hor�rios livres.

                  }
                  else
                  { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; } /* cmd continue desnecess�rio. S� para garantir que em futuras manuten��es do c�digo, nenhum comando seja executado ap�s esse eles. */
               }
               else
               { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; } /* cmd continue desnecess�rio. S� para garantir que em futuras manuten��es do c�digo, nenhum comando seja executado ap�s esse eles. */
            }
            else
            { /*Atualizar hrIni e hrFim.*/ ++hrIni; ++hrFim; continue; } /* cmd continue desnecess�rio. S� para garantir que em futuras manuten��es do c�digo, nenhum comando seja executado ap�s esse eles. */
         }

         /* Se n�o alocou a aula e era o �ltimo professor (sendo ele virtual) e todos os seus hor�rios
         estava livres, parte-se para a gera��o de uma solu��o inicial com algumas inviabilidades. */
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

   /* Mesmo que tenha-se utilizado todos os custos de aloca��o, pode acontecer o caso em que uma, ou mais aulas
   n�o tenham sido alocadas. */
   if(aulasNaoAlocadas.size() > 0)
   {
      // Criando um GGroup para o novo professor.

      GGroup<Professor*,LessPtr<Professor> > professores;  

      if(primeiraTentativaAlocacao)
      {
         /* Pode ser que seja a primeira tentativa de aloca��o, mas que nenhum professor virtual
         tenha sido criado anteriormente. Ent�o, deve-se criar um novo professor e tentar alocar
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
      { professores.add( &addProfessor() ); /* N�o � a primeira tentativa, ent�o, crio um novo professor. */ }

      /* 
      Agora que j� temos um conjunto de professores, tenta-se alocar toda-as as aulas que 
      forem poss�veis.

      Isso � realizado via execu��o recursiva da fun��o de gera��o da solu��o inicial.
      */

      // Deletando os custos de aloca��o.

      ITERA_GGROUP_GREATERPTR(itCustosAlocacaoAulaOrdenado,custosAlocacaoAulaOrdenado,CustoAlocacao)
      { delete *itCustosAlocacaoAulaOrdenado; }

      // Limpando a estrutura <custosAlocacaoAulaOrdenado>

      custosAlocacaoAulaOrdenado.clear();

      // Limpando a estrutura <custoProfTurma>

      custoProfTurma.clear();

      // Fun��o de prioridade.

      executaFuncaoPrioridade(**problemData.campi.begin(),professores);

      // Inicializando a estrutura <custosAlocacaoAulaOrdenado> para poder auxiliar na aloca��o de aulas a professores.

      std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
         itCustoProfTurma = custoProfTurma.begin();

      for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
         custosAlocacaoAulaOrdenado.add(itCustoProfTurma->second);

      /* Invocando recursivamente a fun��o para gera��o da solu��o inicial. */
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

   // Temporariamente vou setar os ids dos magist�rios com valores negativos.
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

      // Copiando todos os hor�rios do campus em quest�o para o professor.
      //ITERA_GGROUP( itHorario, itCampus->horarios, Horario )
      {
         //novoProfessor->horarios.add( *itHorario );
         novoProfessor->horarios = problemData.campi.begin()->horarios;
      }
   }

   // Criando a "agenda semanal" do novo professor
   std::vector< Aula * > * horariosNovoProfessor = new std::vector< Aula * >
      ( ( solIni->getTotalDias() * solIni->getTotalHorarios() ), NULL );

   /* Adicionando os dados do novo professor � solu��o.

   Essa fun��o seta o id e o idOperacional do novo professor.
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

   return false;
}

void SolucaoInicialOperacional::executaFuncaoPrioridade(Campus & campus, GGroup<Professor*, LessPtr<Professor> > & professoresCP)
{
   //ITERA_GGROUP_LESSPTR( itCampus, problemData.campi, Campus )
   {
      //ITERA_GGROUP_LESSPTR( itProfessor, itCampus->professores, Professor )
      ITERA_GGROUP_LESSPTR( itProfessor, professoresCP, Professor )
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

         //itProfessor->setCustoDispProf( itCampus->horarios.size() );
         itProfessor->setCustoDispProf( campus.horarios.size() );

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
               //if ( itAula->ofertas.begin()->campus == ( *itCampus ) )
               if ( *itAula->ofertas.begin()->campus == campus )
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

   // Criando, se necess�rio, um novo CustoAlocacao dada a chave em quest�o.
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
