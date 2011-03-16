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
	//---------------- MÉTODOS DE AVALIAÇÃO DA SOLUÇÃO ---------------//
	// Método que verifica quantas fixações
	// não foram atendidas na soluçao operacional
	double violacaoRestricaoFixacao(SolucaoOperacional &);

	// Método que verifca quantas violações de tempo de
	// deslocamento entre campus e/ou unidades ocorreram na solução
	double violacaoTempoDescolamentoViavel(SolucaoOperacional &);
	//----------------------------------------------------------------//

	//------------------------- UTILITÁRIOS --------------------------//
	// Tempo, em minutos, de cada horário de aula
	int MINUTOS_POR_HORARIO;

	// Calcula o tempo NECESSÁRIO para
	// se deslocar entre uma aula e outra
	double calculaTempoEntreCampusUnidades(
		SolucaoOperacional& ,Campus*, Campus*, Unidade*, Unidade*);
	//----------------------------------------------------------------//
};
