#ifndef _TIPO_SALA_H_
#define _TIPO_SALA_H_

#include <string>
#include "ofbase.h"

class TipoSala :
   public OFBase
{
public:
   TipoSala(void);
   ~TipoSala(void);

   virtual void le_arvore(ItemTipoSala& elem);

   void setNome(std::string s) { nome = s; }
   std::string getNome() const { return nome; }

private:
   std::string nome;
};

#endif