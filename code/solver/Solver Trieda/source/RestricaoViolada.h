#ifndef _RESTRICAO_VIOLADA_H_
#define _RESTRICAO_VIOLADA_H_

#include <ostream>
#include <string>

#include "GGroup.h"

class RestricaoViolada
{
public:
	RestricaoViolada();
	virtual ~RestricaoViolada();

   void setRestricao( std::string aRestricao ) { this->restricao = aRestricao; }
   void setUnidade( std::string aUnidade ) { this->unidade = aUnidade; }
   void setValor( double aValor ) { this->valor = aValor; }

   std::string getRestricao() const { return this->restricao; }
   std::string getUnidade() const { return this->unidade; }
   double getValor() const { return this->valor; }

	bool operator < ( const RestricaoViolada & );

private:
	std::string restricao;
	double valor;
	std::string unidade;
};

std::ostream & operator << ( std::ostream & , RestricaoViolada & );

typedef GGroup< RestricaoViolada *, LessPtr< RestricaoViolada > > RestricaoVioladaGroup;

#endif
