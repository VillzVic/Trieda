#ifndef _ALOCACAO_AULA_H_
#define _ALOCACAO_AULA_H_

#include "Professor.h"
#include "Horario.h"

class AlocacaoAula
{
public:
   AlocacaoAula(void);
   virtual ~AlocacaoAula(void);

   void setDiaSemana(int);
   void setProfessor(Professor*);
   void setProfessorId(int);   
   void setHorario(Horario*);
   void setHorarioId(int);

   int getDiaSemana() const;
   Professor* getProfessor() const;
   int getProfessorId() const;
   Horario* getHorario() const;
   int getHorarioId() const;

   virtual bool operator < (AlocacaoAula const & right) 
   { 
      return 
		  ((professor_id < right.getProfessorId()) &&
         (dia_semana < right.getDiaSemana()) &&
		 (horario_id < right.getHorarioId()));
   }

   virtual bool operator == (AlocacaoAula const & right)
   { 
      return 
		  ((professor_id == right.getProfessorId()) &&
         (dia_semana == right.getDiaSemana()) &&
		 (horario_id == right.getHorarioId()));
   }

private:
   int dia_semana;
   int professor_id;
   int horario_id;
   Professor* professor;
   Horario* horario;
};

#endif /* _ALOCACAO_AULA_H_ */