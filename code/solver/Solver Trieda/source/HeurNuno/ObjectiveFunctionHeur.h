#ifndef _OBJECTIVE_FUNCTION_HEUR_H_
#define _OBJECTIVE_FUNCTION_HEUR_H_

#include "UtilHeur.h"

class ObjectiveFunctionHeur
{
public:
	ObjectiveFunctionHeur(void);
	~ObjectiveFunctionHeur(void);

	double getProfit() const { return profit_; }
	void addProfit(double inc) { profit_ += inc; }

	double getUtilizacaoMediaSalas () const { return utilizacaoMediaSalas_; }
	void addUtilizacaoSala (int nrHoras, double utilizacao) 
	{ 
		nrHorasAlocadas_ += nrHoras;
		utilizacaoTotal_ += utilizacao;

		if(nrHorasAlocadas_ != 0)
			utilizacaoMediaSalas_ = utilizacaoTotal_/nrHorasAlocadas_;
		else
			utilizacaoMediaSalas_ = 0;
	}
	void removeUtilizacaoSala(int nrHoras, double utilizacao) 
	{ 
		nrHorasAlocadas_ -= nrHoras;
		utilizacaoTotal_ -= utilizacao;

		if(nrHorasAlocadas_ < 0 || utilizacaoTotal_ < 0)
			throw "Valores negativos não suportados!";

		if(nrHorasAlocadas_ != 0)
			utilizacaoMediaSalas_ = utilizacaoTotal_/nrHorasAlocadas_;
		else
			utilizacaoMediaSalas_ = 0;
	}

private:

	double profit_;
	double utilizacaoMediaSalas_;			// % capacidade
	double utilizacaoTotal_;
	int nrHorasAlocadas_;
};

#endif

