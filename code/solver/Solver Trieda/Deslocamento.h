#pragma once
#include "ofbase.h"

class Deslocamento :
   public OFBase
{
public:
   Deslocamento(void);
   ~Deslocamento(void);

   void le_arvore(ItemDeslocamento& elem);

   int origem_id;
   int destino_id;
   int tempo;
   double custo;
};
