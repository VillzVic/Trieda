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
	//---------------- M�TODOS DE AVALIA��O DA SOLU��O ---------------//
	// M�todo que verifica quantas fixa��es
	// n�o foram atendidas na solu�ao operacional
	void calculaViolacaoRestricaoFixacao(SolucaoOperacional &);
	double totalViolacaoRestricaoFixacao;

	// M�todo que verifca quantas viola��es de tempo de
	// deslocamento entre campus e/ou unidades ocorreram na solu��o,
	// armazenando o total de tempo ocorrido nessas viola��es
	void calculaViolacoesDescolamento(SolucaoOperacional &);
	double totalViolacoesDescolamento;
	double totalTempoViolacoesDescolamento;
	//----------------------------------------------------------------//

	//------------------------- UTILIT�RIOS --------------------------//
	// Tempo, em minutos, de cada hor�rio de aula
	int MINUTOS_POR_HORARIO;

	// Calcula o tempo NECESS�RIO para
	// se deslocar entre uma aula e outra
	double calculaTempoEntreCampusUnidades(
		SolucaoOperacional& ,Campus*, Campus*, Unidade*, Unidade*);
	//----------------------------------------------------------------//
};
