#ifndef _ATENDIMENTO_OFERTA_H_
#define _ATENDIMENTO_OFERTA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "Oferta.h"

class AtendimentoOferta :
   public OFBase
{
public:
   AtendimentoOferta( int );
   virtual ~AtendimentoOferta( void );

   Oferta * oferta;

   void setOfertaCursoCampiId( std::string value ) { this->oferta_curso_campi_id = value; }
   void setDisciplinaId( int value ) { this->disciplina_id = value; }
   void setQuantidade( int value ) { this->quantidade = value; }
   void setTurma( int value ) { this->turma = value; }

   std::string getOfertaCursoCampiId() const { return this->oferta_curso_campi_id; }
   int getDisciplinaId() const { return this->disciplina_id; }
   int getQuantidade() const { return this->quantidade; }
   int getTurma() const { return this->turma; }

private:
   std::string oferta_curso_campi_id;
   int disciplina_id;
   int quantidade;
   int turma;
};

std::ostream & operator << ( std::ostream &, AtendimentoOferta & );

#endif