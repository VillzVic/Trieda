#pragma once
#include "ofbase.h"
#include "Turno.h"

class Calendario :
   public OFBase
{
public:
   Calendario(void);
   ~Calendario(void);
   virtual void le_arvore(ItemCalendario& raiz);
//private:
   GGroup<Turno*> turnos;
   std::string codigo;
   std::string descricao;
   int tempo_aula;
};
