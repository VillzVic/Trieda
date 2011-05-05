#ifndef _CURRICULO_H_
#define _CURRICULO_H_

#include "ofbase.h"
#include "GGroup.h"

class Curriculo :
   public OFBase
{
public:
   Curriculo();
   virtual ~Curriculo(void);

   virtual void le_arvore( ItemCurriculo & );

   GGroup< std::pair< int /*periodo*/, int /*disciplina_id*/ > > disciplinas_periodo;

   void setCodigo( std::string s ) { codigo = s; }
   std::string getCodigo() const { return codigo; }

private:
   std::string codigo;
};

#endif