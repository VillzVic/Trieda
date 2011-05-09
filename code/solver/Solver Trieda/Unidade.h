#ifndef _UNIDADE_H_
#define _UNIDADE_H_

#include "ofbase.h"
#include "Sala.h"
#include "Horario.h"
#include "ConjuntoSala.h"

class Unidade :
	public OFBase
{
public:
	Unidade(void);
	virtual ~Unidade(void);

	GGroup< Horario * > horarios;
	GGroup< Sala *, LessPtr< Sala > > salas;
	GGroup< ConjuntoSala * > conjutoSalas;

    // Armazena os dias letivos em que, pelo
	// menos, uma sala possui algum crédito disponível
    GGroup< int > dias_letivos;

	virtual void le_arvore( ItemUnidade & );

	void setCodigo( std::string s ) { codigo = s; }
	void setNome( std::string s ) { nome = s; }
	void setMaiorSala( unsigned v ) { maior_sala = v; }
	void setIdCampus( int id ) { id_campus = id; }

	std::string getCodigo() { return codigo; }
	std::string getNome() { return nome; }
    unsigned getNumSalas() const { return salas.size(); }
	unsigned getMaiorSala() { return maior_sala; }
	int getIdCampus() { return id_campus; }

private:
	std::string codigo;
	std::string nome;
    unsigned maior_sala;
	int id_campus;
};

#endif