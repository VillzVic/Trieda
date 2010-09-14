#pragma once
#include "ofbase.h"

class NivelDificuldade :
   public OFBase
{
public:
   NivelDificuldade(void);
   ~NivelDificuldade(void);
   
   virtual void le_arvore(ItemNivelDificuldade& raiz);

   //private:
   std::string nome;
};
