#ifndef _MAGISTERIO_H_
#define _MAGISTERIO_H_

#include "ofbase.h"
#include "Disciplina.h"

class Magisterio :
   public OFBase
{
public:
   Magisterio();
   virtual ~Magisterio(void);

   virtual void le_arvore( ItemProfessorDisciplina & );

   Disciplina * disciplina;

   void setNota( int v ) { nota = v; }
   void setPreferencia( int v ) { preferencia = v; }
   void setDisciplinaId( int v ) { disciplina_id = v;  }

   int getNota() const { return nota; }
   int getPreferencia() const { return preferencia; }
   int getDisciplinaId() const { return disciplina_id; }

   // Ver com o Cleiton antes o que ele faz com o magistério em disciplinasEquivalentes().

   //virtual bool operator == ( const Magisterio & right )
   //{ 
   //   return ((nota == right.getNota()) && (preferencia == right.getPreferencia()) && (disciplina_id == right.getDisciplinaId())); 
   //}

   //virtual bool operator != ( const Magisterio & right )
   //{ 
   //   return !((*this) == right); 
   //}

private:
   int nota;
   int preferencia;
   int disciplina_id;
};

#endif