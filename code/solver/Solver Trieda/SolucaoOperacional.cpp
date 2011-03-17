#include "ofbase.h"
#include "GGroup.h"
#include "SolucaoOperacional.h"

SolucaoOperacional::SolucaoOperacional(ProblemData* prbDt)
{
   this->problemData = prbDt;

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
		}
   }

   // Map de unidades: dado o id da unidade,
   // retorna o ponteiro para a respectiva unidade
   mapUnidades = &( prbDt->refUnidade );	

   // Map de campus: dado o id do campus,
   // retorna o ponteiro para o respectivo campus
   mapCampus = &( prbDt->refCampus );	
}

SolucaoOperacional::~SolucaoOperacional()
{
}

void SolucaoOperacional::carregaSolucaoInicial()
{
	// TODO -- Carregar a solu��o inicial, antes de avaliar
}

MatrizSolucao* SolucaoOperacional::getMatrizAulas() const
{
	return this->matrizAulas;
}

void SolucaoOperacional::setMatrizAulas(MatrizSolucao* m)
{
	this->matrizAulas = m;
}

std::string SolucaoOperacional::toString() const
{
	for (int i=0; i < (int)this->getMatrizAulas()->size(); i++)
	{
		std::vector<Aula*>* aulas = this->getMatrizAulas()->at(i);
		for (int j=0; j < (int)aulas->size(); j++)
		{
			Aula* aula = aulas->at(i);
			if (aula != NULL)
			{
				std::cout << "Turma : " << aula->getTurma() << std::endl;
				std::cout << "Disciplina : " << aula->getDisciplina() << std::endl;
				std::cout << "Sala : " << aula->getSala() << std::endl << std::endl;
			}
		}

		std::cout << "-------" << std::endl;
	}

	return "";
}

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
	Horario* horario = NULL;

	// TODO

	return horario;
}

Professor* SolucaoOperacional::getProfessorMatriz(int linha)
{
	Professor* professor = NULL;

	// TODO

	return professor;
}

ProblemData* SolucaoOperacional::getProblemData() const
{
	return problemData;
}
