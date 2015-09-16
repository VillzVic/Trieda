#pragma once
#include "ofbase.h"

class Oferecimento; 

#include "Oferecimento.h"

class Turma :
   public OFBase
{
public:
   Turma(void);
   ~Turma(void);

//private:
   std::string nome;
   GGroup<Oferecimento*> oferecimentos;

public:
   virtual void le_arvore(ItemTurma& elem);
};
