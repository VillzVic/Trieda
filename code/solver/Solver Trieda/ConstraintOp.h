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
      C_ALOC_PROF_CURSO_LESS = 16,
      C_ALOC_PROF_CURSO_GREATER = 17,
      C_CARGA_HOR_MIN_PROF = 18,
      C_CARGA_HOR_MIN_PROF_SEMANA = 19,
      C_CARGA_HOR_MAX_PROF_SEMANA = 20,
      C_DIAS_PROF_MINISTRA_AULA = 21,
      C_CUSTO_CORPO_DOCENTE = 22,
      C_MAX_DISC_PROF_CURSO = 23,
      C_AVALIACAO_CORPO_DOCENTE = 24,
      C_PREF_DISCIPLINAS = 25
   };

   /** Default constructor. */
   ConstraintOp();

   /** Copy constructor. */
   ConstraintOp( const ConstraintOp & );

   /** Destructor. */
   virtual ~ConstraintOp();

   /** Assign operator. */
   ConstraintOp& operator = ( const ConstraintOp & cons );

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
};

/**
//* Type definition for the hash object.
*/
typedef std::map< ConstraintOp, int > ConstraintOpHash;

/**
* Defines the operations needed by the hash object.
*/
// class ConstraintOpHasher : public stdext::hash_compare< ConstraintOp >
// {
// public:
//    /**
//    * Applies the hash function on a Constraint object.
//    * @param cons The constraint.
//    * @return The hash value for the constraint.
//    */
//    size_t operator() ( const ConstraintOp & ) const;
// 
//    /**
//    * Defines an order rule for two Constraint objects.
//    * @param cons1 The first constraint to be compared.
//    * @param cons2 The second constraint to be compared.
//    * @return True if const1 comes before cons2, false otherwise.
//    */
//    bool operator() ( const ConstraintOp &, const ConstraintOp & ) const;
// };
// 
// /**
// * Type definition for the hash object.
// */
// typedef stdext::hash_map< ConstraintOp, int, ConstraintOpHasher > ConstraintOpHash;

#endif
