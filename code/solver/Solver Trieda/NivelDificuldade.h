#ifndef _NIVEL_DIFICULDADE_H_
#define _NIVEL_DIFICULDADE_H_

#include "ofbase.h"

class NivelDificuldade :
   public OFBase
{
public:
   NivelDificuldade(void);
   ~NivelDificuldade(void);
   
   virtual void le_arvore(ItemNivelDificuldade& raiz);

   void setNome(std::string s) { nome = s; }
   std::string getNome() { return nome; }

private:
   std::string nome;
};

#endif