#pragma once
#include "ofbase.h"
#include "NivelDificuldade.h"

class NivelDificuldadeHorario :
   public OFBase
{
public:
   NivelDificuldadeHorario(void);
   ~NivelDificuldadeHorario(void);

   int nivel_dificuldade_id;
   GGroup<int> horarios_aula; 

   virtual void le_arvore(ItemNivelDificuldadeHorario& elem);

   NivelDificuldade* nivel_dificuldade;
};
 