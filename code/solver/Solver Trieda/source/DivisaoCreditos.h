#ifndef _DIVISAO_CREDITOS_H_
#define _DIVISAO_CREDITOS_H_

#include "ofbase.h"

class DivisaoCreditos :
   public OFBase
{
public:
   DivisaoCreditos( void );
   DivisaoCreditos( DivisaoCreditos const & );
   DivisaoCreditos( int dia1, int dia2, int dia3, int dia4, int dia5, int dia6, int dia7, int idValue = --fakeId );
   virtual ~DivisaoCreditos( void );

   virtual void le_arvore( ItemDivisaoCreditos & );

   int dia[ 8 ];

   void setCreditos( int value ) { this->creditos = value; }
   int getCreditos() const { return this->creditos; }

private:
   int creditos;

   static int fakeId;
};

#endif