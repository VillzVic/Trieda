#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

class AtendimentoOferta:
   public OFBase
{
public:
   AtendimentoOferta(void);
   ~AtendimentoOferta(void);

   std::string oferta_curso_campi_id;
   int quantidade;
   
   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};
