#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

using namespace std;

class AtendimentoOferta:
   public OFBase
{
public:
   AtendimentoOferta(void);
   ~AtendimentoOferta(void);

   std::string oferta_curso_campi_id;
   std::string disciplina_id;
   int quantidade;
   
   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};

std::ostream& operator << (std::ostream& out, AtendimentoOferta& oferta);