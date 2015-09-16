#ifndef _NIVEL_DIFICULDADE_H_
#define _NIVEL_DIFICULDADE_H_

#include "ofbase.h"

class NivelDificuldade :
   public OFBase
{
public:
   NivelDificuldade( void );
   virtual ~NivelDificuldade( void );
   
   virtual void le_arvore(ItemNivelDificuldade& raiz);

   void setNome( std::string s ) { this->nome = s; }
   std::string getNome() const { return this->nome; }

private:
   std::string nome;
};

#endif