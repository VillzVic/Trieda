#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoDiaSemana.h"

#include "Sala.h"

class AtendimentoSala:
   public OFBase
{
public:
   AtendimentoSala(void);
   virtual ~AtendimentoSala(void);

   GGroup< AtendimentoDiaSemana * > * atendimentos_dias_semana;
   Sala * sala;

   void setSalaId(std::string s) { sala_id = s; }
   std::string getSalaId() { return sala_id; }

private:
   std::string sala_id;
};

std::ostream& operator << (std::ostream& out, AtendimentoSala& sala);