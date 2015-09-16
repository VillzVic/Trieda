#ifndef MIP_ALOCAR_PROF_CONS_H
#define MIP_ALOCAR_PROF_CONS_H

#include <map>

#include "ProfessorHeur.h"


// Constraints
class MIPAlocarProfCons
{
public:

   // All constraint types
   enum MIPAlocarProfConsType
   {
      C_ERROR = 0,
      C_TURMA_PROF_ASSIGN = 1,
      C_PROF_USADO = 2,
      C_PROF_DIA = 3,
      C_PROF_MIN_CH = 4,
      C_PROF_CH_ANT = 5,
      C_PROF_HOR_INIC_LB = 6,
	  C_PROF_HOR_INIC_UB = 7,
      C_PROF_HOR_FIN_LB = 8,
	  C_PROF_HOR_FIN_UB = 9,
	  C_PROF_GAP = 10,
	  C_TURMAS_INCOMP = 11,
      C_PROF_MAX_DIAS = 12,
      C_PROF_MAX_CH = 13,
      C_PROF_MIN_CH_DIA = 14,
	  C_PROF_VIRT_SEQ = 15,
	  C_PROF_MIN_CONTRATO = 16,
	  C_PROF_MIN_TITULO = 17
   };

   // Constructors
   MIPAlocarProfCons();
   MIPAlocarProfCons( const MIPAlocarProfCons & );

   // Destructor
   virtual ~MIPAlocarProfCons();

   //==================================================
   // GET METHODS 
   //==================================================
   
   double getValue() const { return this->value_; }
   ProfessorHeur * getProfessor() const { return this->professor_; }
   int getDia() const { return this->dia_; }
   int getFaseDoDia () const { return this->fase_; }
   TurmaHeur* getTurma () const { return this->turma_; }
   pair<TurmaHeur*,TurmaHeur*> getParTurmas () const { return this->parTurmas_; }
   MIPAlocarProfConsType getType() const { return this->type_; }
   Curso* getCurso () const { return this->curso_; }
   int getTitulacaoId () const { return titulacaoId_;}
   int getContratoId () const { return contratoId_;}
   DateTime getDateTime () const { return dateTime_;}

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset constraints values
   void reset();

   // Set value
   void setValue( double v ) { this->value_ = v; }
   void setProfessor( ProfessorHeur * const p ) { this->professor_ = p; }
   void setDia ( int dia ) { this->dia_ = dia; }
   void setFaseDoDia ( int fase ) { this->fase_ = fase; }
   void setTurma ( TurmaHeur* const turma ) { this->turma_ = turma; }
   void setParTurmas ( TurmaHeur* const turma1, TurmaHeur* const turma2 ) { this->parTurmas_ = make_pair(turma1,turma2); }
   void setType( MIPAlocarProfConsType t ) { this->type_ = t; }
   void setCurso( Curso* const curso) { curso_ = curso; }
   void setPerfilVirt ( PerfilVirtual* const perfil ) { titulacaoId_ = perfil->titulacao; contratoId_ = perfil->contrato; curso_ = perfil->curso;  }
   void setDateTime( DateTime dt ){ this->dateTime_ = dt; }

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   MIPAlocarProfCons & operator = ( const MIPAlocarProfCons & );
   // Less 
   bool operator < ( const MIPAlocarProfCons & ) const;
   // Equals 
   bool operator == ( const MIPAlocarProfCons & ) const;

   // Constraint name
   string toString();

private:

   MIPAlocarProfConsType type_;
   double value_;
   ProfessorHeur * professor_;
   int dia_;
   int fase_;
   TurmaHeur* turma_;
   pair<TurmaHeur*,TurmaHeur*> parTurmas_;
   Curso* curso_;
   int titulacaoId_;
   int contratoId_;
   DateTime dateTime_;

};

/**
//* Type definition for the hash object.
*/
typedef std::map< MIPAlocarProfCons, int > ConstraintProfHash;

#endif

