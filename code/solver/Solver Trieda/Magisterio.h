#pragma once
#include "ofbase.h"

class Magisterio :
   public OFBase
{
public:
   Magisterio();
   ~Magisterio(void);

   int nota;
   int preferencia;
   int disciplina_id;

   int getId() { return disciplina_id; }
   virtual void le_arvore(ItemProfessorDisciplina& elem);


};
