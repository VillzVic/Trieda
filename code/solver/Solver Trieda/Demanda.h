#pragma once
#include "ofbase.h"
#include "Turno.h"
#include "Disciplina.h"
#include "Curso.h"

class Demanda :
   public OFBase
{
public:
   Demanda(void);
   ~Demanda(void);
   virtual void le_arvore(ItemDemanda& elem);
   bool operator < (const Demanda& right) 
   { 
      if(disciplina != right.disciplina)
         return (disciplina < right.disciplina); 
      if(turno != right.turno)
         return (turno < right.turno);
      if(curso != right.curso)
         return (curso < right.curso);
      return (quantidade < right.quantidade);
   }
   bool operator == (const Demanda& right) { 
      return (id == right.id); 
   }

   int quantidade;
   Turno* turno;
   Disciplina* disciplina;
   Curso* curso;
};
