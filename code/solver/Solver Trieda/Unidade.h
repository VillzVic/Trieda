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

	//int maior_sala;
   unsigned maiorSala;


	// >>>
	//Campus *campus;
	int id_campus;
	// <<<

private:
      
   // >>> Variáveis e/ou estruturas de dados para realizar o pré processamento dos dados.
   // <<<

public:

   // =========== METODOS SET

   // =========== METODOS GET
   unsigned getNumSalas() const { return salas.size(); }


   // ToDo : implementar os métodos básicos para todos os membros da classe.

	virtual void le_arvore(ItemUnidade& elem);
};
