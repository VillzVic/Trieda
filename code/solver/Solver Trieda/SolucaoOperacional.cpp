#include "ofbase.h"
#include "GGroup.h"
#include "SolucaoOperacional.h"

#include "Aula.h"

SolucaoOperacional::SolucaoOperacional(ProblemData* prbDt)
{
   this->problemData = prbDt;

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
		}
   }
}

SolucaoOperacional::~SolucaoOperacional()
{
}

void SolucaoOperacional::carregaSolucaoInicial()
{
	// TODO -- Carregar a solução inicial, antes de avaliar
}

MatrizSolucao* SolucaoOperacional::getMatrizAulas() const
{
	return this->matrizAulas;
}

void SolucaoOperacional::setMatrizAulas(MatrizSolucao* m)
{
	this->matrizAulas = m;
}

// Imprime as aulas da matriz de solução,
// percorrendo as linhas de cada professor
std::string SolucaoOperacional::toString() const
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
		}

		std::cout << std::endl << "-------" << std::endl;
	}

	return "";
}

// Dado um dia da semana e um horário de aula, devo informar
// a coluna da matriz de solução correspondente a esse par de
// dia da semana/horário. A linha da matriz já é dada pelo índice
// do professor que faz a busca com esse dia e horário
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

	// Procura pelo primeiro horário
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

	// Garante que o índice 'k' estará apontando
	// para a primeira aula do bloc de aula atual
	if (aula == NULL)
	{
		k++;
	}

	// Pega a posição do horário desejado
	// dentro do bloco de aulas atual
	int posicao_aula = (j-k);

	// Recupera o horário desejado
	aula = this->getMatrizAulas()->at(i)->at(k);
	horario = aula->alocacao_aula[k].getHorario();

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

bool SolucaoOperacional::alocaAula(Professor & professor, int dia, Horario & horario, Aula & aula)
{
   // TODO
   std::cout << "Implementar o metodo bool SolucaoOperacional::alocaAula("
			 << "Professor & professor, int dia, Horario & horario, Aula & aula)" << std::endl;
   exit(1);

   return false;
}