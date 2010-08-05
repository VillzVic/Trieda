#pragma once
#include "ofbase.h"
#include "Disciplina.h"
#include "Professor.h"

class Magisterio :
   public OFBase
{
public:
   Magisterio(Professor* professor);
   ~Magisterio(void);

   int nota;
   int ranking;
   Professor* professor;
   Disciplina* disciplina;

   int getId() { return 100000*disciplina->getId() + professor->getId(); }
   virtual void le_arvore(ItemProfessorDisciplina& elem);


};
