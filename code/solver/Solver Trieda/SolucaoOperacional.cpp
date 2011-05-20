#include <fstream>

#include "ofbase.h"
#include "GGroup.h"
#include "SolucaoOperacional.h"
#include "Aula.h"

SolucaoOperacional::SolucaoOperacional( ProblemData * prbDt ) : problem_data( prbDt )
{
   // FIXANDO OS VALORES: 7 (dias)
   total_dias = 7;
   total_horarios = max_horarios_dia();
   total_professores = 0;

   // Montando um map: dado o índice da matriz
   // (o 'idOperacional' do professor) o map
   // retorna o ponteiro para o professor correspondente
   GGroup< Campus *, LessPtr< Campus > >::iterator it_campi
      = prbDt->campi.begin();
   for (; it_campi != prbDt->campi.end(); it_campi++)
   {
      GGroup< Professor * >::iterator it_prof
         = it_campi->professores.begin();
      for (; it_prof != it_campi->professores.end(); it_prof++ )
      {
         this->mapProfessores[ it_prof->getId() ] = ( *it_prof );
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

   Aula * aula_virtual = new Aula( true );
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
      std::vector< Aula * > * linha = *itMatrizAulas;
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
}

int SolucaoOperacional::posicao_horario_aula( int id_horario_aula )
{
	int posicao = -1;
	HorarioAula * horario_aula = NULL;

	for ( int i = 0; i < (int)problem_data->horarios_aula_ordenados.size(); i++ )
	{
		horario_aula = problem_data->horarios_aula_ordenados.at( i );
		if ( horario_aula->getId() == id_horario_aula )
		{
			posicao = i;
			break;
		}
	}

	return posicao;
}

SolucaoOperacional::SolucaoOperacional( SolucaoOperacional const & s )
{
   this->problem_data = s.getProblemData();
   this->mapProfessores = s.mapProfessores;

   this->total_dias = s.getTotalDias();
   this->total_horarios = s.getTotalHorarios();
   this->total_professores = s.getTotalDeProfessores();
   this->matriz_aulas = new MatrizSolucao( s.getMatrizAulas()->size() );

   for( int prof = 0; prof < (int)s.getMatrizAulas()->size(); ++prof )
   {
      std::vector< Aula * > * aulas = new std::vector< Aula * >
		  ( s.getMatrizAulas()->at( prof )->size(), NULL );

      for( int a = 0; a < (int)s.getMatrizAulas()->at( prof )->size(); ++a )
      {
         aulas->at(a) = s.getMatrizAulas()->at( prof )->at( a );
      }

      this->matriz_aulas->at( prof ) = ( aulas );
   }

   this->blocoAulas = s.blocoAulas;
}

// Método relacionado com a issue TRIEDA-887
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
   ITERA_GGROUP( it_horario, professor->horarios, Horario )
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

MatrizSolucao * SolucaoOperacional::getMatrizAulas() const
{
   return ( this->matriz_aulas );
}

bool SolucaoOperacional::seqHorarioLivre(int idOperacionaProf, std::vector<int> & seqHorarioAula) const
{
   std::vector<int>::const_iterator 
      itHorarioAula = seqHorarioAula.begin();

   for(; itHorarioAula != seqHorarioAula.end(); ++itHorarioAula)
      if(matriz_aulas->at(idOperacionaProf)->at(*itHorarioAula) != NULL)
         return false;

   return true;
}

// Imprime as aulas da matriz de solução,
// percorrendo as linhas de cada professor
void SolucaoOperacional::toString() const
{
   std::cout << std::endl << "Alocacao das aulas : "
             << std::endl << std::endl;

   Professor * professor = NULL;
   for ( int i = 0; i < (int)this->getMatrizAulas()->size(); i++ )
   {
      professor = this->getProfessorMatriz(i);
      if ( professor != NULL )
      {
         std::cout << std::endl << "Nome do professor :\n"
                   << professor->getNome() << std::endl << std::endl;
      }

      // Imprime as aulas deste professor
      std::vector< Aula * > * aulas = ( this->getMatrizAulas()->at(i) );
      for ( int j = 0; j < (int)aulas->size(); j++)
      {
         Aula * aula = aulas->at( j );
         if ( aula != NULL && !aula->eVirtual() )
         {
            aula->toString();
         }
      }

      std::cout << "\n-----------------------\n";
   }
}

void SolucaoOperacional::toString2() const
{
   std::cout << "\nAlocacao das aulas\n\n";
   Professor * professor = NULL;
   for ( int i = 0; i < (int)this->getMatrizAulas()->size(); i++ )
   {
      professor = this->getProfessorMatriz( i );
      if ( professor != NULL )
      {
         std::cout << "P" << professor->getId() << ", ";
      }

      std::vector< Aula * > * aulas = ( this->getMatrizAulas()->at( i ) );
      for ( int j = 0; j < (int)aulas->size(); j++ )
      {
         Aula * aula = aulas->at(j);
         if ( aula != NULL && !aula->eVirtual() )
         {
            std::cout << "T(" << aula->getTurma()
					  << ")_D(" << aula->getDisciplina()->getCodigo()
                      << ")_S(" << aula->getSala()->getCodigo()
					  << ")_Dia(" << aula->getDiaSemana() << "),\t";
         }
         else if ( aula != NULL && aula->eVirtual() )
         {
            std::cout << "Aula Virtual,\t";
         }
         else
         {
            std::cout << "Hr. Vago,\t";
         }
      }

      std::cout << "\n\n";
   }
}

Horario * SolucaoOperacional::getHorario( int id_professor_operacional,
                                         int dia_semana, int id_horario_aula ) const
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

Professor * SolucaoOperacional::getProfessorMatriz( int linha ) const
{
   Professor * professor = NULL;

   // Procura pelo professor que possua o 'id_operacioanal'
   // igual ao índice 'linha' informado, que corresponde à
   // linha do professor na matriz de solução operacional
   std::map< int, Professor * >::const_iterator it_professor
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

   int id_operacional = ( matriz_aulas->size() - 1 );

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

void SolucaoOperacional::validaSolucao( std::string msg ) const
{

}

bool SolucaoOperacional::fixacaoDiscSala( Aula * aula )
{
	int disciplina_id = aula->getDisciplina()->getId();
	int sala_id = aula->getSala()->getId();

	std::map< Disciplina *, Sala * >::iterator it_disc_sala
		= problem_data->map_Discicplina_Sala_Fixados.begin();
	for (; it_disc_sala != problem_data->map_Discicplina_Sala_Fixados.end();
		   it_disc_sala++ )
	{
		if ( it_disc_sala->first->getId() == disciplina_id
			&& it_disc_sala->second->getId() == sala_id )
		{
			return true;
		}
	}

	return false;
}

bool SolucaoOperacional::fixacaoDiscSalaDiaHorario( Aula * aula, int indice_horario_aula )
{
	int disciplina_id = aula->getDisciplina()->getId();
	int sala_id = aula->getSala()->getId();
	int dia_semana = aula->getDiaSemana();
	int horairo_aula_id = problem_data->horarios_aula_ordenados[ indice_horario_aula ]->getId();

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Disc_Sala_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->disciplina->getId() == disciplina_id
			&& it_fixacao->sala->getId() == sala_id
			&& it_fixacao->getDiaSemana() == dia_semana
			&& it_fixacao->horario_aula->getId() == horairo_aula_id )
		{
			return true;
		}
	}

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->disciplina->getId() == disciplina_id
			&& it_fixacao->sala->getId() == sala_id
			&& it_fixacao->getDiaSemana() == dia_semana
			&& it_fixacao->horario_aula->getId() == horairo_aula_id )
		{
			return true;
		}
	}

	return false;
}

bool SolucaoOperacional::fixacaoDiscDiaHorario( Aula * aula, int indice_horario_aula )
{
	int disciplina_id = aula->getDisciplina()->getId();
	int dia_semana = aula->getDiaSemana();
	int horairo_aula_id = problem_data->horarios_aula_ordenados[ indice_horario_aula ]->getId();

	std::map< std::pair< Disciplina *, int >, int >::iterator it_map
		= problem_data->map_Discicplina_DiaSemana_CreditosFixados.begin();

	for (; it_map != problem_data->map_Discicplina_DiaSemana_CreditosFixados.end();
		   it_map++ )
	{
		if ( it_map->first.first->getId() == disciplina_id
			&& it_map->first.second == dia_semana
			&& it_map->second > 0 ) // número de créditos fixados
		{
			return true;
		}
	}

	return false;
}

// Fixações do operacional
bool SolucaoOperacional::fixacaoProfDiscSalaDiaHorario(
	 Aula * aula, int id_operacional_professor, int indice_horario_aula )
{
	int disciplina_id = aula->getDisciplina()->getId();
	int sala_id = aula->getSala()->getId();
	int dia_semana = aula->getDiaSemana();
	int horairo_aula_id = problem_data->horarios_aula_ordenados[ indice_horario_aula ]->getId();

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id
			&& it_fixacao->sala->getId() == sala_id
			&& it_fixacao->getDiaSemana() == dia_semana
			&& it_fixacao->horario_aula->getId() == horairo_aula_id )
		{
			return true;
		}
	}

	return false;
}

bool SolucaoOperacional::fixacaoProfDiscDiaHorario( Aula * aula, int id_operacional_professor, int indice_horario_aula )
{
	int disciplina_id = aula->getDisciplina()->getId();
	int dia_semana = aula->getDiaSemana();
	int horairo_aula_id = problem_data->horarios_aula_ordenados[ indice_horario_aula ]->getId();

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id
			&& it_fixacao->getDiaSemana() == dia_semana
			&& it_fixacao->horario_aula->getId() == horairo_aula_id )
		{
			return true;
		}
	}

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id
			&& it_fixacao->getDiaSemana() == dia_semana
			&& it_fixacao->horario_aula->getId() == horairo_aula_id )
		{
			return true;
		}
	}

	return false;
}

bool SolucaoOperacional::fixacaoProfDisc( Aula * aula, int id_operacional_professor )
{
	int disciplina_id = aula->getDisciplina()->getId();

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id )
		{
			return true;
		}
	}

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id )
		{
			return true;
		}
	}

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id )
		{
			return true;
		}
	}

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id )
		{
			return true;
		}
	}

	return false;
}

bool SolucaoOperacional::fixacaoProfDiscSala( Aula * aula, int id_operacional_professor )
{
	int disciplina_id = aula->getDisciplina()->getId();
	int sala_id = aula->getSala()->getId();

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id
			&& it_fixacao->sala->getId() == sala_id )
		{
			return true;
		}
	}

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->disciplina->getId() == disciplina_id
			&& it_fixacao->sala->getId() == sala_id )
		{
			return true;
		}
	}

	return false;
}

bool SolucaoOperacional::fixacaoProfSala( Aula * aula, int id_operacional_professor )
{
	int sala_id = aula->getSala()->getId();

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->sala->getId() == sala_id )
		{
			return true;
		}
	}

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Disc_Sala, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->sala->getId() == sala_id )
		{
			return true;
		}
	}

	ITERA_GGROUP_LESSPTR( it_fixacao,
		problem_data->fixacoes_Prof_Sala, Fixacao )
	{
		if ( it_fixacao->professor->getIdOperacional() == id_operacional_professor
			&& it_fixacao->sala->getId() == sala_id )
		{
			return true;
		}
	}

	return false;
}

void SolucaoOperacional::alocaAulaProf(Aula & aula, Professor & professor, std::vector<HorarioAula*> horariosAula)
{
   // Alocando a aula.
   int diaSemana = aula.getDiaSemana();

   std::vector<HorarioAula*>::iterator itHorariosAula = horariosAula.begin();

   for(; itHorariosAula != horariosAula.end(); ++itHorariosAula)
   {
      int horarioAulaId = (*itHorariosAula)->getId();
      int profIdOp = professor.getIdOperacional();

      this->matriz_aulas->at(profIdOp)->at(
         ((diaSemana-1) * total_horarios ) + posicao_horario_aula(horarioAulaId) )
         
         = &aula;
   }

   /* Anotando na estrutura auxiliar da solução as posições dos horários alocados pra um professor 
   ministrar uma aula. */
   this->blocoAulas[&aula] = std::make_pair(&professor,horariosAula);
}
