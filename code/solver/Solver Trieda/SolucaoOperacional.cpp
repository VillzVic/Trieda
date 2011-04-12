#include "ofbase.h"
#include "GGroup.h"
#include "SolucaoOperacional.h"
#include "Aula.h"

SolucaoOperacional::SolucaoOperacional(ProblemData * prbDt)
{
   // FIXAR OS VALORES: 6 (dias) 4 (horarios)
   total_dias = 7;
   total_horarios = 4;
   total_professores = 0;

   this->problem_data = (prbDt);

   // Montando um map: dado o índice da matriz (o 'idOperacional'
   // do professor) o map retorna o ponteiro para o professor correspondente
   GGroup<Campus*>::iterator it_campi
      = prbDt->campi.begin();
   for (; it_campi != prbDt->campi.end(); it_campi++)
   {
      GGroup<Professor*>::iterator it_prof
         = it_campi->professores.begin();
      for (; it_prof != it_campi->professores.end(); it_prof++)
      {
         mapProfessores[it_prof->getIdOperacional()] = (*it_prof);
         ++total_professores;
      }
   }

   // Inicializando a estrutura <matrizAulas>
   matriz_aulas = new MatrizSolucao (total_professores);
   MatrizSolucao::iterator itMatrizAulas = matriz_aulas->begin();
   for(; itMatrizAulas != matriz_aulas->end(); ++itMatrizAulas)
   {
      *itMatrizAulas = new vector<Aula*> ((total_dias*total_horarios), NULL);
   }

   unsigned int i = 0;
   unsigned int j = 0;
   int dia_semana = 0;
   int horario_aula_id = 0;

   Aula * aula_virtual = new Aula(true);
   Professor* professor = NULL;

   // Deixando livres, apenas os horarios em que o professor pode
   // ministrar aulas. Para os demais, associa-as à uma aula virtual.
   itMatrizAulas = getMatrizAulas()->begin();
   for(i = 0; itMatrizAulas != this->getMatrizAulas()->end(); ++itMatrizAulas, i++)
   {
	   // Professor da linha atual da matriz
	   professor = getProfessorMatriz(i);
       int idProf = professor->getIdOperacional();

	   // Vetor de aulas do professor atual
	   vector<Aula*> * linha = *itMatrizAulas;
	   vector<Aula*>::iterator it_aula = linha->begin();
	   for(j = 0; j < linha->size(); j++, it_aula++)
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

		   if (!horarioDisponivelProfessor(professor,
					dia_semana, horario_aula_id))
		   {
			   // A aula NAO pode ser alocada
			   *it_aula = aula_virtual;
		   }
	   }
   }
}

SolucaoOperacional::~SolucaoOperacional()
{
   // Limpa as referências do map de professores
   this->mapProfessores.clear();

   // Limpa a referência do objeto 'problemData'
   this->problem_data = NULL;

   // Limpa as referências da matriz de solução operacional
   MatrizSolucao::iterator itMatrizAulas = this->getMatrizAulas()->begin();
   for(; itMatrizAulas != this->getMatrizAulas()->end(); ++itMatrizAulas)
   {
      delete *itMatrizAulas;
   }
   matriz_aulas = NULL;
}

bool SolucaoOperacional::horarioDisponivelProfessor(
	Professor* professor, int dia_semana, int horario_aula_id)
{
	// Recupera o horário de aula que
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

	// Para cada horário disponível do professor informado,
	// procura pela disponibilidade 'dia de semana' / 'horário'
	ITERA_GGROUP(it_horario, professor->horarios, Horario)
	{
		// Esse horário corresponde ao mesmo horário de aula procurado
		if ( it_horario->horario_aula->getId() == horario_aula->getId() )
		{
			// Verifica-se se o professor pode atender
			// a esse horário de aula no dia da semana procurado
			GGroup<int>::iterator it_dia_semana
				= it_horario->dias_semana.begin();
			for (; it_dia_semana != it_horario->dias_semana.end(); it_dia_semana++)
			{
				// Dia da semana disponível
				if (*it_dia_semana == dia_semana)
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
   return (this->matriz_aulas);
}

void SolucaoOperacional::setMatrizAulas(MatrizSolucao* matriz)
{
   this->matriz_aulas = (matriz);
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

// Dado um dia da semana e um horário de aula, devo informar
// a coluna da matriz de solução correspondente a esse par de
// dia da semana/horário. A linha da matriz já é dada pelo índice
// do professor que faz a busca com esse dia e horário
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

   // Procura pelo primeiro horário
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

   // Se k = -1, então a aula do índice 'zero' é a aula que
   // procuramos, ou seja, a primeira aula do bloco de aulas
   // atual. Caso contrário, a aula procurada é a primeira à
   // direita da aula de índice 'k'. Nos dois casos, devemos
   // incrementar o índice 'k' em uma unidade.
   k++;

   // Pega a posição do horário desejado
   // dentro do bloco de aulas atual
   int posicao_aula = (j-k);

   // Recupera o horário desejado
   aula = this->getMatrizAulas()->at(i)->at(k);
   horario = aula->horarios_profs_alocados[posicao_aula].second;

   return horario;
}

Professor* SolucaoOperacional::getProfessorMatriz(int linha)
{
   Professor* professor = NULL;

   // Procura pelo professor que possua o 'id_operacioanal'
   // igual ao índice 'linha' informado, que corresponde à
   // linha do professor na matriz de solução operacional
   std::map<int, Professor*>::iterator it_professor
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

int SolucaoOperacional::getItHorariosProf(Professor & professor, int dia, int horario)
{
   if(dia < 1 || dia > 7 )
   {
      throw (out_of_range("Dias validos [1,7] -> dom. a sab."));
   }

   return ( ((dia - 1) * total_horarios) + horario );
}

int SolucaoOperacional::addProfessor(Professor & professor, vector<Aula*> & horariosProf)
{
   matriz_aulas->push_back(&horariosProf);

   int idOperacional = (matriz_aulas->size()-1);

   professor.setId(-idOperacional);
   professor.setIdOperacional(idOperacional);

   if(mapProfessores.find(professor.getIdOperacional()) != mapProfessores.end())
   {
      cerr << "ID OPERACIONAL FORNECIDO JÁ EXISTIA" << endl;
      exit(1);
   }

   mapProfessores[idOperacional] = &professor;

   return idOperacional;
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