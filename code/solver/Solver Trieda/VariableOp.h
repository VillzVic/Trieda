#ifndef VARIABLEOP_H
#define VARIABLEOP_H

#include <hash_map>
#include "input.h"
#include "Aula.h"
#include "Professor.h"

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
      V_F_FIX_PROF_DISC_SALA_DIA_HOR = 3,
      V_F_FIX_PROF_DISC_DIA_HOR = 4,
      V_F_FIX_PROF_DISC = 5,
      V_F_FIX_PROF_DISC_SALA = 6,
      V_F_FIX_PROF_SALA = 7
   };

   //Constructors
   VariableOp();
   VariableOp(const VariableOp& orig);

   //Destructor
   virtual ~VariableOp();

   //==================================================
   // GET METHODS 
   //==================================================
   //Return variable type
   VariableOpType getType() const { return type; }

   // Return value
   double getValue() const { return value; }

   HorarioDia* getHorario() const { return h; }

   Aula* getAula() const { return aula; }

   Professor* getProfessor() const { return professor; }

   Disciplina* getDisciplina() const { return disciplina; }

   Sala* getSala() const { return sala; }

   int getTurma() const { return turma; }


   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set value
   void setValue( double v ) { value = v; }

   void setHorario( HorarioDia* hh ) {  h = hh; }

   void setAula( Aula *a ) {  aula = a; }

   void setProfessor(Professor *p) { professor = p; }

   void setTurma (int aTurma) { turma = aTurma; }

   void setDisciplina (Disciplina *aD) { disciplina = aD; }

   void setSala (Sala *aS) { sala = aS; }

   void setType(VariableOpType t) { type = t; }

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
   HorarioDia* h; // horario
   Aula *aula;
   Sala *sala;
   Professor *professor;
   Disciplina *disciplina;
   int turma;
};


class VariableOpHasher : public stdext::hash_compare<VariableOp>
{
public:
   // Less operator
   bool operator()( const VariableOp &, const VariableOp & ) const;

   // Hash value
   size_t operator()( const VariableOp & ) const;
};

/**
* Type definition for the hash object.
*/
typedef stdext::hash_map< VariableOp, int, VariableOpHasher > VariableOpHash;

#endif 
