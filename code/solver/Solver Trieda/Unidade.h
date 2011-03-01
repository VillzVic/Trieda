#pragma once
#include "ofbase.h"
#include "Sala.h"
#include "Horario.h"

#include "ConjuntoSala.h"

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

   GGroup<ConjuntoSala*> conjutoSalas;

   unsigned maiorSala;

	int id_campus;

   /* Armazena os dias letivos em que, pelo menos, uma sala possui algum cr�dito dispon�vel */
   GGroup<int> diasLetivos;

private:
      
   // Vari�veis e/ou estruturas de dados para realizar o pr� processamento dos dados.

public:

   // =========== METODOS SET

   // =========== METODOS GET
   unsigned getNumSalas() const { return salas.size(); }


   // ToDo : implementar os m�todos b�sicos para todos os membros da classe.

	virtual void le_arvore(ItemUnidade& elem);
};
