#include "disciplina.h"
#include "sala.h"

class Aula
{
public:
   Aula(void);
   ~Aula(void);

   void setTurma(int*);
   void setDisciplina(Disciplina*);
   void setSala(Sala*);

   int* getTurma() const;
   Disciplina* getDisciplina() const;
   Sala* getSala() const;

private:
   int* turma;
   Disciplina* disciplina;
   Sala* sala;
};
