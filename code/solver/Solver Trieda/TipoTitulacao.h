#pragma once
#include "ofbase.h"

class TipoTitulacao :
   public OFBase
{
public:
   TipoTitulacao(void);
   ~TipoTitulacao(void);
   virtual void le_arvore(ItemTipoTitulacao& elem);
//private:
   std::string nome;
};
