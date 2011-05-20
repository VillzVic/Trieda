#ifndef _CURRICULO_H_
#define _CURRICULO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "Disciplina.h"

class Curriculo :
   public OFBase
{
public:
   Curriculo();
   virtual ~Curriculo(void);

   virtual void le_arvore( ItemCurriculo & );

   GGroup< std::pair< int, Disciplina * > > disciplinas_periodo;

   void setCodigo( std::string s ) { codigo = s; }
   std::string getCodigo() const { return codigo; }

   void refDisciplinaPeriodo( GGroup< Disciplina *, LessPtr< Disciplina > > );

private:
   GGroup< std::pair< int, int > > ids_disciplinas_periodo;
   std::string codigo;
};

#endif