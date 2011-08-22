#ifndef VARIABLEOP_H
#define VARIABLEOP_H

#include <map>

#include "Aula.h"
#include "Professor.h"
#include "ProblemData.h"

// Variables
class VariableOp 
{
public:
   // All variable types
   enum VariableOpType
   {
      V_ERROR = 0,
      V_X_PROF_AULA_HOR = 1,
      V_Y_PROF_DISCIPLINA = 2,
      V_Z_DISCIPLINA_HOR = 3,
      V_F_FIX_PROF_DISC_SALA_DIA_HOR = 4,
      V_F_FIX_PROF_DISC_DIA_HOR = 5,
      V_F_FIX_PROF_DISC = 6,
      V_F_FIX_PROF_DISC_SALA = 7,
      V_F_FIX_PROF_SALA = 8,
      V_F_DISC_HOR = 9,
      V_PROF_CURSO = 10,
      V_F_MIN_MEST_CURSO = 11,
      V_F_MIN_DOUT_CURSO = 12,
      V_F_CARGA_HOR_MIN_PROF = 13,
      V_F_CARGA_HOR_MIN_PROF_SEMANA = 14,
      V_F_CARGA_HOR_MAX_PROF_SEMANA = 15,
      V_DIAS_PROF_MINISTRA_AULAS = 16,
      V_CUSTO_CORPO_DOCENTE = 17,
      V_MAX_DISC_PROF_CURSO = 18,
      V_F_MAX_DISC_PROF_CURSO = 19,
      V_AVALIACAO_CORPO_DOCENTE = 20,
      V_F_ULTIMA_PRIMEIRA_AULA_PROF = 21,
      V_GAPS_PROFESSORES = 22
   };

   // Constructors
   VariableOp();
   VariableOp( const VariableOp & );

   // Destructor
   virtual ~VariableOp();

   //==================================================
   // GET METHODS 
   //==================================================
   // Return variable type
   VariableOpType getType() const { return type; }

   // Return value
   double getValue() const { return value; }
   Curso * getCurso() const { return this->curso; }
   HorarioDia * getHorario() const { return h; }
   HorarioAula * getHorarioAula() const { return horarioAula; }
   Aula * getAula() const { return aula; }
   Professor * getProfessor() const { return professor; }
   Disciplina * getDisciplina() const { return disciplina; }
   Disciplina * getDisciplinaD() const { return disciplinaD; }
   Disciplina * getDisciplinaD1() const { return disciplinaD1; }
   Sala * getSala() const { return sala; }
   int getTurma() const { return turma; }
   int getDia() const { return dia; }
   HorarioAula * getH1() const { return h1; }
   HorarioAula * getH2() const { return h2; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set value
   void setValue( double v ) { value = v; }
   void setCurso( Curso * c ) { this->curso = c; }
   void setHorario( HorarioDia * hh ) {  h = hh; }
   void setHorarioAula( HorarioAula * hh ) {  horarioAula = hh; }
   void setAula( Aula * a ) {  aula = a; }
   void setProfessor( Professor * p ) { professor = p; }
   void setTurma (int aTurma) { turma = aTurma; }
   void setDia ( int aDia ) { dia = aDia; }
   void setDisciplina ( Disciplina * aD ) { disciplina = aD; }
   void setDisciplinaD ( Disciplina * aD ) { disciplinaD = aD; }
   void setDisciplinaD1 ( Disciplina * aD ) { disciplinaD1 = aD; }
   void setSala ( Sala * aS ) { sala = aS; }
   void setType( VariableOpType t ) { type = t; }
   void setH1 ( HorarioAula * aH1 ) { h1 = aH1; }
   void setH2 ( HorarioAula * aH2 ) { h2 = aH2; }

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   VariableOp & operator = ( const VariableOp & );
   // Less 
   bool operator < ( const VariableOp & ) const;
   // Equals 
   bool operator == ( const VariableOp & ) const;

   // Variable name
   std::string toString();

private:
   VariableOpType type;
   double value;
   Curso * curso;
   HorarioDia * h; // horario
   Aula * aula;
   Sala * sala;
   Professor * professor;
   Disciplina * disciplina;
   HorarioAula * horarioAula;
   int turma;
   int dia;

   // Disciplinas utilizadas no modelo operacional,
   // no critério de última aula do dia D e primeira aula do dia D+1
   Disciplina * disciplinaD;
   Disciplina * disciplinaD1;

   // Horários de aula utilizados no modelo operacional,
   // no critério de minimização de gaps nos horários dos professores
   HorarioAula * h1;
   HorarioAula * h2;
};

/**
//* Type definition for the hash object.
*/
typedef std::map< VariableOp, int > VariableOpHash;

#endif

