#include "ofbase.h"
#include "GGroup.h"
#include "SolucaoOperacional.h"
#include "Aula.h"

SolucaoOperacional::SolucaoOperacional(ProblemData * prbDt)
{
   // FIXAR OS VALORES: 7 (dias) 4 (horarios)
   total_dias = 7;
   total_horarios = 4;
   total_professores = 0;

   this->problem_data = (prbDt);

   // Montando um map: dado o �ndice da matriz (o 'idOperacional'
   // do professor) o map retorna o ponteiro para o professor correspondente
   GGroup< Campus * >::iterator it_campi
      = prbDt->campi.begin();
   for (; it_campi != prbDt->campi.end(); it_campi++)
   {
      GGroup< Professor * >::iterator it_prof
         = it_campi->professores.begin();
      for (; it_prof != it_campi->professores.end(); it_prof++)
      {
         mapProfessores[ it_prof->getId() ] = (*it_prof);
      }
   }

   total_professores = mapProfessores.size();

   // Inicializando a estrutura <matrizAulas>
   matriz_aulas = new MatrizSolucao (total_professores);
   MatrizSolucao::iterator itMatrizAulas = matriz_aulas->begin();
   for(; itMatrizAulas != matriz_aulas->end(); ++itMatrizAulas)
   {
      *(itMatrizAulas) = new vector< Aula * > ((total_dias * total_horarios), NULL);
   }

   unsigned int i = 0;
   unsigned int j = 0;
   int dia_semana = 0;
   int horario_aula_id = 0;

   Aula * aula_virtual = new Aula(true);
   Professor * professor = NULL;

   // Deixando livres, apenas os horarios em que o professor pode
   // ministrar aulas. Para os demais, associa-as � uma aula virtual.
   itMatrizAulas = getMatrizAulas()->begin();
   for(i = 0; itMatrizAulas != this->getMatrizAulas()->end();
	   ++itMatrizAulas, i++)
   {
	   // Professor da linha atual da matriz
	   professor = getProfessorMatriz(i);
       int idProf = professor->getIdOperacional();

	   // Vetor de aulas do professor atual
	   vector< Aula * > * linha = *( itMatrizAulas );
	   vector< Aula * >::iterator it_aula = linha->begin();
	   for( j = 0; j < linha->size(); j++, it_aula++ )
	   {
		   // DIA DA SEMANA
		   // 1 --> domingo,
		   // 2 --> segunda-feira
		   // (...)
		   // 6 --> sexta-feira
		   // 7 --> s�bado
		   dia_semana = ( j / total_horarios ) + 1;

		   // �ndice do hor�rio da aula
		   horario_aula_id = ( j % total_horarios );

		   if ( !horarioDisponivelProfessor(
					professor, dia_semana, horario_aula_id) )
		   {
			   // A aula NAO pode ser alocada
			   *it_aula = aula_virtual;
		   }
	   }
   }

   // -----------------------------------------------------
   /* Inicializando a estrutura <refHorarios> */

   ITERA_GGROUP(itCampus,problem_data->campi,Campus)
   {
      ITERA_GGROUP(itHorario,itCampus->horarios,Horario)
      {
         ITERA_GGROUP_N_PT(itDia,itHorario->dias_semana,int)
         {
            std::vector<HorarioAula*>::iterator 
               itHA = std::find(
               problem_data->horarios_aula_ordenados.begin(),
               problem_data->horarios_aula_ordenados.end(),
               itHorario->horario_aula);

            int idOperacional = ((*itDia - 1) * total_horarios) + std::distance(problem_data->horarios_aula_ordenados.begin(),itHA);

            refHorarios[std::make_pair(*itHorario,*itDia)] = idOperacional;
         }
      }
   }

   // -----------------------------------------------------
}

SolucaoOperacional::~SolucaoOperacional()
{
   // Limpa as refer�ncias do map de professores
   this->mapProfessores.clear();

   // Limpa a refer�ncia do objeto 'problemData'
   this->problem_data = NULL;

   // Limpa as refer�ncias da matriz de solu��o operacional
   MatrizSolucao::iterator itMatrizAulas
	   = this->getMatrizAulas()->begin();
   for(; itMatrizAulas != this->getMatrizAulas()->end();
	   ++itMatrizAulas)
   {
      delete *itMatrizAulas;
   }

   matriz_aulas = NULL;
}

bool SolucaoOperacional::horarioDisponivelProfessor(
	Professor* professor, int dia_semana, int horario_aula_id)
{
	// Recupera o hor�rio de aula que
	// corresponde ao 'horario_aula_id' informado
	HorarioAula* horario_aula = NULL;
	int tam_vector = this->getProblemData()->horarios_aula_ordenados.size();
	if (horario_aula_id < tam_vector)
	{
		horario_aula = this->getProblemData()->horarios_aula_ordenados.at(horario_aula_id);
	}
	else
	{
		std::cerr << "Acessando indice invalido do vector." << std::endl;
		std::cerr << "SolucaoOperacional::horarioDisponivelProfessor()" << std::endl;
		exit(1);
	}

	// Para cada hor�rio dispon�vel do professor informado,
	// procura pela disponibilidade 'dia de semana' / 'hor�rio'
	ITERA_GGROUP(it_horario, professor->horarios, Horario)
	{
		// Esse hor�rio corresponde ao mesmo hor�rio de aula procurado
		if ( it_horario->horario_aula->getId() == horario_aula->getId() )
		{
			// Verifica-se se o professor pode atender
			// a esse hor�rio de aula no dia da semana procurado
			GGroup<int>::iterator it_dia_semana
				= it_horario->dias_semana.begin();
			for (; it_dia_semana != it_horario->dias_semana.end(); it_dia_semana++)
			{
				// Dia da semana dispon�vel
				if (*it_dia_semana == dia_semana)
				{
					return true;
				}
			}
		}
	}

	// O hor�rio n�o est� dispon�vel
	return false;
}

void SolucaoOperacional::carregaSolucaoInicial()
{
   // TODO -- Carregar a solu��o inicial, antes de avaliar
}

MatrizSolucao* SolucaoOperacional::getMatrizAulas() const
{
   return (this->matriz_aulas);
}

void SolucaoOperacional::setMatrizAulas(MatrizSolucao* matriz)
{
   this->matriz_aulas = (matriz);
}

// Imprime as aulas da matriz de solu��o,
// percorrendo as linhas de cada professor
void SolucaoOperacional::toString()
{
   std::cout << std::endl << "Alocacao das aulas : "
			 << std::endl << std::endl;

   Professor * professor = NULL;
   for (unsigned int i = 0; i < this->getMatrizAulas()->size(); i++)
   {
      professor = this->getProfessorMatriz(i);
      if (professor != NULL)
      {
         std::cout << std::endl << "Nome do professor : " << std::endl
				   << professor->getNome() << std::endl << std::endl;
      }

      // Imprima as aulas deste professor
      std::vector< Aula * > * aulas = ( this->getMatrizAulas()->at(i) );
      for (unsigned int j = 0; j < aulas->size(); j++)
      {
         Aula* aula = aulas->at(j);
         if (aula != NULL && !aula->eVirtual())
         {
            aula->toSring();
         }
         else if (aula != NULL && aula->eVirtual())
         {
            // std::cout << "Aula virtual" << std::endl;
         }
         else
         {
            // std::cout << "Aula nao alocada" << std::endl;
         }
      }

      std::cout << std::endl << "-----------------------" << std::endl;
   }
}

// Dado um dia da semana e um hor�rio de aula, devo informar
// a coluna da matriz de solu��o correspondente a esse par de
// dia da semana/hor�rio. A linha da matriz j� � dada pelo �ndice
// do professor que faz a busca com esse dia e hor�rio
int SolucaoOperacional::getIndiceMatriz(int dia, Horario* horario)
{
   int dia_semana = ((dia-1) * problem_data->max_horarios_professor);
   int horario_dia = (horario->getHorarioAulaId()-1);
   
   return (dia_semana + horario_dia);
}

Horario * SolucaoOperacional::getHorario(int i, int j)
{
   Aula * aula = NULL;
   Aula * aula_referencia = ( this->getMatrizAulas()->at(i)->at(j) );
   Horario * horario = NULL;

   // Procura pelo primeiro hor�rio
   // do bloco de aula correspondente
   int k = (j - 1);
   while( k >= 0 )
   {
      // Recupera a aula atual
      aula = this->getMatrizAulas()->at(i)->at(k);
	  if ( aula == NULL || aula != aula_referencia
			|| aula->eVirtual() == false)
      {
         break;
      }

      k--;
   }

   // Se k = -1, ent�o a aula do �ndice 'zero' � a aula que
   // procuramos, ou seja, a primeira aula do bloco de aulas
   // atual. Caso contr�rio, a aula procurada � a primeira �
   // direita da aula de �ndice 'k'. Nos dois casos, devemos
   // incrementar o �ndice 'k' em uma unidade.
   k++;

   // Pega a posi��o do hor�rio desejado
   // dentro do bloco de aulas atual
   int posicao_aula = (j-k);

   // Recupera o hor�rio desejado
   aula = this->getMatrizAulas()->at(i)->at(k);
   if (posicao_aula < (int)aula->bloco_aula.size())
   {
       horario = aula->bloco_aula[ posicao_aula ].second;
   }
   else
   {
	   std::cout << "metodo SolucaoOperacional::getHorario()" << std::endl;
	   std::cout << "Existe aulas que nao foram alocadas" << std::endl << std::endl;
	   exit(1);
   }

   return horario;
}

Professor * SolucaoOperacional::getProfessorMatriz(int linha)
{
   Professor * professor = NULL;

   // Procura pelo professor que possua o 'id_operacioanal'
   // igual ao �ndice 'linha' informado, que corresponde �
   // linha do professor na matriz de solu��o operacional
   std::map< int, Professor * >::iterator it_professor
      = mapProfessores.begin();
   for (; it_professor != mapProfessores.end(); it_professor++)
   {
      if (it_professor->second->getIdOperacional() == linha)
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
   if(dia < 1 || dia > 7 )
   {
      throw (out_of_range("Dias validos [1,7] -> dom. a sab."));
   }

   return ((matriz_aulas->at(professor.getIdOperacional())->begin()) +
		   ( ((dia - 1) * total_horarios) + horario ) );
}

int SolucaoOperacional::addProfessor(
	Professor & professor, vector< Aula * > & horariosProf)
{
   matriz_aulas->push_back( &(horariosProf) );

   int id_operacional = (matriz_aulas->size() - 1);

   professor.setId(-id_operacional);
   professor.setIdOperacional(id_operacional);

   if(mapProfessores.find(professor.getId()) != mapProfessores.end())
   {
	  std::cerr << "SolucaoOperacional::addProfessor" << std::endl;
      std::cerr << "Erro ao adicionar um professor virtual"
				<< std::endl << std::endl;
      exit(1);
   }

   mapProfessores[ professor.getId() ] = &(professor);

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

bool SolucaoOperacional::podeTrocarHorariosAulas(Aula & aX, Aula & aY) const
{
   std::vector< std::pair< Professor *, Horario * > > novosHorariosAX;
   std::vector< std::pair< Professor *, Horario * > > novosHorariosAY;

   novosHorariosAX = aY.bloco_aula;
   novosHorariosAY = aX.bloco_aula;

   return (!checkConflitoBlocoCurricular(aX, novosHorariosAX) && !checkConflitoBlocoCurricular(aY, novosHorariosAY));
}


bool SolucaoOperacional::checkConflitoBlocoCurricular(
   Aula & aula, std::vector< std::pair< Professor *, Horario * > > & novosHorariosAula) const
{
   std::map<Aula *, GGroup<BlocoCurricular*,LessPtr<BlocoCurricular> > >::iterator
      itAulaBlocosCurriculares = problem_data->aulaBlocosCurriculares.find(&aula);

   if(itAulaBlocosCurriculares == problem_data->aulaBlocosCurriculares.end())
   {
      std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular> alguma aula nao foi encontrada." << std::endl;
      exit(1);
   }

   // Para cada Bloco Curricular ao qual a aula pertence
   ITERA_GGROUP_LESSPTR(itBlocoCurric,itAulaBlocosCurriculares->second,BlocoCurricular)
   {
      std::map<BlocoCurricular*,GGroup<Aula*,LessPtr<Aula> > >::iterator
         itBlocoCurricularAulas = problem_data->blocoCurricularAulas.find(
         *itBlocoCurric);

      if(itBlocoCurricularAulas == problem_data->blocoCurricularAulas.end())
      {
         std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular> algum bloco nao foi encontrado." << std::endl;
         exit(1);
      }

      /* Para todas as aulas do bloco curricular em quest�o, salvo a aula corrente, verificar
      se h� conflito de hor�rio. */
      ITERA_GGROUP_LESSPTR(itAulasBloco,itBlocoCurricularAulas->second,Aula)
      {
         /* Somente se n�o for igual a Aula em quest�o E se a aula selecionada para an�lise
         j� estiver alocada a algum hor�rio. A segunda condi��o � necess�ria para a cria��o de
         uma solu��o inicial. */
         if(**itAulasBloco != aula && !(itAulasBloco->bloco_aula.empty()))
         {
            std::vector< std::pair< Professor *, Horario * > >::iterator 
               itNovosHorariosAula = novosHorariosAula.begin();

            /* Para cada novo hor�rio da aula em quest�o, checo se ele conflita com 
            algum hor�rio da aula selecionada do bloco curricular. */
            for(; itNovosHorariosAula != novosHorariosAula.end(); ++itNovosHorariosAula)
            {
               std::vector< std::pair< Professor *, Horario * > >::iterator 
                  itHorariosAula = itAulasBloco->bloco_aula.begin();

               /* Para cada hor�rio da da aula selecionada do bloco curricular */
               for(; itHorariosAula != itAulasBloco->bloco_aula.end(); ++itHorariosAula)
               {
                  // Se conflitar, o movimento � invi�vel.
                  if( (*(itNovosHorariosAula->first) == *(itHorariosAula->first)) &&
                     (*(itNovosHorariosAula->second) == *(itHorariosAula->second)) )
                     return true;
               }
            }
         }
      }
   }

   return false;
}