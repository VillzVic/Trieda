#ifndef _EQUIVALENCIA_H_
#define _EQUIVALENCIA_H_

#include "ofbase.h"
#include "GGroup.h"

class Equivalencia :
   public OFBase
{
public:
   Equivalencia( void );
   Equivalencia( int id, int cursou, int elimina, bool geral );   
   virtual ~Equivalencia( void );

   virtual void le_arvore( ItemEquivalencia & );

   void setDisciplinaCursouId( int v ) { disciplinaCursouId = v; }
   void setDisciplinaEliminaId( int v ) { disciplinaEliminaId = v; }
   void setGeral( bool v ) { geral = v; }

   int getDisciplinaCursouId() const { return disciplinaCursouId; }
   int getDisciplinaEliminaId() const { return disciplinaEliminaId; }
   bool getGeral() const { return geral; }
   
private:
   int disciplinaCursouId;
   int disciplinaEliminaId;
   bool geral;

};

#endif