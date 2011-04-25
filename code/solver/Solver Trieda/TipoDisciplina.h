#ifndef _TIPO_DISCIPLINA_H_
#define _TIPO_DISCIPLINA_H_

#include "ofbase.h"

class TipoDisciplina :
   public OFBase
{
public:
   TipoDisciplina(void);
   virtual ~TipoDisciplina(void);

   virtual void le_arvore( ItemTipoDisciplina & );

   void setNome(std::string s) { nome = s; }
   std::string getNome() const { return nome; }

private:
   std::string nome;
};

#endif