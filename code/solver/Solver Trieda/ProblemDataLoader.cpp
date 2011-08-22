#include "ProblemDataLoader.h"
#include "ProblemSolution.h"
#include "TRIEDA-InputXSD.h"
#include "GGroup.h"

#include <iostream>

ProblemDataLoader::ProblemDataLoader( char *inputFile, ProblemData* data )
{
   this->inputFile = inputFile;
   this->problemData  = data;
}

ProblemDataLoader::~ProblemDataLoader()
{
}

void ProblemDataLoader::load()
{
   std::cout << "Loading file..." << std::endl;

   root = std::auto_ptr< TriedaInput >( TriedaInput_( inputFile, xml_schema::flags::dont_validate ) );

   std::cout << "Extracting data..." << std::endl;

   problemData->le_arvore( *root );

   std::cout << "Some preprocessing..." << std::endl;

   // ---------
   geraRefsOfertasDemandas();

   // ---------
   referenciaDisciplinasEquivalentesIncompativeis();

   // ---------
   referenciaDisciplinasCurriculos();

   // ---------
   relacionaHorariosAulaDiaSemana();

   // ---------
   relacionaCredsRegras();

   // ---------
   carregaDiasLetivosCampusUnidadeSala();

   // ---------
   carregaDiasLetivosDiscs();

   // ---------
   disciplinasCursosCompativeis();

   // ---------
   // relacionaDisciplinasEquivalentes();

   // ---------
   divideDisciplinas();

   // ---------
   validaInputSolucaoTatico();

   // ---------
   referenciaCampusUnidadesSalas();

   // ---------
   referenciaDisciplinas();

   // ---------
   referenciaOfertas();

   // ---------
   gera_refs();

   // ---------
   calculaDemandas();

   // ---------
   relacionaCursosCampus();

   // ---------
   calculaTamanhoMedioSalasCampus();

   // ---------
   relacionaCampusDiscs();

   // ---------
   relacionaDiscOfertas();

   // ---------
   estima_turmas();

   // ---------
   cria_blocos_curriculares();

   // ---------
   associaDisciplinasSalas();

   // ---------
   criaConjuntoSalasUnidade();

   // ---------
   associaDisciplinasConjuntoSalas();

   // ---------
   cache();

   // ---------
   estabeleceDiasLetivosBlocoCampus();

   // ---------
   estabeleceDiasLetivosDisciplinasSalas();

   // ---------
   estabeleceDiasLetivosDiscCjtSala();

   // ---------
   calculaCredsLivresSalas();

   // ---------
   combinacaoDivCreditos();

   // --------- 
   estabeleceDiasLetivosProfessorDisciplina();

   // --------- 
   criaAulas();

   // ---------
   calculaMaxHorariosProfessor();

   // ---------
   criaListaHorariosOrdenados();

   // ---------
   verificaFixacoesDiasLetivosDisciplinas();

   // ---------
   relacionaFixacoes();

   geraHorariosDia();

   // ---------
   print_stats();
}

void ProblemDataLoader::geraRefsOfertasDemandas()
{
   ITERA_GGROUP_LESSPTR( it_oferta, problemData->ofertas, Oferta )
   {
      find_and_set_lessptr( it_oferta->getCursoId(),
         problemData->cursos,
         it_oferta->curso, false );

      find_and_set_lessptr( it_oferta->getCurriculoId(),
         it_oferta->curso->curriculos,
         it_oferta->curriculo, false );

      find_and_set_lessptr( it_oferta->getTurnoId(),
         problemData->todos_turnos,
         it_oferta->turno, false );

      find_and_set_lessptr( it_oferta->getCampusId(),
         problemData->campi,
         it_oferta->campus, false );
   }

   ITERA_GGROUP_LESSPTR( it_dem, problemData->demandas, Demanda ) 
   {
      find_and_set_lessptr( it_dem->getOfertaId(),
         problemData->ofertas,
         it_dem->oferta, false );

      find_and_set_lessptr( it_dem->getDisciplinaId(),
         problemData->disciplinas,
         it_dem->disciplina, false );
   }
}

void ProblemDataLoader::criaFixacoesDisciplinasDivididas()
{
   Fixacao * fixacao = NULL;
   Disciplina * disciplina_pratica = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      // Verifica se a disciplina é prática
      if ( it_disciplina->getId() > 0 )
      {
         continue;
      }

      disciplina_pratica = ( *it_disciplina );

      // Procura se existe uma fixação
      // correspondente à disciplina teórica 
      ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
      {
         fixacao = ( *it_fixacao );

         if ( fixacao->disciplina != NULL
            && abs( fixacao->disciplina->getId() ) == abs( disciplina_pratica->getId() ) )
         {
            // Cria fixação com os mesmos dados, porém com 'id' negativo
            Fixacao * fixacao_disciplina_pratica = new Fixacao( *fixacao );

            // Atualiza as referência de disciplina para a disciplina prática
            fixacao_disciplina_pratica->setDisciplinaId( disciplina_pratica->getId() );
            fixacao_disciplina_pratica->disciplina = ( disciplina_pratica );

            // Assim como temos dois objetos 'disciplina', agora temos
            // dois objetos 'fixação', para os créditos teóricos e práticos
            problemData->fixacoes.add( fixacao_disciplina_pratica );
         }
      }
   }
}

bool ordena_horarios_aula( HorarioAula * h1, HorarioAula * h2 )
{
   if ( h1 == NULL && h2 == NULL )
   {
      return false;
   }
   else if ( h1 != NULL && h2 == NULL )
   {
      return false;
   }
   else if ( h1 == NULL && h2 != NULL )
   {
      return true;
   }

   return ( h1->getInicio() < h2->getInicio() );
}

void ProblemDataLoader::criaListaHorariosOrdenados()
{
   GGroup< HorarioAula * > horarios_aula;

   // Adiciona os horários de aula, sem repetição
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_professor, it_campi->professores, Professor )
      {
         ITERA_GGROUP( it_horario, it_professor->horarios, Horario )
         {
            horarios_aula.add( it_horario->horario_aula );
         }
      }
   }

   // Insere os horarios de aula (distintos) no vector
   ITERA_GGROUP( it_h, horarios_aula, HorarioAula )
   {
      problemData->horarios_aula_ordenados.push_back( *it_h );
   }

   // Ordena os horarios de aula pelo inicio de cada um
   std::sort( problemData->horarios_aula_ordenados.begin(),
      problemData->horarios_aula_ordenados.end(), ordena_horarios_aula );
}

void ProblemDataLoader::calculaMaxHorariosProfessor()
{
   int temp = 0;
   int totalAtual = 0;

   ITERA_GGROUP_LESSPTR( it_campi,problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_professor, it_campi->professores, Professor )
      {
         temp = it_professor->horarios.size();
         if ( temp > totalAtual )
         {
            totalAtual = temp;
         }
      }
   }

   problemData->max_horarios_professor = totalAtual;
}

void ProblemDataLoader::relacionaCursosCampus()
{
   Campus * campus = NULL;
   Curso * curso = NULL;
   Oferta * oferta = NULL;
   ITERA_GGROUP_LESSPTR( it_oferta, problemData->ofertas, Oferta )
   {
      // Recupera o objeto 'oferta'
      oferta = ( *it_oferta );

      // Recupera o objeto 'campus' dessa 'oferta'
      campus = it_oferta->campus;

      // Recupera o objeto 'curso' dessa 'oferta'
      curso = it_oferta->curso;

      // Relaciona o 'curso' com o 'campus'
      if ( campus != NULL && curso != NULL )
      {
         campus->cursos.add( curso );
         campus->ofertas.add( oferta );
      }
      else
      {
         std::cout << "\nProblemDataLoader::relacionaCursosCampus()"
            << "\nFoi encontrada uma oferta que não possui"
            << "\ncampus e/ou curso valido(s)." << std::endl;

         exit(1);
      }
   }
}

void ProblemDataLoader::referenciaDisciplinasEquivalentesIncompativeis()
{
   Disciplina * disciplina = NULL;
   Disciplina * disc_equivalente = NULL;
   Disciplina * disc_incompativel = NULL;

   // Para cada disciplina, procuramos pelas referencias de suas
   // disciplinas equivalentes e incompatíveis
   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disc );

      ITERA_GGROUP_LESSPTR( it_disc_aux, problemData->disciplinas, Disciplina )
      {
         if ( it_disc_aux->getId() == it_disc->getId() )
         {
            continue;
         }

         // Disciplinas equivalentes
         ITERA_GGROUP_N_PT( id_disc, disciplina->ids_disciplinas_equivalentes, int )
         {
            if ( it_disc_aux->getId() == ( *id_disc ) )
            {
               disc_equivalente = ( *it_disc_aux );
               disciplina->discEquivalentes.add( disc_equivalente );
               break;
            }
         }

         // Disciplinas incompatíveis
         ITERA_GGROUP_N_PT( id_disc, disciplina->ids_disciplinas_incompativeis, int )
         {
            if ( it_disc_aux->getId() == ( *id_disc ) )
            {
               disc_incompativel = ( *it_disc_aux );
               disciplina->discIncompativeis.add( disc_incompativel );
               break;
            }
         }
      }
   }
}

void ProblemDataLoader::referenciaDisciplinasCurriculos()
{
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
      {
         curriculo = ( *it_curriculo );
         curriculo->refDisciplinaPeriodo( problemData->disciplinas );
      }
   }
}

void ProblemDataLoader::relacionaHorariosAulaDiaSemana()
{
   ITERA_GGROUP_LESSPTR( it_turno, this->problemData->todos_turnos, Turno )
   {
      ITERA_GGROUP( it_horario_aula, it_turno->horarios_aula, HorarioAula )
      {
         ITERA_GGROUP_N_PT( it_dia_semana, it_horario_aula->dias_semana, int )
         {
            // Recupera o conjunto de horários de aula do dia da semana atual
            GGroup< HorarioAula *, LessPtr< HorarioAula > > * horarios_dia
               = &( problemData->horarios_aula_dia_semana[ ( *it_dia_semana ) ] );

            // Adiciona o horário de aula atual no
            // conjunto de horários de aula do dia da semana
            horarios_dia->add( ( *it_horario_aula ) );
         }
      }
   }
}

void ProblemDataLoader::relacionaCredsRegras()
{
   ITERA_GGROUP( it_Regra, problemData->regras_div, DivisaoCreditos )
   { 
      problemData->creds_Regras[ it_Regra->getCreditos() ].add( *it_Regra );
   }
}

void ProblemDataLoader::carregaDiasLetivosCampusUnidadeSala()
{
   // Para cada Campus
   ITERA_GGROUP_LESSPTR( it_Campus, problemData->campi, Campus )
   {
      // --------------------------------------------------------
      // Adicionando os dias letivos de cada Campus
      ITERA_GGROUP( it_Horario, it_Campus->horarios, Horario )
      {
         ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Horario->dias_semana, int )
         {
            it_Campus->diasLetivos.add( *it_Dias_Letivos );
         }
      }
      // --------------------------------------------------------

      // Para cada Unidade
      ITERA_GGROUP_LESSPTR( it_Unidade, it_Campus->unidades, Unidade )
      {
         // --------------------------------------------------------
         // Adicionando os dias letivos de cada Unidade
         ITERA_GGROUP( it_Horario, it_Unidade->horarios, Horario )
         {
            ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Horario->dias_semana, int )
            {
               it_Unidade->dias_letivos.add( *it_Dias_Letivos );
            }
         }

         // --------------------------------------------------------
         // Para cada Sala
         ITERA_GGROUP_LESSPTR( it_Sala, it_Unidade->salas, Sala )
         {
            // --------------------------------------------------------
            // Adicionando os dias letivos de cada Sala
            ITERA_GGROUP( it_Horario, it_Unidade->horarios, Horario )
            {
               if(problemData->parametros->modo_otimizacao == "TATICO")
               {
                  ITERA_GGROUP( it_Creds_Disp, it_Sala->creditos_disponiveis, CreditoDisponivel)
                  {
                     if ( it_Creds_Disp->getMaxCreditos() > 0 )
                     {
                        it_Sala->diasLetivos.add( it_Creds_Disp->getDiaSemana() );
                     }
                  }
               }
               else if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
               {
                  ITERA_GGROUP ( itHorarioDisponivel, it_Sala->horarios_disponiveis, Horario )
                  {
                     it_Sala->diasLetivos.add( itHorarioDisponivel->dias_semana );
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::carregaDiasLetivosDiscs()
{
   ITERA_GGROUP_LESSPTR( it_Disc, problemData->disciplinas, Disciplina )
   {
      ITERA_GGROUP( it_Horario, it_Disc->horarios, Horario )
      {
         ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Horario->dias_semana, int )
         { 
            it_Disc->diasLetivos.add( *it_Dias_Letivos );
         }
      }
   }
}

void ProblemDataLoader::criaConjuntoSalasUnidade()
{
   problemData->totalConjuntosSalas = 0;
   ITERA_GGROUP_LESSPTR( it_Campus, problemData->campi, Campus )
   {
      int idCjtSala = 1;
      ITERA_GGROUP_LESSPTR( it_Unidade, it_Campus->unidades, Unidade )
      {
         // Conjunto de salas (LABORATORIOS) que tiveram de
         // ser criadas, dado que a possuiam pelo menos 
         // uma disciplina com a FLAG <eLab> marcada (TRUE)
         GGroup< ConjuntoSala * > conjunto_Salas_Disc_eLab;

         // Conjunto de salas (SALAS OU LABORATORIOS) que
         // foram criadas sendo que não possuiam nenhuma 
         // disciplina com a FLAG <eLab> marcada (TRUE)
         GGroup< ConjuntoSala * > conjunto_Salas_Disc_GERAL;

         ITERA_GGROUP_LESSPTR( it_Sala, it_Unidade->salas, Sala )
         {
            bool exige_Conjunto_Individual = false;

            // Checando se a sala em questão exige
            // a criação de um Conjunto de Salas só
            // pra ela. Ex.: Qdo uma sala, na verdade,
            // um laboratório possui pelo menos uma 
            // disciplina com a FLAG <eLab> marcada (TRUE).

            if ( it_Sala->getTipoSalaId() == 2 )
            {
               ITERA_GGROUP( it_Disc,
                  it_Sala->disciplinasAssociadas, Disciplina )
               {
                  // Procurando por, pelo menos, uma
                  // disciplina que possua a FLAG <eLab> marcada (TRUE)
                  if ( it_Disc->eLab() )
                  {
                     exige_Conjunto_Individual = true;
                     break;
                  }
               }
            }

            // Referência para algum dos GGroup de conjuntos de
            // salas (<conjunto_Salas_Disc_eLab> ou  <conjunto_Salas_Disc_GERAL>).
            GGroup< ConjuntoSala * > * gg_Cjt_Salas_Esc = &( conjunto_Salas_Disc_eLab );

            // Teste para escolher qual estrutura de dados
            // (<conjunto_Salas_Disc_eLab> ou  <conjunto_Salas_Disc_GERAL>) deve-se utilizar.
            if ( !exige_Conjunto_Individual )
            {
               gg_Cjt_Salas_Esc = &( conjunto_Salas_Disc_GERAL );
            }

            // Antes de criar um novo conjunto de salas (labs ou SA),
            // deve-se procurar por algum conjunto de salas existente
            // que represente a capacidade e o tipo da sala em questão.
            // Além disso, a diferença das disciplinas associadas de ambos 
            // tem que ser nula (ou seja, todas as disciplinas que forem
            // associadas a sala em questão, tem de estar associadas ao
            // conjunto de salas encontrado, e vice versa).
            bool encontrou_Conjunto_Compat = false;
            ITERA_GGROUP( it_Cjt_Salas_Disc, ( *gg_Cjt_Salas_Esc ), ConjuntoSala )
            {
               // Se o conjunto de salas em questão representa a capacidade
               // da sala em questão. Estou testando tb se o conjunto de
               // salas representa o mesmo tipo da sala em questão. Acredito
               // que não seja necessário no caso em que estejamos lidando
               // APENAS com laboratórios. Como não sei qual GGroup está
               // sendo referenciado, testo os 2 casos pra lá.
               if (	   it_Cjt_Salas_Disc->getCapacidadeRepr() == it_Sala->getCapacidade()
                  && it_Cjt_Salas_Disc->getTipoSalasRepr() == it_Sala->tipo_sala->getId() )
               {
                  // Checando se tem o mesmo tamanho.
                  bool mesmas_Disciplinas_Associadas = true;
                  if ( it_Cjt_Salas_Disc->disciplinas_associadas.size()
                     == it_Sala->disciplinasAssociadas.size() )
                  {
                     // Iterando sobre as disciplinas associadas da sala em questão.
                     ITERA_GGROUP( it_Disc_Assoc_Sala,
                        it_Sala->disciplinasAssociadas, Disciplina )
                     {
                        // Se encontrei alguma disciplina que não está
                        // associada ao conjunto de salas e  à sala em
                        // questão, paro o teste de compatibilidade
                        // entre a sala e o cjt em questão.
                        if ( it_Cjt_Salas_Disc->disciplinas_associadas.find( *it_Disc_Assoc_Sala )
                           == it_Cjt_Salas_Disc->disciplinas_associadas.end() )
                        {
                           mesmas_Disciplinas_Associadas = false;
                           break;
                        }
                     }
                  }
                  else
                  {
                     mesmas_Disciplinas_Associadas = false;
                  }

                  if ( mesmas_Disciplinas_Associadas )
                  {
                     it_Cjt_Salas_Disc->addSala( **it_Sala );

                     // Adicionando os dias letivos ao conjunto de salas
                     ITERA_GGROUP( it_disc, it_Sala->disciplinasAssociadas, Disciplina )
                     {
                        GGroup< int > dias_fixados = retorna_foxacoes_dias_letivos( *it_disc );

                        if ( dias_fixados.size() == 0 )
                        {
                           ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Sala->diasLetivos, int )
                           {
                              it_Cjt_Salas_Disc->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
                           }
                        }
                        else
                        {
                           ITERA_GGROUP_N_PT( it_Dias_Letivos, dias_fixados, int )
                           {
                              it_Cjt_Salas_Disc->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
                           }
                        }
                     }

                     // COMO AS DISCIPLINAS ASSOCIADAS SÃO AS MESMAS, NÃO HÁ NECESSIDADE DE 
                     // ADICIONAR NENHUMA DISICIPLINA ASSOCIADA AO CONJUNTO DE SALAS EM QUESTÃO.
                     encontrou_Conjunto_Compat = true;
                     break;
                  }
               }
            }

            if ( !encontrou_Conjunto_Compat )
            {
               ConjuntoSala * cjt_Sala = new ConjuntoSala();

               cjt_Sala->setId( idCjtSala );
               cjt_Sala->setCapacidadeRepr( it_Sala->getCapacidade() );
               cjt_Sala->setTipoSalasRepr( it_Sala->getTipoSalaId() );

               cjt_Sala->addSala( **it_Sala );

               // Atualizando para o próximo id.
               ++idCjtSala;

               // Adicionando os dias letivos ao conjunto de salas
               ITERA_GGROUP( it_disc, it_Sala->disciplinasAssociadas, Disciplina )
               {
                  GGroup< int > dias_fixados = retorna_foxacoes_dias_letivos( *it_disc );

                  if ( dias_fixados.size() == 0 )
                  {
                     ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Sala->diasLetivos, int )
                     {
                        cjt_Sala->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
                     }
                  }
                  else
                  {
                     ITERA_GGROUP_N_PT( it_Dias_Letivos, dias_fixados, int )
                     {
                        cjt_Sala->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
                     }
                  }
               }

               // Associando as disciplinas ao conjunto.
               ITERA_GGROUP( it_Disc_Assoc_Sala,
                  it_Sala->disciplinasAssociadas, Disciplina )
               {
                  cjt_Sala->associaDisciplina( **it_Disc_Assoc_Sala );
               }

               // Adicionando ao respectivo conjunto.
               gg_Cjt_Salas_Esc->add( cjt_Sala );
            }
         }

         // AGORA QUE TENHO TODOS OS CONJUNTOS DE SALAS
         // CRIADOS, TENHO QUE ARMAZENA-LOS NA ESTRUTURA
         // <conjuntoSala> da Unidade em questão.

         ITERA_GGROUP( it_Cjt_Salas_Disc_Elab,
            conjunto_Salas_Disc_eLab, ConjuntoSala )
         {
            it_Unidade->conjutoSalas.add( *it_Cjt_Salas_Disc_Elab );
         }

         ITERA_GGROUP( it_Cjt_Salas_Disc_GERAL,
            conjunto_Salas_Disc_GERAL, ConjuntoSala )
         {
            it_Unidade->conjutoSalas.add( *it_Cjt_Salas_Disc_GERAL );
         }

         // ----------------------------
         std::cout << "Cod. Und.: " << it_Unidade->getCodigo() << std::endl;

         ITERA_GGROUP_LESSPTR( it_Cjt_Salas_Und,
            it_Unidade->conjutoSalas, ConjuntoSala )
         {
            std::cout << "\tCod. Cjt. Sala: "
               << it_Cjt_Salas_Und->getId() << std::endl;

            std::map< int /*Id Sala*/, Sala * >::iterator 
               it_Salas_Cjt = it_Cjt_Salas_Und->salas.begin();

            for(; it_Salas_Cjt != it_Cjt_Salas_Und->salas.end();
               it_Salas_Cjt++ )
            {
               std::cout << "\t\tCod. Sala: "
                  << it_Salas_Cjt->second->getCodigo()
                  << std::endl;
            }
         }
         // ----------------------------
      }
   }
}

GGroup< int > ProblemDataLoader::retorna_foxacoes_dias_letivos( Disciplina * disciplina )
{
   GGroup< int > dias_letivos;
   dias_letivos.clear();

   ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
   {
      if ( it_fixacao->disciplina != NULL
         && abs( it_fixacao->disciplina->getId() ) == abs( disciplina->getId() )
         && it_fixacao->getDiaSemana() >= 0 )
      {
         dias_letivos.add( it_fixacao->getDiaSemana() );
      }
   }

   return dias_letivos;
}

void ProblemDataLoader::estabeleceDiasLetivosBlocoCampus()
{
   // Analisar esse metodo e o de criacao de blocos curriculares.
   // Um bloco pode pertencer a mais de um campus !?
   ITERA_GGROUP_LESSPTR( it_Bloco_Curric, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_N_PT( it_Dia_Letivo, it_Bloco_Curric->diasLetivos, int )
      {
         if ( it_Bloco_Curric->campus->diasLetivos.find
            (*it_Dia_Letivo) != it_Bloco_Curric->campus->diasLetivos.end() )
         {
            problemData->bloco_Campus_Dias
               [ std::make_pair( it_Bloco_Curric->getId(),
                                 it_Bloco_Curric->campus->getId() ) ].add( *it_Dia_Letivo );
         }
         else
         {
            // PS: Quando tiver mais de um campus, pode acontecer que
            // uma associação entre um bloco curricular  que não pertence
            // a um determinado campus seja criada. Arrumar isso depois.
            // Ou seja, essa checagem só serve para quando se tem 1 campus.
            // Se tiver mais de um, quando cair aqui, nada pode-se afirmar
            // sobre a corretude da instância.
            std::cerr << "ERRO: Bloco Curricular e Campus Incompativeis. "
               << "(ProblemDataLoader::estabeleceDiasLetivosBlocoCampus())"
               << std::endl;

            exit(1);
         }
      }
   }
}

void ProblemDataLoader::estabeleceDiasLetivosDisciplinasSalas()
{
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itSala, itUnidade->salas, Sala )
         {
            ITERA_GGROUP( itDiscAssoc, itSala->disciplinasAssociadas, Disciplina )
            {
               ITERA_GGROUP_N_PT( itDiasLetivosDisc, itDiscAssoc->diasLetivos, int )
               {
                  // Se o dia letivo da disciplina é também um
                  // dia letivo da sala em questão, adiciona-se
                  // ao map <disc_Salas_Dias> o dia em comum.
                  if ( itSala->diasLetivos.find( *itDiasLetivosDisc )
                     != itSala->diasLetivos.end() )
                  {
                     std::pair< int, int > ids_Disc_Sala 
                        ( itDiscAssoc->getId(), itSala->getId() );

                     problemData->disc_Salas_Dias[ ids_Disc_Sala ].add( *itDiasLetivosDisc );

                     if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
                     {
                        // Adicionando informações referentes aos horários
                        // comuns entre uma sala e uma disciplina para um dado dia.
                        ITERA_GGROUP( itHorarioSala, itSala->horarios_disponiveis, Horario )
                        {
                           // Checando o dia em questão para a sala
                           if ( itHorarioSala->dias_semana.find( *itDiasLetivosDisc ) !=
                              itHorarioSala->dias_semana.end() )
                           {
                              ITERA_GGROUP( itHorarioDisc, itDiscAssoc->horarios, Horario )
                              {
                                 // Checando o dia em questão para a disciplina
                                 if ( itHorarioDisc->dias_semana.find( *itDiasLetivosDisc ) !=
                                    itHorarioDisc->dias_semana.end() )
                                 {
                                    // Checando se é um horário comum entre a disc e a sala.
                                    if ( itHorarioSala->horario_aula == itHorarioDisc->horario_aula )
                                    {
                                       problemData->disc_Salas_Dias_HorariosAula
                                          [ ids_Disc_Sala ][*itDiasLetivosDisc].add(
                                          itHorarioSala->horario_aula );

                                       break;
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::estabeleceDiasLetivosDiscCjtSala()
{
   // Os dias letivos das disciplinas em relação aos
   // conjuntos de salas são obtidos via união dos
   // dias letivos das disciplinas em relação às salas
   // pertencentes ao conjunto de salas em questão.

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         // p tds conjuntos de salas de um campus
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            // p tds as discAssoc de um conjunto
            ITERA_GGROUP_LESSPTR( itDiscAssoc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               std::map<int/*Id Sala*/,Sala*>::iterator itSala =
                  itCjtSala->salas.begin();

               for(; itSala != itCjtSala->salas.end();
                  itSala++ )
               {
                  std::pair< int /*idDisc*/, int /*idSala*/> ids_Disc_Sala 
                     ( itDiscAssoc->getId(), itSala->second->getId() );

                  // Se a disciplina se relaciona com a sala em questao.
                  // Como estamos  lidando com um conjunto de salas,
                  // podemos ter o caso em que uma disciplina
                  // é associada a uma sala do conjunto e a outra não.
                  if ( problemData->disc_Salas_Dias.find(ids_Disc_Sala) !=
                     problemData->disc_Salas_Dias.end() )
                  {
                     ITERA_GGROUP_N_PT( itDiasLetDisc,
                        problemData->disc_Salas_Dias[ ids_Disc_Sala ], int )
                     {
                        problemData->disc_Conjutno_Salas__Dias[ std::make_pair< int, int >
                           ( itDiscAssoc->getId(), itCjtSala->getId() ) ].add( *itDiasLetDisc );
                     }
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::calculaCredsLivresSalas()
{
   //ITERA_GGROUP( itCampus, problemData->campi, Campus )
   //{
   //   ITERA_GGROUP( itUnidade, itCampus->unidades, Unidade )
   //   {
   //      ITERA_GGROUP( itSala, itUnidade->salas, Sala )
   //      {
   //         for ( int dia = 0; dia < 8; dia++ )
   //         {
   //	itSala->credsLivres.push_back(0);
   //}

   //         ITERA_GGROUP( itCredsDisp, itSala->creditos_disponiveis, CreditoDisponivel )
   //         {
   //	itSala->credsLivres.at( itCredsDisp->dia_semana ) = itCredsDisp->max_creditos;
   //}
   //      }
   //   }
   //}
}

void ProblemDataLoader::estabeleceDiasLetivosProfessorDisciplina()
{
   problemData->disc_Dias_Prof_Tatico.clear();
   problemData->usarProfDispDiscTatico = false;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      // TODO: Tem que ser para os blocos do campus em questao !!!!
      ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
      {
         ITERA_GGROUP_LESSPTR( itDisc, itBloco->disciplinas, Disciplina )
         {
            ITERA_GGROUP_LESSPTR( it_prof, itCampus->professores, Professor )
            {							  
               ITERA_GGROUP_LESSPTR( it_mag, it_prof->magisterio, Magisterio )
               {
                  ITERA_GGROUP( it_hor, it_prof->horarios, Horario )
                  {
                     GGroup< int >::iterator itDiasLetDisc =
                        itDisc->diasLetivos.begin();

                     for(; itDiasLetDisc != itDisc->diasLetivos.end();
                        itDiasLetDisc++ )
                     {			
                        if ( it_mag->getDisciplinaId() == itDisc->getId() )
                        {
                           if ( it_hor->dias_semana.find( *itDiasLetDisc )
                              != it_hor->dias_semana.end() )
                           {
                              std::pair< int, int > ids_Prof_Disc 
                                 (it_prof->getId(), itDisc->getId());

                              problemData->prof_Disc_Dias[ ids_Prof_Disc ].add( *itDiasLetDisc );
                              problemData->disc_Dias_Prof_Tatico[itDisc->getId()].add( *itDiasLetDisc );
                              problemData->usarProfDispDiscTatico = true;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::combinacaoDivCreditos()
{
   std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > > combinacao_divisao_creditos; 
   std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > vAux; 
   std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > vec; 
   std::pair< int /*dia*/, int/*numCreditos*/ > p;
   bool atualiza = false;

   ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
   {
      if ( itDisc->divisao_creditos != NULL )
      {
         std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > vAux; 
         for ( int i = 1; i <= 7; i++ )
         {
            p = std::make_pair( i, itDisc->divisao_creditos->dia[i] );
            vAux.push_back( p );
         }

         // Verifica se a regra de divisão de créditos é válida
         for ( int a = 0; a < 7; a++ )
         {
            if ( vAux[a].second != 0 )
            {
               GGroup< int >::iterator itDiasLetivosDiscs
                  = itDisc->diasLetivos.begin();
               for(; itDiasLetivosDiscs != itDisc->diasLetivos.end();
                  itDiasLetivosDiscs++)
               { 
                  if ( vAux[a].first == ( *itDiasLetivosDiscs ) )
                  {
                     atualiza = true;
                     break;
                  }
                  else
                  {
                     atualiza = false;
                  }
               }
            }
         }

         if ( atualiza )
         {
            itDisc->combinacao_divisao_creditos.push_back( vAux );
            atualiza = false;
         }

         // Para cada regra de divisão de creditos pode existir mais 6
         for ( int k = 0; k < 6; k++ )
         {
            for ( int j = 0; j < 7; j++ )
            {
               if ( j == 0 )
               {
                  p = std::make_pair( vAux[0].first, vAux[6].second );
               }
               else
               {
                  p = std::make_pair( vAux[j].first, vAux[j-1].second );
               }

               vec.push_back( p );
            }

            vAux.clear();
            vAux = vec;
            vec.clear();

            // Verifica se as regras de divisão de créditos criadas são válidas
            for ( int b = 0; b < 7; b++ )
            {
               if ( vAux[b].second != 0 )
               {
                  GGroup< int >::iterator itDiasLetivosDiscs
                     = itDisc->diasLetivos.begin();
                  for(; itDiasLetivosDiscs != itDisc->diasLetivos.end();
                     itDiasLetivosDiscs++ )
                  { 
                     if ( vAux[b].first == ( *itDiasLetivosDiscs ) )
                     {
                        atualiza = true;
                        break;
                     }
                     else
                     {
                        atualiza = false;
                     }
                  }
               }
            }

            if ( atualiza )
            {
               itDisc->combinacao_divisao_creditos.push_back( vAux );
               atualiza = false;
            }	
         }
      }
   }
}

void ProblemDataLoader::disciplinasCursosCompativeis()
{
   // Definindo um map de compatibilidade e incompatibilidade entre 2 turmas.
   bool compativel = problemData->parametros->permite_compartilhamento_turma_sel;

   if ( compativel )
   {
      ITERA_GGROUP_LESSPTR( it_fix_curso, problemData->cursos, Curso )
      {
         ITERA_GGROUP_LESSPTR( it_alt_curso, problemData->cursos, Curso )
         {
            std::pair< Curso *, Curso * > idCursos = 
               std::make_pair( ( *it_fix_curso ), ( *it_alt_curso ) );

            problemData->compat_cursos[ idCursos ] = true;
         }
      }
   }

   ITERA_GGROUP_LESSPTR( it_fix_curso, problemData->cursos, Curso )
   {
      GGroup< GGroup< int > * >::iterator it_list_compat =
         problemData->parametros->permite_compart_turma.begin();

      for (; it_list_compat != problemData->parametros->permite_compart_turma.end();
         it_list_compat++ )
      {
         if ( it_list_compat->find( it_fix_curso->getId() ) != it_list_compat->end() )
         {
            ITERA_GGROUP_LESSPTR( it_alt_curso, problemData->cursos, Curso )
            {
               if ( it_list_compat->find( it_alt_curso->getId() ) != it_list_compat->end() )
               {
                  std::pair< Curso *, Curso * > idCursos =
                     std::make_pair( ( *it_fix_curso ), ( *it_alt_curso ) );

                  problemData->compat_cursos.erase( idCursos );
               }
            }
         }
      }
   }

   std::map< std::pair< Curso *, Curso * >, bool >::iterator itCC = 
      problemData->compat_cursos.begin();
   for (; itCC != problemData->compat_cursos.end(); itCC++ )
   {
      std::pair< Curso *, Curso * > normal = 
         std::make_pair< Curso *, Curso * > ( itCC->first.first, itCC->first.second );

      std::pair< Curso *, Curso * > invertido =
         std::make_pair< Curso *, Curso * > ( itCC->first.second, itCC->first.first );

      problemData->compat_cursos[ invertido ] = 
         problemData->compat_cursos.find( normal )->second;
   }
}

bool ProblemDataLoader::contemFixacao(
                                      GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes,
                                      Professor * professor, Disciplina * disciplina,
                                      Sala * sala, int dia_semana, HorarioAula * horario_aula )
{
   Fixacao * fixacao = NULL;

   ITERA_GGROUP_LESSPTR( it_fixacao, fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );

      //----------------------------------------------------------------
      if ( professor != NULL )
      {
         if ( fixacao->professor == NULL
            || professor->getId() != fixacao->professor->getId() )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      //----------------------------------------------------------------
      if ( disciplina != NULL )
      {
         if ( fixacao->disciplina == NULL
            || disciplina->getId() != fixacao->disciplina->getId() )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      //----------------------------------------------------------------
      if ( sala != NULL )
      {
         if ( fixacao->sala == NULL
            || sala->getId() != fixacao->sala->getId() )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      //----------------------------------------------------------------
      if ( dia_semana >= 0 )
      {
         if ( fixacao->getDiaSemana() < 0
            || dia_semana != fixacao->getDiaSemana() )
         {
            fixacao->setDiaSemana( dia_semana );
         }
      }
      //----------------------------------------------------------------

      //----------------------------------------------------------------
      if ( horario_aula != NULL )
      {
         if ( fixacao->horario_aula == NULL
            || horario_aula->getId() != fixacao->horario_aula->getId() )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      // Chegando aqui, verificamos que a fixação foi encontrada
      return true;
   }

   return false;
}

bool ProblemDataLoader::contemFixacaoExato(
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes,
   Professor * professor, Disciplina * disciplina,
   Sala * sala, int dia_semana, HorarioAula * horario_aula )
{
   Fixacao * fixacao = NULL;

   ITERA_GGROUP_LESSPTR( it_fixacao, fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );

      //----------------------------------------------------------------
      if ( professor != NULL || fixacao->professor != NULL )
      {
         if ( ( professor != NULL && fixacao->professor == NULL )
            || ( professor == NULL && fixacao->professor != NULL )
            || ( professor->getId() != fixacao->professor->getId() ) )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      //----------------------------------------------------------------
      if ( disciplina != NULL || fixacao->disciplina != NULL )
      {
         if ( ( disciplina != NULL && fixacao->disciplina == NULL )
            || ( disciplina == NULL && fixacao->disciplina != NULL )
            || ( disciplina->getId() != fixacao->disciplina->getId() ) )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      //----------------------------------------------------------------
      if ( sala != NULL || fixacao->sala != NULL )
      {
         if ( ( sala != NULL && fixacao->sala == NULL )
            || ( sala == NULL && fixacao->sala != NULL )
            || ( sala->getId() != fixacao->sala->getId() ) )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      //----------------------------------------------------------------
      if ( dia_semana >= 0 || fixacao->getDiaSemana() >= 0 )
      {
         if ( ( dia_semana >= 0 && fixacao->getDiaSemana() < 0 )
            || ( dia_semana < 0 && fixacao->getDiaSemana() >= 0 )
            || ( dia_semana != fixacao->getDiaSemana() ) )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      //----------------------------------------------------------------
      if ( horario_aula != NULL || fixacao->horario_aula != NULL )
      {
         if ( ( horario_aula != NULL && fixacao->horario_aula == NULL )
            || ( horario_aula == NULL && fixacao->horario_aula != NULL )
            || ( horario_aula->getId() != fixacao->horario_aula->getId() ) )
         {
            continue;
         }
      }
      //----------------------------------------------------------------

      // Chegando aqui, verificamos que a fixação foi encontrada
      return true;
   }

   return false;
}

Fixacao * ProblemDataLoader::criaFixacao(
   int id_fixacao, Professor * professor, Disciplina * disciplina,
   Sala * sala, int dia_semana, HorarioAula * horario_aula )
{
   Fixacao * fixacao = new Fixacao();
   fixacao->setId( id_fixacao );

   if ( professor != NULL )
   {
      fixacao->setProfessorId( professor->getId() );
      fixacao->professor = professor;
   }

   if ( disciplina != NULL )
   {
      fixacao->setDisciplinaId( disciplina->getId() );
      fixacao->disciplina = disciplina;
   }

   if ( sala != NULL )
   {
      fixacao->setSalaId( sala->getId() );
      fixacao->sala = sala;
   }

   if ( dia_semana >= 0 )
   {
      fixacao->setDiaSemana( dia_semana );
   }

   if ( horario_aula != NULL )
   {
      fixacao->setHorarioAulaId( horario_aula->getId() );
      fixacao->horario_aula = horario_aula;
   }

   return fixacao;
}

// Realiza a separação da fixações por tipo de fixação
void ProblemDataLoader::relacionaFixacoes()
{
   GGroup< Fixacao *, LessPtr< Fixacao > > novas_fixacoes;

   int id_fixacao = -1;
   ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
   {
      if ( it_fixacao->getId() > id_fixacao )
      {
         id_fixacao = it_fixacao->getId();
      }
   }
   id_fixacao++;

   ITERA_GGROUP_LESSPTR( it_fixacao, this->problemData->fixacoes, Fixacao )
   {
      Fixacao * fixacao = ( *it_fixacao );

      //--------------------------------------------------------------------------
      // TÁTICO
      // Apenas disciplina/sala
      if ( fixacao->disciplina != NULL && fixacao->sala != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Disc_Sala,
            NULL, fixacao->disciplina, fixacao->sala, -1, NULL ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, NULL, fixacao->disciplina, fixacao->sala, -1, NULL );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Disc_Sala.add( nova_fixacao );
         }
      }
      // Apenas disciplina/sala/dia/horario
      if ( fixacao->disciplina != NULL && fixacao->sala != NULL
         && fixacao->getDiaSemana() > 0 && fixacao->horario_aula != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Disc_Sala_Dia_Horario,
            NULL, fixacao->disciplina, fixacao->sala, fixacao->getDiaSemana(), fixacao->horario_aula ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, NULL, fixacao->disciplina,
               fixacao->sala, fixacao->getDiaSemana(), fixacao->horario_aula );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Disc_Sala_Dia_Horario.add( nova_fixacao );
         }
      }
      // Apenas disciplina/dia/horario
      if ( fixacao->disciplina != NULL && fixacao->getDiaSemana() > 0
         && fixacao->horario_aula != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Disc_Dia_Horario,
            NULL, fixacao->disciplina, NULL, fixacao->getDiaSemana(), fixacao->horario_aula ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, NULL, fixacao->disciplina,
               NULL, fixacao->getDiaSemana(), fixacao->horario_aula );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Disc_Dia_Horario.add( nova_fixacao );
         }
      }
      //--------------------------------------------------------------------------
      // OPERACIONAL
      // professor/disciplina/sala/dia/horário
      if ( fixacao->professor != NULL && fixacao->disciplina != NULL
         && fixacao->sala != NULL && fixacao->getDiaSemana() > 0
         && fixacao->horario_aula != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Disc_Sala_Dia_Horario,
            fixacao->professor, fixacao->disciplina, fixacao->sala, fixacao->getDiaSemana(), fixacao->horario_aula ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, fixacao->disciplina,
               fixacao->sala, fixacao->getDiaSemana(), fixacao->horario_aula );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Disc_Sala_Dia_Horario.add( nova_fixacao );
         }
      }
      // professor/disciplina/dia/horário
      if ( fixacao->professor != NULL && fixacao->disciplina != NULL
         && fixacao->getDiaSemana() > 0 && fixacao->horario_aula != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Disc_Dia_Horario,
            fixacao->professor, fixacao->disciplina, NULL, fixacao->getDiaSemana(), fixacao->horario_aula ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, fixacao->disciplina,
               NULL, fixacao->getDiaSemana(), fixacao->horario_aula );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Disc_Dia_Horario.add( nova_fixacao );
         }
      }
      // professor/disciplina
      if ( fixacao->professor != NULL && fixacao->disciplina != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Disc,
            fixacao->professor, fixacao->disciplina, NULL, -1, NULL ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, fixacao->disciplina, NULL, -1, NULL );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Disc.add( nova_fixacao );
         }
      }
      // professor/disciplina/sala
      if ( fixacao->professor != NULL && fixacao->disciplina != NULL
         && fixacao->sala != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Disc_Sala, fixacao->professor,
            fixacao->disciplina, fixacao->sala, -1, NULL ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, fixacao->disciplina, fixacao->sala, -1, NULL );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Disc_Sala.add( nova_fixacao );
         }
      }
      // professor/sala
      if ( fixacao->professor != NULL && fixacao->sala != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Sala,
            fixacao->professor, NULL, fixacao->sala, -1, NULL ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, NULL, fixacao->sala, -1, NULL );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Sala.add( nova_fixacao );
         }
      }
      //--------------------------------------------------------------------------

      //--------------------------------------------------------------------------
      // Adiciona mais um crédito fixado da
      // disciplina atual no dia da semana correspondente
      if ( fixacao->disciplina != NULL && fixacao->getDiaSemana() >= 0 )
      {
         std::pair< Disciplina *, int > disciplina_dia
            = std::make_pair( fixacao->disciplina, fixacao->getDiaSemana() );

         this->problemData->map_Discicplina_DiaSemana_CreditosFixados[ disciplina_dia ]++;
      }

      // Informa a fixação entre o par disciplina/sala
      if ( fixacao->disciplina != NULL && fixacao->sala != NULL )
      {
         this->problemData->map_Discicplina_Sala_Fixados[ fixacao->disciplina ] = fixacao->sala;
      }

      // Preenche a estrutura <fixacoesProfDisc>.
      if ( fixacao->professor && fixacao->disciplina )
      {
         std::pair< Professor *, Disciplina * > chave = std::make_pair(
            fixacao->professor, fixacao->disciplina );

         problemData->fixacoesProfDisc[ chave ].add( fixacao );
      }
      //--------------------------------------------------------------------------
   }

   // Adiciona todas as restrições criadas acima no conjunto de fixações
   ITERA_GGROUP_LESSPTR( it_fixacao, novas_fixacoes, Fixacao )
   {
      if ( !contemFixacaoExato( problemData->fixacoes, it_fixacao->professor, it_fixacao->disciplina,
         it_fixacao->sala, it_fixacao->getDiaSemana(), it_fixacao->horario_aula ) )
      {
         problemData->fixacoes.add( ( *it_fixacao ) );
      }
   }
}

// Quando uma disciplina tiver um dia da semana fixado, devo
// remover os demais dias da semana do seu conjunto de dias letivos
void ProblemDataLoader::verificaFixacoesDiasLetivosDisciplinas()
{
   Fixacao * fixacao = NULL;
   Disciplina * disciplina = NULL;
   ConjuntoSala * conjunto_sala = NULL;
   int dia_semana = 0;
   bool encontrou_disciplina = false;

   ITERA_GGROUP_LESSPTR( it_fixacao, this->problemData->fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );
      disciplina = ( fixacao->disciplina );
      dia_semana = ( fixacao->getDiaSemana() );

      // Verifica se o dia da semana e a disciplina foram fixados
      if ( disciplina != NULL && dia_semana >= 0 )
      {
         ////////////////////////////////////////////////////////////////////////////
         // Como a disciplina atual possui um dia da semana fixado,
         // procuro co conjunto de sala ao qual ela está relacionada
         encontrou_disciplina = false;
         GGroup< Campus *, LessPtr< Campus > >::iterator itCampus
            = problemData->campi.begin();
         for (; itCampus != problemData->campi.end() && !encontrou_disciplina;
            itCampus++ )
         {
            GGroup< Unidade *, LessPtr< Unidade > >::iterator itUnidade
               = itCampus->unidades.begin();
            for (; itUnidade != itCampus->unidades.end() && !encontrou_disciplina;
               itUnidade++ )
            {
               GGroup< ConjuntoSala *, LessPtr< ConjuntoSala > >::iterator itCjtSala
                  = itUnidade->conjutoSalas.begin();
               for (; itCjtSala != itUnidade->conjutoSalas.end() && !encontrou_disciplina;
                  itCjtSala++ )
               {
                  conjunto_sala = ( *itCjtSala );

                  std::map< Disciplina *, GGroup< int > >::iterator
                     it_disc_dias = conjunto_sala->dias_letivos_disciplinas.begin();
                  for (; it_disc_dias != conjunto_sala->dias_letivos_disciplinas.end();
                     it_disc_dias++ )
                  {
                     // Procura pela disciplina da fixação no conjunto de salas
                     if ( it_disc_dias->first->getId() == disciplina->getId() )
                     {
                        // Deixa apenas o dia letivo fixado no
                        // conjunto de dias letivos da disciplina
                        GGroup< int > * dias_letivos = &( it_disc_dias->second );
                        dias_letivos->clear();
                        dias_letivos->add( dia_semana );
                        encontrou_disciplina = true;
                        break;
                     }
                  }
               }
            }
         }
         ////////////////////////////////////////////////////////////////////////////
      }
   }
}

template< class T > 
void ProblemDataLoader::find_and_set( int id, GGroup< T * > & haystack, 
                                     T * & needle, bool print = false )
{
   T * finder = new T;
   finder->setId( id );

   // Versão lenta... Entender o porquê depois

#ifndef WIN32
   typename GGroup< T * >::iterator it_g = haystack.begin();
#else
   GGroup< T * >::iterator it_g = haystack.begin();
#endif

   while ( it_g != haystack.end()
      && it_g->getId() != finder->getId() )
   {
      ++it_g;
   }
   // FIM

   if ( it_g != haystack.end() )
   {
      needle = ( *it_g );

      if ( print )
      {
         std::cout << "Found " << id << std::endl;
      }
   }
   else
   {
      std::cout << "Warnning: Problema na funcao"
         << "FindAndSet do ProblemDataLoader." << std::endl;

      exit(1);
   }

   delete finder;
}

template< class T > 
void ProblemDataLoader::find_and_set_lessptr( int id, GGroup< T *, LessPtr< T > > & haystack, 
                                             T * & needle, bool print = false )
{
   T * finder = new T;
   finder->setId( id );

   // Versão lenta... Entender o porquê depois
#ifndef WIN32
   typename GGroup< T *, LessPtr< T > >::iterator it_g = haystack.begin();
#else
   GGroup< T *, LessPtr< T > >::iterator it_g = haystack.begin();
#endif
   while ( it_g != haystack.end()
      && it_g->getId() != finder->getId() )
   {
      ++it_g;
   }
   // FIM

   if ( it_g != haystack.end() )
   {
      needle = ( *it_g );

      if ( print )
      {
         std::cout << "Found " << id << std::endl;
      }
   }
   else
   {
      std::cout << "Warnning: Problema na funcao"
         << "FindAndSet do ProblemDataLoader." << std::endl;

      exit(1);
   }

   delete finder;
}

void ProblemDataLoader::substituiDisciplinasEquivalentes()
{
   Curso * curso = NULL;
   Curso * curso_comp = NULL;

   Curriculo * curriculo = NULL;
   Curriculo * curriculo_comp = NULL;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_comp = NULL;

   bool mesmo_curso = false;
   bool mesmo_curriculo = false;
   bool atualizar_demandas = false;

   // Devo percorrer as disciplinas de todos os curriculos de cada curso,
   // e verificar se há a necessidade de substituição por outras disciplinas.
   // Essa verificação é feita para cada par de cursos (incluindo a verificação
   // deum curso com ele mesmo) e para cada curriculo do par de cursos (não
   // devendo verificar as disciplinas de um mesmo curriculo)
   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      curso = ( *it_curso );

      ITERA_GGROUP_LESSPTR( it_curso_comp, problemData->cursos, Curso )
      {
         curso_comp = ( *it_curso_comp );
         mesmo_curso = ( curso->getId() == curso_comp->getId() );

         // Verifica a compatibilidade entre os cursos
         if ( !problemData->cursosCompativeis( curso, curso_comp ) )
         {
            continue;
         }

         // Chegando aqui, temos que os cursos são compatíveis
         ITERA_GGROUP_LESSPTR( it_curriculo, curso->curriculos, Curriculo )
         {
            curriculo = ( *it_curriculo );

            ITERA_GGROUP_LESSPTR( it_curriculo_comp, curso_comp->curriculos, Curriculo )
            {
               curriculo_comp = ( *it_curriculo_comp );
               mesmo_curriculo = ( curriculo->getId() == curriculo_comp->getId() );

               // Não devo substituir disciplinas do mesmo curso e currículo
               if ( mesmo_curso && mesmo_curriculo )
               {
                  continue;
               }

               GGroup< std::pair< int, Disciplina * > >::iterator it_disciplina
                  = curriculo->disciplinas_periodo.begin();

               // Para cada disciplina do curriculo 'curriculo', procuro se devo
               // substituir por alguma disciplina do curriculo 'curriculo_comp'
               for (; it_disciplina != curriculo->disciplinas_periodo.end();
                  it_disciplina++ )
               {
                  disciplina = ( *it_disciplina ).second;

                  GGroup< std::pair< int, Disciplina * > >::iterator it_disciplina_comp
                     = curriculo_comp->disciplinas_periodo.begin();

                  for (; it_disciplina_comp != curriculo_comp->disciplinas_periodo.end();
                     it_disciplina_comp++ )
                  {
                     disciplina_comp = ( *it_disciplina_comp ).second;

                     // Não devo verificar uma disciplina com ela própria
                     if ( disciplina->getId() == disciplina_comp->getId() )
                     {
                        continue;
                     }

                     //-----------------------------------------------------------------------------
                     // Substituir disciplinas equivalentes
                     Disciplina * disciplina_substituta
                        = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );

                     if ( disciplina_substituta != NULL
                        && disciplina_substituta->getId() == disciplina_comp->getId() )
                     {
                        atualizar_demandas = true;

                        // Antigo par período/disciplina
                        std::pair< int, Disciplina * > pairPeriodoDisciplina
                           = std::make_pair( ( *it_disciplina ).first, ( *it_disciplina ).second );

                        // O período é o mesmo, mas mudamos a disciplina
                        std::pair< int, Disciplina * > novoPeriodoDisciplina
                           = std::make_pair( ( *it_disciplina ).first, disciplina_substituta );

                        // Troca a disciplina e mantém o período
                        curriculo->disciplinas_periodo.remove( pairPeriodoDisciplina );
                        curriculo->disciplinas_periodo.add( novoPeriodoDisciplina );

                        // Volta o iterador ao início, por a alteração feita logo acima
                        // em 'disciplinas_periodo' faz o iterator 'it_disciplina'
                        it_disciplina = curriculo->disciplinas_periodo.begin();
                        break;
                     }
                     //-----------------------------------------------------------------------------
                  }
               }
            }
         }
      }
   }

	// Caso alguma disciplina tenha sido
	// substituída, devo substituir também as demandas
	if ( atualizar_demandas )
	{
		atualizaOfertasDemandas();
	}
}

void ProblemDataLoader::atualizaOfertasDemandas()
{
   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_substituta = NULL;

   Demanda * demanda_anterior = NULL;
   Demanda * demanda_substituta = NULL;

   // Percorre todos os pares de disciplina/disciplina substituta
   std::map< std::pair< Curso *, Curriculo * >,
      std::map< Disciplina *, Disciplina * > >::iterator it_map
      = problemData->map_CursoCurriculo_DiscSubst.begin();

    for (; it_map != problemData->map_CursoCurriculo_DiscSubst.end(); it_map++ )
    {
		// Curso e currículo da disciplina que foi substituída
		curso = ( *it_map ).first.first;
		curriculo = ( *it_map ).first.second;

      // Percorre o map de disciplinas substituídas do curso e currículo atuais
      std::map< Disciplina *, Disciplina * >::iterator
         it_disciplinas = ( *it_map ).second.begin();

      for (; it_disciplinas != ( *it_map ).second.end(); it_disciplinas++ )
      {
         // Disciplina que foi substituída
         disciplina = it_disciplinas->first;

         // Disciplina que substituiu a anterior
         disciplina_substituta = it_disciplinas->second;

			// Preciso pegar todas as demandas relacionadas às disciplinas
			// substituídas, pois essas demandas deixarão de existir, e a quantidade
			// de alunos demandados será adicionada na demanda da disciplina que substituiu
			GGroup< Demanda *, LessPtr< Demanda > > demandas
				= problemData->retornaDemandaDisciplinasSubstituidas( disciplina );

         // Procura por uma demanda da disciplina que substituiu a
         // anterior, em um curso que seja compatível com o curso atual
			demanda_substituta = problemData->buscaDemanda( curso, disciplina_substituta );

         // Não há demanda para 'disciplina_substituta' em
         // um curso que seja compatível com o curso atual
         if ( demanda_substituta == NULL )
         {
            continue;
         }

         int id_demanda = retornaMaiorIdDemandas();
         id_demanda++;

         // A partir de cada demanda existente de disciplinas que
         // forem substituídas, devo criar uma nova demanda para a
         // disciplina substituta, porém referente à oferta da demanda anterior
         ITERA_GGROUP_LESSPTR( it_demanda, demandas, Demanda )
         {
            //---------------------------------------------------------------------------
            // Demanda da disciplina que foi substituída
            demanda_anterior = ( *it_demanda );

            Demanda * nova_demanda = new Demanda();

            nova_demanda->setId( id_demanda );
            id_demanda++;

            nova_demanda->setDisciplinaId( disciplina_substituta->getId() );
            nova_demanda->setOfertaId( demanda_anterior->oferta->getId() );
            nova_demanda->setQuantidade( demanda_anterior->getQuantidade() );

            nova_demanda->oferta = demanda_anterior->oferta;
            nova_demanda->disciplina = disciplina_substituta;

            // A demanda da disciplina substituída
            // deixa de existir e inserimos a nova demanda
            problemData->demandas.remove( demanda_anterior );
            problemData->demandas.add( nova_demanda );

            // Informa que a demanda criada vale para 'disciplina',
            // mas foi criada como demanda de 'disciplina_substituta'
            problemData->demandasDisciplinasSubstituidas[ disciplina ] = nova_demanda;
            //---------------------------------------------------------------------------
			}
		}
    }
}

// Retorna o maior id das demandas já cadastradas
int ProblemDataLoader::retornaMaiorIdDemandas()
{
   int id_demanda = -1;

   ITERA_GGROUP_LESSPTR( it_demanda, problemData->demandas, Demanda )
   {
      if ( it_demanda->getId() > id_demanda )
      {
         id_demanda = it_demanda->getId();
      }
   }

   return id_demanda;
}

void ProblemDataLoader::relacionaDisciplinasEquivalentes()
{
   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;
   Disciplina * disciplina_substituta = NULL;

   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      curso = ( *it_curso );

      ITERA_GGROUP_LESSPTR( it_curriculo, curso->curriculos, Curriculo )
      {
         curriculo = ( *it_curriculo );

         // Período - Disciplina
         GGroup< std::pair< int, Disciplina * > >::iterator
            it_periodo_disciplina = curriculo->disciplinas_periodo.begin();

         for (; it_periodo_disciplina != curriculo->disciplinas_periodo.end();
            it_periodo_disciplina++ )
         {
            disciplina = ( ( *it_periodo_disciplina ).second );

            ITERA_GGROUP_LESSPTR( it_disc_equivalente,
               disciplina->discEquivalentes, Disciplina )
            {
               disciplina_equivalente = ( *it_disc_equivalente );

               //------------------------------------------------------------------------------------------------
               // Procuro por todos os pares curso/curriculo
               // onde a disciplina 'disciplina_equivalente' está incluída,
               // para informar a substituição dessa disciplina por uma equivalente
               GGroup< std::pair< Curso *, Curriculo * > > cursos_curriculos
                  = problemData->retornaCursosCurriculosDisciplina( disciplina_equivalente );

               GGroup< std::pair< Curso *, Curriculo * > >::iterator
                  it_curso_curriculo = cursos_curriculos.begin();

               // Informa a substituição para cada um dos pares curso/curriculo
               for (; it_curso_curriculo != cursos_curriculos.end(); it_curso_curriculo++ )
               {
                  std::pair< Curso *, Curriculo * > curso_curriculo = ( *it_curso_curriculo );

                  disciplina_substituta = disciplina;

                  // Verifica se 'disciplina' já foi anteriormente substituída
                  if ( problemData->map_CursoCurriculo_DiscSubst[ curso_curriculo ].find( disciplina )
                     != problemData->map_CursoCurriculo_DiscSubst[ curso_curriculo ].end() )
                  {
                     disciplina_substituta = problemData->map_CursoCurriculo_DiscSubst[ curso_curriculo ][ disciplina ];
                  }

                  // Informa que a 'disciplina_equivalente' será substituída pela 'disciplina'
                  problemData->map_CursoCurriculo_DiscSubst[ curso_curriculo ][ disciplina_equivalente ] = disciplina_substituta;

                  // Adiciona a 'disciplina_equivalente' no conjunto de disciplina que foram substituidas por 'disciplina'
                  problemData->mapGroupDisciplinasSubstituidas[ curso_curriculo ][ disciplina_substituta ].add( disciplina_equivalente );
               }
               //------------------------------------------------------------------------------------------------
            }
         }
      }
   }

   substituiDisciplinasEquivalentes();
}

void ProblemDataLoader::divideDisciplinas()
{
   int idNovaMag = -1;

   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      if ( it_disc->getCredTeoricos() > 0
         && it_disc->getCredPraticos() > 0
         && it_disc->eLab() == true )
      {
         Disciplina * nova_disc = new Disciplina();

         nova_disc->setId( -it_disc->getId() );
         nova_disc->setCodigo( it_disc->getCodigo() + "-P" );
         nova_disc->setNome( it_disc->getNome() + "PRATICA" );
         nova_disc->setCredTeoricos(0);
         nova_disc->setCredPraticos( it_disc->getCredPraticos() );
         it_disc->setCredPraticos(0);

         nova_disc->setMaxCreds( nova_disc->getCredPraticos() );
         it_disc->setMaxCreds( it_disc->getCredTeoricos() );

         nova_disc->setELab( it_disc->eLab() );
         it_disc->setELab( false );

         nova_disc->setMaxAlunosT( -1 );
         nova_disc->setMaxAlunosP( it_disc->getMaxAlunosP() );
         it_disc->setMaxAlunosP( -1 );

         nova_disc->setTipoDisciplinaId( it_disc->getTipoDisciplinaId() );
         nova_disc->setNivelDificuldadeId( it_disc->getNivelDificuldadeId() );

         if( it_disc->divisao_creditos != NULL )
         {
            std::map< int /*Num. Creds*/ ,
               GGroup< DivisaoCreditos * > >::iterator it_Creds_Regras;

            // Alterações relacionadas à disciplina antiga
            delete it_disc->divisao_creditos;

            it_Creds_Regras = problemData->creds_Regras.find( it_disc->getCredTeoricos() );

            // Checando se existe alguma regra de crédito
            // cadastrada para o total de créditos da nova disciplina.
            if ( it_Creds_Regras != problemData->creds_Regras.end() )
            {
               if ( it_Creds_Regras->second.size() == 1 )
               {
                  it_disc->divisao_creditos = new DivisaoCreditos(
                     **it_Creds_Regras->second.begin());
               }
               else // Greather
               {
                  GGroup< DivisaoCreditos * >::iterator 
                     it_Regra = it_Creds_Regras->second.begin();

                  int continuar = rand() % 2;
                  while ( 1 == continuar
                     && ( it_Regra != it_Creds_Regras->second.end() ) )
                  {
                     ++it_Regra;
                     continuar = rand() % 2;
                  }

                  it_disc->divisao_creditos = new DivisaoCreditos( **it_Regra );
               }
            }

            // Alterações relacionadas à nova disciplina
            it_Creds_Regras = problemData->creds_Regras.find(
               nova_disc->getCredPraticos());

            // Checando se existe alguma regra de crédito
            // cadastrada para o total de créditos da nova disciplina.
            if ( it_Creds_Regras != problemData->creds_Regras.end() )
            {
               if ( it_Creds_Regras->second.size() == 1 )
               {
                  nova_disc->divisao_creditos = new DivisaoCreditos(
                     **it_Creds_Regras->second.begin() );
               }
               else // Greather
               {
                  GGroup< DivisaoCreditos * >::iterator 
                     it_Regra = it_Creds_Regras->second.begin();

                  int continuar = rand() % 2;
                  while( 1 == continuar
                     && ( it_Regra != it_Creds_Regras->second.end() ) )
                  {
                     ++it_Regra;
                     continuar = rand() % 2;
                  }

                  nova_disc->divisao_creditos = new DivisaoCreditos( **it_Regra );
               }
            }
         }

         //>>> Copiando HORARIO
         ITERA_GGROUP( it_hr, it_disc->horarios, Horario )
         {
            Horario * h =  new Horario;
            h->setId( it_hr->getId() );

            //>>> >>> Copiando DiaSemana
            GGroup< int >::iterator it_dia
               = it_hr->dias_semana.begin();
            for ( unsigned dia = 0; dia < it_hr->dias_semana.size(); dia++ )
            {
               h->dias_semana.add( *it_dia );
               it_dia++;
            }
            // <<< <<<

            h->setHorarioAulaId( it_hr->getHorarioAulaId() );
            h->setTurnoId( it_hr->getTurnoId() );

            // >>> >>> Copiando TURNO
            Turno * turno;
            if ( it_hr->turno != NULL )
            {
               turno = new Turno();

               turno->setId( it_hr->turno->getId() );
               turno->setNome( it_hr->turno->getNome() );
               turno->setTempoAula( it_hr->turno->getTempoAula() );

               // >>> >>> >>> Copiando HorariosAula
               HorarioAula * hr_aula;
               if ( it_hr->turno->horarios_aula.size() > 0 )
               {
                  ITERA_GGROUP( it_hr_aula, turno->horarios_aula, HorarioAula )
                  {
                     hr_aula = new HorarioAula();

                     hr_aula->setId( it_hr_aula->getId() );
                     hr_aula->setInicio( it_hr_aula->getInicio() );

                     GGroup< int >::iterator it_dia_sem
                        = it_hr_aula->dias_semana.begin();
                     for ( unsigned dia = 0;
                        dia < it_hr_aula->dias_semana.size(); dia++ )
                     {
                        hr_aula->dias_semana.add( *it_dia_sem );
                        it_dia_sem++;
                     }
                  }

                  turno->horarios_aula.add( hr_aula );
               }

               h->turno = turno;
            }

            HorarioAula * hr_aula;
            if ( it_hr->horario_aula != NULL )
            {
               hr_aula = new HorarioAula();

               hr_aula->setId( it_hr->horario_aula->getId() );
               hr_aula->setInicio( it_hr->horario_aula->getInicio() );

               GGroup<int>::iterator it_dia_sem
                  = it_hr->horario_aula->dias_semana.begin();
               for ( unsigned dia = 0;
                  dia < it_hr->horario_aula->dias_semana.size(); dia++ )
               {
                  hr_aula->dias_semana.add( *it_dia_sem );
                  it_dia_sem++;
               }
            }

            nova_disc->horarios.add(h);
         }

         int idDisc = nova_disc->getId();

         ITERA_GGROUP_LESSPTR( it_cp, problemData->campi, Campus )
         {
            // Adicionando os dados da nova disciplina
            // em <Campi->Unidade->Sala->disciplinasAssociadas>:
            ITERA_GGROUP_LESSPTR( it_und, it_cp->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( it_sala, it_und->salas, Sala )
               {
                  if( ( it_sala->disciplinas_associadas.find( it_disc->getId() )
                     != it_sala->disciplinas_associadas.end() )
                     && ( it_sala->getTipoSalaId() != 1 ) )
                  {
                     // Removendo a associacao da disciplina teorica em
                     // questao com as salas incompativeis, no caso qualquer
                     // uma que nao seja uma sala de aula (de acordo com inputTrivial)
                     it_sala->disciplinas_associadas.remove( it_disc->getId() );

                     // Em relacao a nova disciplina (pratica), so adiciono uma
                     // associacao quando  for com uma sala compativel, no caso LABORATORIO
                     it_sala->disciplinas_associadas.add( nova_disc->getId() );
                  }
               }
            }

            // Adicionando os dados da nova disciplina
            // em <Campi->Professor->disciplinas>:
            Magisterio * novo_mag;
            ITERA_GGROUP_LESSPTR( it_prof, it_cp->professores, Professor )
            {
               ITERA_GGROUP_LESSPTR( it_mag, it_prof->magisterio, Magisterio )
               {
                  if ( it_mag->getDisciplinaId() == it_disc->getId() )
                  {
                     novo_mag = new Magisterio();

                     idNovaMag--;
                     novo_mag->setId(idNovaMag);
                     novo_mag->setNota( it_mag->getNota() );
                     novo_mag->setPreferencia( it_mag->getPreferencia() );
                     novo_mag->setDisciplinaId( nova_disc->getId() );
                     it_prof->magisterio.add( novo_mag );

                     // Garantindo que um mesmo professor nao possui
                     // preferencias diferentes em relacao a uma mesma disciplina.
                     break;
                  }
               }
            }
         }

         // Adicionando os dados da nova disciplina em < GrupoCurso->curriculos >
         ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
         {
            ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
            {
               // FIXME, isto está errado, deveria-se, de algum jeito,
               // saber o periodo da disciplina ou, iterar sobre todos os periodos 
               // validos de um curso e nao sobre uma estimativa.
               for ( int num_periodos = 0; num_periodos < 20 /* FIX-ME */; num_periodos++ )
               {
                  std::pair< int, Disciplina * > disc_periodo( num_periodos, *it_disc );

                  if ( it_curriculo->disciplinas_periodo.find( disc_periodo )
                     != it_curriculo->disciplinas_periodo.end() )
                  {
                     it_curriculo->disciplinas_periodo.add(
                        std::pair< int, Disciplina * >( disc_periodo.first, nova_disc ) );

                     // Garantido que uma disciplina aparece
                     // apenas uma vez em um curriculo de um curso.
                     break;
                  }
               }
            }
         }

         // Procura pelo maior id de demanda já cadastrado
         int id = 0;
         ITERA_GGROUP_LESSPTR( it_dem, problemData->demandas, Demanda )
         {
            if ( id < it_dem->getId() )
            {
               id = it_dem->getId();
            }
         }

         // Adicionando os dados da nova disciplina em <Demanda>
         Demanda * nova_demanda = NULL;
         ITERA_GGROUP_LESSPTR( it_dem, problemData->demandas, Demanda )
         {
            int num_vezes_ecncontrado = 0;
            if( it_dem->getDisciplinaId() == it_disc->getId())
            {
               nova_demanda = new Demanda();

               nova_demanda->setId( id++ );
               nova_demanda->setOfertaId( it_dem->getOfertaId() );
               nova_demanda->setDisciplinaId( nova_disc->getId() );
               nova_demanda->setQuantidade( it_dem->getQuantidade() );
               nova_demanda->oferta = it_dem->oferta;
               nova_demanda->disciplina = nova_disc;

               problemData->demandas.add( nova_demanda );
               if ( num_vezes_ecncontrado > 0 )
               {
                  std::cout << "POSSIVEL ERRO EM <divideDisciplinas()> -> "
                     << "Encontrei mais de uma demanda para uma dada disciplina de um "
                     << "dado curso em um determinado campus." << std::endl;

                  std::cin.get();
               }

               num_vezes_ecncontrado++;
            }
         }

         GGroup< int >::iterator itDiasLetivosDiscs = it_disc->diasLetivos.begin();
         for (; itDiasLetivosDiscs != it_disc->diasLetivos.end(); itDiasLetivosDiscs++ )
         {
            nova_disc->diasLetivos.add( *itDiasLetivosDiscs );
         }

         problemData->novasDisciplinas.add( nova_disc );
      }
   }

   ITERA_GGROUP_LESSPTR( itDisciplina, problemData->novasDisciplinas, Disciplina )
   { problemData->disciplinas.add( *itDisciplina ); }

   // ---------
   criaFixacoesDisciplinasDivididas();

}

void ProblemDataLoader::referenciaCampusUnidadesSalas()
{
   ITERA_GGROUP_LESSPTR( it_cp, problemData->campi, Campus )
   {
      problemData->refCampus[ it_cp->getId() ] = ( *it_cp );

      ITERA_GGROUP_LESSPTR( it_Unidade, it_cp->unidades, Unidade )
      {
         problemData->refUnidade[ it_Unidade->getId() ] = ( *it_Unidade );

         ITERA_GGROUP_LESSPTR( it_Sala, it_Unidade->salas, Sala )
         {
            problemData->refSala[ it_Sala->getId() ] = ( *it_Sala );
         }
      }
   }
}

void ProblemDataLoader::referenciaDisciplinas()
{
   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      problemData->refDisciplinas[ it_disc->getId() ] = ( *it_disc );
   }
}

void ProblemDataLoader::referenciaOfertas()
{
   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
      problemData->refOfertas[ itOferta->getId() ] = ( *itOferta );
   }
}

void ProblemDataLoader::gera_refs()
{
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP( it_horario, it_unidades->horarios, Horario )
         {
            find_and_set_lessptr( it_horario->getTurnoId(),
               problemData->todos_turnos,
               it_horario->turno, false );

            find_and_set( it_horario->getHorarioAulaId(),
               it_horario->turno->horarios_aula,
               it_horario->horario_aula, false );
         }

         ITERA_GGROUP_LESSPTR( it_salas, it_unidades->salas, Sala )
         {
            find_and_set( it_salas->getTipoSalaId(),
               problemData->tipos_sala,
               it_salas->tipo_sala, false );

            ITERA_GGROUP( it_horario, it_salas->horarios_disponiveis,
               Horario )
            {
               find_and_set_lessptr( it_horario->getTurnoId(),
                  problemData->todos_turnos,
                  it_horario->turno, false );

               find_and_set( it_horario->getHorarioAulaId(),
                  it_horario->turno->horarios_aula,
                  it_horario->horario_aula, false );
            }

            ITERA_GGROUP( it_credito,it_salas->creditos_disponiveis,
               CreditoDisponivel )
            {
               find_and_set_lessptr( it_credito->getTurnoId(),
                  problemData->todos_turnos,
                  it_credito->turno, false );
            }

            // Disciplinas associadas ? TODO (ou não)
            ITERA_GGROUP_N_PT( it_id_Disc, it_salas->disciplinas_associadas, int )
            {
               ITERA_GGROUP_LESSPTR( it_Disc, problemData->disciplinas, Disciplina )
               {
                  if ( ( *it_id_Disc ) == it_Disc->getId() )
                  {
                     it_salas->disciplinas_Associadas_Usuario.add( *it_Disc );
                  }
               }
            }
         } // end salas
      }

      ITERA_GGROUP_LESSPTR( it_prof, it_campi->professores, Professor )
      {
         find_and_set( it_prof->getTipoContratoId(), 
            problemData->tipos_contrato, 
            it_prof->tipo_contrato, false );

         ITERA_GGROUP( it_horario, it_prof->horarios, Horario )
         {
            find_and_set_lessptr( it_horario->getTurnoId(),
               problemData->todos_turnos,
               it_horario->turno, false );

            find_and_set( it_horario->getHorarioAulaId(),
               it_horario->turno->horarios_aula,
               it_horario->horario_aula, false );
         }

         ITERA_GGROUP_LESSPTR( it_mag, it_prof->magisterio, Magisterio )
         {
            find_and_set_lessptr( it_mag->getDisciplinaId(),
               problemData->disciplinas,
               it_mag->disciplina, false );
         }
      } // end professores

      ITERA_GGROUP( it_horario, it_campi->horarios, Horario )
      {
         find_and_set_lessptr( it_horario->getTurnoId(),
            problemData->todos_turnos,
            it_horario->turno, false );

         find_and_set( it_horario->getHorarioAulaId(),
            it_horario->turno->horarios_aula,
            it_horario->horario_aula, false );
      } 
   } // campus

   ITERA_GGROUP( it_desl, problemData->tempo_campi, Deslocamento )
   {
      if ( problemData->refCampus[ it_desl->getOrigemId() ] != NULL )
      {
         find_and_set_lessptr( it_desl->getOrigemId(),
            problemData->campi, ( Campus * & ) it_desl->origem, false );
      }

      if ( problemData->refCampus[ it_desl->getDestinoId() ] != NULL )
      {
         find_and_set_lessptr( it_desl->getDestinoId(),
            problemData->campi, ( Campus * & ) it_desl->destino, false );
      }
   } // deslocamento campi

   ITERA_GGROUP( it_desl, problemData->tempo_unidades, Deslocamento )
   {
      // É preciso procurar a unidade nos campi
      ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
      {
         // Posso fazer find_and_set em todos sem ifs,
         // porque ele só seta se encontrar. Posso continuar
         // fazendo mesmo depois de encontrar pelo mesmo motivo

         find_and_set_lessptr( it_desl->getOrigemId(),
            it_campi->unidades, ( Unidade * & ) it_desl->origem, false );

         find_and_set_lessptr( it_desl->getDestinoId(),
            it_campi->unidades, ( Unidade * & ) it_desl->destino, false );
      }
   } // deslocamento unidades 

   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      find_and_set( it_disc->getTipoDisciplinaId(),
         problemData->tipos_disciplina,
         it_disc->tipo_disciplina, false );

      find_and_set( it_disc->getNivelDificuldadeId(),
         problemData->niveis_dificuldade,
         it_disc->nivel_dificuldade, false );

      ITERA_GGROUP( it_horario, it_disc->horarios, Horario )
      {
         find_and_set_lessptr( it_horario->getTurnoId(),
            problemData->todos_turnos,
            it_horario->turno, false );

         find_and_set( it_horario->getHorarioAulaId(),
            it_horario->turno->horarios_aula,
            it_horario->horario_aula, false );
      } 
   } // disciplinas

   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      find_and_set_lessptr( it_curso->getTipoId(),
         problemData->tipos_curso,
         it_curso->tipo_curso, false );

      ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
      {
         find_and_set_lessptr( it_curriculo->getSemanaLetivaId(),
            problemData->calendarios,
            it_curriculo->calendario, false );
      }
   }

   ITERA_GGROUP( it_ndh, problemData->parametros->niveis_dificuldade_horario,
      NivelDificuldadeHorario )
   {
      find_and_set( it_ndh->nivel_dificuldade_id,
         problemData->niveis_dificuldade,
         it_ndh->nivel_dificuldade, false );
   }

   ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
   {
      // Seta a referência à disciplina da fixação
      if ( it_fix->getDisciplinaId() >= 0 )
      {
         find_and_set_lessptr( it_fix->getDisciplinaId(), problemData->disciplinas,
            it_fix->disciplina, false );
      }

      // Seta a referência ao turno da fixação
      if ( it_fix->getTurnoId() >= 0 )
      {
         find_and_set_lessptr( it_fix->getTurnoId(),
            problemData->todos_turnos,
            it_fix->turno, false );
      }

      // Seta a referência ao horário de aula da fixação
      if ( it_fix->getHorarioAulaId() >= 0 )
      {
         if ( it_fix->turno != NULL )
         {
            find_and_set( it_fix->getHorarioAulaId(), it_fix->turno->horarios_aula,
               it_fix->horario_aula, false );
         }
         else
         {
            // Como o turno não foi fixado, mas o horário de
            // aula foi, então procuramos o horário aula fixado
            // dentre todos os horários aula (entre todos os turnos)
            GGroup< HorarioAula * > todos_horarios_aula;
            ITERA_GGROUP_LESSPTR( it_turno, problemData->todos_turnos, Turno )
            {
               ITERA_GGROUP( it_horario_aula, it_turno->horarios_aula, HorarioAula )
               {
                  todos_horarios_aula.add( ( *it_horario_aula ) );
               }
            }

            find_and_set( it_fix->getHorarioAulaId(), todos_horarios_aula,
               it_fix->horario_aula, false );
         }
      }

      ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
      {
         // Seta a referência ao professor da fixação
         if ( it_fix->getProfessorId() >= 0 )
         {
            find_and_set_lessptr( it_fix->getProfessorId(), it_campi->professores,
               it_fix->professor, false );
         }

         // Seta a referência à sala da fixação
         if ( it_fix->getSalaId() >= 0 )
         {
            ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade )
            {
               find_and_set_lessptr( it_fix->getSalaId(), it_unidades->salas,
                  it_fix->sala, false );
            }
         }
      }
   }

   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade ) 
      {
         it_unidades->setIdCampus( it_campi->getId() );
         ITERA_GGROUP_LESSPTR( it_salas, it_unidades->salas, Sala ) 
         {
            it_salas->setIdUnidade( it_unidades->getId() );
         }
      }
   }
}

void ProblemDataLoader::cria_blocos_curriculares()
{
   // Contador de blocos
   int id_Bloco = 1;

   Campus * campus = NULL;
   Curso * curso = NULL;
   Oferta * oferta = NULL;
   Demanda * demanda = NULL;
   Curriculo * curriculo = NULL;
   Disciplina * disciplina = NULL;

   std::pair< int, Disciplina * > disciplina_periodo;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      campus = ( *it_campus );
      ITERA_GGROUP_LESSPTR( it_curso, it_campus->cursos, Curso )
      {
         curso = ( *it_curso );
         ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
         {
            curriculo = ( *it_curriculo );

            // Descobrindo oferta em questão
            oferta = NULL;
            ITERA_GGROUP_LESSPTR( it_oferta, campus->ofertas, Oferta )
            {
               if(it_oferta->campus->getId() == campus->getId() &&
                  it_oferta->curriculo->getId() == curriculo->getId() &&
                  it_oferta->curso->getId() == curso->getId())
               {
                  oferta = *(it_oferta);
                  break;
               }
            }

            // Checando se foi encontrada alguma oferta válida.
            if ( oferta == NULL )
            {
               continue;
            }

            GGroup< std::pair< int, Disciplina * > >::iterator it_disc_periodo =
               curriculo->disciplinas_periodo.begin();

            // Percorrendo todas as disciplinas de
            // um curso cadastradas para um currículo.
            for(; it_disc_periodo != curriculo->disciplinas_periodo.end();
                  it_disc_periodo++ )
            {
               disciplina_periodo = ( *it_disc_periodo );

               int periodo = disciplina_periodo.first;
               disciplina = disciplina_periodo.second;

               std::pair< Campus *, Curso * > campus_curso
                  = std::make_pair( campus, curso );

               // Recupera o conjunto de demandas
               // relacionadas ao par 'Campus' e 'Curso' atual
               GGroup< Demanda *, LessPtr< Demanda > > * demandas
                  = &( problemData->map_campus_curso_demanda[ campus_curso ] );

               // Encontrando e armazenando a demanda
               // específica da disciplina em questão
               int id_disciplina = 0;
               int id_oferta = 0;
               demanda = NULL;
               ITERA_GGROUP_LESSPTR( it_demanda, ( *demandas ), Demanda )
               {
                  id_disciplina = abs( disciplina->getId() );
                  id_oferta = oferta->getId();

                  if ( it_demanda->disciplina->getId() == id_disciplina
                     && it_demanda->oferta->getId() == id_oferta )
                  {
                     demanda = ( *it_demanda );
                     break;
                  }
               }

               if ( demanda == NULL )
               {
                  std::cout << "\nERRO: DEMANDA NAO ENCONTRADA EM:"
                     << "\nProblemDataLoadaer::cria_blocos_curriculares()"
                     << "\nDisciplina : " << id_disciplina
                     << "\nOferta : " << id_oferta
                     << std::endl << std::endl;

                  exit(1);
               }

               bool found = false;

               // Verificando a existência do bloco
               // curricular para a disciplina em questão.
               ITERA_GGROUP_LESSPTR( it_bloco_curricular, problemData->blocos, BlocoCurricular )
               {
                  if ( it_bloco_curricular->campus->getId() == campus->getId()
                     && it_bloco_curricular->curso->getId() == curso->getId()
                     && it_bloco_curricular->curriculo->getId() == curriculo->getId()
                     && it_bloco_curricular->getPeriodo() == periodo )
                  {
                     it_bloco_curricular->disciplinas.add( disciplina );
                     it_bloco_curricular->disciplina_Demanda[ disciplina ] = demanda;
                     found = true;
                     break;
                  }
               }

               if ( !found )
               {
                  BlocoCurricular * bloco_curricular = new BlocoCurricular();

                  bloco_curricular->setId( id_Bloco );
                  bloco_curricular->setPeriodo( periodo );
                  bloco_curricular->campus = campus;
                  bloco_curricular->curso = curso;
                  bloco_curricular->curriculo = curriculo;
                  bloco_curricular->disciplinas.add( disciplina );
                  bloco_curricular->disciplina_Demanda[ disciplina ] = demanda;

                  problemData->blocos.add( bloco_curricular );
                  id_Bloco++;
               }
            }
         }
      }
   }

   // Setando os dias letivos de cada bloco.
   BlocoCurricular * bloco = NULL;
   ITERA_GGROUP_LESSPTR( it_bc, problemData->blocos, BlocoCurricular )
   {
      bloco = ( *it_bc );
      curso = it_bc->curso;

      int totalTurmas = 0;

      ITERA_GGROUP_LESSPTR( it_Disc, it_bc->disciplinas, Disciplina )
      {
         disciplina = ( *it_Disc );

         // Associa o curso correspondente ao bloco atual
         // e a disciplina 'it_disc' ao bloco curricular corrente
         problemData->mapCursoDisciplina_BlocoCurricular
            [ std::make_pair(curso, disciplina) ] = bloco;

         ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Disc->diasLetivos, int )
         { 
            it_bc->diasLetivos.add( *it_Dias_Letivos );
         }

         totalTurmas += it_Disc->getNumTurmas();
      }

      it_bc->setTotalTurmas( totalTurmas );
   }
}

void ProblemDataLoader::relacionaCampusDiscs()
{
   ITERA_GGROUP_LESSPTR( it_oferta, problemData->ofertas, Oferta )
   {
      Curso * curso = it_oferta->curso;

      ITERA_GGROUP_LESSPTR( it_curric, curso->curriculos, Curriculo )
      {
         GGroup< std::pair< int, Disciplina * > >::iterator it_disc_prd =
            it_curric->disciplinas_periodo.begin();

         for (; it_disc_prd != it_curric->disciplinas_periodo.end(); it_disc_prd++ )
         {
            problemData->cp_discs[ it_oferta->getCampusId() ].add( ( *it_disc_prd ).second->getId() );
         }
      }
   }
}

void ProblemDataLoader::calculaTamanhoMedioSalasCampus()
{
   ITERA_GGROUP_LESSPTR( it_cp, problemData->campi, Campus )
   {
      unsigned somaCapSalas = 0;
      unsigned total_Salas = 0;

      ITERA_GGROUP_LESSPTR( it_und, it_cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_und->salas, Sala )
         {
            somaCapSalas += ( it_sala->getCapacidade() );

            it_und->setMaiorSala( std::max( ( (int)it_und->getMaiorSala() ),
               ( (int)it_sala->getCapacidade() ) ) );
         }

         total_Salas += it_und->getNumSalas();
         it_cp->setMaiorSala( std::max( (int)it_cp->getMaiorSala(),
            (int)it_und->getMaiorSala() ) );
      }

      problemData->cp_medSalas[ it_cp->getId() ] =
         ( ( total_Salas > 0 ) ? ( somaCapSalas / total_Salas ) : 0 );
   }
}

void ProblemDataLoader::calculaDemandas()
{
   Demanda * demanda = NULL;
   Campus * campus = NULL;
   Curso * curso = NULL;
   ITERA_GGROUP_LESSPTR( it_demanda, problemData->demandas, Demanda )
   {
      demanda = (*it_demanda);

      campus = demanda->oferta->campus;
      curso = demanda->oferta->curso;

      //-------------------------------------------------------------------
      // Relaciona a 'Demanda' atual a seus respectivos 'Campus' e 'Curso'
      std::pair< Campus *, Curso * > campus_curso
         = std::make_pair( campus, curso );

      GGroup< Demanda *, LessPtr< Demanda > > * demandas
         = &( problemData->map_campus_curso_demanda[ campus_curso ] );

      demandas->add( demanda );
      //-------------------------------------------------------------------

      //-------------------------------------------------------------------
      int dem = it_demanda->getQuantidade();

      it_demanda->disciplina->setMaxDemanda(
         std::max( it_demanda->disciplina->getMaxDemanda(), dem ) );

      it_demanda->disciplina->adicionaDemandaTotal( dem );
      //-------------------------------------------------------------------

      //-------------------------------------------------------------------
      // Armazenando a demanda total de cada Campus
      std::pair< int, int > demanda_campus
         = std::make_pair( it_demanda->disciplina->getId(),
           it_demanda->oferta->campus->getId() );

      // Inicializa com zero caso ainda não exista;
      if ( problemData->demandas_campus.find( demanda_campus ) !=
         problemData->demandas_campus.end() )
      {
         problemData->demandas_campus[ demanda_campus ] = 0;
      }

      problemData->demandas_campus[ demanda_campus ] += ( dem );
      //-------------------------------------------------------------------
   }
}

// TRIEDA-416
void ProblemDataLoader::estima_turmas()
{
   // Estimando o número máximo de turmas de cada
   // disciplina de acordo com o seguinte cálculo:

   // numTurmasDisc = demDisc / tamMedSalasCP

   // Onde:
   // demDisc -> representa a demanda total de uma dada disciplina.
   // tamMedSalasCP -> representa o tamanho médio das salas de um campus.

   int anterior = 0;
   std::map< int /*Id Campus*/, GGroup< int >/*Id Discs*/ >::iterator itCPDiscs =
      problemData->cp_discs.begin();

   for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
   {
      int tamMedSalasCP = problemData->cp_medSalas[ itCPDiscs->first ];
      GGroup< int >::iterator itDisc = itCPDiscs->second.begin();

      for (; itDisc != itCPDiscs->second.end(); itDisc++ )
      {
         int demDisc = problemData->refDisciplinas[ ( *itDisc ) ]->getDemandaTotal();
         if ( problemData->refDisciplinas[ ( *itDisc ) ]->eLab() )
         {
            if ( problemData->refDisciplinas[ ( *itDisc ) ]->getMaxAlunosP() > 0 )
            {
               int numTurmas = ( demDisc / 25 );
               problemData->refDisciplinas[ ( *itDisc ) ]->setNumTurmas( ( numTurmas > 0 ? numTurmas + 2 : 2 ) );

               //if ( abs(problemData->refDisciplinas[*itDisc]->getId()) == 101 || 
               //   abs(problemData->refDisciplinas[*itDisc]->getId()) == 178 ||
               //   abs(problemData->refDisciplinas[*itDisc]->getId()) == 349 )
               //{
               //   anterior = problemData->refDisciplinas[*itDisc]->getNumTurmas();
               //   problemData->refDisciplinas[*itDisc]->setNumTurmas(anterior+1);
               //}
               //else if ( problemData->refDisciplinas[*itDisc]->getMaxCreds() >= 6)
               //{
               //   anterior = problemData->refDisciplinas[*itDisc]->getNumTurmas();
               //   problemData->refDisciplinas[*itDisc]->setNumTurmas(anterior+1);
               //}
            }
            else
            {
               problemData->refDisciplinas[*itDisc]->setNumTurmas( (demDisc / 25) + 2 );

               //if ( abs(problemData->refDisciplinas[*itDisc]->getId()) == 101 || 
               //   abs(problemData->refDisciplinas[*itDisc]->getId()) == 178 ||
               //   abs(problemData->refDisciplinas[*itDisc]->getId()) == 349 )
               //{
               //   anterior = problemData->refDisciplinas[*itDisc]->getNumTurmas();
               //   problemData->refDisciplinas[*itDisc]->setNumTurmas(anterior+1);
               //}
               //else if ( problemData->refDisciplinas[*itDisc]->getMaxCreds() >= 6)
               //{
               //   anterior = problemData->refDisciplinas[*itDisc]->getNumTurmas();
               //   problemData->refDisciplinas[*itDisc]->setNumTurmas(anterior+1);
               //}
            }
         }
         else
         {
            if(problemData->refDisciplinas[*itDisc]->getMaxAlunosP() > 0)
            {
               int numTurmas = (demDisc / 50);
               problemData->refDisciplinas[*itDisc]->setNumTurmas((numTurmas > 0 ? numTurmas + 1 : 1) );

               //if ( abs(problemData->refDisciplinas[*itDisc]->getId()) == 101 || 
               //   abs(problemData->refDisciplinas[*itDisc]->getId()) == 178 ||
               //   abs(problemData->refDisciplinas[*itDisc]->getId()) == 349 )
               //{
               //   anterior = problemData->refDisciplinas[*itDisc]->getNumTurmas();
               //   problemData->refDisciplinas[*itDisc]->setNumTurmas(anterior+1);
               //}
               //else if ( problemData->refDisciplinas[*itDisc]->getMaxCreds() >= 6)
               //{
               //   anterior = problemData->refDisciplinas[*itDisc]->getNumTurmas();
               //   problemData->refDisciplinas[*itDisc]->setNumTurmas(anterior+1);
               //}
            }
            else
            {
               problemData->refDisciplinas[*itDisc]->setNumTurmas( (demDisc / 50) + 1 );

               //if ( abs(problemData->refDisciplinas[*itDisc]->getId()) == 101 || 
               //   abs(problemData->refDisciplinas[*itDisc]->getId()) == 178 ||
               //   abs(problemData->refDisciplinas[*itDisc]->getId()) == 349 )
               //{
               //   anterior = problemData->refDisciplinas[*itDisc]->getNumTurmas();
               //   problemData->refDisciplinas[*itDisc]->setNumTurmas(anterior+1);
               //}
               //else if ( problemData->refDisciplinas[*itDisc]->getMaxCreds() >= 6)
               //{
               //   anterior = problemData->refDisciplinas[*itDisc]->getNumTurmas();
               //   problemData->refDisciplinas[*itDisc]->setNumTurmas(anterior+1);
               //}
            }
         }
      }
   }
}

void ProblemDataLoader::print_stats()
{
   int ncampi(0), nunidades(0), nsalas(0), nconjuntoSalas(0),
       ndiscs(0), ndiscsDiv(0), nturmas(0), nturmasDiscDiv(0),
       nprofs(0), ncursos(0), nofertas(0), tdemanda(0), tdemandaDiv(0);

   ncampi = problemData->campi.size();
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      nunidades += ( it_campi->unidades.size() );
      ITERA_GGROUP_LESSPTR( it_und, it_campi->unidades, Unidade )
      {
         nsalas += it_und->salas.size();
         nconjuntoSalas += it_und->conjutoSalas.size();
      }

      nprofs += it_campi->professores.size();
      ncursos += problemData->cursos.size();
   }

   nofertas = problemData->ofertas.size();
   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
      GGroup< std::pair< int, Disciplina * > >::iterator itPrdDisc =
         itOferta->curriculo->disciplinas_periodo.begin();

      for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end();
         itPrdDisc++ )
      {
         // Disciplina * disc = problemData->refDisciplinas[ ( *itPrdDisc ).second ];
         Disciplina * disc = ( *itPrdDisc ).second;

         if ( disc->getId() > 0 )
         {
            nturmas += disc->getNumTurmas();
         }
         else
         {
            nturmasDiscDiv += ( disc->getNumTurmas() );
         }
      }
   }

   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      if ( it_disc->getId() > 0 )
      {
         ndiscs++;
         tdemanda += ( it_disc->getDemandaTotal() );
      }
      else
      {
         ndiscsDiv++;
         tdemandaDiv += ( it_disc->getDemandaTotal() );
      }
   }

   std::cout << std::endl;
   std::cout << "Estatisticas de dados de entrada\n\n";
   printf("<*> Campi:                 \t%4d\n",ncampi);
   printf("<*> Unidades:              \t%4d\n",nunidades);
   printf("<*> Salas:                 \t%4d\n",nsalas);

   printf("<*> Conjuntos de Salas:    \t%4d\n",nconjuntoSalas);

   printf("<*> Disciplinas:\n");
   printf("\t - Entrada:    \t%4d\n",ndiscs);
   printf("\t - Divididas:  \t%4d\n",ndiscsDiv);   
   printf("\t - Total:  \t%4d\n",ndiscs+ndiscsDiv);

   printf("<*> Blocos Curriculares:  \t%4d\n",problemData->blocos.size());

   printf("<*> Turmas:\n");
   printf("\t - Entrada:    \t%4d\n",nturmas);
   printf("\t - Divididas:  \t%4d\n",nturmasDiscDiv);   
   printf("\t - Total:  \t%4d\n",nturmas+nturmasDiscDiv);

   printf("<*> Professores:  \t%4d\n",nprofs);
   printf("<*> Cursos:       \t%4d\n",ncursos);

   printf("<*> Ofertas:      \t%4d\n",nofertas);

   printf("<*> Demanda total\n");
   printf("\t - Entrada:    \t%4d\n",tdemanda);
   printf("\t - Divididas:  \t%4d\n",tdemandaDiv);   
   printf("\t - Total:  \t%4d\n",tdemanda+tdemandaDiv);

   std::cout << "================================" << std::endl
      << std::endl;
}

// Salva algumas informações que são usadas frequentemente
void ProblemDataLoader::cache()
{
   problemData->totalSalas = 0;
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      int total_salas = 0;
      ITERA_GGROUP_LESSPTR( it_u, it_campus->unidades, Unidade )
      {
         total_salas += it_u->salas.size();
      }

      it_campus->setTotalSalas( total_salas );
      problemData->totalSalas += total_salas;
   }

   problemData->totalTurmas = 0;
   ITERA_GGROUP_LESSPTR( it_disciplinas, problemData->disciplinas, Disciplina )
   {
      problemData->totalTurmas += it_disciplinas->getNumTurmas();
   }

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      int totalTurmas = 0;
      ITERA_GGROUP_LESSPTR( it_disciplinas, it_bloco->disciplinas, Disciplina )
      {
         totalTurmas += it_disciplinas->getNumTurmas();
      }
   }

   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      if ( it_disc->divisao_creditos != NULL )
      {
         it_disc->setMinCreds(24);
         it_disc->setMaxCreds(0);

         for ( int t = 0; t < 8; t++ )
         {
            if ( it_disc->divisao_creditos->dia[t] > 0 )
            {
               it_disc->setMinCreds( 
                  std::min(it_disc->getMinCreds(),
                  it_disc->divisao_creditos->dia[t]) );

               it_disc->setMaxCreds( 
                  std::max(it_disc->getMaxCreds(),
                  it_disc->divisao_creditos->dia[t]) );
            }
         }
      }
      else
      {
         it_disc->setMinCreds(1);
         it_disc->setMaxCreds( it_disc->getCredPraticos() + it_disc->getCredTeoricos() );
      }

      if ( it_disc->getMinCreds() > 2 )
      {
         it_disc->setMinCreds(1);
      }
   }
}

void ProblemDataLoader::associaDisciplinasSalas()
{
   // Dado o id de um campus, retorna o conjunto
   // dos ids das disciplinas relacionadas a esse campus
   std::map< int, GGroup< int > >::iterator it_Cp_Discs
      = problemData->cp_discs.begin();

   // Para cada Campus
   for (; it_Cp_Discs != problemData->cp_discs.end(); it_Cp_Discs++ )
   {
      Campus * campus = problemData->refCampus[ it_Cp_Discs->first ];

      ITERA_GGROUP_LESSPTR( it_und, campus->unidades, Unidade )
      {
         //------------------------------------------------------------------------
         // PASSO 1
         // Armazenando as disciplinas que foram associadas pelo usuário e não 
         // deverão ser consideradas para a associação automática.
         ITERA_GGROUP_LESSPTR( it_sala, it_und->salas, Sala )
         {
            ITERA_GGROUP( it_Disc_Assoc_Sala,
               it_sala->disciplinas_Associadas_Usuario, Disciplina )
            {
               // Adicionando um ponteiro para qdo tiver uma dada
               // disciplina for fácil descobrir a lista de salas associadas.
               problemData->discSalas[ *it_Disc_Assoc_Sala ].push_back( *it_sala );

               // Adicionando uma preferência de sala para uma dada disciplina.
               problemData->disc_Salas_Pref[ *it_Disc_Assoc_Sala ].add( *it_sala );
            }
         }
         //------------------------------------------------------------------------

         //------------------------------------------------------------------------
         // PASSO 2
         // Associando as demais disciplinas às salas

         // Para cada disciplina associada ao campus em questao
         ITERA_GGROUP_N_PT( it_disciplina, it_Cp_Discs->second, int )
         {
            Disciplina * disciplina
               = ( problemData->refDisciplinas.find( *it_disciplina )->second );

            // Se a disciplina foi associada pelo usuário.
            std::map< Disciplina *, GGroup< Sala *, LessPtr< Sala > >, LessPtr< Disciplina > >::iterator
               it_salas_associadas = problemData->disc_Salas_Pref.find( disciplina );

            bool salas_associadas = ( it_salas_associadas != problemData->disc_Salas_Pref.end() );

            if ( disciplina->eLab() && !salas_associadas )
            {
               // RESTRICAO FORTE
               // NAO DEVO CRIAR MAIS NENHUMA ASSOCIACAO.

               std::cout << "\nERRO!!! ProblemDataLoader::associaDisciplinasSalas()"
                         << "\nUma disciplina pratica nao foi"
                         << "\nassociada a nenhuma sala pelo usuario."
                         << "\nId da disciplina   : " << disciplina->getId()
                         << "\nNome da disciplina : " << disciplina->getNome()
                         << std::endl << std::endl;

               exit( 1 );
            }
            else if ( !salas_associadas )
            {
               // RESTRICAO FRACA DEVO CRIAR ASSOCIACOES
               // ENTRE A DISCIPLINA E TODAS AS SALAS DE AULA.

               ITERA_GGROUP_LESSPTR( it_unidade, campus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
                  {
                     if ( it_sala->disciplinas_Associadas_Usuario.find( disciplina )
                           == it_sala->disciplinas_Associadas_Usuario.end() )
                     {
                        // Somente se for uma sala de aula.
                        if ( it_sala->getTipoSalaId() == 1 )
                        {
                           // Estabelecendo o critério de intereseção de dias letivos.
                           // I.E. Só associo uma sala de aula a uma disciplina se a sala tem, 
                           // pelo menos, um dia letivo comum com a disciplina.
                           ITERA_GGROUP_N_PT( it_Dias_Let_Disc, disciplina->diasLetivos, int )
                           {
                              // Só continuo quando a sala possuir o dia
                              // letivo (pertencente à disciplina) em questão.
                              if ( it_sala->diasLetivos.find( *it_Dias_Let_Disc )
                                    != it_sala->diasLetivos.end() )
                              {
                                 it_sala->disciplinas_Associadas_AUTOMATICA.add( disciplina );

                                 // Adicionando um ponteiro para quando
                                 // tiver uma dada disciplina, for fácil
                                 // descobrir a lista de salas associadas.
                                 problemData->discSalas[ disciplina ].push_back( *it_sala );
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
         //------------------------------------------------------------------------
      }

      //------------------------------------------------------------------------
      // Com as duas estruturas <disciplinas_Associadas_Usuario> e 
      // <disciplinas_Associadas_AUTOMATICA> preenchidas para cada sala,
      // deve-se fazer a uniao delas preenchendo a estrutura <disciplinasAssociadas> 
      ITERA_GGROUP_LESSPTR( it_unidade, campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
         {
            ITERA_GGROUP( it_Disc_Assoc_Usr,
               it_sala->disciplinas_Associadas_Usuario, Disciplina )
            {
               it_sala->disciplinasAssociadas.add( *it_Disc_Assoc_Usr );
            }

            ITERA_GGROUP( it_Disc_Assoc_AUTO,
               it_sala->disciplinas_Associadas_AUTOMATICA, Disciplina )
            {
               it_sala->disciplinasAssociadas.add( *it_Disc_Assoc_AUTO );
            }
         }
      }
      //------------------------------------------------------------------------
   }

   Sala * sala = NULL;
   Disciplina * disciplina = NULL;

   // ------------------------ Verificando as Fixações -------------------------
   // Se uma disciplina está fixada a uma determinada sala associa
   // essa disciplina  somente aquela sala (e não a um grupo de salas)
   std::map< Disciplina *, std::vector< Sala * >, LessPtr< Disciplina > >::iterator
      it_Disc_Salas = problemData->discSalas.begin();

   for (; it_Disc_Salas != problemData->discSalas.end(); it_Disc_Salas++ )
   {
      disciplina = ( it_Disc_Salas->first );

      bool encontrou_sala = false;
      for ( int i = 0; i < (int)it_Disc_Salas->second.size() && !encontrou_sala; i++ )
      {
         sala = it_Disc_Salas->second[ i ];

         ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
         {
            if ( disciplina->getId() == it_fix->disciplina->getId()
               && sala->getId() == it_fix->sala->getId() )
            {
               problemData->discSalas[ it_fix->disciplina ].clear();
               problemData->discSalas[ it_fix->disciplina ].push_back( it_fix->sala );

               encontrou_sala = true;
               break;
            }
         }
      }
   }
   // --------------------------------------------------------------------------
}

void ProblemDataLoader::associaDisciplinasConjuntoSalas()
{
   // Guarda uma referência para os 'ids' dos
   // novos conjuntos salas que forem criados aqui
   int maior_id_conjunto_salas = 0;

   //-------------------------------------------------------------------------------
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            // Guarda sempre o maior 'id' dos conjuntos de sala
            if ( itCjtSala->getId() > maior_id_conjunto_salas )
            {
               maior_id_conjunto_salas = itCjtSala->getId();
            }

            // Estrutura do map: Dado o 'id' de uma
            // sala, retorna-se o ponteiro da sala
            std::map< int, Sala * >::iterator
               itSala = itCjtSala->salas.begin();

            for (; itSala != itCjtSala->salas.end(); itSala++ )
            {
               GGroup< Disciplina * >::iterator itDiscs
                  = itSala->second->disciplinasAssociadas.begin();

               for (; itDiscs != itSala->second->disciplinasAssociadas.end();
                      itDiscs++ )
               {
                  itCjtSala->disciplinas_associadas.add( *itDiscs );
               }
            }
         }
      }
   }
   //-------------------------------------------------------------------------------

   maior_id_conjunto_salas++;

   Sala * sala = NULL;
   Fixacao * fixacao = NULL;
   Disciplina * disciplina = NULL;

   //-------------------------------------------------------------------------------
   // Dividindo os conjuntos de sala, em caso de fixação 'disciplina - sala'
   ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );
      sala = fixacao->sala;
      disciplina = fixacao->disciplina;

      // Verifica se é uma fixação de disciplina e sala
      if ( sala == NULL || disciplina == NULL )
      {
         continue;
      }

      bool encontrou = false;
      ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
      {
         if ( encontrou )
         {
            break;
         }

         ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
         {
            if ( encontrou )
            {
               break;
            }

            ITERA_GGROUP_LESSPTR( it_conjunto_sala, itUnidade->conjutoSalas, ConjuntoSala )
            {
               // Verifica se esse conjunto sala possui a sala da fixação
               std::map< int, Sala * >::iterator
                  find_sala = it_conjunto_sala->salas.find( sala->getId() );

               if ( find_sala == it_conjunto_sala->salas.end() )
               {
                  continue;
               }

               // Caso o conjunto de salas possua apenas a sala fixada,
               // não precisamos então criar um novo conjunto de salas
               if ( it_conjunto_sala->salas.size() == 1 )
               {
                  continue;
               }

               // Verifica se já existe nessa unidade um
               // conjunto de sala que corresponda a essa fixação
               if ( existe_conjunto_sala__fixacao( *itUnidade, disciplina, sala ) )
               {
                  continue;
               }

               // Criamos um novo conjunto de sala, que conterá todas
               // as disciplinas do conjunto de sala atual, exceto a disciplina fixada
               ConjuntoSala * novo_conjunto_sala = new ConjuntoSala( **it_conjunto_sala );
               novo_conjunto_sala->setId( maior_id_conjunto_salas );
               maior_id_conjunto_salas++;

               // Chegando aqui, sabemos que o
               // conjunto sala possui a sala que está fixada
               novo_conjunto_sala->salas.clear();

               std::map< int, Sala * >::iterator it_sala
                  = it_conjunto_sala->salas.begin();

               for (; it_sala != it_conjunto_sala->salas.end(); it_sala++ )
               {
                  // Devo incluir no novo conjunto de
                  // salas apenas as salas que NÃO estão fixadas
                  if ( it_sala->second->getId() != sala->getId() )
                  {
                     novo_conjunto_sala->salas[ it_sala->second->getId() ] = ( it_sala->second );
                  }
               }

               // O novo conjunto de salas não possui a disciplina fixada,
               // pois ela está apenas no outro conjunto de salas, já existente
               novo_conjunto_sala->disciplinas_associadas.remove( disciplina );
               novo_conjunto_sala->dias_letivos_disciplinas.erase( disciplina );

               // No conjunto de salas da sala fixada, apenas
               // existirá uma sala, que é exatamente a sala fixada
               it_conjunto_sala->salas.clear();
               it_conjunto_sala->salas[ sala->getId() ] = sala;

               // No novo conjunto de salas, devo remover a sala fixada
               novo_conjunto_sala->salas.erase( sala->getId() );

               // Adiciona o novo conjunto de salas criado na unidade atual
               itUnidade->conjutoSalas.add( novo_conjunto_sala );

               encontrou = true;
               break;
            }
         }
      }
   }
   //-------------------------------------------------------------------------------
}

bool ProblemDataLoader::existe_conjunto_sala__fixacao( Unidade * unidade, Disciplina * disciplina, Sala * sala )
{
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         // Analiso apenas a unidade em questão
         if ( it_unidade->getId() != unidade->getId() )
         {
            continue;
         }

         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            if ( it_conjunto_sala->salas.size() == 1 // O conjunto possui apenas a sala fixada
               && it_conjunto_sala->salas.begin()->second->getId() == sala->getId()
               && it_conjunto_sala->disciplinas_associadas.size() == 1 // O conjunto possui apenas a disciplina fixada
               && it_conjunto_sala->disciplinas_associadas.begin()->getId() == disciplina->getId() )
            {
               // Já existe o conjunto de salas correspondente à fixação.
               // Portanto, não precisamos criar outro conjunto de salas
               return true;
            }
         }
      }
   }

   return false;
}

void ProblemDataLoader::relacionaDiscOfertas()
{
   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
   {
      GGroup< std::pair< int, Disciplina * > >::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();

      for (; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end();
             it_Prd_Disc++ )
      { 
         disciplina = ( *it_Prd_Disc ).second;
         int disc = disciplina->getId();

         problemData->ofertasDisc[ disc ].add( *it_Oferta );

         // Utilizado em equivalências de disciplinas
         std::pair< Curso *, Curriculo * > curso_curriculo
            = std::make_pair( it_Oferta->curso, it_Oferta->curriculo );

         if ( disciplina->getId() == 9 )
         {
            int k = 0;
            k++;
         }

         problemData->map_Disc_CursoCurriculo[ disciplina ] = curso_curriculo;
      }
   }
}

// Método de pré-processamento
// relacionado com a issue TRIEDA-700
void ProblemDataLoader::criaAulas()
{
   // Checando se o XML de entrada possui a saída do TÁTICO,
   if ( problemData->atendimentosTatico )
   {
      AtendimentoOfertaSolucao * atendOferta = NULL;

      ITERA_GGROUP( it_atend_campus, *problemData->atendimentosTatico, AtendimentoCampusSolucao )
      {
         ITERA_GGROUP( it_atend_unidade, it_atend_campus->atendimentosUnidades, AtendimentoUnidadeSolucao )
         {
            ITERA_GGROUP( it_atend_sala, it_atend_unidade->atendimentosSalas, AtendimentoSalaSolucao )
            {
               ITERA_GGROUP( it_atend_dia_semana, it_atend_sala->atendimentosDiasSemana, AtendimentoDiaSemanaSolucao )
               {
                  ITERA_GGROUP( it_atend_tatico, it_atend_dia_semana->atendimentosTatico, AtendimentoTaticoSolucao )
                  {
                     atendOferta = ( it_atend_tatico->atendimento_oferta );

                     // Informa a 'turma' da aula
                     int turma = atoi( atendOferta->getTurma().c_str() );

                     Disciplina * disciplina
                        = problemData->refDisciplinas.find( atendOferta->getDisciplinaId() )->second;

                     int idDisc = disciplina->getId();

                     Sala * sala = problemData->refSala.find( it_atend_sala->getSalaId() )->second;

                     // Informa o dia da semana da aula
                     int diaSemana = it_atend_dia_semana->getDiaSemana();

                     // Informa os créditos teóricos da aula
                     int creditos_teoricos = it_atend_tatico->getQtdeCreditosTeoricos();

                     // Informa os créditos práticos da aula
                     int creditos_praticos = it_atend_tatico->getQtdeCreditosPraticos();

                     // Informa a demanda atendida
                     int demandaAtendida = it_atend_tatico->atendimento_oferta->getQuantidade();

                     if((creditos_praticos > 0) && (creditos_teoricos > 0))
                     { std::cout << "Aula com creditos teoricos e praticos maiores que 0.\n"; exit(1); }

                     // Procurando nas aulas cadastradas, se existe
                     // alguma aula que possui os mesmos ídices de 
                     // dia da semana, sala e turma. Caso encontre,
                     // devo apenas add a oferta à aula existente.
                     bool novaAula = true;
                     Aula *aulaAntiga = NULL;
                     ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
                     {
                        if( ( itAula->getTurma() == turma )
                           && ( *(itAula->getDisciplina()) == *disciplina )
                           && ( itAula->getDiaSemana() == diaSemana )
                           && ( *(itAula->getSala()) == *sala ) 
                           && ( itAula->getCreditosPraticos() == creditos_praticos )
                           && ( itAula->getCreditosTeoricos() == creditos_teoricos ) )
                        {
                           aulaAntiga = *itAula;
                           novaAula = false;
                           problemData->aulas.remove(itAula);
                           break;
                        }
                     }

                     if( novaAula )
                     {
                        // Monta o objeto 'aula'
                        Aula * aula = new Aula();
                        aula->ofertas.add( problemData->refOfertas[ atendOferta->getOfertaCursoCampiId() ] );
                        aula->setTurma( turma );
                        aula->setDisciplina( disciplina );
                        aula->setSala( sala );
                        aula->setDiaSemana( diaSemana );
                        aula->setCreditosTeoricos( creditos_teoricos );
                        aula->setCreditosPraticos( creditos_praticos );
                        aula->setQuantidade((aula->getQuantidade() + demandaAtendida));

                        problemData->aulas.add( aula );
                     }
                     else
                     {
                        aulaAntiga->ofertas.add(problemData->refOfertas[ atendOferta->getOfertaCursoCampiId() ]);
                        aulaAntiga->setQuantidade((aulaAntiga->getQuantidade() + demandaAtendida));
                        problemData->aulas.add(aulaAntiga);
                     }
                  }
               }
            }
         }
      }

      // O método abaixo só pode ser executado
      // após a execução dos método de criação 
      // de blocos curriculares e de criação das aulas.
      relacionaBlocoCurricularAulas();

      std::cout << "Total de aulas criadas: "
                << problemData->aulas.size() << std::endl;
   }
}

void ProblemDataLoader::relacionaBlocoCurricularAulas()
{
   ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
   {
      Disciplina * disciplina = itAula->getDisciplina();

      ITERA_GGROUP_LESSPTR( itOferta, itAula->ofertas, Oferta )
      {
         Curso * curso = itOferta->curso;

         std::map< std::pair< Curso *, Disciplina * > , BlocoCurricular * >::iterator
            itMapCursoDisciplina_BlocoCurricular =
            problemData->mapCursoDisciplina_BlocoCurricular.find( std::make_pair( curso, disciplina ) );

         if ( itMapCursoDisciplina_BlocoCurricular
            != problemData->mapCursoDisciplina_BlocoCurricular.end() )
         {
            // Adicionando a aula ao bloco curricular e dia correspondentes.
            problemData->blocoCurricularDiaAulas
               [ itMapCursoDisciplina_BlocoCurricular->second ][ itAula->getDiaSemana() ].add( *itAula );

            // Adicionando o bloco curricular a aula correspondente.
            problemData->aulaBlocosCurriculares[ ( *itAula ) ].add(
               itMapCursoDisciplina_BlocoCurricular->second );
         }
         else
         {
            std::cout << "Na funcao <ProblemDataLoader::relacionaBlocoCurricularAulas()> "
               << "algum par <curso,disciplina> nao foi encontrado na estrutura "
               << "<mapCursoDisciplina_BlocoCurricular>." << std::endl;

            exit(1);
         }
      }
   }
}

void ProblemDataLoader::validaInputSolucaoTatico()
{
   if( ( problemData->parametros->modo_otimizacao == "OPERACIONAL")
         && ( problemData->atendimentosTatico != NULL ) )
   {
      GGroup< int /*id Sala*/> labs;

      ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
      {
         ITERA_GGROUP_LESSPTR( itUnd, itCampus->unidades, Unidade )
         {
            ITERA_GGROUP_LESSPTR( itSala, itUnd->salas, Sala )
            {
               if ( itSala->getTipoSalaId() == 2 )
               {
                  labs.add( itSala->getId() );
               }
            }
         }
      }

      ITERA_GGROUP( itAtCampus, ( *problemData->atendimentosTatico ), AtendimentoCampusSolucao )
      {
         ITERA_GGROUP( itAtUnd, itAtCampus->atendimentosUnidades, AtendimentoUnidadeSolucao )
         {
            ITERA_GGROUP( itAtSala, itAtUnd->atendimentosSalas, AtendimentoSalaSolucao )
            {
               if ( labs.find( itAtSala->getSalaId() ) != labs.end() )
               {
                  ITERA_GGROUP( itAtDiaSemana, itAtSala->atendimentosDiasSemana, AtendimentoDiaSemanaSolucao )
                  {
                     ITERA_GGROUP( itAtTatico, itAtDiaSemana->atendimentosTatico, AtendimentoTaticoSolucao )
                     {
                        int idDisc = itAtTatico->atendimento_oferta->getDisciplinaId();

                        ITERA_GGROUP_LESSPTR( itDisciplinaDiv, problemData->novasDisciplinas, Disciplina )
                        {
                           if ( -( itDisciplinaDiv->getId() ) == idDisc )
                           {
                              itAtTatico->atendimento_oferta->setDisciplinaId( ( -idDisc ) );
                              break;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::geraHorariosDia()
{
   int contador = 0;

   problemData->maxHorariosDif = 0;

   ITERA_GGROUP_LESSPTR(itTurno,problemData->todos_turnos,Turno)
   {
      Turno *turno = *itTurno;

      ITERA_GGROUP(itHA,turno->horarios_aula,HorarioAula)
      {
         HorarioAula *horarioAula = *itHA;

         if ( horarioAula->getId() > problemData->maxHorariosDif )
            problemData->maxHorariosDif = horarioAula->getId();

         ITERA_GGROUP_N_PT(itDia,horarioAula->dias_semana,int)
         {
            HorarioDia *horarioDia = new HorarioDia();
            horarioDia->setId(contador);
            horarioDia->setHorarioAulaId(horarioAula->getId());
            horarioDia->setHorarioAula(horarioAula);
            horarioDia->setDia(*itDia);
            contador++;

            problemData->horariosDia.add(horarioDia);
         }
      }
   }

   problemData->horariosDiaIdx.resize((problemData->maxHorariosDif+1)*8,NULL);

   ITERA_GGROUP_LESSPTR(itHorario,problemData->horariosDia,HorarioDia)
   {
      HorarioDia *horarioDia = *itHorario;

      problemData->horariosDiaIdx[problemData->getHorarioDiaIdx(horarioDia)] = horarioDia;
   }

   // Professores
   ITERA_GGROUP_LESSPTR(itCamp,problemData->campi,Campus)
   {
      Campus *campus = *itCamp;

      ITERA_GGROUP_LESSPTR(itProf,campus->professores,Professor)
      {
         Professor *professor = *itProf;

         ITERA_GGROUP(itHor,professor->horarios,Horario)
         {
            Horario *horario = *itHor;

            ITERA_GGROUP_N_PT(itD,horario->dias_semana,int)
            {
               HorarioDia *auxHD = new HorarioDia();
               auxHD->setDia(*itD);
               auxHD->setHorarioAula(horario->horario_aula);
               auxHD->setHorarioAulaId(horario->getHorarioAulaId());

               GGroup<HorarioDia*,LessPtr<HorarioDia> >::iterator itHorarioAula = problemData->horariosDia.find(auxHD);

               if ( itHorarioAula == problemData->horariosDia.end() )
               {
                  printf("ERRO: HORARIODIA NAO ENCONTRADO\n");
                  exit(1);
               }

               delete auxHD;
               professor->horariosDia.add(*itHorarioAula);
            }
            
         }
      }
   }

   // Salas
   ITERA_GGROUP_LESSPTR(itCamp,problemData->campi,Campus)
   {
      Campus *campus = *itCamp;

      ITERA_GGROUP_LESSPTR(itUnidade,campus->unidades,Unidade)
      {
         Unidade* unidade = *itUnidade;

         ITERA_GGROUP_LESSPTR(itSala,unidade->salas,Sala)
         {
            Sala* sala = *itSala;

            ITERA_GGROUP(itHor,sala->horarios_disponiveis,Horario)
            {
               Horario *horario = *itHor;

               ITERA_GGROUP_N_PT(itD,horario->dias_semana,int)
               {
                  HorarioDia *auxHD = new HorarioDia();
                  auxHD->setDia(*itD);
                  auxHD->setHorarioAula(horario->horario_aula);
                  auxHD->setHorarioAulaId(horario->getHorarioAulaId());

                  GGroup<HorarioDia*,LessPtr<HorarioDia> >::iterator itHorarioAula = problemData->horariosDia.find(auxHD);

                  if ( itHorarioAula == problemData->horariosDia.end() )
                  {
                     printf("ERRO: HORARIODIA NAO ENCONTRADO\n");
                     exit(1);
                  }

                  delete auxHD;
                  sala->horariosDia.add(*itHorarioAula);
               }      
            }
         }
      }
   }

   // Disciplinas
   ITERA_GGROUP_LESSPTR(itDisc,problemData->disciplinas,Disciplina)
   {
      Disciplina *disciplina = *itDisc;

      ITERA_GGROUP(itHor,disciplina->horarios,Horario)
      {
         Horario *horario = *itHor;

         ITERA_GGROUP_N_PT(itD,horario->dias_semana,int)
         {
            HorarioDia *auxHD = new HorarioDia();
            auxHD->setDia(*itD);
            auxHD->setHorarioAula(horario->horario_aula);
            auxHD->setHorarioAulaId(horario->getHorarioAulaId());

            GGroup<HorarioDia*,LessPtr<HorarioDia> >::iterator itHorarioAula = problemData->horariosDia.find(auxHD);

            if ( itHorarioAula == problemData->horariosDia.end() )
            {
               printf("ERRO: HORARIODIA NAO ENCONTRADO\n");
               exit(1);
            }

            delete auxHD;
            disciplina->horariosDia.add(*itHorarioAula);
         }      
      }
   }
}
