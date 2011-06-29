#ifndef _HORARIO_DISPONIVEL_H_
#define _HORARIO_DISPONIVEL_H_
#include "ofbase.h"

class HorarioDisponivel :
   public OFBase
{
public:
   HorarioDisponivel(void);
   ~HorarioDisponivel(void);
   virtual void le_arvore(ItemHorarioDisponivel &);
//private:
   int id;
   int semana;
};


#endif
