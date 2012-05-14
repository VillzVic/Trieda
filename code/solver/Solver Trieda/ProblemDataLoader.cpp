#include "ProblemDataLoader.h"
#include "ProblemSolution.h"
#include "TRIEDA-InputXSD.h"
#include "GGroup.h"

#include <iostream>

ProblemDataLoader::ProblemDataLoader(
   char * inputFile, ProblemData * data )
{
   this->inputFile = inputFile;
   this->problemData = data;
}

ProblemDataLoader::~ProblemDataLoader()
{

}

void ProblemDataLoader::load()
{
   std::cout << "Carregando o arquivo..." << std::endl;

   root = std::auto_ptr< TriedaInput >(
      TriedaInput_( inputFile, xml_schema::flags::dont_validate ) );

   std::cout << "Extraindo dados..." << std::endl;

   problemData->le_arvore( *root );
   
   std::cout << "Pre-processamento..." << std::endl << std::endl;

   // ---------
   preencheTempoAulaHorarios();

   std::cout << "Relacionando demanda a alunos..." << std::endl;
   // ---------
   relacionaDemandaAlunos();

   std::cout << "Relacionando calendario horarios e aulas..." << std::endl;
   // ---------
   relacionaCalendarioHorariosAula();

   std::cout << "Referenciando ofertas..." << std::endl;
   // ---------
   geraRefsOfertas();

   std::cout << "Referenciando demandas..." << std::endl;
   // ---------
   geraRefsDemandas();

   std::cout << "Referenciando disciplinas equivalentes e incompatibilidades..." << std::endl;
   // ---------
   referenciaDisciplinasEquivalentesIncompativeis();

   std::cout << "Referenciando disciplinas curriculos..." << std::endl;
   // ---------
   referenciaDisciplinasCurriculos();

   std::cout << "Referencia os cursos, os calendarios dos curriculos e os calendarios das disciplinas..." << std::endl;
   // ---------
   referenciaCursos_DiscCalendarios();

   std::cout << "Referencia disciplinas equivalentes..." << std::endl;
   // ---------
   referenciaDisciplinasEquivalentes();

   std::cout << "Removendo fixacoes com disciplinas substituidas..." << std::endl;
   // ---------
   removeFixacoesComDisciplinasSubstituidas();

   std::cout << "Referencia calendarios dos curriculos..." << std::endl;
   // ---------
   referenciaCalendariosCurriculos();

   std::cout << "Relacionando horarios aula dia semana..." << std::endl;
   // ---------
   relacionaHorariosAulaDiaSemana();

   std::cout << "Relacionando creds regras..." << std::endl;
   // ---------
   relacionaCredsRegras();

   std::cout << "Carregando dias letivos campus unidade sala..." << std::endl;
   // ---------
   carregaDiasLetivosCampusUnidadeSala();

   std::cout << "Carregando dias letivos disciplinas..." << std::endl;
   // ---------
   carregaDiasLetivosDiscs();

   std::cout << "Processando disciplinas de cursos compativeis..." << std::endl;
   // ---------
   disciplinasCursosCompativeis();

   //std::cout << "Relacionando disciplinas equivalentes..." << std::endl;
   // ---------
   //relacionaDisciplinasEquivalentes();

   std::cout << "Preenchendo disciplinas de cursos compativeis..." << std::endl;
   // ---------
   preencheDisciplinasDeCursosCompativeis();

   std::cout << "Preenchendo disciplinas de ofertas compativeis..." << std::endl;
   // ---------
   preencheDisciplinasDeOfertasCompativeis();

   std::cout << "Dividindo disciplinas..." << std::endl;
   // ---------
   divideDisciplinas();

   std::cout << "Validando input tatico..." << std::endl;
   // ---------
   validaInputSolucaoTatico();

   std::cout << "Referenciando campus unidades e salas..." << std::endl;
   // ---------
   referenciaCampusUnidadesSalas();

   std::cout << "Referenciando disciplinas..." << std::endl;
   // ---------
   referenciaDisciplinas();

   std::cout << "Referenciando ofertas..." << std::endl;
   // ---------
   referenciaOfertas();

   std::cout << "Gerando referencias..." << std::endl;
   // ---------
   gera_refs();

   std::cout << "Criando alunos..." << std::endl;
   // ---------
   criaAlunos();

   std::cout << "Corrigindo fixacoes com disciplinas e salas..." << std::endl;
   // ---------
   corrigeFixacoesComDisciplinasSalas();

   std::cout << "Calculando demandas..." << std::endl;
   // ---------
   calculaDemandas();

   std::cout << "Relacionando cursos a campus..." << std::endl;
   // ---------
   relacionaCursosCampus();

   std::cout << "Calculando tamanho das salas..." << std::endl;
   // ---------
   calculaTamanhoMedioSalasCampus();

   std::cout << "Relacionando campus a disciplinas..." << std::endl;
   // ---------
   relacionaCampusDiscs();

   std::cout << "Relacionado disciplinas e ofertas..." << std::endl;
   // ---------
   relacionaDiscOfertas();
   
   std::cout << "Associando disciplinas a salas..." << std::endl;
   // ---------
   associaDisciplinasSalas();
   
   std::cout << "Calculando media capacidade de sala por disc..." << std::endl;
   // ---------   
   calculaCapacMediaSalaPorDisc();

   std::cout << "Estimando turmas..." << std::endl;
   // ---------
   estima_turmas();

   std::cout << "Criando blocos curriculares..." << std::endl;
   // ---------
   cria_blocos_curriculares();

   std::cout << "Criando cjt salas..." << std::endl;
   // ---------
   criaConjuntoSalasUnidade();

   std::cout << "Associando disciplinas a cjt de salas..." << std::endl;
   // ---------
   associaDisciplinasConjuntoSalas();

   std::cout << "Cache..." << std::endl;
   // ---------
   cache();

   std::cout << "Estabelecendo Dias Letivos Bloco Campus..." << std::endl;
   // ---------
   estabeleceDiasLetivosBlocoCampus();

   std::cout << "Estabelecendo Dias Letivos Disciplinas Salas..." << std::endl;
   // ---------
   estabeleceDiasLetivosDisciplinasSalas();

   std::cout << "Estabelecendo Dias Letivos Disciplinas Cjt Salas..." << std::endl;
   // ---------
   estabeleceDiasLetivosDiscCjtSala();

   std::cout << "Calculando creditos livres de salas..." << std::endl;
   // ---------
   calculaCredsLivresSalas();

   std::cout << "Calculando divisão de creditos..." << std::endl;
   // ---------
   combinacaoDivCreditos();

   std::cout << "Estabelecendo DiasLetivos Professor Disciplina..." << std::endl;
   // --------- 
   estabeleceDiasLetivosProfessorDisciplina();

   std::cout << "Criando aulas..." << std::endl;
   // --------- 
   criaAulas();

   std::cout << "Calculando maximo de horarios professor..." << std::endl;
   // ---------
   calculaMaxHorariosProfessor();

   std::cout << "Criando listas de horarios ordenados..." << std::endl;
   // ---------
   criaListaHorariosOrdenados();
   
   std::cout << "Verificando fixacoes de dias letivos das disciplinas..." << std::endl;
   // ---------
   verificaFixacoesDiasLetivosDisciplinas();

   std::cout << "Relacionando fixacoes..." << std::endl;
   // ---------
   relacionaFixacoes();

   std::cout << "Gerando horariosDia..." << std::endl;
   // ---------
   geraHorariosDia();

   // ---------
   // print_stats();

   std::cout << "Referenciando horarios aula..." << std::endl;
   // ---------
   referenciaHorariosAula();

   std::cout << "Referenciando turnos..." << std::endl;
   // ---------
   referenciaTurnos();

   std::cout << "Relacionando professor disciplinas associadas..." << std::endl;
   // ---------
   relacionaProfessorDisciplinasAssociadas();

   std::cout << "Calculando maximo de tempo disponivel por sala..." << std::endl;
   
   calculaMaxTempoDisponivelPorSala();

   std::cout << "Calculando maximo de tempo disponivel por sala e semana letiva..." << std::endl;

   calculaMaxTempoDisponivelPorSalaPorSL();

   std::cout << "Calculando compatibilidade de horarios..." << std::endl;

   calculaCompatibilidadeDeHorarios();

   std::cout << "Calculando combinacao de creditos por sala..." << std::endl;

   calculaCombinaCredSLPorSala();

   std::cout << "Calculando combinacao de creditos por bloco curricular..." << std::endl;

   calculaCombinaCredSLPorBlocoCurric();

   std::cout << "Calculando combinacao de creditos por aluno..." << std::endl;

   calculaCombinaCredSLPorAluno();

}

void ProblemDataLoader::preencheTempoAulaHorarios()
{
   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
      ITERA_GGROUP_LESSPTR( it_turno, it_calendario->turnos, Turno )
      {
         ITERA_GGROUP_LESSPTR( it_horario_aula, it_turno->horarios_aula, HorarioAula )
         {
            HorarioAula * horario_aula = ( *it_horario_aula );
            horario_aula->setTempoAula( it_calendario->getTempoAula() );
         }
      }
   }
}

// Preenche o map problemData->mapDemandaAlunos
void ProblemDataLoader::relacionaDemandaAlunos()
{	
   ITERA_GGROUP_LESSPTR( it_demanda,
      this->problemData->demandas, Demanda )
   {
      Demanda * demanda = ( *it_demanda );

      ITERA_GGROUP_LESSPTR( it_aluno_demanda,
         this->problemData->alunosDemanda, AlunoDemanda )
      {
         AlunoDemanda * aluno_demanda = ( *it_aluno_demanda );

         if ( demanda->getId() == aluno_demanda->getDemandaId() )
         {
            this->problemData->mapDemandaAlunos[ demanda ].add( aluno_demanda );
         }
      }

      int quantidade = this->problemData->mapDemandaAlunos[ demanda ].size();
      if ( quantidade > 0 )
      {
         demanda->setQuantidade( quantidade );
      }
   }
}

// Método relacionado com a issue TRIEDA-1054
void ProblemDataLoader::relacionaCalendarioHorariosAula()
{
   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
      Calendario * calendario = ( *it_calendario );

      ITERA_GGROUP_LESSPTR( it_turno, calendario->turnos, Turno )
      {
         Turno * turno = ( *it_turno );

         ITERA_GGROUP_LESSPTR( it_horario_aula, it_turno->horarios_aula, HorarioAula )
         {
            HorarioAula * horario_aula = ( *it_horario_aula );

            horario_aula->setCalendario( calendario );
         }
      }
   }
}

void ProblemDataLoader::geraRefsOfertas()
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
}

void ProblemDataLoader::geraRefsDemandas()
{
   ITERA_GGROUP_LESSPTR( it_dem, problemData->demandasTotal, Demanda ) 
   {
      find_and_set_lessptr( it_dem->getOfertaId(),
         problemData->ofertas,
         it_dem->oferta, false );

      find_and_set_lessptr( it_dem->getDisciplinaId(),
         problemData->disciplinas,
         it_dem->disciplina, false );
   }
}

void ProblemDataLoader::criaAlunos()
{
   ITERA_GGROUP_LESSPTR( itAlunoDem, problemData->alunosDemanda, AlunoDemanda )
   {
	   AlunoDemanda *alunoDemanda = *itAlunoDem;

	   int id = alunoDemanda->getAlunoId();
	   std::string nome = alunoDemanda->getNomeAluno();
	   int demId = alunoDemanda->getDemandaId();
	   Oferta *oft = alunoDemanda->demanda->oferta;

	   // Verifica se o aluno já não existe
	   bool EXISTE = false;
	   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	   {
		   if ( itAluno->getAlunoId() == id )
		   {
			   itAluno->demandas.add( alunoDemanda );

			   EXISTE = true;
			   if ( itAluno->getOfertaId() != oft->getId() )
			   {
					std::cout<<"\nATENCAO em ProblemDataLoader::criaAlunos: o alunoId "<<id<<" esta associado a mais de uma oferta!!\n";
			   }
		   }
	   }
	   	   
	   // Se o aluno não existir ainda, o insere
	   if ( !EXISTE )
	   {
		   Aluno *aluno = new Aluno( id, nome, oft );
		   problemData->alunos.add( aluno );
		   aluno->demandas.add( alunoDemanda );
	   }
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

   std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator it_map_horarios
      = problemData->horarios_aula_dia_semana.begin();

   // Adiciona os horários de aula, sem repetição
   for (; it_map_horarios != problemData->horarios_aula_dia_semana.end();
          it_map_horarios++ )
   {
      GGroup< HorarioAula *, LessPtr< HorarioAula > > horarios
         = it_map_horarios->second;

      ITERA_GGROUP_LESSPTR( it_horario, horarios, HorarioAula )
      {
         horarios_aula.add( *it_horario );
      }
   }

   // Insere os horarios de aula ( distintos ) no vector
   ITERA_GGROUP( it_h, horarios_aula, HorarioAula )
   {
      problemData->horarios_aula_ordenados.push_back( *it_h );

      problemData->mapCalendarioHorariosAulaOrdenados[ it_h->getCalendario() ].push_back( *it_h );
   }

   // Ordena os horarios de aula pelo inicio de cada um
   std::sort( problemData->horarios_aula_ordenados.begin(),
              problemData->horarios_aula_ordenados.end(), ordena_horarios_aula );

   std::map< Calendario *, std::vector< HorarioAula * > >::iterator
      it_map = problemData->mapCalendarioHorariosAulaOrdenados.begin();

   for (; it_map != problemData->mapCalendarioHorariosAulaOrdenados.end();
          it_map++ )
   {
      std::sort( ( *it_map ).second.begin(),
                 ( *it_map ).second.end(), ordena_horarios_aula );
   }
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

   ITERA_GGROUP_LESSPTR( it_oferta,
      problemData->ofertas, Oferta )
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
                   << "\ncampus e/ou curso valido( s )." << std::endl;

         exit( 1 );
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
      ITERA_GGROUP_LESSPTR( it_horario_aula, it_turno->horarios_aula, HorarioAula )
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
            (*it_Campus)->diasLetivos.add( *it_Dias_Letivos );
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
               (*it_Unidade)->dias_letivos.add( *it_Dias_Letivos );
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
                ITERA_GGROUP ( itHorarioDisponivel, it_Sala->horarios_disponiveis, Horario )
                {
                    (*it_Sala)->diasLetivos.add( itHorarioDisponivel->dias_semana );
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
            (*it_Disc)->diasLetivos.add( *it_Dias_Letivos );
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
         // ser criadas, dado que possuiam pelo menos 
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
            // tem que ser nula ( ou seja, todas as disciplinas que forem
            // associadas a sala em questão, tem de estar associadas ao
            // conjunto de salas encontrado, e vice versa ).
            bool encontrou_Conjunto_Compat = false;
			
			bool PERMITE_MAIS_DE_UMA_SALA_POR_CJT = false;	// TODO: tirar isso quando estiver estavel
			
			if ( PERMITE_MAIS_DE_UMA_SALA_POR_CJT ){

            ITERA_GGROUP( it_Cjt_Salas_Disc,
               ( *gg_Cjt_Salas_Esc ), ConjuntoSala )
            {
               // Se o conjunto de salas em questão representa a capacidade
               // da sala em questão. Estou testando tb se o conjunto de
               // salas representa o mesmo tipo da sala em questão. Acredito
               // que não seja necessário no caso em que estejamos lidando
               // APENAS com laboratórios. Como não sei qual GGroup está
               // sendo referenciado, testo os 2 casos pra lá.
               if (  it_Cjt_Salas_Disc->getCapacidadeRepr() == it_Sala->getCapacidade()
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
                     (*it_Cjt_Salas_Disc)->addSala( **it_Sala );

                     // Adicionando os dias letivos ao conjunto de salas
                     ITERA_GGROUP( it_disc, it_Sala->disciplinasAssociadas, Disciplina )
                     {
                        GGroup< int > dias_fixados = retorna_foxacoes_dias_letivos( *it_disc );

                        if ( dias_fixados.size() == 0 )
                        {
                           ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Sala->diasLetivos, int )
                           {
                              (*it_Cjt_Salas_Disc)->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
                           }
                        }
                        else
                        {
                           ITERA_GGROUP_N_PT( it_Dias_Letivos, dias_fixados, int )
                           {
                              (*it_Cjt_Salas_Disc)->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
                           }
                        }
                     }

                     // COMO AS DISCIPLINAS ASSOCIADAS SÃO AS MESMAS,
                     // NÃO HÁ NECESSIDADE DE ADICIONAR NENHUMA
                     // DISICIPLINA ASSOCIADA AO CONJUNTO DE SALAS EM QUESTÃO.
                     encontrou_Conjunto_Compat = true;

                     break;
                  }
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
            (*it_Unidade)->conjutoSalas.add( *it_Cjt_Salas_Disc_Elab );
         }

         ITERA_GGROUP( it_Cjt_Salas_Disc_GERAL,
            conjunto_Salas_Disc_GERAL, ConjuntoSala )
         {
            (*it_Unidade)->conjutoSalas.add( *it_Cjt_Salas_Disc_GERAL );
         }

         // std::cout << "Cod. Und.: " << it_Unidade->getCodigo() << std::endl;

         ITERA_GGROUP_LESSPTR( it_Cjt_Salas_Und,
            it_Unidade->conjutoSalas, ConjuntoSala )
         {
            // std::cout << "\tCod. Cjt. Sala: " << it_Cjt_Salas_Und->getId() << std::endl;

            std::map< int /*Id Sala*/, Sala * >::iterator 
               it_Salas_Cjt = it_Cjt_Salas_Und->salas.begin();

            for(; it_Salas_Cjt != it_Cjt_Salas_Und->salas.end();
               it_Salas_Cjt++ )
            {
               // std::cout << "\t\tCod. Sala: "
               //           << it_Salas_Cjt->second->getCodigo()
               //           << std::endl;
            }
         }
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
	if ( problemData->parametros->otimizarPor == "ALUNO" )
	{
		return;
	}

   // Analisar esse metodo e o de criacao de blocos curriculares.
   // Um bloco pode pertencer a mais de um campus !?
   ITERA_GGROUP_LESSPTR( it_Bloco_Curric, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_N_PT( it_Dia_Letivo, it_Bloco_Curric->diasLetivos, int )
      {
         if ( it_Bloco_Curric->campus->diasLetivos.find
            (*it_Dia_Letivo) != it_Bloco_Curric->campus->diasLetivos.end() )
         {
            problemData->bloco_Campus_Dias[ 
				std::make_pair( it_Bloco_Curric->getId(),
								it_Bloco_Curric->campus->getId() ) ].add( *it_Dia_Letivo );
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

						// Adicionando informações referentes aos horários
						// comuns entre uma sala e uma disciplina para um dado dia.
						ITERA_GGROUP( itHorarioSala, itSala->horarios_disponiveis, Horario )
						{
							// Checando o dia em questão para a sala
							if ( itHorarioSala->dias_semana.find( *itDiasLetivosDisc )
								!= itHorarioSala->dias_semana.end() )
							{
								ITERA_GGROUP( itHorarioDisc, itDiscAssoc->horarios, Horario )
								{
									// Checando o dia em questão para a disciplina
									if ( itHorarioDisc->dias_semana.find( *itDiasLetivosDisc )
									!= itHorarioDisc->dias_semana.end() )
									{
									// Checando se é um horário comum entre a disc e a sala.
									if ( itHorarioSala->horario_aula == itHorarioDisc->horario_aula )
									{
										problemData->disc_Salas_Dias_HorariosAula
											[ ids_Disc_Sala ][ ( *itDiasLetivosDisc ) ].add(
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
   //	(*itSala)->credsLivres.push_back(0);
   //}

   //         ITERA_GGROUP( itCredsDisp, itSala->creditos_disponiveis, CreditoDisponivel )
   //         {
   //	(*itSala)->credsLivres.at( itCredsDisp->dia_semana ) = itCredsDisp->max_creditos;
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
		ITERA_GGROUP_LESSPTR( it_prof, itCampus->professores, Professor )
		{							  
			ITERA_GGROUP_LESSPTR( it_mag, it_prof->magisterio, Magisterio )
			{
				int discId = it_mag->getDisciplinaId();
				Disciplina *disc = problemData->refDisciplinas[ discId ];

				ITERA_GGROUP( it_hor, it_prof->horarios, Horario )
				{
					GGroup< int >::iterator itDiasLetDisc =
					disc->diasLetivos.begin();

					for(; itDiasLetDisc != disc->diasLetivos.end(); itDiasLetDisc++ )
					{
						if ( it_hor->dias_semana.find( *itDiasLetDisc )
							!= it_hor->dias_semana.end() )
						{
							std::pair< int, int > ids_Prof_Disc 
								( it_prof->getId(), discId );

							problemData->prof_Disc_Dias[ ids_Prof_Disc ].add( *itDiasLetDisc );
							problemData->disc_Dias_Prof_Tatico[ discId ].add( *itDiasLetDisc );
							problemData->usarProfDispDiscTatico = true;
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

               for (; itDiasLetivosDiscs != itDisc->diasLetivos.end();
                      itDiasLetivosDiscs++ )
               { 
                  if ( vAux[ a ].first == ( *itDiasLetivosDiscs ) )
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
               if ( vAux[ b ].second != 0 )
               {
                  GGroup< int >::iterator itDiasLetivosDiscs
                     = itDisc->diasLetivos.begin();

                  for (; itDiasLetivosDiscs != itDisc->diasLetivos.end();
                         itDiasLetivosDiscs++ )
                  { 
                     if ( vAux[ b ].first == ( *itDiasLetivosDiscs ) )
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
               (*itDisc)->combinacao_divisao_creditos.push_back( vAux );
               atualiza = false;
            }	
         }
      }
   }
}

/*
	Define um map de compatibilidade e incompatibilidade entre 2 cursos.
	É setado mesmo que não seja permitido compartilhamento.
*/
void ProblemDataLoader::disciplinasCursosCompativeis()
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

	   ITERA_GGROUP_LESSPTR( it_fix_curso, problemData->cursos, Curso )
	   {
		  int curso1Id = it_fix_curso->getId();

		  GGroup< GGroup< int > * >::iterator it_list_compat =
			 problemData->parametros->nao_permite_compart_turma.begin();
		  
		  for (; it_list_compat != problemData->parametros->nao_permite_compart_turma.end(); it_list_compat++ )
		  {
			 if ( it_list_compat->find( curso1Id ) != it_list_compat->end() )
			 {
				ITERA_GGROUP_INIC_LESSPTR( it_alt_curso, it_fix_curso, problemData->cursos, Curso )
				{
				   int curso2Id = it_alt_curso->getId();
				   if ( curso1Id == curso2Id ) continue;

				   if ( it_list_compat->find( curso2Id ) != it_list_compat->end() )
				   {
					  std::pair<Curso*, Curso*> parCursos = std::make_pair( *it_fix_curso, *it_alt_curso );
					  
					  problemData->compat_cursos.erase( parCursos );
					  std::swap(parCursos.first, parCursos.second);
					  problemData->compat_cursos.erase( parCursos );
				   }
				}
			 }
		  }
	   }

}


void ProblemDataLoader::preencheDisciplinasDeCursosCompativeis()
{
	problemData->preencheCursosCompDisc();
}

void ProblemDataLoader::preencheDisciplinasDeOfertasCompativeis()
{
	problemData->preencheOftsCompDisc();
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

      if ( professor != NULL )
      {
         if ( fixacao->professor == NULL
            || professor->getId() != fixacao->professor->getId() )
         {
            continue;
         }
      }

      if ( disciplina != NULL )
      {
         if ( fixacao->disciplina == NULL
            || disciplina->getId() != fixacao->disciplina->getId() )
         {
            continue;
         }
      }

      if ( sala != NULL )
      {
         if ( fixacao->sala == NULL
            || sala->getId() != fixacao->sala->getId() )
         {
            continue;
         }
      }

      if ( dia_semana >= 0 )
      {
         if ( fixacao->getDiaSemana() < 0
            || dia_semana != fixacao->getDiaSemana() )
         {
            fixacao->setDiaSemana( dia_semana );
         }
      }

      if ( horario_aula != NULL )
      {
         if ( fixacao->horario_aula == NULL
            || horario_aula->getId() != fixacao->horario_aula->getId() )
         {
            continue;
         }
      }

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

      if ( professor != NULL || fixacao->professor != NULL )
      {
         if ( ( professor != NULL && fixacao->professor == NULL )
            || ( professor == NULL && fixacao->professor != NULL )
            || ( professor->getId() != fixacao->professor->getId() ) )
         {
            continue;
         }
      }

      if ( disciplina != NULL || fixacao->disciplina != NULL )
      {
         if ( ( disciplina != NULL && fixacao->disciplina == NULL )
            || ( disciplina == NULL && fixacao->disciplina != NULL )
            || ( disciplina->getId() != fixacao->disciplina->getId() ) )
         {
            continue;
         }
      }

      if ( sala != NULL || fixacao->sala != NULL )
      {
         if ( ( sala != NULL && fixacao->sala == NULL )
            || ( sala == NULL && fixacao->sala != NULL )
            || ( sala->getId() != fixacao->sala->getId() ) )
         {
            continue;
         }
      }

      if ( dia_semana >= 0 || fixacao->getDiaSemana() >= 0 )
      {
         if ( ( dia_semana >= 0 && fixacao->getDiaSemana() < 0 )
            || ( dia_semana < 0 && fixacao->getDiaSemana() >= 0 )
            || ( dia_semana != fixacao->getDiaSemana() ) )
         {
            continue;
         }
      }

      if ( horario_aula != NULL || fixacao->horario_aula != NULL )
      {
         if ( ( horario_aula != NULL && fixacao->horario_aula == NULL )
            || ( horario_aula == NULL && fixacao->horario_aula != NULL )
            || ( horario_aula->getId() != fixacao->horario_aula->getId() ) )
         {
            continue;
         }
      }

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

void ProblemDataLoader::removeFixacoesComDisciplinasSubstituidas()
{
	bool recomecar = true;

	// Remove todas as fixações referentes a disciplinas que foram substituídas
	while ( recomecar )
	{
	   recomecar = false;

	   ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
	   {
		   int discId = it_fixacao->getDisciplinaId();

		   // Para fixações com disciplinas
		   if ( discId != -1 )
		   {
			   Disciplina *d = problemData->refDisciplinas[ discId ];

			   // Se a disciplina d tiver sido substituída
			   if ( problemData->mapDiscSubstituidaPor.find( d ) ==
					problemData->mapDiscSubstituidaPor.end() )
			   {
				   // Remove a fixação referente a d
				   GGroup< Fixacao *, LessPtr< Fixacao > >::iterator it = problemData->fixacoes.remove( *it_fixacao );
				   recomecar = true;
				   break;
			   }
		   }
	   }
	}
}

void ProblemDataLoader::corrigeFixacoesComDisciplinasSalas()
{
	// Relaciona corretamente as fixações de salas para disciplinas práticas
	ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
	{
		int discId = it_fixacao->getDisciplinaId();
		int salaId = it_fixacao->getSalaId();
		
		Sala * sala = it_fixacao->sala;

		// Para fixações com disciplinas e salas
		if ( discId != -1 && salaId != -1)
		{
			int credsP = it_fixacao->disciplina->getCredPraticos();

			if ( sala->tipo_sala->getNome() == "Laboratório" &&
				 it_fixacao->disciplina->getCredPraticos() == 0 )
			{
				Disciplina *discPratica = problemData->refDisciplinas[ -discId ];

				if ( discPratica != NULL )
				{
					it_fixacao->disciplina = discPratica;
					it_fixacao->setDisciplinaId( -discId );				
				}
				else
				{
					std::cout << "\nAtencao em ProblemDataLoader::corrigeFixacoesComDisciplinasSalas()" << endl;
					std::cout << "Fixacao da disciplina teorica " << discId << "no laboratorio " << salaId << endl;
				}
			}
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

   ITERA_GGROUP_LESSPTR( it_fixacao,
      this->problemData->fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );
      disciplina = ( fixacao->disciplina );
      dia_semana = ( fixacao->getDiaSemana() );

      // Verifica se o dia da semana e a disciplina foram fixados
      if ( disciplina != NULL && dia_semana >= 0 )
      {
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
      }
   }
}

template< class T > 
void ProblemDataLoader::find_and_set(
   int id, GGroup< T * > & haystack,
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

      exit( 1 );
   }

   delete finder;
}

template< class T > 
void ProblemDataLoader::find_and_set_lessptr(
   int id, GGroup< T *, LessPtr< T > > & haystack,
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

      exit( 1 );
   }

   delete finder;
}

void ProblemDataLoader::substituiDisciplinasEquivalentes()
{
    bool atualizar_demandas = false;

    ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
    {
		Curso *curso = *it_curso;

        ITERA_GGROUP_LESSPTR( it_curriculo, curso->curriculos, Curriculo )
        {
			Curriculo *curriculo = *it_curriculo;

			map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_periodo_disc
				= curriculo->disciplinas_periodo.begin();
			
			bool reiniciar = false;
			
			// Para cada disciplina do 'curriculo', procuro se devo
			// substituir por alguma disciplina equivalente
			while ( it_periodo_disc != curriculo->disciplinas_periodo.end() )
			{
				Disciplina *discAntiga = it_periodo_disc->first;
				int periodo = it_periodo_disc->second;

				std::map< Disciplina*, Disciplina* >::iterator
					itMapEquiv = problemData->mapDiscSubstituidaPor.find( discAntiga );

				if ( itMapEquiv != problemData->mapDiscSubstituidaPor.end() )
				{

				//-----------------------------------------------------------------------------
				// Substituir disciplinas equivalentes
					Disciplina * discNova = problemData->mapDiscSubstituidaPor[ discAntiga ];

					atualizar_demandas = true;

					// Troca a disciplina e mantém o período
					curriculo->disciplinas_periodo.erase( discAntiga );
					curriculo->disciplinas_periodo[discNova] = periodo;

					reiniciar = true;
					
                    // Adiciona a 'discAntiga' no conjunto de disciplinas que foram substituídas por 'discNova'
					std::pair< Curso*, Curriculo* > parCursoCurr = std::make_pair( curso, curriculo );
                    problemData->mapGroupDisciplinasSubstituidas[ parCursoCurr ][ discNova ].add( discAntiga );

					if ( problemData->mapGroupDisciplinasSubstituidas[ parCursoCurr ][ discNova ].size() > 1 )
					{
						std::cout << "\nATENCAO em void ProblemDataLoader::substituiDisciplinasEquivalentes():"
								  << " a disciplina " << discNova->getId() << " esta substituindo "
								  << "mais de uma disciplina no curriculo " << curriculo->getId() << "\n";
					}

					it_periodo_disc = curriculo->disciplinas_periodo.begin();
				 //-----------------------------------------------------------------------------
				 }

				 if ( reiniciar )
				 {
					 it_periodo_disc = curriculo->disciplinas_periodo.begin();
					 reiniciar = false;
				 }
				 else
				 {
					 it_periodo_disc++;
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
    int id_demanda = retornaMaiorIdDemandas();
    id_demanda++;

    // A partir de cada demanda existente de disciplinas que
    // forem substituídas, troca-se a disciplina pela substituta
    ITERA_GGROUP_LESSPTR( itDemanda, problemData->demandasTotal, Demanda )
    {
		if ( problemData->mapDiscSubstituidaPor.find( itDemanda->disciplina ) ==
			problemData->mapDiscSubstituidaPor.end() )
		{
			continue; // Disciplina que não foi substituída
		}
			
		Disciplina *discAntiga = (*itDemanda)->disciplina;
		Disciplina *discNova = problemData->mapDiscSubstituidaPor[ discAntiga ];
			  
		itDemanda->setDisciplinaId( discNova->getId() );
		itDemanda->disciplina = discNova;	
	}
}

// Retorna o maior id das demandas já cadastradas
int ProblemDataLoader::retornaMaiorIdDemandas()
{
   int id_demanda = -1;

   ITERA_GGROUP_LESSPTR( it_demanda, problemData->demandasTotal, Demanda )
   {
      if ( it_demanda->getId() > id_demanda )
      {
         id_demanda = it_demanda->getId();
      }
   }

   return id_demanda;
}

// Retorna o maior id das demandas já cadastradas
int ProblemDataLoader::retornaMaiorIdAlunoDemandas()
{
   int alDemId = -1;

   ITERA_GGROUP_LESSPTR( it_alunoDemanda, problemData->alunosDemandaTotal, AlunoDemanda )
   {
      if ( it_alunoDemanda->getId() > alDemId )
      {
         alDemId = it_alunoDemanda->getId();
      }
   }

   return alDemId;
}


void ProblemDataLoader::referenciaCursos_DiscCalendarios()
{ 
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
   
   // Referencia a semana letiva (calendario) de cada disciplina
   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
      {
		  map < Disciplina*, int, LessPtr< Disciplina > >::iterator
			  itPeriodoDisc = it_curriculo->disciplinas_periodo.begin();

		  for ( ; itPeriodoDisc != it_curriculo->disciplinas_periodo.end(); itPeriodoDisc++ )
		  {
			  itPeriodoDisc->first->setCalendario( it_curriculo->calendario );
		  }
      }
   }

}

void ProblemDataLoader::referenciaCalendariosCurriculos()
{    
   // Referencia as semanas letivas (calendarios) de cada curriculo
   // Tem que ser chamada depois das substituições de equivalência!

   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
      {
		  map < Disciplina*, int, LessPtr< Disciplina > >::iterator
			  itPeriodoDisc = it_curriculo->disciplinas_periodo.begin();

		  for ( ; itPeriodoDisc != it_curriculo->disciplinas_periodo.end(); itPeriodoDisc++ )
		  {
			  it_curriculo->semanasLetivas[itPeriodoDisc->second].add( itPeriodoDisc->first->getCalendario() );
		  }
      }
   }

}

void ProblemDataLoader::referenciaDisciplinasEquivalentes()
{
	// Não considera transitividade entre equivalências
	if ( ! problemData->EQUIV_TRANSITIVIDADE )
	{
		// Preenche mapDiscSubstituidaPor
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
		{
			Disciplina *discNova = *itDisc;

			ITERA_GGROUP_LESSPTR( itDiscAntiga, discNova->discEquivalentes, Disciplina )
			{
				Disciplina *discAntiga = *itDiscAntiga;
					
				// Se a disciplina antiga não possuir uma substituição no map (não for uma chave do map)
				if ( problemData->mapDiscSubstituidaPor.find( discAntiga ) ==
					 problemData->mapDiscSubstituidaPor.end() )
				{
					problemData->mapDiscSubstituidaPor[ discAntiga ] = discNova;
				}
				else
				{
					std::cout << "\nAtencao em ProblemDataLoader::referenciaDisciplinasEquivalentes():";
					std::cout << "\nJa existe uma substituicao da disciplina " << discAntiga->getId()
							  << " pela disciplina " <<  problemData->mapDiscSubstituidaPor[ discAntiga ]->getId()
							  << ". Nao eh possivel substitui-la pela disciplina " << discNova->getId();
				}
			}
		}
	}

	// Considera transitividade entre equivalências
	else
	{
		// Preenche mapDiscSubstituidaPor
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
		{
			Disciplina *discNova = *itDisc;

			ITERA_GGROUP_LESSPTR( itDiscAntiga, discNova->discEquivalentes, Disciplina )
			{
				Disciplina *discAntiga = *itDiscAntiga;
					
				// Se a disciplina antiga já possuir uma substituição no map (for uma chave do map)
				if ( problemData->mapDiscSubstituidaPor.find( discAntiga ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					// Não faz nada. A disciplina que foi inserida primeiro no map prevalece.
				}
				else // discAntiga não é chave
				{
					bool jaSubstituiu = false;

					std::map< Disciplina*, Disciplina* >::iterator itMap;
					for ( itMap = problemData->mapDiscSubstituidaPor.begin();
						  itMap != problemData->mapDiscSubstituidaPor.end();
						  itMap++ )
					{
						if ( itMap->second == discAntiga )
						{
							jaSubstituiu = true; // Se a disciplina antiga já tiver substituído alguma disciplina no map (for substituta)
						}
					}

					if ( jaSubstituiu ) // discAntiga já é substituta
					{
						if ( problemData->mapDiscSubstituidaPor.find( discNova ) != 
							 problemData->mapDiscSubstituidaPor.end() ) // discNova já é chave do map
						{
							Disciplina* equiv = ( problemData->mapDiscSubstituidaPor.find( discNova ) )->second;

							for ( itMap = problemData->mapDiscSubstituidaPor.begin();
								  itMap != problemData->mapDiscSubstituidaPor.end();
								  itMap++ )
							{
								if ( itMap->second == discAntiga )
								{
									problemData->mapDiscSubstituidaPor[ itMap->first ] = equiv;
								}
							}

							problemData->mapDiscSubstituidaPor[discAntiga] = equiv;
						}
						else  // discNova NÃO é chave do map
						{
							for ( itMap = problemData->mapDiscSubstituidaPor.begin();
								  itMap != problemData->mapDiscSubstituidaPor.end();
								  itMap++ )
							{
								if ( itMap->second == discAntiga )
								{
									problemData->mapDiscSubstituidaPor[ itMap->first ] = discNova;
								}
							}

							problemData->mapDiscSubstituidaPor[discAntiga] = discNova;
						}

					}
					else // Se a disciplina antiga não estiver no map
					{
						itMap = problemData->mapDiscSubstituidaPor.find( discNova );
			
						// Se a disciplina nova já possuir uma substituição no map (for uma chave do map)
						if ( itMap != problemData->mapDiscSubstituidaPor.end() )
						{
							problemData->mapDiscSubstituidaPor[discAntiga] = itMap->second;
						
							if ( problemData->mapDiscSubstituidaPor.find( itMap->second ) != 
								 problemData->mapDiscSubstituidaPor.end() )
							{
								std::cout<<"\nNão deveria entrar aqui\n";
							}

						}
						else
						{
							// Se a disciplina nova já tiver substituído alguma disciplina no map
							// ou se também não estiver no map
							problemData->mapDiscSubstituidaPor[discAntiga] = discNova;

							if ( problemData->mapDiscSubstituidaPor.find( discNova ) != 
								 problemData->mapDiscSubstituidaPor.end() )
							{
								std::cout<<"\nNão deveria entrar aqui\n";
							}

						}
					}			
				}
			}
		}
	}

	substituiDisciplinasEquivalentes();

}

/*
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
         map < Disciplina*, int, LessPtr< Disciplina > >::iterator
            it_periodo_disciplina = curriculo->disciplinas_periodo.begin();

         for (; it_periodo_disciplina != curriculo->disciplinas_periodo.end();
            it_periodo_disciplina++ )
         {
			 disciplina = it_periodo_disciplina->first;

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

                  // Informa que a 'disciplina_equivalente' será substituída pela 'disciplina_substituta'
                  problemData->map_CursoCurriculo_DiscSubst[ curso_curriculo ][ disciplina_equivalente ] = disciplina_substituta;

                  // Adiciona a 'disciplina_equivalente' no conjunto de disciplinas que foram substituidas por 'disciplina_substituta'
                  problemData->mapGroupDisciplinasSubstituidas[ curso_curriculo ][ disciplina_substituta ].add( disciplina_equivalente );
               }
               //------------------------------------------------------------------------------------------------
            }
         }
      }
   }

   substituiDisciplinasEquivalentes();
}
*/

void ProblemDataLoader::divideDisciplinas()
{
   int idNovaMag = -1;

   ITERA_GGROUP_LESSPTR( it_disc,
      problemData->disciplinas, Disciplina )
   {
      if ( it_disc->getCredTeoricos() > 0
         && it_disc->getCredPraticos() > 0
         && it_disc->eLab() == true )
      {
         Disciplina * nova_disc = new Disciplina();

         nova_disc->setId( -it_disc->getId() );
         nova_disc->setCodigo( it_disc->getCodigo() + "-P" );
         nova_disc->setNome( it_disc->getNome() + "PRATICA" );
         nova_disc->setCredTeoricos( 0 );
         nova_disc->setCredPraticos( it_disc->getCredPraticos() );
         it_disc->setCredPraticos( 0 );

         nova_disc->setMaxCreds( nova_disc->getCredPraticos() );
         it_disc->setMaxCreds( it_disc->getCredTeoricos() );

         nova_disc->setELab( it_disc->eLab() );
         it_disc->setELab( false );

         nova_disc->setMaxAlunosT( -1 );
         nova_disc->setMaxAlunosP( it_disc->getMaxAlunosP() );
         it_disc->setMaxAlunosP( -1 );

         nova_disc->setTipoDisciplinaId( it_disc->getTipoDisciplinaId() );
         nova_disc->setNivelDificuldadeId( it_disc->getNivelDificuldadeId() );
		 
		 nova_disc->setCalendario( it_disc->getCalendario() );

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

                  int continuar = ( rand() % 2 );

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
               nova_disc->getCredPraticos() );

            // Checando se existe alguma regra de crédito
            // cadastrada para o total de créditos da nova disciplina.
            if ( it_Creds_Regras != problemData->creds_Regras.end() )
            {
               if ( it_Creds_Regras->second.size() == 1 )
               {
                  nova_disc->divisao_creditos = new DivisaoCreditos(
                     ( **it_Creds_Regras->second.begin() ) );
               }
               else // Greather
               {
                  GGroup< DivisaoCreditos * >::iterator 
                     it_Regra = it_Creds_Regras->second.begin();

                  int continuar = ( rand() % 2 );

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
            Horario * h =  new Horario();
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

               // >>> >>> >>> Copiando HorariosAula
               HorarioAula * hr_aula;

               if ( it_hr->turno->horarios_aula.size() > 0 )
               {
                  ITERA_GGROUP_LESSPTR( it_hr_aula,
                    turno->horarios_aula, HorarioAula )
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

               GGroup< int >::iterator it_dia_sem
                  = it_hr->horario_aula->dias_semana.begin();

               for ( unsigned dia = 0;
                  dia < it_hr->horario_aula->dias_semana.size(); dia++ )
               {
                  hr_aula->dias_semana.add( *it_dia_sem );
                  it_dia_sem++;
               }
            }

            nova_disc->horarios.add( h );
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
            // em < Campi->Professor->disciplinas >:
            Magisterio * novo_mag;

            ITERA_GGROUP_LESSPTR( it_prof, it_cp->professores, Professor )
            {
               ITERA_GGROUP_LESSPTR( it_mag, it_prof->magisterio, Magisterio )
               {
                  if ( it_mag->getDisciplinaId() == it_disc->getId() )
                  {
                     novo_mag = new Magisterio();

                     idNovaMag--;
                     novo_mag->setId( idNovaMag );
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
				if ( it_curriculo->disciplinas_periodo.find( *it_disc  )
                     == it_curriculo->disciplinas_periodo.end())
					 continue;

               // FIXME, isto está errado, deveria-se, de algum jeito,
               // saber o periodo da disciplina ou, iterar sobre todos os periodos 
               // validos de um curso e nao sobre uma estimativa.
               for ( int num_periodos = 0; num_periodos < 20 /* FIX-ME */; num_periodos++ )
               {
 
                  if (it_curriculo->disciplinas_periodo[*it_disc] == num_periodos )
                  {
                     it_curriculo->disciplinas_periodo[nova_disc] = num_periodos;

                     // Garantido que uma disciplina aparece
                     // apenas uma vez em um curriculo de um curso.
                     break;
                  }
               }
            }
         }


		 // -----------------------------------------------------------------
		 //							DEMANDAS

		 // Só cria nova demanda para disciplinas que não foram substituidas.
		 // Disciplina substituidas tiveram suas demandas tambem substituidas,
		 // e não criadas.
		 if ( problemData->mapDiscSubstituidaPor.find( *it_disc ) ==
			  problemData->mapDiscSubstituidaPor.end() )
		 {
			 // Procura pelo maior id de demanda já cadastrado
			 int id = retornaMaiorIdDemandas();

			 // Procura pelo maior id de alunoDemanda já cadastrado
			 int idAlDemanda = retornaMaiorIdAlunoDemandas();

			 // Adicionando os dados da nova disciplina em <Demanda>			  
			 ITERA_GGROUP_LESSPTR( it_dem, problemData->demandasTotal, Demanda )
			 {
				if( it_dem->getDisciplinaId() == it_disc->getId())
				{
					// Copia demanda da disciplina teorica correspondente
					Demanda *nova_demanda = new Demanda();
					id++;
					nova_demanda->setId( id );
					nova_demanda->setOfertaId( it_dem->getOfertaId() );
					nova_demanda->setDisciplinaId( nova_disc->getId() );
					nova_demanda->setQuantidade( it_dem->getQuantidade() );
					nova_demanda->oferta = it_dem->oferta;
					nova_demanda->disciplina = nova_disc;

					problemData->demandasTotal.add( nova_demanda );

					if ( problemData->demandas.find( *it_dem ) !=
						 problemData->demandas.end() )
					{
						problemData->demandas.add( nova_demanda );
					}

					// Adicionando novo AlunoDemanda com a nova_demanda (disciplina pratica)
					// para cada aluno que já possuia demanda para a disciplina teorica correspondente
					ITERA_GGROUP_LESSPTR( it_aluno_dem, problemData->alunosDemandaTotal, AlunoDemanda )
					{
						if ( it_aluno_dem->getDemandaId() == it_dem->getId() )
						{
							int alunoId = it_aluno_dem->getAlunoId();
							std::string alunoNome = it_aluno_dem->getNomeAluno();
							int prior = it_aluno_dem->getPrioridade();
							idAlDemanda++;

							AlunoDemanda *aluno_demanda = new AlunoDemanda( idAlDemanda, alunoId, alunoNome, prior, nova_demanda );
							
							problemData->alunosDemandaTotal.add( aluno_demanda );

							if ( problemData->alunosDemanda.find( *it_aluno_dem ) !=
								 problemData->alunosDemanda.end() )
							{
								problemData->alunosDemanda.add( aluno_demanda );
								this->problemData->mapDemandaAlunos[ nova_demanda ].add( aluno_demanda );
							}
						}
					}
				}
			 }
		 }
		 // -----------------------------------------------------------------

         GGroup< int >::iterator itDiasLetivosDiscs = it_disc->diasLetivos.begin();

         for (; itDiasLetivosDiscs != it_disc->diasLetivos.end();
                itDiasLetivosDiscs++ )
         {
            nova_disc->diasLetivos.add( *itDiasLetivosDiscs );
         }

         problemData->novasDisciplinas.add( nova_disc );

      }
   }

   ITERA_GGROUP_LESSPTR( itDisciplina, problemData->novasDisciplinas, Disciplina )
   {
      problemData->disciplinas.add( *itDisciplina );
   }

   // Equivalências para disciplinas praticas
   relacionaEquivalenciasDisciplinasPraticas();


   // ---------
   criaFixacoesDisciplinasDivididas();
}

void ProblemDataLoader::relacionaEquivalenciasDisciplinasPraticas()
{
	// Para cada nova disciplina Pratica criada, verifica se a sua Teorica
	// correspondente substituiu alguma disciplina. Se sim, acrescenta essa
	// Pratica nos maps necessários, substituindo a original prática
	// correspondente ou, se essa não existir, a teórica corresponte.
   ITERA_GGROUP_LESSPTR( itDisciplina, problemData->novasDisciplinas, Disciplina )
   {
	    Disciplina *dp = *itDisciplina;
		Disciplina *dt = NULL;

		int idT = - dp->getId(); // Id da disciplina teorica correspondente
   
		ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
		{
			if ( it_disc->getId() == idT )
				dt = *it_disc;
		}

		if ( dt != NULL )
		{
			GGroup< std::pair< Curso *, Curriculo * > > cursoCurriculos =
				problemData->retornaCursosCurriculosDisciplina( dt );

			GGroup< std::pair< Curso *, Curriculo * > >::iterator it_curso_curr = 
				cursoCurriculos.begin();

			for ( ; it_curso_curr != cursoCurriculos.end(); it_curso_curr++ )
			{
				Disciplina *discOriginalT = problemData->ehSubstitutaDe( dt, *it_curso_curr );

				if ( discOriginalT != NULL )
				{
					int idOrigP = - discOriginalT->getId();

					Disciplina * discOriginalP = NULL;
					ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
					{
						if ( it_disc->getId() == idOrigP )
							discOriginalP = *it_disc;
					}

					if ( discOriginalP != NULL )
					{
						problemData->mapDiscSubstituidaPor[ discOriginalP ] = dp;
						problemData->mapGroupDisciplinasSubstituidas[*it_curso_curr][dp].add( discOriginalP );

						if ( problemData->mapGroupDisciplinasSubstituidas[ *it_curso_curr ][ dp ].size() > 1 )
						{
							std::cout<<"ATENCAO em void ProblemDataLoader::():relacionaEquivalenciasDisciplinasPraticas"
								<<" a disciplina "<<dp->getId()<<" esta substituindo "
								<<"mais de uma disciplina no curriculo "<< (*it_curso_curr).second->getId();
						}
					}
					else
					{
						problemData->mapDiscSubstituidaPor[ discOriginalT ] = dp;
						problemData->mapGroupDisciplinasSubstituidas[*it_curso_curr][dp].add( discOriginalT );

						if ( problemData->mapGroupDisciplinasSubstituidas[ *it_curso_curr ][ dp ].size() > 1 )
						{
							std::cout<<"ATENCAO em void ProblemDataLoader::():relacionaEquivalenciasDisciplinasPraticas"
								<<" a disciplina "<<dp->getId()<<" esta substituindo "
								<<"mais de uma disciplina no curriculo "<< (*it_curso_curr).second->getId();
						}
					}
				}
			}
		}
    }

   // Imprime o mapDiscSubstituidaPor

	ofstream outTestFile;
	char equivFilename[] = "mapDiscSubstituidaPor.txt";
	outTestFile.open(equivFilename, ios::out);
	if (!outTestFile) {
		cerr << "Can't open output file " << equivFilename << endl;
		exit(1);
	}

   for ( std::map< Disciplina*, Disciplina* >::iterator it = problemData->mapDiscSubstituidaPor.begin();
	     it != problemData->mapDiscSubstituidaPor.end(); it++ )
   {
	   outTestFile <<"\nAntiga "<< it->first->getId() <<" Substituta "<< it->second->getId();
   }
   outTestFile.close();
   
   // ------------------------------------
   // Imprime o mapGroupDisciplinasSubstituidas

	ofstream outTestFile2;
	char equivFilename2[] = "mapGroupDisciplinasSubstituidasProblematicas.txt";
	outTestFile2.open(equivFilename2, ios::out);
	if (!outTestFile) {
		cerr << "Can't open output file " << equivFilename2 << endl;
		exit(1);
	}

   for ( std::map< std::pair< Curso *, Curriculo * >, 
				   std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > > >::iterator it
				   = problemData->mapGroupDisciplinasSubstituidas.begin();
	     it != problemData->mapGroupDisciplinasSubstituidas.end(); it++ )
   {	   
	   for ( std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > >::iterator it2 = it->second.begin();
			 it2 != it->second.end(); it2++ )
	   {
		   if ( it2->second.size() > 1 )
		   {
				outTestFile2 <<"\nCurso "<< it->first.first->getId() <<" Curric "<< it->first.second->getId();
				outTestFile2 <<"  Substituta "<< it2->first->getId();
			    ITERA_GGROUP_LESSPTR( itDisciplina, it2->second, Disciplina )
				{
					outTestFile2 <<" Antiga "<< itDisciplina->getId();
				}
		   }
	   }
   }
   outTestFile2.close();

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

void ProblemDataLoader::referenciaHorariosAula()
{
   ITERA_GGROUP_LESSPTR( it_horario_dia, problemData->horariosDia, HorarioDia )
   {
      problemData->refHorarioAula[ it_horario_dia->getHorarioAula()->getId() ]
         = ( it_horario_dia->getHorarioAula() );
   }
}

void ProblemDataLoader::referenciaTurnos()
{
   ITERA_GGROUP_LESSPTR( it_turno, problemData->todos_turnos, Turno )
   {
      problemData->refTurnos[ it_turno->getId() ] = ( *it_turno );
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
   GGroup< HorarioAula *, LessPtr< HorarioAula > > todos_horarios_aula;

   ITERA_GGROUP_LESSPTR( it_calendario,
      this->problemData->calendarios, Calendario )
   {
      ITERA_GGROUP_LESSPTR( it_turno,
         it_calendario->turnos, Turno )
      {
         ITERA_GGROUP_LESSPTR( it_horario_aula,
            it_turno->horarios_aula, HorarioAula )
         {
            todos_horarios_aula.add( *it_horario_aula );
         }
      }
   }

   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP( it_horario, it_unidades->horarios, Horario )
         {
            find_and_set_lessptr( it_horario->getTurnoId(),
               problemData->todos_turnos,
               it_horario->turno, false );

            find_and_set_lessptr( it_horario->getHorarioAulaId(),
               // it_horario->turno->horarios_aula,
               todos_horarios_aula,
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

               find_and_set_lessptr( it_horario->getHorarioAulaId(),
                  // it_horario->turno->horarios_aula,
                  todos_horarios_aula,
                  it_horario->horario_aula, false );
            }

            // Disciplinas associadas
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

            find_and_set_lessptr( it_horario->getHorarioAulaId(),
               // it_horario->turno->horarios_aula,
               todos_horarios_aula,
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

         find_and_set_lessptr( it_horario->getHorarioAulaId(),
            // it_horario->turno->horarios_aula,
            todos_horarios_aula,
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

         find_and_set_lessptr( it_horario->getHorarioAulaId(),
            // it_horario->turno->horarios_aula,
            todos_horarios_aula,
            it_horario->horario_aula, false );
      }

   } // disciplinas

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
            find_and_set_lessptr( it_fix->getHorarioAulaId(),
               // it_fix->turno->horarios_aula,
               todos_horarios_aula,
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
               ITERA_GGROUP_LESSPTR( it_horario_aula, it_turno->horarios_aula, HorarioAula )
               {
                  todos_horarios_aula.add( ( *it_horario_aula ) );
               }
            }

            find_and_set( it_fix->getHorarioAulaId(),
               todos_horarios_aula, it_fix->horario_aula, false );
         }
      }

      ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
      {
         // Seta a referência ao professor da fixação
         if ( it_fix->getProfessorId() >= 0 )
         {
            find_and_set_lessptr( it_fix->getProfessorId(),
               it_campi->professores, it_fix->professor, false );
         }

         // Seta a referência à sala da fixação
         if ( it_fix->getSalaId() >= 0 )
         {
            ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade )
            {
				if ( it_unidades->possuiSala( it_fix->getSalaId() ) )
				{
					find_and_set_lessptr( it_fix->getSalaId(),
					it_unidades->salas, it_fix->sala, false );

					break;
				}
            }
         }
      }
   } // fixações

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

   ITERA_GGROUP_LESSPTR( it_aluno_demanda,
      problemData->alunosDemanda, AlunoDemanda )
   {
      bool encontrouDemanda = false;

      ITERA_GGROUP_LESSPTR( it_dem, problemData->demandas, Demanda )
      {
         if ( it_dem->getId() == it_aluno_demanda->getDemandaId() )
         {
            encontrouDemanda = true;
            break;
         }
      }

      if ( !encontrouDemanda )
      {
         std::cout << "Demanda nao encontrada em gera_refs(): "
                   << it_aluno_demanda->getDemandaId() << std::endl;       
         continue;
      }

      find_and_set_lessptr( it_aluno_demanda->getDemandaId(),
         problemData->demandas, it_aluno_demanda->demanda, false );
   }
  
   ITERA_GGROUP_LESSPTR( it_aluno_demanda,
      problemData->alunosDemandaTotal, AlunoDemanda )
   {
      bool encontrouDemanda = false;

      ITERA_GGROUP_LESSPTR( it_dem, problemData->demandasTotal, Demanda )
      {
         if ( it_dem->getId() == it_aluno_demanda->getDemandaId() )
         {
            encontrouDemanda = true;
            break;
         }
      }

      if ( !encontrouDemanda )
      {
         std::cout << "Demanda nao encontrada em gera_refs(): "
                   << it_aluno_demanda->getDemandaId() << std::endl;       
         continue;
      }

      find_and_set_lessptr( it_aluno_demanda->getDemandaId(),
         problemData->demandasTotal, it_aluno_demanda->demanda, false );
   }
}

void ProblemDataLoader::cria_blocos_curriculares()
{
	if ( problemData->parametros->otimizarPor == "ALUNO" )
	{
		return;
	}

   // Contador de blocos
   int id_Bloco = 1;

   Campus * campus = NULL;
   Curso * curso = NULL;
   Oferta * oferta = NULL;
   Demanda * demanda = NULL;
   Curriculo * curriculo = NULL;
   Disciplina * disciplina = NULL;

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
               if ( it_oferta->campus->getId() == campus->getId()
                  && it_oferta->curriculo->getId() == curriculo->getId()
                  && it_oferta->curso->getId() == curso->getId() )
               {
                  oferta = ( *it_oferta );
                  break;
               }
            }

            // Checando se foi encontrada alguma oferta válida.
            if ( oferta == NULL )
            {
               continue;
            }

            map < Disciplina*, int, LessPtr< Disciplina > >::iterator
               it_disc_periodo = curriculo->disciplinas_periodo.begin();

            // Percorrendo todas as disciplinas de
            // um curso cadastradas para um currículo

            for (; it_disc_periodo != curriculo->disciplinas_periodo.end();
                   it_disc_periodo++ )
            {
               int periodo = it_disc_periodo->second;
               disciplina = it_disc_periodo->first;
               
			   int id_disciplina = abs( disciplina->getId() );
               int id_oferta = oferta->getId();
			   
			   Disciplina *d = problemData->ehSubstitutaDe( disciplina, std::make_pair(curso, curriculo) );

               std::pair< Campus *, Curso * > campus_curso
                  = std::make_pair( campus, curso );

               // Encontrando e armazenando a demanda específica da disciplina em questão
               demanda = NULL;

               ITERA_GGROUP_LESSPTR( it_demanda, problemData->demandas, Demanda )
               {
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

                  exit( 0 );

                  demanda = new Demanda();
                  demanda->setDisciplinaId( id_disciplina );
                  demanda->disciplina = disciplina;
                  demanda->setOfertaId( id_oferta );
                  demanda->oferta = oferta;
                  demanda->setQuantidade( 0 );

                  problemData->map_campus_curso_demanda[ campus_curso ].add( demanda );
                  problemData->demandas.add( demanda );
               }

               bool found = false;

               // Verificando a existência do bloco
               // curricular para a disciplina em questão.

               ITERA_GGROUP_LESSPTR( it_bloco_curricular,
                  problemData->blocos, BlocoCurricular )
               {
                  if ( it_bloco_curricular->campus->getId() == campus->getId()
                     && it_bloco_curricular->curso->getId() == curso->getId()
                     && it_bloco_curricular->curriculo->getId() == curriculo->getId()
                     && it_bloco_curricular->getPeriodo() == periodo
					 && it_bloco_curricular->oferta->getId() == oferta->getId() )
                  {
                     (*it_bloco_curricular)->disciplinas.add( disciplina );
                     (*it_bloco_curricular)->disciplina_Demanda[ disciplina ] = demanda;

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
				  bloco_curricular->oferta = oferta;
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

   ITERA_GGROUP_LESSPTR( it_bc,
      problemData->blocos, BlocoCurricular )
   {
      bloco = ( *it_bc );
      curso = it_bc->curso;
      Curriculo* curriculo = it_bc->curriculo;

      int totalTurmas = 0;

      ITERA_GGROUP_LESSPTR( it_Disc,
         it_bc->disciplinas, Disciplina )
      {
         disciplina = ( *it_Disc );

         // Associa o curso correspondente ao bloco atual
         // e a disciplina 'it_disc' ao bloco curricular corrente
         Trio<Curso*,Curriculo*,Disciplina*> auxTrio;
         auxTrio.first = curso;
         auxTrio.second = curriculo;
         auxTrio.third = disciplina;

         problemData->mapCursoDisciplina_BlocoCurricular
            [ auxTrio ] = bloco;

         ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Disc->diasLetivos, int )
         { 
            (*it_bc)->diasLetivos.add( *it_Dias_Letivos );
         }

         totalTurmas += it_Disc->getNumTurmas();
      }

      (*it_bc)->setTotalTurmas( totalTurmas );
   }
   
   /* Preenche, para cada bloco curricular, o numero maximo de creditos possiveis
	  de serem alocados para cada dia letivo */
   ITERA_GGROUP_LESSPTR( it_bc, problemData->blocos, BlocoCurricular )
   {
		(*it_bc)->preencheMaxCredsPorDia();
   }

}

void ProblemDataLoader::relacionaCampusDiscs()
{
   ITERA_GGROUP_LESSPTR( it_oferta,
      problemData->ofertas, Oferta )
   {
      Curso * curso = it_oferta->curso;

      ITERA_GGROUP_LESSPTR( it_curric,
         curso->curriculos, Curriculo )
      {
         map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc_prd =
            it_curric->disciplinas_periodo.begin();

         for (; it_disc_prd != it_curric->disciplinas_periodo.end(); it_disc_prd++ )
         {
			 problemData->cp_discs[ it_oferta->getCampusId() ].add( it_disc_prd->first->getId() );
         }
      }
   }
}

void ProblemDataLoader::calculaDemandas()
{
	problemData->calculaDemandas();
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

            (*it_und)->setMaiorSala( std::max( ( (int)it_und->getMaiorSala() ),
											( (int)it_sala->getCapacidade() ) ) );
		 }

         total_Salas += (*it_und)->getNumSalas();
         (*it_cp)->setMaiorSala( std::max( (int)it_cp->getMaiorSala(), (int)it_und->getMaiorSala() ) );
      }

      problemData->cp_medSalas[ it_cp->getId() ] =
         ( ( total_Salas > 0 ) ? ( somaCapSalas / total_Salas ) : 0 );
   }
}

void ProblemDataLoader::calculaMenorCapacSalaPorDisc()
{
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   int campusId = itCampus->getId();
	   GGroup< int > disciplinas = problemData->cp_discs[campusId];

	   ITERA_GGROUP_N_PT( it_disc, disciplinas, int )
	   {	   
		    Disciplina* disciplina = problemData->refDisciplinas[ *it_disc ];

			#pragma region Equivalência de disciplinas
			if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

			int menorCapacSala = 10000;

			std::vector< Sala * >::iterator it_sala = problemData->discSalas[ disciplina ].begin();
			for ( ; it_sala != problemData->discSalas[disciplina].end(); it_sala++ )
			{
				if ( problemData->retornaCampus( ( *it_sala )->getIdUnidade() )->getId() != campusId )
			   {
				   continue;
			   }

		 	   menorCapacSala = std::min( menorCapacSala, ( *it_sala )->getCapacidade() );
			}

			disciplina->setMenorCapacSala(menorCapacSala, campusId);
	   }
   }

}

void ProblemDataLoader::calculaCapacMediaSalaPorDisc()
{
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   int campusId = itCampus->getId();
	   GGroup< int > disciplinas = problemData->cp_discs[campusId];

	   ITERA_GGROUP_N_PT( it_disc, disciplinas, int )
	   {	   
		   Disciplina* disciplina = problemData->refDisciplinas[ *it_disc ];

			#pragma region Equivalência de disciplinas
			if ( problemData->mapDiscSubstituidaPor.find(disciplina) !=
				 problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

		   int soma = 0;
		   int size = 0;

		   std::vector< Sala * >::iterator it_sala = problemData->discSalas[ disciplina ].begin();
		   for ( ; it_sala != problemData->discSalas[ disciplina ].end(); it_sala++ )
		   {
			   if ( problemData->retornaCampus( ( *it_sala )->getIdUnidade() )->getId() != campusId )
			   {
				   continue;
			   }

			   soma += ( *it_sala )->getCapacidade();
			   size++;
		   }

		   if ( soma == 0 )
				std::cout<<"ATENCAO: disciplina " << disciplina->getId() << ", lab " << disciplina->eLab() << " nao possui sala associada";
	   
		   int capacMediaSala = ( ( size > 0 ) ? ( soma / size ) : 0 );
	   
		   disciplina->setCapacMediaSala( capacMediaSala, campusId );
		   
	   }
   }

}

void ProblemDataLoader::estima_turmas()
{
	estima_turmas_sem_compart();
}

/*
 TRIEDA-416
    Estimando o número máximo de turmas de cada
    disciplina, quando não é permitido o compartilhamento
    de turmas de cursos diferentes, de acordo com o seguinte cálculo:
	
	numTurmasDisc = sum{ demDiscCurso_{i} / tamMedioSalasCP }

    Onde:
    demDiscOferta -> representa a demanda total do curso i de uma dada disciplina.
    tamMedioSalasCP -> representa o tamanho medio das salas de um campus que têm a disciplina associada.
*/
void ProblemDataLoader::estima_turmas_sem_compart()
{
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCpDisc = problemData->cp_discs.begin();
   for ( ; itCpDisc != problemData->cp_discs.end(); itCpDisc++ )
   {
	   int campusId = itCpDisc->first;
	   Campus * cp = problemData->refCampus[campusId];

	   // Para cada disciplina do campus
	   GGroup< int >::iterator itDisc = itCpDisc->second.begin();
	   for ( ; itDisc != itCpDisc->second.end(); itDisc++ )
	   {
		   int discId = *itDisc;
		   Disciplina *d = problemData->refDisciplinas[discId];

		   int capacMediaSala = d->getCapacMediaSala( cp->getId() );
		   
		   if ( d->getNumTurmas() < 0 ) d->setNumTurmas( 0 );
		   
		   int numTurmas = d->getNumTurmas();
		   
		   // Para cada curso que contem a disciplina
		   ITERA_GGROUP_LESSPTR( itCurso, cp->cursos, Curso )
		   {
			   Curso *c = *itCurso;

			   if ( !c->possuiDisciplina( d ) )
			   {
					continue;
			   }
			   
			   int quantidade = 0;

			   GGroup< Demanda*, LessPtr< Demanda > > demandas = problemData->buscaTodasDemandas( c , d, cp );

			   ITERA_GGROUP_LESSPTR( itDemanda, demandas, Demanda )
			   {
				   quantidade += itDemanda->getQuantidade();  
			   }

			   // Numero de turmas necessarias para atender a demanda da disciplina d do curso c
			   if ( capacMediaSala > 0 )
					numTurmas += int( std::floor( double(quantidade+capacMediaSala-1)/capacMediaSala ) );
		   }

		   d->setNumTurmas( numTurmas + 1 );
	   }
   }

}


void ProblemDataLoader::print_stats()
{
   int ncampi( 0 ), nunidades( 0 ), nsalas( 0 ), nconjuntoSalas( 0 ),
       ndiscs( 0 ), ndiscsDiv( 0 ), nturmas( 0 ), nturmasDiscDiv( 0 ),
       nprofs( 0 ), ncursos( 0 ),   nofertas( 0 ), tdemanda( 0 ), tdemandaDiv( 0 );

   ncampi = problemData->campi.size();

   ITERA_GGROUP_LESSPTR( it_campi,
      problemData->campi, Campus )
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

   ITERA_GGROUP_LESSPTR( itOferta,
      problemData->ofertas, Oferta )
   {
      map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPrdDisc =
         itOferta->curriculo->disciplinas_periodo.begin();

      for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end();
            itPrdDisc++ )
      {
		  Disciplina * disc = itPrdDisc->first;

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

   ITERA_GGROUP_LESSPTR( it_disc,
      problemData->disciplinas, Disciplina )
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

   std::cout << "\nEstatisticas de dados de entrada:\n\n";

   printf( "<*> Campi:                 \t%4d\n", ncampi );
   printf( "<*> Unidades:              \t%4d\n", nunidades );
   printf( "<*> Salas:                 \t%4d\n", nsalas );

   printf( "<*> Conjuntos de Salas:    \t%4d\n", nconjuntoSalas );

   printf( "<*> Disciplinas:\n");
   printf( "\t - Entrada:    \t%4d\n", ndiscs );
   printf( "\t - Divididas:  \t%4d\n", ndiscsDiv );   
   printf( "\t - Total:  \t%4d\n", ndiscs + ndiscsDiv );

   printf( "<*> Blocos Curriculares:  \t%4d\n", problemData->blocos.size() );

   printf( "<*> Turmas:\n");
   printf( "\t - Entrada:    \t%4d\n", nturmas );
   printf( "\t - Divididas:  \t%4d\n", nturmasDiscDiv );
   printf( "\t - Total:  \t%4d\n", nturmas + nturmasDiscDiv );

   printf( "<*> Professores:  \t%4d\n", nprofs );
   printf( "<*> Cursos:       \t%4d\n", ncursos );

   printf( "<*> Ofertas:      \t%4d\n", nofertas );

   printf( "<*> Demanda total\n");
   printf( "\t - Entrada:    \t%4d\n", tdemanda );
   printf( "\t - Divididas:  \t%4d\n", tdemandaDiv );   
   printf( "\t - Total:  \t%4d\n", tdemanda + tdemandaDiv );

   std::cout << "================================"
             << std::endl << std::endl;
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
	   int nCredsP = it_disc->getCredPraticos();
	   int nCredsT = it_disc->getCredTeoricos();

	   // Se o total de creditos é par
	   if ( (nCredsP + nCredsT)%2 == 0 )
		   it_disc->setMinCreds( 2 );

	   // Se o total de creditos é ímpar
	   else
			it_disc->setMinCreds( 1 );


       if ( it_disc->divisao_creditos != NULL )
       {
         it_disc->setMaxCreds(0);

         for ( int t = 0; t < 8; t++ )
         {
            if ( it_disc->divisao_creditos->dia[t] > 0 )
            {
               it_disc->setMaxCreds( 
                  std::max(it_disc->getMaxCreds(),
                  it_disc->divisao_creditos->dia[t]) );
            }
         }
       }
       else
       {
         it_disc->setMaxCreds( nCredsP + nCredsT );
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
			 if ( *it_disciplina == 5937 )
				 std::cout<<" AQUI ";

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
			   system("pause");
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
               break; // TODO: acho que tem coisa errada nessa parte de fixações!
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

   
   // --------------------------------------------------------------------------
   // Conta para cada disciplina a quantas salas ela está associada.

   ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
   {
		ITERA_GGROUP_LESSPTR( itUnidade, itCp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{							
				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					itDisc->setNSalasAptas( itDisc->getNSalasAptas() + 1 );
				}
			}
		}

    }

}

bool ProblemDataLoader::existe_conjunto_sala__fixacao(
   Unidade * unidade, Disciplina * disciplina, Sala * sala )
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
      map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();

      for (; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end();
             it_Prd_Disc++ )
      { 
		  disciplina = it_Prd_Disc->first;
         int disc = disciplina->getId();

         problemData->ofertasDisc[ disc ].add( *it_Oferta );

         // Utilizado em equivalências de disciplinas
         std::pair< Curso *, Curriculo * > curso_curriculo
            = std::make_pair( it_Oferta->curso, it_Oferta->curriculo );

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

      ITERA_GGROUP( it_atend_campus,
         ( *problemData->atendimentosTatico ), AtendimentoCampusSolucao )
      {
		  int campusId = it_atend_campus->getCampusId();

         ITERA_GGROUP( it_atend_unidade,
            it_atend_campus->atendimentosUnidades, AtendimentoUnidadeSolucao )
         {
            ITERA_GGROUP( it_atend_sala,
               it_atend_unidade->atendimentosSalas, AtendimentoSalaSolucao )
            {
               ITERA_GGROUP( it_atend_dia_semana,
                  it_atend_sala->atendimentosDiasSemana, AtendimentoDiaSemanaSolucao )
               {
                  ITERA_GGROUP( it_atend_tatico,
                     it_atend_dia_semana->atendimentosTatico, AtendimentoTaticoSolucao )
                  {
                     atendOferta = ( it_atend_tatico->atendimento_oferta );

					 Oferta* oferta = problemData->refOfertas[ atendOferta->getOfertaCursoCampiId() ];

                     // Informa a 'turma' da aula
                     int turma = atoi( atendOferta->getTurma().c_str() );

					 // Informa a disciplina atendida (caso tenha havido substituição, é a substituta)
                     Disciplina * disciplina = problemData->refDisciplinas.find( atendOferta->getDisciplinaId() )->second;

                     int idDisc = disciplina->getId();

					 // Informa a disciplina substituida (caso não tenha havido substituição, é NULL)
					 Disciplina * disciplinaSubstituida = NULL;
					 if ( atendOferta->getDisciplinaSubstituidaId() != NULL )
					 {
						 disciplinaSubstituida = problemData->refDisciplinas.find( atendOferta->getDisciplinaSubstituidaId() )->second;
					 }

                     Sala * sala = problemData->refSala.find(
                        it_atend_sala->getSalaId() )->second;

                     // Informa o dia da semana da aula
                     int diaSemana = it_atend_dia_semana->getDiaSemana();

                     // Informa os créditos teóricos da aula
                     int creditos_teoricos = it_atend_tatico->getQtdeCreditosTeoricos();

                     // Informa os créditos práticos da aula
                     int creditos_praticos = it_atend_tatico->getQtdeCreditosPraticos();

                     // Informa a demanda atendida
                     int demandaAtendida = it_atend_tatico->atendimento_oferta->getQuantidade();

                     if ( ( creditos_praticos > 0 )
                        && ( creditos_teoricos > 0 ) )
                     {
                        std::cout << "Aula com creditos teoricos e praticos maiores que 0.\n";
                        exit( 1 );
                     }
					 
					 // Preenche os maps de atendimentos de alunos
					 Trio< int, int, Disciplina* > trio;
					 trio.set( campusId, turma, disciplina );

					 ITERA_GGROUP_N_PT( itAlunoDem, atendOferta->alunosDemandasAtendidas, int )
					 {						 
						 int alunoDemId = *itAlunoDem;

						 AlunoDemanda *alunoDemanda = problemData->retornaAlunoDemanda( alunoDemId );

						 if ( alunoDemanda == NULL )
						 {
							 std::cout<<"\nErro em criaAulas(). AlunoDemanda nao encontrado.\n";
							 continue;
						 }

						 Aluno *aluno = problemData->retornaAluno( alunoDemanda->getAlunoId() );

						 problemData->mapAluno_CampusTurmaDisc[aluno].add( trio );					 						
						 problemData->mapCampusTurmaDisc_AlunosDemanda[trio].add( alunoDemanda );
					 }

                     // Procurando nas aulas cadastradas, se existe
                     // alguma aula que possui os mesmos índices de 
                     // dia da semana, sala e turma. Caso encontre,
                     // devo apenas add a oferta à aula existente.
                     bool novaAula = true;
                     Aula * aulaAntiga = NULL;

                     ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
                     {
                        if( ( itAula->getTurma() == turma )
                           && ( *(itAula->getDisciplina()) == *disciplina )
                           && ( itAula->getDiaSemana() == diaSemana )
                           && ( *(itAula->getSala()) == *sala ) 
                           && ( itAula->getCreditosPraticos() == creditos_praticos )
                           && ( itAula->getCreditosTeoricos() == creditos_teoricos ) )
                        {
							if ( itAula->getDisciplinaSubstituida() != NULL && disciplinaSubstituida != NULL )
							{
								if ( *(itAula->getDisciplinaSubstituida()) == *disciplinaSubstituida )
								{
									aulaAntiga = *itAula;
									novaAula = false;
									problemData->aulas.remove(itAula);
									break;
								}
							}	 
							else if ( itAula->getDisciplinaSubstituida() == NULL && disciplinaSubstituida == NULL )
							{
							   aulaAntiga = *itAula;
							   novaAula = false;
							   problemData->aulas.remove(itAula);
							   break;
							}
                        }
                     }

                     if( novaAula )
                     {
                        // Monta o objeto 'aula'
                        Aula * aula = new Aula();
                        aula->ofertas.add( problemData->refOfertas[
                           atendOferta->getOfertaCursoCampiId() ] );

                        aula->setTurma( turma );
                        aula->setDisciplina( disciplina );
                        aula->setSala( sala );
                        aula->setDiaSemana( diaSemana );
                        aula->setCreditosTeoricos( creditos_teoricos );
                        aula->setCreditosPraticos( creditos_praticos );
						aula->setQuantidade( demandaAtendida, oferta );
                        aula->setDisciplinaSubstituida( disciplinaSubstituida );

                        problemData->aulas.add( aula );
                     }
                     else
                     {
                        aulaAntiga->ofertas.add( problemData->refOfertas[ atendOferta->getOfertaCursoCampiId() ] );
                        aulaAntiga->setQuantidade( demandaAtendida, oferta );
                        problemData->aulas.add( aulaAntiga );
                     }
                  }
               }
            }
         }
      }

	  if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
	  {
		  // O método abaixo só pode ser executado
		  // após a execução dos método de criação 
		  // de blocos curriculares e de criação das aulas.
		  relacionaBlocoCurricularAulas();
	  }
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
         Curriculo *curriculo = itOferta->curriculo;

         Trio<Curso*, Curriculo*, Disciplina*> auxTrio;
         auxTrio.first = curso;
         auxTrio.second = curriculo;
         auxTrio.third = disciplina;

         std::map< Trio<Curso*, Curriculo*, Disciplina*> , BlocoCurricular * >::iterator
            itMapCursoDisciplina_BlocoCurricular =
            problemData->mapCursoDisciplina_BlocoCurricular.find( auxTrio );

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
            std::cout << "\nNa funcao <ProblemDataLoader::relacionaBlocoCurricularAulas()> "
                      << "algum trio <curso,curriculo,disciplina> nao foi encontrado na estrutura "
                      << "<mapCursoDisciplina_BlocoCurricular>." << std::endl;

            exit( 1 );
         }
      }
   }
}

void ProblemDataLoader::validaInputSolucaoTatico()
{
   if ( ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
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

   ITERA_GGROUP_LESSPTR( it_calendario,
      problemData->calendarios, Calendario )
   {
      ITERA_GGROUP_LESSPTR( itTurno,
         it_calendario->turnos, Turno )
      {
         Turno * turno = ( *itTurno );

         ITERA_GGROUP_LESSPTR( itHA,
            turno->horarios_aula, HorarioAula )
         {
            HorarioAula * horarioAula = ( *itHA );

            if ( horarioAula->getId() > problemData->maxHorariosDif )
            {
               problemData->maxHorariosDif = horarioAula->getId();
            }

            ITERA_GGROUP_N_PT( itDia,
               horarioAula->dias_semana, int )
            {
               HorarioDia * horarioDia = new HorarioDia();

               horarioDia->setId( contador );
               horarioDia->setHorarioAulaId( horarioAula->getId() );
               horarioDia->setHorarioAula( horarioAula );
               horarioDia->setDia( *itDia );
               contador++;

               problemData->horariosDia.add( horarioDia );
            }
         }
      }
   }

   problemData->horariosDiaIdx.resize( ( problemData->maxHorariosDif + 1 ) * 8, NULL );

   ITERA_GGROUP_LESSPTR( itHorario, problemData->horariosDia, HorarioDia )
   {
      HorarioDia * horarioDia = ( *itHorario );

      problemData->horariosDiaIdx[ problemData->getHorarioDiaIdx( horarioDia ) ] = horarioDia;
   }

   // Professores
   ITERA_GGROUP_LESSPTR( itCamp, problemData->campi, Campus )
   {
      Campus * campus = ( *itCamp );

      ITERA_GGROUP_LESSPTR( itProf, campus->professores, Professor )
      {
         Professor * professor = ( *itProf );

         ITERA_GGROUP( itHor, professor->horarios, Horario )
         {
            Horario * horario = ( *itHor );

            ITERA_GGROUP_N_PT( itD, horario->dias_semana, int )
            {
               HorarioDia * auxHD = new HorarioDia();

               auxHD->setDia( *itD );
               auxHD->setHorarioAula( horario->horario_aula );
               auxHD->setHorarioAulaId( horario->getHorarioAulaId() );

               GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
                  itHorarioAula = problemData->horariosDia.find( auxHD );

               if ( itHorarioAula == problemData->horariosDia.end() )
               {
                  printf( "ERRO: HORARIODIA NAO ENCONTRADO\n" );
                  exit( 1 );
               }

               delete auxHD;

               professor->horariosDia.add( *itHorarioAula );
            }
         }
      }
   }

   // Salas
   ITERA_GGROUP_LESSPTR( itCamp, problemData->campi, Campus )
   {
      Campus * campus = ( *itCamp );

      ITERA_GGROUP_LESSPTR( itUnidade, campus->unidades, Unidade )
      {
         Unidade * unidade = ( *itUnidade );

         ITERA_GGROUP_LESSPTR( itSala, unidade->salas, Sala )
         {
            Sala * sala = ( *itSala );

            ITERA_GGROUP( itHor, sala->horarios_disponiveis, Horario )
            {
               Horario * horario = ( *itHor );

               ITERA_GGROUP_N_PT( itD, horario->dias_semana, int )
               {
                  HorarioDia * auxHD = new HorarioDia();

                  auxHD->setDia( *itD );
                  auxHD->setHorarioAula( horario->horario_aula );
                  auxHD->setHorarioAulaId( horario->getHorarioAulaId() );

                  GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
                     itHorarioAula = problemData->horariosDia.find( auxHD );

                  if ( itHorarioAula == problemData->horariosDia.end() )
                  {
                     printf( "ERRO: HORARIODIA NAO ENCONTRADO\n" );

                     exit( 1 );
                  }

                  delete auxHD;

                  sala->horariosDia.add( *itHorarioAula );
               }      
            }
         }
      }
   }

   // Disciplinas
   ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
   {
      Disciplina * disciplina = ( *itDisc );

      ITERA_GGROUP( itHor, disciplina->horarios, Horario )
      {
         Horario * horario = ( *itHor );

         ITERA_GGROUP_N_PT( itD, horario->dias_semana, int )
         {
            HorarioDia * auxHD = new HorarioDia();

            auxHD->setDia( *itD );
            auxHD->setHorarioAula( horario->horario_aula );
            auxHD->setHorarioAulaId( horario->getHorarioAulaId() );

            GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
               itHorarioAula = problemData->horariosDia.find( auxHD );

            if ( itHorarioAula == problemData->horariosDia.end() )
            {
               printf( "ERRO: HORARIODIA NAO ENCONTRADO\n" );

               exit( 1 );
            }

            delete auxHD;

            // Verificação de disponibilidade, relacionada com a issue TRIEDA-1054
            if ( this->problemData->verificaDisponibilidadeDisciplinaHorario(
                  disciplina, itHorarioAula->getHorarioAula() ) )
            {
               disciplina->horariosDia.add( *itHorarioAula );
            }
         }      
      }
   }
}

void ProblemDataLoader::relacionaProfessorDisciplinasAssociadas( void )
{
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      Campus * campus = ( *it_campus );

      ITERA_GGROUP_LESSPTR( it_professor, campus->professores, Professor )
      {
         Professor * professor = ( *it_professor );

         ITERA_GGROUP_LESSPTR( it_mag, professor->magisterio, Magisterio )
         {
            Magisterio * magisterio = ( *it_mag );
            problemData->mapProfessorDisciplinasAssociadas[ professor ].add( magisterio->disciplina );
         }
      }
   }
}


void ProblemDataLoader::calculaMaxTempoDisponivelPorSala()
{
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
         {
			 it_sala->calculaTempoDispPorDia();
         }
      }
   }
}


void ProblemDataLoader::calculaMaxTempoDisponivelPorSalaPorSL()
{
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
         {
			 it_sala->calculaTempoDispPorDiaSL();
         }
      }
   }
}

/*
	Preenche o map compatibilidadesDeHorarios, que relaciona para
	cada horarioAula H existente, os horarioAula h tal que a semana letiva
	de h é diferente da semana letiva de H, e h e H não possuem interseção
	(não se sobrepõem).
*/
void ProblemDataLoader::calculaCompatibilidadeDeHorarios()
{
	// calendario 1
	ITERA_GGROUP_LESSPTR( it_calendarios1, problemData->calendarios, Calendario )
    {
		ITERA_GGROUP_LESSPTR( it_turno1, it_calendarios1->turnos, Turno )
		{
			ITERA_GGROUP_LESSPTR( it_hor1, it_turno1->horarios_aula, HorarioAula )
			{
				HorarioAula *h1 = *it_hor1;

				// calendario 2
				ITERA_GGROUP_LESSPTR( it_calendarios2, problemData->calendarios, Calendario )
				{
					if ( *it_calendarios2 == *it_calendarios1)
						continue;

					ITERA_GGROUP_LESSPTR( it_turno2, it_calendarios2->turnos, Turno )
					{
						ITERA_GGROUP_LESSPTR( it_hor2, it_turno2->horarios_aula, HorarioAula )
						{
							HorarioAula *h2 = *it_hor2;
							
							if ( h1->getCalendario() == h2->getCalendario() )
								continue;
					
							if ( !h1->sobrepoe( *h2 ) )
								problemData->compatibilidadesDeHorarios[ h1 ].insert( h2 );
						}
					}
				}
			}
		}
	}
}

void ProblemDataLoader::calculaCombinaCredSLPorSala()
{
	// O caso de 1 unica semana letiva é simples, não requer essa função
	if ( problemData->calendarios.size() == 1 )
	{
		return;
	}

	if ( problemData->calendarios.size() != 2 )
	{
		std::cerr << "Atencao em ProblemDataLoader::calculaCombinaCredSLPorSala: esta funcao esta preparada "
				  << "para trabalhar somente o caso de 2 semanas letivas.";
	}

	GGroup< Calendario *, LessPtr< Calendario > >::iterator it_sl = problemData->calendarios.begin();

	Calendario *sl1 = *it_sl;
	Calendario *sl2 = *(++it_sl);

   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
         {
            Sala * sala = ( *it_sala );
            
			ITERA_GGROUP_N_PT( itDia, sala->diasLetivos, int )
            {
				int dia = *itDia;

				GGroup<HorarioAula*> horDispDiaSL1 = sala->retornaHorariosDisponiveisNoDiaPorSL( dia, sl1 );
				GGroup<HorarioAula*> horDispDiaSL2 = sala->retornaHorariosDisponiveisNoDiaPorSL( dia, sl2 );

				#pragma region Cria o mapa de compatibilidade de horarios de sl1 e sl2

				// Map para armazenar as compatibilidades
				std::map< HorarioAula*, GGroup<HorarioAula*> > map_compatHorarioSL1PorSalaDia;

				ITERA_GGROUP( it_H, horDispDiaSL1, HorarioAula )
				{
					ITERA_GGROUP( it_h, horDispDiaSL2, HorarioAula )
					{
						// Se it_H não se sobrepor com it_h
						if ( problemData->compatibilidadesDeHorarios[ *it_H ].find( *it_h ) !=
							 problemData->compatibilidadesDeHorarios[ *it_H ].end() )
						{
							map_compatHorarioSL1PorSalaDia[*it_H].add( *it_h );
						}
					}
				}
				#pragma endregion

				#pragma region Calcula todas as combinações possíveis entre horarios compativeis de sl2

				// Map para armazenar as combinações (sub-conjuntos)
				std::map< std::set< HorarioAula* /*sl2*/ >, int /*nro de ocorrencias*/ > todosSubConjuntos;

				for ( std::map< HorarioAula*, GGroup<HorarioAula*> >::iterator it_H = map_compatHorarioSL1PorSalaDia.begin();
					  it_H != map_compatHorarioSL1PorSalaDia.end(); it_H++  )
				{
					// Conjunto com todos os subconjuntos com h's compativeis com H
					GGroup< std::set< HorarioAula* > > subCjt = calculaSubCjtHorarios( it_H->second ); // TODO: tentar reaproveitar os calculos anteriores
					
					for ( GGroup< std::set< HorarioAula* > >::iterator it_h = subCjt.begin();
						  it_h != subCjt.end(); it_h++ )
					{
						if ( todosSubConjuntos.find( *it_h ) == todosSubConjuntos.end() )
						{
							todosSubConjuntos[ *it_h ] = 1; // Insere um novo subCjt
						}
						else
						{
							// Incrementa o nro de ocorrencias do subCjt, que corresponde ao nro de horarios de sl1 de contêm o subconj it_h
							todosSubConjuntos[ *it_h ]++;
						}
					}
				}
				
				#pragma endregion

				#pragma region Cria as combinações de maximos de creditos

				bool REMOCAO = false;
				int k=0;
				sala->getCombinaCredSLSize()[dia] = k;

				for ( int i = 0; i <= (int) horDispDiaSL1.size(); i++ )
				{
					int maxSL2 = 0;
					
					std::map< std::set< HorarioAula* /*sl2*/ >, int /*nro de ocorrencias*/ >::iterator it = todosSubConjuntos.begin();
					
					for (; it != todosSubConjuntos.end(); it++ )
					{
						int nHorariosSl1 = it->second;
						int nHorariosSl2 = it->first.size();

						if ( i == nHorariosSl1 && maxSL2 < nHorariosSl2 )
						{
							maxSL2 = it->first.size();
						}
					}
					if (i==0)
						maxSL2 = horDispDiaSL2.size();

					if ( !sala->combinaCredSL_eh_dominado( i, sl1, maxSL2, sl2, dia ) &&
						 !sala->combinaCredSL_eh_repetido( i, sl1, maxSL2, sl2, dia ) ) // atencao para a ordem: i refere-se a sl1 & maxSL2 refere-se a sl2
					{
						sala->setCombinaCredSL( dia, k, sl1, i );
						sala->setCombinaCredSL( dia, k, sl2, maxSL2 );
												
						k++;
						sala->setCombinaCredSLSize(dia, k);

						if ( sala->combinaCredSL_domina( i, sl1, maxSL2, sl2, dia ) != NULL )
						{
							// sera necessario remover os itens dominados
							REMOCAO = true;
						}
					}
				}
				
				#pragma region Remoção de itens dominados

				if (REMOCAO)
				{
					std::map< Trio<int, int, Calendario*>, int > dominados = sala->retornaCombinaCredSL_Dominados(dia);
					
					// Deleta itens dominados
					std::map< Trio<int, int, Calendario*>, int >::iterator it_mapDominados = dominados.begin();
					for ( ; it_mapDominados != dominados.end(); it_mapDominados++  )
					{
						sala->removeCombinaCredSL( it_mapDominados->first );						
					}
					int size = k - dominados.size()/2;
					sala->setCombinaCredSLSize(dia, size);
					
				}
				#pragma endregion
				
				#pragma endregion
			}

         }
      }
   }
}


void ProblemDataLoader::calculaCombinaCredSLPorBlocoCurric()
{
	if ( problemData->parametros->otimizarPor != "BLOCOCURRICULAR" )
	{
		return;
	}

	if ( problemData->calendarios.size() > 2 )
	{
		std::cerr << "Atencao em ProblemDataLoader::calculaCombinaCredSLPorBlocoCurric: esta funcao esta preparada "
				  << "para trabalhar somente o caso de 1 ou 2 semanas letivas.";
	}
		
	ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
    {
	    BlocoCurricular *bc = *itBloco;

	 	GGroup< Calendario*, LessPtr<Calendario> > calendarios = bc->curriculo->retornaSemanasLetivasNoPeriodo( bc->getPeriodo() );
       
		if ( calendarios.size() == 1 )
		{
			Calendario *sl1 = *calendarios.begin();
			
			ITERA_GGROUP_N_PT( itDia, bc->diasLetivos, int )
			{
				int dia = *itDia;

				bc->setCombinaCredSL( dia, 0, sl1, sl1->getNroDeHorariosAula(dia) );				
				
				bc->setCombinaCredSLSize(dia, 1);
			}
		}
		else if ( calendarios.size() >= 2 )
		{
			// só trata 2 semanas letivas por bloco
			Calendario *sl1 = *calendarios.begin();
			Calendario *sl2 = *(++calendarios.begin());

			ITERA_GGROUP_N_PT( itDia, bc->diasLetivos, int )
			{
				int dia = *itDia;

				GGroup<HorarioAula*> horDispDiaSL1 = bc->retornaHorariosDisponiveisNoDiaPorSL( dia, sl1 );
				GGroup<HorarioAula*> horDispDiaSL2 = bc->retornaHorariosDisponiveisNoDiaPorSL( dia, sl2 );

				#pragma region Cria o mapa de compatibilidade de horarios de sl1 e sl2

				// Map para armazenar as compatibilidades
				// Para cada HorarioAula H, lista todos os HorarioAula h que não sobrepõem H
				std::map< HorarioAula*, GGroup<HorarioAula*> > map_compatHorarioSL1PorSalaDia;

				ITERA_GGROUP( it_H, horDispDiaSL1, HorarioAula )
				{
					ITERA_GGROUP( it_h, horDispDiaSL2, HorarioAula )
					{
						// Se it_H não se sobrepor com it_h
						if ( problemData->compatibilidadesDeHorarios[ *it_H ].find( *it_h ) !=
								problemData->compatibilidadesDeHorarios[ *it_H ].end() )
						{
							map_compatHorarioSL1PorSalaDia[*it_H].add( *it_h );
						}
					}
				}

				#pragma endregion

				#pragma region Calcula todas as combinações possíveis entre horarios compativeis de sl2

				// Map para armazenar as combinações (sub-conjuntos)
				std::map< std::set< HorarioAula* >, int /*nro de ocorrencias*/ > todosSubConjuntos;

				for ( std::map< HorarioAula*, GGroup<HorarioAula*> >::iterator it_H = map_compatHorarioSL1PorSalaDia.begin();
						it_H != map_compatHorarioSL1PorSalaDia.end(); it_H++  )
				{
					// Conjunto com todos os subconjuntos com h's compativeis com H
					GGroup< std::set< HorarioAula* > > subCjt = calculaSubCjtHorarios( it_H->second ); // TODO: tentar reaproveitar os calculos anteriores
					
					for ( GGroup< std::set< HorarioAula* > >::iterator it_h = subCjt.begin();
							it_h != subCjt.end(); it_h++ )
					{
						if ( todosSubConjuntos.find( *it_h ) == todosSubConjuntos.end() )
						{
							todosSubConjuntos[ *it_h ] = 1; // Insere um novo subCjt
						}
						else
						{
							todosSubConjuntos[ *it_h ]++; // Incrementa o numero de ocorrencias do subCjt
						}
					}
				}
				
				#pragma endregion

				#pragma region Cria as combinações de maximos de creditos

				bool REMOCAO = false;
				int k=0;
				bc->getCombinaCredSLSize()[dia] = k;

				for ( int i = 0; i <= (int) horDispDiaSL1.size(); i++ )
				{
					int maxSL2 = 0;
					
					std::map< std::set< HorarioAula* >, int /*nro de ocorrencias*/ >::iterator it = todosSubConjuntos.begin();
					
					for (; it != todosSubConjuntos.end(); it++ )
					{
						int nHorariosSl1 = it->second;
						int nHorariosSl2 = it->first.size();

						if ( i == nHorariosSl1 && maxSL2 < nHorariosSl2 )
						{
							maxSL2 = it->first.size();
						}
					}
					if (i==0)
						maxSL2 = horDispDiaSL2.size();

					if ( !bc->combinaCredSL_eh_dominado( i, sl1, maxSL2, sl2, dia ) &&
						 !bc->combinaCredSL_eh_repetido( i, sl1, maxSL2, sl2, dia ) ) // atencao para a ordem: i refere-se a sl1 & maxSL2 refere-se a sl2
					{
						bc->setCombinaCredSL( dia, k, sl1, i );
						bc->setCombinaCredSL( dia, k, sl2, maxSL2 );
						
						k++;
						bc->setCombinaCredSLSize(dia, k);

						if ( bc->combinaCredSL_domina( i, sl1, maxSL2, sl2, dia ) != NULL )
						{
							// sera necessario remover itens dominados
							REMOCAO = true;
						}
					}
				}
				
				#pragma region Remoção de itens dominados

				if (REMOCAO)
				{
					std::map< Trio<int, int, Calendario*>, int > dominados = bc->retornaCombinaCredSL_Dominados(dia);
					
					// Deleta itens dominados
					std::map< Trio<int, int, Calendario*>, int >::iterator it_mapDominados = dominados.begin();
					for ( ; it_mapDominados != dominados.end(); it_mapDominados++  )
					{
						bc->removeCombinaCredSL( it_mapDominados->first );						
					}
					int size = k - dominados.size()/2;
					bc->setCombinaCredSLSize(dia, size);
					
				}
				#pragma endregion
				
				#pragma endregion
			}
		}
	}
}


void ProblemDataLoader::calculaCombinaCredSLPorAluno()
{
	if ( problemData->parametros->otimizarPor != "ALUNO" )
	{
		return;
	}

	if ( problemData->calendarios.size() > 2 )
	{
		std::cerr << "Atencao em ProblemDataLoader::calculaCombinaCredSLPorAluno: esta funcao esta preparada "
				  << "para trabalhar somente o caso de 1 ou 2 semanas letivas.";
	}
		
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
    {
	    Aluno *aluno = *itAluno;

		Campus * campus = aluno->getOferta()->campus;

	 	GGroup< Calendario*, LessPtr<Calendario> > calendarios = aluno->retornaSemanasLetivas();
       
		if ( calendarios.size() == 1 )
		{
			Calendario *sl1 = *calendarios.begin();
			
			ITERA_GGROUP_N_PT( itDia, campus->diasLetivos, int )
			{
				int dia = *itDia;

				aluno->setCombinaCredSL( dia, 0, sl1, sl1->getNroDeHorariosAula(dia) );				
				
				aluno->setCombinaCredSLSize(dia, 1);
			}
		}
		else if ( calendarios.size() == 2 )
		{
			// só trata 2 semanas letivas por bloco
			Calendario *sl1 = *calendarios.begin();
			Calendario *sl2 = *(++calendarios.begin());

			ITERA_GGROUP_N_PT( itDia, campus->diasLetivos, int )
			{
				int dia = *itDia;

				GGroup<HorarioAula*> horDispDiaSL1 = sl1->retornaHorariosDisponiveisNoDia( dia );
				GGroup<HorarioAula*> horDispDiaSL2 = sl2->retornaHorariosDisponiveisNoDia( dia );

				#pragma region Cria o mapa de compatibilidade de horarios de sl1 e sl2

				// Map para armazenar as compatibilidades
				// Para cada HorarioAula H, lista todos os HorarioAula h que não sobrepõem H
				std::map< HorarioAula*, GGroup<HorarioAula*> > map_compatHorarioSL1PorSalaDia;

				ITERA_GGROUP( it_H, horDispDiaSL1, HorarioAula )
				{
					ITERA_GGROUP( it_h, horDispDiaSL2, HorarioAula )
					{
						// Se it_H não se sobrepor com it_h
						if ( problemData->compatibilidadesDeHorarios[ *it_H ].find( *it_h ) !=
								problemData->compatibilidadesDeHorarios[ *it_H ].end() )
						{
							map_compatHorarioSL1PorSalaDia[*it_H].add( *it_h );
						}
					}
				}

				#pragma endregion

				#pragma region Calcula todas as combinações possíveis entre horarios compativeis de sl2

				// Map para armazenar as combinações (sub-conjuntos)
				std::map< std::set< HorarioAula* >, int /*nro de ocorrencias*/ > todosSubConjuntos;

				for ( std::map< HorarioAula*, GGroup<HorarioAula*> >::iterator it_H = map_compatHorarioSL1PorSalaDia.begin();
						it_H != map_compatHorarioSL1PorSalaDia.end(); it_H++  )
				{
					// Conjunto com todos os subconjuntos com h's compativeis com H
					GGroup< std::set< HorarioAula* > > subCjt = calculaSubCjtHorarios( it_H->second ); // TODO: tentar reaproveitar os calculos anteriores
					
					for ( GGroup< std::set< HorarioAula* > >::iterator it_h = subCjt.begin();
							it_h != subCjt.end(); it_h++ )
					{
						if ( todosSubConjuntos.find( *it_h ) == todosSubConjuntos.end() )
						{
							todosSubConjuntos[ *it_h ] = 1; // Insere um novo subCjt
						}
						else
						{
							todosSubConjuntos[ *it_h ]++; // Incrementa o numero de ocorrencias do subCjt
						}
					}
				}
				
				#pragma endregion

				#pragma region Cria as combinações de maximos de creditos

				bool REMOCAO = false;
				int k=0;
				aluno->getCombinaCredSLSize()[dia] = k;

				for ( int i = 0; i <= (int) horDispDiaSL1.size(); i++ )
				{
					int maxSL2 = 0;
					
					std::map< std::set< HorarioAula* >, int /*nro de ocorrencias*/ >::iterator it = todosSubConjuntos.begin();
					
					for (; it != todosSubConjuntos.end(); it++ )
					{
						int nHorariosSl1 = it->second;
						int nHorariosSl2 = it->first.size();

						if ( i == nHorariosSl1 && maxSL2 < nHorariosSl2 )
						{
							maxSL2 = it->first.size();
						}
					}
					if (i==0)
						maxSL2 = horDispDiaSL2.size();

					if ( !aluno->combinaCredSL_eh_dominado( i, sl1, maxSL2, sl2, dia ) &&
						 !aluno->combinaCredSL_eh_repetido( i, sl1, maxSL2, sl2, dia ) ) // atencao para a ordem: i refere-se a sl1 & maxSL2 refere-se a sl2
					{
						aluno->setCombinaCredSL( dia, k, sl1, i );
						aluno->setCombinaCredSL( dia, k, sl2, maxSL2 );
						
						k++;
						aluno->setCombinaCredSLSize(dia, k);

						if ( aluno->combinaCredSL_domina( i, sl1, maxSL2, sl2, dia ) != NULL )
						{
							// sera necessario remover itens dominados
							REMOCAO = true;
						}
					}
				}
				
				#pragma region Remoção de itens dominados

				if (REMOCAO)
				{
					std::map< Trio<int, int, Calendario*>, int > dominados = aluno->retornaCombinaCredSL_Dominados(dia);
					
					// Deleta itens dominados
					std::map< Trio<int, int, Calendario*>, int >::iterator it_mapDominados = dominados.begin();
					for ( ; it_mapDominados != dominados.end(); it_mapDominados++  )
					{
						aluno->removeCombinaCredSL( it_mapDominados->first );						
					}
					int size = k - dominados.size()/2;
					aluno->setCombinaCredSLSize(dia, size);
					
				}
				#pragma endregion
				
				#pragma endregion
			}
		}
	}
}




GGroup< std::set<HorarioAula*> > ProblemDataLoader::calculaSubCjtHorarios( GGroup< HorarioAula* > cjtTotal )
{
	int n = cjtTotal.size();
	int *vetor = new int[ n+1 ];
	
	std::set< std::set<int> > subCjtsInt;

	// chama a função de calcular todos os subconjuntos considerando a posição de cada elemento
	backtrack( vetor, 0, n, &subCjtsInt );

	GGroup< std::set<HorarioAula*> > subCjts;
	 
	for ( std::set< std::set<int> >::iterator it_subCjtInt = subCjtsInt.begin();
		  it_subCjtInt != subCjtsInt.end(); it_subCjtInt++ )
    {
		if ( (*it_subCjtInt).size() == 0 )
			continue;

		std::set<HorarioAula*> sub_cjt;

		for ( std::set<int>::iterator it_elemInt = (*it_subCjtInt).begin();
			  it_elemInt != (*it_subCjtInt).end();
			  it_elemInt++ )
		{
			// o valor de it_elemInt indica a posição do vetor cjtTotal que entra no subConjunto corrente.	
			int pos = 1;
			for ( GGroup< HorarioAula* >::iterator it_h = cjtTotal.begin();
				  it_h != cjtTotal.end(); it_h++, pos++ )
			{
				if ( pos == *it_elemInt )
				{
					sub_cjt.insert( *it_h );
					break;
				}				
			}
		}
		
		subCjts.add( sub_cjt );

	}
	
	return subCjts;
}

/*
	-------------------------------------------------------------------------------
	Início dos métodos genéricos para calcular todos os subconjuntos de um conjunto de inteiros
	Chamar backtrack( a, k, n, subCjts ), aonde:
	a -> é um vetor vazio de inteiros. A posicao 0 é desconsiderada, entao deve-se ter 1 posicao a mais!
	k -> a partir de qual posicao deve-se considerar todas as combinações
	n -> valor do limite superior de inteiro considerado para as combinações
	subCjts -> aonde será armazenado o resultado
	
	Para obter-se todas as combinações de elementos de a, fazer k = 0 e n = size(a)-1.
	Ex: para a[5] => k=0 e n=4, e o resultado terá as combinações de {1, 2, 3, 4}.
*/

bool ProblemDataLoader::eh_uma_solucao(int a[], int k, int n)
{
return (k == n);
}
 
void ProblemDataLoader::construir_candidatos(int a[], int k, int n, int c[], int *ncandidatos)
{
c[0] = 1;
c[1] = 0;
 
*ncandidatos = 2;
}
 
void ProblemDataLoader::processar_solucao( int a[], int k, std::set< std::set<int> > *subCjts )
{
	int i; /* Contador */

	std::set<int> cjt;
	
//	printf("{");
	for (i=1; i<=k; i++)
	{
		if (a[i])
		{
			cjt.insert(i);
//			printf(" %d", i);
		}
	}
//	printf(" }\n");

	subCjts->insert( cjt );

}

void ProblemDataLoader::backtrack(int a[], int k, int n, std::set< std::set<int> > *subCjts)
{
        int c[2];  /* Candidatos para a próxima posição */
        int ncandidatos;       /* Número de candidatos para a próxima posição */
        int i;                 /* Contador */
        
        if (eh_uma_solucao(a, k, n))
        {
                processar_solucao(a, k, subCjts);
        }
        else
        {
                k = k + 1;
                construir_candidatos(a, k, n, c, &ncandidatos);
                for (i=0; i<ncandidatos; i++)
                {
                        a[k] = c[i];
                        backtrack(a, k, n, subCjts);
                }
        }
}

/*
	Fim dos métodos de cálculo de subconjuntos 
	-----------------------------------------------------------------------------
*/