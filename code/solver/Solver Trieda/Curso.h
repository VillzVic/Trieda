#ifndef _CURSO_H_
#define _CURSO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "TipoCurso.h"
#include "Curriculo.h"

class Curso :
   public OFBase
{
public:
   Curso( void );
   virtual ~Curso( void );

   virtual void le_arvore( ItemCurso & );

   GGroup< int > area_ids;
   GGroup< Curriculo *, LessPtr< Curriculo > > curriculos;

   TipoCurso * tipo_curso;

   std::pair< int, double > regra_min_mestres;
   std::pair< int, double > regra_min_doutores;

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

   bool possuiDisciplina( Disciplina *d );

private:
   std::string codigo;
   int tipo_id;
   int num_periodos;
   int qtd_max_prof_disc;
   bool mais_de_uma;

   // Porcentagens m�nimas de professores com regime
   // de dedica��o por tempo integral e por tempo parcial
   double minTempoIntegral;
   double minTempoIntegralParcial;
};

#endif