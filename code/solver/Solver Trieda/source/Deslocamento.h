#ifndef _DESLOCAMENTO_H_
#define _DESLOCAMENTO_H_

#include "ofbase.h"

class Deslocamento
{
public:
   Deslocamento( void );
   virtual ~Deslocamento( void );

   void le_arvore( ItemDeslocamento & );

   OFBase * origem;
   OFBase * destino;

   void setOrigemId( int v ) { origem_id = v; }
   void setDestinoId( int v ) { destino_id = v; }
   void setTempo( int v ) { tempo = v; }
   void setCusto( double v ) { custo = v; }

   int getOrigemId() { return origem_id; }
   int getDestinoId() { return destino_id; }
   int getTempo() { return tempo; }
   double getCusto() { return custo; }

   bool operator < ( const Deslocamento & right ) const
   { 
	   if (origem_id < right.origem_id)
		   return true;
	   if (origem_id > right.origem_id)
		   return false;
	   if (destino_id < right.destino_id)
		   return true;
	   if (destino_id > right.destino_id)
		   return false;
	   return false;
   }
   bool operator > ( const Deslocamento & right ) const
   { 
      return ( right < *this );
   }
   bool operator == ( const Deslocamento & right ) const
   { 
      return ( !(right < *this) && !(*this < right) );
   }
   bool operator != ( const Deslocamento & right ) const
   { 
      return !( *this == right );
   }

private:
   int origem_id;
   int destino_id;
   int tempo;
   double custo;
};

#endif
