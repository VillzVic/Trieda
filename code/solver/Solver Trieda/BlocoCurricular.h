#pragma once
#include "ofbase.h"

#include "Curso.h"
#include "Disciplina.h"

class BlocoCurricular :
   public OFBase
{
public:
   BlocoCurricular(void);
   ~BlocoCurricular(void);

   int getId() { return curso->getId() * 100 + periodo; }

   int periodo;
   Curso* curso;
   GGroup<Disciplina*> disciplinas;
};
