#ifndef _CONSTRAINT_OP_H_
#define _CONSTRAINT_OP_H_

#include <map>

#include "Aula.h"
#include "Professor.h"
#include "HorarioDia.h"
#include "ProblemData.h"

/**
* Class which defines a contraint in the LP.
*/
class ConstraintOp
{
public:
   /** All constraint types */
   enum ConstraintOpType
   {
      C_ERROR = 0,
      C_SALA_HORARIO = 1,
      C_PROFESSOR_HORARIO = 2,
      C_BLOCO_HORARIO = 3,
      C_ALOC_AULA = 4,
      C_PROF_DISC = 5,
      C_PROF_DISC_UNI = 6,
      C_FIX_PROF_DISC_SALA_DIA_HOR = 7,
      C_FIX_PROF_DISC_DIA_HOR = 8,
      C_FIX_PROF_DISC = 9,
      C_FIX_PROF_DISC_SALA = 10,
      C_FIX_PROF_SALA = 11,
      C_DISC_HORARIO = 12,
      C_DISC_HORARIO_UNICO = 13,
      C_MIN_MEST_CURSO = 14,
      C_MIN_DOUT_CURSO = 15,
      C_ALOC_PROF_CURSO = 16,
      C_CARGA_HOR_MIN_PROF = 17,
      C_CARGA_HOR_MIN_PROF_SEMANA = 18,
      C_CARGA_HOR_MAX_PROF_SEMANA = 19,
      C_DIAS_PROF_MINISTRA_AULA = 20,
      C_CUSTO_CORPO_DOCENTE = 21,
      C_MAX_DISC_PROF_CURSO = 22,
      C_AVALIACAO_CORPO_DOCENTE = 23,
      C_DESLOC_PROF = 25,
      C_DESLOC_VIAVEL = 26,
      C_ULTIMA_PRIMEIRA_AULA_PROF = 27,
      C_GAPS_PROFESSORES = 28
   };

   /** Default constructor. */
   ConstraintOp();

   /** Copy constructor. */
   ConstraintOp( const ConstraintOp & );

   /** Destructor. */
   virtual ~ConstraintOp();

   /** Assign operator. */
   ConstraintOp & operator = ( const ConstraintOp & );

   /** Less operator. */
   bool operator < ( const ConstraintOp & ) const;

   /** Equals operator. */
   bool operator == ( const ConstraintOp & ) const;

   /**
   * Returns a string containing an unique name for the constraint.
   * @return a string containing an unique name for the constraint.
   */
   std::string toString();

   //==================================================
   // GET METHODS 
   //==================================================
   // Return constraint type
   ConstraintOpType getType() const { return type; }

   Curso * getCurso() const { return this->curso; }
   Sala * getSala() const { return s; }
   BlocoCurricular * getBloco() const { return b; }
   Professor * getProfessor() const { return professor; } 
   Aula * getAula() const { return aula; } 
   int getSubBloco() const { return j; }
   int getDia() const { return t; }
   HorarioDia * getHorario() const { return h; }
   HorarioAula * getHorarioAula() const { return horarioAula; }
   Disciplina * getDisciplina() const { return disciplina; }
   int getTurma() const { return turma; }
   HorarioAula * getH1() const { return h1; }
   HorarioAula * getH2() const { return h2; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set constraint type
   void setType( ConstraintOpType t ) { type = t; }
   void setCurso( Curso * c ) { this->curso = c; }
   void setSala( Sala * ss ) {  s = ss; }
   void setBloco( BlocoCurricular * bb ) {  b = bb; } 
   void setAula( Aula * a ) {  aula = a; }
   void setProfessor( Professor * p ) { professor = p; }
   void setSubBloco( int jj ) { j = jj; }   
   void setDia( int tt ) {  t = tt; }
   void setHorario( HorarioDia * hh ) { h = hh; }
   void setHorarioAula( HorarioAula * hh ) { horarioAula = hh; }
   void setDisciplina ( Disciplina * aD ) { disciplina = aD; }
   void setTurma( int aT ) { turma = aT; }
   void setH1 ( HorarioAula * aH1 ) { h1 = aH1; }
   void setH2 ( HorarioAula * aH2 ) { h2 = aH2; }

private:
   /** Attribute which defines the constraint type of the instance. */
   ConstraintOpType type;

   Curso * curso;
   Sala * s;
   HorarioDia * h; // horario
   HorarioAula * horarioAula;
   Aula * aula;
   Professor * professor;
   BlocoCurricular * b;
   Disciplina * disciplina;
   int turma;
   int j; // subbloco
   int t; // dia

   // Hor�rios de aula utilizados no modelo operacional,
   // no crit�rio de minimiza��o de gaps nos hor�rios dos professores
   HorarioAula * h1;
   HorarioAula * h2;
};

/**
//* Type definition for the hash object.
*/
typedef std::map< ConstraintOp, int > ConstraintOpHash;

#endif
