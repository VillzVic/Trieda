#ifndef _DESLOCAMENTO_H_
#define _DESLOCAMENTO_H_

#include "ofbase.h"

class Deslocamento :
   public OFBase
{
public:
   Deslocamento(void);
   ~Deslocamento(void);

   void le_arvore(ItemDeslocamento& elem);

   OFBase* origem;
   OFBase* destino;

   void setOrigemId(int v) { origem_id = v; }
   void setDestinoId(int v) { destino_id = v; }
   void setTempo(int v) { tempo = v; }
   void setCusto(double v) { custo = v; }

   int getOrigemId() { return origem_id; }
   int getDestinoId() { return destino_id; }
   int getTempo() { return tempo; }
   double getCusto() { return custo; }

private:
   int origem_id;
   int destino_id;
   int tempo;
   double custo;
};

#endif