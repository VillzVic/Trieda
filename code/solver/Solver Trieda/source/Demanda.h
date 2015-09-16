#ifndef _DEMANDA_H_
#define _DEMANDA_H_

#include "ofbase.h"
#include "Oferta.h"
#include "Disciplina.h"
#include <unordered_set>

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
   Calendario *getCalendario() const { return oferta->curriculo->calendario; }
   TurnoIES *getTurnoIES() const { return oferta->turno; }

   virtual bool operator < ( const Demanda & ) const;
   virtual bool operator > ( const Demanda & ) const;
   virtual bool operator == ( const Demanda & ) const;
   virtual bool operator != ( const Demanda & ) const;

   bool ehSubstitutaPossivel( int discId );
   void addSubstitutaPossivel( Disciplina* discSubstituta );

   map<int, Disciplina*> discIdSubstitutasPossiveis;

   // preencher set com disciplina original + substitutas
   void getAllDiscPossiveis (unordered_set<Disciplina*> &disciplinas) const;

private:
   int quantidade;
   int oferta_id;
   int disciplina_id;
};

#endif