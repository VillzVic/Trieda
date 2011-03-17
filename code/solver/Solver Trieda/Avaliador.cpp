#include <cmath>

#include "Avaliador.h"
#include "ofbase.h"
#include "Fixacao.h"

Avaliador::Avaliador()
{
	MINUTOS_POR_HORARIO = 60;

	totalViolacaoRestricaoFixacao = 0.0;
	totalViolacoesDescolamento = 0.0;
	totalTempoViolacoesDescolamento = 0.0;
	totalGapsHorariosProfessores = 0.0;
}

Avaliador::~Avaliador()
{
}

double Avaliador::avaliaSolucao(SolucaoOperacional & solucao)
{
	// Chamada dos métodos que fazem a avaliação da solução
	calculaViolacaoRestricaoFixacao(solucao);
	calculaViolacoesDescolamento(solucao);
	calculaGapsHorariosProfessores(solucao);

	double funcaoObjetivo = 0.0;

	// Contabilização do valor da solução
	funcaoObjetivo += totalViolacaoRestricaoFixacao;
	funcaoObjetivo += totalViolacoesDescolamento;
	funcaoObjetivo += totalTempoViolacoesDescolamento;
	funcaoObjetivo += totalGapsHorariosProfessores;

	return funcaoObjetivo;
}

void Avaliador::calculaViolacaoRestricaoFixacao(SolucaoOperacional & solucao)
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

			// TODO -- devo considerar 'turno' e 'horário' ?????
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

	totalViolacaoRestricaoFixacao = numViolacoes;
}

void Avaliador::calculaViolacoesDescolamento(SolucaoOperacional & solucao)
{
	double numViolacoes = 0.0;
	double tempoViolacoes = 0.0;

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

			// Para cada professor, devo percorrer a linha correspondente às suas aulas
			for (unsigned int i=0; i < solucao.getMatrizAulas()->at(id_linha_professor)->size(); i++)
			{
				aula_atual = solucao.getMatrizAulas()->at(id_linha_professor)->at(i);

				// O professor não teve aula atribuída nesse horário
				if (aula_atual == NULL)
				{
					continue;
				}

				// O índice 'i' corresponde à coluna da matriz
				indice_horario_atual = i;

				// Procura pelas unidades (anterior e atual)
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

				// Procura pelos campus (anterior e atual)
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

				// Verifica se houve violação no deslocamento viável
				if (aula_anterior != NULL)
				{
					if (aula_anterior->getDiaSemana() == aula_atual->getDiaSemana())
					{
						// Tempo de deslocamento entre uma aula e outra
						tempo_minimo = calculaTempoEntreCampusUnidades(solucao,
							campus_atual, campus_anterior, unidade_atual, unidade_anterior);

						// Tempo existente entre as aulas 'aula_anterior' e 'aula_atual'
						tempo_disponivel = (indice_horario_atual - indice_horario_anterior) * (MINUTOS_POR_HORARIO);

						// Verifica se ocorreu a violação de tempo mínimo
						// necessário para se deslocar entre campus/unidades
						if (tempo_disponivel < tempo_minimo)
						{
							numViolacoes++;
							tempoViolacoes += abs( (tempo_minimo - tempo_disponivel) * (MINUTOS_POR_HORARIO) );
						}
					}
				}

				// Atualiza os ponteiros para a próxima iteração
				campus_anterior = campus_atual;
				unidade_anterior = unidade_atual;
				aula_anterior = aula_atual;
				indice_horario_anterior = indice_horario_atual;
			}
		}
	}

	totalViolacoesDescolamento = numViolacoes;
	totalTempoViolacoesDescolamento = tempoViolacoes;
}

double Avaliador::calculaTempoEntreCampusUnidades(SolucaoOperacional& solucao,
		Campus* campus_atual, Campus* campus_anterior,
		Unidade* unidade_atual, Unidade* unidade_anterior)
{
	double distancia = 0.0;

	// As aulas são realizadas em campus diferentes
	if (campus_atual->getId() != campus_anterior->getId())
	{
		GGroup<Deslocamento*>::iterator it_tempo_campi
			= solucao.getProblemData()->tempo_campi.begin();
		for (; it_tempo_campi != solucao.getProblemData()->tempo_campi.end(); it_tempo_campi++)
		{
			if (it_tempo_campi->getOrigemId() == campus_anterior->getId()
				&& it_tempo_campi->getDestinoId() == campus_atual->getId())
			{
				distancia = it_tempo_campi->getTempo();
			}
		}
	}
	// As aulas são realizadas em unidades diferentes
	else if (unidade_atual->getId() != unidade_anterior->getId())
	{
		GGroup<Deslocamento*>::iterator it_tempo_unidade
			= solucao.getProblemData()->tempo_unidades.begin();
		for (; it_tempo_unidade != solucao.getProblemData()->tempo_unidades.end(); it_tempo_unidade++)
		{
			if (it_tempo_unidade->getOrigemId() == unidade_anterior->getId()
				&& it_tempo_unidade->getDestinoId() == unidade_atual->getId())
			{
				distancia = it_tempo_unidade->getTempo();
			}
		}
	}

	return distancia;
}

void Avaliador::calculaGapsHorariosProfessores(SolucaoOperacional & solucao)
{
	double numGaps = 0.0;

	Aula* aula_atual = NULL;
	Aula* aula_anterior = NULL;

	int indice_aula_atual = -1;
	int indice_aula_anterior = -1;

	Professor* professor = NULL;
	Horario* h1 = NULL;
	Horario* h2 = NULL;

	int dia_semana = 0;

	for (unsigned int i = 0; i < solucao.getMatrizAulas()->size(); i++)
	{
		for (unsigned int j = 0; j < solucao.getMatrizAulas()->at(i)->size(); j++)
		{
			indice_aula_atual = j;
			aula_atual = solucao.getMatrizAulas()->at(i)->at(j);

			// O professor não tem aula atribuída nesse horário
			if (aula_atual == NULL)
			{
				continue;
			}

			if ( aula_anterior != NULL
				&& aula_anterior->getDiaSemana() == aula_atual->getDiaSemana() )
			{
				// Avalia se ocorreu um gap no horário
				int gap = (indice_aula_atual - indice_aula_anterior);
				if (gap > 1)
				{
					professor = solucao.getProfessorMatriz(i);
					dia_semana = aula_atual->getDiaSemana();
					h1 = solucao.getHorario(i, indice_aula_anterior);
					h2 = solucao.getHorario(i, indice_aula_atual);

					if (horariosDisponiveisIntervalo(professor, dia_semana, h1, h2) > 0)
					{
						totalGapsHorariosProfessores++;
						gapsProfessores.at(i).push_back(gap);
					}
				}
			}

			// Atualiza os ponteiros para a próxima iteração
			indice_aula_anterior = indice_aula_atual;
			aula_anterior = aula_atual;
		}
	}

	totalGapsHorariosProfessores = numGaps;
}

int Avaliador::horariosDisponiveisIntervalo(Professor* professor, int dia_semana, Horario* h1, Horario* h2)
{
	int horariosDisponiveis = 0;

	GGroup<Horario*>::iterator it_horario
		= professor->horarios.begin();
	for (; it_horario != professor->horarios.end(); it_horario++)
	{
		// Se o horário disponível estiver dentro do intervalo de gap,
		// então encontrei mais um gap indesejado de horário para esse professor
		if ( ( it_horario->dias_semana.find(dia_semana) != it_horario->dias_semana.end() ) &&
			 ( h1->horario_aula->getInicio() < it_horario->horario_aula->getInicio() ) &&
			 ( h2->horario_aula->getInicio() > it_horario->horario_aula->getInicio() ) )
		{
			horariosDisponiveis++;
		}
	}

	return horariosDisponiveis;
}