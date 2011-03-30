#include "ofbase.h"
#include "GGroup.h"
#include "SolucaoOperacional.h"

#include "Aula.h"

SolucaoOperacional::SolucaoOperacional(ProblemData* prbDt)
{
   // FIXAR OS VALORES: 6 (dias) 4 (horarios)
   totalDias = 6;
   totalHorarios = 4;
   totalDeProfessores = 0;

   this->problemData = (prbDt);

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
			++totalDeProfessores;
		}
   }

   // Inicializando a estrutura <matrizAulas>
   matrizAulas = new MatrizSolucao (totalDeProfessores);
   MatrizSolucao::iterator itMatrizAulas = matrizAulas->begin();
   for(; itMatrizAulas != matrizAulas->end(); ++itMatrizAulas)
   {
      *itMatrizAulas = new vector<Aula*> ((totalDias*totalHorarios), NULL);
   }

   // Deixando livres, apenas os horarios em que o professor pode
   // ministrar aulas. Para os demais, associa-as � uma aula virtual.

   Aula * aulaVirtual = new Aula(true);

   /* TODO (Cleiton) : PREENCHER COM AULAS VIRTUAIS, OS HORARIOS EM QUE UM DADO PROFESSOR, NAO 
   ESTA DISPONIVEL. USAR SEMPRE O MESMO OBJETO DE AULA VIRTUAL. */
}

SolucaoOperacional::~SolucaoOperacional()
{
   // Limpa as refer�ncias do map de professores
   this->mapProfessores.clear();

   // Limpa a refer�ncia do objeto 'problemData'
   this->problemData = NULL;

   // Limpa as refer�ncias da matriz de solu��o operacional
   MatrizSolucao::iterator itMatrizAulas = matrizAulas->begin();
   for(; itMatrizAulas != matrizAulas->end(); ++itMatrizAulas)
   {
	  delete *itMatrizAulas;
   }
   matrizAulas = NULL;
}

void SolucaoOperacional::carregaSolucaoInicial()
{
	// TODO -- Carregar a solu��o inicial, antes de avaliar
}

MatrizSolucao* SolucaoOperacional::getMatrizAulas() const
{
	return (this->matrizAulas);
}

void SolucaoOperacional::setMatrizAulas(MatrizSolucao* matriz)
{
	this->matrizAulas = (matriz);
}

// Imprime as aulas da matriz de solu��o,
// percorrendo as linhas de cada professor
void SolucaoOperacional::toString() const
{
   for (unsigned int i = 0; i < this->getMatrizAulas()->size(); i++)
   {
      std::vector<Aula*>* aulas = this->getMatrizAulas()->at(i);
      for (unsigned int j = 0; j < aulas->size(); j++)
      {
         Aula* aula = aulas->at(j);
         if (aula != NULL)
         {
            std::cout << "Turma : " << aula->getTurma() << " -- ";
            std::cout << "Disciplina : " << aula->getDisciplina() << " -- ";
            std::cout << "Sala : " << aula->getSala() << std::endl << std::endl;
         }
         else
		 {
            cout << "NULL" << endl;
		 }
      }

      std::cout << std::endl << "-------" << std::endl;
   }
}

// Dado um dia da semana e um hor�rio de aula, devo informar
// a coluna da matriz de solu��o correspondente a esse par de
// dia da semana/hor�rio. A linha da matriz j� � dada pelo �ndice
// do professor que faz a busca com esse dia e hor�rio
int SolucaoOperacional::getIndiceMatriz(int dia, Horario* horario)
{
	int indice_matriz = 0;

	int dia_semana = ((dia-2) * problemData->max_horarios_professor);
	int horario_dia = (horario->getHorarioAulaId()-1);
	indice_matriz = (dia_semana + horario_dia);

	return indice_matriz;
}

Horario* SolucaoOperacional::getHorario(int i, int j)
{
	Aula* aula = NULL;
	Horario* horario = NULL;

	// Procura pelo primeiro hor�rio
	// do bloc de aula correspondente
	int k = (j - 1);
	while( k >= 0 )
	{
		// Recupera a aula atual
		aula = this->getMatrizAulas()->at(i)->at(k);
		if ( aula == NULL )
		{
			break;
		}

		k--;
	}

	// Garante que o �ndice 'k' estar� apontando
	// para a primeira aula do bloc de aula atual
	if (aula == NULL)
	{
		k++;
	}

	// Pega a posi��o do hor�rio desejado
	// dentro do bloco de aulas atual
	int posicao_aula = (j-k);

	// Recupera o hor�rio desejado
	aula = this->getMatrizAulas()->at(i)->at(k);
	horario = aula->alocacao_aula[k].getHorario();

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
	for (; it_professor != mapProfessores.end(); it_professor)
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
	return problemData;
}

vector<Aula*>::iterator SolucaoOperacional::getHorariosDia(Professor & professor, int dia)
{
   vector<Aula*>::iterator itHorarios = matrizAulas->at(professor.getIdOperacional())->begin();

   // Ajustando para o primeiro hor�rio do dia em quest�o.
   //itHorarios += ((dia - 1) * totalHorarios) + 1;
   itHorarios += ((dia - 2) * totalHorarios) + 1;

   return itHorarios;
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
   return totalHorarios;
}

//bool SolucaoOperacional::alocaAula(Professor & professor, int dia, Horario & horario, Aula & aula)
//bool SolucaoOperacional::alocaAula(Professor & professor, Aula & aula, vector<Aula*> & horarios)
//{
//   // TODO
//
//   vector<Aula*>::iterator itHorarios = horarios.begin();
//
//   for(; itHorarios != horarios.end(); ++itHorarios)
//      *itHorarios = &aula;
//
//   std::cout << "Implementar o metodo bool SolucaoOperacional::alocaAula("
//			 << "Professor & professor, ... )" << std::endl;
//   exit(1);
//
//   return true;
//}
