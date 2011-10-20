#ifndef _AREA_TITULACAO_H_
#define _AREA_TITULACAO_H_

#include "ofbase.h"

class AreaTitulacao :
   public OFBase
{
public:
   AreaTitulacao( void );
   virtual ~AreaTitulacao( void );

   virtual void le_arvore( ItemAreaTitulacao & );

   void setNome( std::string s ) { this->nome = s; }
   std::string getNome() const { return this->nome; }

private:
   std::string nome;
};

#endif
