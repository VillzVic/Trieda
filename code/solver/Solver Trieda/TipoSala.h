#pragma once
#include "ofbase.h"

class TipoSala :
   public OFBase
{
public:
   TipoSala(void);
   ~TipoSala(void);
   virtual void le_arvore(ItemTipoSala& elem);
//private:
   std::string nome;
   std::string descricao;
};
