#ifndef _VALIDATE_SOLUTION_H_
#define _VALIDATE_SOLUTION_H_

#include <iostream>
#include <sstream>

#include "ProblemSolution.h"
#include "ProblemData.h"

#ifndef E_MENOR
#define E_MENOR( a, b ) \
   ( a == NULL && b != NULL ) || \
   ( b != NULL && a != NULL && ( *a < *b ) )
#endif

class AtendimentoBase
{
public:

   AtendimentoBase() { this->reset(); }
   virtual ~AtendimentoBase() { this->reset(); }

   virtual bool operator < ( const AtendimentoBase & right )
   { 
      if ( E_MENOR( this->getProfessor(), right.getProfessor() ) ) return true;
      if ( E_MENOR( right.getProfessor(), this->getProfessor() ) ) return false;

      if ( E_MENOR( this->getCampus(), right.getCampus() ) ) return true;
      if ( E_MENOR( right.getCampus(), this->getCampus() ) ) return false;

      if ( E_MENOR( this->getUnidade(), right.getUnidade() ) ) return true;
      if ( E_MENOR( right.getUnidade(), this->getUnidade() ) ) return false;

      if ( E_MENOR( this->getSala(), right.getSala() ) ) return true;
      if ( E_MENOR( right.getSala(), this->getSala() ) ) return false;

      if ( E_MENOR( this->getTurno(), right.getTurno() ) ) return true;
      if ( E_MENOR( right.getTurno(), this->getTurno() ) ) return false;

      if ( this->getDiaSemana() < right.getDiaSemana() ) return true;
      if ( right.getDiaSemana() < this->getDiaSemana() ) return false;

      if ( E_MENOR( this->getHorarioAula(), right.getHorarioAula() ) ) return true;
      if ( E_MENOR( right.getHorarioAula(), this->getHorarioAula() ) ) return false;

      return false;
   }

   Professor * getProfessor() const { return this->professor; }
   Campus * getCampus() const { return this->campus; }
   Unidade * getUnidade() const { return this->unidade; }
   Sala * getSala() const { return this->sala; }
   Turno * getTurno() const {  return this->turno; }
   int getDiaSemana() const { return this->dia_semana; }
   HorarioAula * getHorarioAula() const { return this->horario_aula; }

   void setProfessor( Professor * p ) { this->professor = p; }
   void setCampus( Campus * cpp ) { this->campus = cpp; }
   void setUnidade( Unidade * uu ) {  this->unidade = uu; }
   void setSala( Sala * ss ) {  this->sala = ss; }
   void setTurno( Turno * t ) {  this->turno = t; }
   void setDiaSemana( int dia ) { this->dia_semana = dia; }
   void setHorarioAula( HorarioAula * h ) { this->horario_aula = h; }

private:
   Professor * professor;
   Campus * campus;
   Unidade * unidade;
   Sala * sala;
   Turno * turno;
   int dia_semana;
   HorarioAula * horario_aula;

   void reset()
   {
      professor = NULL;
      campus = NULL;
      unidade = NULL;
      sala = NULL;
      turno = NULL;
      dia_semana = -1;
      horario_aula = NULL;
   }
};

class ValidateSolutionOp
{
public:

   ValidateSolutionOp( ProblemData * );
   virtual ~ValidateSolutionOp( void  );

   void setSolution( ProblemSolution * sol ) { this->solution = sol; }
   ProblemSolution * getSolution() const { return this->solution; }

   bool checkSolution( ProblemSolution * );

private:

   ProblemSolution * solution;
   ProblemData * pData;

   bool checkRestricaoProfessorHorario( void );
   bool checkRestricaoSalaHorario( void );
   bool checkRestricaoBlocoHorario( void );
   bool checkRestricaoFixProfDiscSalaDiaHorario( void );
   bool checkRestricaoFixProfDiscDiaHor( void );
   bool checkRestricaoDisciplinaMesmoHorario( void );
   bool checkRestricaoDeslocamentoViavel( void );
   bool checkRestricaoDeslocamentoProfessor( void );

   // Métodos utilitários
   bool deslocamentoViavel( AtendimentoBase *, AtendimentoBase * );
};

#endif