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
      V_CREDITOS = 1,							// x_{idust}
      V_OFERECIMENTO = 2,						// o_{idust}
      V_ABERTURA = 3,							// z_{i,d,cp}
      V_ALUNOS = 4,							   // a_{i,d,o}
      V_ALOC_ALUNO = 5,				         // b_{i,d,c,cp}
      V_N_SUBBLOCOS = 6,				        // w_{bjtu} -> w_{b,t,cp}
      V_DIAS_CONSECUTIVOS = 7,				// c_{idt}
      V_MIN_CRED_SEMANA = 8,					// h_{bi}
      V_MAX_CRED_SEMANA = 9,					// H_{bi}
      V_ALOC_DISCIPLINA = 10,					// y_{i,d,tps,u}
      V_N_ABERT_TURMA_BLOCO = 11,				// v_{bt}
      V_SLACK_DIST_CRED_DIA_SUPERIOR = 12,	// fcp_{dt}
      V_SLACK_DIST_CRED_DIA_INFERIOR = 13,		// fcm_{dt}
      V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS = 14,		   // r_{b,t,cp}

      V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT = 15,   // bs_{i,d,c,cp}
      V_SLACK_DEMANDA = 16, // fd_{d,o}
	  V_COMBINACAO_DIVISAO_CREDITO = 17, // m{i,d,k}
	  V_SLACK_COMBINACAO_DIVISAO_CREDITO = 18 // fk{i,d,k}

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

   Campus* getCampus() const { return cp; }
   Unidade* getUnidade() const { return u; }
   Sala* getSala() const { return s; }

   ConjuntoSala* getSubCjtSala() const { return tps; }

   int getTurma() const { return i; }
   
   Curso* getCurso() const { return c; }

   Curso* getCursoIncompat() const { return c_incompat; }

   BlocoCurricular* getBloco() const { return b; }
   Disciplina* getDisciplina() const { return d; }

   int getSubBloco() const { return j; }

   int getDia() const { return t; }

   Oferta * getOferta() const { return o; }

   int getK() const { return k; }

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

   void setCampus(Campus* cpp) { cp = cpp; }
   void setUnidade(Unidade* uu) {  u = uu; }
   void setSala(Sala* ss) {  s = ss; }

   void setSubCjtSala(ConjuntoSala * tpss)  { tps = tpss; }

   void setTurma(int ii) { i = ii; }
   void setCurso(Curso* cc) { c = cc; }

   void setCursoIncompat(Curso* cc) { c_incompat = cc; }

   void setBloco(BlocoCurricular* bb) {  b = bb; } 
   void setDisciplina(Disciplina* dd) {  d = dd; }

   void setSubBloco(int jj) { j = jj; }   

   void setDia(int tt) {  t = tt; }

   void setOferta(Oferta * oferta) { o = oferta; }

   void setK(int kk) { k = kk; }

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

   Campus *cp;
   Unidade* u;
   Sala* s;

   ConjuntoSala* tps;

   int i; // Turma
   Curso* c;

   Curso* c_incompat;

   BlocoCurricular* b;
   Disciplina* d;

   int j; // subbloco

   int t; // dia

   Oferta * o; // oferta

   int k; // combinação de divisão de créditos

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
