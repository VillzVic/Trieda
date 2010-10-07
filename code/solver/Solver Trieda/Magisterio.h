#pragma once
#include "ofbase.h"
#include "Disciplina.h"

class Magisterio :
   public OFBase
{
public:
   Magisterio();
   ~Magisterio(void);

   int nota;
   int preferencia;
   int disciplina_id;

   //int getId() { return disciplina_id; } // FIXME, isto está errado
   virtual void le_arvore(ItemProfessorDisciplina& elem);
   Disciplina* disciplina;
};
