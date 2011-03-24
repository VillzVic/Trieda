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

	GGroup<Horario*> horarios;
	GGroup<Sala*> salas;
	GGroup<ConjuntoSala*> conjutoSalas;

    // Armazena os dias letivos em que, pelo
	// menos, uma sala possui algum crédito disponível
    GGroup<int> diasLetivos;

	virtual void le_arvore(ItemUnidade& elem);

    // =========== METODOS SET
	void setCodigo(std::string s) { codigo = s; }
	void setNome(std::string s) { nome = s; }
	void setMaiorSala(unsigned v) { maiorSala = v; }
	void setIdCampus(int id) { id_campus = id; }

	// =========== METODOS GET
	std::string getCodigo() { return codigo; }
	std::string getNome() { return nome; }
    unsigned getNumSalas() const { return salas.size(); }
	unsigned getMaiorSala() { return maiorSala; }
	int getIdCampus() { return id_campus; }

private:
	std::string codigo;
	std::string nome;
    unsigned maiorSala;
	int id_campus;
};
