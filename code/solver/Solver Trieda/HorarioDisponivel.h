#pragma once
#include "ofbase.h"

class HorarioDisponivel :
   public OFBase
{
public:
   HorarioDisponivel(void);
   ~HorarioDisponivel(void);
   virtual void le_arvore(ItemHorarioDisponivel& elem);
//private:
   int id;
   int semana;
};
