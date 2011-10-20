#ifndef _TIPO_CONTRATO_H_
#define _TIPO_CONTRATO_H_

#include "ofbase.h"

class TipoContrato :
   public OFBase
{
public:
   TipoContrato( void );
   virtual ~TipoContrato( void );

   virtual void le_arvore( ItemTipoContrato & );

   void setNome( std::string s ) { this->nome = s; }
   std::string getNome() const { return this->nome; }

private:
   std::string nome;
};

#endif
