#include "RestricaoViolada.h"

RestricaoViolada::RestricaoViolada()
{
	restricao = "";
	unidade = "";
	valor = 0.0;
}

RestricaoViolada::~RestricaoViolada()
{

}

bool RestricaoViolada::operator < ( const RestricaoViolada & right )
{
	if ( restricao != right.restricao )
   {
		return ( restricao < right.restricao );
   }
   else
   {
      return false;
   }
}

std::ostream & operator << ( std::ostream & out, RestricaoViolada & right )
{
	out << "<restricaoViolada>\n";
	out << "<restricao>" << right.getRestricao().c_str() << "</restricao>\n";
	out << "<valor>" << right.getValor() << "</valor>\n";
	out << "<unidade>" << right.getUnidade().c_str() << "</unidade>\n";
	out << "</restricaoViolada>\n";

	return out;
}
