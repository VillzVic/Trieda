#ifndef _RESTRICAO_VIOLADA_H_
#define _RESTRICAO_VIOLADA_H_

#include <ostream>
#include <string>

class RestricaoViolada
{
public:
	RestricaoViolada() : restricao(""), unidade(""), valor(0.0) {}

	void setRestricao(const std::string& value) { restricao = value; }
	void setUnidade(const std::string& value) { unidade = value; }
	void setValor(double value) { valor = value; }

	std::string getRestricao() const { return restricao; }
	std::string getUnidade() const { return unidade; }
	double getValor() const { return valor; }

	bool operator<(const RestricaoViolada& right) const { return (restricao < right.restricao); }

private:
	std::string restricao;
	std::string unidade;
	double valor;
};

std::ostream& operator<<(std::ostream& out, RestricaoViolada& right)
{
	out << "<restricaoViolada>\n";
	out << "<restricao>" << right.getRestricao().c_str() << "</restricao>\n";
	out << "<valor>" << right.getValor() << "</valor>\n";
	out << "<unidade>" << right.getUnidade().c_str() << "</unidade>\n";
	out << "</restricaoViolada>\n";

	return out;
}

typedef GGroup<RestricaoViolada *, Less<RestricaoViolada*>> RestricaoVioladaGroup;

#endif
