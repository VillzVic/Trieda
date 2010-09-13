#pragma once
#include "ofbase.h"

typedef std::pair<int,int> DisciplinaPeriodo;

class Curriculo :
   public OFBase
{
public:
   Curriculo();
   ~Curriculo(void);
   virtual void le_arvore(ItemCurriculo& elem);

//private:

   std::string codigo;
   GGroup<DisciplinaPeriodo> disciplinas_periodo;
};
