#ifndef _TIPO_TITULACAO_H_
#define _TIPO_TITULACAO_H_

#include "ofbase.h"
#include <string>

class TipoTitulacao :
   public OFBase
{
public:
   TipoTitulacao( void );
   TipoTitulacao( int v );
   virtual ~TipoTitulacao( void );

   virtual void le_arvore( ItemTipoTitulacao &  );

   void setNome( std::string s ) { nome = s; }
   std::string getNome() const { return nome; }

   std::string toString() const { return nome; }

   void setTitulacao();									// Valor usado internamente!
   int getTitulacao() const { return titulacao; }		// Valor usado internamente!

   void operator = ( TipoTitulacao const & right );
   bool operator < ( TipoTitulacao const & right ) const;
   bool operator > ( TipoTitulacao const & right ) const;
   bool operator == ( TipoTitulacao const & right ) const;

   enum Titulacao
   {
	    Error = 0,
		Licenciado = 1,
		Bacharel = 2,
		Especialista = 3,
		Mestre = 4,
		Doutor = 5		
   };

private:
   std::string nome;
   int titulacao;		// Valor usado internamente! O valor passado ao cliente deve ser o valor de 'id' da OFBase.
};



#endif