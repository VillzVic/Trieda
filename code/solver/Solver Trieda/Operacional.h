#ifndef _OPERACIONAL_H_
#define _OPERACIONAL_H_

class Operacional
{
public:
   Operacional(void);
   virtual ~Operacional(void);

   void setHorarioAulaId( int value ) { horario_aula_id = value; }
   void setProfessorId( int value ) { professor_id = value; }

   int getHorarioAulaId() { return horario_aula_id; }
   int getProfessorId() { return professor_id; }

private:
   int horario_aula_id;
   int professor_id;
};

#endif