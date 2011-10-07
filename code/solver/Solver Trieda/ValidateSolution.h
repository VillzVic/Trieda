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

      if ( this->getTurma() < right.getTurma() ) return true;
      if ( right.getTurma() < this->getTurma() ) return false;

      if ( E_MENOR( this->getDisciplina(), right.getDisciplina() ) ) return true;
      if ( E_MENOR( right.getDisciplina(), this->getDisciplina() ) ) return false;

      if ( this->getQuantidade() < right.getQuantidade() ) return true;
      if ( right.getQuantidade() < this->getQuantidade() ) return false;

      return false;
   }

   Professor * getProfessor() const { return this->professor; }
   Campus * getCampus() const { return this->campus; }
   Unidade * getUnidade() const { return this->unidade; }
   Sala * getSala() const { return this->sala; }
   Turno * getTurno() const {  return this->turno; }
   int getDiaSemana() const { return this->dia_semana; }
   HorarioAula * getHorarioAula() const { return this->horario_aula; }
   int getTurma() const { return this->turma; }
   Disciplina * getDisciplina() const { return this->disciplina; }
   int getQuantidade() const { return this->quantidade; }

   int getIdAtCampus() const { return this->idAtCampus; }
   int getIdAtUnidade() const { return this->idAtUnidade; }
   int getIdAtSala() const { return this->idAtSala; }
   int getIdAtDiaSemana() const { return this->idAtDiaSemana; }
   int getIdAtTurno() const { return this->idAtTurno; }
   int getIdAtHorario() const { return this->idAtHorario; }
   int getIdAtOferta() const { return this->idAtOferta; }
   bool isSwap() const { return this->swap; }

   void setProfessor( Professor * p ) { this->professor = p; }
   void setCampus( Campus * cpp ) { this->campus = cpp; }
   void setUnidade( Unidade * uu ) {  this->unidade = uu; }
   void setSala( Sala * ss ) {  this->sala = ss; }
   void setTurno( Turno * t ) {  this->turno = t; }
   void setDiaSemana( int dia ) { this->dia_semana = dia; }
   void setHorarioAula( HorarioAula * h ) { this->horario_aula = h; }
   void setTurma( int t ) { this->turma = t; }
   void setDisciplina( Disciplina * d ) { this->disciplina = d; }
   void setQuantidade( int q ) { this->quantidade = q; }

   void setIdAtCampus( int value ) { this->idAtCampus = value; }
   void setIdAtUnidade( int value ) { this->idAtUnidade = value; }
   void setIdAtSala( int value ) { this->idAtSala = value; }
   void setIdAtDiaSemana( int value ) { this->idAtDiaSemana = value; }
   void setIdAtTurno( int value ) { this->idAtTurno = value; }
   void setIdAtHorario( int value ) { this->idAtHorario = value; }
   void setIdAtOferta( int value ) { this->idAtOferta = value; }
   void setSwap( bool value ) { this->swap = value; }

private:
   Professor * professor;
   Campus * campus;
   Unidade * unidade;
   Sala * sala;
   Turno * turno;
   int dia_semana;
   HorarioAula * horario_aula;
   int turma;
   Disciplina * disciplina;
   int quantidade;

   int idAtCampus;
   int idAtUnidade;
   int idAtSala;
   int idAtDiaSemana;
   int idAtTurno;
   int idAtHorario;
   int idAtOferta;
   bool swap;

   void reset()
   {
      this->professor = NULL;
      this->campus = NULL;
      this->unidade = NULL;
      this->sala = NULL;
      this->turno = NULL;
      this->dia_semana = -1;
      this->horario_aula = NULL;
      this->turma = -1;
      this->disciplina = NULL;
      this->quantidade = -1;

      this->idAtCampus = -1;
      this->idAtUnidade = -1;
      this->idAtSala = -1;
      this->idAtDiaSemana = -1;
      this->idAtTurno = -1;
      this->idAtHorario = -1;
      this->idAtOferta = -1;
      this->swap = false;
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

   bool checkRestricaoAlocacaoAulas( void );
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