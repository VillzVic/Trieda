#ifndef _HORARIO_AULA_H_
#define _HORARIO_AULA_H_

#include "GGroup.h"
#include "ofbase.h"
#include "DateTime.h"

class HorarioAula :
   public OFBase
{
public:
   HorarioAula(void);
   ~HorarioAula(void);

   virtual void le_arvore(ItemHorarioAula& elem);

   GGroup<int> diasSemana;

   DateTime getInicio() { return inicio; }
   void setInicio(DateTime dt) { inicio = dt; }

private:
   DateTime inicio;
};

#endif