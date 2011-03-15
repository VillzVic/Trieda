#include "SolucaoOperacional.h"

class Avaliador
{
public:
   Avaliador(void);
   virtual ~Avaliador(void);

   double avaliaSolucao(SolucaoOperacional &);

private:
	double violacaoRestricaoFixacao(SolucaoOperacional &, GGroup<Fixacao*>);
};
