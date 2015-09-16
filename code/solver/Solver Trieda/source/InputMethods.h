#ifndef _INPUT_METHODS_H_
#define _INPUT_METHODS_H_ 

#include <iostream>
#include <stdio.h>

using namespace std;

class Campus;

class InputMethods
{
public:
	InputMethods(void);
	~InputMethods(void);

	virtual void foo() = 0;	// virtual function to make this an abstract class

	// obtem um campo de um xml entalado por tags. Exc: <campusId>49</campusId>
	// string
	static void getInlineAttrStr(std::string const &line, std::string const &tag, std::string &value);
	// int
	static void getInlineAttrInt(std::string const &line, std::string const &tag, int &value);
	// bool
	static void getInlineAttrBool(std::string const &line, std::string const &tag, bool &value);

	// excepcao no carregamento de um campo
	static void excCarregarCampo(std::string const &objeto, std::string const &atributo, std::string const &line);

	// Carregar Informação para objetos
	static void loadCampus(int const &id, Campus* &campus, std::string const &objeto);

	// Fake id
	static const int fakeId = -2987348;
	static const string fakeStrId;
};

#endif
