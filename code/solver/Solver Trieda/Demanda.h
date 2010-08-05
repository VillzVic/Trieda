#pragma once
#include "ofbase.h"
#include "Turno.h"
#include "Curso.h"
class Unidade;
#include "Unidade.h"

class Demanda :
   public OFBase
{
public:
   Demanda(void);
   ~Demanda(void);
   virtual void le_arvore(ItemDemanda& elem);
   bool operator < (const Demanda& right) 
   { 
      if(unidade != right.unidade)
         return (unidade < right.unidade); 
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
   Unidade* unidade;
   Curso* curso;
};
