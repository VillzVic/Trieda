#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoOferta.h"

using namespace std;

class AtendimentoHorarioAula:
   public OFBase
{
public:
   AtendimentoHorarioAula(void);
   virtual ~AtendimentoHorarioAula(void);

   int horario_aula_id;
   int professor_id;
   bool credito_teorico;
   GGroup<AtendimentoOferta*> * atendimentos_ofertas;

   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};

std::ostream& operator << (std::ostream& out, AtendimentoHorarioAula& horario_aula);