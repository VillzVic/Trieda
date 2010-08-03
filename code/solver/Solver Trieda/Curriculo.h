#pragma once
#include "ofbase.h"

class Curriculo :
   public OFBase
{
public:
   Curriculo(void);
   ~Curriculo(void);
   virtual void le_arvore(ItemCurriculo& elem);

//private:

   std::string codigo;
   std::string descricao;
};
