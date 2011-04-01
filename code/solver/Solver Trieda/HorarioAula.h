#ifndef _HORARIO_AULA_H_
#define _HORARIO_AULA_H_

#include "ofbase.h"
#include "GGroup.h"
#include "DateTime.h"

class HorarioAula :
   public OFBase
{
public:
   HorarioAula(void);
   ~HorarioAula(void);

   virtual void le_arvore(ItemHorarioAula& elem);

   GGroup<int> dias_semana;

   DateTime getInicio() const { return inicio; }
   void setInicio(DateTime dt) { inicio = dt; }

private:
   DateTime inicio;
};

#endif