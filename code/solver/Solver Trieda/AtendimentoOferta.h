#ifndef _ATENDIMENTO_OFERTA_H_
#define _ATENDIMENTO_OFERTA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "Oferta.h"

class AtendimentoOferta:
   public OFBase
{
public:
   AtendimentoOferta(void);
   virtual ~AtendimentoOferta(void);

   Oferta * oferta;

   void setOfertaCursoCampiId(std::string value) { oferta_curso_campi_id = value; }
   void setDisciplinaId(int value) { disciplina_id = value; }
   void setQuantidade(int value) { quantidade = value; }
   void setTurma(int value) { turma = value; }

   std::string getOfertaCursoCampiId() { return oferta_curso_campi_id; }
   int getDisciplinaId() { return disciplina_id; }
   int getQuantidade() { return quantidade; }
   int getTurma() { return turma; }

private:
   std::string oferta_curso_campi_id;
   int disciplina_id;
   int quantidade;
   int turma;
};

std::ostream & operator << (std::ostream &, AtendimentoOferta &);

#endif