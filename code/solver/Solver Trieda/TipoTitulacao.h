#ifndef _TIPO_TITULACAO_H_
#define _TIPO_TITULACAO_H_

#include "ofbase.h"

class TipoTitulacao :
   public OFBase
{
public:
   TipoTitulacao( void );
   virtual ~TipoTitulacao( void );

   virtual void le_arvore( ItemTipoTitulacao &  );

   void setNome( std::string s ) { nome = s; }

   std::string getNome() const { return nome; }

   std::string toString() const { return nome; }

   /*
		id = 1 - Licenciado
		id = 2 - Bacharel
		id = 3 - Especialista
		id = 4 - Mestre
		id = 5 - Doutor
   */

private:
   std::string nome;
};

#endif