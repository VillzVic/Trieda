#ifndef AULA_H
#define AULA_H

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

   int getTurma() const;
   Disciplina* getDisciplina() const;
   Sala* getSala() const;

   virtual bool operator < (Aula const & right) 
   { 
      return 
         ((turma < right.getTurma()) &&
         (disciplina < right.getDisciplina()) &&
         (sala < right.getSala()));
   }

   virtual bool operator == (Aula const & right) { 
      return 
         ((turma == right.getTurma()) &&
         (disciplina == right.getDisciplina()) &&
         (sala == right.getSala()));
   }

private:
   int turma;
   Disciplina* disciplina;
   Sala* sala;
};

#endif /* AULA_H */