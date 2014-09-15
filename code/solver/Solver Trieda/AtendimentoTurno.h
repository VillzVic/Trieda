#ifndef _ATENDIMENTO_TURNO_H_
#define _ATENDIMENTO_TURNO_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

class AtendimentoHorarioAula;
class TurnoIES;

class AtendimentoTurno :
   public OFBase
{
public:
   AtendimentoTurno( int );
   AtendimentoTurno( void );
   virtual ~AtendimentoTurno( void );

   GGroup< AtendimentoHorarioAula*, LessPtr<AtendimentoHorarioAula> > * atendimentos_horarios_aula;

   void setTurnoId( int value ) { this->turno_id = value; }
   int getTurnoId() const { return this->turno_id; }

   TurnoIES * turno;

   // procura o atendimento horario aula com um determinado id. se não encontra cria um novo e retorna-o
   AtendimentoHorarioAula* getAddAtendHorarioAula (int id);

private:
   int turno_id;
};

std::ostream & operator << ( std::ostream &, AtendimentoTurno & );

std::istream & operator >> ( std::istream &file, AtendimentoTurno* const &ptrAtendTurno );

#endif