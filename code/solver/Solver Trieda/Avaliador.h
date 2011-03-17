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

   // Retorna o valor de uma solução operacional
   double avaliaSolucao(SolucaoOperacional &);

private:
	//------------------ MÉTODOS DE AVALIAÇÃO DA SOLUÇÃO -----------------//
	// Avaliação de fixações não atendidas na soluçao
	void calculaViolacaoRestricaoFixacao(SolucaoOperacional &);
	double totalViolacaoRestricaoFixacao;

	// Avaliação de violações de tempo de deslocamento
	// entre campus e/ou unidades ocorreram na solução,
	// armazenando o total de tempo ocorrido nessas violações
	void calculaViolacoesDescolamento(SolucaoOperacional &);
	double totalViolacoesDescolamento;
	double totalTempoViolacoesDescolamento;

	// Avaliação de gap's nos horários dos professores
	// --> Cada Professor possui um conjunto de gap's
	void calculaGapsHorariosProfessores(SolucaoOperacional &);
	vector<vector<int>> gapsProfessores;
	double totalGapsHorariosProfessores;
	//--------------------------------------------------------------------//

	//--------------------------- UTILITÁRIOS ----------------------------//
	// Tempo, em minutos, de cada horário de aula
	int MINUTOS_POR_HORARIO;

	// Calcula o tempo NECESSÁRIO para
	// se deslocar entre uma aula e outra
	double calculaTempoEntreCampusUnidades(
		SolucaoOperacional& ,Campus*, Campus*, Unidade*, Unidade*);

	// Informa quantos horários um determinado
	// professor tem disponíveis em um intervalo de aulas
	int horariosDisponiveisIntervalo(Professor*, int, Horario*, Horario*);
	//--------------------------------------------------------------------//
};

#endif