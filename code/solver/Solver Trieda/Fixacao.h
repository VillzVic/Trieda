#ifndef _FIXACAO_H_
#define _FIXACAO_H_

#include "ofbase.h"
#include "Professor.h"
#include "Disciplina.h"
#include "Sala.h"
#include "Turno.h"
#include "Horario.h"

class Fixacao :
   public OFBase
{
public:
   Fixacao( void );
   Fixacao( Fixacao const & );
   virtual ~Fixacao( void );

   virtual void le_arvore( ItemFixacao & );

   Professor * professor;
   Disciplina * disciplina;
   Sala * sala;
   Turno * turno;
   HorarioAula * horario_aula;

   void setProfessorId( int v ) { professor_id = v; }
   void setDisciplinaId( int v ) { disciplina_id = v; }
   void setSalaId( int v ) { sala_id = v; }
   void setDiaSemana( int v ) { dia_semana = v; }
   void setTurnoId( int v ) { turno_id = v; }
   void setHorarioAulaId( int v ) { horario_aula_id = v; }

   int getProfessorId() const { return professor_id; }
   int getDisciplinaId() const { return disciplina_id; }
   int getSalaId() const { return sala_id; }
   int getDiaSemana() const { return dia_semana; }
   int getTurnoId() const { return turno_id; }
   int getHorarioAulaId() const { return horario_aula_id; }

private:
   int professor_id;
   int disciplina_id;
   int sala_id;
   int dia_semana;
   int turno_id;
   int horario_aula_id;
};

#endif