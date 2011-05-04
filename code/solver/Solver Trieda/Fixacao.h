#pragma once
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
   Fixacao(void);
   ~Fixacao(void);

   void le_arvore(ItemFixacao &);

   Professor * professor;
   Disciplina * disciplina;
   Sala * sala;
   Turno * turno;
   HorarioAula * horario_aula;

   void setProfessorId(int v) { professor_id = v; }
   void setDisciplinaId(int v) { disciplina_id = v; }
   void setSalaId(int v) { sala_id = v; }
   void setDiaSemana(int v) { dia_semana = v; }
   void setTurnoId(int v) { turno_id = v; }
   void setHorarioId(int v) { horario_id = v; }

   int getProfessorId() { return professor_id; }
   int getDisciplinaId() { return disciplina_id; }
   int getSalaId() { return sala_id; }
   int getDiaSemana() { return dia_semana; }
   int getTurnoId() { return turno_id; }
   int getHorarioId() { return horario_id; }

private:
   int professor_id;
   int disciplina_id;
   int sala_id;
   int dia_semana;
   int turno_id;
   int horario_id;
};

