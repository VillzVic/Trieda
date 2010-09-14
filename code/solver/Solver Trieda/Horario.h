#pragma once
#include "ofbase.h"

class Horario :
   public OFBase
{
public:
   Horario(void);
   ~Horario(void);
   
   virtual void le_arvore(ItemHorario& raiz);

   virtual int getId();

   GGroup<int> dias_semana;
   int horarioAulaId;
   int turnoId;

private:
   int id;

};
