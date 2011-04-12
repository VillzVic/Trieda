#include <cmath>
#include <vector>
#include <map>

#include "Avaliador.h"
#include "ofbase.h"
#include "Fixacao.h"
#include "DateTime.h"

Avaliador::Avaliador()
{
	MINUTOS_POR_HORARIO = 50;

	// Inicializa o total de viola��es
	totalViolacaoRestricaoFixacao = 0.0;
	totalViolacoesDescolamento = 0;
	totalTempoDescolamento = 0.0;
	totalViolacoesDeslocamentoProfessor = 0;
	totalDeslocamentosProfessor = 0;
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
	totalViolacoesDiscProfCurso = 0;
	totalPreferenciasProfessorDisciplina = 0;
	totalProfessoresVirtuais = 0;
	totalCreditosProfessoresVirtuais = 0;

	// Atribui o peso de cada crit�rio
	// na nota de avalia��o da solu��o
	PESO_FIXACAO = 1;
	PESO_DESLOCAMENTO = 1;
	PESO_TEMPO_DESLOCAMENTO = 1;
    PESO_VIOLACAO_DESLOCAMENTO_PROFESSOR = 1;
    PESO_DESLOCAMENTO_PROFESSOR = 1;
	PESO_GAPS_HORARIO = 1;
	PESO_NOTA_CORPO_DOCENTE = 1;
	PESO_CUSTO_CORPO_DOCENTE = 1;
	PESO_CH_MINIMA_ANTERIOR = 1;
	PESO_CH_MINIMA_PROFESSOR = 1;
	PESO_CH_MAXIMA_PROFESSOR = 1;
	PESO_TOTAL_DIAS_SEMANA = 1;
	PESO_ULTIMA_E_PRIMEIRA_AULA = 1;
	PESO_PERCENTUAL_MESTRES = 1;
	PESO_PERCENTUAL_DOUTORES = 1;
	PESO_DISCIPLINAS_PROFESSOR_CURSO = 1;
	PESO_PREFERENCIA_DISCIPLINA = 1;
	PESO_NUMERO_PROFESSORES_VIRTUAIS = 1;
	PESO_CREDITOS_PROFESSORES_VIRTUAIS = 1;
}

Avaliador::~Avaliador()
{

}

void Avaliador::imprimeResultados()
{
	std::cout << std::endl << "Violacoes de restricao de fixacao : "
			  << totalViolacaoRestricaoFixacao << std::endl;

	std::cout << std::endl << "Violacoes de deslocamento de professor : "
			  << totalViolacoesDeslocamentoProfessor << std::endl;

	std::cout << std::endl << "Tempo de deslocamento de professor : "
			  << totalDeslocamentosProfessor << std::endl;

	std::cout << std::endl << "Violacoes de deslocamento de alunos : "
			  << totalViolacoesDescolamento << std::endl;

	std::cout << std::endl << "Tempo de deslocamento de alunos : "
			  << totalTempoDescolamento << std::endl;

	std::cout << std::endl << "Total de gaps nos horarios dos professores : "
			  << totalGapsHorariosProfessores << std::endl;

	std::cout << std::endl << "Avaliacao do corpo docente : "
			  << totalAvaliacaoCorpoDocente << std::endl;

	std::cout << std::endl << "Custo do corpo docente : "
			  << totalCustoCorpoDocente << std::endl;

	std::cout << std::endl << "Violacoes de carga horaria minima do semestre anterior (sindicato) : "
			  << totalViolacoesCHMinimaSemestreAterior << std::endl;

	std::cout << std::endl << "Violacoes de carga horaria minima dos professores : "
			  << totalViolacoesCHMinimaProfessor << std::endl;

	std::cout << std::endl << "Violacoes de carga horaria maxima dos professores : "
			  << totalViolacoesCHMaximaProfessor << std::endl;

	std::cout << std::endl << "Dias em que os professores ministram aulas : "
			  << totalDiasProfessorMinistraAula << std::endl;

	std::cout << std::endl << "Violacoes do tipo ultima aula dia D / primeira aula dia D+1 : "
			  << totalViolacoesUltimaPrimeiraAula << std::endl;

	std::cout << std::endl << "Violacoes de percentual minimo de mestres : "
			  << totalViolacoesMestres << std::endl;

	std::cout << std::endl << "Violacoes de percentual minimo de doutores : "
			  << totalViolacoesDoutores << std::endl;

	std::cout << std::endl << "Violacoes de numero de disciplinas por professor por curso : "
			  << totalViolacoesDiscProfCurso << std::endl;

	std::cout << std::endl << "Avaliacao de preferencia de professor pelas disciplinas : "
			  << totalPreferenciasProfessorDisciplina << std::endl;

	std::cout << std::endl << "Total de professores virtuais : "
			  << totalProfessoresVirtuais << std::endl << std::endl;
}

double Avaliador::avaliaSolucao(SolucaoOperacional & solucao, bool imprime_resultados)
{
	// Para testar as avalia��es de deslocamento,
	// precisa-se de uma inst�ncia multi-campi

    // Chamada dos m�todos que fazem a avalia��o da solu��o
	calculaViolacaoRestricaoFixacao(solucao); // finalizado
	calculaDescolamentoProfessor(solucao); // pendente
	calculaDescolamentoBlocoCurricular(solucao); // pendente
	calculaGapsHorariosProfessores(solucao); // pendente
	avaliacaoCustoCorpoDocente(solucao); // finalizado
	violacoesCargasHorarias(solucao); // finalizado
	avaliaDiasProfessorMinistraAula(solucao); // finalizado
	violacaoUltimaPrimeiraAula(solucao); // finalizado
	avaliaNumeroMestresDoutores(solucao); // finalizado
	avaliaMaximoDisciplinasProfessorPorCurso(solucao); // finalizado
	avaliaPreferenciasProfessorDisciplina(solucao); // finalizado
	avaliaCustoProfessorVirtual(solucao); // finalizado

	double funcao_objetivo = 0.0;

	// Contabiliza��o do valor da solu��o
	// TODO -- atribuir valores adequados aos PESOS
	funcao_objetivo += (PESO_FIXACAO * totalViolacaoRestricaoFixacao);
	funcao_objetivo += (PESO_DESLOCAMENTO * totalViolacoesDescolamento);
	funcao_objetivo += (PESO_TEMPO_DESLOCAMENTO * totalTempoDescolamento);
	funcao_objetivo += (PESO_VIOLACAO_DESLOCAMENTO_PROFESSOR * totalViolacoesDeslocamentoProfessor);
	funcao_objetivo += (PESO_DESLOCAMENTO_PROFESSOR * totalDeslocamentosProfessor);
	funcao_objetivo += (PESO_GAPS_HORARIO * totalGapsHorariosProfessores);
	funcao_objetivo += (PESO_NOTA_CORPO_DOCENTE * totalAvaliacaoCorpoDocente);
	funcao_objetivo += (PESO_CUSTO_CORPO_DOCENTE * totalCustoCorpoDocente);
	funcao_objetivo += (PESO_CH_MINIMA_ANTERIOR * totalViolacoesCHMinimaSemestreAterior);
	funcao_objetivo += (PESO_CH_MINIMA_PROFESSOR * totalViolacoesCHMinimaProfessor);
	funcao_objetivo += (PESO_CH_MAXIMA_PROFESSOR * totalViolacoesCHMaximaProfessor);
	funcao_objetivo += (PESO_TOTAL_DIAS_SEMANA * totalDiasProfessorMinistraAula);
	funcao_objetivo += (PESO_ULTIMA_E_PRIMEIRA_AULA * totalViolacoesUltimaPrimeiraAula);
	funcao_objetivo += (PESO_PERCENTUAL_MESTRES * totalViolacoesMestres);
	funcao_objetivo += (PESO_PERCENTUAL_DOUTORES * totalViolacoesDoutores);
	funcao_objetivo += (PESO_DISCIPLINAS_PROFESSOR_CURSO * totalViolacoesDiscProfCurso);
	funcao_objetivo += (PESO_PREFERENCIA_DISCIPLINA * totalPreferenciasProfessorDisciplina);
	funcao_objetivo += (PESO_NUMERO_PROFESSORES_VIRTUAIS * totalProfessoresVirtuais);
	funcao_objetivo += (PESO_CREDITOS_PROFESSORES_VIRTUAIS * totalCreditosProfessoresVirtuais);

	// Exibe os resultados detalhados da avalia��o
	if (imprime_resultados)
	{
		this->imprimeResultados();
	}

	return funcao_objetivo;
}

void Avaliador::calculaViolacaoRestricaoFixacao(SolucaoOperacional & solucao)
{
	double numViolacoes = 0.0;
	bool encontrou_fixacao = false;

	// Para cada fixa��o de aula a um professor, verifica se h�
	// uma aula atribu�da ao professor que corresponda a essa fixa��o
	ITERA_GGROUP(it_fixacao, solucao.getProblemData()->fixacoes, Fixacao)
	{
		// Recupera o professor correspondente � fixa��o
	 	int id_professor = it_fixacao->getProfessorId();
		Professor * professor = solucao.mapProfessores[id_professor];
		int linha_professor = professor->getIdOperacional();

		// Percorre a linha correspondente ao professor na matriz de solu��o
		for (unsigned int i = 0; i < solucao.getMatrizAulas()->at(linha_professor)->size(); i++)
		{
			Aula * aula = solucao.getMatrizAulas()->at(linha_professor)->at(i);
			if (aula == NULL || aula->eVirtual() == true)
			{
				continue;
			}

			// TODO -- devo considerar 'turno' e 'hor�rio' ?????
			if ( (it_fixacao->getDiaSemana() == aula->getDiaSemana()) &&
				(it_fixacao->getDisciplinaId() == aula->getDisciplina()->getId()) &&
				(it_fixacao->getSalaId() == aula->getSala()->getId()) )
			{
				encontrou_fixacao = true;
				break;
			}
		}

		// Caso n�o tenha encontrado a
		// fixa��o, anota-se a viola��o
		if (!encontrou_fixacao)
		{
			numViolacoes++;
		}
	}

	totalViolacaoRestricaoFixacao = numViolacoes;
}

void Avaliador::calculaDescolamentoProfessor(SolucaoOperacional & solucao)
{
	int violacoes_deslocamento_professor = 0; // TRIEDA-776
	int num_deslocamentos_professor = 0; // TRIEDA-777

	Unidade * unidade_atual = NULL;
	Unidade * unidade_anterior = NULL;
	Campus * campus_atual = NULL;
	Campus * campus_anterior = NULL;
	Aula * aula_atual = NULL;
	Aula * aula_anterior = NULL;

	int id_unidade_atual = -1;
	int id_unidade_anterior = -1;
	int id_campus_atual = -1;
	int id_campus_anterior = -1;

	double tempo_minimo = 0.0;
	double tempo_disponivel = 0.0;
	int indice_horario_atual = -1;
	int indice_horario_anterior = -1;

	// Armazena o limite de deslocamentos entre campus para os professores
	int limite_deslocamentos_professor
		= solucao.getProblemData()->parametros->maxDeslocProf;

	// Armazena o n�mero de deslocamentos entre campus distintos
	// feito por um dado professor, para verificar se excedeu o liimte
	int num_desloc_professor_temp = 0;

	// Percorrer a matriz da solu��o, verificando cada par
	// de aulas consecutivo, em um mesmo dia da semana, para
	// avaliar o deslocamento entre as respectivas salas de aula
	for (unsigned int i = 0; i < solucao.getMatrizAulas()->size(); i++)
	{
		// Inicializa o n�mero de deslocamentos entre
		// campus diferentes para o professor atual
		num_desloc_professor_temp = 0;

		// Inicializa o �ndice do hor�rio anterior
		indice_horario_anterior = 0;

		// Para cada par de aulas consecutivas de um determinado
		// professor, no mesmo dia da semana, verifica-se se houve
		// um deslocamento acima do desejado entre uma sala e outra
		for (unsigned int j = 0; j < solucao.getMatrizAulas()->at(i)->size(); j++)
		{
			// O �ndice 'j' corresponde � coluna da matriz
			indice_horario_atual = j;

			// Recupera o objeto 'aula' atual
			aula_atual = solucao.getMatrizAulas()->at(i)->at(indice_horario_atual);

			// O professor n�o teve aula atribu�da nesse hor�rio
			if (aula_atual == NULL || aula_atual->eVirtual() == true)
			{
				continue;
			}

			// Unidade da sala atual
			id_unidade_atual = aula_atual->getSala()->getIdUnidade();
			unidade_atual = solucao.getProblemData()->refUnidade[id_unidade_atual];

			// Campus da sala atual
			id_campus_atual = unidade_atual->getIdCampus();
			campus_atual = solucao.getProblemData()->refCampus[id_campus_atual];

			// Recupera o objeto 'aula' anterior
			aula_anterior = solucao.getMatrizAulas()->at(i)->at(indice_horario_anterior);
			if (aula_anterior == NULL || aula_anterior->eVirtual() == true)
			{
				// Atualiza os ponteiros para a pr�xima itera��o
				campus_anterior = campus_atual;
				id_campus_anterior = id_campus_atual;

				unidade_anterior = unidade_atual;
				id_unidade_anterior = id_unidade_atual;

				aula_anterior = aula_atual;
				indice_horario_anterior = indice_horario_atual;

				continue;
			}

			// Verifica se houve viola��o no deslocamento vi�vel
			// Verifica se as aulas s�o em um mesmo dia da semana
			if (aula_anterior->getDiaSemana() == aula_atual->getDiaSemana())
			{
				// O professor teve que se deslocar entre CAMPUS diferentes
				// Obs.: N�o est� sendo considerado o deslocamento entre UNIDADES
				if (id_campus_atual != id_campus_anterior)
				{
					// Essa vari�vel guarda apenas os
					// deslocamentos de um �nico professor,
					// para verificar se excedeu o limite permitido
					// (Ser� utilizado no crit�rio n� 3)
					num_desloc_professor_temp++;

					// Crit�rio de avalia��o n� 4:
					// N�mero de deslocamentos do professor
					// Obs.: Armazena TODOS os deslocamentos ocorridos
					num_deslocamentos_professor++;
				}
			}

			// Atualiza os ponteiros para a pr�xima itera��o
			campus_anterior = campus_atual;
			id_campus_anterior = id_campus_atual;

			unidade_anterior = unidade_atual;
			id_unidade_anterior = id_unidade_atual;

			aula_anterior = aula_atual;
			indice_horario_anterior = indice_horario_atual;
		}

		// Crit�rio de avalia��o n� 3:
		// Verifica se o n�mero de deslocamentos entre campus
		// diferentes excedeu o limite fornecido como par�metro de entrada
		if (num_desloc_professor_temp > limite_deslocamentos_professor)
		{
			violacoes_deslocamento_professor++;
		}
	}

	// Total de viola��es de deslocamento entre campus diferentes pelos professores
	totalViolacoesDeslocamentoProfessor = violacoes_deslocamento_professor;

	// Total de deslocamentos entre campus diferentes pelos professores
	totalDeslocamentosProfessor = num_deslocamentos_professor;
}

void Avaliador::calculaDescolamentoBlocoCurricular(SolucaoOperacional& solucao)
{
	int violacoes_deslocamento = 0;  // TRIEDA-739
	double tempo_deslocamento = 0.0; // TRIEDA-740

	unsigned int i, j;

	Aula * aula = NULL;
	Curso * curso = NULL;
	Disciplina * disciplina = NULL;
	Oferta * oferta = NULL;
	BlocoCurricular * bloco = NULL;

	int id_unidade_atual = -1;
	int id_unidade_anterior = -1;
	int id_campus_atual = -1;
	int id_campus_anterior = -1;

	Unidade* unidade_atual = NULL;
	Unidade* unidade_anterior = NULL;
	Campus* campus_atual = NULL;
	Campus* campus_anterior = NULL;
	Aula* aula_atual = NULL;
	Aula* aula_anterior = NULL;

	Horario * horario = NULL;

	// Dado um bloco curricular, recupera-se as aulas desse bloco
	// Estrutura do map 'mapBlocoAulaHorario':
	// Dado um bloco curricular qualquer (BlocoCurricular *), o map
	// retorna a lista de hor�rios desse bloco curricular, onde cada
	// hor�rio � representado por um par `Aula*, Horario*`
	std::map< BlocoCurricular *, GGroup< pair< Aula *, Horario * > > > mapBlocoAulaHorario;
	for (i = 0; i < solucao.getMatrizAulas()->size(); i++)
	{
		for (j = 0; j < solucao.getMatrizAulas()->at(i)->size(); j++)
		{
			// Aula atual
			aula = solucao.getMatrizAulas()->at(i)->at(j);
			if (aula == NULL || aula->eVirtual() == true)
			{
				continue;
			}

			// Recupera o hor�rio referente � aula atual
			horario = solucao.getHorario(i, j);

			// Disciplina correspondente � aula atual
			disciplina = aula->getDisciplina();

			ITERA_GGROUP(it_oferta, aula->ofertas, Oferta)
			{
				// Oferta atendida pela aula
				oferta = *(it_oferta);

				// Curso correspondente a essa oferta
				curso = oferta->curso;

				// Bloco curricular que corresponde a esse par curso/disciplina
				bloco = solucao.getProblemData()->
					mapCursoDisciplina_BlocoCurricular[ std::make_pair(curso, disciplina) ];

				// Adicona o par 'aula/hor�rio' no conjunto de aulas que
				// est�o relacionadas ao bloco curricular atual
				GGroup< std::pair< Aula *, Horario * > > * aulas_horarios
					= &( mapBlocoAulaHorario[ bloco ] );

				aulas_horarios->add( std::make_pair(aula, horario) );
			}
		}
	}

	// Ordena as aulas de cada bloco curricular
	std::map< BlocoCurricular * , vector< Aula * > > mapBlocoAulas_Ordenado;
	std::map< BlocoCurricular * , GGroup< std::pair< Aula *, Horario * > > > ::iterator it_map
		= mapBlocoAulaHorario.begin();
	for (; it_map != mapBlocoAulaHorario.end(); it_map++)
	{
		// Bloco curricular
		bloco = it_map->first;

		// Conjunto de aulas (e respectivos hor�rios) desse bloco curricular
		GGroup< std::pair< Aula *, Horario * > > * aulas_horarios = &( it_map->second );

		// Associa o bloco curricular ao conjunto de aulas
		mapBlocoAulas_Ordenado[ bloco ] = retornaVectorAulasOrdenado( *aulas_horarios );
	}

	double tempo_minimo = 0.0;
	double tempo_disponivel = 0.0;
	int indice_horario_anterior = 0;
	int indice_horario_atual = 0;

	// Verificar viola��es de deslocamento
	// entre as aulas de um memso bloco curricular
	std::map< BlocoCurricular * , vector< Aula * > >::iterator it_bloco_aulas
		= mapBlocoAulas_Ordenado.begin();
	for (; it_bloco_aulas != mapBlocoAulas_Ordenado.end(); it_bloco_aulas++)
	{
		// Recupera o bloco curricular
		bloco = it_bloco_aulas->first;

		// Recupera as aulas desse bloco
		vector< Aula * > aulas = it_bloco_aulas->second;

		// Inicializa o �ndice do hor�rio anterior
		indice_horario_anterior = 0;

		for (i = 1; i < aulas.size(); i++)
		{
			// �ndice da aula atual no vector
			indice_horario_atual = i;

			// Recupera a aula atual
			aula_atual = aulas.at(indice_horario_atual);
			if (aula_atual == NULL || aula_atual->eVirtual() == true)
			{
				continue;
			}

			// Recupera a aula anterior
			aula_anterior = aulas.at(indice_horario_anterior);
			if (aula_anterior == NULL || aula_anterior->eVirtual() == true)
			{
				continue;
			}

			// Unidade da sala atual
			id_unidade_atual = aula_atual->getSala()->getIdUnidade();
			unidade_atual = solucao.getProblemData()->refUnidade[id_unidade_atual];

			// Campus da sala atual
			id_campus_atual = unidade_atual->getIdCampus();
			campus_atual = solucao.getProblemData()->refCampus[id_campus_atual];

			// Verifica se houve viola��o no deslocamento vi�vel
			// Verifica se as aulas s�o em um mesmo dia da semana
			if (aula_anterior->getDiaSemana() == aula_atual->getDiaSemana())
			{
				// Tempo de deslocamento entre uma aula e outra
				tempo_minimo = calculaTempoEntreCampusUnidades(solucao,
					campus_atual, campus_anterior, unidade_atual, unidade_anterior);
				tempo_minimo = abs(tempo_minimo);

				// Tempo existente entre as aulas 'aula_anterior' e 'aula_atual'
				tempo_disponivel = (indice_horario_atual - indice_horario_anterior) * (MINUTOS_POR_HORARIO);
				tempo_disponivel = abs(tempo_disponivel);

				// Verifica se ocorreu a viola��o de tempo m�nimo
				// necess�rio para se deslocar entre campus/unidades
				if (tempo_disponivel < tempo_minimo)
				{
					// Crit�rio de avalia��o n� 1:
					// N�mero de viola��es ocorridas de tempo vi�vel
					violacoes_deslocamento++;
				}

				// Crit�rio de avalia��o n� 2:
				// Tempo de deslocamento entre uma aula e outra
				tempo_deslocamento += (tempo_minimo);
			}

			// Atualiza os ponteiros para a pr�xima itera��o
			campus_anterior = campus_atual;
			unidade_anterior = unidade_atual;
			aula_anterior = aula_atual;
			indice_horario_anterior = indice_horario_atual;
		}
	}

	// Total de deslocamentos entre campus e/ou
	// unidades que excederam o tempo m�nimo necess�rio
	totalViolacoesDescolamento = violacoes_deslocamento;

	// Tempo total de deslocamentos entre campus e/ou
	// unidades que excederam o tempo m�nimo necess�rio
	totalTempoDescolamento = tempo_deslocamento;
}

// M�todo que compara duas aulas, de acordo
// com o dia da semana e o hor�rio da aula
bool ordenaAulas( pair<Aula*, Horario*> aula_horario1,
				  pair<Aula*, Horario*> aula_horario2 )
{
	// Verifica a consist�ncia dos objetos 'aula'
	if ( aula_horario1.first == NULL
			&& aula_horario1.first == NULL )
	{
		return false;
	}
	else if ( aula_horario1.first == NULL
			&& aula_horario2.first != NULL )
	{
		return true;
	}
	else if ( aula_horario1.first != NULL
			&& aula_horario2.first == NULL )
	{
		return false;
	}

	// Primeiro crit�rio: dia da semana
	int dia_semana1 = ( aula_horario1.first->getDiaSemana() );
	int dia_semana2 = ( aula_horario2.first->getDiaSemana() );
	if ( dia_semana1 < dia_semana2 )
	{
		return true;
	}
	else if ( dia_semana1 > dia_semana2 )
	{
		return false;
	}

	// Segundo crit�rio: hor�rio da aula
	DateTime horario1 = ( aula_horario1.second->horario_aula->getInicio() );
	DateTime horario2 = ( aula_horario2.second->horario_aula->getInicio() );
	if ( horario1 < horario2 )
	{
		return true;
	}
	else if ( horario1 > horario2 )
	{
		return false;
	}

	return false;
}

// Ordena as aulas por 'dia da semana'
// e 'hor�rio', nessa ordem de prioridade
vector< Aula * > Avaliador::retornaVectorAulasOrdenado(
	GGroup< pair<Aula*, Horario*> > aulas_horarios)
{
	// Passa da representa�ao de 'GGroup'
	// para 'vector', para ordenar as aulas
	std::vector< std::pair< Aula *, Horario * > > pares_aulas_horarios;
	GGroup< std::pair< Aula *, Horario * > >::iterator it_pair
		= aulas_horarios.begin();
	for (; it_pair != aulas_horarios.end(); it_pair++)
	{
		pares_aulas_horarios.push_back( *it_pair );
	}

	// Ordena o vetor com os crit�rios de dia da semana e hor�rios de aula
	std::sort(pares_aulas_horarios.begin(),
			  pares_aulas_horarios.end(), ordenaAulas);

	// Recupera as aulas do vertor ordenado
	std::vector< Aula* > aulas_ordenado;
	for (unsigned int i = 0; i < pares_aulas_horarios.size(); i++)
	{
		aulas_ordenado.push_back( pares_aulas_horarios.at(i).first );
	}

	return aulas_ordenado;
}

double Avaliador::calculaTempoEntreCampusUnidades(SolucaoOperacional & solucao,
		Campus * campus_atual, Campus * campus_anterior,
		Unidade * unidade_atual, Unidade * unidade_anterior)
{
	double distancia = 0.0;

	// As aulas s�o realizadas em campus diferentes
	if (campus_atual->getId() != campus_anterior->getId())
	{
		GGroup<Deslocamento*>::iterator it_tempo_campi
			= solucao.getProblemData()->tempo_campi.begin();
		for (; it_tempo_campi != solucao.getProblemData()->tempo_campi.end();
			   it_tempo_campi++)
		{
			if (it_tempo_campi->getOrigemId() == campus_anterior->getId()
				&& it_tempo_campi->getDestinoId() == campus_atual->getId())
			{
				distancia = it_tempo_campi->getTempo();
			}
		}
	}
	// As aulas s�o realizadas em unidades diferentes
	else if (unidade_atual->getId() != unidade_anterior->getId())
	{
		GGroup<Deslocamento*>::iterator it_tempo_unidade
			= solucao.getProblemData()->tempo_unidades.begin();
		for (; it_tempo_unidade != solucao.getProblemData()->tempo_unidades.end();
			   it_tempo_unidade++)
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

	Aula * aula_atual = NULL;
	Aula * aula_anterior = NULL;

	int indice_aula_atual = -1;
	int indice_aula_anterior = -1;

	Professor * professor = NULL;
	Horario * h1 = NULL;
	Horario * h2 = NULL;

	// Inicializa o vetor de gaps de cada professor
	gapsProfessores.clear();
	for (unsigned int i = 0; i < solucao.mapProfessores.size(); i++)
	{
		std::vector< int > gaps;
		gaps.clear();
		gapsProfessores.push_back( gaps );
	}

	int dia_semana = 0;

	// Percorre as aulas alocadas a cada professor
	for (unsigned int i = 0; i < solucao.getMatrizAulas()->size(); i++)
	{
		// Inicializa os �ndices
		aula_anterior = NULL;
		indice_aula_anterior = -1;

		// Recupera o professor atual
		professor = solucao.getProfessorMatriz(i);

		for (unsigned int j = 0; j < solucao.getMatrizAulas()->at(i)->size(); j++)
		{
			indice_aula_atual = j;
			aula_atual = solucao.getMatrizAulas()->at(i)->at(j);

			// O professor n�o tem aula atribu�da nesse hor�rio
			if (aula_atual == NULL || aula_atual->eVirtual() == true)
			{
				continue;
			}

			if ( aula_anterior != NULL
				&& aula_anterior->getDiaSemana() == aula_atual->getDiaSemana() )
			{
				// Avalia se ocorreu um gap no hor�rio
				int gap = (indice_aula_atual - indice_aula_anterior);
				if (gap > 1)
				{
					dia_semana = aula_atual->getDiaSemana();
					h1 = solucao.getHorario(i, indice_aula_anterior);
					h2 = solucao.getHorario(i, indice_aula_atual);

					// Dado que ocorreu um gap entre duas aulas, devo
					// verificar se o professor possui hor�rios dispon�veis
					// no per�odo entre a aula anterior e a aula atual, o que
					// caracterizaria um fato 'indesejado' na solu��o operacional
					if (horariosDisponiveisIntervalo(professor, dia_semana, h1, h2) > 0)
					{
						totalGapsHorariosProfessores++;
						gapsProfessores.at(i).push_back(gap);
					}
				}
			}

			// Atualiza os ponteiros para a pr�xima itera��o
			indice_aula_anterior = indice_aula_atual;
			aula_anterior = aula_atual;
		}
	}

	totalGapsHorariosProfessores = numGaps;
}

// Dados dois hor�rios de um professor, em um mesmo dia da semana,
// devo informar se existem hor�rios dispon�veis ENTRE esses dois
// hor�rios, N�O CONSIDERANDO os extremos do intervalo de hor�rios
int Avaliador::horariosDisponiveisIntervalo(Professor * professor,
											int dia_semana, Horario * h1, Horario * h2)
{
	HorarioAula * horario_aula1 = h1->horario_aula;
	HorarioAula * horario_aula2 = h2->horario_aula;

	int horariosDisponiveis = 0;
	GGroup<Horario*>::iterator it_horario = professor->horarios.begin();
	for (; it_horario != professor->horarios.end(); it_horario++)
	{
		// Se o hor�rio dispon�vel estiver dentro do intervalo de gap,
		// ent�o encontrei mais um gap indesejado de hor�rio para esse professor
		if ( ( it_horario->dias_semana.find(dia_semana) != it_horario->dias_semana.end() ) &&
			 ( horario_aula1->getInicio() < it_horario->horario_aula->getInicio() ) &&
			 ( horario_aula2->getInicio() > it_horario->horario_aula->getInicio() ) )
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

	// Para cada professor na solu��o, devo avaliar o custo da
	// inclus�o desse professor e tamb�m o somat�rio das notas
	// de avalia��o desse professor nas disciplinas que ele leciona
	std::map< int, Professor * >::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end();
		   it_professor++)
	{
		// Somando as notas de avalia��o do professor nas suas disciplinas
		ITERA_GGROUP(it_mag, it_professor->second->magisterio, Magisterio)
		{
			avaliacaoCorpoDocente += it_mag->getNota();
		}

		// Adiciona o custo desse professor
		// no custo total do corpo docente da solu��o
		custoCorpoDocente += it_professor->second->getValorCredito();
	}

	totalAvaliacaoCorpoDocente = avaliacaoCorpoDocente;
	totalCustoCorpoDocente = custoCorpoDocente;
}

void Avaliador::violacoesCargasHorarias(SolucaoOperacional & solucao)
{
	// Armazena o total de viola��es de cada tipo
	int violacoesSemestreAnterior = 0;
	int violacoesCHMinima = 0;
	int violacoesCHMaxima = 0;

	int tempSemestreAnterior = 0;
	int tempCHMinima = 0;
	int tempCHMaxima = 0;

	int semestre_anterior = 0;
	int ch_min = 0;
	int ch_max = 0;

	// Inicializa o vetor de viola��es de cada professor
	violacoesCHMinimaSemestreAterior.clear();
	violacoesCHMinimaProfessor.clear();
	violacoesCHMaximaProfessor.clear();
	for (unsigned int i = 0; i < solucao.mapProfessores.size(); i++)
	{
		violacoesCHMinimaSemestreAterior.push_back(0);
		violacoesCHMinimaProfessor.push_back(0);
		violacoesCHMaximaProfessor.push_back(0);
	}

	Aula * aula = NULL;
	int cont_creditos = 0;
	int linha_professor = 0;

	std::map<int, Professor*>::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end();
		   it_professor++)
	{
		cont_creditos = 0;
		linha_professor = it_professor->second->getIdOperacional();

		// Verifica quantos cr�ditos o professor ministrar� na semana
		for (unsigned i = 0; i < solucao.getMatrizAulas()->at(linha_professor)->size(); i++)
		{
			aula = solucao.getMatrizAulas()->at(linha_professor)->at(i);
			if (aula != NULL && aula->eVirtual() == false)
			{
				cont_creditos++;
			}
		}

		semestre_anterior = it_professor->second->getChAnterior();
		ch_min = it_professor->second->getChMin();
		ch_max = it_professor->second->getChMax();

		// Verifica carga hor�ria do semestre anterior
		tempSemestreAnterior = ( cont_creditos - semestre_anterior );
		if (tempSemestreAnterior < 0)
		{
			violacoesSemestreAnterior++;
			violacoesCHMinimaSemestreAterior[linha_professor] = abs( tempSemestreAnterior );
		}

		// Verifica carga hor�ria m�nima do professor
		tempCHMinima = ( cont_creditos - ch_min );
		if (tempCHMinima < 0)
		{
			violacoesCHMinima++;
			violacoesCHMinimaProfessor[linha_professor] = abs( tempCHMinima );
		}

		// Verifica carga hor�ria m�xima do professor
		tempCHMaxima = ( cont_creditos - ch_max );
		if (tempCHMaxima > 0)
		{
			violacoesCHMaxima++;
			violacoesCHMaximaProfessor[linha_professor] = abs( tempCHMaxima );
		}
	}

	// Informa o total de viola��es de cada tipo foram verificadas
	totalViolacoesCHMinimaSemestreAterior = violacoesSemestreAnterior;
	totalViolacoesCHMinimaProfessor = violacoesCHMinima;
	totalViolacoesCHMaximaProfessor = violacoesCHMaxima;
}

void Avaliador::avaliaDiasProfessorMinistraAula(SolucaoOperacional & solucao)
{
	int numDias = 0;

	Aula * aula = NULL;
	int linha_professor = 0;

	// Inicializa o vetor de dias da semana dos professores
	professorMinistraAula.clear();
	for (unsigned int i = 0; i < solucao.mapProfessores.size(); i++)
	{
		professorMinistraAula.push_back(0);
	}

	// Para cada professor da solu��o operacional, procura-se
	// o total de dias da semana que ele tem aulas para ministrar
	std::map< int, Professor * >::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end(); it_professor++)
	{
		// ID da linha correspondente a esse professor, na matriz de solu��o
		linha_professor = it_professor->second->getIdOperacional();

		// Para cada aula que o professor ministrar, devo inserir
		// o dia da semana dessa aula na lista de dias, ignorando repeti��es
		GGroup< int > dias_semana;
		for (unsigned i = 0; i < solucao.getMatrizAulas()->at(linha_professor)->size(); i++)
		{
			aula = solucao.getMatrizAulas()->at(linha_professor)->at(i);
			if (aula != NULL && aula->eVirtual() == false)
			{
				dias_semana.add(aula->getDiaSemana());
			}
		}

		// Armazena o total de dias 
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
	int total_horarios = 0;

	// Informa o n�mero de aulas que
	// cada dia da semana poder� possuir
	int bloco_aula = calculaTamanhoBlocoAula( solucao );

	// �ltima aula do dia D
	Aula * aula1 = NULL;

	// Primeira aula do dia D+1
	Aula * aula2 = NULL;

	// Inicializa o n�mero de viola��es de cada professor
	violacoesUltimaPrimeiraAulaProfessor.clear();
	for (unsigned i = 0; i < solucao.mapProfessores.size(); i++)
	{
		violacoesUltimaPrimeiraAulaProfessor.push_back(0);
	}

	std::map< int, Professor * >::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end(); it_professor++)
	{
		violacoesProfessor = 0;
		linha_professor = it_professor->second->getIdOperacional();

		// Verifica as aulas do professor, procurando popr viola��es
		// do tipo "�ltima aula do dia D e primeira aula do dia D+1"
		total_horarios = solucao.getMatrizAulas()->at(linha_professor)->size();
		for (int i = bloco_aula-1; i < total_horarios-1; i += bloco_aula)
		{
			aula1 = solucao.getMatrizAulas()->at(linha_professor)->at(i);
			aula2 = solucao.getMatrizAulas()->at(linha_professor)->at(i+1);

			// Verifica se os dois hor�rios foram alocados
			if (aula1 != NULL && aula1->eVirtual() == false
				&& aula2 != NULL && aula2->eVirtual() == false )
			{
				violacoesProfessor++;
			}
		}

		// Armazena as viola��es do professor individualmente e
		// incrementa o total de viola��es encontrados na solu��o
		violacoesUltimaPrimeiraAulaProfessor[ linha_professor ] = violacoesProfessor;
		numViolacoes += violacoesProfessor;
	}

	totalViolacoesUltimaPrimeiraAula = numViolacoes;
}

int Avaliador::calculaTamanhoBlocoAula(SolucaoOperacional & solucao)
{
	int bloco_aula = 0;

	unsigned i = 0;
	vector< int > cont_horarios;

	// Esse vetor cont�m uma posi��o para
	// cada dia da semana, iniciando o contador
	// de hor�rios de cada dia em zero hor�rios
	cont_horarios.clear();
	for (i = 0; i < 8; i++)
	{
		// Insere um n�mero de dias da semana no vetor
		// quantos forem os dias da semana considerados na solu��o
		cont_horarios.push_back(0);
	}

	// Para cada um dos hor�rios dispon�veis, essa itera��o
	// incrementa o n�mero de hor�rios dispon�veis em cada dia da
	// semana, a cada ocorr�ncia de hor�rio/dia da semana encontrada
	ITERA_GGROUP(it_campi, solucao.getProblemData()->campi, Campus)
	{
		ITERA_GGROUP(it_horario, it_campi->horarios, Horario)
		{
			GGroup< int >::iterator it_dia_semana
				= it_horario->dias_semana.begin();
			for (; it_dia_semana != it_horario->dias_semana.end();
				   it_dia_semana++)
			{
				cont_horarios[ *it_dia_semana ]++;
			}
		}
	}

	// Procura pelo dia da semana com mais hor�rios
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

	int id_titulacao_mestre = 4;
	int id_titulacao_doutor = 5;

	int id_curso = 0;
	int id_titulacao = 0;

	Aula * aula = NULL;
	Professor * professor = NULL;

	// Relaciona um curso com o conjunto de professores
	// que leciona pelo menos uma disciplina desse curso.
	// Estrutura do map: dado o 'id' de um curso, retorna-se
	// o conjunto de professores relacinado a esse curso.
	std::map< int, GGroup< Professor * > > mapCursosProfessores;

	//-------------------------------------------------------------------------------------
	// Associa as turmas de cada disciplina � seu professor correspondente
	for (unsigned int i = 0; i < solucao.getMatrizAulas()->size(); i++)
	{
		// Professor da itera��o atual
		professor = solucao.getProfessorMatriz(i);

		for (unsigned int j = 0; j < solucao.getMatrizAulas()->at(i)->size(); j++)
		{
			aula = solucao.getMatrizAulas()->at(i)->at(j);
			if (aula == NULL || aula->eVirtual() == true )
			{
				continue;
			}

			// Adiciona o professor na lista de professores
			// do curso ao qual a aula atual atende
			Oferta * oferta = NULL;
			ITERA_GGROUP(it_oferta, solucao.getProblemData()->ofertas, Oferta)
			{
				ITERA_GGROUP(it_oferta_aula, aula->ofertas, Oferta)
				{
					if (it_oferta_aula->getId() == it_oferta->getId())
					{
						oferta = *(it_oferta_aula);
						id_curso = oferta->getCursoId();
						mapCursosProfessores[ id_curso ].add(professor);
						break;
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------

	int num_professores_curso = 0;
	int cont_mestres = 0;
	int cont_doutores = 0;
	double percentual_mestres = 0.0;
	double percentual_doutores = 0.0;

	//-------------------------------------------------------------------------------------
	// Para cada curso, devo verificar a titula��o de
	// seus professores, verificando se a solu��o atende
	// ao percentual m�nimo exigido de mestres e doutores
	std::map< int, GGroup< Professor * > >::iterator it_cursos_professores
		= mapCursosProfessores.begin();
	for (; it_cursos_professores != mapCursosProfessores.end();
		   it_cursos_professores++)
	{
		// Id do curso atual
		id_curso = it_cursos_professores->first;

		// Inicia o n�mero de mestres e doutores com zero
		cont_mestres = 0;
		cont_doutores = 0;

		// Percorre a lista de professores que
		// ministram pelo menos uma aula do curso atual
		GGroup< Professor * > professores = it_cursos_professores->second;

		// Total de professores que ministram aulas desse curso
		num_professores_curso = professores.size();

		ITERA_GGROUP(it_professor, professores, Professor)
		{
			// Se for doutor, incrementa-se o contador de mestres
			// e tamb�m o contador de doutores associados ao curso
			if (it_professor->getTitulacaoId() == id_titulacao_doutor)
			{
				cont_mestres++;
				cont_doutores++;
			}
			// Se for mestre, incrementa-se apenas
			// o contador de mestres associados ao curso atual
			else if (it_professor->getTitulacaoId() == id_titulacao_mestre)
			{
				cont_mestres++;
			}
		}

		// Procura pelo curso atual na lista de cursos do 'problemaData'
		Curso * curso = procuraCurso( id_curso, solucao.getProblemData()->cursos );

		// Recupera os dados de porcentagem m�nima exigida
		// 'first'  -> c�digo do tipo da titula��o
		// 'second' -> percentual m�nimo de professores com essa titula��o
		percentual_mestres  = ( curso->regra_min_mestres.second );
		percentual_doutores = ( curso->regra_min_doutores.second );

		// Verifica se o n�mero de mestres e doutores
		// da solu��o atende ao n�mero m�nimo exigido
		if (cont_mestres / (double)num_professores_curso < percentual_mestres)
		{
			violacoesMestres++;
		}
		if (cont_doutores / (double)num_professores_curso < percentual_doutores)
		{
			violacoesDoutores++;
		}
	}
	//-------------------------------------------------------------------------------------

	totalViolacoesMestres = violacoesMestres;
	totalViolacoesDoutores = violacoesDoutores;
}

void Avaliador::avaliaMaximoDisciplinasProfessorPorCurso(SolucaoOperacional & solucao)
{
	int violacoes = 0;

	int id_curso = 0;
	int id_disciplina = 0;
	int id_professor = 0;

	// Map que relaciona cada professor com as disciplinas �s quais ele
	// est� alocado na solu��o Estrutura do map: dado o 'id' de um professor,
	// retorna-se um conjunto de disciplinas que esse professor leciona.
	// Essas disciplinas estar�o agrupadas pelo curso ao qual elas pertencem.
	std::map< int, std::map< int, GGroup< int > > > mapProfessorCursoDisciplinas;

	//-------------------------------------------------------------------------------------
	// Para cada propfessor, devo procurar pelas disciplinas
	// distintas que esse professor tem alocadas a ele na solu��o
	Aula * aula = NULL;
	Curso * curso = NULL;
	Professor * professor = NULL;
	for (unsigned int i = 0; i < solucao.getMatrizAulas()->size(); i++)
	{
		// Recupera o objeto 'professor' atual
		professor = solucao.getProfessorMatriz(i);

		// Lista de disciplinas desse professor
		GGroup< int > ids_disciplinas;
		for (unsigned int j = 0; j < solucao.getMatrizAulas()->at(i)->size(); j++)
		{
			aula = solucao.getMatrizAulas()->at(i)->at(j);
			if (aula != NULL && aula->eVirtual() == false)
			{
				// Id do professor atual
				id_professor = professor->getId();

				// Id da disciplina atual
				id_disciplina = aula->getDisciplina()->getId();

				// Recupera a lista de cursos do professor atual
				std::map< int, GGroup< int > > * map_cursos
					= &( mapProfessorCursoDisciplinas[ id_professor ] );

				// Como uma aula pode atender a mais de uma
				// oferta, ent�o devo relacionar a aula atual
				// com cada curso correspondente a cada oferta
				GGroup< Oferta * >::iterator it_oferta
					= aula->ofertas.begin();
				for (; it_oferta != aula->ofertas.end(); it_oferta++)
				{
					// Id do curso correspondente � oferta
					id_curso = it_oferta->getCursoId();

					// Recupera a lista de disciplinas do professor no curso
					GGroup< int > * ids_disciplinas = &( (*map_cursos)[id_curso] );

					// Adiciona a disciplina atual na lista
					ids_disciplinas->add( id_disciplina );
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------

	int linha_professor = 0;
	int disc_professor = 0;
	int max_disc_professor = 0;
	int diferenca_disciplinas = 0;
	int violacoes_professor = 0;

	// Inicializa as viola��es de cada professor como zero
	violacoesDisciplinasProfessor.clear();
	for (unsigned int i = 0; i < solucao.mapProfessores.size(); i++)
	{
		violacoesDisciplinasProfessor.push_back(0);
	}

	//-------------------------------------------------------------------------------------
	// Verifica o limite de disciplinas de cada professor
	std::map< int, std::map< int, GGroup< int > > >::iterator it_prof_cursco_disc
		= mapProfessorCursoDisciplinas.begin();
	for (; it_prof_cursco_disc != mapProfessorCursoDisciplinas.end();
		   it_prof_cursco_disc++)
	{
		// C�digo do professor
		id_professor = it_prof_cursco_disc->first;

		// Linha correspondente ao professor na matriz de solu��o
		professor = solucao.mapProfessores[ id_professor ];
		linha_professor = professor->getIdOperacional();

		// Verifica cada curso ao qual o professor atual
		// tem pelo menos uma aula alocada na solu��o operacional
		std::map< int, GGroup< int > >::iterator it_cursos
			= it_prof_cursco_disc->second.begin();
		for (; it_cursos != it_prof_cursco_disc->second.end(); it_cursos++)
		{
			// ID do curso
			id_curso = it_cursos->first;
			curso = procuraCurso(id_curso, solucao.getProblemData()->cursos);

			// Disciplinas �s quais o professor est� alocado nesse curso
			disc_professor = it_cursos->second.size();

			// M�ximo de disciplinas que o professor pode ministrar nesse curso
			max_disc_professor = curso->getQtdMaxProfDisc();

			// Verifica se houve viola��o do n�mero de disciplinas
			diferenca_disciplinas = (disc_professor - max_disc_professor);
			if (diferenca_disciplinas > 0)
			{
				// Incrementa o total de viola��es encontrado
				violacoes++;

				// Incrementa o n�mero de viola��es do professor atual
				violacoes_professor += diferenca_disciplinas;
			}
		}

		// Armazena o n�mero de disciplinas excedidas para esse professor
		violacoesDisciplinasProfessor[ linha_professor ] = violacoes_professor;
	}
	//-------------------------------------------------------------------------------------

	totalViolacoesDiscProfCurso = violacoes;
}

Curso* Avaliador::procuraCurso(int id_curso, GGroup<Curso*> cursos)
{
	// Procura pelo curso desejado na lista de cursos
	Curso* curso = NULL;
	ITERA_GGROUP(it_curso, cursos, Curso)
	{
		if (it_curso->getId() == id_curso)
		{
			curso = *(it_curso);
			break;
		}
	}

	return curso;
}

void Avaliador::avaliaPreferenciasProfessorDisciplina(SolucaoOperacional & solucao)
{
	int nota_acumulada = 0;

	Aula* aula = NULL;
	Professor* professor = NULL;

	int id_professor = 0;
	int id_disciplina = 0;
	int preferencia_disciplina = 0;

	// Inicializa o vetor de viola��es de acda professor
	preferenciasProfessor.clear();
	for (unsigned int i = 0; i < solucao.mapProfessores.size(); i++)
	{
		preferenciasProfessor.push_back(0);
	}

	// Para cada professor, criamos um map que relaciona cada
	// disciplina que esse professor leciona com a prefer�ncia
	// desse professor em lecionar essa dada disciplina
	std::map< int, std::map< int, int > > mapProfDiscPreferencia;

	// Para cada professor, criamos um 'map' que
	// relaciona cada uma de suas disciplinas com
	// a prefer�ncia do professor em lecionar essa disciplina
	ITERA_GGROUP(it_campi, solucao.getProblemData()->campi, Campus)
	{
		ITERA_GGROUP(it_professor, it_campi->professores, Professor)
		{
			// Recupera o 'objeto' professor 
			id_professor = it_professor->getId();

			// Recupera o map de disciplinas desse professor
			std::map<int, int> * mapDiscPreferencia
				= &( mapProfDiscPreferencia[ id_professor ] );
			mapDiscPreferencia->clear();

			// Percorre as disciplinas desse professor,
			// formando os pares 'disciplina'/'prefer�ncia'
			ITERA_GGROUP(it_disciplina, it_professor->magisterio, Magisterio)
			{
				// Id da disciplina
				id_disciplina = abs( it_disciplina->getDisciplinaId() );

				// Prefer�ncia em lecionar essa disciplina
				preferencia_disciplina = it_disciplina->getNota();

				// Relaciona o par disciplina/prefer�ncia ao professor
				(*mapDiscPreferencia)[ id_disciplina ] = preferencia_disciplina;
			}
		}
	}

	// Vari�vel que armazena o quanto a prefer�ncia do professor para
	// lecionar uma disciplina a ele alocada est� longe da prefer�ncia m�xima
	int nota_avaliacao = 0;

	// Com as prefer�ncias de cada disciplinas relacionadas, devo agora
	// calcular a avalia��o da solu��o no crit�rio 'prefer�ncia por discipina'
	for (unsigned int i = 0; i < solucao.getMatrizAulas()->size(); i++)
	{
		// Recupera o 'objeto' professor
		professor = solucao.getProfessorMatriz(i);
		id_professor = professor->getId();

		// Recupera o map de disciplinas do professor
		std::map< int, int > * mapDiscPreferencia
			= &( mapProfDiscPreferencia[ id_professor ] );

		// Percorre as aulas do professor na matriz de solu��o
		for (unsigned int j = 0; j < solucao.getMatrizAulas()->at(i)->size(); j++)
		{
			// Recupera o objeto 'aula' atual
			aula = solucao.getMatrizAulas()->at(i)->at(j);
			if (aula == NULL || aula->eVirtual() == true)
			{
				// N�o tem aula alocada nesse hor�rio
				continue;
			}

			// Recupera o id da disciplina correspondente a essa aula
			id_disciplina = abs( aula->getDisciplina()->getId() );

			// Recupera a prefer�ncia do professor nessa disciplina
			preferencia_disciplina = (*mapDiscPreferencia)[ id_disciplina ];

			// A nota acumulada no crit�rio de avalia��o segue o crit�rio de
			// 'quanto maior o valor, pior a solu��o'. Como a 'pior' prefer�ncia
			// tem nota 1 e a 'maior' prefer�ncia tem nota 10, um valor mais pr�ximo
			// de 1 deve contribuir mais para o valor da avalia��o no crit�rio atual
			nota_avaliacao = (10 - preferencia_disciplina);

			// Adiciona a avalia��o no somat�rio total
			nota_acumulada += nota_avaliacao;

			// Adiciona a avalia��o no somat�rio do professor
			preferenciasProfessor[i] += nota_avaliacao;
		}
	}

	totalPreferenciasProfessorDisciplina = nota_acumulada;
}

void Avaliador::avaliaCustoProfessorVirtual(SolucaoOperacional & solucao)
{
	// Total de cr�tidos atribu�dos a professores virtuais
	int creditos_virtuais = 0;

	// Total de professores virtuais
	int professores_virtuais = 0;

	unsigned int i = 0;
	int id_professor = 0;
	int linha_professor = 0;
	Professor* professor = NULL;
	Aula* aula = NULL;

	// Percorre o conjunto de professores da solu��o
	std::map<int, Professor*>::iterator it_professor
		= solucao.mapProfessores.begin();
	for (; it_professor != solucao.mapProfessores.end();
		   it_professor++)
	{
		// Recupera o objeto 'professor'
		id_professor = it_professor->first;
		professor = it_professor->second;
		linha_professor = it_professor->second->getIdOperacional();

		// Caso o professor seja um 'professor virtual'
		if (professor->eVirtual())
		{
			// Incrementa-se o n�mero de professores
			// virtuais utilizados na solu��o operacional
			professores_virtuais++;

			// Para o professor atual, contabilizamos o
			// n�mero de cr�ditos atribu�dos a ele na solu��o
			for (i = 0; i < solucao.getMatrizAulas()->at(linha_professor)->size(); i++)
			{
				// Recupera a aula atual
				aula = solucao.getMatrizAulas()->at(linha_professor)->at(i);
				if (aula != NULL && aula->eVirtual() == false)
				{
					// Incrementa o total de cr�ditos
					// atribu�dos a professor virtual na solu��o
					creditos_virtuais++;
				}
			}
		}
	}

	totalCreditosProfessoresVirtuais = creditos_virtuais;
	totalProfessoresVirtuais = professores_virtuais;
}
