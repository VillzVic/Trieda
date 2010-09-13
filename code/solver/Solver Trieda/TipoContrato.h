#pragma once
#include "ofbase.h"

class TipoContrato :
   public OFBase
{
public:
   TipoContrato(void);
   ~TipoContrato(void);
   virtual void le_arvore(ItemTipoContrato& elem);
//private:
   std::string nome;
};
