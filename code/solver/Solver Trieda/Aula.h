#ifndef _AULA_H_
#define _AULA_H_

#include "disciplina.h"
#include "sala.h"

class Aula
{
public:
   Aula(void);
   virtual ~Aula(void);

   void setTurma(int);
   void setDisciplina(Disciplina*);
   void setSala(Sala*);
   void setDiaSemana(int);
   void setCreditosTeoricos(int);
   void setCreditosPraticos(int);

   int getTurma() const;
   Disciplina* getDisciplina() const;
   Sala* getSala() const;
   int getDiaSemana() const;
   int getCreditosTeoricos() const;
   int getCreditosPraticos() const;

   virtual bool operator < (Aula const & right) 
   { 
      return 
         ((turma < right.getTurma()) &&
         (disciplina < right.getDisciplina()) &&
         (sala < right.getSala()) &&
         (diaSemana < right.getDiaSemana()) &&
         (creditos_teoricos < right.getCreditosTeoricos()) &&
         (creditos_praticos < right.getCreditosPraticos()));
   }

   virtual bool operator == (Aula const & right)
   { 
      return 
         ((turma == right.getTurma()) &&
         (disciplina == right.getDisciplina()) &&
         (sala == right.getSala()) &&
         (diaSemana == right.getDiaSemana()) &&
         (creditos_teoricos == right.getCreditosTeoricos()) &&
         (creditos_praticos == right.getCreditosPraticos()));
   }

private:
   int turma;
   Disciplina* disciplina;
   Sala* sala;
   int diaSemana;
   int quantidade;
   int creditos_teoricos;
   int creditos_praticos;
};

#endif /* AULA_H */