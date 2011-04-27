#include "ofbase.h"
#include "GGroup.h"
#include "SolucaoOperacional.h"
#include "Aula.h"

SolucaoOperacional::SolucaoOperacional( ProblemData * prbDt )
{
   this->problem_data = ( prbDt );

   // FIXANDO OS VALORES: 7 (dias)
   total_dias = 7;
   total_horarios = max_horarios_dia();
   total_professores = 0;

   // Montando um map: dado o índice da matriz
   // (o 'idOperacional' do professor) o map
   // retorna o ponteiro para o professor correspondente
   GGroup< Campus * >::iterator it_campi
      = prbDt->campi.begin();
   for (; it_campi != prbDt->campi.end(); it_campi++)
   {
      GGroup< Professor * >::iterator it_prof
         = it_campi->professores.begin();
      for (; it_prof != it_campi->professores.end(); it_prof++)
      {
         this->mapProfessores[ it_prof->getId() ] = (*it_prof);
      }
   }

   this->total_professores = this->mapProfessores.size();

   // Inicializando a estrutura <matrizAulas>
   matriz_aulas = new MatrizSolucao ( total_professores );
   MatrizSolucao::iterator itMatrizAulas = matriz_aulas->begin();
   for(; itMatrizAulas != matriz_aulas->end();
		 ++itMatrizAulas )
   {
	   ( *itMatrizAulas ) = new std::vector< Aula * > ( (total_dias * total_horarios), NULL );
   }

   unsigned int i = 0;
   unsigned int j = 0;
   int dia_semana = 0;
   int horario_aula_id = 0;

   Aula * aula_virtual = new Aula(true);
   Professor * professor = NULL;

   // Deixando livres, apenas os horarios em que o professor pode
   // ministrar aulas. Para os demais, associa-as à uma aula virtual.
   itMatrizAulas = getMatrizAulas()->begin();
   for ( i = 0; itMatrizAulas != this->getMatrizAulas()->end();
	     ++itMatrizAulas, i++ )
   {
	   // Professor da linha atual da matriz
	   professor = getProfessorMatriz(i);
       int idProf = professor->getIdOperacional();

	   // Vetor de aulas do professor atual
	   std::vector< Aula * > * linha = *( itMatrizAulas );
	   std::vector< Aula * >::iterator it_aula = linha->begin();
	   for( j = 0; j < linha->size(); j++, it_aula++ )
	   {
		   // DIA DA SEMANA
		   // 1 --> domingo,
		   // 2 --> segunda-feira
		   // (...)
		   // 6 --> sexta-feira
		   // 7 --> sábado
		   dia_semana = ( j / total_horarios ) + 1;

		   // Índice do horário da aula
		   horario_aula_id = ( j % total_horarios );

		   if ( !horarioDisponivelProfessor(
					professor, dia_semana, horario_aula_id ) )
		   {
			   // A aula NAO pode ser alocada
			   ( *it_aula ) = aula_virtual;
		   }
	   }
   }
   // -----------------------------------------------------

   // -----------------------------------------------------
}

int SolucaoOperacional::max_horarios_dia()
{
	int tam = 0;
	int max_horarios = 0;
	std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
		it_map = this->getProblemData()->horarios_aula_dia_semana.begin();
	for (; it_map != this->getProblemData()->horarios_aula_dia_semana.end();
		   it_map++ )
	{
		tam = it_map->second.size();
		if ( tam > max_horarios )
		{
			max_horarios = tam;
		}
	}

	return tam;
}

SolucaoOperacional::~SolucaoOperacional()
{
   // Limpa as referências do map de professores
   this->mapProfessores.clear();

   // Limpa a referência do objeto 'problemData'
   this->problem_data = NULL;

   // Limpa as referências da matriz de solução operacional
   MatrizSolucao::iterator itMatrizAulas
	   = this->getMatrizAulas()->begin();
   for(; itMatrizAulas != this->getMatrizAulas()->end();
	     ++itMatrizAulas )
   {
      delete ( *itMatrizAulas );
   }

   matriz_aulas = NULL;
}

bool SolucaoOperacional::horarioDisponivelProfessor(
	Professor * professor, int dia_semana, int horario_aula_id )
{
	// Recupera o horário de aula que
	// corresponde ao 'horario_aula_id' informado
	HorarioAula * horario_aula = NULL;
	std::vector< HorarioAula * > horarios
		= this->getProblemData()->horarios_aula_ordenados;

	int tam_vector = horarios.size();
	if ( horario_aula_id < tam_vector )
	{
		horario_aula = horarios.at( horario_aula_id );
	}
	else
	{
		// Quando o índice de horário informado não correspopnde
		// a um horário de aula válido significa que possivelmente no
		// dia da semana em questão, o professor ou o dia da semana
		// não tem esse horário de aula disponível. Portanto, informamos
		// apenas que esse horário de aula não está disponível, para que
		// uma aula virtual seja alocada na posição correspondente na solução.
		return false;
	}

	// Para cada horário disponível do professor informado,
	// procura pela disponibilidade 'dia de semana' / 'horário'
	ITERA_GGROUP( it_horario, professor->horarios,
				  Horario )
	{
		// Esse horário corresponde ao mesmo horário de aula procurado
		if ( it_horario->horario_aula->getId() == horario_aula->getId() )
		{
			// Verifica-se se o professor pode atender
			// a esse horário de aula no dia da semana procurado
			GGroup< int >::iterator it_dia_semana
				= it_horario->dias_semana.begin();
			for (; it_dia_semana != it_horario->dias_semana.end();
				   it_dia_semana++ )
			{
				// Dia da semana disponível
				if ( (*it_dia_semana) == dia_semana )
				{
					return true;
				}
			}
		}
	}

	// O horário não está disponível
	return false;
}

void SolucaoOperacional::carregaSolucaoInicial()
{
   // TODO -- Carregar a solução inicial, antes de avaliar
}

MatrizSolucao* SolucaoOperacional::getMatrizAulas() const
{
   return ( this->matriz_aulas );
}

void SolucaoOperacional::setMatrizAulas( MatrizSolucao * matriz )
{
   this->matriz_aulas = ( matriz );
}

// Imprime as aulas da matriz de solução,
// percorrendo as linhas de cada professor
void SolucaoOperacional::toString()
{
   std::cout << std::endl << "Alocacao das aulas : "
		     << std::endl << std::endl;

   Professor * professor = NULL;
   for (unsigned int i = 0; i < this->getMatrizAulas()->size(); i++)
   {
      professor = this->getProfessorMatriz(i);
      if ( professor != NULL )
      {
         std::cout << std::endl << "Nome do professor :\n"
				   << professor->getNome() << std::endl << std::endl;
      }

      // Imprima as aulas deste professor
      std::vector< Aula * > * aulas = ( this->getMatrizAulas()->at(i) );
      for (unsigned int j = 0; j < aulas->size(); j++)
      {
         Aula * aula = aulas->at(j);
         if ( aula != NULL && !aula->eVirtual() )
         {
            aula->toString();
         }
         else if ( aula != NULL && aula->eVirtual() )
         {
             // std::cout << "Aula virtual" << std::endl;
         }
         else
         {
             // std::cout << "Aula nao alocada" << std::endl;
         }
      }
      std::cout << "\n-----------------------\n";
   }
}

void SolucaoOperacional::toString2()
{
   std::cout << "\nAlocacao das aulas\n\n";
   Professor * professor = NULL;
   for ( unsigned int i = 0; i < this->getMatrizAulas()->size(); i++ )
   {
      professor = this->getProfessorMatriz(i);

      if ( professor != NULL )
	  {
         std::cout << "P" << professor->getId() << ": ";
	  }

      std::vector< Aula * > * aulas = ( this->getMatrizAulas()->at(i) );
      for (unsigned int j = 0; j < aulas->size(); j++)
      {
         Aula * aula = aulas->at(j);
         if ( aula != NULL && !aula->eVirtual() )
		 {
            std::cout << "A" << aula->getDisciplina()->getCodigo()
					  << "_" << aula->getSala()->getCodigo()
					  << "_" << aula->getDiaSemana() << ",\t";
		 }
         else if ( aula != NULL && aula->eVirtual() )
		 {
             std::cout << "Aula Virtual,\t";
		 }
         else
		 {
             std::cout << "Aula nao aloc.,\t";
		 }
      }
      std::cout << std::endl;
   }
}

// Dado um dia da semana e um horário de aula, devo informar
// a coluna da matriz de solução correspondente a esse par de
// dia da semana/horário. A linha da matriz já é dada pelo índice
// do professor que faz a busca com esse dia e horário
int SolucaoOperacional::getIndiceMatriz( int dia, Horario * horario )
{
   int dia_semana = ( (dia-1) * problem_data->max_horarios_professor );
   int horario_dia = ( horario->getHorarioAulaId()-1 );

   return ( dia_semana + horario_dia );
}

Horario * SolucaoOperacional::getHorario( int id_professor_operacional,
										  int dia_semana, int id_horario_aula )
{
   Professor * professor = this->getProfessorMatriz( id_professor_operacional );
   HorarioAula * horario_aula = this->getProblemData()->horarios_aula_ordenados[ id_horario_aula ];
   Aula * aula = this->getMatrizAulas()->at( id_professor_operacional )->at( id_horario_aula );

   // Se o professor tem aula alocada nesse horário
   if ( aula != NULL )
   {
	   ITERA_GGROUP( it_horario, professor->horarios, Horario )
	   {
		   GGroup< int >::iterator it_dia_semana
			   = it_horario->dias_semana.find( dia_semana ); 
		   if ( it_dia_semana != it_horario->dias_semana.end()
				&& it_horario->horario_aula->getInicio() == horario_aula->getInicio() )
		   {
			   return ( *it_horario );
		   }
	   }
   }

   return NULL;
}

Professor * SolucaoOperacional::getProfessorMatriz( int linha )
{
   Professor * professor = NULL;

   // Procura pelo professor que possua o 'id_operacioanal'
   // igual ao índice 'linha' informado, que corresponde à
   // linha do professor na matriz de solução operacional
   std::map< int, Professor * >::iterator it_professor
      = mapProfessores.begin();
   for (; it_professor != mapProfessores.end(); it_professor++)
   {
      if ( it_professor->second->getIdOperacional() == linha )
      {
         professor = it_professor->second;
         break;
      }
   }

   return professor;
}

ProblemData* SolucaoOperacional::getProblemData() const
{
   return problem_data;
}

std::vector< Aula * >::iterator SolucaoOperacional::getItHorariosProf(
	Professor & professor, int dia, int horario)
{
   if ( dia < 1 || dia > 7 )
   {
      throw ( std::out_of_range( "Dias validos [1,7] -> dom. a sab." ) );
   }

   return ( (matriz_aulas->at(professor.getIdOperacional())->begin()) +
		    ( ((dia - 1) * total_horarios) + horario ) );
}

int SolucaoOperacional::addProfessor(
	Professor & professor, std::vector< Aula * > & horariosProf )
{
   matriz_aulas->push_back( &(horariosProf) );

   int id_operacional = (matriz_aulas->size() - 1);

   professor.setId( -id_operacional );
   professor.setIdOperacional( id_operacional );

   if ( mapProfessores.find( professor.getId() ) != mapProfessores.end() )
   {
	  std::cerr << "SolucaoOperacional::addProfessor"
				<< "\nErro ao adicionar um professor virtual"
				<< std::endl << std::endl;

      exit(1);
   }

   mapProfessores[ professor.getId() ] = &( professor );
   return id_operacional;
}

int SolucaoOperacional::getTotalHorarios() const
{
   return total_horarios;
}

int SolucaoOperacional::getTotalDeProfessores() const
{
	return total_professores;
}

int SolucaoOperacional::getTotalDias() const
{
	return total_dias;
}

//bool SolucaoOperacional::podeTrocarHorariosAulas( Aula & aX, Aula & aY ) const
//{
//   std::vector< std::pair< Professor *, Horario * > > novosHorariosAX;
//   std::vector< std::pair< Professor *, Horario * > > novosHorariosAY;
//
//   novosHorariosAX = aY.bloco_aula;
//   novosHorariosAY = aX.bloco_aula;
//
//   return ( !checkConflitoBlocoCurricular(aX, novosHorariosAX)
//      && !checkConflitoBlocoCurricular(aY, novosHorariosAY)
//      && checkDisponibilidadeHorarioSalaAula(aX, novosHorariosAX)
//      && checkDisponibilidadeHorarioSalaAula(aY, novosHorariosAY) );
//}


//bool SolucaoOperacional::checkConflitoBlocoCurricular(
//   Aula & aula, std::vector< std::pair< Professor *, Horario * > > & novosHorariosAula) const
//{
//   std::map<Aula *, GGroup<BlocoCurricular*,LessPtr<BlocoCurricular> > >::iterator
//      itAulaBlocosCurriculares = problem_data->aulaBlocosCurriculares.find( &aula );
//
//   if ( itAulaBlocosCurriculares == problem_data->aulaBlocosCurriculares.end() )
//   {
//      std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular>"
//         << "alguma aula nao foi encontrada." << std::endl;
//
//      exit(1);
//   }
//
//   // Para cada Bloco Curricular ao qual a aula pertence
//   ITERA_GGROUP_LESSPTR( itBlocoCurric, itAulaBlocosCurriculares->second, BlocoCurricular )
//   {
//      std::map< BlocoCurricular *, std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > > >::iterator
//         itBlocoCurricularAulas = problem_data->blocoCurricularDiaAulas.find( *itBlocoCurric );
//
//      if ( itBlocoCurricularAulas == problem_data->blocoCurricularDiaAulas.end() )
//      {
//         std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular>"
//            << "algum bloco nao foi encontrado." << std::endl;
//
//         exit(1);
//      }
//
//      std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > >::iterator 
//         itBlocoCurricularDiaAulas = itBlocoCurricularAulas->second.find( aula.getDiaSemana() );
//
//      if ( itBlocoCurricularDiaAulas == itBlocoCurricularAulas->second.end() )
//      {
//         std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular>"
//            << "algum dia nao foi encontrado." << std::endl;
//
//         exit(1);
//      }
//
//      // Para todas as aulas de um dado dia do bloco curricular
//      // em questão, salvo a aula corrente, verificar se há conflito de horário.
//      ITERA_GGROUP_LESSPTR( itAulasBloco, itBlocoCurricularDiaAulas->second, Aula )
//      {
//         // Somente se não for igual a Aula em questão E se
//         // a aula selecionada para análise já estiver alocada
//         // a algum horário. A segunda condição é necessária
//         // para a criação de uma solução inicial.
//         if ( **itAulasBloco != aula && !( itAulasBloco->bloco_aula.empty() ) )
//         {
//            std::vector< std::pair< Professor *, Horario * > >::iterator 
//               itNovosHorariosAula = novosHorariosAula.begin();
//
//            // Para cada novo horário da aula em questão,
//            // checo se ele conflita com  algum horário da
//            // aula selecionada do bloco curricular.
//            for (; itNovosHorariosAula != novosHorariosAula.end();
//               ++itNovosHorariosAula )
//            {
//               std::vector< std::pair< Professor *, Horario * > >::iterator 
//                  itHorariosAula = itAulasBloco->bloco_aula.begin();
//
//               // Para cada horário da da aula selecionada do bloco curricular
//               for(; itHorariosAula != itAulasBloco->bloco_aula.end();
//                  ++itHorariosAula)
//               {
//                  // Se conflitar, o movimento é inviável.
//                  if(    (*(itNovosHorariosAula->first) == *(itHorariosAula->first))
//                     && (*(itNovosHorariosAula->second) == *(itHorariosAula->second)) )
//                  {
//                     return true;
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return false;
//}

//bool SolucaoOperacional::checkDisponibilidadeHorarioSalaAula(
//   Aula & aula, 
//   std::vector< std::pair< Professor *, Horario * > > & novosHorariosAula) const
//{
//   /* Verificando se os possíveis novos horários da aula são compatíveis com a sala a qual
//   a aula está alocada. */
//
//   // Obtendo as listas de horários para cada dia letivo comum entre uma sala e uma disciplina (aula).
//   std::map<std::pair< int /*idDisc*/, int /*idSala*/ >, 
//      std::map< int /*Dias*/, GGroup<Horario*, LessPtr<Horario> > > >::iterator
//      it_Disc_Salas_Dias_Horarios_Aula = problem_data->disc_Salas_Dias_Horarios.find(
//      std::make_pair(aula.getDisciplina()->getId(),aula.getSala()->getId()));
//
//   if(it_Disc_Salas_Dias_Horarios_Aula != problem_data->disc_Salas_Dias_Horarios.end())
//   {
//      std::vector< std::pair< Professor *, Horario * > >::iterator 
//         itNovosHorariosAula = novosHorariosAula.begin();
//
//      // Para cada horário
//      for(; (itNovosHorariosAula != novosHorariosAula.end()); ++itNovosHorariosAula)
//      {
//
//         std::map< int /*Dias*/, GGroup<Horario*, LessPtr<Horario> > >::iterator
//            it_Dias_Horarios_Aula = it_Disc_Salas_Dias_Horarios_Aula->second.find(aula.getDiaSemana());
//
//         // Se encontrei o dia procurado para as duas aulas
//         if(it_Dias_Horarios_Aula != it_Disc_Salas_Dias_Horarios_Aula->second.end())
//         {
//            // Verificando os horários.
//            if(it_Dias_Horarios_Aula->second.find(itNovosHorariosAula->second) == it_Dias_Horarios_Aula->second.end())
//            {
//               return false;
//            }
//         }
//         else
//         {
//            std::cout << "ERRO 1 em <>\n";
//            exit(1);
//         }
//
//         //if(aX.getSala()->horarios_disponiveis.find(itNovosHorariosAX->second) == 
//         //   aX.getSala()->horarios_disponiveis.end() 
//         //   ||
//         //   aY.getSala()->horarios_disponiveis.find(itNovosHorariosAY->second) == 
//         //   aY.getSala()->horarios_disponiveis.end() )
//         //{
//         //   return false;
//         //}
//
//      }
//   }
//   else
//   {
//      std::cout << "ERRO 2 em <>\n";
//      exit(1);
//   }
//
//   return true;
//}
