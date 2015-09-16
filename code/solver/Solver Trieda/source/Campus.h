#ifndef _CAMPUS_H_
#define _CAMPUS_H_

#include <iostream>
#include <vector>

#include "ofbase.h"
#include "Unidade.h"
#include "Professor.h"
#include "Horario.h"
#include "Curso.h"

class Oferta;
class ConjUnidades;

class Campus:
   public OFBase
{
	friend class ClusterUnidades;
public:
   Campus( void );
   virtual ~Campus( void );

   GGroup< Unidade *, LessPtr< Unidade > > unidades;
   GGroup< Professor *, LessPtr< Professor > > professores;
   GGroup< Horario * > horarios;
   
   GGroup< Curso  *, LessPtr< Curso  > > cursos;
   GGroup< Oferta *, LessPtr< Oferta > > ofertas;

   // Armazena os dias letivos para um determinado campus
   GGroup< int > diasLetivos;

   std::map< int, // ConjuntoSalaId
	   GGroup< std::pair< Unidade *, ConjuntoSala * > > > conjutoSalas;

   virtual void le_arvore( ItemCampus & );

   void setCodigo( std::string s ) { this->codigo = s; }
   void setNome( std::string s ) { this->nome = s; }
   void setTotalSalas( int value ) { this->totalSalas = value; }
   void setMaiorSala( int value ) { this->maiorSala = value; }
   void setCusto( double c ) { this->custo = c; }
   void setPossuiAlunoFormando( bool value ) { this->possuiAlunoFormando = value; }

   std::string getCodigo() const { return this->codigo; }
   std::string getNome() const { return this->nome; }
   int getTotalSalas() const { return this->totalSalas; }
   int getMaiorSala() const { return this->maiorSala; }
   double getCusto() const { return this->custo; }
   bool getPossuiAlunoFormando() const { return this->possuiAlunoFormando; }

   GGroup<Calendario*> getCalendarios();

   GGroup<Oferta*, LessPtr<Oferta>> retornaOfertasComCursoDisc( int idCurso, Disciplina *d );

   int nrClustersUnidades (void) const { return (int) clustersUnidades.size(); }
   vector<ConjUnidades*> getClustersUnids(void) const { return clustersUnidades; }

private:
   std::string codigo;
   std::string nome;
   int totalSalas;
   int maiorSala;
   double custo; // custo semanal de 1 credito no campus.
   bool possuiAlunoFormando;

   // clusters de unidades (usado na heuristica). Não mexer!
   vector<ConjUnidades*> clustersUnidades;
};

#endif
