#ifndef _CAMPUS_H_
#define _CAMPUS_H_

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
   Campus(void);
   virtual ~Campus(void);

   GGroup< Unidade *, LessPtr< Unidade > > unidades;
   GGroup< Professor * > professores;
   GGroup< Horario * > horarios;

   GGroup< Curso  *, LessPtr< Curso  > > cursos;
   GGroup< Oferta *, LessPtr< Oferta > > ofertas;

   // Armazena os dias letivos para um determinado campus
   GGroup< int > diasLetivos;

   std::map< int, // ConjuntoSalaId
	   GGroup< std::pair< Unidade *, ConjuntoSala * > > > conjutoSalas;

   virtual void le_arvore( ItemCampus & );

   void setCodigo(std::string s) { codigo = s; }
   void setNome(std::string s) { nome = s; }
   void setTotalSalas(int value) { totalSalas = value; }
   void setMaiorSala(int value) { maiorSala = value; }

   std::string getCodigo() { return codigo; }
   std::string getNome(std::string s) { return nome; }
   int getTotalSalas() { return totalSalas; }
   int getMaiorSala() { return maiorSala; }

private:
   std::string codigo;
   std::string nome;
   int totalSalas;
   int maiorSala;
};

#endif