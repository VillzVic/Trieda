#ifndef _MAGISTERIO_H_
#define _MAGISTERIO_H_

#include "ofbase.h"
#include "Disciplina.h"

class Magisterio :
   public OFBase
{
public:
   Magisterio( void );
   virtual ~Magisterio( void );

   virtual void le_arvore( ItemProfessorDisciplina & );

   Disciplina * disciplina;

   void setNota( int v ) { nota = v; }
   void setPreferencia( int v ) { preferencia = v; }
   void setDisciplinaId( int v ) { disciplina_id = v;  }

   int getNota() const { return nota; }
   int getPreferencia() const { return preferencia; }
   int getDisciplinaId() const { return disciplina_id; }

   virtual bool operator < ( const Magisterio & ) const;
   virtual bool operator > ( const Magisterio & ) const;
   virtual bool operator == ( const Magisterio & ) const;
   virtual bool operator != ( const Magisterio & ) const;

private:
   int nota;
   int preferencia;
   int disciplina_id;
};

#endif