#ifndef _AVALIADOR_H_
#define _AVALIADOR_H_

#include <vector>
#include "ProblemData.h"
#include "SolucaoOperacional.h"

class Avaliador
{
public:
   Avaliador(void);
   virtual ~Avaliador(void);

   // Retorna o valor de uma solu��o operacional
   double avaliaSolucao(SolucaoOperacional &);

private:
	//------------------ M�TODOS DE AVALIA��O DA SOLU��O -----------------//
	// Avalia��o de fixa��es n�o atendidas na solu�ao
	void calculaViolacaoRestricaoFixacao(SolucaoOperacional &);
	double totalViolacaoRestricaoFixacao;

	// Avalia��o de viola��es de tempo de deslocamento
	// entre campus e/ou unidades ocorreram na solu��o,
	// armazenando o total de tempo ocorrido nessas viola��es
	void calculaViolacoesDescolamento(SolucaoOperacional &);
	double totalViolacoesDescolamento;
	double totalTempoViolacoesDescolamento;

	// Avalia��o de gap's nos hor�rios dos professores
	// --> Cada Professor possui um conjunto de gap's
	void calculaGapsHorariosProfessores(SolucaoOperacional &);
	vector<vector<int>> gapsProfessores;
	double totalGapsHorariosProfessores;
	//--------------------------------------------------------------------//

	//--------------------------- UTILIT�RIOS ----------------------------//
	// Tempo, em minutos, de cada hor�rio de aula
	int MINUTOS_POR_HORARIO;

	// Calcula o tempo NECESS�RIO para
	// se deslocar entre uma aula e outra
	double calculaTempoEntreCampusUnidades(
		SolucaoOperacional& ,Campus*, Campus*, Unidade*, Unidade*);

	// Informa quantos hor�rios um determinado
	// professor tem dispon�veis em um intervalo de aulas
	int horariosDisponiveisIntervalo(Professor*, int, Horario*, Horario*);
	//--------------------------------------------------------------------//
};

#endif