#pragma once
#include "ofbase.h"
#include "TipoSala.h"

class Sala :
   public OFBase
{
public:
   Sala(void);
   ~Sala(void);
//private:
   TipoSala* tipo;
   std::string codigo;
   int num_salas;
   std::string andar;
   int capacidade;
public:
   virtual void le_arvore(ItemSala& elem);
};
