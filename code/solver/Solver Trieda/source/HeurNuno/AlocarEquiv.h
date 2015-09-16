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

// M�todo heur�stico de tentar alocar alunos com demanda equivalente a turmas j� abertas
class AlocarEquiv
{
public:
	AlocarEquiv(SolucaoHeur* const solucao, bool mudarSalas, bool priorUm = false);
	~AlocarEquiv(void);

	// Vers�o em teste!
	void tryAlocEquiv(void);

private:
	SolucaoHeur* solucao_;	// solu��o a ser trabalhada
	bool mudarSalas_;		// indica se � permitido mudar a sala ou n�o
	bool mudarSalasFlag_;	// flag que activa a mudan�a de salas
	bool priorUm_;			// tamb�m considerar prioridade um ?

};

#endif
