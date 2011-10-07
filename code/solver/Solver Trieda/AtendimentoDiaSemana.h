#ifndef _ATENDIMENTO_DIA_SEMANA_H_
#define _ATENDIMENTO_DIA_SEMANA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "AtendimentoTatico.h"
#include "AtendimentoTurno.h"

class AtendimentoDiaSemana:
   public OFBase
{
public:
   AtendimentoDiaSemana( int );
   virtual ~AtendimentoDiaSemana( void );

   GGroup< AtendimentoTatico * > * atendimentos_tatico;
   GGroup< AtendimentoTurno * > * atendimentos_turno;

   void setDiaSemana( int value ) { this->dia_semana = value; }
   int getDiaSemana() const { return this->dia_semana; }

private:
   int dia_semana;
};

std::ostream & operator << ( std::ostream &, AtendimentoDiaSemana & );

#endif
