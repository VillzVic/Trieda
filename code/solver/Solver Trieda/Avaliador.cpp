#include "Avaliador.h"
#include "ofbase.h"
#include "Fixacao.h"

Avaliador::Avaliador()
{
}

Avaliador::~Avaliador()
{
}

double Avaliador::avaliaSolucao(SolucaoOperacional & solucao)
{
	double funcaoObjetivo = 0.0;

	funcaoObjetivo += violacaoRestricaoFixacao(solucao);

	return funcaoObjetivo;
}

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

	return numViolacoes;
}
