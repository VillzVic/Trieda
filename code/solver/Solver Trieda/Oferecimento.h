#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

class Oferecimento :
   public OFBase
{
public:
   Oferecimento(void);
   ~Oferecimento(void);
public:
   virtual void escreve_arvore(ItemAtendimento& elem);
};
