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

   executaFuncaoPrioridade();

   // ----------------------------------------------------------------------
   // Inicializando a estrutura <custosAlocacaoAulaOrdenado> para poder auxiliar na alocação de aulas a professores.

   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
      custosAlocacaoAulaOrdenado.push_back( itCustoProfTurma->second );

   // ----------------------------------------------------------------------
   /* Removendo os custos de alocação referentes a aulas que possuam fixações do tipo: professor, dia e horário.
   Ou seja, são fixações bastante restritivas que impedem qualquer tipo de movimento por parte das estruturas
   de vizinhança. */

   /* ToDo !!! Fazer a alocação aqui msm e marcar a aula como alocada !!! */

   // ----------------------------------------------------------------------
   // Ordenando os custos de alocação obtidos via execução da função de prioridade.

   std::sort( custosAlocacaoAulaOrdenado.begin(),custosAlocacaoAulaOrdenado.end(), ordenaCustosAlocacao );

   // ----------------------------------------------------------------------

   moveValidator = new MoveValidator( &problemData );

   // ----------------------------------------------------------------------
   /* Inicializando a estrutura que mantem referência para as aulas que não foram relacionadas (associadas)
   a nenhum professor. Caso existam aulas nessa situação, deve-se alocá-las à professores virtuais no final 
   do método.
   */

   ITERA_GGROUP( itAula, problemData.aulas, Aula )
   { 
      std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator
         itCustoProfTurma = custoProfTurma.begin();

      for (; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma )
      {
         if ( *(itCustoProfTurma->first.second->getDisciplina()) == *(itAula->getDisciplina()) )
            break;
      }

      if ( itCustoProfTurma == custoProfTurma.end() )
         aulasNaoRelacionadasProf.add( *itAula );
   }

   // ----------------------------------------------------------------------
}

SolucaoInicialOperacional::~SolucaoInicialOperacional()
{
   delete moveValidator;
}

SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial()
{
   SolucaoOperacional * solIni = new SolucaoOperacional( &problemData );
   return * solIni;
}
//{
//   GGroup< Aula * > aulasNaoAlocadas;
//
//   SolucaoOperacional * solucaoInicial = new SolucaoOperacional( &problemData );
//
//   int total_horarios = ( solucaoInicial->getTotalHorarios() );
//
//   // -----------------------------------------------------------
//   // Inicialmente, todas as aulas (que possuem, pelo
//   // menos, um professor associado) serao consideradas
//   // como não alocadas. Portanto, todas as aulas devem
//   // ser adicionadas a estrutura <aulasNaoAlocadas>
//   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * >::iterator
//      itCustoProfTurma = custoProfTurma.begin();
//
//   for(; itCustoProfTurma != custoProfTurma.end();
//      ++itCustoProfTurma )
//   {
//      aulasNaoAlocadas.add( itCustoProfTurma->first.second );
//   }
//
//   // -----------------------------------------------------------
//   // Enquanto a estrutura <custosAlocacaoAulaOrdenado> nao estiver vazia
//   while ( !custosAlocacaoAulaOrdenado.empty() )
//   {
//      std::vector< CustoAlocacao * >::iterator 
//         itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();
//
//      // Para cada custo
//      for(; itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end();
//         ++itCustosAlocacaoAulaOrdenado )
//      {
//         Aula & aula = ( *itCustosAlocacaoAulaOrdenado )->getAula();
//         GGroup< Aula * >::iterator itAulasNaoAlocadas = aulasNaoAlocadas.find( &(aula) );
//
//         // Somente se a aula do custo em questão não foi alocada.
//         if ( itAulasNaoAlocadas != aulasNaoAlocadas.end() )
//         {
//            Professor & professor = ( *itCustosAlocacaoAulaOrdenado )->getProfessor();
//
//            std::vector< Aula * >::iterator it_horarios_prof
//               = solucaoInicial->getItHorariosProf( professor, aula.getDiaSemana() );
//
//            // Recupera a posição do primeiro horário de aula
//            int coluna_matriz = std::distance(
//               solucaoInicial->getMatrizAulas()->at( professor.getIdOperacional() )->begin(), it_horarios_prof );
//
//            int horario_aula_id = ( coluna_matriz % solucaoInicial->getTotalHorarios() );
//
//            int dia_semana = ( coluna_matriz / solucaoInicial->getTotalHorarios() + 1 );
//
//            // Verifica se essa aula é fixada ao professor
//            bool possui_fixacao = possui_fixacao_professor_dia_horario(
//               professor, dia_semana, horario_aula_id );
//
//            bool alocouProfAula = false;
//            if( !professorRepetido( professor, aula ) )
//            {
//               alocouProfAula = alocaAulaSeq(
//                  solucaoInicial, it_horarios_prof, total_horarios, professor, aula );
//            }
//
//            if ( alocouProfAula )
//            {
//               // Informa que a aula alocada não poderá ser movida
//               if ( possui_fixacao )
//               {
//                  aula.setAulaFixada( true );
//               }
//
//               // -------------------------
//               // Atualizando a estrutura <blocosProfs>
//
//               // Para cada oferta da aula
//               ITERA_GGROUP( itOferta, aula.ofertas, Oferta )
//               {
//                  // Descobrindo o bloco da oferta em questão.
//                  BlocoCurricular * bc = problemData.mapCursoDisciplina_BlocoCurricular
//                     [ std::make_pair( itOferta->curso, aula.getDisciplina() ) ];
//
//                  blocosProfs[ bc ].add( &professor );
//               }
//               // -------------------------
//
//               std::cout << "\nForam alocados " << aula.getTotalCreditos()
//                  << " horarios CONSECUTIVOS para a aula da turma "
//                  << aula.getTurma() << " da disciplina "
//                  << aula.getDisciplina()->getCodigo()
//                  << " no dia " << aula.getDiaSemana()
//                  << " ao professor " << professor.getCpf() << std::endl;
//
//               // Para não tentar alocar esse custo novamente.
//               ( *itCustosAlocacaoAulaOrdenado ) = NULL;
//
//               // Para não tentar alocar essa aula novamente.
//               aulasNaoAlocadas.remove( itAulasNaoAlocadas );
//            }
//            else
//            {
//               std::cout << "\nTENTATIVA de alocacao de " << aula.getTotalCreditos()
//                  << " horarios CONSECUTIVOS para a aula da turma " << aula.getTurma()
//                  <<  " da disciplina " << aula.getDisciplina()->getCodigo()
//                  << " no dia " << aula.getDiaSemana()
//                  << " ao professor " << professor.getCpf()
//                  << " FRACASSOU." << std::endl;
//
//               // Para não tentar alocar esse custo novamente.
//               ( *itCustosAlocacaoAulaOrdenado ) = NULL;		
//            }
//         }
//         else
//         {
//            // Essa aula ja foi alocada. Portanto,
//            // devo remover esse custo sem aloca-lo.
//            ( *itCustosAlocacaoAulaOrdenado ) = NULL;
//         }
//      }
//
//      // Removendo os custos das aulas que foram alocadas na rodada atual.
//      for ( int p = ( custosAlocacaoAulaOrdenado.size() - 1 ); p >= 0; --p )
//      {
//         if ( custosAlocacaoAulaOrdenado.at( p ) == NULL )
//         {
//            custosAlocacaoAulaOrdenado.erase(
//               custosAlocacaoAulaOrdenado.begin() + p );
//         }
//      }
//   }
//
//   if ( aulasNaoAlocadas.size() > 0 )
//   {
//      // Aulas que não puderam ser alocadas a nenhum professor.
//      std::cout << "\nATENCAO: Alocando as aulas pendentes a professores virtuais."
//         << std::endl << std::endl;
//
//      // Estrutura responsável por
//      // referenciar os professores virtuais criados
//      this->problemData.professores_virtuais.clear();
//
//      // A ESTRUTURA INTERNA <professoresVirtuais> SO GUARDA UMA REFERENCIA
//      // DOS PROFESSORES VIRTUAIS CRIADOS. PARA DESCOBRIR A LINHA CORRESPONDENTE
//      // NA MATRIZ, BASTA USAR O GETIDOPERACIONAL. POR EQTO SO TEM 1 PROFESSOR
//      // VIRTUAL SENDO CRIADO. DEVE-SE TENTAR ALOCAR AS AULAS PARA ELE
//      // E, SOMENTE SE FOR NECESSARIO, CRIAR OUTRO PROFESSOR VIRTUAL.
//
//      // Criando o primeiro professor virtual.
//      // Criando a "agenda semanal" do novo professor
//      std::vector< Aula * > * horariosNovoProfessor = new std::vector< Aula * >
//         ( ( solucaoInicial->getTotalDias() * solucaoInicial->getTotalHorarios() ), NULL );
//
//      // Criando um professor virtual.
//      Professor * novoProfessor = new Professor( true );
//
//      // Setando alguns dados para o novo professor
//      novoProfessor->tipo_contrato = ( *problemData.tipos_contrato.begin() );
//
//      ITERA_GGROUP_LESSPTR( itDisciplina, problemData.disciplinas,
//         Disciplina )
//      {
//         Magisterio * mag = new Magisterio();
//
//         mag->disciplina = ( *itDisciplina );
//         mag->setDisciplinaId( itDisciplina->getId() );
//         mag->setNota( 10 );
//         mag->setPreferencia( 1 );
//
//         novoProfessor->magisterio.add( mag );
//      }
//
//      ITERA_GGROUP_LESSPTR( itCampus, problemData.campi, Campus )
//      {
//         ITERA_GGROUP( itHorario, itCampus->horarios,
//            Horario )
//         {
//            novoProfessor->horarios.add( *itHorario );
//         }
//      }
//
//      // Adicionando os horários do novo professor à solução.
//      solucaoInicial->addProfessor( ( *novoProfessor ), ( *horariosNovoProfessor ) );
//
//      // Adicionando o novo professor a todos os campus
//      ITERA_GGROUP_LESSPTR( itCampus, problemData.campi, Campus )
//      {
//         itCampus->professores.add( novoProfessor );
//      }
//
//      this->problemData.professores_virtuais.push_back( novoProfessor );
//
//      // Enquanto todas as aulas não forem alocadas
//      while ( aulasNaoAlocadas.size() > 0 )
//      {
//         Aula & aula = ( **aulasNaoAlocadas.begin() );
//         bool alocouAula = false;
//
//         // Obtendo um iterador para o primeiro professor virtual.
//         std::vector< Professor * >::iterator itProfessoresVirtuais
//            = this->problemData.professores_virtuais.begin();
//
//         // Para todos os professores virtuais existentes,
//         // tentar alocar as aulas que não puderam ser alocadas
//         // a professores reais.
//         // OBS.:
//         // Tenta-se alocar as aulas na ordem em que
//         // os professores virtuais estão sendo criados.
//
//         // Para cada professor virtual
//         for(; itProfessoresVirtuais != problemData.professores_virtuais.end();
//            ++itProfessoresVirtuais )
//         {
//            Professor & professor = ( **itProfessoresVirtuais );
//
//            std::vector< Aula * >::iterator it_horarios_prof
//               = solucaoInicial->getItHorariosProf( professor, aula.getDiaSemana() );
//
//            alocouAula = alocaAulaSeq( solucaoInicial, it_horarios_prof,
//               total_horarios, professor, aula );
//
//            if ( alocouAula )
//            {
//               std::cout << "\nForam alocados " << aula.getTotalCreditos()
//                  << " horarios CONSECUTIVOS para a aula da turma "
//                  << aula.getTurma() << " da disciplina "
//                  << aula.getDisciplina()->getCodigo()
//                  << " no dia " << aula.getDiaSemana()
//                  << " ao professor " << novoProfessor->getCpf()
//                  << std::endl;
//
//               aulasNaoAlocadas.remove( aulasNaoAlocadas.begin() );
//               break;
//            }
//         }
//
//         // Código relacionado à issue TRIEDA-882
//         if ( !alocouAula )
//         {
//            Professor * novoProfessor = NULL;
//
//            // Criando o primeiro professor virtual.
//            // Criando a "agenda semanal" do novo professor
//            std::vector< Aula * > * horariosNovoProfessor = new std::vector< Aula * >
//               ( (solucaoInicial->getTotalDias() * solucaoInicial->getTotalHorarios() ), NULL );
//
//            // Criando um professor virtual.
//            novoProfessor = new Professor( true );
//
//            // Setando alguns dados para o novo professor
//            novoProfessor->tipo_contrato = ( *problemData.tipos_contrato.begin() );
//
//            ITERA_GGROUP_LESSPTR( itDisciplina, problemData.disciplinas, Disciplina )
//            {
//               Magisterio * mag = new Magisterio();
//
//               mag->disciplina = ( *itDisciplina );
//               mag->setDisciplinaId( itDisciplina->getId() );
//               mag->setNota(10);
//               mag->setPreferencia(1);
//
//               novoProfessor->magisterio.add( mag );
//            }
//
//            ITERA_GGROUP_LESSPTR( itCampus, problemData.campi, Campus )
//            {
//               ITERA_GGROUP( itHorario, itCampus->horarios, Horario )
//               {
//                  novoProfessor->horarios.add( *itHorario );
//               }
//            }
//
//            // Adicionando os horários do novo professor à solução.
//            solucaoInicial->addProfessor( *novoProfessor, *horariosNovoProfessor );
//
//            // Adicionando o novo professor a todos os campus
//            ITERA_GGROUP_LESSPTR( itCampus, problemData.campi, Campus )
//            {
//               itCampus->professores.add( novoProfessor );
//            }
//
//            problemData.professores_virtuais.push_back( novoProfessor );
//
//            std::vector< Aula * >::iterator it_horarios_prof
//               = solucaoInicial->getItHorariosProf( *novoProfessor, aula.getDiaSemana() );
//
//            if ( !alocaAulaSeq( solucaoInicial, it_horarios_prof,
//               total_horarios, *novoProfessor, aula ) )
//            {
//               std::cout << "ERRO: Deveria ter alocado a "
//                  << "\naula ao novo professor criado."
//                  << std::endl;
//
//               exit(1);
//            }
//            else
//            { 
//               std::cout << "\nForam alocados " << aula.getTotalCreditos()
//                  << " horarios CONSECUTIVOS para a aula da turma "
//                  << aula.getTurma() << " da disciplina "
//                  << aula.getDisciplina()->getCodigo()
//                  << " no dia " << aula.getDiaSemana()
//                  << " ao professor " << novoProfessor->getCpf()
//                  << std::endl;
//
//               aulasNaoAlocadas.remove( aulasNaoAlocadas.begin() );
//            }
//         }
//      }
//   }
//
//   // Aulas, que nem sequer foram associadas a algum professor.
//   if ( aulasNaoRelacionadasProf.size() > 0 )
//   {
//      std::cout << "ATENCAO: Existem aulas sem professor associado, "
//         << "ou seja, nao foi calculado um custo para ela pq o "
//         << "usuario nao associou a disciplina da aula "
//         << "em questao a nenhum professor."
//         << std::endl;
//
//      exit(1);
//   }
//
//   return ( *solucaoInicial );
//}

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
   ITERA_GGROUP_LESSPTR(itFixacoes,problemData.fixacoes,Fixacao)
   {
      //if(*(itFixacoes->disciplina) == *aula.getDisciplina())


   }

   return false;
}

//TRIEDA-896
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

            // ToDo : Iterar sobre as aulas que não possuem a fixação mais restritiva.
            ITERA_GGROUP( itAula, problemData.aulas, Aula )
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
