#pragma once
#include "ofbase.h"

class TipoCurso :
   public OFBase
{
public:
   TipoCurso(void);
   ~TipoCurso(void);
   virtual void le_arvore(ItemTipoCurso& elem);
//private:
   std::string nome;
};
