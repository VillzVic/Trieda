#include "Avaliador.h"

Avaliador::Avaliador()
{
}

Avaliador::~Avaliador()
{
}

double Avaliador::avaliaSolucao(SolucaoOperacional & solucao)
{
	double funcaoObjetivo = 0.0;

	GGroup<Fixacao*> fixacoes;
	funcaoObjetivo += violacaoRestricaoFixacao(solucao, fixacoes);

	return funcaoObjetivo;
}

double Avaliador::violacaoRestricaoFixacao(SolucaoOperacional & solucao, GGroup<Fixacao*> fixacoes)
{
	double numViolacoes = 0.0;

	// TODO

	return numViolacoes;
}
