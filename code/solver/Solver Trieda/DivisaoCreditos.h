#ifndef _DIVISAO_CREDITOS_H_
#define _DIVISAO_CREDITOS_H_

#include "ofbase.h"

class DivisaoCreditos :
   public OFBase
{
public:
   DivisaoCreditos( void );
   DivisaoCreditos( DivisaoCreditos const & );
   virtual ~DivisaoCreditos( void );

   virtual void le_arvore( ItemDivisaoCreditos & );

   int dia[ 8 ];

   void setCreditos( int value ) { this->creditos = value; }
   int getCreditos() const { return this->creditos; }

private:
   int creditos;
};

#endif