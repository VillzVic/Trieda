#ifndef _SALA_H_
#define _SALA_H_

#include "ofbase.h"
#include "GGroup.h"

#include "TipoSala.h"
#include "Horario.h"
#include "HorarioDia.h"
#include "CreditoDisponivel.h"

#include "Disciplina.h"

#include "Trio.h"

class Sala :
	public OFBase
{
public:
	Sala( void );
	virtual ~Sala( void );

	GGroup< Horario * > horarios_disponiveis;

    GGroup< HorarioDia * > horariosDia;

	// Alterar para ref, depois
	GGroup< int > disciplinas_associadas;

    // Conjunto de disciplinas que foram associadas a sala em questão pelo usuario.
    GGroup< Disciplina * > disciplinas_Associadas_Usuario;

    // Conjunto de disciplinas que foram associadas a sala em questão
    // por um pré processamento realizado. Aqui, as associações são criadas
    // independentemente da unidade em que uma sala se encontra.
    GGroup< Disciplina * > disciplinas_Associadas_AUTOMATICA;

    // UNIAO dos conjuntos acima.
    GGroup< Disciplina * > disciplinasAssociadas;

    // Armazena os dias letivos para uma determinada sala
    GGroup< int > diasLetivos;
	TipoSala * tipo_sala;

	virtual void le_arvore( ItemSala & );
	int max_creds( int );
	int max_credsPorSL( int, Calendario* );

	// Métodos get
	int getTipoSalaId() const { return tipo_sala_id; }
	int getCapacidade() const { return capacidade; }
	int getIdUnidade() const { return id_unidade; }
	std::string getCodigo() const { return codigo; }
	std::string getAndar() const { return andar; }
	std::string getNumero() const { return numero; }
	int getTempoDispPorDia( int dia );
	int getTempoDispPorDiaSL( int dia, Calendario* sl );
	int getNroCredCombinaSL( int k, Calendario *c, int dia );
	std::map< Trio< int, int, Calendario* >, int > getCombinaCredSL() const { return combinaCredSL; }
	std::map< int /*dia*/, int /*size*/ > getCombinaCredSLSize() const { return combinaCredSLSize; }

	// Métodos set
	void setTipoSalaId( int id ) { tipo_sala_id = id; }
	void setCapacidade( int cap ) { capacidade = cap; }
	void setIdUnidade( int id ) { id_unidade = id; }
	void setCodigo( std::string cod ) { codigo = cod; }
	void setAndar( std::string a ) { andar = a; }
	void setNumero( std::string n ) { numero  = n; }
	void setCombinaCredSL( int dia, int k, Calendario* sl , int n );
	void setCombinaCredSLSize(int dia, int size) { combinaCredSLSize[dia] = size; }
	
	// Alocações na sala
	void addHorarioDiaOcupado( GGroup<HorarioDia*> hds );
	bool sobrepoeHorarioDiaOcupado( HorarioAula *ha, int dia );
	bool sobrepoeAulaJaAlocada( HorarioAula *hi, HorarioAula *hf, int dia );


	void removeCombinaCredSL( Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t );
	void construirCreditosHorarios( ItemSala &, std::string, bool );

	void calculaTempoDispPorDia();
	void calculaTempoDispPorDiaSL();
	int somaTempo( GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosAula);
	GGroup<HorarioAula*> retornaHorariosDisponiveisNoDiaPorSL( int dia, Calendario* sl );

	bool combinaCredSL_eh_dominado( int i, Calendario *sl1, int maxSL2, Calendario *sl2, int dia );
	bool combinaCredSL_domina( int i, Calendario *sl1, int j, Calendario *sl2, int dia );
	bool combinaCredSL_eh_repetido( int i, Calendario *sl1, int j, Calendario *sl2, int dia );
	std::map< Trio<int, int, Calendario*>, int > retornaCombinaCredSL_Dominados( int dia );

    virtual bool operator < ( Sala & right ) const
    { 
       return ( getId() < right.getId() );
    }

    virtual bool operator == ( Sala & right ) const
    { 
		return ( getId() == right.getId() );
    }

	// Associa a cada Calendario (sl) e tipo de combinação de creditos das semanas letivas,
	// o numero maximo de creditos possivel para cada dia da semana.
	std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >, int/*nroCreds*/ > combinaCredSL;

private:
	std::string codigo;
	std::string andar;
   std::string numero;

	// Alterar para ref, depois
	int tipo_sala_id;

	int capacidade;
	int id_unidade;
	GGroup<Calendario*> temposSL; // lista os possiveis Calendarios existentes, obtido a partir dos horarios disponiveis da sala
	std::map<int, int> tempoDispPorDia; // mapeia para cada dia, o tempo total disponivel na sala, tratando intersecoes de semanas letivas
	std::map< std::pair<int, Calendario*>, int> tempoDispPorDiaSL; // mapeia para cada par <dia, Calendario>, o tempo disponivel na sala.

	// Dado um conjunto de horários, retorna o conjunto de créditos correspondentes
	GGroup< CreditoDisponivel * > converteHorariosParaCreditos();
	
	void calculaTemposSL();
	void setTempoDispPorDia( int dia, int t ){ tempoDispPorDia[dia] = t; }
	void setTempoDispPorDiaSL( int dia, Calendario* sl, int t ){ tempoDispPorDiaSL[std::make_pair(dia,sl)] = t; }

	std::map< int /*dia*/, int /*size*/ > combinaCredSLSize;
	
    GGroup< HorarioDia*, LessPtr<HorarioDia> > horariosDiaOcupados; // aulas alocadas. Preenchido após alocado por modelo tático / integrado.
};

#endif
