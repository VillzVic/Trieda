#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <hash_map>
#include "input.h"

/**
* Class which defines a contraint in the LP.
*/
class Constraint
{
public:
   /** All constraint types */
   enum ConstraintType
   {
      C_ERROR = 0,
      C_CARGA_HORARIA = 1,
      C_MAX_CREDITOS_SD = 2,
      C_MIN_CREDITOS = 3,
      C_VAR_O = 4,
      C_EVITA_SOBREPOSICAO = 5,
      C_MESMA_UNIDADE = 6,
      C_TURMAS_BLOCO = 7,
      C_MAX_CREDITOS_BLOCO_SD = 8,
      C_MAX_CREDITOS = 9,
      C_CAP_DEMANDA = 10,
      C_CAP_SALA = 11,
      C_CAP_SALA_U = 12,
      C_DIAS_CONSECUTIVOS = 13,
      C_MIN_CREDS_BLOCO = 14,
      C_MAX_CREDS_BLOCO = 15,
   };

   /** Default constructor. */
   Constraint();

   /** Copy constructor. */
   Constraint(const Constraint& cons);

   /** Destructor. */
   ~Constraint();

   /** Assign operator. */
   Constraint& operator= (const Constraint& cons);

   /** Less operator. */
   bool operator< (const Constraint& cons) const;

   /** Equals operator. */
   bool operator== (const Constraint& cons) const;

   /**
   * Returns a string containing an unique name for the constraint.
   * @return a string containing an unique name for the constraint.
   */
   std::string toString();

   //==================================================
   // GET METHODS 
   //==================================================
   //Return constraint type
   ConstraintType getType() const            { return type; }

   Turma* getTurma() const { return i; }
   Disciplina* getDisciplina() const { return d; }
   Unidade* getUnidade() const { return u; }
   Sala* getSala() const { return s; }
   int getDia() const { return t; }
   BlocoCurricular* getBloco() const { return b; }

   /*
   ToDo:
   All get methods of the private attributes should be defined here
   */

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();
   // Set constraint type
   void setType(ConstraintType t)               { type = t; }

   /*
   ToDo:
   All set methods of the private attributes should be defined here
   */
   void setTurma(Turma* ii) {  i = ii; }
   void setDisciplina(Disciplina* dd) {  d = dd; }
   void setUnidade(Unidade* uu) {  u = uu; }
   void setSala(Sala* ss) {  s = ss; }
   void setDia(int tt) {  t = tt; }
   void setBloco(BlocoCurricular* bb) {  b = bb; } 


private:

   /** Attribute which defines the constraint type of the instance. */
   ConstraintType type;

   /**
   ToDo:
   All atributes which define a constraint should be declared here
   **/
   Unidade* u;
   Disciplina* d;
   Turma* i;
   Sala* s;
   int t /* dia */;
   BlocoCurricular* b; 

};

/**
* Defines the operations needed by the hash object.
*/
class ConstraintHasher : public stdext::hash_compare<Constraint>
{
public:
   /**
   * Applies the hash function on a Constraint object.
   * @param cons The constraint.
   * @return The hash value for the constraint.
   */
   size_t operator() (const Constraint& cons) const;

   /**
   * Defines an order rule for two Constraint objects.
   * @param cons1 The first constraint to be compared.
   * @param cons2 The second constraint to be compared.
   * @return True if const1 comes before cons2, false otherwise.
   */
   bool operator() (const Constraint& cons1, const Constraint& cons2) const;
};

/**
* Type definition for the hash object.
*/
typedef stdext::hash_map<Constraint, int, ConstraintHasher> ConstraintHash;

#endif