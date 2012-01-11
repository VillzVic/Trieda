#ifndef _CAMPUS_H_
#define _CAMPUS_H_

#include <iostream>

#include "ofbase.h"
#include "Unidade.h"
#include "Professor.h"
#include "Horario.h"
#include "Curso.h"

class Oferta;

class Campus:
   public OFBase
{
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

   std::string getCodigo() const { return this->codigo; }
   std::string getNome() const { return this->nome; }
   int getTotalSalas() const { return this->totalSalas; }
   int getMaiorSala() const { return this->maiorSala; }
   double getCusto() const { return this->custo; }

   GGroup<Oferta*, LessPtr<Oferta>> retornaOfertasComCursoDisc( int idCurso, int idDisc );

private:
   std::string codigo;
   std::string nome;
   int totalSalas;
   int maiorSala;
   double custo;
};

#endif
