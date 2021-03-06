#ifndef _TIPO_CURSO_H_
#define _TIPO_CURSO_H_

#include "ofbase.h"

class TipoCurso :
   public OFBase
{
public:
   TipoCurso( void );
   virtual ~TipoCurso( void );

   virtual void le_arvore( ItemTipoCurso & );

   void setNome( std::string s ) { this->nome  = s; }
   std::string getNome() const { return this->nome; }

private:
   std::string nome;
};

#endif
