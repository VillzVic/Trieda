#include <sstream>
#include <boost/algorithm/string.hpp>
#include "InputMethods.h"
#include "HeurNuno/HeuristicaNuno.h"
#include "CentroDados.h"


const string InputMethods::fakeStrId = "fakeId";

InputMethods::InputMethods(void)
{
}


InputMethods::~InputMethods(void)
{
}

// obtem um campo de um xml entalado por tags. Exc: <campusId>49</campusId>
// string
void InputMethods::getInlineAttrStr(std::string const &line, std::string const &tag, std::string &value)
{
	value = line;
	size_t pos;

	// remover tag
	if( (pos = value.find(tag)) != string::npos )
		value.erase(pos, tag.length());
	else
	{
		stringstream ss;
		ss << "Tag " << tag << " nao encontrada!";
		cout << "[WARN: InputMethods::getInlineValue] " << ss.str() << endl;
		return;
	}

	// remover endtag
	std::string endTag(tag);
	endTag.insert(1, "/");
	if( (pos = value.find(endTag)) != string::npos )
		value.erase(pos, endTag.length());
	else
	{
		stringstream ss;
		ss << "Endtag " << endTag << " nao encontrada!";
		cout << "[WARN: InputMethods::getInlineValue] " << ss.str() << endl;
		return;
	}

	// remover espaços brancos
	boost::erase_all(value, " ");
}

// int
void InputMethods::getInlineAttrInt(std::string const &line, std::string const &tag, int &value)
{
	// get em string
	std::string valueStr;
	getInlineAttrStr(line, tag, valueStr);
	// converter em int
	value = std::stoi(valueStr);
}

// bool
void InputMethods::getInlineAttrBool(std::string const &line, std::string const &tag, bool &value)
{
	// get em string
	std::string valueStr;
	getInlineAttrStr(line, tag, valueStr);
	// verificar se tem 'true' ou 'false'
	if(valueStr.find("true") != string::npos)
	{
		value = true;
		return;
	}
	else if(valueStr.find("false") != string::npos)
	{
		value = false;
		return;
	}
	
	// verificar valor inteiro (0 - false, 1 - true)
	int valInt = std::stoi(valueStr);
	if(valInt == 1)
	{
		value = true;
		return;
	}
	else if(valInt == 0)
	{
		value = false;
		return;
	}

}

// excepcao no carregamento de um campo
void InputMethods::excCarregarCampo(std::string const &objeto, std::string const &atributo, std::string const &line)
{
	std::stringstream ss;
	ss << "[EXC: " << objeto << "] Campo: " << atributo << endl;
	ss << "Linha: " << line << endl;

	throw ss.str();
}

// Carregar Informação para objetos
void InputMethods::loadCampus(int const &id, Campus* &campus, std::string const &objeto)
{
	
}