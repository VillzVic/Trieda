#pragma once
#include "ofbase.h"

class TipoDisciplina :
   public OFBase
{
public:
   TipoDisciplina(void);
   ~TipoDisciplina(void);
   virtual void le_arvore(ItemTipoDisciplina& elem);

//private:
   std::string nome;
};
