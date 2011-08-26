#ifndef _CALENDARIO_H_
#define _CALENDARIO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "Turno.h"

class Calendario :
   public OFBase
{
public:
   Calendario( void );
   virtual ~Calendario( void );

   GGroup< Turno *, LessPtr< Turno > > turnos;

   virtual void le_arvore( ItemCalendario & );

   void setCodigo( std::string s ) { codigo = s; }
   void setDescricao( std::string s ) { descricao = s; }
   void setTempoAula( int value ) { tempo_aula = value; }

   std::string getCodigo() { return codigo; }
   std::string getDescricao() { return descricao; }
   int getTempoAula() { return tempo_aula; }

private:   
   std::string codigo;
   std::string descricao;
   int tempo_aula;
};

#endif