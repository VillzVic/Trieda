#ifndef _ATENDIMENTO_SALA_H_
#define _ATENDIMENTO_SALA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoDiaSemana.h"

#include "Sala.h"

class AtendimentoSala:
   public OFBase
{
public:
   AtendimentoSala( int );
   virtual ~AtendimentoSala( void );

   GGroup< AtendimentoDiaSemana * > * atendimentos_dias_semana;
   Sala * sala;

   void setSalaId( std::string s ) { this->sala_id = s; }
   std::string getSalaId() const { return this->sala_id; }

private:
   std::string sala_id;
};

std::ostream& operator << ( std::ostream &, AtendimentoSala & );

#endif