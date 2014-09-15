#ifndef _ALOCAR_EQUIV_H_
#define _ALOCAR_EQUIV_H_

//#include "UtilHeur.h"
//#include "AlunoHeur.h"
#include <map>

using namespace std;

class SolucaoHeur;
class OfertaDisciplina;
class AlunoHeur;
class TurmaHeur;

// Método heurístico de tentar alocar alunos com demanda equivalente a turmas já abertas
class AlocarEquiv
{
public:
	AlocarEquiv(SolucaoHeur* const solucao, bool mudarSalas, bool priorUm = false);
	~AlocarEquiv(void);

	// Versão em teste!
	void tryAlocEquiv(void);

private:
	SolucaoHeur* solucao_;	// solução a ser trabalhada
	bool mudarSalas_;		// indica se é permitido mudar a sala ou não
	bool mudarSalasFlag_;	// flag que activa a mudança de salas
	bool priorUm_;			// também considerar prioridade um ?

};

#endif
