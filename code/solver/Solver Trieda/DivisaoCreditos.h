#ifndef _DIVISAO_CREDITOS_H_
#define _DIVISAO_CREDITOS_H_

#include "ofbase.h"

class DivisaoCreditos :
   public OFBase
{
public:
   DivisaoCreditos(void);   
   ~DivisaoCreditos(void);
   DivisaoCreditos(DivisaoCreditos const & div_Creds);

   virtual void le_arvore(ItemDivisaoCreditos& elem);

   int dia[8];

   void setCreditos(int value) { creditos = value; }
   int getCreditos() const { return creditos; }

private:
   int creditos;
};

#endif