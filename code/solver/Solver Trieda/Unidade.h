#pragma once
#include "ofbase.h"
#include "Sala.h"
#include "Horario.h"

class Unidade :
	public OFBase
{
public:
	Unidade(void);
	~Unidade(void);

	//private:
	std::string codigo;
	std::string nome;

	GGroup<Horario*> horarios;
	GGroup<Sala*> salas;

	int maior_sala;


	// >>>
	//Campus *campus;
	int id_campus;
	// <<<


public:
	virtual void le_arvore(ItemUnidade& elem);
};
