#pragma once
#include "ofbase.h"

class AreaTitulacao :
   public OFBase
{
public:
   AreaTitulacao(void);
   ~AreaTitulacao(void);

   virtual void le_arvore(ItemAreaTitulacao& elem);

//private:
   std::string nome;
};
