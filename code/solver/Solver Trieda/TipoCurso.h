#pragma once
#include "ofbase.h"

class TipoCurso :
   public OFBase
{
public:
   TipoCurso(void);
   ~TipoCurso(void);

   virtual void le_arvore(ItemTipoCurso& elem);

   void setNome(std::string s) { nome  = s; }
   std::string getNome() const { return nome; }

private:
   std::string nome;
};
