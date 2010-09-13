#pragma once
#include "ofbase.h"
#include "DateTime.h"

class HorarioAula :
   public OFBase
{
public:
   HorarioAula(void);
   ~HorarioAula(void);
   virtual void le_arvore(ItemHorarioAula& elem);

//private:
   int id;
   DateTime inicio;
   GGroup<int> diasSemana;
};
