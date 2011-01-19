#pragma once
#include "ofbase.h"
#include "Oferta.h"
#include "Disciplina.h"

class Demanda :
   public OFBase
{
public:
   Demanda(void);

   Demanda(Demanda const & demanda);

   ~Demanda(void);

   virtual void le_arvore(ItemDemanda& elem);

   int quantidade;
   int oferta_id;
   int disciplina_id;

   Oferta* oferta;
   Disciplina* disciplina;

   bool operator ==(Demanda const & demanda);

};