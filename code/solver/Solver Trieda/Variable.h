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
		V_CREDITOS = 1,            /** x_{idust} */
		V_OFERECIMENTO = 2,        /** o_{idust} */
		V_ABERTURA = 3,            /** z_{idu} */
		V_ALUNOS = 4,              /** a_{iduc} */
		V_ALOC_ALUNO = 5,          /** b_{icdu} */
		V_N_SUBBLOCOS = 6,         /** w_{bjtu} */
		V_DIAS_CONSECUTIVOS = 7,   /** c_{idt} */
		V_MIN_CRED_SEMANA = 8,     /** h_{bi} */
		V_MAX_CRED_SEMANA = 9,      /** H_{bi} */
		V_ALOC_DISCIPLINA = 10,    /** y_{idsu} */
        V_N_ABERT_TURMA_BLOCO = 11,  /** v_{bt} */
        V_SLACK_ = 12
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

   // Turma* getTurma() const { return i; }
   int getTurma() const { return i; }

   Disciplina* getDisciplina() const { return d; }
   Unidade* getUnidade() const { return u; }
   Sala* getSala() const { return s; }
   int getDia() const { return t; }
   BlocoCurricular* getBloco() const { return b; }

   int getSubBloco() const { return j; }
   Curso* getCurso() const { return c; }

   Campus* getCampus() const { return campus; }

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
//   void setTurma(Turma* ii) {  i = ii; }
   void setTurma(int ii) { i = ii; }

   void setDisciplina(Disciplina* dd) {  d = dd; }
   void setUnidade(Unidade* uu) {  u = uu; }
   void setSala(Sala* ss) {  s = ss; }
   void setDia(int tt) {  t = tt; }
   void setBloco(BlocoCurricular* bb) {  b = bb; } 

   void setSubBloco(int jj) { j = jj; }
   void setCurso(Curso* cc) { cc = c; }

	void setCampus(Campus* c) { campus = c; }


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

   //   Turma* i;
   int i; // Turma

   Disciplina* d;
   Unidade* u;
   Sala* s;
   int t /* dia */;
   BlocoCurricular* b;
   int j; // subbloco
   Curso* c;

   Campus *campus;
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
