#pragma once
#include "ofbase.h"

class Fixacao :
   public OFBase
{
public:
   Fixacao(void);
   ~Fixacao(void);

   int professor_id;
   int disciplina_id;
   int sala_id;
   int dia_semana;
   int turno_id;
   int horario_id;

   void le_arvore(ItemFixacao& elem);
};
