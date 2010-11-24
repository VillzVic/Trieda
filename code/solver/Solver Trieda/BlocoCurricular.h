#pragma once
#include "ofbase.h"

#include "Curso.h"
#include "Disciplina.h"
#include "Campus.h"

class BlocoCurricular :
   public OFBase
{
public:
   BlocoCurricular(void);
   ~BlocoCurricular(void);

   //int getId() { return curso->getId() * 100 + campus->getId() * 20000 + periodo; }
   int getId() { return curso->getId() * 100 + periodo; }
   //int getId() { return curso->getId() * 100 + campus->getId() * 200 + periodo; }

   int periodo;
   Curso* curso;
   Campus* campus;
   GGroup<Disciplina*> disciplinas;

   GGroup<int> diasLetivos;

   int total_turmas;
};
