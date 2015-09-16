#ifndef _GERADOR_COMBS_DIV_
#define _GERADOR_COMBS_DIV_

#include "UtilHeur.h"

class Disciplina;
class Calendario;

class GeradorCombsDiv
{
public:
	GeradorCombsDiv(Disciplina* const disciplina);
	~GeradorCombsDiv(void);

	// gera combina��es de divis�es. adiciona �s estrutura disciplina->combinacoes_divisoes_creditos
	void gerarDivisoes(void);

	// print maximo numero de combina��es encontrado e para quantos cr�ditos foi
	static void printMaxCombsInfo(void);

private:
	// info
	Disciplina* const disciplina_;
	const int nrCreds_;
	unordered_map<int, int> maxCredsDia_;
	int minCredsDia_;
	int maxDias_;
	int nrCombs_;
	bool set_;

	// nova vers�o. nr cr�ditos permitidos
	set<int> nrCredsPermitidos_;
	
	// setup
	void setCredsPermitidos_(bool all);
	void setMaxCredsCalendarios_(bool div = false);
	void setCredsPermDivisoes_(void);
	void setCredsPermMinMax_(void);

	// funcoes
	void fixaGeraCombs(int dia, int sumCreds, int nrDias, unordered_map<int, int> const &comb, vector<vector<pair<int, int>>> &combinacoes); 
	void addComb_(vector<vector<pair<int, int>>> &combinacoes, unordered_map<int, int> const &comb);

	void printMaxCreds_(void);

	// nr creditos -> nr combinacoes
	static int maxCombs;
	static int maxCombsNrCreds;
};

#endif

