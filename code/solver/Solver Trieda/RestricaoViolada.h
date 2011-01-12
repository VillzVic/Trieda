#ifndef RESTRICAOVIOLADA_H_
#define RESTRICAOVIOLADA_H_

#include <ostream>
#include <string>
#include "GGroup.h"

class RestricaoViolada
{
public:
	RestricaoViolada();
	~RestricaoViolada();

   void setRestricao(std::string aRestricao) { restricao = aRestricao; }
   void setUnidade(std::string aUnidade) { unidade = aUnidade; }
   void setValor(double aValor) { valor = aValor; }

   std::string getRestricao() const { return restricao; }
   std::string getUnidade() const { return unidade; }
   double getValor() const { return valor; }

	bool operator <(RestricaoViolada& right);

private:
	std::string restricao;
	double valor;
	std::string unidade;
};

std::ostream& operator <<(std::ostream& out, RestricaoViolada& right);

typedef GGroup<RestricaoViolada*, LessPtr<RestricaoViolada> > RestricaoVioladaGroup;

#endif