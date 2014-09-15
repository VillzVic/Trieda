#ifndef _TIPO_CONTRATO_H_
#define _TIPO_CONTRATO_H_

#include "ofbase.h"

class TipoContrato :
   public OFBase
{
public:
   TipoContrato( int );
   TipoContrato( void );
   virtual ~TipoContrato( void );

   virtual void le_arvore( ItemTipoContrato & );

   void setNome( std::string s ) { this->nome = s; }
   std::string getNome() const { return this->nome; }

   void setContrato();   
   int getContrato() const { return contrato; }
   
   void operator = ( TipoContrato const & right );
   bool operator < ( TipoContrato const & right ) const;
   bool operator > ( TipoContrato const & right ) const;
   bool operator == ( TipoContrato const & right ) const;

   enum Contrato
   {
	    Error = 0,
		Outro = 1,
		Parcial = 2,
		Integral = 3
   };

private:
   std::string nome;
   int contrato;
};

#endif
