#ifndef VARIABLE_H
#define VARIABLE_H

#include <hash_map>
#include "input.h"

//Variables
class Variable 
{
public:
   // All variable types
   enum VariableType
   {
      V_ERROR = 0,
      V_CREDITOS = 1, /** x_{idust} */
      V_OFERECIMENTO = 2, /** o_{idust} */
      V_ABERTURA = 3, /** z_{idu} */
      V_ALUNOS = 4, /** a_{idu} */
      V_TURMA_BLOCO = 5, /** w_{btu} */
      V_DIAS_CONSECUTIVOS = 6, /** c_{idt} */
      V_MIN_CRED_SEMANA = 7, /** h_{bi} */
      V_MAX_CRED_SEMANA = 8, /** H_{bi} */
   };

   //Constructors
   Variable();
   Variable(const Variable& orig);

   //Destructor
   virtual ~Variable();

   //==================================================
   // GET METHODS 
   //==================================================
   //Return variable type
   VariableType getType() const { return type; }

   // Return value
   double getValue() const { return value; }

   /*
   ToDo:
   All get methods of the private attributes should be defined here
   */
   Turma* getTurma() const { return i; }
   Disciplina* getDisciplina() const { return d; }
   Unidade* getUnidade() const { return u; }
   Sala* getSala() const { return s; }
   int getDia() const { return t; }
   BlocoCurricular* getBloco() const { return b; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set variable type
   void setType(VariableType t)                 { type = t; }

   // Set value
   void setValue(double v)                      { value = v; }

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



   //==================================================
   // OPERATORS 
   //==================================================
   //Assignment 
   Variable& operator=(const Variable& var);
   //Less 
   bool operator<(const Variable& var) const;
   //Equals 
   bool operator==(const Variable& var) const;

   //Variable name
   std::string toString();

private:
   VariableType type;
   double value;

   /* ToDo:
   All attributes that define a variable should be declared here
   */
   Turma* i;
   Disciplina* d;
   Unidade* u;
   Sala* s;
   int t /* dia */;
   BlocoCurricular* b; 
};


class VariableHasher : public stdext::hash_compare<Variable>
{
public:
   //Less operator
   bool operator()(const Variable& v1, const Variable& v2) const;

   //Hash value
   size_t operator()(const Variable& v) const;
};

/**
* Type definition for the hash object.
*/
typedef stdext::hash_map<Variable, int, VariableHasher> VariableHash;

#endif 
