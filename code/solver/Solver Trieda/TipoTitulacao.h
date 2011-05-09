#ifndef _TIPO_TITULACAO_H_
#define _TIPO_TITULACAO_H_

#include "ofbase.h"

class TipoTitulacao :
   public OFBase
{
public:
   TipoTitulacao(void);
   virtual ~TipoTitulacao(void);

   virtual void le_arvore(ItemTipoTitulacao& elem);

   void setNome( std::string s ) { nome = s; }

   std::string getNome() { return nome; }

private:
   std::string nome;
};

#endif