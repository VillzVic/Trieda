#ifndef _CURSO_H_
#define _CURSO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "TipoCurso.h"
#include "Curriculo.h"
#include "TipoContrato.h"

class Curso :
   public OFBase
{
public:
	Curso( void );
	virtual ~Curso( void );

	virtual void le_arvore( ItemCurso & );

	GGroup< int > area_ids;
	GGroup< Curriculo *, LessPtr< Curriculo > > curriculos;
	GGroup< int > equiv_ids; // equivalencias especificas

	TipoCurso * tipo_curso;

	std::pair< int, double > regra_min_mestres;
	std::pair< int, double > regra_min_doutores;
	bool temMinMestres ( void ) const { return (regra_min_mestres.second > 0.00001); }
	bool temMinDoutores ( void ) const { return (regra_min_doutores.second > 0.00001); }

	void setCodigo( std::string s ) { codigo = s; }
	void setTipoId( int v ) { tipo_id = v; }
	void setNumPeriodos( int v ) { num_periodos = v; }
	void setQtdMaxProfDisc( int v ) { qtd_max_prof_disc = v; }
	void setMaisDeUma( bool v ) { mais_de_uma = v; }
	void setMinTempoIntegral( double value ) { minTempoIntegral = value; }
	void setMinTempoIntegralParcial( double value ) { minTempoIntegralParcial = value; }

	std::string getCodigo() const { return codigo; }
	int getTipoId() const { return tipo_id; }
	int getNumPeriodos() const { return num_periodos; }
	int getQtdMaxProfDisc() const { return qtd_max_prof_disc; }
	bool getMaisDeUma() const { return mais_de_uma; }
	double getMinTempoIntegral() { return minTempoIntegral; }
	double getMinTempoIntegralParcial() { return minTempoIntegralParcial; }
	int getQtdMaxDiscNoPeriodoPorProf();
	double getMinPercContrato( TipoContrato *contr );

	bool temMinTempoIntegral() { return (minTempoIntegral > 0.000001); }
	bool temMinTempoIntegralParcial() { return (minTempoIntegralParcial > 0.000001); }

	bool possuiDisciplina( Disciplina *d );
	int getNumTotalDisciplinasNoCurso();
	bool possuiEquiv( int equivId );

	// todo: implementar!
	bool ehIncompativel(Curso* const curso) { return false; }

private:
   std::string codigo;
   int tipo_id;
   int num_periodos;
   int qtd_max_prof_disc;
   bool mais_de_uma;

   // Porcentagens mínimas de professores com regime
   // de dedicação por tempo integral e por tempo parcial
   double minTempoIntegral;
   double minTempoIntegralParcial;
};

#endif