#pragma once
#include "ofbase.h"

class DivisaoCreditos :
   public OFBase
{
public:
   DivisaoCreditos(void);
   
   ~DivisaoCreditos(void);

   DivisaoCreditos(DivisaoCreditos const & div_Creds);

   virtual void le_arvore(ItemDivisaoCreditos& elem);
//private:
   int creditos;
   int dia[8];
};
