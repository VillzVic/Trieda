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
      C_CARGA_HORARIA = 1,				// Restricao 1.2.3
      C_MAX_CREDITOS_SD = 2,				// Restricao 1.2.4
      C_MIN_CREDITOS_DD = 3,				// Restricao 1.2.5
      C_VAR_O = 4,						// Restricao 1.2.6
      C_EVITA_SOBREPOSICAO_TD = 5,		// Restricao 1.2.7
      C_TURMA_DISCIPLINA_SALA = 6,		// Restricao 1.2.8
      C_TURMA_SALA = 7,					// Restricao 1.2.9
      C_EVITA_TURMA_DISC_CAMP_D = 8,		// Restricao 1.2.10
      C_TURMAS_BLOCO = 9,					// Restricao 1.2.11
      C_MAX_CRED_DISC_BLOCO = 10,			// Restricao 1.2.12
      C_NUM_TUR_BLOC_DIA_DIFUNID = 11,	// Restricao 1.2.13
      C_LIM_CRED_DIAR_DISC = 12,			// Restricao 1.2.14
      C_CAP_ALOC_DEM_DISC = 13,			// Restricao 1.2.15
      C_CAP_SALA_COMPATIVEL_TURMA = 14,	// Restricao 1.2.16
      C_CAP_SALA_UNIDADE = 15,			// Restricao 1.2.17
      C_TURMA_DISC_DIAS_CONSEC = 16,		// Restricao 1.2.18
      C_MIN_CREDS_TURM_BLOCO = 17,		// Restricao 1.2.19
      C_MAX_CREDS_TURM_BLOCO = 18,		// Restricao 1.2.20
      C_ALUNO_CURSO_DISC = 19,			// Restricao 1.2.21
      C_ALUNOS_CURSOS_DIF = 20,			// Restricao 1.2.22
      C_SLACK_DIST_CRED_DIA = 21,			// Restricao 1.2.23
      C_VAR_R = 22,			            // Restricao 1.2.24

      C_LIMITA_ABERTURA_TURMAS = 23,			            // Restricao NOVA
      C_ABRE_TURMAS_EM_SEQUENCIA = 24,			            // Restricao NOVA
	  C_DIVISAO_CREDITO = 25,			//Restricao 1.2.27
	  C_COMBINACAO_DIVISAO_CREDITO = 26	//Restricao 1.2.28

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

private:

   /** Attribute which defines the constraint type of the instance. */
   ConstraintType type;

   /**
   ToDo:
   All atributes which define a constraint should be declared here
   **/
   /*
   Unidade* u;
   Disciplina* d;
   int i;
   Sala* s;
   int t;
   BlocoCurricular* b; 
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