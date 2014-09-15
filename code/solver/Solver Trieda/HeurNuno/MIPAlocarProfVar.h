#ifndef MIP_ALOCAR_PROF_VAR_H
#define MIP_ALOCAR_PROF_VAR_H

#include <map>

#include "ProfessorHeur.h"


// Variables
class MIPAlocarProfVar
{
public:

   // All variable types
   enum MIPAlocarProfVarType
   {
      V_ERROR = 0,
      V_X_PROF_TURMA = 1,
      V_Y_PROF_USADO = 2,
      V_Z_PROF_DIA = 3,
      V_CH_PROF_MIN_CH = 4,
      V_CHA_PROF_CH_ANT = 5,
      V_HIP_PROF_HOR_INIC = 6,
      V_HFP_PROF_HOR_FIN = 7
   };

   // Constructors
   MIPAlocarProfVar();
   MIPAlocarProfVar( const MIPAlocarProfVar & );

   // Destructor
   virtual ~MIPAlocarProfVar();

   //==================================================
   // GET METHODS 
   //==================================================
   
   double getValue() const { return this->value_; }
   ProfessorHeur * getProfessor() const { return this->professor_; }
   int getDia() const { return this->dia_; }
   int getFase () const { return this->fase_; }
   TurmaHeur* getTurma () const { return this->turma_; }
   MIPAlocarProfVarType getType() const { return this->type_; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set value
   void setValue( double v ) { this->value_ = v; }
   void setProfessor( ProfessorHeur * const p ) { this->professor_ = p; }
   void setDia ( int dia ) { this->dia_ = dia; }
   void setFase ( int fase ) { this->fase_ = fase; }
   void setTurma ( TurmaHeur* const turma ) { this->turma_ = turma; }
   void setType( MIPAlocarProfVarType t ) { this->type_ = t; }

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   MIPAlocarProfVar & operator = ( const MIPAlocarProfVar & );
   // Less 
   bool operator < ( const MIPAlocarProfVar & ) const;
   // Equals 
   bool operator == ( const MIPAlocarProfVar & ) const;

   // Variable name
   std::string toString();

private:

   MIPAlocarProfVarType type_;
   double value_;
   ProfessorHeur * professor_;
   int dia_;
   int fase_;
   TurmaHeur* turma_;
};

/**
//* Type definition for the hash object.
*/
typedef std::map< MIPAlocarProfVar, int > VariableProfHash;

#endif

