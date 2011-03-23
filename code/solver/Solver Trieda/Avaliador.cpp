#include <cmath>
#include <map>

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
	totalAvaliacaoCorpoDocente = 0.0;
	totalCustoCorpoDocente = 0.0;
	totalViolacoesCHMinimaSemestreAterior = 0;
	totalViolacoesCHMinimaProfessor = 0;
	totalViolacoesCHMaximaProfessor = 0;
	totalDiasProfessorMinistraAula = 0;
	totalViolacoesUltimaPrimeiraAula = 0;
	totalViolacoesMestres = 0;
	totalViolacoesDoutores = 0;
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
	avaliacaoCustoCorpoDocente(solucao);
	violacoesCargasHorarias(solucao);
	avaliaDiasProfessorMinistraAula(solucao);
	violacaoUltimaPrimeiraAula(solucao);
	avaliaNumeroMestresDoutores(solucao);

	double funcaoObjetivo = 0.0;

	// Contabilização do valor da solução
	// Obs.: atribuir PESOS aos valores
	funcaoObjetivo += totalViolacaoRestricaoFixacao;
	funcaoObjetivo += totalViolacoesDescolamento;
	funcaoObjetivo += totalTempoViolacoesDescolamento;
	funcaoObjetivo += totalGapsHorariosProfessores;
	funcaoObjetivo += totalAvaliacaoCorpoDocente;
	funcaoObjetivo += totalCustoCorpoDocente;
	funcaoObjetivo += totalViolacoesCHMinimaSemestreAterior;
	funcaoObjetivo += totalViolacoesCHMinimaProfessor;
	funcaoObjetivo += totalViolacoesCHMaximaProfessor;
	funcaoObjetivo += totalDiasProfessorMinistraAula;
	funcaoObjetivo += totalViolacoesUltimaPrimeiraAula;
	funcaoObjetivo += totalViolacoesMestres;
	funcaoObjetivo += totalViolacoesDoutores;

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
					= solucao.getProblemData()->refUnidade.begin();
				for (; it_unidade != solucao.getProblemData()->refUnidade.end(); it_unidade++)
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
					= solucao.getProblemData()->refCampus.begin();
				for (; it_campus != solucao.getProblemData()->refCampus.end(); it_campus++)
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

	// Percorre as aulas alocadas a cada professor
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

					// Dado que ocorreu um gap entre duas aulas, devo
					// verificar se o professor possui horários disponíveis
					// no período entre a aula anterior e a aula atual, o que
					// caracterizaria um fato 'indesejado' na solução operacional
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


void Avaliador::avaliacaoCustoCorpoDocente(SolucaoOperacional & solucao)
{
	int avaliacaoCorpoDocente = 0;
	double custoCorpoDocente = 0.0;

	std::map<int, Professor*>::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end(); it_professor++)
	{
		ITERA_GGROUP(it_mag, it_professor->second->magisterio, Magisterio)
		{
			avaliacaoCorpoDocente += it_mag->getNota();
		}

		custoCorpoDocente += it_professor->second->getValorCredito();
	}

	totalAvaliacaoCorpoDocente = avaliacaoCorpoDocente;
	totalCustoCorpoDocente = custoCorpoDocente;
}

void Avaliador::violacoesCargasHorarias(SolucaoOperacional & solucao)
{
	// Armazena o total de violações de cada tipo
	int violacoesSemestreAnterior = 0;
	int violacoesCHMinima = 0;
	int violacoesCHMaxima = 0;

	int tempSemestreAnterior = 0;
	int tempCHMinima = 0;
	int tempCHMaxima = 0;

	Aula* aula = NULL;
	int contCreditos, linha_professor;

	std::map<int, Professor*>::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end(); it_professor++)
	{
		contCreditos = 0;
		linha_professor = it_professor->second->getIdOperacional();

		// Verifica quantos créditos o professor ministrará na semana
		for (unsigned i = 0; i < solucao.getMatrizAulas()->at(linha_professor)->size(); i++)
		{
			aula = solucao.getMatrizAulas()->at(linha_professor)->at(i);
			if (aula != NULL)
			{
				contCreditos++;
			}
		}

		// Verifica carga horária do semestre anterior
		tempSemestreAnterior = ( contCreditos - it_professor->second->getChAnterior() );
		if (tempSemestreAnterior < 0)
		{
			violacoesSemestreAnterior++;
		}

		// Verifica carga horária mínima do professor
		tempCHMinima = ( contCreditos - it_professor->second->getChMin() );
		if (tempCHMinima < 0)
		{
			violacoesCHMinima++;
		}

		// Verifica carga horária máxima do professor
		tempCHMaxima = ( contCreditos - it_professor->second->getChMax() );
		if (tempCHMaxima > 0)
		{
			violacoesCHMaxima++;
		}

		// Informa quantas violações de cada tipo ocorreram com o professor
		violacoesCHMinimaSemestreAterior[linha_professor] = abs(tempSemestreAnterior);
		violacoesCHMinimaProfessor[linha_professor] = abs(tempCHMaxima);
		violacoesCHMaximaProfessor[linha_professor] = abs(tempCHMaxima);
	}

	// Informa o total de violações de cada tipo foram verificadas
	totalViolacoesCHMinimaSemestreAterior = violacoesSemestreAnterior;
	totalViolacoesCHMinimaProfessor = violacoesCHMinima;
	totalViolacoesCHMaximaProfessor = violacoesCHMaxima;
}

void Avaliador::avaliaDiasProfessorMinistraAula(SolucaoOperacional & solucao)
{
	int numDias = 0;

	Aula* aula = NULL;
	int linha_professor = 0;

	std::map<int, Professor*>::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end(); it_professor++)
	{
		linha_professor = it_professor->second->getIdOperacional();

		GGroup<int> dias_semana;
		for (unsigned i = 0; i < solucao.getMatrizAulas()->at(linha_professor)->size(); i++)
		{
			aula = solucao.getMatrizAulas()->at(linha_professor)->at(i);
			dias_semana.add(aula->getDiaSemana());
		}

		professorMinistraAula[linha_professor] = dias_semana.size();
		numDias += dias_semana.size();
	}

	totalDiasProfessorMinistraAula = numDias;
}

void Avaliador::violacaoUltimaPrimeiraAula(SolucaoOperacional & solucao)
{
	int numViolacoes = 0;
	int violacoesProfessor = 0;

	int linha_professor = 0;

	// Informa o número de aulas que
	// cada dia da semana poderá possuir
	int bloco_aula = calculaTamanhoBlocoAula(solucao);

	// Última aula do dia D
	Aula* aula1 = NULL;

	// Primeira aula do dia D+1
	Aula* aula2 = NULL;

	std::map<int, Professor*>::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end(); it_professor++)
	{
		violacoesProfessor = 0;
		linha_professor = it_professor->second->getIdOperacional();

		// Verifica as aulas do professor, procurando popr violações
		// do tipo "última aula do dia D e primeira aula do dia D+1"
		for (unsigned i = bloco_aula-1;
			 i < solucao.getMatrizAulas()->at(linha_professor)->size();
			 i += bloco_aula)
		{
			aula1 = solucao.getMatrizAulas()->at(linha_professor)->at(i);
			aula2 = solucao.getMatrizAulas()->at(linha_professor)->at(i+1);

			// Verifica se os dois horários foram alocados
			if (aula1 != NULL && aula2 != NULL)
			{
				violacoesProfessor++;
			}
		}

		// Armazena as violações do professor individualmente e
		// incrementa o total de violações encontrados na solução
		violacoesUltimaPrimeiraAulaProfessor[linha_professor] = violacoesProfessor;
		numViolacoes += violacoesProfessor;
	}

	totalViolacoesUltimaPrimeiraAula = numViolacoes;
}

int Avaliador::calculaTamanhoBlocoAula(SolucaoOperacional & solucao)
{
	int bloco_aula = 0;

	unsigned i = 0;
	vector<int> cont_horarios;

	// Esse vetor contém uma posição para
	// cada dia da semana, iniciando o contador
	// de horários de cada dia em zero horários
	cont_horarios.clear();
	for (i = 0; i < 8; i++)
	{
		cont_horarios.push_back(0);
	}

	// Para cada um dos horários disponíveis, essa iteração
	// incrementa o número de horários disponíveis em cada dia da
	// semana, a cada ocorrência de horário/dia da semana encontrada
	ITERA_GGROUP(it_campi, solucao.getProblemData()->campi, Campus)
	{
		ITERA_GGROUP(it_horario, it_campi->horarios, Horario)
		{
			GGroup<int>::iterator it_dia_semana
				= it_horario->dias_semana.begin();
			for (; it_dia_semana != it_horario->dias_semana.end(); it_dia_semana++)
			{
				cont_horarios[*it_dia_semana]++;
			}
		}
	}

	// Procura pelo dia da semana com mais horários
	for (i = 0; i < cont_horarios.size(); i++)
	{
		if (cont_horarios[i] > bloco_aula)
		{
			bloco_aula = cont_horarios[i];
		}
	}

	return bloco_aula;
}

void Avaliador::avaliaNumeroMestresDoutores(SolucaoOperacional & solucao)
{
	int violacoesMestres = 0,
		violacoesDoutores = 0;

	// TODO -- preencher índices corretamente
	int id_titulacao_mestre = 4;
	int id_titulacao_doutor = 5;

	int id_curso = 0;
	int id_titulacao = 0;

	Aula* aula = NULL;
	Professor* professor = NULL;

	// Relaciona um curso com o conjunto de professores
	// que leciona pelo menos uma disciplina desse curso
	std::map<int/*curso*/, GGroup<Professor*>/*Professores relacionados ao curso*/ > mapCursosProfessores;

	// Associa as turmas de cada disciplina à seu professor correspondente
	for (unsigned int i = 0; i < solucao.getMatrizAulas()->size(); i++)
	{
		// Professor da iteração atual
		professor = solucao.getProfessorMatriz(i);

		for (unsigned int j = 0; j < solucao.getMatrizAulas()->at(i)->size(); j++)
		{
			aula = solucao.getMatrizAulas()->at(i)->at(j);
			if (aula == NULL)
			{
				continue;
			}

			// Adiciona o professor na lista de professores
			// do curso ao qual a aula atual atende
			ITERA_GGROUP(it_oferta, solucao.getProblemData()->ofertas, Oferta)
			{
				// Procura pela oferta da aula atual
				if (it_oferta->getId() == aula->getOfertacursoCampusId())
				{
					id_curso = it_oferta->getCursoId();
					mapCursosProfessores[ id_curso ].add(professor);

					break;
				}
			}
			//////////////
		}
		//////////////
	}
	//////////////

	int num_professores_curso = 0;
	int cont_mestres = 0;
	int cont_doutores = 0;
	double percentual_mestres = 0.0;
	double percentual_doutores = 0.0;

	// Para cada curso, devo verificar a titulação de
	// seus professores, verificando se a solução atende
	// ao percentual mínimo exigido de mestres e doutores
	std::map<int, GGroup<Professor*> >::iterator it_cursos_professores
		= mapCursosProfessores.begin();
	for (; it_cursos_professores != mapCursosProfessores.end(); it_cursos_professores++)
	{
		// Id do curso atual
		id_curso = it_cursos_professores->first;

		// Inicia o número de mestres e doutores com zero
		cont_mestres = 0;
		cont_doutores = 0;

		// Percorre a lista de professores que
		// ministram pelo menos uma aula do curso atual
		GGroup<Professor*> professores = it_cursos_professores->second;

		// Total de professores que ministram aulas desse curso
		num_professores_curso = professores.size();

		ITERA_GGROUP(it_professor, professores, Professor)
		{
			// Se for doutor, incrementa-se o contador de mestres
			// e também o contador de doutores associados ao curso
			if (it_professor->titulacao->getId() == id_titulacao_doutor)
			{
				cont_mestres++;
				cont_doutores++;
			}
			// Se for mestre, incrementa-se apenas
			// o contador de mestres associados ao curso atual
			else if (it_professor->titulacao->getId() == id_titulacao_mestre)
			{
				cont_mestres++;
			}
		}

		// Procura pelo curso atual na lista de cursos do 'problemaData'
		Curso* curso = NULL;
		ITERA_GGROUP(it_curso, solucao.getProblemData()->cursos, Curso)
		{
			if (it_curso->getId() == id_curso)
			{
				curso = *(it_curso);
				break;
			}
		}

		// Recupera os dados de porcentagem mínima exigida
		// 'first'  -> código do tipo da titulação
		// 'second' -> percentual mínimo de professores com essa titulação
		percentual_mestres = curso->regra_min_mestres.second;
		percentual_doutores = curso->regra_min_doutores.second;

		// Verifica se o número de mestres e doutores
		// da solução atende ao número mínimo exigido
		if (cont_mestres / (double)num_professores_curso < percentual_mestres)
		{
			violacoesMestres++;
		}
		if (cont_doutores / (double)num_professores_curso < percentual_doutores)
		{
			violacoesDoutores++;
		}
	}

	totalViolacoesMestres = violacoesMestres;
	totalViolacoesDoutores = violacoesDoutores;
}