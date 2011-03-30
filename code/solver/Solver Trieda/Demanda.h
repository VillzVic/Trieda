#ifndef _DEMANDA_H_
#define _DEMANDA_H_

#include "ofbase.h"
#include "Oferta.h"
#include "Disciplina.h"

class Demanda :
   public OFBase
{
public:
   Demanda(void);
   Demanda(Demanda const & demanda);
   ~Demanda(void);

   virtual void le_arvore(ItemDemanda& elem);

   Oferta * oferta;
   Disciplina * disciplina;

   bool operator ==(Demanda const & demanda);

   void setQuantidade(int value) { quantidade = value; }
   void setOfertaId(int value) { oferta_id = value; }
   void setDisciplinaId(int value) { disciplina_id = value; }

   int getQuantidade() const { return quantidade; }
   int getOfertaId() const { return oferta_id; }
   int getDisciplinaId() const { return disciplina_id; }

private:
   int quantidade;
   int oferta_id;
   int disciplina_id;
};

#endif