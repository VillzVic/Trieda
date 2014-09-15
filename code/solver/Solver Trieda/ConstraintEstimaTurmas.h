#ifndef _CONSTRAINT_ESTIMA_TURMAS_H_
#define _CONSTRAINT_ESTIMA_TURMAS_H_

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

/**
* Class which defines a contraint in the LP.
*/
class ConstraintEstimaTurmas
{
public:
   /** All constraint types */
   enum ConstraintType
   {
      C_ERROR = 0,
      C_EST_TEMPO_MAX_SALA					= 1,			// Restricao 1.2
      C_EST_ATEND_DEMANDA					= 2,			// Restricao 1.1
	  C_EST_CAPACIDADE_SALAS				= 3,			// Restricao 1.2
	  C_EST_DISTRIBUI_ENTRE_SALAS			= 4,			// Restricao 1.2
	  C_EST_TURMA_DIF_MESMA_DISC_SALA_DIF	= 5,			// Restricao 1.2
	  C_EST_DISC_PRATICA_TEORICA			= 6,			// Restricao 1.2
      C_EST_TEMPO_MAX_SALA_SAB				= 7,			// Restricao 1.3
	  C_EST_DEMANDA_EQUIV_ALUNO				= 8,
	  C_EST_SOMA_CRED_SALA					= 9,
	  C_EST_ALUNO_SALA						= 10,
	  C_EST_NRO_TURMAS_DISC_PRAT_TEOR_IGUAL = 11,
	  C_EST_TEMPO_MAX_ALUNO_SAB				= 12,
	  C_EST_USA_DISC						= 13
   };


   /** Default constructor. */
   ConstraintEstimaTurmas();

   /** Copy constructor. */
   ConstraintEstimaTurmas( const ConstraintEstimaTurmas & );

   /** Destructor. */
   virtual ~ConstraintEstimaTurmas();

   /** Assign operator. */
   ConstraintEstimaTurmas & operator = ( const ConstraintEstimaTurmas & );

   /** Less operator. */
   bool operator < ( const ConstraintEstimaTurmas & ) const;

   /** Equals operator. */
   bool operator == ( const ConstraintEstimaTurmas & ) const;

   /**
   * Returns a string containing an unique name for the constraint.
   * @return a string containing an unique name for the constraint.
   */
   std::string toString();

   //==================================================
   // GET METHODS 
   //==================================================
   // Return constraint type
   ConstraintType getType() const { return type; }

   Campus * getCampus() const { return cp; }
   Unidade * getUnidade() const { return u; }
   Sala * getSala() const { return s; }
   ConjuntoSala * getSubCjtSala() const { return tps; }
   Disciplina* getDisciplina() const { return d; }
   Calendario * getSemanaLetiva() const {  return sl; }
   int getTurno() const { return turno; }
   Aluno* getAluno() const { return aluno; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set constraint type
   void setType( ConstraintType t ) { type = t; }

   void setCampus( Campus * cpp ) { cp = cpp; }
   void setUnidade( Unidade * uu ) {  u = uu; }
   void setSala( Sala * ss ) {  s = ss; }
   void setSubCjtSala( ConjuntoSala * tpss ) { tps = tpss; } 
   void setDisciplina( Disciplina * dd ) {  d = dd; }
   void setSemanaLetiva( Calendario * c ) {  sl = c; }
   void setTurno( int t ) {  turno = t; }
   void setAluno( Aluno *a ) { aluno = a; }

private:

   /** Attribute which defines the constraint type of the instance. */
   ConstraintType type;

   Campus *cp;
   Unidade* u;
   Sala* s;
   ConjuntoSala* tps;
   Disciplina* d;
   Calendario * sl;
   int turno;
   Aluno* aluno;

};

/**
//* Type definition for the hash object.
*/
typedef std::map< ConstraintEstimaTurmas, int > ConstraintEstimaTurmasHash;


#endif
