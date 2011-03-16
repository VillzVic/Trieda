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

	// Métdos de avaliação da solução
	double violacaoRestricaoFixacao(SolucaoOperacional &);
	double violacaoTempoDescolamentoViavel(SolucaoOperacional &);

	// Métodos utilitários

	// Calcula o tempo que o professor precisa
	// para se deslocar entre uma aula e outra
	double calculaTempoEntreCampusUnidades(Campus*, Campus*, Unidade*, Unidade*);
};
