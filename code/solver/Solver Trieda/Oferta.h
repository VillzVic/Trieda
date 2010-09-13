#pragma once
#include "ofbase.h"

class Oferta :
   public OFBase
{
public:
   Oferta(void);
   ~Oferta(void);

   int curriculo_id;
   int curso_id;
   int turno_id;
   int campus_id;

   void le_arvore(ItemOfertaCurso& elem);
};
