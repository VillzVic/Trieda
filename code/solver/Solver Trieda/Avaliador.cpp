#include "Avaliador.h"
#include "ofbase.h"
#include "Fixacao.h"

Avaliador::Avaliador()
{
	MINUTOS_POR_HORARIO = 60;
}

Avaliador::~Avaliador()
{
}

double Avaliador::avaliaSolucao(SolucaoOperacional & solucao)
{
	double funcaoObjetivo = 0.0;

	funcaoObjetivo += violacaoRestricaoFixacao(solucao);
	funcaoObjetivo += violacaoTempoDescolamentoViavel(solucao);

	return funcaoObjetivo;
}

// M�todo que verifica quantas fixa��o de professores
// n�o foram atendidas pela solu��o do modelo operacional
double Avaliador::violacaoRestricaoFixacao(SolucaoOperacional & solucao)
{
	double numViolacoes = 0.0;
	bool encontrou_fixacao = false;

	ITERA_GGROUP(it_fixacao, solucao.getProblemData()->fixacoes, Fixacao)
	{
	 	int id_professor = it_fixacao->getProfessorId();
		Professor* professor = solucao.mapProfessores[id_professor];
		int id_linha_professor = professor->getIdOperacional();

		for (unsigned int i=0; i < solucao.getMatrizAulas()->at(id_linha_professor)->size(); i++)
		{
			Aula* aula = solucao.getMatrizAulas()->at(id_linha_professor)->at(i);

			// TODO -- devo considerar 'turno' e 'hor�rio' ?????
			if ( (it_fixacao->getDiaSemana() == aula->getDiaSemana()) &&
				(it_fixacao->getDisciplinaId() == aula->getDisciplina()->getId()) &&
				(it_fixacao->getSalaId() == aula->getSala()->getId()) )
			{
				encontrou_fixacao = true;
				break;
			}
		}

		if (!encontrou_fixacao)
		{
			numViolacoes++;
		}
	}

	return numViolacoes;
}

// M�todo que conta 'QUANTAS VEZES' foi atribu�da
// algum professor duas aulas no mesmo dia que violassem
// o tempo de deslocamento entre campus e/ou unidades
double Avaliador::violacaoTempoDescolamentoViavel(SolucaoOperacional & solucao)
{
	double numViolacoes = 0.0;

	Unidade* unidade_atual = NULL;
	Unidade* unidade_anterior = NULL;
	Campus* campus_atual = NULL;
	Campus* campus_anterior = NULL;
	Aula* aula_atual = NULL;
	Aula* aula_anterior = NULL;

	int id_unidade_atual = -1;
	int id_unidade_anterior = -1;
	int id_campus_atual = -1;
	int id_campus_anterior = -1;

	double tempo_minimo = 0.0;
	double tempo_disponivel = 0.0;
	int indice_horario_atual = -1;
	int indice_horario_anterior = -1;

	ITERA_GGROUP(it_campi, solucao.getProblemData()->campi, Campus)
	{
		ITERA_GGROUP(it_professor, it_campi->professores, Professor)
		{
			int id_linha_professor = it_professor->getIdOperacional();

			for (unsigned int i=0; i < solucao.getMatrizAulas()->at(id_linha_professor)->size(); i++)
			{
				aula_atual = solucao.getMatrizAulas()->at(id_linha_professor)->at(i);

				// O professor n�o teve aula atribu�da nesse hor�rio
				if (aula_atual == NULL)
				{
					continue;
				}

				indice_horario_atual = i;

				// Procura pelas unidades
				id_unidade_atual = aula_atual->getSala()->getIdUnidade();
				if (unidade_anterior != NULL)
				{
					id_unidade_anterior = unidade_anterior->getId();
				}

				std::map<int, Unidade*>::iterator it_unidade
					= solucao.mapUnidades->begin();
				for (; it_unidade != solucao.mapUnidades->end(); it_unidade++)
				{
					if (it_unidade->first == id_unidade_atual)
					{
						unidade_atual = it_unidade->second;
					}
					else if (it_unidade->first == id_unidade_anterior)
					{
						unidade_anterior = it_unidade->second;
					}
				}
				/////

				// Procura pelos campus
				id_campus_atual = unidade_atual->id_campus;
				if (campus_anterior != NULL)
				{
					id_campus_anterior = campus_anterior->getId();
				}

				std::map<int, Campus*>::iterator it_campus
					= solucao.mapCampus->begin();
				for (; it_campus != solucao.mapCampus->end(); it_campus++)
				{
					if (it_campus->first == id_campus_atual)
					{
						campus_atual = it_campus->second;
					}
					else if (it_campus->first == id_campus_anterior)
					{
						campus_anterior = it_campus->second;
					}
				}
				/////

				// Verifica se houve viola��o no deslocamento vi�vel
				if (aula_anterior != NULL)
				{
					if (aula_anterior->getDiaSemana() == aula_atual->getDiaSemana())
					{
						// Tempo de deslocamento entre uma aula e outra
						tempo_minimo = calculaTempoEntreCampusUnidades(
							campus_atual, campus_anterior, unidade_atual, unidade_anterior);

						// Tempo existente entre as aulas 'aula_anterior' e 'aula_atual'
						tempo_disponivel = (indice_horario_atual - indice_horario_anterior) * (MINUTOS_POR_HORARIO);

						// Verifica se ocorreu a viola��o de tempo m�nimo
						// necess�rio para se deslocar entre campus/unidades
						if (tempo_disponivel < tempo_minimo)
						{
							numViolacoes++;
						}
					}
				}

				// Atualiza os ponteiros para a pr�xima itera��o
				campus_anterior = campus_atual;
				unidade_anterior = unidade_atual;
				aula_anterior = aula_atual;
				indice_horario_anterior = indice_horario_atual;
			}
		}
	}

	return numViolacoes;
}

double Avaliador::calculaTempoEntreCampusUnidades(
		Campus* campus_atual, Campus* campus_anterior,
		Unidade* unidade_atual, Unidade* unidade_anterior)
{
	double distancia = 0.0;

	if (campus_atual->getId() != campus_anterior->getId())
	{
		// TODO
	}
	else if (unidade_atual->getId() != unidade_anterior->getId())
	{
		// TODO
	}

	return distancia;
}