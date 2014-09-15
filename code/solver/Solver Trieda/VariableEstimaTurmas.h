#ifndef VARIABLE_ESTIMA_TURMAS_H
#define VARIABLE_ESTIMA_TURMAS_H

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

// Variables
class VariableEstimaTurmas 
{
public:
   // All variable types
   enum VariableType
   {
      V_EST_ERROR = 0,
      V_EST_NUM_TURMAS = 1,							// x_{d,s,g}
	  V_EST_LIM_SUP_CREDS_SALA = 2,					// Hs_{cp}
	  V_EST_SLACK_DISC_SALA = 3,					// fs_{d,s}
	  V_EST_SLACK_DEMANDA = 4,						// fd_{d,sl}
	  V_EST_NUM_ATEND = 5,							// a_{d,g,sl}
	  V_EST_SLACK_DEMANDA_ALUNO = 6,				// fd_{d,a}
	  V_EST_ALOCA_ALUNO_DISC = 7,					// s_{a,d,g}
	  V_EST_CRED_SALA_F1 = 8,						// xcs1_{s}
	  V_EST_CRED_SALA_F2 = 9,						// xcs2_{s}
	  V_EST_CRED_SALA_F3 = 10,						// xcs3_{s}
      V_EST_CRED_SALA_F4 = 11,						// xcs4_{s}
	  V_EST_ALUNO_SALA = 12,						// as_{a,s}
	  V_EST_USA_DISCIPLINA = 13						// k_{d}
   };

   //Constructors
   VariableEstimaTurmas();
   VariableEstimaTurmas( const VariableEstimaTurmas & );

   //Destructor
   virtual ~VariableEstimaTurmas();

   //==================================================
   // GET METHODS 
   //==================================================
   //Return variable type
   VariableType getType() const { return type; }

   // Return value
   double getValue() const { return value; }

   Campus * getCampus() const { return cp; }

   Unidade * getUnidade() const { return u; }

   Sala * getSala() const { return s; }
   
   ConjuntoSala * getSubCjtSala() const { return tps; }
   
   Disciplina * getDisciplina() const { return d; }

   Calendario * getSemanaLetiva() const {  return sl; }
   
   int getTurno() const { return turno; }

   Aluno* getAluno() const { return aluno; }
   
   AlunoDemanda* getAlunoDemanda() const { return alunoDemanda; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set variable type
   void setType( VariableType t ) { type = t; }

   // Set value
   void setValue( double v ) { value = v; }

   void setCampus( Campus * cpp ) { cp = cpp; }

   void setUnidade( Unidade * uu ) {  u = uu; }

   void setSala( Sala * ss ) {  s = ss; }

   void setSubCjtSala( ConjuntoSala * tpss )  { tps = tpss; }
   
   void setDisciplina( Disciplina * dd ) {  d = dd; }
   
   void setSemanaLetiva( Calendario * c ) {  sl = c; }
   
   void setTurno( int t ) {  turno = t; }

   void setAluno( Aluno *a ) { aluno = a; }
   
   void setAlunoDemanda( AlunoDemanda *a ) { alunoDemanda = a; }

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   VariableEstimaTurmas & operator = ( const VariableEstimaTurmas & );
   // Less 
   bool operator < ( const VariableEstimaTurmas & ) const;
   // Equals 
   bool operator == ( const VariableEstimaTurmas & ) const;

   // Variable name
   std::string toString();

private:
   VariableType type;
   double value;
   Campus * cp;
   Unidade * u;
   Sala * s;
   ConjuntoSala * tps;
   Disciplina * d;
   Calendario * sl;
   int turno;
   Aluno* aluno;
   AlunoDemanda* alunoDemanda;
};


/**
//* Type definition for the hash object.
*/
typedef std::map< VariableEstimaTurmas, int > VariableEstimaTurmasHash;


#endif 
