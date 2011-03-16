#pragma once
#include "ofbase.h"

#include <iostream>

class AtendimentoOfertaSolucao :
   public OFBase
{
public:
   AtendimentoOfertaSolucao(void);
   ~AtendimentoOfertaSolucao(void);
   virtual void le_arvore(ItemAtendimentoOfertaSolucao& elem);

   virtual bool operator < (AtendimentoOfertaSolucao & right) 
   { 
	   return ( ( ofertaCursoCampiId < right.getOfertaCursoCampiId()) &&
			( disciplinaId < right.getDisciplinaId()) &&
			( turma < right.getTurma()) );
   }

   virtual bool operator == (AtendimentoOfertaSolucao & right)
   { 
	   return ( ( ofertaCursoCampiId == right.getOfertaCursoCampiId()) &&
			( disciplinaId == right.getDisciplinaId()) &&
			( turma == right.getTurma()) );
   }

   void setOfertaCursoCampiId(int v) { ofertaCursoCampiId = v; }
   void setDisciplinaId(int v) { disciplinaId = v; }
   void setQuantidade(int v) { quantidade = v; }
   void setTurma(std::string s) { turma = s; }

   int getOfertaCursoCampiId() { return ofertaCursoCampiId; }
   int getDisciplinaId() { return disciplinaId; }
   int getQuantidade() { return quantidade; }
   std::string getTurma() { return turma; }

private:
   int ofertaCursoCampiId;
   int disciplinaId;
   int quantidade;
   std::string turma;
};