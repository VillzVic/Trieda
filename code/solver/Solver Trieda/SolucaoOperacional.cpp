#include "ofbase.h"
#include "GGroup.h"
#include "SolucaoOperacional.h"

#include "Aula.h"
#include "AlocacaoAula.h"

SolucaoOperacional::SolucaoOperacional(ProblemData* prbDt)
{
   // FIXAR OS VALORES: 6 (dias) 4 (horarios)
   total_dias = 6;
   total_horarios = 4;
   total_professores = 0;

   this->problem_data = (prbDt);

   // Montando um map: dado o �ndice da matriz (o 'idOperacional'
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

   /*
   DEBUGAR ESSE TRECHO DO CODIGO COM O CLEITON DEPOIS.
   Acho que est� dando alguma coisa errada.
   Esse trecho de cod. diz respeito � aloca��o pr�via de aulas virtuais.
   */

   unsigned int i = 0;
   unsigned int j = 0;
   int dia_semana = 0;
   int horario_aula_id = 0;

   Aula * aula_virtual = new Aula(true);
   Professor* professor = NULL;

   // Deixando livres, apenas os horarios em que o professor pode
   // ministrar aulas. Para os demais, associa-as � uma aula virtual.
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
		   // Dia da semana
		   dia_semana = ( j / total_horarios );

		   // �ndice do hor�rio da aula
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
   // Limpa as refer�ncias do map de professores
   this->mapProfessores.clear();

   // Limpa a refer�ncia do objeto 'problemData'
   this->problem_data = NULL;

   // Limpa as refer�ncias da matriz de solu��o operacional
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
   int dia_semana  = ((dia-2) * (this->getProblemData()->max_horarios_professor));
   int horario_dia = (horario->getHorarioAulaId() - 1);

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
   horario = aula->alocacao_aula[posicao_aula].getHorario();

   return horario;
}

Professor* SolucaoOperacional::getProfessorMatriz(int linha)
{
   Professor* professor = NULL;

   // Procura pelo professor que possua o 'id_operacioanal'
   // igual ao �ndice 'linha' informado, que corresponde �
   // linha do professor na matriz de solu��o operacional
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

vector<Aula*>::iterator SolucaoOperacional::getHorariosDia(Professor & professor, int dia)
{
   vector<Aula*>::iterator itHorarios
	   = this->getMatrizAulas()->at(professor.getIdOperacional())->begin();

   // Ajustando para o primeiro hor�rio do dia em quest�o.
   itHorarios += ((dia - 2) * total_horarios);

   return itHorarios;
}

int SolucaoOperacional::getHorariosProfDia(Professor & professor, int dia)
{
   return ((dia - 2) * total_horarios);
}

//vector<Aula*>::iterator SolucaoOperacional::getHorariosDia(vector<Aula*> & horariosProfessor, int dia)
//{
//   vector<Aula*>::iterator itHorarios = horariosProfessor.begin();
//
//   // Ajustando para o primeiro hor�rio do dia em quest�o.
//   itHorarios += ((dia - 1) * totalHorarios) + 1;
//   
//   return itHorarios;
//}

//vector<Aula*> & SolucaoOperacional::getHorarios(Professor & professor)
//{
//   return *matrizAulas->at(professor.getIdOperacional());
//}

int SolucaoOperacional::getTotalHorarios() const
{
   return total_horarios;
}

//bool SolucaoOperacional::alocaAula(Professor * professor,
//	  							     Aula * aula, int dia, Horario * horario)
//{
//   int linha_matriz  = ( professor->getIdOperacional() );
//   int coluna_matriz = ( this->getIndiceMatriz(dia, horario) );
//
//   int creditos_teoricos = aula->getCreditosTeoricos();
//   int creditos_praticos = aula->getCreditosPraticos();
//   int total_creditos = ( creditos_teoricos + creditos_praticos );
//
//   AlocacaoAula * alocacao_aula = new AlocacaoAula();
//   alocacao_aula->setDiaSemana(dia);
//   alocacao_aula->setHorario(horario);
//   alocacao_aula->setProfessor(professor);
//   aula->alocacao_aula.push_back(*alocacao_aula);
//
//   vector< Aula * >::iterator it_aula
//	   = this->getMatrizAulas()->at(linha_matriz)->begin();
//
//   it_aula += coluna_matriz;
//   for (int i = 0; i < total_creditos; i++)
//   {
//	   it_aula += i;
//	   *it_aula = aula;
//   }
//
//   return true;
//}

int SolucaoOperacional::getTotalDeProfessores() const
{
	return total_professores;
}

int SolucaoOperacional::getTotalDias() const
{
	return total_dias;
}