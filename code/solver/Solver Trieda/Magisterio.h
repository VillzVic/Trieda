#pragma once
#include "ofbase.h"
#include "Disciplina.h"

class Magisterio :
   public OFBase
{
public:
   Magisterio();
   ~Magisterio(void);

   virtual void le_arvore(ItemProfessorDisciplina& elem);

   Disciplina * disciplina;

   void setNota(int v) { nota = v; }
   void setPreferencia(int v) { preferencia = v; }
   void setDisciplinaId(int v) { disciplina_id = v;  }

   int getNota() { return nota; }
   int getPreferencia() { return preferencia; }
   int getDisciplinaId() { return disciplina_id; }

private:
   int nota;
   int preferencia;
   int disciplina_id;
};
