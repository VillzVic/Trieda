#ifndef _CURRICULO_H_
#define _CURRICULO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "Disciplina.h"
#include "Calendario.h"

class Curriculo :
   public OFBase
{
public:
   Curriculo( void );
   virtual ~Curriculo( void );

   Calendario * calendario;

   virtual void le_arvore( ItemCurriculo & );

   GGroup< std::pair< int, Disciplina * > > disciplinas_periodo;

   void setCodigo( std::string s ) { codigo = s; }
   void setSemanaLetivaId( int value ) { semanaLetivaId = value; }

   std::string getCodigo() const { return codigo; }
   int getSemanaLetivaId() const { return semanaLetivaId; }
   int getMaxCreds( int dia );

   int getPeriodo( Disciplina *d );

   void refDisciplinaPeriodo( GGroup< Disciplina *, LessPtr< Disciplina > > );

   bool possuiDisciplina( int idDisciplina );
   
   GGroup< Calendario*, LessPtr<Calendario> > retornaSemanasLetivasNoPeriodo( int periodo );

private:
   GGroup< std::pair< int, int > > ids_disciplinas_periodo;
   std::string codigo;
   int semanaLetivaId;
};

#endif