#ifndef _ATENDIMENTO_TURNO_H_
#define _ATENDIMENTO_TURNO_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "AtendimentoHorarioAula.h"

class AtendimentoTurno :
   public OFBase
{
public:
   AtendimentoTurno( void );
   virtual ~AtendimentoTurno( void );

   GGroup< AtendimentoHorarioAula * > * atendimentos_horarios_aula;

   void setTurnoId( int value ) { turno_id = value; }
   int getTurnoId() const { return turno_id; }

private:
   int turno_id;
};

std::ostream & operator << ( std::ostream &, AtendimentoTurno & );

#endif