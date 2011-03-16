#include "ProblemData.h"
#include "SolucaoOperacional.h"

class Avaliador
{
public:
   Avaliador(void);
   virtual ~Avaliador(void);

   double avaliaSolucao(SolucaoOperacional &);

private:
	int MINUTOS_POR_HORARIO;

	// M�tdos de avalia��o da solu��o
	double violacaoRestricaoFixacao(SolucaoOperacional &);
	double violacaoTempoDescolamentoViavel(SolucaoOperacional &);

	// M�todos utilit�rios

	// Calcula o tempo que o professor precisa
	// para se deslocar entre uma aula e outra
	double calculaTempoEntreCampusUnidades(Campus*, Campus*, Unidade*, Unidade*);
};
