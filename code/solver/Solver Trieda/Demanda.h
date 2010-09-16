#pragma once
#include "ofbase.h"
#include "Oferta.h"
#include "Disciplina.h"

class Demanda :
   public OFBase
{
public:
   Demanda(void);
   ~Demanda(void);
   virtual void le_arvore(ItemDemanda& elem);
   //bool operator < (const Demanda& right) 
   //{ 
   //   if(unidade != right.unidade)
   //      return (unidade < right.unidade); 
   //   if(turno != right.turno)
   //      return (turno < right.turno);
   //   if(curso != right.curso)
   //      return (curso < right.curso);
   //   return (quantidade < right.quantidade);
   //}
   //bool operator == (const Demanda& right) { 
   //   return (id == right.id); 
   //}

   int quantidade;
   int oferta_id;
   int disciplina_id;
   Oferta* oferta;
   Disciplina* disciplina;
};
