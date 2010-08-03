#pragma once
#include "ofbase.h"
#include "HorarioAula.h"

class Turno :
   public OFBase
{
public:
   Turno(void);
   ~Turno(void);
   virtual void le_arvore(ItemTurno& elem);

//private:
     std::string nome;
   GGroup<HorarioAula*> horarios_aula;
};
