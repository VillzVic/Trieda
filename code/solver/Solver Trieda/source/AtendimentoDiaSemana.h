#ifndef _ATENDIMENTO_DIA_SEMANA_H_
#define _ATENDIMENTO_DIA_SEMANA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

class AtendimentoTatico;
class AtendimentoTurno;

class AtendimentoDiaSemana:
   public OFBase
{
public:
   AtendimentoDiaSemana( int );
   AtendimentoDiaSemana( void );
   virtual ~AtendimentoDiaSemana( void );

   GGroup< AtendimentoTatico*, LessPtr<AtendimentoTatico> > * atendimentos_tatico;
   GGroup< AtendimentoTurno*, LessPtr<AtendimentoTurno> > * atendimentos_turno;

   void setDiaSemana( int value ) { this->dia_semana = value; }
   int getDiaSemana() const { return this->dia_semana; }

   // procura o atendimento turno com um determinado id. se não encontra cria um novo e retorna-o
   AtendimentoTurno* getAddAtendTurno (int id_turno);

private:
   int dia_semana;
};

std::ostream & operator << ( std::ostream &, AtendimentoDiaSemana & );

std::istream& operator >> ( std::istream &file, AtendimentoDiaSemana* const &ptrAtendDiaSem);

#endif
