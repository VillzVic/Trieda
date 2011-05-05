#ifndef _AREA_TITULACAO_H_
#define _AREA_TITULACAO_H_

#include "ofbase.h"

class AreaTitulacao :
   public OFBase
{
public:
   AreaTitulacao(void);
   virtual ~AreaTitulacao(void);

   virtual void le_arvore( ItemAreaTitulacao & );

   void setNome( std::string s ) { nome = s; }
   std::string getNome() { return nome; }

private:
   std::string nome;
};

#endif