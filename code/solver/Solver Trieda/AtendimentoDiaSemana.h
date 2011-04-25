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
   AtendimentoDiaSemana(void);
   virtual ~AtendimentoDiaSemana(void);

   GGroup< AtendimentoTatico * > * atendimentos_tatico;
   GGroup< AtendimentoTurno * > * atendimentos_turno;

   void setDiaSemana(int value) { dia_semana = value; }
   int getDiaSemana() { return dia_semana; }

private:
   int dia_semana;
};

std::ostream & operator << ( std::ostream &, AtendimentoDiaSemana & );

#endif