#ifndef MIP_ALOCAR_ALUNO_VAR_H
#define MIP_ALOCAR_ALUNO_VAR_H

#include "AlunoHeur.h"


// Variables
class MIPAlocarAlunoVar
{
public:

   // All variable types
   enum MIPAlocarAlunoVarType
   {
      V_ERROR = 0,
      V_X_ALUNO_DISC = 1,
	  V_W_ASSOC_TURMA_PT = 2,
	  V_Y_ALUNO_TURMA = 3,
	  V_AT_ABRIR_TURMA = 4,
	  V_R_SALA_TURMA = 5
   };

   // Constructors
   MIPAlocarAlunoVar();
   MIPAlocarAlunoVar( const MIPAlocarAlunoVar & );

   // Destructor
   virtual ~MIPAlocarAlunoVar();

   //==================================================
   // GET METHODS 
   //==================================================
   
   double getValue() const { return this->value_; }
   AlunoHeur * getAluno() const { return this->aluno_; }
   int getDia() const { return this->dia_; }
   TurmaHeur* getTurma () const { return this->turma_; }
   TurmaHeur* getTurmaP () const { return this->turmap_; }
   TurmaHeur* getTurmaT () const { return this->turmat_; }
   SalaHeur* getSala () const { return this->sala_; }
   OfertaDisciplina* getOfertaDisciplina() const { return this->oferta_; }
   MIPAlocarAlunoVarType getType() const { return this->type_; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set value
   void setValue( double v ) { this->value_ = v; }
   void setAluno( AlunoHeur * const p ) { this->aluno_ = p; }
   void setDia ( int dia ) { this->dia_ = dia; }
   void setTurma ( TurmaHeur* const turma ) { this->turma_ = turma; }
   void setTurmaP ( TurmaHeur* const turmap ) { this->turmap_ = turmap; }
   void setTurmaT ( TurmaHeur* const turmat ) { this->turmat_ = turmat; }
   void setSala ( SalaHeur* const sala ) { this->sala_ = sala; }
   void setOfDisciplina ( OfertaDisciplina* const oferta ) { this->oferta_ = oferta; }
   void setType( MIPAlocarAlunoVarType t ) { this->type_ = t; }

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   MIPAlocarAlunoVar & operator = ( const MIPAlocarAlunoVar & );
   // Less 
   bool operator < ( const MIPAlocarAlunoVar & ) const;
   // Equals 
   bool operator == ( const MIPAlocarAlunoVar & ) const;

   // Variable name
   std::string toString();
   static std::string toString(TurmaHeur* turma);

private:

   MIPAlocarAlunoVarType type_;
   double value_;
   int dia_;
   OfertaDisciplina *oferta_;
   TurmaHeur* turma_;
   AlunoHeur* aluno_;
   SalaHeur* sala_;

   TurmaHeur* turmap_;
   TurmaHeur* turmat_;
};


#endif

