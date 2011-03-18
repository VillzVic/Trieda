#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "Oferta.h"

using namespace std;

class AtendimentoOferta:
   public OFBase
{
public:
   AtendimentoOferta(void);
   virtual ~AtendimentoOferta(void);

   std::string oferta_curso_campi_id;
   int disciplina_id;
   int quantidade;
   int turma;

   Oferta * oferta;
   
   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);
};

std::ostream& operator << (std::ostream& out, AtendimentoOferta& oferta);