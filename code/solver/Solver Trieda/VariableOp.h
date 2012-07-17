#ifndef VARIABLEOP_H
#define VARIABLEOP_H

#include <map>

#include "Aula.h"
#include "Professor.h"
#include "ProblemData.h"

// Variables
class VariableOp 
{
public:
   // All variable types
   enum VariableOpType
   {
      V_ERROR = 0,
      V_X_PROF_AULA_HOR = 1,
      V_Y_PROF_DISCIPLINA = 2,
      V_Z_DISCIPLINA_HOR = 3,
      V_F_FIX_PROF_DISC_SALA_DIA_HOR = 4,
      V_F_FIX_PROF_DISC_DIA_HOR = 5,
      V_F_FIX_PROF_DISC = 6,
      V_F_FIX_PROF_DISC_SALA = 7,
      V_F_FIX_PROF_SALA = 8,
      V_PROF_CURSO = 10,
      V_F_MIN_MEST_CURSO = 11,
      V_F_MIN_DOUT_CURSO = 12,
      V_F_CARGA_HOR_MIN_PROF = 13,
      V_F_CARGA_HOR_MIN_PROF_SEMANA = 14,
      V_F_CARGA_HOR_MAX_PROF_SEMANA = 15,
      V_DIAS_PROF_MINISTRA_AULAS = 16,
      V_CUSTO_CORPO_DOCENTE = 17,
      V_MAX_DISC_PROF_CURSO = 18,
      V_F_MAX_DISC_PROF_CURSO = 19,
      V_AVALIACAO_CORPO_DOCENTE = 20,
      V_F_ULTIMA_PRIMEIRA_AULA_PROF = 21,
      V_GAPS_PROFESSORES = 22,
	  V_FOLGA_DEMANDA = 23,
	  V_FOLGA_DISCIPLINA_HOR = 24,
	  V_HI_PROFESSORES = 25,
	  V_HF_PROFESSORES = 26,
	  V_NRO_PROFS_CURSO = 27,
	  V_NRO_PROFS_VIRTUAIS_CURSO = 28,
	  V_NRO_PROFS_VIRTUAIS_MEST_CURSO = 29,
	  V_NRO_PROFS_VIRTUAIS_DOUT_CURSO = 30
   };

   // Constructors
   VariableOp();
   VariableOp( const VariableOp & );

   // Destructor
   virtual ~VariableOp();

   //==================================================
   // GET METHODS 
   //==================================================
   // Return variable type
   VariableOpType getType() const { return this->type; }

   // Return value
   double getValue() const { return this->value; }
   Curso * getCurso() const { return this->curso; }
   HorarioDia * getHorario() const { return this->h; }
   HorarioAula * getHorarioAula() const { return this->horarioAula; }
   Aula * getAula() const { return this->aula; }
   Professor * getProfessor() const { return this->professor; }
   Disciplina * getDisciplina() const { return this->disciplina; }
   HorarioDia * getHorarioDiaD() const { return this->horarioDiaD; }
   HorarioDia * getHorarioDiaD1() const { return this->horarioDiaD1; }
   Sala * getSala() const { return this->sala; }
   int getTurma() const { return this->turma; }
   int getDia() const { return this->dia; }
   HorarioAula * getH1() const { return this->h1; }
   HorarioAula * getH2() const { return this->h2; }
   Unidade * getUnidade () const { return this->unidade; }
   Campus * getCampus () const { return this->campus; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set value
   void setValue( double v ) { this->value = v; }
   void setCurso( Curso * c ) { this->curso = c; }
   void setHorario( HorarioDia * hh ) {  this->h = hh; }
   void setHorarioAula( HorarioAula * hh ) {  this->horarioAula = hh; }
   void setAula( Aula * a ) {  this->aula = a; }
   void setProfessor( Professor * p ) { this->professor = p; }
   void setTurma ( int aTurma ) { this->turma = aTurma; }
   void setDia ( int aDia ) { this->dia = aDia; }
   void setDisciplina ( Disciplina * aD ) { this->disciplina = aD; }
   void setHorarioDiaD ( HorarioDia * hD ) { this->horarioDiaD = hD; }
   void setHorarioDiaD1 ( HorarioDia * hD ) { this->horarioDiaD1 = hD; }
   void setSala ( Sala * aS ) { this->sala = aS; }
   void setType( VariableOpType t ) { this->type = t; }
   void setH1 ( HorarioAula * aH1 ) { this->h1 = aH1; }
   void setH2 ( HorarioAula * aH2 ) { this->h2 = aH2; }
   void setUnidade ( Unidade *u ) { this->unidade = u; }
   void setCampus ( Campus *cp ) { this->campus = cp; }

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   VariableOp & operator = ( const VariableOp & );
   // Less 
   bool operator < ( const VariableOp & ) const;
   // Equals 
   bool operator == ( const VariableOp & ) const;

   // Variable name
   std::string toString();

private:
   VariableOpType type;
   double value;
   Curso * curso;
   HorarioDia * h; // horario
   Aula * aula;
   Sala * sala;
   Professor * professor;
   Disciplina * disciplina;
   HorarioAula * horarioAula;
   int turma;
   int dia;
   Unidade *unidade;
   Campus *campus;

   // Horarios-Dia utilizados no modelo operacional,
   // no critério de última aula do dia D e primeira aula do dia D+1
   HorarioDia * horarioDiaD;
   HorarioDia * horarioDiaD1;

   // Horários de aula utilizados no modelo operacional,
   // no critério de minimização de gaps nos horários dos professores
   HorarioAula * h1;
   HorarioAula * h2;
};

/**
//* Type definition for the hash object.
*/
typedef std::map< VariableOp, int > VariableOpHash;

#endif

