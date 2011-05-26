#ifndef _DEMANDA_H_
#define _DEMANDA_H_

#include "ofbase.h"
#include "Oferta.h"
#include "Disciplina.h"

class Demanda :
   public OFBase
{
public:
   Demanda( void );
   Demanda( Demanda const & );
   virtual ~Demanda( void );

   virtual void le_arvore( ItemDemanda & );

   Oferta * oferta;
   Disciplina * disciplina;

   void setQuantidade( int value ) { quantidade = value; }
   void setOfertaId( int value ) { oferta_id = value; }
   void setDisciplinaId( int value ) { disciplina_id = value; }

   int getQuantidade() const { return quantidade; }
   int getOfertaId() const { return oferta_id; }
   int getDisciplinaId() const { return disciplina_id; }

   virtual bool operator < ( const Demanda& right );
   virtual bool operator > ( const Demanda& right );
   virtual bool operator == ( const Demanda & right );
   virtual bool operator != ( const Demanda & right );

private:
   int quantidade;
   int oferta_id;
   int disciplina_id;
};

#endif