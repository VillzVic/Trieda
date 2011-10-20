#ifndef _SALA_H_
#define _SALA_H_

#include "ofbase.h"
#include "GGroup.h"

#include "TipoSala.h"
#include "Horario.h"
#include "HorarioDia.h"
#include "CreditoDisponivel.h"

#include "Disciplina.h"

class Sala :
	public OFBase
{
public:
	Sala( void );
	virtual ~Sala( void );

	GGroup< Horario * > horarios_disponiveis;
	GGroup< CreditoDisponivel * > creditos_disponiveis;

   GGroup< HorarioDia * > horariosDia;

	// Alterar para ref, depois
	GGroup< int > disciplinas_associadas;

    // Conjunto de disciplinas que foram associadas a sala em questão pelo usuario.
    GGroup< Disciplina * > disciplinas_Associadas_Usuario;

    // Conjunto de disciplinas que foram associadas a sala em questão
    // por um pré processamento realizado. Aqui, as associãções são criadas
    // independentemente da unidade em que uma sala se encontra.
    GGroup< Disciplina * > disciplinas_Associadas_AUTOMATICA;

    // UNIAO dos conjuntos acima.
    GGroup< Disciplina * > disciplinasAssociadas;

    // Armazena os dias letivos para uma determinada sala
    GGroup< int > diasLetivos;
	TipoSala * tipo_sala;

	virtual void le_arvore( ItemSala & );
	int max_creds( int );

	// Métodos get
	int getTipoSalaId() const { return tipo_sala_id; }
	int getCapacidade() const { return capacidade; }
	int getIdUnidade() const { return id_unidade; }
	std::string getCodigo() const { return codigo; }
	std::string getAndar() const { return andar; }
	std::string getNumero() const { return numero; }

	// Métodos set
	void setTipoSalaId( int id ) { tipo_sala_id = id; }
	void setCapacidade( int cap ) { capacidade = cap; }
	void setIdUnidade( int id ) { id_unidade = id; }
	void setCodigo( std::string cod ) { codigo = cod; }
	void setAndar( std::string a ) { andar = a; }
	void setNumero( std::string n ) { numero  = n; }

	void construirCreditosHorarios( ItemSala &, std::string, bool );

    virtual bool operator < ( Sala & right ) const
    { 
       return ( getId() < right.getId() );
    }

    virtual bool operator == ( Sala & right ) const
    { 
		return ( getId() == right.getId() );
    }

private:
	std::string codigo;
	std::string andar;
   std::string numero;

	// Alterar para ref, depois
	int tipo_sala_id;

	int capacidade;
	int id_unidade;

	// Dado um conjunto de horários, retorna o conjunto de créditos correspondentes
	GGroup< CreditoDisponivel * > converteHorariosParaCreditos();
};

#endif
